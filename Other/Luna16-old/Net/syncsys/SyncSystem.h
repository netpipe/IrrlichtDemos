/*! \mainpage SyncSys Documentation
* \htmlinclude readme.html
Licensing: Just contact me under obscurusnox@sourceforge.net :)
 */
#ifdef SYNCSYS

#pragma once

#ifndef SYNCSYS_SYNCSYSTEM_H
#define SYNCSYS_SYNCSYSTEM_H



#ifndef DONT_INCLUDE_HEADERS
#include "NetworkBuffer.h"
#include "SecureVector.h"
#include "SecureList.h"
#include "SecureMap.h"
#endif

//!contains the parts for the synchronisation system itself
namespace syncsys
{

//!bitfield for the NetAction flags
typedef unsigned char NetActionFlag;

//!the possible NetActions
enum NetAction : NetActionFlag	{	NOTHING			= 0,
									INITDATA		= 1,
									UPDATE1			= 2,
									UPDATE2			= 4,
									UPDATE3			= 8,
									UPDATE4			= 16,
									CHILDREN1		= 32,
									CHILDREN2		= 64,
									CHILDREN3		= 128,
									ALL				= 255};

const size_t NUMNETACTION = sizeof(NetActionFlag) * 8;

//********************************************************************
// Synchronize System
//********************************************************************

class NetEntity;
class SyncClient;
class SyncServerClient;

class ChangedClientContainer
{
public:
	struct ClientEntity
	{
		mutable SyncServerClient*	Client;

		ClientEntity(SyncServerClient* c);
		ClientEntity(const ClientEntity &c);
		~ClientEntity();


		bool operator < (const ClientEntity &e)
		{
			return(Client < e.Client);
		}
		void operator = (const ClientEntity &e);
	};

	bool Force_Check;
	std::vector<ClientEntity> vec;

	ChangedClientContainer(void) : Force_Check(false)	{ }

	bool NeedCheck(void) const
	{
		return (Force_Check || !vec.empty());
	}

	void push_back(SyncServerClient* e)
	{
		if(vec.empty() || vec.back().Client != e)
		{
			if(vec.size() == vec.capacity())
				vec.reserve(vec.size()*2 + 512);
			vec.push_back(e);
		}
	}

	void insert(SyncServerClient* e)
	{
		if(vec.size() == vec.capacity())
			vec.reserve(vec.size()*2 + 512);

		bool inserted = false;
		for(std::vector<ClientEntity>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			if(e == it->Client)
			{
				inserted = true;
				break;
			}
			else if(e < it->Client)
			{
				vec.insert(it, e);
				inserted = true;
				break;
			}
		}
		if(!inserted)
			vec.push_back(e);
	}

	void insert(const ChangedClientContainer& c)
	{
		//Annahme: beide Container müssen sortiert sein
		std::vector<ClientEntity> tempvec;

		tempvec.reserve(vec.size() + c.vec.size());

		std::vector<ClientEntity>::iterator			it1 = vec.begin();
		std::vector<ClientEntity>::const_iterator	it2 = c.vec.begin();

		std::vector<ClientEntity>::iterator			end1 = vec.end();
		std::vector<ClientEntity>::const_iterator	end2 = c.vec.end();

		while(it1 != end1 && it2 != end2)
		{
			if(it1->Client == it2->Client)
			{
				tempvec.push_back(*it1);
				++it1;
				++it2;
			}
			else if(it1->Client < it2->Client)
			{
				tempvec.push_back(*it1);
				++it1;
			}
			else
			{
				tempvec.push_back(*it2);
				++it2;
			}
		}
		//Reste hinzufügen
		if(it1 != end1)			tempvec.insert(tempvec.end(), it1, end1);
		else if(it2 != end2)	tempvec.insert(tempvec.end(), it2, end2);

		if(!Force_Check)
			Force_Check = c.Force_Check;

		vec.swap(tempvec);
	}
};

//!class for managing the NetIDs of NetEntity
class NetIDManager
{
	friend class NetEntity;
protected:
	helpers::SecureMap<unsigned int, NetEntity*>	Elemente;
	unsigned int actID;

	NetIDManager();
	virtual ~NetIDManager(void);

