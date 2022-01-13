#ifndef CMOTIONSTATE_H_
#define CMOTIONSTATE_H_

#include <btBulletCollisionCommon.h>

#include <irrlicht.h>
#include "convert.h"


class CIrrBPRigidBody;

///Should not be used. Only for internal use.
class CMotionState : public btDefaultMotionState
{
    public:
		CMotionState(CIrrBPRigidBody * body,const btTransform &startTrans=btTransform::getIdentity(), const btTransform &centerOfMassOffset=btTransform::getIdentity());

        virtual ~CMotionState();

		virtual void getWorldTransform(btTransform &worldTrans);
        virtual void setWorldTransform(const btTransform &worldTrans);

    protected:
		CIrrBPRigidBody * m_body;
		irr::scene::ISceneNode * m_irrNode;
};

#endif 
