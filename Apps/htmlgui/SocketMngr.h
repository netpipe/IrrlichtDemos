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

#ifndef _SOCKET_MANAGER_
#define _SOCKET_MANAGER_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
//"in" per "sockaddr_in"
#include <netinet/in.h>
#include <arpa/inet.h>
//"fcntl" per la funzione "fcntl"
#include <fcntl.h>
#include <unistd.h>


//#ifdef __cplusplus
//extern "C" {
//#endif
void InitSockMnger(unsigned int port);
int WriteSocketLen(char * cData, int len);
int WriteSocket(char * cData);
int ReadSocket(char *cGetData,unsigned int Len);
void CloseConnection(void);
void CloseTransaction(void);

//#ifdef __cplusplus
//}
//#endif


#endif