	//! called if a NetEntity is created at server.
	virtual unsigned int RegistateElement(NetEntity* e);
	//! called if a NetEntity is destroyed at server.
	virtual void RemoveElement(NetEntity* e);
};
//contains serialized data of the NetEntity and works like a kind of smartpoints (securerelease)
class ContainerConnector
{
	friend class SyncClient;
	struct Container
	{
		unsigned						ID;
		volatile unsigned				RefCounter;
		volatile NetActionFlag			ChangedData;
		helpers::MY_CRITICALSECTION		Critic;

		struct Shunk
		{
			volatile char*				Data;
			volatile unsigned			Size;
			volatile unsigned			Version;

			Shunk(void) : Data(0), Size(0), Version(0)	{}
			~Shunk()									{ delete Data; }

			bool Internal_Read(helpers::NetworkBuffer &buf, unsigned newsize = -1)
			{
				if(newsize == (unsigned)-1)
					buf.Read(newsize);

				if(!newsize) return false;

				if(newsize != Size)
				{
					Size = newsize;
					delete[] Data;
					Data = new char[Size];
				}
				buf.ReadPlain(Data, Size);
				Version++;
				return true;
			}

			bool Internal_Write(helpers::NetworkBuffer &buf)
			{
				if(!Size) return false;
				buf.Write(Size);
				buf.WritePlain(Data, Size);
				return true;
			}
		}data[NUMNETACTION];

		Container(unsigned int id) : ID(id), RefCounter(1)	{ }

	} *pointer;

public:
	ContainerConnector(unsigned int id)				{ pointer = new Container(id); }
	ContainerConnector(const ContainerConnector& c)	{ *this = c; }
	~ContainerConnector()							{ if(0 == helpers::Atomic_Dec(pointer->RefCounter)) delete pointer; }

	NetActionFlag GetChangedData(void) const								{ return pointer->ChangedData; }
	unsigned GetNetID(void) const											{ return pointer->ID; }
	void operator = (const ContainerConnector& c)							{ pointer = c.pointer; helpers::Atomic_Inc(pointer->RefCounter); }
	bool operator == (const ContainerConnector& c) const					{ return (GetNetID() == c.GetNetID()); }
	bool operator < (const ContainerConnector& c) const						{ return (GetNetID() < c.GetNetID()); }

	void SetDataExt(NetActionFlag flag, helpers::NetworkBuffer &buf);
	NetActionFlag GetDataExt(NetActionFlag flag, helpers::NetworkBuffer &buf);

	void GetVersion(unsigned Version[NUMNETACTION]) const;
};

//!Base class for all classes which should be to synchronized by the sync system.
class NetEntity
{
	friend class SyncClient;
	friend class NetIDManager;

protected:
	struct RegisteredClient
	{
		mutable SyncServerClient*	Client;
		NetActionFlag				UpdateFlag;
		unsigned					ClientVersion;
		unsigned					Version[NUMNETACTION];

		RegisteredClient(const RegisteredClient& e);
		RegisteredClient(SyncServerClient* e);
		~RegisteredClient();

		bool operator < (const RegisteredClient& e)	{ return(Client < e.Client); }
		void operator = (const RegisteredClient& e);
	};

private:
	bool									AutoloadEnabled;
	SyncClient*								Client;
	NetIDManager*							Server;
	unsigned int							ID;
	unsigned int							StoredTime;
	volatile NetActionFlag					ChangedData;
	ContainerConnector						Connector;
	ChangedClientContainer					ChangedClient;
	helpers::MY_CRITICALSECTION				ChangedClientLock;
	helpers::SecureVector<RegisteredClient>	Clientlist;

	unsigned int Internal_GetID(void)	{ return (Server ? Server->RegistateElement(this) : 0); }

	std::vector<RegisteredClient>::iterator Internal_InsertClient(std::vector<RegisteredClient>::iterator pos, SyncServerClient* e, ChangedClientContainer* out);
	void Internal_UpdateClient(RegisteredClient& e, bool needupdate, ChangedClientContainer* out);

protected:
	//! constructor for server or no network.
	/** Register the NetEntity by the server (NetIDManager) automatically.*/
	NetEntity(NetIDManager*);
	//! \brief constructor for server or no network.
	/**Register the NetEntity by the server (NetIDManager) automatically.
	If TakeClientListFromElement is true, the instance will copy the clientlist of its parent and check it with the next NetEntity::PerformNeededUpdates-call.*/
	NetEntity(NetEntity*, bool TakeClientListFromElement = true);
	//!constructor for client.
	/**The ContainerConnector contains the serialised data and if dodeserialise is true, Deserialize will be called once after creation.*/
	NetEntity(const ContainerConnector&, SyncClient*, bool dodeserialize);

