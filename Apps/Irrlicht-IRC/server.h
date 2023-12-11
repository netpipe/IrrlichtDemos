
#ifndef SERVER_H
#define SERVER_H

#include "config.h"

class Server
{
public:
	Server(const ServerConfig &sc);
	~Server();
	const ServerConfig &config() const;

	void connectToServer();

	void joinToChannel(std::string channel);

	void leaveToChannel(std::string channel);
	
    void message( const std::string channel, const std::string msg );

	void me(const std::string channel, const std::string msg);

private:
	// explicitly disable copy constructor
	Server(const Server&);
	void operator=(const Server&);

	ServerConfig config_;
    void *irc_;

};


#endif
