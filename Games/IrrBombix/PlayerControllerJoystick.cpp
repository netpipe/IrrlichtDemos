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

#include "PlayerControllerJoystick.h"
#include "PlayerIrrlicht.h"
#include "irrlicht.h"
#include <iostream>
#include "GameStateManager.h"
#include "GameState.h"

PlayerControllerJoystick::PlayerControllerJoystick() {
	player = 0;
	// calibration
	maxmoveHorizontal = 0.0;
	maxmoveVertical = 0.0;
	minmoveHorizontal = 0.0;
	minmoveVertical = 0.0;

}

PlayerControllerJoystick::~PlayerControllerJoystick() {
	// TODO Auto-generated destructor stub
}

//irr::SEvent::SJoystickEvent & PlayerControllerJoystick::GetJoystickState()
//{
//	return JoystickState;
//}


void PlayerControllerJoystick::update() {
	irr::f32 moveHorizontal = 0.f; // Range is -1.f for full left to +1.f for full right
	irr::f32 moveVertical = 0.f; // -1.f for full down to +1.f for full up.

	irr::SEvent::SJoystickEvent & joystickData = GetJoystickState();

	if (joystickData.IsButtonPressed(3)) {
		if (statemanager.getActiveGameState()->getName() != "Pause") {
			statemanager.changeGameState("Pause");
		}
	}

	if (player) {

		//if (joystickInfo.size() > 0) {


		if (joystickData.IsButtonPressed(1)) {
			//std::cout<<"Button 1 pressed\n";
			player->button1Pressed();
			//player->igniteBomb();
		} else {
			player->button1Released();
		}

		if (joystickData.IsButtonPressed(2)) {
			std::cout << "Button 2 pressed\n";
			player->button2Pressed();
			//player->igniteBomb();
		} else {
			player->button2Released();
		}

		// We receive the full analog range of the axes, and so have to implement our
		// own dead zone.  This is an empirical value, since some joysticks have more
		// jitter or creep around the center point than others.  We'll use 5% of the
		// range as the dead zone, but generally you would want to give the user the
		// option to change this.
		const irr::f32 DEAD_ZONE = 0.05f;

		moveHorizontal
				= (irr::f32) joystickData.Axis[irr::SEvent::SJoystickEvent::AXIS_X]
						/ 32767.f;
		if (fabs(moveHorizontal) < DEAD_ZONE)
			moveHorizontal = 0.f;

		moveVertical
				= (irr::f32) joystickData.Axis[irr::SEvent::SJoystickEvent::AXIS_Y]
						/ -32767.f;

		// std::cout << " PlayerController joystick "<<controllernr <<" "<<  joystickData.Axis[irr::SEvent::SJoystickEvent::AXIS_X] << ", "<<  joystickData.Axis[irr::SEvent::SJoystickEvent::AXIS_Y]<<"\n";

		if (fabs(moveVertical) < DEAD_ZONE)
			moveVertical = 0.f;

		// POV hat info is only currently supported on Windows, but the value is
		// guaranteed to be 65535 if it's not supported, so we can check its range.
		const irr::u16 povDegrees = joystickData.POV / 100;
		if (povDegrees < 360) {
			if (povDegrees > 0 && povDegrees < 180) {
				moveHorizontal = 1.f;
			} else if (povDegrees > 180) {
				moveHorizontal = -1.f;
			}
			if (povDegrees > 90 && povDegrees < 270) {
				moveVertical = -1.f;
			} else if (povDegrees > 270 || povDegrees < 90) {
				moveVertical = +1.f;

			}
		}

		maxmoveHorizontal = std::max(maxmoveHorizontal, moveHorizontal);
		minmoveHorizontal = std::min(minmoveHorizontal, moveHorizontal);

		maxmoveVertical = std::max(maxmoveVertical, moveVertical);
		minmoveVertical = std::max(minmoveVertical, moveVertical);

		// std::cout << " PlayerController joystick "<<controllernr <<" "<< moveVertical << ", "<< moveHorizontal<<"\n";
		if (!irr::core::equals(moveHorizontal, 0.f) || !irr::core::equals(
				moveVertical, 0.f)) {
			//std::cout << "Joystick (x,y) = (" << moveHorizontal << ", "<< moveVertical << ")\n";
		}
		if (moveHorizontal > 0.0) {
			// if calibrated
			if (maxmoveHorizontal && minmoveHorizontal)
				player->goRight();
		} else if (moveHorizontal < 0.0) {
			// if calibrated
			if (maxmoveHorizontal && minmoveHorizontal)
				player->goLeft();
		} else if (moveVertical > 0.0) {
			if (maxmoveVertical && minmoveVertical)
				player->goUp();
		} else if (moveVertical < 0.0) {
			if (maxmoveVertical && minmoveVertical)
				player->goDown();
		} else if (moveHorizontal == 0.0 && moveVertical == 0.0) {
			player->updateDirection();
		}
		//}
	}
}