	virtual ~NetEntity();

	void GiveClientListToSubElement(NetEntity* e);

	//!Getter for Client variable to enable a clientobject to create other objects
	SyncClient* GetClient(void) const	{ return Client; }

	//!Updatemethode for NetEntity.
	/**Update the ClientList and checks if a (de)serialize is necessary. out should be passed to children (NetEntity).
	\param out: pointer to a ChangedClientContainer instance which should be filled an given to the children.
	\param update_interval: if the passed time is under this limit, PerformNeededUpdates wont do anything
	\param time: time (relative or absolute) used for the update_interval check
	\param add_time: if true the given time will be added to the internal variable. Can be used for delta times.
	\param update_interval_server_only:  if true the update_interval affects only the server*/
	void PerformNeededUpdates(ChangedClientContainer* out, unsigned int update_interval = 0, unsigned int time = -1, bool add_time = false, bool update_interval_server_only = true);
	//!Updatemethode for NetEntity.
	/**Update the ClientList and checks if a (de)serialize is necessary. in could be get from parent (NetEntity), out should be passed to children (NetEntity).
	\param in: ChangedClientContainer instance filled from the parent PerformNeededUpdates.
	\param out: pointer to a ChangedClientContainer instance which should be filled an given to the children.
	\param update_interval: if the passed time is under this limit, PerformNeededUpdates wont do anything
	\param time: time (relative or absolute) used for the update_interval check
	\param add_time: if true the given time will be added to the internal variable. Can be used for delta times.
	\param update_interval_server_only:  if true the update_interval affects only the server*/
	void PerformNeededUpdates(const ChangedClientContainer &in, ChangedClientContainer* out = NULL, unsigned int update_interval = 0, unsigned int time = -1, bool add_time = false, bool update_interval_server_only = true)
	{
		AddClients(in);
		PerformNeededUpdates(out, update_interval, time, add_time, update_interval_server_only);
	}
	//! Methode for serialize the NetEntity.
	virtual void Serialize(NetAction, helpers::NetworkBuffer&) = 0;
	//! Methode for deserialize the NetEntity.
	virtual void Deserialize(NetAction, helpers::NetworkBuffer&) = 0;
	//!callback-methode to determinate if it is needed by the client.
	/** Could (and should) be overloaded to specify the behavour.
	Will be called if (a) a new client is passed to this instance / (b) a Client changed its version and the clientlist gets updated
	\return should be a combination of NetAction. Default is ALL*/
	virtual NetActionFlag ClientNeedsUpdates(SyncServerClient* client)	{ return ALL; }

public:
	//! Standard update-methode for extern call of NetEntity::PerformNeededUpdates.
	/**calls PerformNeededUpdates(NULL, update_interval, time, add_time). Enables a foreign element to call NetEntity::PerformNeededUpdates. Overload if you dont want this behaviour.*/
	virtual void Update(unsigned int update_interval = -1, unsigned int time = -1, bool add_time = false, bool update_interval_server_only = true)
	{ PerformNeededUpdates(NULL, update_interval, time, add_time, update_interval_server_only); }
	//! Standard update-methode for extern call of NetEntity::PerformNeededUpdates.
	/**calls PerformNeededUpdates(in, NULL). Enables a foreign element to call NetEntity::PerformNeededUpdates. Overload if you dont want this behaviour.*/
	virtual void Update(const ChangedClientContainer &in, unsigned int update_interval = -1, unsigned int time = -1, bool add_time = false, bool update_interval_server_only = true)
	{ PerformNeededUpdates(in, NULL, update_interval, time, add_time, update_interval_server_only); }

