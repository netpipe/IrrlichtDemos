/*
 * Copyright 2011
 * Copyright 2009 Mathias Kraus <k.hias@gmx.de>
 * Copyright 2007-2008 Thomas Gallinari <tg8187@yahoo.fr>
 * Copyright 2007-2008 Pierre-Benoît Besse <besse.pb@gmail.com>
 *
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

#ifndef FlyBombIrrlicht_H_
#define FlyBombIrrlicht_H_
#include "BombIrrlicht.h"

#include "irrlicht.h"

class PlayerIrrlicht;
class ArenaIrrlicht;

class FlyBombIrrlicht: public BombIrrlicht{
public:
	FlyBombIrrlicht(ArenaIrrlicht* arena, irr::u32 laytime,int bp);
	FlyBombIrrlicht();
	virtual ~FlyBombIrrlicht();
	void setflybombPower(int i);
	void throwBomb(int dx, int dy, float distance);
	void land();
	virtual void update();
	virtual void reset();
	virtual void updatemove();
	virtual void move();
	float mymodulo(float a, float b);
	bool landed;

	float flystartx;
	float flystarty;
	float flystartz;

	float flydonex;
	float flydoney;
	float flydonez;

	float dirx;
	float diry;

	float flylength;

private:
	//irr::u32 laytime;
	//irr::u32 ignitetime;
	int flybombpower;



};

#endif /* IRRLICHT_H_ */
