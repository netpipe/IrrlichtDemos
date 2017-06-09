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

#include "GameTimerIrrlicht.h"
#include "Mainwindow.h"
#include <iostream>

GameTimerIrrlicht::GameTimerIrrlicht(irr::IrrlichtDevice* dev) {
	device=dev;
	oldrealtime=0;
	time=0;
	//starttime = Mainwindow::device->getTimer()->getTime();
}

GameTimerIrrlicht::~GameTimerIrrlicht() {
	// TODO Auto-generated destructor stub
}

irr::u32 GameTimerIrrlicht::getTime() const
 {
     return time;//-startime;
 }

irr::u32 GameTimerIrrlicht::update()
 {
	irr::u32 newtime= Mainwindow::device->getTimer()->getTime();
	if (oldrealtime){
		time = time + (newtime-oldrealtime);
	}

	oldrealtime=newtime;
	return time;//-startime;
 }

irr::u32 GameTimerIrrlicht::updateOldTime()
 {
	irr::u32 newtime= Mainwindow::device->getTimer()->getTime();
	oldrealtime=newtime;
	return time;//-startime;
 }

void GameTimerIrrlicht::setPause(bool p){
	pause=p;
}
