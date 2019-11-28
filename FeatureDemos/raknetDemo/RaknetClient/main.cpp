//Raknet Client
#include <iostream>
#include <stdio.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "FileTransferControl.h"
#include "FileTransferProgress.h"
#include "IncrementalReadInterface.h"
#define MAX_CLIENTS 10
#define SERVER_PORT 60000

int main(void)
{
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isRunning = true;
	RakNet::Packet *packet;
    RakNet::FileListTransfer flt1;
    FileTransferControl ftc;
	peer->Startup(1,&RakNet::SocketDescriptor(), 1);
    printf("Enter server IP or hit enter for 127.0.0.1\n");
//	gets(str);
//	if (str[0]==0){
		strcpy(str, "127.0.0.1");
//	}

    peer->Connect(str, SERVER_PORT, 0,0);
	peer->AttachPlugin(&flt1);
	peer->SetSplitMessageProgressInterval(9);
    flt1.SetCallback(&testFileListProgress);

	printf("Starting the client.\n");
    while (isRunning)
	{
		for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
		{
			switch (packet->data[0])
            {
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					printf("Another client has disconnected.\n");
					break;
				case ID_REMOTE_CONNECTION_LOST:
					printf("Another client has lost the connection.\n");
					break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					printf("Another client has connected.\n");
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					printf("Our connection request has been accepted.\n");

					flt1.SetupReceive(&ftc, false, packet->systemAddress);
					break;
				case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n");
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("The server is full.\n");
					break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("We have been disconnected.\n");

					break;
				case ID_CONNECTION_LOST:
                    printf("Connection lost.\n");

					break;
				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
            }
		}
	}
	// TODO - Add code body here
    std::cout<<"Done!";
	RakNet::RakPeerInterface::DestroyInstance(peer);
    std::cin.get();
	return 0;
}
