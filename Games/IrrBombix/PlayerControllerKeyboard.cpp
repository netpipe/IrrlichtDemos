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


#include "PlayerControllerKeyboard.h"
#include "ControllerManager.h"
#include "PlayerIrrlicht.h"
#include "irrlicht.h"
#include "GameStateManager.h"
#include "Serializer.h"
#include "Deserializer.h"

#include <iostream>
#include <string>

PlayerControllerKeyboard::PlayerControllerKeyboard() {
	player = 0;
	rightbutton=irr::KEY_NUMPAD6;
	leftbutton=irr::KEY_NUMPAD4;
	upbutton=irr::KEY_NUMPAD8;
	downbutton=irr::KEY_NUMPAD2;
	button1=irr::KEY_NUMPAD0;
	button2=irr::KEY_ADD;
}

bool PlayerControllerKeyboard::readConfig(long nr) {
	std::stringstream st;

	st << nr;


	/* Fast config creation
	 rightbutton=irr::KEY_NUMPAD6;
		leftbutton=irr::KEY_NUMPAD4;
		upbutton=irr::KEY_NUMPAD8;
		downbutton=irr::KEY_NUMPAD2;
		button1=irr::KEY_NUMPAD0;
		button2=irr::KEY_ADD;


	Serializer* s = new Serializer("./config/Controller"+st.str()+".xml");
	writeTo(s);
	std::cout <<"PlayerControllerKeyboard creating config " << "./config/Controller"+st.str()+".xml"<<"\n";


	if (nr ==0){
			Serializer* s = new Serializer("./config/Controller"+st.str()+".xml");
			writeTo(s);
			std::cout <<"PlayerControllerKeyboard creating config " << "./config/Controller"+st.str()+".xml"<<"\n";

	}
	*/

	Deserializer* d = new Deserializer("./config/Controller"+st.str()+".xml");
	if(d->fileok){
		readFrom(d);
		return true;
	}
	free(d);
	return false;



	//std::cout <<"PlayerControllerKeyboard creating config " << "./config/Controller"+st.str()+".xml"<<"\n";
}

void PlayerControllerKeyboard::setKeys(int l,int u,int r, int d, int b1, int b2){
	//(keyStatesENUM l,keyStatesENUM u,keyStatesENUM r,keyStatesENUM d,keyStatesENUM b1, keyStatesENUM b2){
	player = 0;
	rightbutton=r;
	leftbutton=l;
	upbutton=u;
	downbutton=d;
	button1=b1;
	button2=b2;
}


PlayerControllerKeyboard::~PlayerControllerKeyboard() {
	// TODO Auto-generated destructor stub
}

void PlayerControllerKeyboard::update() {
	if (player) {

		if (statemanager.playerController->keyReleased(button1)
				|| statemanager.playerController->keyDown(button1)) {
			player->button1Pressed();
			//player->igniteBomb();
		} else {
			player->button1Released();
		}

		if (statemanager.playerController->keyReleased(button2)
				|| statemanager.playerController->keyDown(button2)) {
			player->button2Pressed();
			//player->igniteBomb();
		} else {
			player->button2Released();
		}

		if (statemanager.playerController->keyReleased(rightbutton)
				|| statemanager.playerController->keyDown(rightbutton)) {
			player->goRight();
		} else if (statemanager.playerController->keyReleased(leftbutton)
				|| statemanager.playerController->keyDown(leftbutton)) {
			player->goLeft();
		} else if (statemanager.playerController->keyReleased(upbutton)
				|| statemanager.playerController->keyDown(upbutton)) {
			player->goUp();
		} else if (statemanager.playerController->keyReleased(downbutton)
				|| statemanager.playerController->keyDown(downbutton)) {
			player->goDown();
		} else {
			player->updateDirection();
		}
		//}
	}
}


void PlayerControllerKeyboard::readFrom(Deserializer *d){
	d->beginAttribute("ControllerKeyboard");
		d->readParameter("up", this->upbutton);
		d->readParameter("left", this->leftbutton);
		d->readParameter("down", this->downbutton);
		d->readParameter("right", this->rightbutton);
		d->readParameter("button1", this->button1);
		d->readParameter("button2", this->button2);

};

void PlayerControllerKeyboard::writeTo(Serializer *s){
	s->beginAttribute("ControllerKeyboard");
		s->writeParameter("up", this->upbutton);
		s->writeParameter("left", this->leftbutton);
		s->writeParameter("down", this->downbutton);
		s->writeParameter("right", this->rightbutton);
		s->writeParameter("button1", this->button1);
		s->writeParameter("button2", this->button2);
		s->endAttribute();
		s->endAttribute();
};
