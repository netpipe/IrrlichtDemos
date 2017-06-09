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

#include "irrlicht.h"
#include "IrrlichtEventReceiver.h"
#include <iostream>
#include "ControllerManager.h"
#include "GameStateManager.h"
#include "GameState.h"
#include "Mainwindow.h"

IrrlichtEventReceiver::IrrlichtEventReceiver() {
	// TODO Auto-generated constructor stub
	device = 0;
	playercontroller = new ControllerManager;
	std::cout << "IrrlichtEventReceiver constructor done\n\n";
}

void IrrlichtEventReceiver::setDevice(irr::IrrlichtDevice* device) {
	this->device = device;
	playercontroller->setDevice(device);
	playercontroller->init();
	std::cout << "IrrlichtEventReceiver setDevice done\n\n";
}

IrrlichtEventReceiver::~IrrlichtEventReceiver() {
	// TODO Auto-generated destructor stub
}

ControllerManager* IrrlichtEventReceiver::getPlayerController() {
	return this->playercontroller;
}

// This is the one method that we have to implement
bool IrrlichtEventReceiver::OnEvent(const irr::SEvent& event) {
	//std::cout << "IrrlichtEventReceiver joystick handler: " <<event.EventType << "\n";

	if (!device)
		return false;

	// The state of each connected joystick is sent to us
	// once every run() of the Irrlicht device.  Store the
	// state of the first joystick, ignoring other joysticks.
	// This is currently only supported on Windows and Linux.

	//std::cout << "IrrlichtEventReceiver joystick handler: " <<event.EventType << "\n";

	if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT) {
		//std::cout << "IrrlichtEventReceiver joystick handler: Joystick event!\n";
		playercontroller->parseJoy(event);
	}
	/* Gui Event */

	/* key events */
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {

		if (event.KeyInput.PressedDown == false) {
			if (event.KeyInput.Key == irr::KEY_F9) {
				irr::video::IImage* image =
						device->getVideoDriver()->createScreenShot();
				if (image) {
					device->getVideoDriver()->writeImageToFile(image,
							"screenshot.bmp");
					image->drop();
					return true;
				}
			}
			if (event.KeyInput.Key == irr::KEY_ESCAPE) {
				device->closeDevice();				
				/* fix resolution
				device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(oldscreenwidth, oldscreenheight), 16, false, false, false, eventreceiver);
				device->closeDevice();
				*/

				//Mainwindow::endloop=false;
				return false;
			}
		}
		playercontroller->parseKey(event);

	}
	return false;
}

