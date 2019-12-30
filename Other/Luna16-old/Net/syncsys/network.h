#ifdef SYNCSYS
#pragma once

#ifndef SYNCSYS_NETWORK_H
#define SYNCSYS_NETWORK_H

#ifndef DONT_INCLUDE_HEADERS
#include "Portable.h"
#include "ThreadedObject.h"
#include "zlib.h"
#include "SecureList.h"
#include "NetworkBuffer.h"
#endif

//!the TCP layer which can be easily be replaced by another networklib
namespace netlib
{

enum NetworkState {INITEDSOCKET, CONNECTING, CLOSING, STARTED, CLOSED, FAILED };

const unsigned short CLOSEMSG	= (unsigned short)-1;
const unsigned int PAKETSIZE	= 2 * sizeof(unsigned int) + sizeof(unsigned short);

class BasicServer;
class BasicServerClient;

//!Container for some important info for a new connected client
struct InitData
{
	sockaddr_in		Address;
	BasicServer*	Host;
	SOCKET			socket;

 	InitData(sockaddr_in add, BasicServer* host, SOCKET s) : Address(add), Host(host), socket(s)	{}
};

//!Class carring the sent data and the message.
class Packet
{
public:
	unsigned int			ZippedSize;
	unsigned int			UnZippedSize;
	unsigned short			MSG;
	helpers::NetworkBuffer	Buffer;

	Packet()				: ZippedSize(0), MSG(CLOSEMSG) { }
	Packet(const Packet& p) : ZippedSize(0), MSG(CLOSEMSG) { *this = p; }
	~Packet()			{ }

	void ReadHeader(helpers::NetworkBuffer& buf)
	{
		buf.Read(ZippedSize);
		buf.Read(UnZippedSize);
		buf.Read(MSG);
	}

	void WriteHeader(void)
	{
		Buffer.CheckBufferSize(PAKETSIZE, 0);

		Buffer.WriteAt(ZippedSize, 0);
		Buffer.WriteAt(UnZippedSize, sizeof(int));
		Buffer.WriteAt(MSG, 2 * sizeof(int));
	}
	void clear(void)
	{
		MSG				= CLOSEMSG;
		ZippedSize		= 0;
		UnZippedSize	= 0;
	}

	void operator = (const Packet& p)
	{
		MSG				= p.MSG;
		UnZippedSize	= p.UnZippedSize;
		ZippedSize		= p.ZippedSize;
		Buffer			= p.Buffer;
	}
};
//!base class for every socket based communication (UDP or TCP)
class Network
{
protected:
	bool			UseTCP;
	bool			DisableNagle;
	SOCKET			Socket;
	NetworkState	state;

	NetworkState InitSocket(void);

public:
	Network(bool TCP, bool DisableNagle);
	virtual ~Network();

	void geterror(void);
	///getter for the networkstate
	NetworkState GetNetworkState(void)						{ return state; }
	///Callback for handling messages. Messagehandler must be threadsafe because this can be called from several threads.
	virtual void HandleErrorMessage(const char* message)	{}
};

class Clientclass : public Network
{
	int							SendBufferSize;
	bool						ParsedHeader;
	bool						AllowSendBlock;
	Packet						RecvPacket;
	Packet						SendPacket;
	helpers::NetworkBuffer		RecvBuffer;
	helpers::SecureList<Packet> SyncList;
	helpers::MY_CRITICALSECTION	Critic;

protected:

	z_stream *Istream, *Dstream;

	void CheckAndDoResize(helpers::NetworkBuffer& buffer, unsigned size, bool copydata)
	{
		if(IsBufferToBig(buffer, size))	buffer.ResizeBuffer(size, copydata);
		else							buffer.CheckBufferSize(size, 0);
	}
	void CloseConnection(bool forceclose);
	//!this callback is called for every stored packet if HandleSyncPacket is called.
	virtual void SyncCallbackFunc(Packet& p)		{}
	//!this callback is called for every recieved packet.
	/**Return false if you want to process the packet later synchronously by calling HandleSyncPacket.
	\attention This callback is called asychronously! */
	virtual bool AsyncCallbackFunc(Packet& p)		{ return false; }

