// Copyright (C) 1999-2005 Open Source Telecom Corporation.
// Copyright (C) 2006-2008 David Sugar, Tycho Softworks.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// As a special exception to the GNU General Public License, permission is
// granted for additional uses of the text contained in its release
// of APE.
//
// The exception is that, if you link the APE library with other files
// to produce an executable, this does not by itself cause the
// resulting executable to be covered by the GNU General Public License.
// Your use of that executable is in no way restricted on account of
// linking the APE library code into it.
//
// This exception does not however invalidate any other reasons why
// the executable file might be covered by the GNU General Public License.
//
// This exception applies only to the code released under the
// name APE.  If you copy code from other releases into a copy of
// APE, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
//
// If you write modifications of your own for APE, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.

#include <cc++/socket.h>
#include <cstdlib>
#include "PlayerIrrlicht.h"
#include "irrlicht.h"
#include <pthread.h>
#include "Mainwindow.h"
#include "TcpInputServer.h"
#include "TcpSocket.h"
#include <iostream>
#include "PlayerControllerNetwork.h"
#include "Serializer.h"
#include "Deserializer.h"


#ifdef	CCXX_NAMESPACES
// using namespace std;
using namespace ost;
#endif


int TcpInputServer::port=7895;
std::string* TcpInputServer::addr = new std::string("127.0.0.1");



TcpInputServer::TcpInputServer() {
}

void* TcpInputServer::serverThread(void* pthis) {
	PlayerControllerNetwork *tcp;

	// addr = "127.0.0.1";
	IPV4Address seraddr= addr->c_str();
	std::cout << "binding for: " << seraddr.getHostname() << ":" << port << std::endl;

	try {
		TcpSocket server(seraddr, port);

		while (1) {
			std::cout << "before create" << std::endl;
			tcp = new PlayerControllerNetwork(server);
			std::cout << "after create" << std::endl;
			tcp->detach();
		}
	} catch (Socket *socket) {
		tpport_t port;
		int err = socket->getErrorNumber();
		InetAddress saddr = (InetAddress) socket->getPeer(&port);
		std::cerr << "socket error " << saddr.getHostname() << ":" << port << " = "
				<< err << std::endl;
		if (err == Socket::errBindingFailed) {
			std::cerr << "bind failed; port busy" << std::endl;
			::exit(-1);
		} else
			std::cerr << "client socket failed" << std::endl;
	}
	std::cout << "TcpInputServer closed " << std::endl;
	//return 0;
}

int TcpInputServer::startServer() {
	Deserializer* d = new Deserializer("./config/net.xml");
			if (d->fileok) {
				readFrom(d);
			} else {
				//Allow the user to a net port

				std::cout << "Using the defaults 127.0.0.1 7895 for the Controller Server\n";

				//std::cin >> port;
				//std::cout << std::endl;
				std::cout << "edit ./config/net.xml if you want something different\n";
				writeTo(new Serializer("./config/net.xml"));
			}


	pthread_t threads;
	pthread_create(&threads, NULL, TcpInputServer::serverThread, this);
	std::cout << "starting TcpInputServer\n";
	//sleep(5);

}


void TcpInputServer::save() {
	writeTo(new Serializer("./config/net.xml"));
}

void TcpInputServer::readFrom(Deserializer *d) {
d->beginAttribute("net");
long tmp;
d->readParameter("port", tmp);
port = tmp;
d->readParameter("addr", *addr);

}

void TcpInputServer::writeTo(Serializer *s) {
s->beginAttribute("net");
long tmp = port;
s->writeParameter("port", tmp);
s->writeParameter("addr", *addr);
s->endAttribute();
s->endAttribute();
}


