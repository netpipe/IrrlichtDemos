/**
 * DMUX
 * Copyright (C) 2016 Collective Tyranny
 */
#pragma once
#include <string>
#include <SFML/Network.hpp>
#include <vector>

class Server {
public:
    Server();
    ~Server();
    struct Client {
        std::string uuid;
        sf::IpAddress ip;
        unsigned short port;
        std::string username;
        float pos[3];
    };

private:
    // Boolean for the server loop
    bool run;
    const unsigned short port;
    sf::UdpSocket socket;
    std::vector<Client> clients;
};