	//! Force the NetEntity to update its clientlist within next NetEntity::PerformNeededUpdates-call.
	void ForceClientlistCheck(void)										{ ChangedClient.Force_Check = true;	}
	//!Add the clients which changed
	/**normally you get the list of changed clients from the parent (NetBaseClient). Threadsafe.*/
	void AddClients(const ChangedClientContainer &list)					{ if(!list.NeedCheck()) return; ChangedClientLock.Lock(); ChangedClient.insert(list); ChangedClientLock.Unlock(); }
	//!Add a single client.
	/**normally you use this methode only with the root NetEntity to add a new SyncServerClient to the system. Threadsafe. very slow!*/
	void AddClient(SyncServerClient* client)							{ if(!client) return; ChangedClientLock.Lock(); ChangedClient.insert(client); ChangedClientLock.Unlock(); }
	//!provoke a (de)serialize within next NetEntity::PerformNeededUpdates-call.
	/**Call ManualRefresh if a instance attribute or children has changed so the Serialize-methode
	will be called within next NetEntity::PerformNeededUpdates-call.*/
	void ManualRefresh(NetActionFlag f)							{ if(Server) helpers::Atomic_Or(ChangedData, f); }
	//!Returns the unique NetID of this element.
	unsigned int GetNetID(void) const							{ return ID; }
	//!Returns the size of ChangedClient. Useful for performancetests
	size_t GetChangedClientSize(void) const						{ return ChangedClient.vec.size(); }
	//!Returns if the element is a serverside element
	bool IsServerElement(void) const							{ return (Server != 0); }
};

//!base class for a client
/**this base client is abled to receive and store data from a server for serialised NetEntity-instances.
Provides methodes for creating the serialised instances at clientsite.*/
class SyncClient : private NetIDManager
{
	struct InternalContainer
	{
		helpers::SecureList<std::pair<unsigned int, NetAction> >	ElementsNeedingUpdate;
		ContainerConnector											Connector;
		NetEntity*													AttachedNetEntity;

		InternalContainer(unsigned int id) : Connector(id), AttachedNetEntity(NULL)	{ }
	};

	helpers::SecureMap<unsigned int, InternalContainer> RecvContainer;
	typedef std::map<unsigned int, InternalContainer>::iterator	Iter;

	bool Internal_FindOrCreate(Iter &it, unsigned int id, NetEntity* Callback, NetAction n);
	void Internal_UpdateAfterCreate(Iter &it, NetEntity* ptr);

public:
	//!constructor for a SyncClient
	SyncClient(void);
	virtual ~SyncClient();

