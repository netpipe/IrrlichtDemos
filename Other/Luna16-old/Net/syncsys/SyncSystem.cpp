#ifdef SYNCSYS
#include <map>
#include <list>
#include <vector>
#include <string>
#include <algorithm>

#include "Portable.h"
#include "SyncSystem.h"

#if defined _WIN32 && defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace helpers;
using namespace syncsys;

void ContainerConnector::SetDataExt(NetActionFlag flag, NetworkBuffer &buf)
{
	NetActionFlag ret = 0;
	pointer->Critic.Lock();

	for(unsigned counter = 0; counter < NUMNETACTION; counter++)
	{
		if(((flag >> counter) & 1) && pointer->data[counter].Internal_Read(buf))
			ret |= 1 << counter;
	}

	pointer->ChangedData |= ret;
	pointer->Critic.Unlock();
}

NetActionFlag ContainerConnector::GetDataExt(NetActionFlag flag, NetworkBuffer &buf)
{
	NetActionFlag ret = 0;
	pointer->Critic.Lock();

	for(unsigned counter = 0; counter < NUMNETACTION; counter++)
	{
		if(((flag >> counter) & 1) && pointer->data[counter].Internal_Write(buf))
			ret |= 1 << counter;
	}

	pointer->ChangedData &= ~flag;
	pointer->Critic.Unlock();
	return ret;
}

void ContainerConnector::GetVersion(unsigned version[NUMNETACTION]) const
{
	for(unsigned counter = 0; counter < NUMNETACTION; counter++)
		version[counter] = pointer->data[counter].Version;
}

NetEntity::RegisteredClient::RegisteredClient(SyncServerClient* e) : Client(e), UpdateFlag(0), ClientVersion(0)
{
	_ASSERT(Client);
	Client->grab();
	memset(Version, 0, sizeof(Version));
}

NetEntity::RegisteredClient::RegisteredClient(const RegisteredClient& e) : Client(e.Client), UpdateFlag(e.UpdateFlag), ClientVersion(e.ClientVersion)
{
	_ASSERT(Client);
	Client->grab();
	memcpy(Version, e.Version, sizeof(Version));
}
NetEntity::RegisteredClient::~RegisteredClient()
{
	if(Client) Client->drop();
}

void NetEntity::RegisteredClient::operator = (const RegisteredClient& e)
{
	if(Client) Client->drop();
	UpdateFlag		= e.UpdateFlag;
	Client			= e.Client;
	ClientVersion	= e.ClientVersion;
	e.Client		= NULL;

	memcpy(Version, e.Version, sizeof(Version));
}

ChangedClientContainer::ClientEntity::ClientEntity(SyncServerClient* c) : Client(c)
{
	_ASSERT(Client);
	Client->grab();
}

ChangedClientContainer::ClientEntity::ClientEntity(const ClientEntity &c) : Client(c.Client)
{
	_ASSERT(Client);
	Client->grab();
}

ChangedClientContainer::ClientEntity::~ClientEntity()
{
	if(Client) Client->drop();
}

void ChangedClientContainer::ClientEntity::operator = (const ClientEntity &e)
{
	if(Client) 	Client->drop();
	Client		= e.Client;
	e.Client	= 0;
}

NetEntity::NetEntity(NetIDManager* v)
	: AutoloadEnabled(false), Client(NULL), Server(v), ID(Internal_GetID()), StoredTime(0), ChangedData(0), Connector(ID)
{
	ManualRefresh(ALL);
}

NetEntity::NetEntity(NetEntity* e, bool TakeList)
	: AutoloadEnabled(false), Client(NULL), Server(e->Server), ID(Internal_GetID()), StoredTime(0), ChangedData(0), Connector(ID)
{
	ManualRefresh(ALL);
	if(e && TakeList)	e->GiveClientListToSubElement(this);
}

NetEntity::NetEntity(const ContainerConnector& c, SyncClient* cli, bool autoload)
	: AutoloadEnabled(autoload), Client(cli), Server(NULL), ID(c.GetNetID()), StoredTime(0), ChangedData(0), Connector(c)
{
}

NetEntity::~NetEntity()
{
	if(Server)	Server->RemoveElement(this);
	if(Client)	Client->RemoveElement(this);
}

