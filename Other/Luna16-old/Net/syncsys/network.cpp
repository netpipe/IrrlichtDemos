#ifdef SYNCSYS
#define FD_SETSIZE 10000 //!this limits the count of clients per serverthread. I guess 10k should be enought

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2tcpip.h>

typedef int SOCKLEN;
#else
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define closesocket	close
typedef socklen_t SOCKLEN;
typedef timeval TIMEVAL;
#endif

#include "network.h"

#include <memory>
#include <cstdio>
#include <sstream>
#include <errno.h>


#if defined _WIN32 && defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace helpers;
using namespace netlib;


//////////////////////////////////////////////////////////////////////
// NETWORK
//////////////////////////////////////////////////////////////////////

#ifdef _WIN32
struct WSAInit
{
	WSAInit()	{ WSADATA wsaData; WSAStartup(MAKEWORD(2,2), &wsaData); }
	~WSAInit()	{ WSACleanup(); }
} WSAInitVar;
#endif

Network::Network(bool TCP, bool disable)
	: UseTCP(TCP), DisableNagle(disable), Socket((SOCKET)-1), state(CLOSED)
{
}

Network::~Network()
{
	if(Socket != -1 && 0 != closesocket(Socket))
		geterror();
}

NetworkState Network::InitSocket(void)
{
	if(Socket != -1 && 0 != closesocket(Socket))
		geterror();

	Socket = socket(AF_INET, (UseTCP ? SOCK_STREAM : SOCK_DGRAM), 0);

	if(-1 == Socket)
	{
		geterror();
		state = FAILED;
	}
	else
		state = INITEDSOCKET;

	return state;
}

void Network::geterror(void)
{
#ifdef _WIN32
	char* lpMsgBuf;
	int errorcode = WSAGetLastError();
	if(!errorcode)
		return;

	if (!FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(char*)&lpMsgBuf,
		0,
		NULL ))
		return;

	HandleErrorMessage(lpMsgBuf);
	LocalFree( lpMsgBuf );
#else
	int errorcode = errno;
	char* lpMsgBuf = strerror(errorcode);
	if(!lpMsgBuf)
		return;

	HandleErrorMessage(lpMsgBuf);
#endif
}
//////////////////////////////////////////////////////////////////////
// SERVER
//////////////////////////////////////////////////////////////////////
BasicServer::BasicServer(unsigned int maxbuffer)
	: Network(true, false), maxbuffersize(maxbuffer)
{
}

BasicServer::~BasicServer()
{
	if(state == STARTED)
		StopRunning();
}

bool BasicServer::StartRunning(int port)
{
	if(state != INITEDSOCKET && INITEDSOCKET != InitSocket())
		return false;

	sockaddr_in	eigeneAddy;
	eigeneAddy.sin_family		= AF_INET;
	eigeneAddy.sin_addr.s_addr	= INADDR_ANY;
	eigeneAddy.sin_port			= htons(port);

	if(-1 == bind(Socket, (sockaddr*)&eigeneAddy, sizeof(sockaddr_in)) || -1 == listen(Socket, SOMAXCONN))
	{
		geterror();
		state = FAILED;
		return false;
	}

	state = STARTED;
	StartThread();

	return true;
}

void BasicServer::StopRunning(void)
{
	state = CLOSING;

	StopThread();

	for(std::list<BasicServerClient*>::iterator temp = ClientList.begin(true); temp != ClientList.end(); ++temp)
	{
		(*temp)->Kick();
		(*temp)->drop();
	}
	ClientList.clear(false);
	ClientList.unlock();

	InitSocket();
}