	void Internal_CheckSocketSettings(void);

public:
	unsigned int NumRecvPackets, NumSendPackets, SizeRecvPackets, SizeSendPackets;

	Clientclass(bool AllowSendBlock, bool DisableNagle, int SendBufferSize);
	virtual ~Clientclass();
	void ClientCallback(void);

	void Internal_ZipPacket(Packet& paket, const char* buffer, unsigned size, short Compression = Z_BEST_SPEED, bool lock = true);
	void Internal_SendZippedPacket(const Packet& paket, bool lock = true);

	//!Send a paket to peer.
	void Send(const Packet& p, short Compression = Z_BEST_SPEED)
	{
		Send(p.Buffer, p.MSG, Compression);
	}
	//!Send buffercontent to peer.
	void Send(const helpers::NetworkBuffer& buffer, unsigned short MSG, short Compression = Z_BEST_SPEED)
	{
		Send(buffer.GetData(), static_cast<unsigned int>(buffer.GetWriteOffset()), MSG, Compression);
	}
	//!Send plaindata to peer.
	void Send(const char* daten, unsigned int size, unsigned short MSG, short Compression = Z_BEST_SPEED);
	//!Call this methode to handle the recieved and stored packets.
	void HandleSyncPacket(void)
	{
		for(std::list<Packet>::iterator temp = SyncList.begin(true); temp != SyncList.end(); ++temp)
			SyncCallbackFunc(*temp);

		SyncList.clear(false);
		SyncList.unlock();
	}
	//!Callback which is called if connection is lost.
	/**\attention This callback is called asychronously! */
	virtual void ReactToDisc(bool NormalTermination)										{}
	//!Callback to check a recieved header, if it fits condition like max buffersize etc.
	/**\attention This callback is called asychronously! */
	virtual bool IsHeaderOkay(const Packet &p) const										{ return true; }
	//!Callback for checking if a resize is needed
	/**\attention This callback is called asychronously! */
	virtual bool IsBufferToBig(const helpers::NetworkBuffer& c, size_t neededsize) const	{ return (c.GetBufferSize() > neededsize * 100 + 10000); }
};

//!basisnetwork-client for connection to a basicnetwork-server
class BasicClient : public Clientclass, public helpers::ThreadedObject
{
	///Address of host.
	/**This variable will be set and used by the BasicClient::connectto-methodes.*/
	sockaddr_in ConnectionAddress;

	void ThreadUpdate(unsigned);
	bool tryconnect(void);

	bool ShouldStop(void) const		{ return state != STARTED && state != CLOSING && state != CONNECTING; }
	void PerformLastWishes(void)	{ ReactToDisc(state == CLOSED); }

public:
	//!standard c'tor
	BasicClient(bool AllowSendBlock = false, bool DisableNagle = true, int SendBufferSize=256*1024);
	virtual ~BasicClient();

	//!connect to a server.
	/**if blocking = true the methode waits for the accept.
	Else try to connect asycronously until success or a call to disconnect. Return true, if no error occurs.*/
	virtual bool connectto(sockaddr_in, bool blocking = false);
	//!connect to a server.
	/**if blocking = true the methode waits for the accept.
	Else try to connect asycronously until success or a call to disconnect. Return true, if no error occurs.*/
	virtual bool connectto(const char*, int, bool blocking = false);
	//!disconnect from server, if connected and reinit socket.
	virtual bool disconnect(void);
};

//!basisnetwork-server
class BasicServer : protected Network, public helpers::ThreadedObject
{
	unsigned int							maxbuffersize;
	helpers::SecureList<BasicServerClient*>	ClientList;

