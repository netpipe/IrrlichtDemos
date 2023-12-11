
/*******************************************************************************
* Game Net Lib - ���������� ��� ����������� �������� �������������� � �����.   *
* ������ 1.0                                                                   *
* ����� - ��������� �����                                                      *
* ��������������� ��������                                                     *
*******************************************************************************/

#define NL_UNIX
//#define NL_WINDOWS

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#if defined(NL_WINDOWS)
# include <string.h>
# include <winsock.h>
# pragma comment (lib, "ws2_32.Lib")
#elif defined(NL_UNIX)
# include <strings.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <errno.h>
#include <unistd.h>//??
#include <arpa/inet.h>//??
#endif



#include "GameNetLib.h"

////////////////////////////////////////////////////////////////////////////////
// ����������� �����

typedef unsigned char    ui_8;
typedef   signed char    si_8;
typedef unsigned short   ui_16;
typedef   signed short   si_16;
typedef unsigned long    ui_32;
typedef   signed long    si_32;

#ifndef INVALID_SOCKET
# define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
# define SOCKET_ERROR (-1)
#endif

#ifndef INADDR_NONE
# define INADDR_NONE ((ui_32)-1)
#endif

#ifndef INADDR_ANY
# define INADDR_ANY 0
#endif


#define SOCKET int //??
#define closesocket close//??

#if defined(NL_UNIXa)

#define SOCKET int

struct in_addr {
  union {
    struct { ui_8 s_b1,s_b2,s_b3,s_b4; } S_un_b;
    struct { ui_16 s_w1,s_w2; } S_un_w;
    ui_32 S_addr;
  } S_un;
};

struct sockaddr_in {
  si_16 sin_family;
  ui_16 sin_port;
  struct in_addr sin_addr;
  char  sin_zero[8];
};

#define closesocket close

#endif

struct TMessage{
  NLS_TInterpriterProcedure Sproc; // ��������� ��������� ��������� �� �������
  NLC_TInterpriterProcedure Cproc; // ��������� ��������� ��������� �� �������
  ui_8 *var;                       // ������������ ���������� ��������� ��������� ���������
  unsigned n;                      // ���������� ���������� ���������
};

////////////////////////////////////////////////////////////////////////////////
// ��������� ����������

// ����� ����������
int ErrorNumber = 0;                  // ����� ������ ���������� ��������
char *ErrorMessage = NULL;            // �������� ������ ���������� ��������
struct timeval null_time = {0, 0};    // ��������� �������� �������
union NL_Variable *variables=NULL;          // ������������ � ����������� ��������� ���������
ui_32 variables_number=0;             // ������������ ���������� ���������� � ���������

// ���������� ��������� �������
SOCKET *Descriptors_list = NULL;      // ������ ���� ������������
ui_32 Descriptors_number = 0;         // ���������� ������������
SOCKET mother_desc = INVALID_SOCKET;  // ��������� ����������
ui_32 max_players = 0;                // ������������ ���������� ����������
struct TMessage SMessages[256];              // ������������ ���������
NL_TClientConnect ClientConnect = NULL;       // ���������� ����������� �������
NL_TClientDisconnect ClientDisconnect = NULL; // ���������� ���������� �������

// ���������� ��������� �������
SOCKET Descriptor = INVALID_SOCKET;  // ���������� ���������� � ��������
struct TMessage CMessages[256];              // ������������ ���������

////////////////////////////////////////////////////////////////////////////////
// ��������� �������

int set_sendbuf(SOCKET s)
{
  int opt = NL_MAX_MESSAGE;
  if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *) &opt, sizeof(opt)) < 0)
    return 0;
  return 1;
}

#if defined(NL_WINDOWS)

void nonblock(SOCKET s)
{
  unsigned long val = 1;
  ioctlsocket(s, FIONBIO, &val);
}

#elif defined(NL_UNIX)

#ifndef O_NONBLOCK
#define O_NONBLOCK O_NDELAY
#endif

void nonblock(SOCKET s)
{
  int flags;
  flags = fcntl(s, F_GETFL, 0);
  flags |= O_NONBLOCK;
}

#endif

