#ifndef __COSMIC_GAMEMESHOBJECT_H_INCLUDED__
#define __COSMIC_GAMEMESHOBJECT_H_INCLUDED__


#include "GameGlobals.h"


class GameMeshObject : public GameObject
{
public:
	scene::IMeshSceneNode* pMeshNode;

	GameMeshObject(core::stringc file);
	virtual ~GameMeshObject();

	void Load(io::IXMLReader *xml);
};


#endif