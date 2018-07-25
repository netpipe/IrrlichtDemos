#include "movemodelanimator.h"
#include <QDebug>

MoveModelAnimator::MoveModelAnimator(
        const vector3df& startpoint,
        const vector3df& endPoint,
        u32 timeForWay,
        const vector3df& rotation)
    : mStartPoint(startpoint)
    , mEndPoint(endPoint)
    , mAnimatedTime(0)
    , mAnimationTime(timeForWay)
    , mRotation(rotation){

    vector3df direction = mEndPoint - mStartPoint;
    mRotation.Y = (-atan2(direction.Z, direction.X) * irr::core::RADTODEG);
}

void MoveModelAnimator::animateNode(ISceneNode* node, u32 timeMs){
    Q_UNUSED(timeMs);

    if(node->getRotation().Y != mRotation.Y){
        node->setRotation(mRotation);
        return;
    }
    ++mAnimatedTime;
    node->setPosition(node->getPosition() + (mEndPoint - mStartPoint)/mAnimationTime);
    if(hasFinished()){
        node->removeAnimator(this);
    }
}


ISceneNodeAnimator* MoveModelAnimator::createClone(ISceneNode* node, ISceneManager* newManager){
    Q_UNUSED(node);
    Q_UNUSED(newManager);
    return NULL;
}

bool MoveModelAnimator::hasFinished() const {
    return mAnimatedTime >= mAnimationTime;
}
