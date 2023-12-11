#ifndef __COSMIC_GAMEBILLBOARDOBJECT_H_INCLUDED__
#define __COSMIC_GAMEBILLBOARDOBJECT_H_INCLUDED__


#include "GameGlobals.h"


class GameBillboardObject : public GameObject
{
public:
	scene::IBillboardSceneNode* pBillboardNode;

	GameBillboardObject();
	virtual ~GameBillboardObject();

	void Load(io::IXMLReader *xml);
};


#endif
