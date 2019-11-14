#ifndef __COSMIC_GAMEANIMATEDMESHOBJECT_H_INCLUDED__
#define __COSMIC_GAMEANIMATEDMESHOBJECT_H_INCLUDED__


#include "GameGlobals.h"


class GameAnimatedMeshObject : public GameObject
{
public:
	scene::IAnimatedMeshSceneNode* pAnimatedMeshNode;

	GameAnimatedMeshObject(core::stringc file);
	virtual ~GameAnimatedMeshObject();

	void Load(io::IXMLReader *xml);
};


#endif
