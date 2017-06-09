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
#ifndef StateIntro_H_
#define StateIntro_H_

#include "../GameState.h"
#include "irrlicht.h"
#include "../SoundManager.h"

class GameIrrlicht;
class GameSceneIrrlicht;

class StateIntro: public GameState {
public:
	StateIntro();
	StateIntro(irr::core::stringc newName);
	virtual ~StateIntro();

	void render();

	void updateThisState();


	void  OnEnter(void);


	void  OnLeave(void);

	bool  OnEvent(const irr::SEvent &event);

	GameIrrlicht* m_game;
	GameSceneIrrlicht* m_gamescene;

	irr::u32 countdowntimer;

	static cAudio::IAudioSource* menumusic;

};


#endif /* StateIntro_H_ */
