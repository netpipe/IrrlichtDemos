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

#include "BonusthrowIrrlicht.h"
#include "BonusXMLitemIrrlicht.h"
#include "BonusIrrlicht.h"
#include "../PlayerIrrlicht.h"
#include <iostream>

BonusthrowIrrlicht::BonusthrowIrrlicht(){
	// TODO Auto-generated constructor stub
	//this->setView(this);
	BonusXMLItemIrrlicht* tmp = new BonusXMLItemIrrlicht(this, "./media/Items/Bonusthrow.xml");
	this->setView(tmp);
	tmp->setModel(this);
	done = false;
	//std::cout << this->getI() << ", "<< this->getJ() <<" in BonusthrowIrrlicht\n";
}

BonusthrowIrrlicht::~BonusthrowIrrlicht() {
	// TODO Auto-generated destructor stub
}

void BonusthrowIrrlicht::apply(PlayerIrrlicht* pl){
	if (!done){
			done = true;
			pl->setThrowBomb();
	}
}
