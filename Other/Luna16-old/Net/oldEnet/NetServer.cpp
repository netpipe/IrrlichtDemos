#include <stdlib.h>
#include <stdio.h>

#include <enet/enet.h>

#define MAX_CLIENTS 3

#include <string.h>
using namespace std;

void ServerLoop();
void SendPacket(int Channel, char pack[11]);
void InitNetwork();


    ENetEvent event;
    ENetHost * host=0;
    ENetPeer * peer=0;
    ENetAddress address;


void ServerLoop(){
    
        // processing incoming events:
        while (enet_host_service (host, &event, 1000) > 0) {
            printf("Checking Incoming");
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                printf("  A new connected has been established to %u:%u\n", 
                    event.peer -> address.host,event.peer-> address.port);
                fflush(stdout);
                
                char buf[64];
                sprintf(buf,"%u:%u",event.peer -> address.host, event.peer -> address.port);
                int buflen=strlen(buf);
                event.peer -> data=malloc(buflen+1);
                strncpy((char*)event.peer -> data,buf,buflen);
                peer=event.peer;
                break;
            }
            case ENET_EVENT_TYPE_RECEIVE:
                
                    printf("%s says %s on channel %u\n",
                        (char*)event.peer -> data,event.packet -> data,event.channelID);
                    fflush(stdout);
                    
                enet_packet_destroy (event.packet); // clean up the packet now that we're done using it
                
                     SendPacket(0,"Data Received")               ;
                break;
               
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("  host disconnected.\n");
                fflush(stdout);
                free(event.peer -> data);
                event.peer -> data = 0; // reset the peer's client information.
                peer=0;
            default:
                break;
            }
        }
}     

void SendPacket(int Channel, char pack[11])
{
    /* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket * packet = enet_packet_create (pack, 
                                              strlen (pack) + 1, 
                                              ENET_PACKET_FLAG_RELIABLE);

    
    /* Send the packet to the peer over channel id 0. */
    /* One could also broadcast the packet by         */
    /* enet_host_broadcast (host, 0, packet);         */
    enet_peer_send (peer, Channel, packet);
    /* One could just use enet_host_service() instead. */
    enet_host_flush (host);
     
}
void InitNetwork(){
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
    }
    atexit (enet_deinitialize);
    
      printf("Initialized.\n");
    
    
        /* Bind the server to the default localhost.     */
        /* A specific host address can be specified by   */
        /* enet_address_set_host (& address, "x.x.x.x"); */
    
        address.port = 1234;
    
            printf("I am server...\n");
            fflush(stdout);
            
            address.host = ENET_HOST_ANY;
            host = enet_host_create (&address, // the address to bind the server host to 
                                          1,   // allow only 1 client and/or outgoing connections
                                          0,   // assume any amount of incoming bandwidth
                                          0);  // assume any amount of outgoing bandwidth
            if (!host) {
                fprintf(stderr,"An error occurred while trying to create an ENet server host.\n");
                exit (EXIT_FAILURE);
            }        
      printf("Host Created at %s.\n",address.host);


}

int main () {
        
InitNetwork();

int run=1;
    while (run==1){
      ServerLoop();
      
    }
}
//END_OF_MAIN();
