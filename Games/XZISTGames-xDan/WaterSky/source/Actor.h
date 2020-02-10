
#ifndef __Actor_h
#define __Actor_h

#include "object.h"
#include "ODESphereAnimator.h"

struct AnimationCycle
{
    bool set; // is this animation present?
    int startFrame;
    int endFrame;
    core::stringc sound;
};

class Actor : public Object, public scene::IAnimationEndCallBack
{
public:
    Actor(WorldBase *);
    
    enum ANIMATION
    {
        ANIM_UNSET = -1,
        ANIM_IDLE = 0,
        ANIM_WALK = 1
    };
    
    void update(f32);
    void rotateTo(f32);
    //core::array <dBodyID> *getBodies();
    void setNextAnimation(ANIMATION);
    
    Object *isCarrying(u32);
    Object *isCarrying(OBJECT_TYPE);       // returns last one of that type (top of the stack)
    void carry(Object *);
    void stopCarrying(Object *);
    void pruneObjectsCarrying();
    
    virtual OBJECT_TYPE getType() {return OBJECT_TYPE_ACTOR; }
    
protected:
    core::array <AnimationCycle> anims;
    
    void enableCollisionResponse();
    void setAnimation(ANIMATION);
    void defineAnimation(ANIMATION, int,int,char *);
    
private:
    core::vector2df desiredAim;
    core::vector3df lastPos;
    ANIMATION currentAnimation;
    ANIMATION nextAnimation;
    
    core::array <u32> objectsCarrying;
    
    void OnAnimationEnd(scene::IAnimatedMeshSceneNode *);
};

#endif
