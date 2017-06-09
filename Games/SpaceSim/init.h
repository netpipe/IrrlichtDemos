
//INITIALIZER HEADER FIEEL

#ifndef INIT_H
#define INIT_H

#include "irrlicht.h"
//#include "irrKlang.h"
#include "gameloop.h"
#include "keylistener.h"




class initGame
{
public:
	initGame();
	virtual ~initGame();


	int StartGame();



	friend class Game;
	float time;

//	irrklang::ISoundEngine *sound;
	KeyListener *receiver;

	irr::IrrlichtDevice *graphics;




	GameLoop *Game;


};

#endif
