#ifndef __TKNETWORK__
#define __TKNETWORK__

class TKNetwork;

#include "TKCore.h"
#include <iostream>
#include <enet/enet.h>
#include <pthread.h>
#include <string.h>
#include <vector>



using namespace std;

class TKNetwork
{
public:
    TKNetwork();
    ~TKNetwork();
    void connectToServer(std::string ip, int port);
    void createClient();
    void createServer(int port);
    void destroyServer();
    void disconnectClient();
    bool isServer();
    void stopListen();
    void bufferMessage(std::string message);
    void sendMessage(std::string message);

    void setTKCore(TKCore* ntkCore);

private:
    ENetAddress address;
    ENetHost* server;///TODO: remover server e client, substituir por host
    ENetHost* client;
    ENetPeer *peer;

    bool isserver;
    bool listen;

    vector<std::string> messagesToSend;

    static void* startListen(void*);
    void serverListen();
    void sendBufferedMessages();

    TKCore* tkCore;

    int networkNullMessage;
};

#endif

