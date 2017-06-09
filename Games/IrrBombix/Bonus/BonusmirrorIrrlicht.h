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

#ifndef BonusmirrorIRRLICHT_H_
#define BonusmirrorIRRLICHT_H_
#include "../IJElementIrrlicht.h"
#include "../Serializable.h"
#include "BonusIrrlicht.h"

class BonusmirrorIrrlicht: public BonusIrrlicht {
public:
	BonusmirrorIrrlicht();
	virtual ~BonusmirrorIrrlicht();
	void apply(PlayerIrrlicht* pl);
	int done;
	float starttime;
	float duration;

};

#endif /* IRRLICHT_H_ */
