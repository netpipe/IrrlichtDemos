#ifndef __COSMIC_GAMECONFIG_H_INCLUDED__
#define __COSMIC_GAMECONFIG_H_INCLUDED__


#include "GameGlobals.h"


class GameConfig
{
public:
	static void Load(core::stringc fn, GameStruct *pGame);
private:
	GameConfig();
};


#endif
