//
// Sockets.cpp
//
// this file contains the wrappers (implementation)
// that can be used as an IOStream-compatible TCP/IP or Unix sockets
//
// on Windows the program that uses this library
// should be linked with Ws2_32.lib
//
// Copyright (C) 2001-2008 Maciej Sobczak
//
// you can use this code for any purpose without limitations
// (and for your own risk) as long as this notice remains
//

#include "Sockets.h"

#include <sstream>

#ifndef WIN32
// this is for Linux/Unix
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <cerrno>
#include <netdb.h>
#include <arpa/inet.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) ::close(s)
#endif

using namespace std;

// class SocketException

SocketRunTimeException::SocketRunTimeException(const string &what)
    : runtime_error(what)
{
#ifdef WIN32
    errnum_ = ::WSAGetLastError();
#else
    errnum_ = errno;
#endif
}

const char * SocketRunTimeException::what() const throw()
{
    ostringstream ss;
    ss << runtime_error::what();
    ss << " error number: " << errnum_;
    msg_ = ss.str();
    return msg_.c_str();
}

// class BaseSocketWrapper

BaseSocketWrapper::~BaseSocketWrapper()
{
    if (sockstate_ != CLOSED)
    {
        closesocket(sock_);
    }
}

void BaseSocketWrapper::write(const void *buf, size_t len)
{
    if (sockstate_ != CONNECTED && sockstate_ != ACCEPTED)
    {
        throw SocketLogicException("socket not connected");
    }

    int written;
    while (len != 0)
    {
        if ((written = send(sock_, (const char*)buf, (int)len, 0))
            == SOCKET_ERROR)
        {
            throw SocketRunTimeException("write failed");
        }

        len -= written;
        buf = (const char*)buf + written;
    }
}

size_t BaseSocketWrapper::read(void *buf, size_t len)
{
    if (sockstate_ != CONNECTED && sockstate_ != ACCEPTED)
    {
        throw SocketLogicException("socket not connected");
    }

    int readn = recv(sock_, (char*)buf, (int)len, 0);
    if (readn == SOCKET_ERROR)
    {
        throw SocketRunTimeException("read failed");
    }

    return (size_t)readn;
}

void BaseSocketWrapper::close()
{
    if (sockstate_ != CLOSED)
    {
        if (closesocket(sock_) == SOCKET_ERROR)
        {
            throw SocketRunTimeException("close failed");
        }
        sockstate_ = CLOSED;
    }
}

// class TCPSocketWrapper

TCPSocketWrapper::TCPSocketWrapper(
    const TCPSocketWrapper::TCPAcceptedSocket &as)
    : BaseSocketWrapper(as), sockaddress_(as.addr_)
{
}

void TCPSocketWrapper::listen(int port, int backlog)
{
    if (sockstate_ != CLOSED)
    {
        throw SocketLogicException("socket not in CLOSED state");
    }

    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ == INVALID_SOCKET)
    {
        throw SocketRunTimeException("socket failed");
    }

    sockaddr_in local;

    memset(&local, 0, sizeof(local));

    local.sin_family = AF_INET;
    local.sin_port = htons((u_short)port);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(sock_, (sockaddr*)&local, sizeof(local)) == SOCKET_ERROR)
    {
        closesocket(sock_);
        throw SocketRunTimeException("bind failed");
    }

    if (::listen(sock_, backlog) == SOCKET_ERROR)
    {
        closesocket(sock_);
        throw SocketRunTimeException("listen failed");
    }

    memset(&sockaddress_, 0, sizeof(sockaddress_));
    sockstate_ = LISTENING;
}

TCPSocketWrapper::TCPAcceptedSocket TCPSocketWrapper::accept()
{
    if (sockstate_ != LISTENING)
    {
        throw SocketLogicException("socket not listening");
    }

    sockaddr_in from;
    socklen_t len = sizeof(from);

    memset(&from, 0, len);

    socket_type newsocket = ::accept(sock_, (sockaddr*)&from, &len);
    if (newsocket == INVALID_SOCKET)
    {
        throw SocketRunTimeException("accept failed");
    }

    return TCPAcceptedSocket(newsocket, from);
}

