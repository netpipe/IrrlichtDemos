/*
 * Input.cpp
 *
 *  Created on: 13-12-2011
 *      Author: Marcin Załuski
 */

#include "Input.h"
#include <iostream>
Input::Input() {
	for(int i=0;i<irr::KEY_KEY_CODES_COUNT;++i){
		pressed[i]=false;
	}
	wheel=0;
}

Input::~Input() {
	// TODO Auto-generated destructor stub
}

bool Input::OnEvent(const irr::SEvent & event)
{
	if(event.EventType == irr::EET_KEY_INPUT_EVENT){
		pressed[event.KeyInput.Key] =event.KeyInput.PressedDown;
	}
	if(event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL){
		std::cout << event.MouseInput.Wheel << std::endl;
		wheel = event.MouseInput.Wheel;
	}
    return false;
}
bool Input::isPressed(irr::EKEY_CODE key) const{
	return pressed[key];
}
void Input::unpress(irr::EKEY_CODE key){
	pressed[key] = false;
}
float Input::getWheel(){
	float ret = wheel;
	wheel = 0;
	return ret;
}
