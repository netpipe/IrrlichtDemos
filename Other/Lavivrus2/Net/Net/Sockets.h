//
// Sockets.h
//
// this file contains the wrappers that can be used
// as an IOStream-compatible TCP/IP or Unix sockets
//
// on Windows the program that uses this utility
// should be linked with Ws2_32.lib
//
// Copyright (C) 2001-2008 Maciej Sobczak
//
// you can use this code for any purpose without limitations
// (and for your own risk) as long as this notice remains
//

#ifndef INCLUDED_SOCKETS_H
#define INCLUDED_SOCKETS_H

#ifdef WIN32
// this is for MS Windows
#include <Winsock2.h>
typedef int socklen_t;
#else
// this is for Linux/Unix
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>
#endif

#include <string>
#include <streambuf>
#include <ostream>
#include <stdexcept>

using namespace std;

// exception class which designates errors from socket functions
class SocketRunTimeException : public std::runtime_error
{
public:
    explicit SocketRunTimeException(const std::string &what);
    virtual const char * what() const throw();
    int errornumber() const throw() { return errnum_; }
    ~SocketRunTimeException() throw () { }
private:
    // this will serve as a message returned from what()
    mutable std::string msg_;
    int errnum_;
};

// exception class which designates logic (programming) errors with sockets
class SocketLogicException : public std::logic_error
{
public:
    explicit SocketLogicException(const std::string &what)
        : std::logic_error(what)
    {
    }
};

// this class is a base class for both TCP and Unix sockets
class BaseSocketWrapper
{
public:
#ifdef WIN32
    // on Windows, socket is represented by the opaque handler
    typedef SOCKET socket_type;
#else
    // on Linux, socket is just a descriptor number
    typedef int socket_type;
#endif

    enum sockstate_type { CLOSED, LISTENING, ACCEPTED, CONNECTED };

    BaseSocketWrapper() : sockstate_(CLOSED) {}
    ~BaseSocketWrapper();

    // general methods

    // get the current state of the socket wrapper
    sockstate_type state() const { return sockstate_; }

    // write data to the socket
    void write(const void *buf, size_t len);

    // read data from the socket
    // returns the number of bytes read
    size_t read(void *buf, size_t len);

    void close();

protected:

    // proxy helper for syntax:
    // Sock s2(s1.accept());
    template <class AddressType, class SocketWrapper>
    class AcceptedSocket
    {
    public:
        // only copy constructor provided for the proxy
        // so that the SocketWrapper::accept can
        // successfully return by value
        AcceptedSocket(const AcceptedSocket &a)
            : sock_(a.sock_), addr_(a.addr_)
        {
        }

        AcceptedSocket(socket_type s, AddressType a)
            : sock_(s), addr_(a)
        {
        }

        // assignment not provided
        void operator=(const AcceptedSocket &);

        socket_type sock_;
        AddressType addr_;
    };

    template <class AddressType, class SocketWrapper>
    BaseSocketWrapper(const AcceptedSocket<AddressType, SocketWrapper> &as)
        : sock_(as.sock_), sockstate_(ACCEPTED)
    {
    }

    socket_type sock_;
    sockstate_type sockstate_;

private:
    // not for user
    BaseSocketWrapper(const BaseSocketWrapper &);
    void operator=(const BaseSocketWrapper &);
};

// this class serves as a socket wrapper
class TCPSocketWrapper : public BaseSocketWrapper
{
    typedef BaseSocketWrapper::AcceptedSocket<sockaddr_in, TCPSocketWrapper>
    TCPAcceptedSocket;

public:

    TCPSocketWrapper() {}

    // this is provided for syntax
    // TCPSocketWrapper s2(s2.accept());
    TCPSocketWrapper(const TCPAcceptedSocket &as);

    // server methods

    // binds and listens on a given port number
    void listen(int port, int backlog = 100);

    // accepts the new connection
    // it requires the earlier call to listen
    TCPAcceptedSocket accept();

    // client methods

    // creates the new connection
    void connect(std::string const &address, int port);

    // general methods

    // get the network address and port number of the socket
    std::string address() const;
    int port() const;

private:
    // not for use
    TCPSocketWrapper(const TCPSocketWrapper&);
    void operator=(const TCPSocketWrapper&);

