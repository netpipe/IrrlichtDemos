#ifndef NetClient_H
#define NetClient_H
#include <stdlib.h>
#include <stdio.h>

#include <enet/enet.h>

#include <string.h>
using namespace std;

class netClient
{
    public:

    netClient();
    ~netClient();
    void ClientLoop();
    void SendPacket(int Channel, char pack[11]);
    void InitNetwork();

};

#endif
