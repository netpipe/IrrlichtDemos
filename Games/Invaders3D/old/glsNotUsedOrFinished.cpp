#include "gls.h"

void Gls::SendString(string str)
{
    send(*sock,str.c_str(),strlen(str.c_str()),0);
    //ToDo: All sendstrings should be logged!
    ///////////////////////////////////////////
}

Gls::Gls(){
 //   sock = socket (AF_INET, SOCK_STREAM, 0);
     
   // WSAStartup (0x0101, &WsaData);							// Init Winsock
    WSAStartup (MAKEWORD(1, 1), &wsaData);

    u_long nRemoteAddress = LookupAddress(host.c_str());
    //sock = new SOCKET;
    sock = EstablishConnection(nRemoteAddress, htons(port));
	if (*sock == INVALID_SOCKET)
        init = false;
    else
        init = true; 
}
void Gls::SendPoGdata(unsigned method,string name, unsigned value){
     switch(method){
         case  METHOD_POST:{
                string str = "GET "+path+"?";
                str = str + "&action=addhigh";
                str = str + "&auth="+HighScore_Auth;
                str = str + "&user="+name+"&";
               // string score = value;
                str = str + "&score=";
                str += value;
                str = str + " HTTP/1.0";
                SendString(str);
                SendString("HOST: "+host);
                SendString("");
         } 
         case METHOD_GET:{
              string combined = "action=viewscores&";
              string str = "POST "+path+" HTTP/1.0";
              SendString(str);
              SendString("Accept: */*");
              SendString("Host: "+host);
              SendString("Content-type: application/x-www-form-urlencoded");
              SendString("User-agent: blitzconn");
              SendString("Content-length: " + strlen(combined.c_str()));
              SendString("Pragma: no-cache");
              SendString("Connection: keep-alive");
              SendString("");
              SendString(combined);
              SendString("");
             // string combined;
             // fo 
             }
         
     }  
     //clean stream
     string returns;//= //ReadLine;
     while(returns != ""){
                   
        //  returns = //readline;
     }
     return;
}

//// LookupAddress /////////////////////////////////////////////////////
// Given an address string, determine if it's a dotted-quad IP address
// or a domain address.  If the latter, ask DNS to resolve it.  In
// either case, return resolved IP address.  If we fail, we return
// INADDR_NONE.

u_long Gls::LookupAddress(const char* pcHost)
{
    u_long nRemoteAddr = inet_addr(pcHost);
    if (nRemoteAddr == INADDR_NONE) {
        // pcHost isn't a dotted IP, so resolve it through DNS
        hostent* pHE = gethostbyname(pcHost);
        if (pHE == 0) {
            return INADDR_NONE;
        }
        nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
    }

    return nRemoteAddr;
}

SOCKET* Gls::EstablishConnection(u_long nRemoteAddr, u_short nPort)
{
    // Create a stream socket
    SOCKET* sd = new SOCKET;
    *sd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sd != INVALID_SOCKET) {
        sockaddr_in sinRemote;
        sinRemote.sin_family = AF_INET;
        sinRemote.sin_addr.s_addr = nRemoteAddr;
        sinRemote.sin_port = nPort;
        if (connect(*sd, (sockaddr*)&sinRemote, sizeof(sockaddr_in)) ==
                SOCKET_ERROR) {
            *sd = INVALID_SOCKET;
        }
    }

    return sd;
}

