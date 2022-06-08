#ifndef __SUPRASOCKET_H__
#define __SUPRASOCKET_H__

#include <cstdio>
#include <string>
#include <map>

#include <unistd.h>
#include <fcntl.h>

#ifdef WIN32
	#include <winsock.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#define SOCKET int
	#include <cstring>
	#include <errno.h>
#endif

enum encType
{
	NONE,
	BLOWFISH
};

#ifdef WIN32
class SupraSocket
#else
class SupraSocket
#endif
{
	public:
		SupraSocket();
		SupraSocket(int type);
		~SupraSocket();
		int Connect(char *, unsigned short);
		int Connect(char *, unsigned short, bool type);
		void Disconnect();
		int Send(void* data, int size);
		int Recv(void* data, int size, bool clearmem, bool peek);
		int Bind(unsigned int);
		int Listen(int);
		int Accept();
		int Settype(int);
		void SetEncKey(char*);
		void Setencryption(enum encType);
		int Descriptor();
	private:
		void Encrypt(char*);

		int sock;
		struct sockaddr_in host;
		struct sockaddr_in client;
		int encryptionType;
		int sockettype;
};

#endif