void NetEntity::PerformNeededUpdates(ChangedClientContainer* out, unsigned int update_interval, unsigned int time, bool add_time, bool update_interval_server_only)
{
	if(update_interval != 0 && (Server || !update_interval_server_only))
	{
		if(add_time)
		{
			StoredTime += time;
			if(StoredTime < update_interval)
				return;
			else
				StoredTime = 0;
		}
		else
		{
			if(time < StoredTime + update_interval)
				return;
			else
				StoredTime = time;
		}
	}

	bool changes = false;
	//perform needed serializations
	NetActionFlag copy = helpers::Atomic_And(ChangedData, 0);

	if(Server && copy)
	{
		changes = true;
		NetworkBuffer buffer;

		for(unsigned counter = 0; counter < NUMNETACTION; counter++)
			if((copy >> counter) & 1)
			{
				unsigned size		= 0;
				unsigned woffset	= buffer.GetWriteOffset();
				buffer.Write(size);
				Serialize(static_cast<NetAction>(1 << counter), buffer);
				size                = buffer.GetWriteOffset() - (woffset + sizeof(unsigned));
				buffer.WriteAt(size, woffset);
			}

		Connector.SetDataExt(copy, buffer);
	}
	else if(Client && (copy || Connector.GetChangedData()))
	{
		unsigned size;
		unsigned roffset;
		NetworkBuffer buffer;

		copy |= Connector.GetChangedData();
		NetActionFlag flag = Connector.GetDataExt(copy, buffer);

		for(unsigned counter = 0; counter < NUMNETACTION; counter++)
			if((flag >> counter) & 1)
			{
				buffer.Read(size);
				roffset = buffer.GetReadOffset();

				Deserialize(static_cast<NetAction>(1 << counter), buffer);
				//if this assertion fails the deserialize didnt work properly. Check the deserialize of this object!
				_ASSERT(buffer.GetReadOffset() == roffset + size);
			}
	}

	if(changes || ChangedClient.NeedCheck())
	{
		ChangedClientLock.Lock();
		Clientlist.lock();

		bool UpdateChildren	= ChangedClient.Force_Check;

		std::vector<RegisteredClient>::iterator								it1 = Clientlist.begin(false);
		std::vector<ChangedClientContainer::ClientEntity>::const_iterator	it2 = ChangedClient.vec.begin();

		while(it1 != Clientlist.end() && it2 != ChangedClient.vec.end())
		{
			//is the client still logged in?
			if(it1->Client->IsKicked())
			{
				UpdateChildren = true;
				it1 = Clientlist.erase(it1);
			}
			else if(it2->Client->IsKicked())
			{
				UpdateChildren = true;
				++it2;
			}
			//is the client allready inserted
			else if(it1->Client == it2->Client)
			{
				Internal_UpdateClient(*it1, changes, out);

				++it1;
				++it2;
			}
			//has the client only to be updated?
			else if(it1->Client < it2->Client)
			{
				Internal_UpdateClient(*it1, changes, out);
				++it1;
			}
			//client has to be added and updated
			else
			{
				it1 = Internal_InsertClient(it1, it2->Client, out);
				++it2;
			}
		}

		while(it1 != Clientlist.end())
		{
			if(it1->Client->IsKicked())
			{
				UpdateChildren = true;
				it1 = Clientlist.erase(it1);
			}
			else
			{
				Internal_UpdateClient(*it1, changes, out);
				++it1;
			}
		}

		while(it2 != ChangedClient.vec.end())
		{
			if(it2->Client->IsKicked())	UpdateChildren = true;
			else						Internal_InsertClient(Clientlist.end(), it2->Client, out);
			++it2;
		}

		ChangedClient.Force_Check = false;
		ChangedClient.vec.clear();

		if(UpdateChildren && out)
			out->Force_Check = true;

		Clientlist.unlock();
		ChangedClientLock.Unlock();
	}
}

std::vector<NetEntity::RegisteredClient>::iterator NetEntity::Internal_InsertClient(std::vector<RegisteredClient>::iterator pos, SyncServerClient* e, ChangedClientContainer* out)
{
	NetActionFlag flag = ClientNeedsUpdates(e);

	if(flag == 0)
		return pos;

	std::vector<RegisteredClient>::iterator it = Clientlist.insert(pos, e, false);

	it->UpdateFlag		= flag;
	it->ClientVersion	= it->Client->GetVersion();

	if(out && (it->UpdateFlag & (CHILDREN1 | CHILDREN2 | CHILDREN3)))
		out->push_back(it->Client);

	if(it->UpdateFlag)
	{
		NetActionFlag tFlag = 0;
		unsigned version[NUMNETACTION];

		Connector.GetVersion(version);

		for(unsigned counter = 0; counter < NUMNETACTION; counter++)
		{
			NetActionFlag flag = (1 << counter);
			if((it->UpdateFlag & flag) && it->Version[counter] != version[counter])
			{
				tFlag |= flag;
				it->Version[counter] = version[counter];
			}
		}

		if(tFlag) it->Client->AddNetEntity(Connector, tFlag);
	}
	return it;
}