void BasicServer::ThreadUpdate(unsigned Time)
{
	TIMEVAL		time;
	fd_set		readset;
	SOCKET max = Socket;

	time.tv_sec = 0;
	time.tv_usec = 1000;

	FD_ZERO(&readset);
	FD_SET(Socket, &readset);

	for(std::list<BasicServerClient*>::iterator temp = ClientList.begin(true); temp != ClientList.end(); ++temp)
	{
		_ASSERT((*temp)->Socket != -1);

		FD_SET((*temp)->Socket, &readset);
		if((*temp)->Socket > max)
			max = (*temp)->Socket;
	}
	ClientList.unlock();

	if(-1 == select(static_cast<int>(max + 1), &readset, NULL, NULL, &time))
		geterror();

	if(FD_ISSET(Socket, &readset))
	{
		SOCKET tempsocket;
		sockaddr tempaddy;

		SOCKLEN le = sizeof(sockaddr_in);

		tempsocket = accept(Socket, &tempaddy, &le);

		if(tempsocket == -1)
			geterror();
		else
		{
			InitData info(*(sockaddr_in*)&tempaddy, this, tempsocket);
			BasicServerClient* c = ClientConnected(info);
			if(!c)
			{
				//max of clients is reached
				Packet post;
				post.MSG = CLOSEMSG;
				post.WriteHeader();
				send(tempsocket, post.Buffer.GetData(), PAKETSIZE, 0);
				if(0 != closesocket(tempsocket))
					geterror();
			}
			else
			{
				//add new client
				ClientList.push_back(c, true);
			}
		}
	}

	for(std::list<BasicServerClient*>::iterator temp = ClientList.begin(true); temp != ClientList.end();)
	{
		if(FD_ISSET((*temp)->Socket, &readset))
			(*temp)->ClientCallback();

		if((*temp)->NeedUpdate(Time))
		{
			(*temp)->LastUpdate = Time;
			(*temp)->UpdateClient();
		}

		if((*temp)->IsKicked())
		{
			(*temp)->ReactToDisc((*temp)->state == CLOSED);
			(*temp)->drop();
			temp = ClientList.erase(temp);
		}
		else
			++temp;
	}
	ClientList.unlock();
}

void BasicServer::Send(const char *daten, unsigned int size, unsigned short MSG, short Compression)
{
	bool	zipped = false;
	Packet	SendPacket;
	SendPacket.MSG = MSG;
	for(std::list<BasicServerClient*>::iterator temp = ClientList.begin(true); temp != ClientList.end(); ++temp)
	{
		if(!zipped)
			(*temp)->Internal_ZipPacket(SendPacket, daten, size, Compression);

		(*temp)->Internal_SendZippedPacket(SendPacket);
	}
	ClientList.unlock();
}

void BasicServer::Kick(BasicServerClient* c)
{
	if(!c) return;

	for(std::list<BasicServerClient*>::iterator temp = ClientList.begin(true); temp != ClientList.end(); ++temp)
	{
		if(*temp == c)
		{
			c->Kick();
			break;
		}
	}

	ClientList.unlock();
}

void BasicServer::HandleClientSyncPacket(void)
{
	for(std::list<BasicServerClient*>::iterator temp = ClientList.begin(true); temp != ClientList.end(); ++temp)
		(*temp)->HandleSyncPacket();
	ClientList.unlock();
}

//////////////////////////////////////////////////////////////////////
// Clientclass
//////////////////////////////////////////////////////////////////////
Clientclass::Clientclass(bool block, bool disable, int size)
	: Network(true, disable), SendBufferSize(size), ParsedHeader(false), AllowSendBlock(block), RecvBuffer(PAKETSIZE)
{
	NumRecvPackets	= 0;
	NumSendPackets	= 0;
	SizeRecvPackets	= 0;
	SizeSendPackets	= 0;

	Istream			= new z_stream;
	Istream->zalloc	= (alloc_func)0;
	Istream->zfree	= (free_func)0;

	inflateInit(Istream);

	Dstream			= new z_stream;
	Dstream->zalloc	= (alloc_func)0;
	Dstream->zfree	= (free_func)0;
	Dstream->opaque	= (voidpf)0;

	deflateInit(Dstream, Z_BEST_SPEED);
}

Clientclass::~Clientclass()
{
	CloseConnection(true);

	inflateEnd(Istream);
	deflateEnd(Dstream);
	delete Istream;
	delete Dstream;
}

void Clientclass::CloseConnection(bool forceclose)
{
	if(!forceclose && state == STARTED)
	{
		Packet post;
		post.MSG = CLOSEMSG;
		Send(post);
		state = CLOSING;
	}
	else if(forceclose || state != CLOSING)
	{
		state = CLOSED;
		if(Socket != -1 && 0 != closesocket(Socket))
			geterror();

		Socket = (SOCKET)-1;
	}
}

void Clientclass::Internal_CheckSocketSettings(void)
{

	if(DisableNagle)
	{
		int boolean = 1;
		setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&boolean, sizeof(int));
	}

	setsockopt(Socket, SOL_SOCKET, SO_SNDBUF, (char*)&SendBufferSize, sizeof(int));

	if(!AllowSendBlock)
	{
	#ifdef _WIN32
		u_long arg = 1;
		if(-1 == ioctlsocket(Socket, FIONBIO, &arg))
			geterror();
	#else
		fcntl(Socket, F_SETFL, O_NONBLOCK);
	#endif
	}
}

