#ifndef MOVEMODELANIMATOR_H
#define MOVEMODELANIMATOR_H

#include "irrlicht.h"

using namespace irr;
using namespace irr::scene;
using namespace irr::core;
using namespace irr::video;

class MoveModelAnimator : public ISceneNodeAnimator {
public:
    MoveModelAnimator(const vector3df& startpoint,
                      const vector3df& endpoint, u32 timeForWay, const vector3df& rotation);
    virtual void animateNode(ISceneNode* node, u32 timeMs);
    virtual ISceneNodeAnimator* createClone(ISceneNode* node,
                                            ISceneManager* newmanager=0);
    virtual bool hasFinished(void) const;

private:
    vector3df mStartPoint;
    vector3df mEndPoint;
    u32 mAnimatedTime;
    u32 mAnimationTime;
    vector3df mRotation;
};

#endif // MOVEMODELANIMATOR_H
