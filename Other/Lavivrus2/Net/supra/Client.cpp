#include "Client.h"
#include "../GUI/Console.h"
#include "../CustomFunctions.h"
#include "../Azadi.h"

using GUI::Console;

union EncData
{
    unsigned long int word;
    char data[8];
};

//! Client constructor
Client::Client()
{

}

//! Client destructor
Client::~Client()
{

}

//! Connects to the master server
bool Client::Connect()
{
    return false;
}

//! Connects to the specified server
bool Client::Connect(const std::string& server, unsigned int port)
{
    return true;
}

//! Disconnects from the master server
bool Client::Disconnect()
{
    return true;
}

bool Client::Pushdata(int direction, void *buffer, unsigned int size)
{
    if (encrypting)
    {
        for (unsigned int i = 0; i < size; i+= 16)
        {
            EncData temp;
            EncData temp2;
            char *p = (char *)buffer;
            memcpy(temp.data, p, 8);
            p+=8;
            memcpy(temp2.data, p, 8);
            p-=8;
            crypto.encipher(&temp.word, &temp2.word);
            memcpy(p, temp.data, 8);
            p+=8;
            memcpy(p, temp2.data, 8);
        }
    }
    return true;
}

//! Receive data from the wire
int Client::Receivedata(int direction, void *buffer)
{
    return 0;
}
