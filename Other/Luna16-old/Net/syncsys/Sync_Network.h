#ifdef SYNCSYS
#pragma once

#ifndef SYNCSYS_SYNC_NETWORK_H
#define SYNCSYS_SYNC_NETWORK_H

#ifndef DONT_INCLUDE_HEADERS
#include "network.h"
#include "SyncSystem.h"
#endif


namespace syncsys
{

//!specialised version of netlib::BasicClient for the sync system.
/**this specialised client is abled to receive and store data from a MemExServer for serialised NetEntity-instances.
Provides methodes for creating the serialised instances at clientsite.*/
class MemExClient : public netlib::BasicClient, public SyncClient
{
public:
	//!constructor for a MemExClient
	MemExClient(bool AllowSendBlock = false, bool DisableNagle = true)
		: netlib::BasicClient(AllowSendBlock, DisableNagle)	{}
	virtual ~MemExClient()	{}
};

//!specialised version of netlib::BasicServer for the sync system.
/**On serverside created NetEntities should be assigned to server.
Normally you only have to give the root-NetEntity a pointer to a serverinstance. Children will get the pointer from parentelements.
\attention all registered NetEntities should be destroyed before deallocating the server.*/
class MemExServer : public netlib::BasicServer, public NetIDManager
{
public:
	MemExServer(unsigned int maxbuffer = 1024 * 1024)
		: netlib::BasicServer(maxbuffer)	{}
};

//!specialised version of netlib::BasicServerClient for the sync system.
/**Representate a to the server connected client.
Stores the changed NetEntities, packs them into a updatepacket and Sends it to the client.*/
class MemExServerClient : public netlib::BasicServerClient, public syncsys::SyncServerClient
{
	short	SyncCompression;

protected:
	virtual ~MemExServerClient()	{}

public:
	//!workaround
	std::vector<bool>			BoolArray;
	//!workaround
	std::vector<unsigned int>	IDArray;

	//!constructor
	MemExServerClient(netlib::InitData &info, unsigned short StartUpdateMsg, unsigned int UpdateInterval = 10, bool AllowSendBlock = false, bool DisableNagle = true)
		: netlib::BasicServerClient(info, UpdateInterval, AllowSendBlock, DisableNagle), syncsys::SyncServerClient(StartUpdateMsg), SyncCompression(Z_BEST_SPEED) {}

	//!Setter for the used compression
	/**Sets the used zlib compression. Default: Z_BEST_SPEED.*/
	void			SetCompression(short c = Z_BEST_SPEED)		{ SyncCompression = c; }
	//!Specialised callback of BasicServerClient.
	/**Calls PackData of SyncServerClient and send the returned buffer if needed.*/
	virtual void UpdateClient(void)													{ unsigned short msg; helpers::NetworkBuffer buf; if(PackData(buf, msg)) Send(buf, msg, SyncCompression); }
	void grab(void)																	{ BasicServerClient::grab(); }
	void drop(void)																	{ BasicServerClient::drop(); }
	bool IsKicked(void)const														{ return BasicServerClient::IsKicked(); }
};

};
#endif
#endif
