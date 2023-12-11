#ifndef __COSMIC_GAMEOBJECT_H_INCLUDED__
#define __COSMIC_GAMEOBJECT_H_INCLUDED__


#include "GameGlobals.h"


class GameObject
{
public:
	int ID; // (int) Unique ID
	enum GameObjectTypeEnum Type; // (GameObjectTypeEnum) Type of the object
	core::stringc Name; // (stringc) Human readable name of the object
	scene::ISceneNode* pNode; // (scene::ISceneNode*) Pointer to scene node in Game.Scene

	GameObject();
	virtual ~GameObject();

	enum video::E_MATERIAL_TYPE GetMaterialTypeByName(core::stringc s);
	enum video::E_MATERIAL_FLAG GetMaterialFlagByName(core::stringc s);
	bool GetBooleanByName(core::stringc s);

	void LoadObject(io::IXMLReader *xml);
	void LoadMaterial(io::IXMLReader *xml);
	void LoadRotationAnimator(io::IXMLReader *xml);
	void LoadFlyCircleAnimator(io::IXMLReader *xml);

private:
	void InitHeightMap(const char *texture, float height, float parallax);
};


#endif