void TCPSocketWrapper::connect(const string &address, int port)
{
    if (sockstate_ != CLOSED)
    {
        throw SocketLogicException("socket not in CLOSED state");
    }

    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ == INVALID_SOCKET)
    {
        throw SocketRunTimeException("socket failed");
    }

    hostent *hp;

    unsigned long addr = inet_addr(address.c_str());
    if (addr != INADDR_NONE)
    {
        hp = gethostbyaddr((const char*)&addr, 4, AF_INET);
    }
    else
    {
        hp = gethostbyname(address.c_str());
    }

    if (hp == NULL)
    {
        closesocket(sock_);
        throw SocketRunTimeException("cannot resolve address");
    }

    if (hp->h_addrtype != AF_INET)
    {
        closesocket(sock_);
        throw SocketRunTimeException
            ("address resolved with TCP incompatible type");
    }

    memset(&sockaddress_, 0, sizeof(sockaddress_));
    memcpy(&(sockaddress_.sin_addr), hp->h_addr_list[0], hp->h_length);
    sockaddress_.sin_family = AF_INET;
    sockaddress_.sin_port = htons((u_short)port);

    if (::connect(sock_, (sockaddr*)&sockaddress_, sizeof(sockaddress_))
        == SOCKET_ERROR)
    {
        closesocket(sock_);
        throw SocketRunTimeException("connect failed");
    }

    sockstate_ = CONNECTED;
}

string TCPSocketWrapper::address() const
{
    if (sockstate_ != CONNECTED && sockstate_ != ACCEPTED)
    {
        throw SocketLogicException("socket not connected");
    }

    return inet_ntoa(sockaddress_.sin_addr);
}

int TCPSocketWrapper::port() const
{
    if (sockstate_ != CONNECTED && sockstate_ != ACCEPTED)
    {
        throw SocketLogicException("socket not connected");
    }

    return ntohs(sockaddress_.sin_port);
}

#ifndef WIN32

// class UnixSocketWrapper

UnixSocketWrapper::UnixSocketWrapper(
    const UnixSocketWrapper::UnixAcceptedSocket &as)
    : BaseSocketWrapper(as), sockaddress_(as.addr_)
{
}

void UnixSocketWrapper::listen(const string &path, int backlog)
{
    if (sockstate_ != CLOSED)
        throw SocketLogicException("socket not in CLOSED state");

    ::unlink(path.c_str());

    sock_ = ::socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sock_ == INVALID_SOCKET)
    {
        throw SocketRunTimeException("socket failed");
    }

    sockaddr_un local;

    memset(&local, 0, sizeof(local));

    local.sun_family = AF_LOCAL;
    ::strcpy(local.sun_path, path.c_str());

    if (::bind(sock_, (sockaddr*)&local, sizeof(local)) == SOCKET_ERROR)
    {
        closesocket(sock_);
        throw SocketRunTimeException("bind failed");
    }

    if (::listen(sock_, backlog) == SOCKET_ERROR)
    {
        closesocket(sock_);
        throw SocketRunTimeException("listen failed");
    }

    memset(&sockaddress_, 0, sizeof(sockaddress_));
    sockstate_ = LISTENING;
}

UnixSocketWrapper::UnixAcceptedSocket UnixSocketWrapper::accept()
{
    if (sockstate_ != LISTENING)
    {
        throw SocketLogicException("socket not listening");
    }

    sockaddr_un from;
    socklen_t len = sizeof(from);

    memset(&from, 0, len);

    socket_type newsocket = ::accept(sock_, (sockaddr*)&from, &len);
    if (newsocket == INVALID_SOCKET)
    {
        throw SocketRunTimeException("accept failed");
    }

    return UnixAcceptedSocket(newsocket, from);
}

void UnixSocketWrapper::connect(const string &path)
{
    if (sockstate_ != CLOSED)
        throw SocketLogicException("socket not in CLOSED state");

    sock_ = ::socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sock_ == INVALID_SOCKET)
    {
        throw SocketRunTimeException("socket failed");
    }

    memset(&sockaddress_, 0, sizeof(sockaddress_));
    sockaddress_.sun_family = AF_LOCAL;
    ::strcpy(sockaddress_.sun_path, path.c_str());

    if (::connect(sock_, (sockaddr*)&sockaddress_, sizeof(sockaddress_))
        == SOCKET_ERROR)
    {
        closesocket(sock_);
        throw SocketRunTimeException("connect failed");
    }

    sockstate_ = CONNECTED;
}

string UnixSocketWrapper::path() const
{
    if (sockstate_ != CONNECTED && sockstate_ != ACCEPTED)
    {
        throw SocketLogicException("socket not connected");
    }

    return sockaddress_.sun_path;
}

#endif // WIN32

bool socketsInit()
{
#ifdef WIN32
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 0), &wsadata) == 0)
        return true;
    else
        return false;
#else
    // Linux/Unix do not require any initialization
    return true;
#endif
}

void socketsEnd()
{
#ifdef WIN32
    // we do not care about the error codes
    // anyway, we end the program
    WSACleanup();
#endif
}
