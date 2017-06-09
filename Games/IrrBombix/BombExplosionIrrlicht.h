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

#ifndef bombexplosionIRRLICHT_H_
#define bombexplosionIRRLICHT_H_
#include "IJElementIrrlicht.h"
#include "irrlicht.h"

class BombExplosionIrrlicht: public IJElementIrrlicht{
public:
	BombExplosionIrrlicht(irr::u32 explodetime);
	virtual ~BombExplosionIrrlicht();
	void update();
	void explode();
	bool exploded;
	bool done;
	bool bombspot;
private:


	irr::u32 starttime;
	irr::u32 explodetime;
};

#endif /* IRRLICHT_H_ */
