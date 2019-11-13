/**
 * DMUX
 * Copyright (C) 2016 Collective Tyranny
 */
#include "Server.hpp"
#include <iostream>
#include <thread>
#include <string>

Server::Server() :
    port(1337),
    run(true)
{
    // Bind the server to the specified port
    if(socket.bind(port) != sf::Socket::Done) {
        std::cerr << "There was an error connecting to the port.\n";
        exit(1);
    } else {
        std::cout << "Server bound to port correctly.\n";
    }
    std::cout << "Waiting to receive packets...\n";

    // The server loop
    while(run) {
        // Client variable used for incoming packets
        Server::Client tmpClient;
        // Incoming packets will be stored in the inPacket
        sf::Packet inPacket;
        // Receive a packet
        if(socket.receive(inPacket, tmpClient.ip, tmpClient.port) !=
                        sf::Socket::Done) {
            std::cerr << "Failed to receive information.\n";
        } else {
            // Empty contents of inPacket into tmpClient variable
            inPacket >> tmpClient.uuid >> tmpClient.username >>
                    tmpClient.pos[0] >> tmpClient.pos[1] >> tmpClient.pos[2];
            // Check to see if this is a new client or an already existing one
            bool newClient = true;
            for(unsigned int i = 0; i < clients.size(); i++) {
                if(tmpClient.uuid == clients[i].uuid) {
                    // If it already exists update the information
                    clients[i] = tmpClient;
                    newClient = false;
                    break;
                }
            }
            if(newClient) {
                // If it's a new client add it to the vector
                clients.push_back(tmpClient);
                std::cout << "A new client has connected: " <<
                        tmpClient.uuid << ":" << tmpClient.port << std::endl;
            }
            // Packet used to store outgoing data (BIG DATA!!!! :P)
            sf::Packet outPacket;
            // Store the BIG DATA!!!
            outPacket << tmpClient.uuid << tmpClient.username <<
                    tmpClient.pos[0] << tmpClient.pos[1] << tmpClient.pos[2];
            // Loop through all the clients and send them the updated data
            for(unsigned int i = 0; i < clients.size(); i++) {
                if(clients[i].uuid != tmpClient.uuid) {
                    if(socket.send(outPacket, clients[i].ip, clients[i].port) !=
                                    sf::Socket::Done) {
                        std::cerr << "An error was encountered when " <<
                                "sending packet to " << clients[i].username <<
                                std::endl;
                    }
                }
            }
        }
    }

    socket.unbind();
}

Server::~Server() { }
