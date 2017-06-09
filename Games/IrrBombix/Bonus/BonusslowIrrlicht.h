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

#ifndef BonusslowIRRLICHT_H_
#define BonusslowIRRLICHT_H_
#include "../IJElementIrrlicht.h"
#include "../Serializable.h"
#include "BonusIrrlicht.h"

class BonusslowIrrlicht: public BonusIrrlicht {
public:
	BonusslowIrrlicht();
	virtual ~BonusslowIrrlicht();
	void apply(PlayerIrrlicht* pl);
	int done;
	int slowspeed;
	float starttime;
	float duration;
};

#endif /* IRRLICHT_H_ */
