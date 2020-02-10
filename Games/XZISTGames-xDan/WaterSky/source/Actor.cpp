
#include "Actor.h"

Actor::Actor(WorldBase *world)
    : Object(world)
{
    currentAnimation = ANIM_UNSET;
    nextAnimation = ANIM_UNSET;
}

void Actor::update(f32 dt)
{
    // Rotate towards an angle

    f32 currentAngle = sceneNode->getRotation().Y * M_PI/180.0;

    core::vector2df currentAim = core::vector2df(
        cos(currentAngle),
        -sin(currentAngle)
        );

    currentAim.normalize();
    currentAim = currentAim.getInterpolated(desiredAim, 1.0 - (10.0*dt));

    core::vector3df newRotation = sceneNode->getRotation();
    newRotation.Y = currentAim.getAngle();

    sceneNode->setRotation(newRotation);

    if (dAnimator)
    {
        dAnimator->addTorqueDamping(0.1);
    }

    // Remove any objects from list that no longer exist...
    pruneObjectsCarrying();

    // update positions of anything it is carrying
    core::vector3df pos = sceneNode->getPosition();
    pos.Y += sceneNode->getBoundingBox().MaxEdge.Y;
    for (unsigned int i = 0; i < objectsCarrying.size(); i ++)
    {
        Object *obj = getObject(objectsCarrying[i]);
        pos.Y += 0.1;
        pos.Y += -obj->getNode()->getBoundingBox().MinEdge.Y;
        obj->setPosition(pos);
        pos.Y += obj->getNode()->getBoundingBox().MaxEdge.Y;
    }
}

void Actor::rotateTo(f32 angle)
{
    desiredAim.X = cos(angle);
    desiredAim.Y = -sin(angle);
    desiredAim.normalize();
}

void Actor::enableCollisionResponse()
{
    //dAnimator = new ODESphereAnimator(sceneNode, dPhysics, ODEANIM_CENTRE_NODE,0.76);//|ODEANIM_NO_ROTATION, 0.76);
    dAnimator = new ODESphereAnimator(sceneNode, dPhysics, ODEANIM_CENTRE_NODE|ODEANIM_NO_ROTATION, 0.76);
    sceneNode->addAnimator(dAnimator);
    dAnimator->takeTransformation(sceneNode);
    dAnimator->drop();
}
/*
core::array <dBodyID> *Actor::getBodies()
{
    return dAnimator->getBodies();
}
*/
void Actor::setNextAnimation(ANIMATION anim)
{
    if (anim == currentAnimation || anim == nextAnimation) return;

    nextAnimation = anim;

    scene::IAnimatedMeshSceneNode *amSceneNode = (scene::IAnimatedMeshSceneNode *)sceneNode;
    amSceneNode->setAnimationEndCallback(this);
    amSceneNode->setLoopMode(false);
}

void Actor::OnAnimationEnd(scene::IAnimatedMeshSceneNode *node)
{
    setAnimation(nextAnimation);
}

void Actor::setAnimation(ANIMATION anim)
{
    currentAnimation = anim;
    nextAnimation = ANIM_UNSET;

    scene::IAnimatedMeshSceneNode *amSceneNode = (scene::IAnimatedMeshSceneNode *)sceneNode;
    amSceneNode->setLoopMode(true);
    //amSceneNode->setAnimationSpeed(6000);

    if (currentAnimation < (int)anims.size() && currentAnimation != ANIM_UNSET)
    {
        if (anims[currentAnimation].set)
        {
            amSceneNode->setFrameLoop(anims[currentAnimation].startFrame, anims[currentAnimation].endFrame);
//            if (anims[currentAnimation].sound.size() && soundAnimator)
//            {
//                soundAnimator->clearAll();
//                soundAnimator->play(anims[currentAnimation].sound.c_str(), SOUNDANIM_ENQUEUE|SOUNDANIM_LOOP);
//            }
//            else if (soundAnimator)
//            {
//                soundAnimator->breakLoop();
//            }
        }
        else
        {
            printf("Actor::setAnimation: Animation not set!\n");
        }
    }
    else
    {
        printf("Actor::setAnimation: Animation not present!\n");
    }

    // FOLLOWING SWITCH() SHOULD PROBABLY BE IN DERIVED CLASS.
    // e.g. virtual void onAnimationChange() = 0;
    // Heck, could even load it from XML!
    /*
    defineAnimation(ANIM_IDLE, 0, 41, NULL);
    defineAnimation(ANIM_WALK, 0, 41, "media/sfx/step.wav");
    switch (currentAnimation)
    {
    case ANIM_IDLE:
        amSceneNode->setFrameLoop(0, 41);
        //amSceneNode->setCurrentFrame(0);
        if (soundAnimator) soundAnimator->breakLoop();
        break;
    case ANIM_WALK:
        amSceneNode->setFrameLoop(0, 41);
        if (soundAnimator)
        {
            soundAnimator->clearAll();
            soundAnimator->play("media/sfx/step.wav", SOUNDANIM_ENQUEUE|SOUNDANIM_LOOP);
        }
        break;
    default:
        printf("setAnimation: Animation not present!\n");
        break;
    }
    */
}

void Actor::defineAnimation(ANIMATION anim, int startFrame, int endFrame, char *sound)
{
    while ((int)anims.size() <= anim)
    {
        AnimationCycle a = { false,0,0,"" };
        anims.push_back( a );
    }

    anims[anim].set = true;
    anims[anim].startFrame = startFrame;
    anims[anim].endFrame = endFrame;
    anims[anim].sound = sound;
}

Object *Actor::isCarrying(u32 otherID)
{
    for (unsigned int i = 0; i < objectsCarrying.size(); i ++)
    {
        if (objectsCarrying[i] == otherID) return getObject(otherID);
    }
    return NULL;
}

Object *Actor::isCarrying(OBJECT_TYPE type)
{
    Object *ret = NULL;
    for (unsigned int i = 0; i < objectsCarrying.size(); i ++)
    {
        Object *obj = getObject(objectsCarrying[i]);
        if (obj)
        {
            if (obj->getType() == type) ret = obj;
        }
    }
    return ret;
}

void Actor::carry(Object *other)
{
    objectsCarrying.push_back( other->getID() );
}

void Actor::stopCarrying(Object *other)
{
    for (unsigned int i = 0; i < objectsCarrying.size(); i ++)
    {
        if ( objectsCarrying[i] == other->getID() )
        {
            objectsCarrying.erase(i);
            return;
        }
    }
}

void Actor::pruneObjectsCarrying()
{
    for (u32 i = 0; i < objectsCarrying.size(); i ++)
    {
        if ( !getObject(objectsCarrying[i]) )
        {
            objectsCarrying.erase(i);
            i --;
        }
    }
}



