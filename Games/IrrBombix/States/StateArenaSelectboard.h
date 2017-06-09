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
#ifndef StateArenaSelectboard_H_
#define StateArenaSelectboard_H_

#include "../GameState.h"
#include "../hasModell.h"
#include "irrlicht.h"
#include "../OptionsWidgetContainer.h"
#include <string>

class GameIrrlicht;
class GameSceneIrrlicht;

class StateArenaSelectboard: public GameState, public hasModell {
public:
	StateArenaSelectboard();
	StateArenaSelectboard(irr::core::stringc newName);
	virtual ~StateArenaSelectboard();

	void render();

	void updateThisState();


	void  OnEnter(void);


	void  OnLeave(void);

	bool  OnEvent(const irr::SEvent &event);

	std::string  getValue(std::string name);

	static StateArenaSelectboard* statearenaselectboard;



private:
	irr::video::ITexture* rt;
	// the texturecam
	irr::scene::ICameraSceneNode* fixedCam;
	irr::scene::ICameraSceneNode* camera;
	OptionsWidgetContainer entries;

	irr::u32 lastInterfaceAction;
};


#endif /* StateArenaSelectboard_H_ */