void Error(int num, char *msg, ...)
{
  char buf[1024];
  va_list args;

  ErrorNumber = num;
  if(ErrorMessage)
    free(ErrorMessage);

  va_start(args, msg);
  vsprintf(buf, msg, args);
  va_end(args);

  ErrorMessage = strdup(msg);
}

#if defined(NL_WINDOWS)

// �������� ����, ��� ��������� � ������������ ������
int PerformWrite(SOCKET desc, void *buf, ui_32 len)
{
  int bytes_written;
  ui_32 write_total = 0;

  while (write_total < len) {
    bytes_written = send(desc, (char *)buf + write_total, len - write_total, 0);

    if (!bytes_written)
      return 0;

    if (bytes_written < 0){
      if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR){
        memcpy(buf, (char *)buf + len, len - write_total);
        len -= write_total;
        return 1;
      }
      return 0;
    }
    write_total += bytes_written;
  }
  return 1;
}

#elif defined(NL_UNIX)

int PerformWrite(SOCKET desc, void *buf, ui_32 len)
{
  int bytes_written;
  ui_32 write_total = 0;

  while (write_total < len) {
    bytes_written = send(desc, (char *)buf + write_total, len - write_total, 0);

    if (!bytes_written)
      return 0;

    if (bytes_written < 0){
      if (errno == EWOULDBLOCK){
        memcpy(buf, (char *)buf + len, len - write_total);
        len -= write_total;
        return 1;
      }
      return 0;
    }
    write_total += bytes_written;
  }
  return 1;
}

#endif

// ���������� ���������� �������, ������������� ��� id � �������� �� ���� ����������.
void AddClient(SOCKET desc, char *host)
{
  ui_32 i;

  for(i = 0; i<Descriptors_number; i++)
    if(Descriptors_list[i] == INVALID_SOCKET){
      Descriptors_list[i] = desc;
      if(ClientConnect)
        (*ClientConnect)(i, host);
      return;
    }

  if(Descriptors_number >= max_players){
    closesocket(desc);
    return;
  }

  Descriptors_list[Descriptors_number] = desc;

  if(ClientConnect){
    if(!(*ClientConnect)(Descriptors_number, host)){ // ������ ������ ����� ����������
      closesocket(desc);
      return;
    }
  }

  Descriptors_number++;
}

// ��������� ���������� � ������ id � �������� �� ���� ����������.
void DelClient(int id)
{
  if((ui_32)id >= Descriptors_number)
    return;
  if(Descriptors_list[id] == INVALID_SOCKET)
    return;
  closesocket(Descriptors_list[id]);
  Descriptors_list[id] = INVALID_SOCKET;
  if(ClientDisconnect)
    (*ClientDisconnect)(id);
}

// �������� ����������� ������ �����������
void AcceptClient()
{
  SOCKET desc;
  unsigned i;
  struct sockaddr_in peer;
  char from[16]; //255.255.255.255

  i = sizeof(peer);
  desc = accept(mother_desc, (struct sockaddr *) &peer, (int*)&i);
  if (desc == INVALID_SOCKET)
    return;

  nonblock(desc);

  if (!set_sendbuf(desc)) {
    closesocket(desc);
    return;
  }

  sprintf(from, "%d.%d.%d.%d", peer.sin_addr.S_un.S_un_b.s_b1, peer.sin_addr.S_un.S_un_b.s_b2, peer.sin_addr.S_un.S_un_b.s_b3, peer.sin_addr.S_un.S_un_b.s_b4);
  AddClient(desc, from);
}

