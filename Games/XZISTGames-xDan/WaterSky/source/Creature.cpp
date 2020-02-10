
#include "Creature.h"

Creature::Creature(WorldBase *world, core::vector3df pos)
    : Actor(world)
{
    sceneNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("twinkle.b3d"));
    sceneNode->setPosition(pos);
    sceneNode->updateAbsolutePosition();
    
    defineAnimation(ANIM_IDLE, 0, 41, NULL);
    defineAnimation(ANIM_WALK, 0, 41, "media/sfx/step.wav");
    setAnimation(ANIM_IDLE);
    
    createSoundAnimator();
    enableCollisionResponse();
}

Object *Creature::createNew()
{
    return new Creature(this);
}
