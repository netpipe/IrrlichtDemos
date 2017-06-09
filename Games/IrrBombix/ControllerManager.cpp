/*
 * PlayerController.cpp
 *
 *  Created on: Mar 20, 2011
 *      Code is based on
 */

/// ==================================================================================================
/// MastEventReceiver code is © (Copyright) Robert E. Demarest, AKA Mastiff or Mastiff Odit
/// This file may be used in any non-commercial or commercial project as long as the following conditions are met:
/// You may not claim this code as being your own.
/// You may not use this code for any harmful, malicious or otherwise damaging programs.
///
/// This is version 1.2a of the class.
/// This class is designed for use with the Irrlicht Engine, it was written for version 1.3 of the engine.
/// ==================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////


#include "ControllerManager.h"
#include "irrlicht.h"
#include "PlayerControllerJoystick.h"
#include "PlayerControllerKeyboard.h"
#include "PlayerControllerNetwork.h"
#include <iostream>
#include "TcpInputServer.h"
#include "States/StatePlayerSelectionboard.h"
#include "GameIrrlicht.h"

ControllerManager::ControllerManager() {
	server = 0;
	device = 0;
	dojoysticks = true;
	newcontroller = false;
}

ControllerManager::~ControllerManager() {
	// TODO Auto-generated destructor stub
}

void ControllerManager::init() {
	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++) {
		keyState[i] = UP;
	}

	playerkeyboards.clear();

	// add all Keyboardcontrollers
	//for (int i = 0; i < 2; i++) {
	for (int i = 0; true; i++) {
		PlayerControllerKeyboard* tmp = new PlayerControllerKeyboard();
		if (tmp->readConfig(i)) {
			playerkeyboards.push_back(tmp);
		} else {
			break;
		}
	}

	// add all Joysticks

	if (device) {
		// Irrlicht events.joysticks numbers can get greater than real existings joysticks, they are simply added and doesnt seem to realize disconnects
		if (dojoysticks) {
			playerjoysticks.clear();
			detectJoysticks();
			dojoysticks = false;
		} else {
			// clear players to reassign
			for (int i = 0; i < playerjoysticks.size(); i++) {
				playerjoysticks[i]->setPlayer(0);
			}
		}
	} else {
		std::cout << " ControllerManager::init and no devive set\n";
	}

	// start server TCPInputServer
	if (!server) {
		server = new TcpInputServer();
		server->startServer();
	}else{

		for (int i = 0; i < playernetworks.size(); i++) {
			playernetworks[i]->setPlayer(0);
		}
	}

}

void ControllerManager::parseJoy(const irr::SEvent& event) {
	if (event.JoystickEvent.Joystick < playerjoysticks.size()) {
		playerjoysticks[event.JoystickEvent.Joystick]->setJoystickState(
				event.JoystickEvent);
	}
}

void ControllerManager::parseKey(const irr::SEvent& event) {
	std::cout << "playerController parseKey enter " << event.KeyInput.Key
			<< "\n";
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		std::cout << "playerController parseKey enter " << event.KeyInput.Key
				<< "\n";
		// if key is Pressed Down
		if (event.KeyInput.PressedDown == true) {
			std::cout << "playerController pressedDown \n";
			// If key was not down before
			if (keyState[event.KeyInput.Key] != DOWN) {
				keyState[event.KeyInput.Key] = PRESSED; // Set to Pressed
			} else {
				// if key was down before
				keyState[event.KeyInput.Key] = DOWN; // Set to Down
			}
		} else {
			std::cout << "playerController !pressedDown \n";

			// if the key is down
			if (keyState[event.KeyInput.Key] != UP) {
				std::cout << "playerController released " << event.KeyInput.Key
						<< "\n";
				keyState[event.KeyInput.Key] = RELEASED; // Set to Released
			}
		}
		// eventprocessed = true;
	}
}

void ControllerManager::startEventProcess() {

	// processState = STARTED;
	//Keyboard Key States
	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++) {
		if (keyState[i] == RELEASED) {
			keyState[i] = UP;
		}

		if (keyState[i] == PRESSED) {
			keyState[i] = DOWN;
		}
	}

}

bool ControllerManager::keyReleased(char keycode) {
	if (keyState[keycode] == RELEASED) {
		return true;
	} else {
		return false;
	}
}

bool ControllerManager::keyDown(char keycode) {
	if (keyState[keycode] == DOWN) {
		return true;
	} else {
		return false;
	}
}

void ControllerManager::setDevice(irr::IrrlichtDevice* device) {
	this->device = device;

}

void ControllerManager::update() {
	for (int i = 0; i < playerjoysticks.size(); i++) {
		playerjoysticks[i]->update();
	}
	for (int i = 0; i < playerkeyboards.size(); i++) {
		playerkeyboards[i]->update();
	}
	for (int i = 0; i < playernetworks.size(); i++) {
		playernetworks[i]->update();
	}
}

bool ControllerManager::assignPlayer(PlayerIrrlicht* player) {

	for (int i = 0; i < playerjoysticks.size(); i++) {
		if (!playerjoysticks[i]->getPlayer()) {
			playerjoysticks[i]->setPlayer(player);
			return true;
		};
	}
	for (int i = 0; i < playerkeyboards.size(); i++) {
		if (!playerkeyboards[i]->getPlayer()) {
			playerkeyboards[i]->setPlayer(player);
			return true;
		};
	}
	for (int i = 0; i < playernetworks.size(); i++) {
		if (!playernetworks[i]->getPlayer()) {
			playernetworks[i]->setPlayer(player);
			return true;
		};
	}

	return false;
}

void ControllerManager::detectJoysticks() {
	//joystickInfo.clear();
	if (device->activateJoysticks(joystickInfo)) {
		std::cout << "Joystick support is enabled and " << joystickInfo.size()
				<< " joystick(s) are present." << std::endl;

		for (irr::u32 joystick = 0; joystick < joystickInfo.size(); ++joystick) {
			std::cout << "Joystick " << joystick << ":" << std::endl;
			std::cout << "\tName: '" << joystickInfo[joystick].Name.c_str()
					<< "'" << std::endl;
			std::cout << "\tAxes: " << joystickInfo[joystick].Axes << std::endl;
			std::cout << "\tButtons: " << joystickInfo[joystick].Buttons
					<< std::endl;

			std::cout << "\tHat is: ";

			switch (joystickInfo[joystick].PovHat) {
			case irr::SJoystickInfo::POV_HAT_PRESENT:
				std::cout << "present" << std::endl;
				break;

			case irr::SJoystickInfo::POV_HAT_ABSENT:
				std::cout << "absent" << std::endl;
				break;

			case irr::SJoystickInfo::POV_HAT_UNKNOWN:
			default:
				std::cout << "unknown" << std::endl;
				break;
			}

			playerjoysticks.push_back(new PlayerControllerJoystick());
			playerjoysticks[joystick]->setControllernr(joystick);
		}
	} else {
		std::cout << "Joystick support is not enabled." << std::endl;
	}

}

// invoked by TcpServer via NetworkController
void ControllerManager::addNetworkController(PlayerControllerNetwork* p) {
	playernetworks.push_back(p);
	newcontroller = true;
}

void ControllerManager::removeNetworkController(PlayerControllerNetwork* p) {
	int pos = playernetworks.linear_search(p);
	if (pos != -1) {
		playernetworks.erase(pos);
	}
	newcontroller = true;

	if (p->getPlayer()) {
		pos = GameIrrlicht::players.linear_reverse_search(p->getPlayer());
		if (pos != -1) {
			GameIrrlicht::players.erase(pos);
		}
	}
}

