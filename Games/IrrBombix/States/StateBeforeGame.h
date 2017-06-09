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

#ifndef StateBeforeGame_H_
#define StateBeforeGame_H_

#include "../GameState.h"
#include "irrlicht.h"

class GameIrrlicht;
class GameSceneIrrlicht;

class StateBeforeGame: public GameState {
public:
	StateBeforeGame();
	StateBeforeGame(irr::core::stringc newName);
	virtual ~StateBeforeGame();

	void render();

	void updateThisState();


	void  OnEnter(void);


	void  OnLeave(void);

	bool  OnEvent(const irr::SEvent &event);

	GameIrrlicht* m_game;
	GameSceneIrrlicht* m_gamescene;

	irr::u32 countdowntimer;

private:
	bool one;
	bool two;
	bool three;
	bool go;


};


#endif /* StateBeforeGame_H_ */
