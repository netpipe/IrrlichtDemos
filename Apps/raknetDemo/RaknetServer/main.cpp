//Raknet Server
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "IncrementalReadInterface.h"
#include "FileListTransfer.h"
#include "FileOperations.h"
#include "FileTransferProgress.h"

#include "RakSleep.h"
#define MAX_CLIENTS 10
#define SERVER_PORT 60000

int main(void)
{
    RakNet::Packet *packet;
    RakNet::RakString file;
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isRunning = true;
    peer->Startup(MAX_CLIENTS, &RakNet::SocketDescriptor(SERVER_PORT,0), 1);
    RakNet::FileListTransfer flt1;
	std::cout<<"Started Server\n";
	peer->AttachPlugin(&flt1);
	peer->SetSplitMessageProgressInterval(9);
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	flt1.SetCallback(&testFileListProgress);
    RakNet::FileList fileList;
    RakNet::IncrementalReadInterface incrementalReadInterface;
    std::cout<<"Insert path of file to transfer\n";
    char str[256];
 //   gets(str);
//	if (str[0]==0)
		strcpy(str, "./test1.mp3");

    file=str;

    unsigned int fileLength = GetFileLength(file.C_String());

    if (fileLength==0)
	{
		printf("Test file %s not found.\n", file.C_String());

		std::cout<<"Shutting Down\n";
		RakNet::RakPeerInterface::DestroyInstance(peer);
        std::cin.get();
		return 1;
	}

    fileList.AddFile(file.C_String(), file.C_String(), 0 , fileLength, fileLength, FileListNodeContext(0,0), true);
    printf("File Added.\n");
    RakSleep(100);

    while(isRunning)
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

					break;
				case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n");

					flt1.Send(&fileList,peer,packet->systemAddress,0,HIGH_PRIORITY,0,&incrementalReadInterface,2000000);

					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					printf("The server is full.\n");

					break;
				case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");

					break;
				case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
                    isRunning = false;
					break;
				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);

					break;
            }
		}
    }
	// TODO - Add code body here
    std::cout<<"Server has stopped.";
	RakNet::RakPeerInterface::DestroyInstance(peer);
    std::cin.get();
	return 0;
}