void Clientclass::Internal_ZipPacket(Packet &paket, const char* buffer, unsigned size, short Compression, bool lock)
{
	if(lock) Critic.Lock();

	unsigned		neededsize	= size * 120 / 100 + 20 + PAKETSIZE;
	NetworkBuffer &SendBuffer	= paket.Buffer;

	deflateParams(Dstream, Compression, Z_DEFAULT_STRATEGY);

	CheckAndDoResize(SendBuffer, neededsize, false);
	SendBuffer.SetWriteOffset(PAKETSIZE);

	//der Zip-Vorgang
	Dstream->next_out	= (Bytef*)	SendBuffer.GetDataFromWOffset();
	Dstream->avail_out	= (uInt)	SendBuffer.GetUnusedSize();
	Dstream->next_in	= (Bytef*)	buffer;
	Dstream->avail_in	= (uInt)	size;

	int ret = deflate(Dstream, Z_FINISH);
	if(Z_STREAM_END != ret)
		HandleErrorMessage("Message from syncsys: deflate-call failed");

	paket.UnZippedSize	= static_cast<unsigned int>(size);
	paket.ZippedSize	= Dstream->total_out;

	if(Z_OK != deflateReset(Dstream))
		HandleErrorMessage("Message from syncsys: deflateReset-call failed");

	paket.WriteHeader();

	if(lock) Critic.Unlock();
}

void Clientclass::Internal_SendZippedPacket(const Packet &paket, bool lock)
{
	if(state != STARTED)		return;

	if(lock) Critic.Lock();

	if(-1 == send(Socket, paket.Buffer.GetData(), PAKETSIZE + paket.ZippedSize, 0))
	{
		geterror();
		state = FAILED;
	}

	NumSendPackets++;
	SizeSendPackets += PAKETSIZE + paket.ZippedSize;

	if(lock) Critic.Unlock();
}

void Clientclass::Send(const char* daten, unsigned int size, unsigned short MSG, short Compression)
{
	if(state != STARTED)		return;

	Critic.Lock();
	SendPacket.MSG = MSG;
	Internal_ZipPacket(SendPacket, daten, size, Compression, false);
	Internal_SendZippedPacket(SendPacket, false);
	Critic.Unlock();
}

void Clientclass::ClientCallback(void)
{
	int rl = recv(Socket, const_cast<char*>(RecvBuffer.GetDataFromWOffset()), (int)RecvBuffer.GetUnusedSize(), 0);
	if(rl == -1 || rl == 0)// occur an error?
	{
		state = FAILED;
		geterror();
		return;
	}

	RecvBuffer.SetWriteOffset(rl + RecvBuffer.GetWriteOffset());

	while(RecvBuffer.GetWriteOffset() >= PAKETSIZE)
	{
		if(!ParsedHeader)
		{
			ParsedHeader = true;

			//parse data into header
			RecvPacket.ReadHeader(RecvBuffer);

			//is the packet correct?
			if(!IsHeaderOkay(RecvPacket))
			{
				HandleErrorMessage("Message from syncsys: Header is corrupted");
				state = FAILED;
				return;
			}

			unsigned CompleteSize = RecvPacket.ZippedSize + PAKETSIZE;
			if(CompleteSize < RecvBuffer.GetWriteOffset())	CompleteSize = RecvBuffer.GetWriteOffset();
			CheckAndDoResize(RecvBuffer, CompleteSize, true);
		}
		unsigned int CompleteSize = RecvPacket.ZippedSize + PAKETSIZE;
		//are there enough buffered data?
		if(RecvBuffer.GetWriteOffset() >= CompleteSize)
		{
			CheckAndDoResize(RecvPacket.Buffer, RecvPacket.UnZippedSize, false);

			//deflate
			Istream->next_in	= (Bytef*)	RecvBuffer.GetDataFromROffset();
			Istream->avail_in	= (uInt)	RecvPacket.ZippedSize;
			Istream->next_out	= (Bytef*)	RecvPacket.Buffer.GetData();
			Istream->avail_out	= (uInt)	RecvPacket.Buffer.GetBufferSize();

			int ret = inflate(Istream, Z_FINISH);
			if(Z_STREAM_END != ret)
			{
				std::stringstream stream;
				stream << "Message from syncsys: Data are corrupted. ZLib Errorcode: " << ret;
				HandleErrorMessage(stream.str().c_str());
				state = FAILED;
				return;
			}
			if(Z_OK !=inflateReset(Istream))
			{
				HandleErrorMessage("Message from syncsys: Could not reset inflatestream.");
				state = FAILED;
				return;
			}

			RecvBuffer.SetReadOffset(RecvBuffer.GetReadOffset() + RecvPacket.ZippedSize);
			RecvPacket.Buffer.SetReadOffset(0);
			RecvPacket.Buffer.SetWriteOffset(RecvPacket.UnZippedSize);

			if(RecvPacket.MSG == CLOSEMSG)
			{
				Send(RecvPacket);
				if(0 != closesocket(Socket))
					geterror();
				Socket = (SOCKET)-1;
				state = CLOSED;
			}
			else if(!AsyncCallbackFunc(RecvPacket))
				SyncList.push_back(RecvPacket, true);

			//reset RecvLen + Buffer, cleaning up etc.
			NumRecvPackets++;
			SizeRecvPackets	+= CompleteSize;

			RecvBuffer.ShiftBuffer(true, CompleteSize);
			ParsedHeader	= false;
		}
		else
			break;//lets go away
	}
}