void NetEntity::Internal_UpdateClient(RegisteredClient &e, bool needupdate, ChangedClientContainer* out)
{
	//has the client changed its state
	if(e.ClientVersion != e.Client->GetVersion())
	{
		e.UpdateFlag	= ClientNeedsUpdates(e.Client);
		e.ClientVersion	= e.Client->GetVersion();
		needupdate		= true;

		if(out && (e.UpdateFlag & (CHILDREN1 | CHILDREN2 | CHILDREN3)))
			out->push_back(e.Client);
	}
	if(needupdate && e.UpdateFlag)
	{
		NetActionFlag tFlag = 0;
		unsigned version[NUMNETACTION];

		Connector.GetVersion(version);

		for(unsigned counter = 0; counter < NUMNETACTION; counter++)
		{
			NetActionFlag flag = (1 << counter);
			if((e.UpdateFlag & flag) && e.Version[counter] != version[counter])
			{
				tFlag |= flag;
				e.Version[counter] = version[counter];
			}
		}

		if(tFlag) e.Client->AddNetEntity(Connector, tFlag);	}
}

void NetEntity::GiveClientListToSubElement(NetEntity *e)
{
	if(!Server || !e)
		return;

	ChangedClientContainer tempcon;
	for(std::vector<RegisteredClient>::iterator it = Clientlist.begin(true); it != Clientlist.end(); ++it)
		if(it->UpdateFlag & (CHILDREN1 | CHILDREN2 | CHILDREN3))
			tempcon.push_back(it->Client);
	Clientlist.unlock();

	if(!tempcon.NeedCheck())
		return;

	e->ChangedClientLock.Lock();
	e->ChangedClient.insert(tempcon);
	e->ChangedClientLock.Unlock();
}

NetIDManager::NetIDManager(void) : actID(0)
{
}

NetIDManager::~NetIDManager(void)
{
	for(std::map<unsigned int, NetEntity*>::iterator it = Elemente.begin(true); it != Elemente.end(); ++it)
		it->second->Server = NULL;
}

unsigned int NetIDManager::RegistateElement(NetEntity* e)
{
	bool successful = false;

	Elemente.lock();
	while(!successful)//EDIT potential while of eternity
	{
		++actID;
		if(actID == 0) actID = 1;//there should never be a element with ID 0.

 		successful = Elemente.insert(std::pair<unsigned int, NetEntity*>(actID, e), false).second;
	}
	unsigned int id = actID;
	Elemente.unlock();
	return id;
}

void NetIDManager::RemoveElement(NetEntity* e)
{
	std::map<unsigned int, NetEntity*>::iterator temp = Elemente.find(e->GetNetID(), true);
	if(temp != Elemente.end())
		Elemente.erase(temp);

	Elemente.unlock();
}

SyncClient::SyncClient()
{}

SyncClient::~SyncClient()
{}

void SyncClient::RemoveElement(NetEntity* e)
{
	Iter it = RecvContainer.find(e->GetNetID(), true);
	if(it != RecvContainer.end())	it->second.AttachedNetEntity = NULL;
	RecvContainer.unlock();
}

bool SyncClient::Internal_FindOrCreate(Iter &it, unsigned int id, NetEntity* call, NetAction n)
{
	it = RecvContainer.find(id, true);

	if(it == RecvContainer.end())
		it = RecvContainer.insert(std::pair<unsigned int, InternalContainer>(id, id), false).first;
	else
	{
		unsigned version[NUMNETACTION];
		it->second.Connector.GetVersion(version);

		for(unsigned counter = 0; counter < NUMNETACTION; counter++)
			if(version[counter])
				return true;
	}

	if(call) it->second.ElementsNeedingUpdate.push_back(std::pair<unsigned int, NetAction>(call->GetNetID(), n), true);

	return false;
}

