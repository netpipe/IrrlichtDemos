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


#ifndef PLAYERCONTROLLERNETWORK_H_
#define PLAYERCONTROLLERNETWORK_H_

#include <irrlicht.h>
#include <pthread.h>
#include <cc++/socket.h>
#include "PlayerIrrlicht.h"

class PlayerControllerNetwork: public ost::TCPSession {
private:
	static ost::Mutex mutex;
	void run(void);
	void final(void);

	PlayerIrrlicht* player;
	float moveHorizontal;
	float moveVertical;
	float button1pressed;
	float button2pressed;
	float entermenu;

public:
	PlayerControllerNetwork(ost::TCPSocket &server);
	static volatile int count;
	void setPlayer(PlayerIrrlicht *player) {
		this->player = player;
	}
	PlayerIrrlicht *getPlayer() const {
			return player;
		}
	void update();
};

#endif /* PLAYERCONTROLLER_H_ */
