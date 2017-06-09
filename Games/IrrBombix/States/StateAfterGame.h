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

#ifndef StateAfterGame_H_
#define StateAfterGame_H_

#include "../GameState.h"
#include "irrlicht.h"

class GameIrrlicht;
class GameSceneIrrlicht;

class StateAfterGame: public GameState {
public:
	StateAfterGame();
	StateAfterGame(irr::core::stringc newName);
	virtual ~StateAfterGame();

	void render();

	void updateThisState();


	void  OnEnter(void);


	void  OnLeave(void);

	bool  OnEvent(const irr::SEvent &event);

	GameIrrlicht* m_game;
	GameSceneIrrlicht* m_gamescene;

	irr::u32 countdowntimer;



};


#endif /* StateAfterGame_H_ */
