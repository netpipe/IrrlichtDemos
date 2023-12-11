#include "TKNetwork.h"

///Constructor, also initializes eNet.
TKNetwork::TKNetwork()
{
    if (enet_initialize () != 0)
    {
        cout << "An error occurred while initializing ENet." << endl;
    }
    atexit (enet_deinitialize);

    isserver=false;
    client = NULL;
    server = NULL;
}

///destructor, destroy all entities created at runtime
TKNetwork::~TKNetwork()
{
    if(this->isServer())
    {
        enet_host_destroy(server);
        server = NULL;
        stopListen();
    }
    else
    {
        enet_host_destroy(client);
        client = NULL;
    }
}

void TKNetwork::bufferMessage(string message)
{
    if(messagesToSend.size() > 10)
        cout << "BUFFER FULL!" << endl;
    else
        messagesToSend.push_back(message);
}

void TKNetwork::connectToServer(string ip, int port)
{
    ENetEvent event;

    enet_address_set_host (& address, ip.c_str());
    address.port = port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect (client, & address, 2,0);

    if (peer == NULL)
    {
       cout << "No available peers for initiating an ENet connection." << endl;
       exit (EXIT_FAILURE);
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service (client, & event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        cout << "Connection to server succeeded." << endl;

        pthread_t* thread;
        thread=(pthread_t *)malloc(sizeof(*thread));

        pthread_create(thread,
            NULL,
            startListen,
            (void*)this);
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset (peer);

        cout << "Connection to server failed." << endl;
    }

}

void TKNetwork::createClient()
{
    client = enet_host_create (NULL /* create a client host */,
                1 /* only allow 1 outgoing connection */,
                57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
                14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */,0);

    if (client == NULL)
    {
        cout << "An error occurred while trying to create an ENet client host." << endl;
        exit (EXIT_FAILURE);
    }
    else
    {
        cout << "Client created" << endl;
        isserver=false;
    }
}

///Creates a new server at the specified port
void TKNetwork::createServer(int port)
{
    address.host = ENET_HOST_ANY;
    address.port = port;

    server = enet_host_create (& address, // the address to bind the server host to
                                  1,      // allow up to 1 clients and/or outgoing connections
                                  0,      // assume any amount of incoming bandwidth
                                  0,0);     // assume any amount of outgoing bandwidth


    if (server == NULL)
    {
        cout << "An error occurred while trying to create an ENet server host." << endl;
        exit (EXIT_FAILURE);
    }
    else
    {
        cout << "Server started at port:" << port << endl;
        isserver=true;
    }

    pthread_t* thread;
    thread=(pthread_t *)malloc(sizeof(*thread));

    pthread_create(thread,
        NULL,
        startListen,
        (void*)this);
}

///destroy the server
void TKNetwork::destroyServer()
{
    if(server!=NULL) enet_host_destroy(server);
    server = NULL;
}

void TKNetwork::disconnectClient()
{
    ENetEvent event;

    enet_peer_disconnect (peer, 0);

    // Allow up to 3 seconds for the disconnect to succeed and drop any packets received packets.
    while (enet_host_service (client, & event, 3000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy (event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            cout << "Disconnection succeeded." << endl;
            return;
        }
    }

    /* We've arrived here, so the disconnect attempt didn't */
    /* succeed yet.  Force the connection down.             */
    enet_peer_reset (peer);
}

///returns true if i'm the server and false if client
bool TKNetwork::isServer()
{
    return isserver;
}

void TKNetwork::sendMessage(string message)
{
    ENetPacket * packet = enet_packet_create (message.c_str(),
                                              strlen((char*)message.c_str()) + 1,
                                              ENET_PACKET_FLAG_RELIABLE);

    if(isServer())
    {
        enet_host_broadcast (server, 0, packet);
        enet_host_flush(server);
    }
    else
    {

        enet_peer_send (peer, 0, packet);
        enet_host_flush(client);
    }
}

void* TKNetwork::startListen(void* pthis)
{
    TKNetwork* tkNetwork=(TKNetwork*)pthis;
    tkNetwork->serverListen();
}

void TKNetwork::stopListen()
{
    listen=false;
}

void TKNetwork::serverListen()
{
    ENetEvent event;

    cout << "Start Listen" << endl;
    listen=true;

    ENetPacket* nullPacket;

    ENetHost* host=NULL;

    if(isServer())
        host=server;
    else
        host=client;

    string msg="";

    while (enet_host_service (host, & event, 100) > 0 || listen)
    {
        //cout << "Event:" << event.type << endl;
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            cout << "A new client connected from" << event.peer -> address.host << ":" << event.peer -> address.port << endl;

            /* Store any relevant client information here. */
            event.peer -> data = (void*)"Client information";

            break;

        case ENET_EVENT_TYPE_RECEIVE:
            //cout << "A packet of length "<< event.packet -> dataLength << " containing "<< event.packet -> data <<" was received from "<< event.peer -> data <<" on channel " << event.channelID << endl;

            msg="";
            msg+=(char*)event.packet->data;

            if(msg!="")
            {
                if(isServer()) bufferMessage(msg);
                cout << "RECV:" << event.packet->data << endl;
            }

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy (event.packet);

            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            cout << event.peer -> data << " disconected.\n" <<endl;

            /* Reset the peer's client information. */
            event.peer -> data = NULL;
        }

        sendBufferedMessages();
    }

    cout << "Stop Listen" << endl;
}

void TKNetwork::sendBufferedMessages()
{
    if(messagesToSend.size() > 0)
    {
        for(int i=0;i<messagesToSend.size();i++)
        {
            sendMessage(messagesToSend[i]);
            //sleep(1);
        }
        messagesToSend.clear();
    }
    else
    {
        //send a null packet to keep connection up avoiding timeout
        sendMessage("");
        //sleep(1);
    }
}


