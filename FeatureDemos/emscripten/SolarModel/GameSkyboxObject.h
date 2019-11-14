#ifndef __COSMIC_GAMESKYBOXOBJECT_H_INCLUDED__
#define __COSMIC_GAMESKYBOXOBJECT_H_INCLUDED__


#include "GameGlobals.h"


class GameSkyboxObject : public GameObject
{
public:
	GameSkyboxObject();
	virtual ~GameSkyboxObject();

	void Load(io::IXMLReader *xml);
};


#endif
