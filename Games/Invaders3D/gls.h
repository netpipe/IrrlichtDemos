ifndef __GLS_H__
#define __GLS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <winsock2.h>
#include <list>

//using namespace System::Net::Sockets;
using namespace std;
static const unsigned METHOD_GET  = 1;
static const unsigned METHOD_POST = 2;
const     string   HighScore_Auth = "l0S3eG9zkQ2Ghv";
const     string   host = "g0dsoft.com";
const     string   path = "/gls/gls_breakout.php";  
const     int      port = 80;

class Gls {
      public:
          Gls();
          void SendPoGdata(unsigned method,string name,unsigned value);            
      private:  
          bool init;
          SOCKET *sock;    
          list<string> lst;
          void SendString(string str);
          void ClearList();
          WSAData wsaData;
          u_long LookupAddress(const char* pcHost);
          SOCKET* EstablishConnection(u_long nRemoteAddr, u_short nPort);
};

#endif

