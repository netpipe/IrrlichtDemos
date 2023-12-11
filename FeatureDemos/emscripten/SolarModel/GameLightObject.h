#ifndef __COSMIC_GAMELIGHTOBJECT_H_INCLUDED__
#define __COSMIC_GAMELIGHTOBJECT_H_INCLUDED__


#include "GameGlobals.h"


class GameLightObject : public GameObject
{
public:
	scene::ILightSceneNode* pLightNode;

	GameLightObject();
	virtual ~GameLightObject();

	enum video::E_LIGHT_TYPE GetLightTypeByName(core::stringc s);
	void Load(io::IXMLReader *xml);
};


#endif
