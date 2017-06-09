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

#ifndef BombIrrlicht_H_
#define BombIrrlicht_H_
#include "XYElement.h"
#include "irrlicht.h"

class XYElement;
class PlayerIrrlicht;
class ArenaIrrlicht;

class BombIrrlicht: public XYElement {
public:
	BombIrrlicht(ArenaIrrlicht* arena, irr::u32 laytime, int bp);
	BombIrrlicht();
	virtual ~BombIrrlicht();
	virtual void update();
	virtual void explode();
	bool getExploded();
	void setlaytime(irr::u32 lt);
	PlayerIrrlicht* getowner();
	void setowner(PlayerIrrlicht* po);
	void setarena(ArenaIrrlicht* ar);
	virtual void toArsenal();
	bool remove();
	void setBombPower(int i);
	virtual void reset();
	virtual void updatemove();
	irr::u32 laytime;
	irr::u32 ignitetime;
	bool playsound;
private:
	void lineExplosion(int i, int j, int di, int dj, int bombpower, int s);

	//irr::u32 laytime;
	//irr::u32 ignitetime;

protected:
	bool exploded;
	ArenaIrrlicht* m_arena;
	int bombpower;
	PlayerIrrlicht* owner;

};

#endif /* IRRLICHT_H_ */
