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


#ifndef PLAYERCONTROLLERKEYBOARD_H_
#define PLAYERCONTROLLERKEYBOARD_H_

#include <irrlicht.h>
#include "Serializable.h"
class PlayerIrrlicht;
class IrrlichtEventReceiver;

class PlayerControllerKeyboard: public Serializable {
public:
	PlayerControllerKeyboard();
	~PlayerControllerKeyboard();
	int getControllernr() const {
		return controllernr;
	}

	PlayerIrrlicht *getPlayer() const {
		return player;
	}

	void setControllernr(int controllernr) {
		this->controllernr = controllernr;
	}

	void setPlayer(PlayerIrrlicht *player) {
		this->player = player;
	}
	void update();

	bool getActive() const {
		return active;
	}

	void setActive(bool active) {
		this->active = active;
	}

	void setKeys(int l, int u, int r, int d, int b1, int b2);

	bool readConfig(long nr);

	void readFrom(Deserializer* id);
	void writeTo(Serializer* is);

private:
	PlayerIrrlicht* player;
	long controllernr;
	bool active;

	long button1;
	long button2;
	long upbutton;
	long downbutton;
	long leftbutton;
	long rightbutton;

};

#endif /* PLAYERCONTROLLER_H_ */
