#ifndef _PLANET_H_
#define _PLANET_H_

#include "irrlicht.h"

using namespace irr;
using namespace scene;
using namespace core;

class planet
{
public:
	scene::IAnimatedMeshSceneNode *model;
	planet(irr::IrrlichtDevice *graphics, IAnimatedMesh *model, core::vector3df position, core::vector3df scale);
	void rotate(f32 frameDeltaTime);
	~planet(); //supernova

};



#endif