	bool	ShouldStop(void) const		{ return state != STARTED; }
	void	ThreadUpdate(unsigned time);

protected:
	//!give you a startiterator.
	/** \note List gets locked so you have to unlock it afterwards. */
	std::list<BasicServerClient*>::const_iterator	GetClientListBegin(void)	{ return ClientList.begin(true); }
	//!give you a enditerator
	std::list<BasicServerClient*>::const_iterator	GetClientListEnd(void)		{ return ClientList.end(); }
	//!Unlock the clientlist
	void											UnlockClientList(void)		{ ClientList.unlock(); }

public:
	//!standard c'tor
	BasicServer(unsigned int maxbuffer = 1024 * 1024);
	virtual ~BasicServer();

	//!set server in listening mode.
	bool StartRunning(int port);
	//!stop and reset the server.
	/** \note i can not guarantee, that all is cleaned up. Better Realloc Server than reuse.*/
	void StopRunning(void);
	//!kick a definit client. Not very fast.
	void Kick(BasicServerClient* c);
	//!call HandleSyncPacket for all connected clients.
	/** \note Not very fast.*/
	void HandleClientSyncPacket(void);
	//!Callback for the clientconnected-event. O
	/**overload and return a allocated instance of your serverclient/participant/connected client or return NULL if you dont want to accept the client.
	\attention This callback is called asychronously! */
	virtual BasicServerClient* ClientConnected(InitData&) = 0;
	//!Send a message to all(!) connected clients.
	/** \note Not very fast because a complete clientlist-lock is needed.*/
	void Send(const char* daten, unsigned int size, unsigned short MSG, short Compression = Z_BEST_SPEED);
	//!Send a message to all(!) connected clients.
	/** \note Not very fast because a complete clientlist-lock is needed.*/
	void Send(const helpers::NetworkBuffer& buffer, unsigned short MSG, short Compression = Z_BEST_SPEED)
	{
		Send(buffer.GetData(), static_cast<unsigned int>(buffer.GetWriteOffset()), MSG, Compression);
	}
	//!getter for max of allowed paketsize (=unzipped size and zipped size).
	unsigned int GetMaxBufferSize(void)		{ return maxbuffersize; }
	//!getter for the connected client count.
	size_t GetClientCount(void)				{ return ClientList.size(); }
};

//!basisnetwork-serverclient which representates a participant/ a to the server connected client.
class BasicServerClient : public Clientclass
{
	friend class BasicServer;

	unsigned int	LastUpdate;
	unsigned int	RefCounter;
	sockaddr_in		Address;

	bool			NeedUpdate(unsigned int t)								{ return(LastUpdate < t && t - LastUpdate > UpdateInterval); }
	virtual void	UpdateClient(void)										{ }
	virtual void	DeleteClient(void)										{ delete this; }

protected:
	BasicServer*	Host;
	unsigned int	UpdateInterval;
	//!standard c'tor.
	/**UpdateInterval derterminates how offen UpdateClient is called.*/
	BasicServerClient(InitData&, unsigned int UpdateInterval = 50, bool AllowSendBlock = false, bool DisableNagle = true, int SendBufferSize=256*1024);
	virtual ~BasicServerClient();

public:
	//!setter for UpdateInterval.
	/**UpdateInterval derterminates how offen UpdateClient is called.*/
	void SetUpdateInterval(unsigned int i)		{ UpdateInterval = i; }
	//!shifts the LastUpdate.
	void AddSingularDelay(unsigned int i)		{ LastUpdate += i; }
	//!getter for the peer-address.
	sockaddr_in GetAddress(void) const			{ return Address; }
	//!Should be clear ;)
	void Kick(void)								{ CloseConnection(false); }

	void grab(void)								{ helpers::Atomic_Inc(RefCounter); }
	void drop(void)								{ if(0 == helpers::Atomic_Dec(RefCounter)) DeleteClient(); }
	bool IsKicked(void) const					{ return (state == FAILED || state == CLOSED); }
	//!finalised version of Clientclass callback
	bool IsHeaderOkay(const Packet &p) const	{ return p.UnZippedSize < Host->GetMaxBufferSize() && p.ZippedSize < Host->GetMaxBufferSize(); }
};

};

#endif
#endif