//////////////////////////////////////////////////////////////////////
// Clients
//////////////////////////////////////////////////////////////////////
BasicServerClient::BasicServerClient(InitData &info, unsigned int Interval, bool block, bool disable, int size)
: Clientclass(block, disable, size), LastUpdate(0), RefCounter(1), Address(info.Address), Host(info.Host), UpdateInterval(Interval)
{
	Socket	= info.socket;
	state	= STARTED;

	Internal_CheckSocketSettings();
}

BasicServerClient::~BasicServerClient()
{

}

//////////////////////////////////////////////////////////////////////
// Client
//////////////////////////////////////////////////////////////////////
BasicClient::BasicClient(bool block, bool disable, int size)
	: Clientclass(block, disable, size)
{
}

BasicClient::~BasicClient()
{
	CloseConnection(false);
}

void BasicClient::ThreadUpdate(unsigned acttime)
{
	if(state == CONNECTING && !tryconnect())
		return;

	TIMEVAL		time;
	fd_set		readset;

	time.tv_sec = 0;
	time.tv_usec = 1000;

	FD_ZERO(&readset);
	FD_SET(Socket, &readset);

	if(-1 == select(static_cast<int>(Socket + 1), &readset, NULL, NULL, &time))
	{
		geterror();//EDIT: has to be logged
		state = FAILED;
	}

	if(FD_ISSET(Socket, &readset))
		ClientCallback();
}

bool BasicClient::tryconnect(void)
{
	if(0 != connect(Socket, (sockaddr*)&ConnectionAddress, sizeof(sockaddr)))
	{
		geterror();
		return false;
	}
	else
	{
		Internal_CheckSocketSettings();
		PortableSleep(1000);
		state = STARTED;
		return true;
	}
}

bool BasicClient::connectto(sockaddr_in Address, bool blocking)
{
	if(state != INITEDSOCKET && !disconnect())
		return false;

	ConnectionAddress = Address;

	sockaddr_in eigeneAddy;

	eigeneAddy.sin_port			= 0;// random Port
	eigeneAddy.sin_family		= AF_INET;
	eigeneAddy.sin_addr.s_addr	= INADDR_ANY;

	if(-1 == bind(Socket, (sockaddr*)&eigeneAddy, sizeof(sockaddr_in)))
	{
		state = FAILED;
		geterror();
		return false;
	}

	state = CONNECTING;
	if(blocking && !tryconnect())
		return false;

	StartThread();
	return true;
}

bool BasicClient::connectto(const char* Name, int Port, bool blocking)
{
	addrinfo* p;
	addrinfo hink;
	memset(&hink, 0, sizeof(addrinfo));

	hink.ai_family = AF_INET ;
	hink.ai_socktype = SOCK_STREAM;
	hink.ai_flags = 0;

	std::stringstream str;
	str << Port;
	if(0 != getaddrinfo(Name, str.str().c_str(), &hink, &p))
	{
		state = FAILED;
		geterror();
		return false;
	}

	sockaddr_in Address = *((sockaddr_in*)p->ai_addr);
	freeaddrinfo(p);

	return connectto(Address, blocking);
}

bool BasicClient::disconnect(void)
{
	CloseConnection(false);
	for(unsigned counter = 0; counter < 10 && state == CLOSING; counter++)
		helpers::PortableSleep(1000);

	StopThread();

	return netlib::INITEDSOCKET == InitSocket();
}
#endif