void SyncClient::Internal_UpdateAfterCreate(Iter &it, NetEntity* ptr)
{
	it->second.AttachedNetEntity = ptr;
	//simulate a full change to get a full reload in everycase if the element is created again
	helpers::Atomic_Or(it->second.AttachedNetEntity->ChangedData, syncsys::ALL);
	if(ptr->AutoloadEnabled) ptr->PerformNeededUpdates(NULL);

	for(std::list<std::pair<unsigned int, NetAction> >::iterator it2 = it->second.ElementsNeedingUpdate.begin(true); it2 != it->second.ElementsNeedingUpdate.end(); ++it2)
	{
		std::map<unsigned int, InternalContainer>::iterator it3 = RecvContainer.find(it->first, true);
		if(it3 != RecvContainer.end() && it3->second.AttachedNetEntity)
			helpers::Atomic_Or(it3->second.AttachedNetEntity->ChangedData, it2->second);
		RecvContainer.unlock();
	}
	it->second.ElementsNeedingUpdate.clear(false);
	it->second.ElementsNeedingUpdate.unlock();
}

void SyncClient::UnPackData(NetworkBuffer& Buffer, bool updatedirectly)
{
	NetActionFlag	flag;
	unsigned int	tID;
	unsigned int	NumUpdate;

	Buffer.Read(NumUpdate);

	RecvContainer.lock();
	std::list<NetEntity*> UpdateList;
	for(unsigned int counter = 0; counter < NumUpdate; counter++)
	{
		Buffer.Read(tID);
		Buffer.Read(flag);

		Iter RegisteredClient = RecvContainer.find(tID, false);

		if(RegisteredClient == RecvContainer.end()) //Der Container does not exist
			RegisteredClient = RecvContainer.insert(std::pair<unsigned int, InternalContainer>(tID, tID), false).first;

		RegisteredClient->second.Connector.SetDataExt(flag, Buffer);

		if(updatedirectly && RegisteredClient->second.AttachedNetEntity)
			UpdateList.push_back(RegisteredClient->second.AttachedNetEntity);

		for(std::list<std::pair<unsigned int, NetAction> >::iterator it2 = RegisteredClient->second.ElementsNeedingUpdate.begin(true); it2 != RegisteredClient->second.ElementsNeedingUpdate.end(); ++it2)
		{
			Iter it = RecvContainer.find(it2->first, false);
			if(it != RecvContainer.end() && it->second.AttachedNetEntity)
			{
				helpers::Atomic_Or(it->second.AttachedNetEntity->ChangedData, it2->second);
				if(updatedirectly) UpdateList.push_back(it->second.AttachedNetEntity);
			}
		}

		RegisteredClient->second.ElementsNeedingUpdate.clear(false);
		RegisteredClient->second.ElementsNeedingUpdate.unlock();
	}

	if(updatedirectly)
	{
		UpdateList.sort();
		UpdateList.unique();
		for(std::list<NetEntity*>::iterator it = UpdateList.begin(); it != UpdateList.end(); ++it)
			(*it)->Update();
	}
	RecvContainer.unlock();
	Buffer.ShiftBuffer(true, Buffer.GetReadOffset());
}

SyncServerClient::SyncServerClient(unsigned short SUM)
	: NumUpdates(0), ForceSend(false), Version(1), UpdateMessage(SUM), UpdateData(BUFFER_DOUBLE)
{
}

SyncServerClient::~SyncServerClient()
{}

bool SyncServerClient::PackData(helpers::NetworkBuffer& out_buffer, unsigned short &out_message)
{
	//is a update nessessary?
	if(!NumUpdates && !ForceSend)
		return false;

	Critic[1].Lock();
	{
		unsigned temp = NumUpdates;
		out_buffer.Write(temp);
		out_buffer.Write(UpdateData);

		if(UpdateData.GetWriteOffset() * 4 + 1024 < UpdateData.GetBufferSize())
			UpdateData.ResizeBuffer(UpdateData.GetBufferSize() / 2, false);
		else
			UpdateData.ResetOffsets();
		NumUpdates = 0;
	}
	Critic[1].Unlock();

	Critic[0].Lock();
	{
		out_buffer.Write(AdditionalData);
		out_message	= UpdateMessage;
		ForceSend	= false;
	}
	Critic[0].Unlock();

	return true;
}

void SyncServerClient::AddNetEntity(ContainerConnector &e, NetActionFlag flag)
{
	_ASSERT(flag);

	Critic[1].Lock();
	UpdateData.Write(e.GetNetID());
	UpdateData.Write(flag);
	e.GetDataExt(flag, UpdateData);
	++NumUpdates;
	Critic[1].Unlock();
}
#endif
