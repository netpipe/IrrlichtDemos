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


#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "irrlicht.h"
class PlayerControllerJoystick;
class PlayerControllerKeyboard;
class PlayerControllerNetwork;
class PlayerIrrlicht;
class TcpInputServer;

class ControllerManager{
public:
	ControllerManager();
	virtual ~ControllerManager();
	void detectJoysticks();
	void addNetworkController(PlayerControllerNetwork* p);
	void removeNetworkController(PlayerControllerNetwork* p);

	void setDevice(irr::IrrlichtDevice* device);
	void init();
	void parseJoy(const irr::SEvent& event);
	void parseKey(const irr::SEvent& event);
	void update();
	bool assignPlayer(PlayerIrrlicht* player);

	bool keyReleased(char keycode);
	bool keyDown(char keycode);
	void startEventProcess();

	bool newcontroller;

private:
	irr::IrrlichtDevice* device;
	irr::core::array<PlayerControllerJoystick*> playerjoysticks;
	irr::core::array<PlayerControllerKeyboard*> playerkeyboards;
	irr::core::array<PlayerControllerNetwork*> playernetworks;
	irr::core::array<irr::SJoystickInfo> joystickInfo;

	TcpInputServer* server;

	bool dojoysticks;

	// Enumeration for UP, DOWN, PRESSED and RELEASED key states. Also used for mouse button states.

	enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};
	// Keyboard key states.
	keyStatesENUM keyState[irr::KEY_KEY_CODES_COUNT];

};

#endif /* IRRLICHTEVENTRECEIVER_H_ */

