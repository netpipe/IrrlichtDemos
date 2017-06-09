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
#include "BonusrestrainIrrlicht.h"
#include "BonusXMLitemIrrlicht.h"
#include "BonusIrrlicht.h"
#include <iostream>

#include "../PlayerIrrlicht.h"
#include "../GameIrrlicht.h"
#include "../GameTimerIrrlicht.h"
#include "../Cell.h"

BonusrestrainIrrlicht::BonusrestrainIrrlicht() {
	// TODO Auto-generated constructor stub
	//this->setView(this);
	BonusXMLItemIrrlicht* tmp = new BonusXMLItemIrrlicht(this,
			"./media/Items/Bonusrestrain.xml");
	this->setView(tmp);
	tmp->setModel(this);
	done = 0;
	duration = 10000.0;
	//std::cout << this->getI() << ", "<< this->getJ() <<" in BonusrestrainIrrlicht\n";

}

BonusrestrainIrrlicht::~BonusrestrainIrrlicht() {
	// TODO Auto-generated destructor stub
}

void BonusrestrainIrrlicht::apply(PlayerIrrlicht* pl) {
	if (done == 0) {
		done = 1;
		starttime = GameIrrlicht::m_gametimerIrrlicht->getTime();
		pl->m_restrain=pl->m_restrain+1;
	} else if (done == 1) {
		for (int i = 0; i < GameIrrlicht::players.size(); i++) {
			// Collect Bonus
			float dx = GameIrrlicht::players[i]->getX() - pl->getX();
			float dy = GameIrrlicht::players[i]->getY() - pl->getY();

			if (dx * dx + dy * dy < 0.25 * Cell::SIZE * Cell::SIZE
					&& GameIrrlicht::players[i] != pl && GameIrrlicht::players[i]->m_restrain==0) {
				// infect
				BonusrestrainIrrlicht* tmp = new BonusrestrainIrrlicht;
				GameIrrlicht::players[i]->getBonustaken()->push_back(tmp);
				tmp->collected=true;
			}
		}

		if (GameIrrlicht::m_gametimerIrrlicht->getTime() > +starttime
				+ duration) {
			done == 2;
			//pl->incRestrain(-1);
			// dont update anymore
			int pos = pl->getBonustaken()->linear_search(this);
			if (pos >= 0) {
				pl->getBonustaken()->erase(pos);
			}
			pl->m_restrain=pl->m_restrain-1;
		}

	}

}
