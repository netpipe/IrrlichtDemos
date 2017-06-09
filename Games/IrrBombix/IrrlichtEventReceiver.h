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

#ifndef IRRLICHTEVENTRECEIVER_H_
#define IRRLICHTEVENTRECEIVER_H_

#include "irrlicht.h"
class ControllerManager;

class IrrlichtEventReceiver  : public irr::IEventReceiver{
public:
	IrrlichtEventReceiver();
	virtual ~IrrlichtEventReceiver();
	virtual bool OnEvent(const irr::SEvent& event);
	void detectJoysticks();
	void poolJoystick();
	void setDevice(irr::IrrlichtDevice* device);
	const irr::SEvent::SJoystickEvent & GetJoystickState(void) const;
	ControllerManager* getPlayerController();

private:
	irr::SEvent::SJoystickEvent JoystickState;
	irr::IrrlichtDevice* device;
	irr::core::array<irr::SJoystickInfo> joystickInfo;
	ControllerManager* playercontroller;
};

#endif /* IRRLICHTEVENTRECEIVER_H_ */

