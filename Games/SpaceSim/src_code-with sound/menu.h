#ifndef _MENU_H_
#define _MENU_H_

#include "irrlicht.h"
#include "irrKlang.h"
#include "keylistener.h"



class menu
{
public:
	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;
	KeyListener *receiver;

	menu(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, KeyListener *receiver);
	~menu();

	bool menu_open;
	void menuLoop();
};



#endif
