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

#ifndef STATEPause_H_
#define STATEPause_H_

#include "../GameState.h"
#include "../OptionsWidgetContainer.h"
#include "irrlicht.h"

class GameIrrlicht;
class GameSceneIrrlicht;

class StatePause: public GameState {
public:
	StatePause();
	StatePause(irr::core::stringc newName);
	virtual ~StatePause();

	void render();

	void updateThisState();


	void  OnEnter(void);


	void  OnLeave(void);

	bool  OnEvent(const irr::SEvent &event);

private:
	void init();
	irr::core::array<GameState*> pausedStates;
	OptionsWidgetContainer entries;

	irr::u32 lastInterfaceAction;
	irr::scene::ICameraSceneNode* camera;
};


#endif /* STATEPause_H_ */
