#ifndef MOTION_MANAGER_H
#include <irrlicht.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "bullethelper.h"
using namespace irr;
/** MotionStateManager handles physics motion of objects. */
class MotionStateManager : public btMotionState {

    private:
        /** Graphic node to transform */
        scene::ISceneNode* node;
        /** Initial state */
        btTransform initialPosition;

    public:
        /** Set the private attributes.
        * @param pos btTransform (translation and rotation)
        * @param node ISceneNode graphic object
        */
        MotionStateManager(btTransform &pos, scene::ISceneNode* node);
        virtual ~MotionStateManager(){}
        /** Virtual method called by Bullet before setWorldTransform */
        virtual void getWorldTransform(btTransform &wordTrans) const;
        /** Virtual method called by Bullet whenever there is a physics change to an object.
        * This is the right place to update graphic objects.
        */
        virtual void setWorldTransform(const btTransform &worldTrans);
};
#endif