    sockaddr_in sockaddress_;
};

#ifndef WIN32

// this class serves as a Unix socket wrapper
// (obviously, available only on Linux/Unix systems)
class UnixSocketWrapper : public BaseSocketWrapper
{
    typedef BaseSocketWrapper::AcceptedSocket<sockaddr_un, UnixSocketWrapper>
    UnixAcceptedSocket;

public:

    UnixSocketWrapper() {}

    // this is provided for syntax
    // UnixSocketWrapper s2(s2.accept());
    UnixSocketWrapper(const UnixAcceptedSocket &as);

    // server methods

    // binds and listens on a given socket path
    void listen(const std::string &path, int backlog = 100);

    // accepts the new connection
    // it requires the earlier call to listen
    UnixAcceptedSocket accept();

    // client methods

    // creates the new connection
    void connect(const std::string &path);

    // general methods

    // get the socket path
    std::string path() const;

private:
    // not for use
    UnixSocketWrapper(const UnixSocketWrapper&);
    void operator=(const UnixSocketWrapper&);

    sockaddr_un sockaddress_;
};

#endif // WIN32

// this class is supposed to serve as a stream buffer associated with a socket
template <class SocketWrapper,
          class charT, class traits = std::char_traits<charT> >
class SocketStreamBuffer : public std::basic_streambuf<charT, traits>
{
    typedef std::basic_streambuf<charT, traits> sbuftype;
    typedef typename sbuftype::int_type         int_type;
    typedef charT                               char_type;

public:

    // the buffer will take ownership of the socket (ie. it will close it
    // in the destructor) if takeowner == true
    explicit SocketStreamBuffer(SocketWrapper &sock,
        bool takeowner = false, std::streamsize bufsize = 512)
        : rsocket_(sock), ownsocket_(takeowner),
          inbuf_(NULL), outbuf_(NULL), bufsize_(bufsize),
          remained_(0), ownbuffers_(false)
    {
    }

    ~SocketStreamBuffer()
    {
        try
        {
            if (rsocket_.state() == SocketWrapper::CONNECTED ||
                rsocket_.state() == SocketWrapper::ACCEPTED)
            {
                _flush();
            }

            if (ownsocket_ == true)
            {
                rsocket_.close();
            }
        }
        catch (...)
        {
            // we don't want exceptions to fly out of here
            // and there is not much we can do with errors
            // in this context anyway
        }

        if (ownbuffers_)
        {
            delete [] inbuf_;
            delete [] outbuf_;
        }
    }

protected:
    sbuftype * setbuf(char_type *s, std::streamsize n)
    {
        if (this->gptr() == NULL)
        {
           std::streambuf::setg(s, s + n, s + n);
            std::streambuf::setp(s, s + n);
            inbuf_ = s;
            outbuf_ = s;
            bufsize_ = n;
            ownbuffers_ = false;
        }

        return this;
    }

    void _flush()
    {
        rsocket_.write(outbuf_,
            (this->pptr() - outbuf_) * sizeof(char_type));
    }

    int_type overflow(int_type c = traits::eof())
    {
        // this method is supposed to flush the put area of the buffer
        // to the I/O device

        // if the buffer was not already allocated nor set by user,
        // do it just now
        if (this->pptr() == NULL)
        {
            outbuf_ = new char_type[bufsize_];
            ownbuffers_ = true;
        }
        else
        {
            _flush();
        }

        std::streambuf::setp(outbuf_, outbuf_ + bufsize_);

        if (c != traits::eof())
        {
            std::streambuf::sputc(traits::to_char_type(c));
        }

        return 0;
    }

    int sync()
    {
        // just flush the put area
        _flush();
        std::streambuf::setp(outbuf_, outbuf_ + bufsize_);
        return 0;
    }

