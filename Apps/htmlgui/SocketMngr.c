/*
    GUIHtml Version 1.0
    GUIHtml is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "SocketMngr.h"

static unsigned int SockPort= 30000;

char CheckConnection(void);
char InitConnection (unsigned int Port );


static char cInit =0;
static char cConnected =0;
static   int m_socket;
static   int AcceptSocket;
int error;

void InitSockMnger(unsigned int port){
        SockPort=port;
	return;
}

int ReadSocket(char *cGetData,unsigned int Len){
     #define SOCKET_ERROR -1

    int bytesRecv = 0;

    if(0 == cInit){
     cInit = InitConnection (SockPort );
    }
    if(1 == cInit && 0 == cConnected ){
        cConnected = CheckConnection();
    }

    if(cInit &&cConnected){

       if (( bytesRecv = recv( AcceptSocket, cGetData, Len,0 ))<0){
         printf("Can not read message\n");
         CloseTransaction();
      }
      if(bytesRecv > 0){
            cGetData[bytesRecv]=0;
            //*cGetData =   recvbuf;
            //printf("Server: %s \n",cGetData);
      }

    }
    return bytesRecv;
}


int WriteSocket(char * cData){
    int iRet =0;

     iRet = WriteSocketLen( cData, strlen(cData) );

    return iRet;
}

int WriteSocketLen(char * cData, int len){
    int iRet =0;

    if(cInit && cConnected ){
     iRet = send( AcceptSocket, cData, len,0 );
    }

    return iRet;
}

char  InitConnection (unsigned int Port ){

 int optVal =1;
 static struct sockaddr_in service;
  #define INVALID_SOCKET  -1
 
  m_socket = socket(AF_INET, SOCK_STREAM, 0);

  if ( m_socket == INVALID_SOCKET ) {
        printf( "Error at socket(): \n" );
        return  0;
    }

     

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr( "127.0.0.1" ); //INADDR_ANY; //
    service.sin_port = htons( Port );

     //Il socket will be not blocking
    //error =fcntl(m_socket,F_SETFL,O_NONBLOCK);
   
    optVal =1;
    if (setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&optVal,sizeof(optVal)) < 0) {
       printf("errror during setsockopt");
        close(m_socket);
        return 0;;
     }

    sleep(1);
    optVal = bind( m_socket, (struct sockaddr*) &service, sizeof(service) ) ;
    if ( SOCKET_ERROR == optVal ) {
        printf( "bind() failed.\n" );
        close(m_socket);
        return 0;
    }

    if ( listen( m_socket, 1 ) == SOCKET_ERROR ){ // listen(.. , 1)  max number of connection 
        printf( "Error listening on socket.\n");
        close(m_socket);
        return 0;
    }

    return 1;
}

char CheckConnection(void){
        char cRet =0;

        printf( "Waiting for a client to connect...\n" );
        AcceptSocket = SOCKET_ERROR;

        AcceptSocket = accept( m_socket, NULL, NULL );

        if ( AcceptSocket != SOCKET_ERROR ) {
            cRet =1;
        }
        printf( "Client Connected.\n");
        return cRet;
}


void CloseConnection(void){
        close(m_socket);
        close(AcceptSocket);
        cInit =0;
        cConnected =0;
 return;
}

void CloseTransaction(void){
        close(AcceptSocket);
        cConnected =0;
	return;
}


