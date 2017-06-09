/*
 * Copyright 2011
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PlayerControllerNetwork.h"
#include "ControllerManager.h"
#include "PlayerIrrlicht.h"
#include "irrlicht.h"
#include "GameStateManager.h"
#include "Serializer.h"
#include "Deserializer.h"
#include <stdio.h>
#include "GameState.h"

#include <iostream>
#include <string>

ost::Mutex PlayerControllerNetwork::mutex;

PlayerControllerNetwork::PlayerControllerNetwork(ost::TCPSocket &server) :
	ost::TCPSession(server) {
	std::cout << "creating PlayerControllerNetwork" << std::endl;
	mutex.enterMutex();
	// add to ControllerManager

	//GameIrrlicht::game->playerController->addNetworkController(this);
	statemanager.playerController->addNetworkController(this);

	mutex.leaveMutex();
	// unsetf(ios::binary);
	moveHorizontal = 0;
	moveVertical = 0;
	button1pressed = false;
	button2pressed = false;
	player = 0;
	entermenu = 0;
}

void PlayerControllerNetwork::update() {
	if (player) {

		if (button1pressed) {
			player->button1Pressed();
		} else {
			player->button1Released();
		}

		if (button2pressed) {
			player->button2Pressed();
		} else {
			player->button2Released();
		}

		// std::std::cout << " PlayerController joystick "<<controllernr <<" "<< moveVertical << ", "<< moveHorizontal<<"\n";
		if (!irr::core::equals(moveHorizontal, 0.f) || !irr::core::equals(
				moveVertical, 0.f)) {
			//std::std::cout << "Joystick (x,y) = (" << moveHorizontal << ", "<< moveVertical << ")\n";
		}
		if (moveHorizontal > 0.0) {
			// if calibrated
			player->goRight();
		} else if (moveHorizontal < 0.0) {
			// if calibrated
			player->goLeft();
		} else if (moveVertical > 0.0) {
			player->goUp();
		} else if (moveVertical < 0.0) {
			player->goDown();
		} else if (moveHorizontal == 0.0 && moveVertical == 0.0) {
			player->updateDirection();
		}
		if (entermenu) {
			if (statemanager.getActiveGameState()->getName() != "Pause") {
				statemanager.changeGameState("Pause");
			}
		}
		//}
	}
}

void PlayerControllerNetwork::run(void) {
	char i = 1;
	ost::tpport_t port;
	ost::IPV4Address addr = getLocal(&port);
	*tcp() << "welcome to " << addr.getHostname() << " from socket "
			<< (int) so << std::endl;

	*tcp() << "called from thread " << std::endl;

	char line[100];
	int parsed = 5;

	while (parsed != -1) {
		try {

			std::iostream* tmp = tcp();

			tmp->getline(line, 100);
			std::cout << "PlayerControllerNetwork reads:" << line << std::endl;

			parsed
					= sscanf(line, "%f %f %f %f %f", &moveHorizontal,
							&moveVertical, &button1pressed, &button2pressed,
							&entermenu);

			std::cout << "PlayerControllerNetwork parsed:" << parsed
					<< std::endl;

		} catch (...) {
			std::cout << "invalid network income " << line << "\n";

			moveVertical = 0;
			moveHorizontal = 0;
			// bomb yourself
			//button1pressed = 1;
		}

		sleep(50);
	}
	statemanager.playerController->removeNetworkController(this);
	*tcp() << "ending session" << std::endl;
}

void PlayerControllerNetwork::final(void) {
}
