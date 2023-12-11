// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#ifndef _CRENDERMANAGER_H_
#define _CRENDERMANAGER_H_

#include "CGTASceneNode.h"

#include "irrlicht.h"

#define UNIT_DISTANCE_TRIGGER 20.0f

using namespace irr;
using namespace core;

class CRenderManager
{
public:

    CRenderManager(CGTASceneNode *sceneNode);
	void update(core::vector3df position);

private:
	void process();

    CGTASceneNode *SceneNode;
    bool initialised;
    core::vector3df Position;
};

#endif /* _CRENDERMANAGER_H_ */
