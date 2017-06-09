#include "stdafx.h"
#include "menu.h"

menu::menu(irr::IrrlichtDevice *graphics, KeyListener *receiver)
{
	this->graphics=graphics;
//	//this->sound = sound;
	this->receiver = receiver;
}

menu::~menu()
{
}
