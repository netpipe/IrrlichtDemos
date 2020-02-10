
#include "Block.h"
#include "ODEBoxAnimator.h"

Block::Block(WorldBase *world, core::vector3df pos)
    : Object(world)
{
    sceneNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("block.b3d"));
    sceneNode->setScale( core::vector3df(1,1,1) * 0.5);
    
    setPosition(pos);
    
    dAnimator = new ODEBoxAnimator(sceneNode, dPhysics, ODEANIM_CENTRE_NODE,
        sceneNode->getTransformedBoundingBox().getExtent() * core::vector3df(1,1,1) * 10.0/11.0);      
    sceneNode->addAnimator(dAnimator);
    dAnimator->takeTransformation(sceneNode);
    dAnimator->drop();
}

Object *Block::createNew()
{
    return new Block(this);
}
