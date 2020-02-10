
#include "FuelCan.h"
#include "ODEBoxAnimator.h"

FuelCan::FuelCan(WorldBase *world, core::vector3df pos)
    : Carriable(world)
{
    sceneNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("fuel.b3d"));
    setPosition(pos);
    
    dAnimator = new ODEBoxAnimator(sceneNode, dPhysics, ODEANIM_CENTRE_NODE,
        sceneNode->getTransformedBoundingBox().getExtent() * core::vector3df(1,1,1) * 10.0/11.0);
    sceneNode->addAnimator(dAnimator);
    dAnimator->takeTransformation(sceneNode);
    dAnimator->drop();
    
    createSoundAnimator();
    pickUpSound = "media/sfx/fuelpickup.wav";
}

Object *FuelCan::createNew()
{
    return new FuelCan(this);
}
