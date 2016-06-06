#include "CBallAnimator.h"
#include "ISceneNode.h"
#include "ISceneManager.h"
#include "ISceneCollisionManager.h"
#include "assert.h"
namespace irr{
namespace game{
CBallAnimator::CBallAnimator():Velocity(0.0f,0.0f,0.0f){
    Selector=NULL;
    LastTimeMs=0;
}
CBallAnimator::~CBallAnimator(){
    if(Selector)
        Selector->drop();
}
void CBallAnimator::animateNode (scene::ISceneNode *node, u32 timeMs){
    if(LastTimeMs && Selector){
        //dirty hack here: if velocity of each direction is too small we set it to a min value
        //so it won't be so boring :p
        const static float minForward = 2.0f;
        const static float maxSpeed = 6.0f;
        if(fabs(Velocity.Z)<minForward)
            Velocity.Z = Velocity.Z>0?minForward:-minForward;
        if(fabs(Velocity.X)<minForward)
            Velocity.X = Velocity.X>0?minForward:-minForward;
        if(fabs(Velocity.Y)<minForward)
            Velocity.Y = Velocity.Y>0?minForward:-minForward;
        Velocity.normalize();
        Velocity=Velocity*maxSpeed;
        //when reflecting, the new might need to start slightly before the actual collision point
        const static float epsilon=0.0f; //not needed if no multi bounce in one frame
        //
        f32 frameDelta=f32(timeMs-LastTimeMs)/1000.0f; //in seconds
        f32 displacementLeft = Velocity.getLength()*frameDelta;
        core::vector3df pos = node->getPosition();
        core::vector3df desiredPos = pos+Velocity*frameDelta;
        core::line3d<float> ray(pos,desiredPos);
        core::triangle3df triangle;
        scene::ISceneNode* collisionNode;
        core::vector3df intersect;
        //find if there's collision between pos and desiredPos
        //if so, let the collide point as new pos and calc new desired point based on reflection
        //repeat(currently disabled), in case it gets reflected multiple times (for example near the corner)
        while(node->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray,Selector,intersect,triangle,collisionNode)){
            core::vector3df tmpVelocity; //use to get normalized Velocity
            //calc new pos
            f32 traveled = pos.getDistanceFrom(intersect)-epsilon;
            tmpVelocity = Velocity;
            pos += tmpVelocity.normalize()*traveled;
            //calc new desiredPos
            displacementLeft -= traveled;
            Velocity = reflectVector(Velocity,triangle.getNormal()).normalize() * Velocity.getLength();
            tmpVelocity = Velocity;
            desiredPos = tmpVelocity.normalize()*displacementLeft+pos;
            ray = core::line3df(pos,desiredPos);
            break; //multi bonce seems not needed
        }
        node->setPosition(desiredPos);
    }
    LastTimeMs=timeMs;
}
scene::ISceneNodeAnimator*  CBallAnimator::createClone(scene::ISceneNode* node,scene::ISceneManager* newManager){
    assert(false);
    return NULL;
}
void CBallAnimator::setTriangleSelector(scene::ITriangleSelector* selector){
    if(Selector)
        Selector->drop();
    Selector=selector;
    Selector->grab();
}
core::vector3df CBallAnimator::reflectVector(const core::vector3df& vector, const core::vector3df& axis){
    core::vector3df out = -vector;
    out.normalize();
    core::vector3df tmpAxis = axis;
    tmpAxis.normalize();
    f32 radians = acos(out.dotProduct(tmpAxis));
    //we introduce a little anomoly to make the game more fun :p
    radians += f32(rand() % 5) * core::DEGTORAD;
    //end if anomoly introducing
    core::vector3df a1 = out;
    core::vector3df a2 = -out.crossProduct(out.crossProduct(axis));
    out = (a1 * cos(radians*2) + a2 * sin(radians*2)) * vector.getLength();
    return out;
}
}}//
