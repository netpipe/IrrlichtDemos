// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "CRenderManager.h"
#include <vector>

CRenderManager::CRenderManager(CGTASceneNode *sceneNode)
{
    initialised = false;

    SceneNode = sceneNode;

    // for initialisation, disable rendering for all instances
    std::vector<CGTASceneNode::ObjectInstance> *instances = SceneNode->getObjectInstances();
    vector<CGTASceneNode::ObjectInstance>::iterator instance;
    for(instance = instances->begin(); instance != instances->end(); instance++)
    {
        instance->setRender(false);
    }
}

void CRenderManager::update(core::vector3df position)
{
    if(!initialised)
    {
        initialised = true;
    }

	Position = position;

	process();
}

void CRenderManager::process()
{
    std::vector<CGTASceneNode::ObjectInstance> *instances = SceneNode->getObjectInstances();

    double distance;

    vector<CGTASceneNode::ObjectInstance>::iterator instance;
    for(instance = instances->begin(); instance != instances->end(); instance++)
    {
        distance = instance->getAbsolutePosition().getDistanceFrom(Position);

		if(!instance->isLODMesh())
		{
			if(distance < instance->getDrawDistance())
			{
				// object is in visible distance

				instance->setRender(true);
			}
			else
			{
				instance->setRender(false);
			}
		}
    }
}