	//!Unpack all serialised data from a updatepacket and stores them.
	/** Updates allready created NetEntity-instances directly, if updatedirectly is true and they got new data.
	Otherwise you have to call NetEntity::PerformNeededUpdates frequently.
	If all elements are updated frequently anyway set updatedirectly to false.
	\note Shifts the given Buffer after usage, so you can read the rest normally.
	\attention Never call UnPackData with updatedirectly = true asynchronously (for example from MemExClient::AsyncCallBack).*/
	void UnPackData(helpers::NetworkBuffer&, bool updatedirectly);
	//!Register a NetEntity.
	/**Registered NetEntities will be notified by a ManualRefresh, if either a NetEntity with NetID equals NeededID is created or serialised data for NeededID arrives.*/
	void AddElementNeedingData(NetEntity* Callback, unsigned int NeededID, NetAction NeededAction)
	{
		std::map<unsigned int, InternalContainer>::iterator it = RecvContainer.find(NeededID, true);

		if(it == RecvContainer.end())
			it = RecvContainer.insert(std::pair<unsigned int, InternalContainer>(NeededID, NeededID), false).first;

		it->second.ElementsNeedingUpdate.push_back(std::pair<unsigned int, NetAction>(Callback->GetNetID(), NeededAction), true);
		RecvContainer.unlock();
	}
	//!For creation of NetEntities at clientsite
	/**You should only use this methode for creating/getting NetEntity-instances on clientsize. Otherwise the NetEntity may not be assigned correctly to the system.
	If createanyway is true, the NetEntity will be created although if the serialised data did not arrive.
	Registrate Callback if the element is not created. Callback will be "informed" by changing the version causing a new deserialisation.*/
	template<class T1> T1* GetReceivedElement(unsigned int id, NetAction n = NOTHING, NetEntity* Callback = NULL, bool createanyway = false)
	{
		T1* pointer = NULL;
		Iter it;

		if(Internal_FindOrCreate(it, id, Callback, n) || createanyway)
		{
			if(it->second.AttachedNetEntity)
				pointer = static_cast<T1*>(it->second.AttachedNetEntity);
			else
			{
				pointer = new T1(it->second.Connector, this);
				Internal_UpdateAfterCreate(it, pointer);
			}
		}
		RecvContainer.unlock();
		return pointer;
	}
	//!For creation of NetEntities at clientsite
	/**You should only use this methode for creating/getting NetEntity-instances on clientsize. Otherwise the NetEntity may not be assigned correctly to the system.
	If createanyway is true, the NetEntity will be created although if the serialised data did not arrive.
	Registrate Callback if the element is not created. Callback will be "informed" by changing the version causing a new deserialisation.*/
	template<class T1, typename T2> T1* GetReceivedElement(unsigned int id, T2 arg1, NetAction n = NOTHING, NetEntity* Callback = NULL, bool createanyway = false)
	{
		T1* pointer = NULL;
		Iter it;
		if(Internal_FindOrCreate(it, id, Callback, n) || createanyway)
		{
			if(it->second.AttachedNetEntity)
				pointer = static_cast<T1*>(it->second.AttachedNetEntity);
			else
			{
				pointer = new T1(it->second.Connector, this, arg1);
				Internal_UpdateAfterCreate(it, pointer);
			}
		}
		RecvContainer.unlock();
		return pointer;
	}
	//!For creation of NetEntities at clientsite
	/**You should only use this methode for creating/getting NetEntity-instances on clientsize. Otherwise the NetEntity may not be assigned correctly to the system.
	If createanyway is true, the NetEntity will be created although if the serialised data did not arrive.
	Registrate Callback if the element is not created. Callback will be "informed" by changing the version causing a new deserialisation.*/
	template<class T1, typename T2, typename T3> T1* GetReceivedElement(unsigned int id, T2 arg1, T3 arg2, NetAction n = NOTHING, NetEntity* Callback = NULL, bool createanyway = false)
	{
		T1* pointer = NULL;
		Iter it;
		if(Internal_FindOrCreate(it, id, Callback, n) || createanyway)
		{
			if(it->second.AttachedNetEntity)
				pointer = static_cast<T1*>(it->second.AttachedNetEntity);
			else
			{
				pointer = new T1(it->second.Connector, this, arg1, arg2);
				Internal_UpdateAfterCreate(it, pointer);
			}
		}
		RecvContainer.unlock();
		return pointer;
	}
	//!Removes the AttachedNetEntity for a NetEntity
	void RemoveElement(NetEntity* e);
};

//!base class for a to the server connected client
/**Representate a to the server connected client.
Stores the changed NetEntities, packs them into a updatepacket and Sends it to the client.*/
class SyncServerClient
{
	volatile unsigned				NumUpdates;
	volatile bool					ForceSend;
	volatile unsigned				Version;
	volatile unsigned short			UpdateMessage;
	helpers::NetworkBuffer			AdditionalData;
	helpers::NetworkBuffer			UpdateData;
	helpers::MY_CRITICALSECTION		Critic[2];

protected:

	virtual ~SyncServerClient();
	//!Setter for additional informations
	/**The updatepackets Send to the clients has the set Message and the additional data stored in the buffer will be append to the updatepacket.
	So the client can read the additional data after it has passed the updatepacket to UnPackData (which will only read the update data and shift the buffer after usage).*/
	void SetUpdatePacketInfos(unsigned short Msg, const helpers::NetworkBuffer& buffer = helpers::NetworkBuffer())
	{
		Critic[0].Lock();
		ForceSend		= true;
		UpdateMessage	= Msg;
		AdditionalData	= buffer;
		AdditionalData.SetReadOffset(0);
		Critic[0].Unlock();
	}

public:
	//!constructor
	/**	\param StartUpdateMsg: sets UpdateMessage*/
	SyncServerClient(unsigned short StartUpdateMsg);

	//!Increase the clientversion.
	/**If the clientsversions changes, NetEntities which update their clientlist will acknowledge this change and call NetEntity::ClientNeedsUpdates.*/
	void			IncreaseVersion(void)					{ helpers::Atomic_Inc(Version); }
	//!Getter for the clientversion
	unsigned int	GetVersion(void) const					{ return Version; }
	//!Packs data for the client
	/**Checks if data need to be sent to the client. Collects the data,
	appends additional data and returns UpdateMessage and the Buffer if a update is needed.
	/return true = update needed => send the data*/
	bool PackData(helpers::NetworkBuffer& out_buffer, unsigned short &out_message);
	//Methode adds a ContainerConnector (NetEntity-data) to the 'must-be-sent'-list. Should only be called by the system!
	void AddNetEntity(ContainerConnector &e, NetActionFlag flag);

	virtual void grab(void) = 0;
	virtual void drop(void) = 0;
	virtual bool IsKicked(void) const = 0;
};

};

#endif
#endif
