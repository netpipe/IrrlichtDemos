#ifndef __COSMIC_GAMEMAP_H_INCLUDED__
#define __COSMIC_GAMEMAP_H_INCLUDED__


#include "GameGlobals.h"


class GameMap
{
public:
	GameObject **Objects;
	int ObjectsCount;

	GameMap();
	virtual ~GameMap();

	int GetFreeID();
	GameObject* GetObjectByName(core::stringc s);
	void AddObject(GameObject* obj);

	void Load(core::stringc fn);
};


#endif
