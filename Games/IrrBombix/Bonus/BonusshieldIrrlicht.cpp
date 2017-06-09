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

#include "BonusshieldIrrlicht.h"
#include "BonusXMLitemIrrlicht.h"
#include "BonusIrrlicht.h"
#include <iostream>

BonusshieldIrrlicht::BonusshieldIrrlicht(){
	// TODO Auto-generated constructor stub
	//this->setView(this);
	BonusXMLItemIrrlicht* tmp = new BonusXMLItemIrrlicht(this, "/tmp/Bonusitem.xml");
	this->setView(tmp);
	tmp->setModel(this);
	//std::cout << this->getI() << ", "<< this->getJ() <<" in BonusshieldIrrlicht\n";
}

BonusshieldIrrlicht::~BonusshieldIrrlicht() {
	// TODO Auto-generated destructor stub
}