// ������ �� �����������
si_32 ReadFromSocket(SOCKET desc, void *buf, ui_32 size)
{
  si_32 bytes_read;
  fd_set input_set;

  FD_ZERO(&input_set);
  FD_SET(desc, &input_set);
#if defined(NL_WINDOWS)
  select(0, &input_set, 0, 0, &null_time);
#elif defined(NL_UNIX)
  select(desc+1, &input_set, 0, 0, &null_time);
#endif
  if(!FD_ISSET(desc, &input_set))
    return 0;

  bytes_read = recv(desc, (char *)buf, size, 0);
  if (bytes_read == 0)
    return 0;
  if (bytes_read < 0)
#if defined(NL_WINDOWS)
    if(WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
#elif defined(NL_UNIX)
    if(errno == EWOULDBLOCK)
#endif
      return 0;
    else
      return -1;

  return bytes_read;
}

// ������� ��� �� �������� ������, ��� ����� ������� ���
void ReadClear(SOCKET desc)
{
  si_32 bytes_read;
  char buf[256];
  do{
    bytes_read = ReadFromSocket(desc, buf, 250);
  }while(bytes_read > 0);
}

// ��������� �������� ������ �� ������� � ������������ ��
int CPerformRead(int id)
{
  SOCKET desc;
  si_32 bytes_read;
  ui_8 buf_data[NL_MAX_MESSAGE]; // ����� ��� ������ ������. � ���� �� ����������� ������� ������, �������� ������
  ui_8 *buf = buf_data;
  ui_8 msg;
  ui_32 i;
  ui_16 len;

  desc = Descriptors_list[id];

  for(;;) {
    bytes_read = ReadFromSocket(desc, buf, 1);
    if(!bytes_read) // ��������� ����������
      return 1;

    if(bytes_read < 0) // ������
      return 0;

    msg = buf[0];

    if(!CMessages[msg].Cproc){ // ������ ������� �� ����������, � �����
      ReadClear(desc);
      Error(NL_ERROR_UNKNOWN_MESSAGE, "����������� ��������� %d", msg);
      return 1;
    }

    for(i = 0; i < CMessages[msg].n; i++){
      switch(CMessages[msg].var[i]){
        case NL_UI_8:
          bytes_read = ReadFromSocket(desc, &(variables[i].ui_8), 1);
          if(bytes_read != 1) goto Err;
          break;
        case NL_SI_8:
          bytes_read = ReadFromSocket(desc, &(variables[i].si_8), 1);
          if(bytes_read != 1) goto Err;
          break;
        case NL_UI_16:
          bytes_read = ReadFromSocket(desc, &(variables[i].ui_16), 2);
          if(bytes_read != 2) goto Err;
          break;
        case NL_SI_16:
          bytes_read = ReadFromSocket(desc, &(variables[i].si_16), 2);
          if(bytes_read != 2) goto Err;
          break;
        case NL_UI_32:
          bytes_read = ReadFromSocket(desc, &(variables[i].ui_32), 4);
          if(bytes_read != 4) goto Err;
          break;
        case NL_SI_32:
          bytes_read = ReadFromSocket(desc, &(variables[i].si_32), 4);
          if(bytes_read != 4) goto Err;
          break;
        case NL_STR8:
          bytes_read = ReadFromSocket(desc, buf, 1);
          if(bytes_read != 1) goto Err;
          len = buf[0];
          bytes_read = ReadFromSocket(desc, buf, len);
          if(bytes_read != len) goto Err;
          variables[i].str = (char *)buf;
          buf+=len;
          *buf=0;
          buf++;
          break;
        case NL_STR16:
          bytes_read = ReadFromSocket(desc, buf, 2);
          if(bytes_read != 2) goto Err;
          len = ((ui_16 *)buf)[0];
          bytes_read = ReadFromSocket(desc, buf, len);
          if(bytes_read != len) goto Err;
          variables[i].str = (char *)buf;
          buf+=len;
          *buf=0;
          buf++;
          break;
        case NL_BIN8:
          bytes_read = ReadFromSocket(desc, buf, 1);
          if(bytes_read != 1) goto Err;
          len = buf[0];
          bytes_read = ReadFromSocket(desc, buf, len);
          if(bytes_read != len) goto Err;
          variables[i].bin.data = buf;
          variables[i].bin.length = len;
          buf+=len;
          break;
        case NL_BIN16:
          bytes_read = ReadFromSocket(desc, buf, 2);
          if(bytes_read != 2) goto Err;
          len = ((ui_16 *)buf)[0];
          bytes_read = ReadFromSocket(desc, buf, len);
          if(bytes_read != len) goto Err;
          variables[i].bin.data = buf;
          variables[i].bin.length = len;
          buf+=len;
          break;
      }
    }

    CMessages[msg].Cproc(id, variables);
  }
Err:
  ReadClear(desc);
  Error(NL_ERROR_INCORRECT_MESSAGE, "������������ ��������� %d", msg);
  return 1;
}

// ��������� �������� ������ �� ������� � ������������ ��
int SPerformRead()
{
  si_32 bytes_read;
  ui_8 buf_data[NL_MAX_MESSAGE]; // ����� ��� ������ ������.
  ui_8 *buf = buf_data;
  ui_8 msg;
  ui_32 i;
  ui_16 len;

  for(;;) {
    bytes_read = ReadFromSocket(Descriptor, buf, 1);
    if(!bytes_read) // ��������� ����������
      return 1;

    if(bytes_read < 0) // ������
      return 0;

    msg = buf[0];

    if(!SMessages[msg].Sproc){ // ������ ������� �� ����������, � �����
      ReadClear(Descriptor);
      Error(NL_ERROR_UNKNOWN_MESSAGE, "����������� ��������� %d", msg);
      return 1;
    }

    for(i = 0; i < SMessages[msg].n; i++){
      switch(SMessages[msg].var[i]){
        case NL_UI_8:
          bytes_read = ReadFromSocket(Descriptor, &(variables[i].ui_8), 1);
          if(bytes_read != 1) goto Err;
          break;
        case NL_SI_8:
          bytes_read = ReadFromSocket(Descriptor, &(variables[i].si_8), 1);
          if(bytes_read != 1) goto Err;
          break;
        case NL_UI_16:
          bytes_read = ReadFromSocket(Descriptor, &(variables[i].ui_16), 2);
          if(bytes_read != 2) goto Err;
          break;
        case NL_SI_16:
          bytes_read = ReadFromSocket(Descriptor, &(variables[i].si_16), 2);
          if(bytes_read != 2) goto Err;
          break;
        case NL_UI_32:
          bytes_read = ReadFromSocket(Descriptor, &(variables[i].ui_32), 4);
          if(bytes_read != 4) goto Err;
          break;
        case NL_SI_32:
          bytes_read = ReadFromSocket(Descriptor, &(variables[i].si_32), 4);
          if(bytes_read != 4) goto Err;
          break;
        case NL_STR8:
          bytes_read = ReadFromSocket(Descriptor, buf, 1);
          if(bytes_read != 1) goto Err;
          len = buf[0];
          bytes_read = ReadFromSocket(Descriptor, buf, len);
          if(bytes_read != len) goto Err;
          variables[i].str = (char *)buf;
          buf+=len;
          *buf=0;
          buf++;
          break;
        case NL_STR16:
          bytes_read = ReadFromSocket(Descriptor, buf, 2);
          if(bytes_read != 2) goto Err;
          len = ((ui_16 *)buf)[0];
          bytes_read = ReadFromSocket(Descriptor, buf, len);
          if(bytes_read != len) goto Err;
          variables[i].str = (char *)buf;
          buf+=len;
          *buf=0;
          buf++;
          break;
        case NL_BIN8:
          bytes_read = ReadFromSocket(Descriptor, buf, 1);
          if(bytes_read != 1) goto Err;
          len = buf[0];
          bytes_read = ReadFromSocket(Descriptor, buf, len);
          if(bytes_read != len) goto Err;
          variables[i].bin.data = buf;
          variables[i].bin.length = len;
          buf+=len;
          break;
        case NL_BIN16:
          bytes_read = ReadFromSocket(Descriptor, buf, 2);
          if(bytes_read != 2) goto Err;
          len = ((ui_16 *)buf)[0];
          bytes_read = ReadFromSocket(Descriptor, buf, len);
          if(bytes_read != len) goto Err;
          variables[i].bin.data = buf;
          variables[i].bin.length = len;
          buf+=len;
          break;
      }
    }

    SMessages[msg].Sproc(variables);
  }
Err:
  ReadClear(Descriptor);
  Error(NL_ERROR_INCORRECT_MESSAGE, "������������ ��������� %d", msg);
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
// ������� ������ �������

// ����� �������

int NL_Error()
{
  return ErrorNumber;
}

char *NL_ErrorDescription()
{
  return ErrorMessage;
}

// ������� �������

int NLS_Init(unsigned short port, NL_TClientConnect connect, NL_TClientDisconnect disconnect)
{
  struct sockaddr_in sa;
#if defined(NL_WINDOWS)
  WSADATA wsaData;

  NLS_Close(); // �� ������ ������

  if (WSAStartup(0x0101, &wsaData)){
    Error(NL_ERROR_WINSOCK, "WinSock not available!");
    return 0;
  }

  // 4 = stdin, stdout, stderr, mother_desc.  Windows might
  // keep sockets and files separate, in which case this isn't
  // necessary, but we will err on the side of caution.
  max_players = wsaData.iMaxSockets - 4;
  if(max_players <= 0)
    return 0;

  mother_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (mother_desc == INVALID_SOCKET){
    WSACleanup ();  // ��������� WinSock
    Error(NL_ERROR_WINSOCK, "Error opening network connection: Winsock error #%d", WSAGetLastError());
    return 0;
  }

#elif defined(NL_UNIX)

  NLS_Close(); // �� ������ ������

  mother_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (mother_desc < 0){
    Error(NL_ERROR_WINSOCK, "Error opening network connection");
    return 0;
  }

  max_players = 1000; // �� �����, �.�. ���������� ��� ����������� ���� �� ����

#endif

  nonblock(mother_desc);

  sa.sin_family = AF_INET;                  // ��������� ������� - Internet
  sa.sin_port = htons(port);                // ��������� ���� ������
  sa.sin_addr.S_un.S_addr = htonl (INADDR_ANY);  // ��� ����������� ������

  if (bind(mother_desc, (struct sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR) {
    closesocket(mother_desc);
#if defined(NL_WINDOWS)
    WSACleanup ();  // ��������� WinSock
#endif
    Error(NL_ERROR_WINSOCK, "bind");
    return 0;
  }

  listen(mother_desc, 5);

  Descriptors_list = (SOCKET *)calloc(max_players, sizeof(SOCKET));

  ClientConnect    = connect;
  ClientDisconnect = disconnect;

  return max_players;
}

void NLS_Close()
{
  ui_32 i;
  for (i = 0; i < Descriptors_number; i++)
    if(Descriptors_list[i] != INVALID_SOCKET)
      closesocket(Descriptors_list[i]);
  if(Descriptors_list)
    free(Descriptors_list);
  closesocket(mother_desc);
  if(ErrorMessage) free(ErrorMessage);
  if(variables) free(variables);

  Descriptors_number = 0;
  Descriptors_list = NULL;
  ErrorNumber = 0;
  ErrorMessage = NULL;
  mother_desc = INVALID_SOCKET;
  max_players = 0;
  memset(SMessages, 0, sizeof(SMessages));
  ClientConnect = NULL;
  ClientDisconnect = NULL;
  variables = NULL;
  variables_number = 0;

#if defined(NL_WINDOWS)
  WSACleanup();
#endif
}

void NLS_SetInterpriter(int message, NLS_TInterpriterProcedure proc, int count, ...)
{
  va_list args;
  int i;

  SMessages[message].Sproc = proc;
  SMessages[message].Cproc = NULL;
  if(!count){
    SMessages[message].n = 0;
    return;
  }
  SMessages[message].n = count;
  SMessages[message].var = (ui_8 *)calloc(count, 1);
  va_start(args, count);
  for(i=0; i<count; i++)
    SMessages[message].var[i] = va_arg(args, int);
  va_end(args);

  if((ui_32)count > variables_number){
    if(variables) free(variables);
    variables_number++;
    variables = (union NL_Variable *)calloc(variables_number, sizeof(union NL_Variable));
  }
}

int NLS_Message(int id, ui_8 msg, ...)
{
  va_list args;
  ui_32 i;
  union NL_Variable var;
  ui_8 buf[NL_MAX_MESSAGE]; // ����� ��� ���������
  ui_32 n, n1; // ����� ��������������� ���������

  if((ui_32)id >= Descriptors_number || Descriptors_list[id] == INVALID_SOCKET){
    Error(NL_ERROR_MESSAGE, "Unknown client id %d", id);
    return 0;
  }

  buf[0] = msg;
  n = 1;
  va_start(args, msg);
  for(i = 0; i < SMessages[msg].n; i++){
    n1 = n;
    switch(SMessages[msg].var[i]){
      case NL_UI_8:  var.ui_8  = (ui_8 )va_arg(args, int); n1+=1; break;
      case NL_SI_8:  var.si_8  = (si_8 )va_arg(args, int); n1+=1; break;
      case NL_UI_16: var.ui_16 = (ui_16)va_arg(args, int); n1+=2; break;
      case NL_SI_16: var.si_16 = (si_16)va_arg(args, int); n1+=2; break;
      case NL_UI_32: var.ui_32 = (ui_32)va_arg(args, int); n1+=4; break;
      case NL_SI_32: var.si_32 = (si_32)va_arg(args, int); n1+=4; break;
      case NL_STR8:  var.str = va_arg(args, char *); n1+=(ui_32)strlen(var.str)+1; break;
      case NL_STR16: var.str = va_arg(args, char *); n1+=(ui_32)strlen(var.str)+2; break;
      case NL_BIN8:  var.bin = va_arg(args, struct NL_TBinary); n1+=var.bin.length+1; break;
      case NL_BIN16: var.bin = va_arg(args, struct NL_TBinary); n1+=var.bin.length+2; break;
    }
    if(n1 > NL_MAX_MESSAGE){
      Error(NL_ERROR_MESSAGE, "too long message");
      return 0;
    }
    switch(SMessages[msg].var[i]){
      case NL_UI_8:  *(ui_8 *)(buf + n) = var.ui_8 ; break;
      case NL_SI_8:  *(si_8 *)(buf + n) = var.si_8 ; break;
      case NL_UI_16: *(ui_16*)(buf + n) = var.ui_16; break;
      case NL_SI_16: *(si_16*)(buf + n) = var.si_16; break;
      case NL_UI_32: *(ui_32*)(buf + n) = var.ui_32; break;
      case NL_SI_32: *(si_32*)(buf + n) = var.si_32; break;
      case NL_STR8:
        *(ui_8 *)(buf + n) = (ui_8)strlen(var.str);
        n+=1;
        strcpy((char *)buf + n, var.str);
        break;
      case NL_STR16:
        *(ui_16 *)(buf + n) = (ui_16)strlen(var.str);
        n+=2;
        strcpy((char *)buf + n, var.str);
        break;
      case NL_BIN8:
        *(ui_8 *)(buf + n) = (ui_8)var.bin.length;
        n+=1;
        memcpy((char *)buf + n, var.bin.data, var.bin.length);
        break;
      case NL_BIN16:
        *(ui_16 *)(buf + n) = (ui_16)var.bin.length;
        n+=2;
        memcpy((char *)buf + n, var.bin.data, var.bin.length);
        break;
    }
    n = n1;
  }
  va_end(args);

  if(!PerformWrite(Descriptors_list[id], buf, n))
    DelClient(id);
  return 1;
}

void NLS_Refresh()
{
  fd_set input_set, exc_set;
  ui_32 i;
  SOCKET desc;
#if defined(NL_UNIX)
  ui_32 nfds = mother_desc;
#endif

  if(mother_desc == INVALID_SOCKET){
    Error(NL_ERROR_MESSAGE, "Not connected");
    return;
  }

  FD_ZERO(&input_set);
  FD_ZERO(&exc_set);
  FD_SET(mother_desc, &input_set);
  for (i=0; i<Descriptors_number; i++){
    desc = Descriptors_list[i];
    if(desc != INVALID_SOCKET){
      FD_SET(desc, &input_set);
      FD_SET(desc, &exc_set);
#if defined(NL_UNIX)
      if(nfds < desc) nfds = desc;
#endif
    }
  }

#if defined(NL_WINDOWS)
  if (select(0, &input_set, NULL, &exc_set, &null_time) < 0){
#elif defined(NL_UNIX)
  if (select(nfds+1, &input_set, NULL, &exc_set, &null_time) < 0){
#endif
    Error(NL_ERROR_WINSOCK, "select < 0");
    return;
  }

  if (FD_ISSET(mother_desc, &input_set))
    AcceptClient();

  for (i = 0; i < Descriptors_number; i++){
    if(Descriptors_list[i] == INVALID_SOCKET)
      continue;
    if (FD_ISSET(Descriptors_list[i], &exc_set)){
      DelClient(i);
      continue;
    }
    if (FD_ISSET(Descriptors_list[i], &input_set)){
      if(!CPerformRead(i)){
        DelClient(i);
        continue;
      }
    }
  }
}

void NLS_Disconnect(int id)
{
  DelClient(id);
}

// ������� �������

int NLC_Init(ui_16 port, char *host)
{
  struct sockaddr_in sa;
  ui_32 addr;
#if defined(NL_WINDOWS)
  WSADATA wsaData;

  NLC_Close(); // �� ������ ������

  if (WSAStartup(0x0101, &wsaData)){
    Error(NL_ERROR_WINSOCK, "WinSock not available!");
    return 0;
  }

  Descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (Descriptor == INVALID_SOCKET){
    Error(NL_ERROR_WINSOCK, "Error opening network connection: Winsock error #%d", WSAGetLastError());
    return 0;
  }

#elif defined(NL_UNIX)

  NLC_Close(); // �� ������ ������

  Descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (Descriptor < 0){
    Error(NL_ERROR_WINSOCK, "Error opening network connection");
    return 0;
  }

#endif

  addr = inet_addr(host);
  if (addr == INADDR_NONE) {
#if defined(NL_WINDOWS)
    struct hostent* HE = gethostbyname(host);
    addr = *((ui_32*)(HE->h_addr_list[0]));
#elif defined(NL_UNIX)
    addr = gethostbyname(host);
#endif
  }

  sa.sin_family      = AF_INET;              // �������� ������� - ��������
  sa.sin_port        = htons(port);          // ���� �������
  sa.sin_addr.S_un.S_addr = addr;            // IP-����� �������
  if(connect(Descriptor, (struct sockaddr *)&sa, sizeof(sa)) != SOCKET_ERROR)
    return 1;
  else {
    Error(NL_ERROR_WINSOCK, "Can not connect");
    NLC_Disconnect();
#if defined(NL_WINDOWS)
    WSACleanup();
#endif
    return 0;
  }
}

void NLC_Close()
{
  NLC_Disconnect();
  if(ErrorMessage) free(ErrorMessage);
  if(variables) free(variables);

  memset(CMessages, 0, sizeof(CMessages));
  ErrorNumber = 0;
  ErrorMessage = NULL;
  variables = NULL;
  variables_number = 0;

#if defined(NL_WINDOWS)
  WSACleanup();
#endif
}

void NLC_SetInterpriter(int message, NLC_TInterpriterProcedure proc, int count, ...)
{
  va_list args;
  int i;

  CMessages[message].Cproc = proc;
  CMessages[message].Sproc = NULL;
  if(!count){
    CMessages[message].n = 0;
    return;
  }
  CMessages[message].n = count;
  CMessages[message].var = (ui_8 *)calloc(count, 1);
  va_start(args, count);
  for(i=0; i<count; i++)
    CMessages[message].var[i] = (ui_8)va_arg(args, int);
  va_end(args);

  if((ui_32)count > variables_number){
    if(variables) free(variables);
    variables_number++;
    variables = (union NL_Variable *)calloc(variables_number, sizeof(union NL_Variable));
  }
}

int NLC_Message(ui_8 msg, ...)
{
  va_list args;
  ui_32 i;
  union NL_Variable var;
  ui_8 buf[NL_MAX_MESSAGE]; // ����� ��� ���������
  ui_32 n, n1; // ����� ��������������� ���������

  if(Descriptor == INVALID_SOCKET){
    Error(NL_ERROR_MESSAGE, "Not connected");
    return 0;
  }

  buf[0] = msg;
  n = 1;
  va_start(args, msg);
  for(i = 0; i < CMessages[msg].n; i++){
    n1 = n;
    switch(CMessages[msg].var[i]){
      case NL_UI_8:  var.ui_8  = (ui_8 )va_arg(args, int); n1+=1; break;
      case NL_SI_8:  var.si_8  = (si_8 )va_arg(args, int); n1+=1; break;
      case NL_UI_16: var.ui_16 = (ui_16)va_arg(args, int); n1+=2; break;
      case NL_SI_16: var.si_16 = (si_16)va_arg(args, int); n1+=2; break;
      case NL_UI_32: var.ui_32 = (ui_32)va_arg(args, int); n1+=4; break;
      case NL_SI_32: var.si_32 = (si_32)va_arg(args, int); n1+=4; break;
      case NL_STR8:  var.str = va_arg(args, char *); n1+=(ui_32)strlen(var.str)+1; break;
      case NL_STR16: var.str = va_arg(args, char *); n1+=(ui_32)strlen(var.str)+2; break;
      case NL_BIN8:  var.bin = va_arg(args, struct NL_TBinary); n1+=var.bin.length+1; break;
      case NL_BIN16: var.bin = va_arg(args, struct NL_TBinary); n1+=var.bin.length+2; break;
    }
    if(n1 > NL_MAX_MESSAGE){
      Error(NL_ERROR_MESSAGE, "too long message");
      return 0;
    }
    switch(CMessages[msg].var[i]){
      case NL_UI_8:  *(ui_8 *)(buf + n) = var.ui_8 ; break;
      case NL_SI_8:  *(si_8 *)(buf + n) = var.si_8 ; break;
      case NL_UI_16: *(ui_16*)(buf + n) = var.ui_16; break;
      case NL_SI_16: *(si_16*)(buf + n) = var.si_16; break;
      case NL_UI_32: *(ui_32*)(buf + n) = var.ui_32; break;
      case NL_SI_32: *(si_32*)(buf + n) = var.si_32; break;
      case NL_STR8:
        *(ui_8 *)(buf + n) = (ui_8)strlen(var.str);
        n+=1;
        strcpy((char *)buf + n, var.str);
        break;
      case NL_STR16:
        *(ui_16 *)(buf + n) = (ui_16)strlen(var.str);
        n+=2;
        strcpy((char *)buf + n, var.str);
        break;
      case NL_BIN8:
        *(ui_8 *)(buf + n) = (ui_8)var.bin.length;
        n+=1;
        memcpy((char *)buf + n, var.bin.data, var.bin.length);
        break;
      case NL_BIN16:
        *(ui_16 *)(buf + n) = (ui_16)var.bin.length;
        n+=2;
        memcpy((char *)buf + n, var.bin.data, var.bin.length);
        break;
    }
    n = n1;
  }
  va_end(args);

  if(!PerformWrite(Descriptor, buf, n)){
    Error(NL_ERROR_MESSAGE, "Not connected");
    NLC_Disconnect();
    return 0;
  }
  return 1;
}

void NLC_Disconnect()
{
  if(Descriptor != INVALID_SOCKET)
    closesocket(Descriptor);
  Descriptor = INVALID_SOCKET;
}

int NLC_Refresh()
{
  fd_set input_set, exc_set;

  if(Descriptor == INVALID_SOCKET){
    Error(NL_ERROR_MESSAGE, "Not connected");
    return 0;
  }

  FD_ZERO(&input_set);
  FD_ZERO(&exc_set);
  FD_SET(Descriptor, &input_set);
  FD_SET(Descriptor, &exc_set);

#if defined(NL_WINDOWS)
  if (select(0, &input_set, NULL, &exc_set, &null_time) < 0){
#elif defined(NL_UNIX)
  if (select(Descriptor+1, &input_set, NULL, &exc_set, &null_time) < 0){
#endif
    NLC_Disconnect();
    Error(NL_ERROR_WINSOCK, "select < 0");
    return 0;
  }

  if (FD_ISSET(Descriptor, &exc_set)){
    NLC_Disconnect();
    return 0;
  }
  if (FD_ISSET(Descriptor, &input_set))
    if(!SPerformRead()){
      NLC_Disconnect();
      return 0;
    }
  return 1;
}
