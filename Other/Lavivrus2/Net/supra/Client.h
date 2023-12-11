#ifndef CLIENT_H
#define CLIENT_H

#include "SupraSocket.h"
#include "../Encryption/Blowfish.h"
#include <sys/time.h>

enum direction
{
    CHAT,
    GAME,
    AUTH,
    HEART
};

//! Class that contains the functions to connect to the master server
class Client
{
	public:
		Client();
		~Client();
		bool Connect();
		bool Connect(const std::string&, unsigned int);
		bool Disconnect();
        bool Pushdata(int direction, void*, unsigned int size);
		int Receivedata(int direction, void*);
    private:
        Encryption::Blowfish crypto;
        bool encrypting;
};

#endif

