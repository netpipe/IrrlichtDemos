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

#ifndef GAMETIMERIRRLICHT_H_
#define GAMETIMERIRRLICHT_H_
#include "irrlicht.h"

class GameTimerIrrlicht {
public:
	GameTimerIrrlicht(irr::IrrlichtDevice* dev);
	virtual ~GameTimerIrrlicht();
    irr::u32 getTime() const;
    irr::u32 update();
    irr::u32 updateOldTime();
    void setPause(bool p);

private:
    bool pause;
	irr::u32 time;
	irr::u32 startime;
	irr::u32 oldrealtime;
	irr::IrrlichtDevice* device;
};

#endif /* GAMETIMERIRRLICHT_H_ */
