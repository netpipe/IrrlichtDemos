// SupraSocket.cpp : Defines the entry point for the DLL application.
//

#include "SupraSocket.h"
#include <cassert>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

// Globals are a 1 way ticket to hell!!!
#ifdef WIN32
#define socklen_t int
#else
#define SOCKET_ERROR -1
#endif

SupraSocket::SupraSocket()
{
    host.sin_family = AF_INET;
    client.sin_family = AF_INET;
    memset(&(host.sin_zero), '\0', 8);
    memset(&(client.sin_zero), '\0', 8);
}

SupraSocket::SupraSocket(int type)
{
    host.sin_family = AF_INET;
    client.sin_family = AF_INET;
    memset(&(host.sin_zero), '\0', 8);
    memset(&(client.sin_zero), '\0', 8);

    if (type == SOCK_STREAM)
        sockettype = SOCK_STREAM;
    else if (type == SOCK_DGRAM)
        sockettype = SOCK_DGRAM;
    else
        assert((type != SOCK_STREAM)&&(type != SOCK_DGRAM));
}

SupraSocket::~SupraSocket()
{
    #ifdef WIN32
    closesocket(sock);
    #else
    close(sock);
    #endif
}

int SupraSocket::Connect(char *domain, unsigned short port)
{
    struct hostent *result;
    fprintf(stderr, "SupraSocket->Attempting to resolve: %s\n", domain);
    if ((result = gethostbyname(domain)) == NULL)
    {
        fprintf(stderr, "SupraSocket->Error resolving: %s\n", domain);
        return -1;
    }
    fprintf(stderr, "SupraSocket->%s resolved to %s\n", domain, inet_ntoa(*(struct in_addr *)result->h_addr));
    #ifdef WIN32
    client.sin_addr = *((struct in_addr *)result->h_addr);
    #else
    client.sin_addr.s_addr = *((in_addr_t *)result->h_addr);
    #endif
    client.sin_port = htons(port);
    fprintf(stderr, "SupraSocket->Connecting to: %s:%u(%s)\n", inet_ntoa(*(struct in_addr *)result->h_addr), ntohs(client.sin_port), domain);
    if (connect(sock, (struct sockaddr *)&client, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {
        fprintf(stderr, "SupraSocket->Could not connect to: %s on port %u\n", inet_ntoa(*(struct in_addr *)result->h_addr), ntohs(client.sin_port));
        return -1;
    }
    else
    {
        fprintf(stderr, "SupraSocket->Connected\n");
        return 0;
    }
}

int SupraSocket::Connect(char *ip, unsigned short port, bool type)
{
    if (!type)
    {
        return Connect(ip, port);
    }
    else
    {
        #ifdef WIN32
        client.sin_addr.S_un.S_addr = inet_addr(ip);
        #else
        client.sin_addr.s_addr = inet_addr(ip);
        #endif
    }
    client.sin_port = htons(port);
    fprintf(stderr, "SupraSocket->Connecting to: %s:%u\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    if (connect(sock, (struct sockaddr *)&client, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {
        fprintf(stderr, "SupraSocket->Could not connect to: %s on port %u\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        return -1;
    }
    else
    {
        fprintf(stderr, "SupraSocket->Connected\n");
        return 0;
    }
}

void SupraSocket::Disconnect()
{
    close(sock);
}

int SupraSocket::Send(void *buffer, int size)
{
    if (!encryptionType==NONE)
    {
        // Do encryption
    }
    int written;
    int left;
    const char *p;
    p = (char *)buffer;
    left = size;
    while (left > 0)
    {
        written = send(sock, p, left, 0);
        if (written <= 0)
            if (written < 0 && errno==EINTR)
                written=0;
            else
                return -1;

        left -= written;
        p += written;
    }

    return written;
}

int SupraSocket::Recv(void *buffer, int size, bool clearmem, bool peek)
{
    int popped=0;
    int left=0;
    int received=0;
    char *p=NULL;

    memset(buffer, 0, size);

    if (sockettype == SOCK_DGRAM)
    {
        fd_set rset;
        memset(&rset, 0, sizeof(fd_set));

        FD_SET(sock, &rset);

        struct timeval timeout;

        timeout.tv_sec = 0;
        timeout.tv_usec = 10000;

        select(sock+1, &rset, NULL, NULL, &timeout);
        if (FD_ISSET(sock, &rset) < 1)
            return 0;
    }

    char temp[size];
    memset(temp, 0, size);

    p = temp;
    if (size > 0)
    {
        left = size;
        while (left > 0)
        {
            popped = recv(sock, p, left, (peek)?MSG_PEEK:0);
            #ifndef WIN32
            if (popped < 0)
                #else
                if (popped == SOCKET_ERROR)
                    #endif
                    {
                        if (peek)
                        {
                            strncpy((char*)buffer, temp, size);
                        }

                        #ifndef WIN32
                        if (errno == EINTR)
                            popped = 0;
                        else
                            return -1;
                        #else
                        if (WSAGetLastError() == WSAEINTR)
                            popped = 0;
                        else
                            return -1;
                        #endif
                    }
                else if (popped == 0 || peek)
                    break;

                left -= popped;
                received += popped;
                p += popped;
        }
    }
    else
    {
        received = recv(sock, temp, 4096, 0);
    }
    strncpy((char*)buffer, temp, size);

    return received;
}

int SupraSocket::Bind(unsigned int port)
{
    const char yes=1;

    host.sin_port = htons(port);
    #ifdef WIN32
    host.sin_addr.S_un.S_addr = INADDR_ANY;
    #else
    host.sin_addr.s_addr = INADDR_ANY;
    #endif
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
        perror("setsockopt");
        return -1;
    }
    host.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&host, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {
        fprintf(stderr, "SupraSocket->Could not bind server to port: %u\n", ntohs(host.sin_port));
        perror("bind");
        return -1;
    }
    return 0;
}

int SupraSocket::Listen(int connections)
{
    if (listen(sock, connections) == SOCKET_ERROR)
    {
        fprintf(stderr, "SupraSocket->Could not listen on socket: %u\n", ntohs(host.sin_port));
        perror("listen");
        return -1;
    }
    fprintf(stderr, "SupraSocket->Listening to connections on port: %u\n", ntohs(host.sin_port));
    return 0;
}

int SupraSocket::Accept()
{
    int sin_size = sizeof(struct sockaddr_in);
    // Ignore the signed/unsigned integer comparison warnings here
    #ifdef WIN32
    if (accept(sock, (struct sockaddr *)&client, &sin_size) == unsigned(SOCKET_ERROR))
    #else
    if (accept(sock, (struct sockaddr *)&client, (socklen_t*)&sin_size) == SOCKET_ERROR)
    #endif
    {
        fprintf(stderr, "SupraSocket->Error accepting connection\n");
        return -1;
    }
    return 0;
}

int SupraSocket::Settype(int protocol)
{
    if (protocol == 1)
        sockettype=SOCK_STREAM;
    else if (protocol == 2)
        sockettype=SOCK_DGRAM;
    else
        sockettype=SOCK_STREAM;
    sock = socket(PF_INET, sockettype, 0);
    #ifdef WIN32
    if (unsigned(sock) == INVALID_SOCKET)
    #else
    if (sock == -1)
    #endif
    {
        fprintf(stderr, "SupraSocket->Problem with the sockets interface!\n");
        perror("socket");
        return -1;
    }
    return 0;
}

void SupraSocket::Setencryption(enum encType type)
{
    encryptionType = type;
}

void SupraSocket::SetEncKey(char *key)
{

}

void SupraSocket::Encrypt(char *buffer)
{

}

int SupraSocket::Descriptor()
{
    return sock;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