    int_type underflow()
    {
        // this method is supposed to read some bytes from the I/O device

        // if the buffer was not already allocated nor set by user,
        // do it just now
        if (this->gptr() == NULL)
        {
            inbuf_ = new char_type[bufsize_];
            ownbuffers_ = true;
        }

        if (remained_ != 0)
        {
            inbuf_[0] = remainedchar_;
        }

        size_t readn = rsocket_.read(static_cast<char*>(inbuf_) + remained_,
            bufsize_ * sizeof(char_type) - remained_);

        // if (readn == 0 && remained_ != 0)
        // error - there is not enough bytes for completing
        // the last character before the end of the stream
        // - this can mean error on the remote end

        if (readn == 0)
        {
            return traits::eof();
        }

        size_t totalbytes = readn + remained_;
        std::streambuf::setg(inbuf_, inbuf_,
            inbuf_ + totalbytes / sizeof(char_type));

        remained_ = totalbytes % sizeof(char_type);
        if (remained_ != 0)
        {
            remainedchar_ = inbuf_[totalbytes / sizeof(char_type)];
        }

        return this->sgetc();
    }

private:

    // not for use
    SocketStreamBuffer(const SocketStreamBuffer&);
    void operator=(const SocketStreamBuffer&);

    SocketWrapper &rsocket_;
    bool ownsocket_;
    char_type *inbuf_;
    char_type *outbuf_;
    std::streamsize bufsize_;
    size_t remained_;
    char_type remainedchar_;
    bool ownbuffers_;
};


// this class is an ultimate stream associated with a socket
template <class SocketWrapper,
          class charT, class traits = std::char_traits<charT> >
class SocketGenericStream :
    private SocketStreamBuffer<SocketWrapper, charT, traits>,
    public std::basic_iostream<charT, traits>
{
public:

    // this constructor takes 'ownership' of the socket wrapper if btakeowner == true,
    // so that the socket will be closed in the destructor of the
    // TCPStreamBuffer object
    explicit SocketGenericStream(SocketWrapper &sock, bool takeowner = false)
        : SocketStreamBuffer<SocketWrapper, charT, traits>(sock, takeowner),
          std::basic_iostream<charT, traits>(this)
    {
    }

private:
    // not for use
    SocketGenericStream(const SocketGenericStream&);
    void operator=(const SocketGenericStream&);
};

namespace SocketsDetails
{

// helper type for hiding conflicting names
    template <class Isolated>
    struct BaseClassIsolator
    {
        Isolated isedmember_;
    };

} // namespace SocketsDetails

// specialized for use as a TCP client
template <class charT, class traits = std::char_traits<charT> >
class TCPGenericClientStream :
    private SocketsDetails::BaseClassIsolator<TCPSocketWrapper>,
    public SocketGenericStream<TCPSocketWrapper, charT, traits>
{
public:

    TCPGenericClientStream(const std::string &address, int port)
        : SocketGenericStream<TCPSocketWrapper, charT, traits>(isedmember_, false)
    {
        isedmember_.connect(address, port);
    }

private:
    // not for use
    TCPGenericClientStream(const TCPGenericClientStream&);
    TCPGenericClientStream& operator=(const TCPGenericClientStream&);
};

#ifndef WIN32

// specialized for use as a Unix socket client
template <class charT, class traits = std::char_traits<charT> >
class UnixGenericClientStream :
    private SocketsDetails::BaseClassIsolator<UnixSocketWrapper>,
    public SocketGenericStream<UnixSocketWrapper, charT, traits>
{
public:

    UnixGenericClientStream(const std::string &path)
        : SocketGenericStream<UnixSocketWrapper, charT, traits>(isedmember_, false)
    {
        isedmember_.connect(path);
    }

private:
    // not for use
    UnixGenericClientStream(const UnixGenericClientStream&);
    void operator=(const UnixGenericClientStream&);
};

#endif // WIN32

// helper declarations for narrow and wide streams
typedef SocketGenericStream<TCPSocketWrapper, char> TCPStream;
typedef SocketGenericStream<TCPSocketWrapper, wchar_t> TCPWStream;
typedef TCPGenericClientStream<char> TCPClientStream;
typedef TCPGenericClientStream<wchar_t> TCPWClientStream;

#ifndef WIN32
typedef SocketGenericStream<UnixSocketWrapper, char> UnixStream;
typedef SocketGenericStream<UnixSocketWrapper, wchar_t> UnixWStream;
typedef UnixGenericClientStream<char> UnixClientStream;
typedef UnixGenericClientStream<wchar_t> UnixWClientStream;
#endif // WIN32

// 'portable' code should call those on the beginning and end of the program
// (Linux/Unix code does not require any initialization and cleanup)
bool socketsInit(); // returns true in success
void socketsEnd();

#endif
