#include "CIrrBPCamera.h"

#include "body/CIrrBPBoxBody.h"
#include "ISceneNodeAnimatorCameraFPS.h"

#include "CSceneNodeAnimatorCameraFPS.h"

class CFPSCameraMotionState : public CMotionState
{
public:
	CFPSCameraMotionState(CIrrBPRigidBody * body,const btTransform &startTrans=btTransform::getIdentity(), const btTransform &centerOfMassOffset=btTransform::getIdentity())
		: CMotionState(body,startTrans,centerOfMassOffset)
	{
	}
	void setWorldTransform(const btTransform &worldTrans)
	{
		if(m_irrNode)
		{
			m_irrNode->setPosition(bullet::bulletVectorToIrrVector(worldTrans.getOrigin()));
			m_graphicsWorldTrans = worldTrans * m_centerOfMassOffset ;

		}
		else
		{
			assert("Error during motion state updating.");
		}

	}
};
void CIrrBPCamera::drop()
{
	anim->drop();
	delete this;
}
CIrrBPCamera::CIrrBPCamera(irr::scene::ICameraSceneNode * cam,irr::IrrlichtDevice *dev,irr::f32 mass,int size, irr::s32 bodyId)
{  
	m_IrrSceneNode = cam;
	m_BodyId = bodyId;

	m_MotionState = new CFPSCameraMotionState(this,bullet::getTransformFromIrrlichtNode(cam));

	m_Shape = new btSphereShape(size);

	btVector3 LocalInertia;
	m_Shape->calculateLocalInertia(mass, LocalInertia);

	btRigidBody::btRigidBodyConstructionInfo m_ci(mass,m_MotionState,m_Shape,LocalInertia);
	
	m_ci.m_linearDamping=0.0;//btScalar(0.);
	m_ci.m_angularDamping=0.0;//btScalar(0.);
	m_ci.m_friction=0.0;//btScalar(0.5);
	m_ci.m_restitution=0.0;//btScalar(0.);
	m_ci.m_linearSleepingThreshold=0.0;//btScalar(0.8);
	m_ci.m_angularSleepingThreshold=0.0;//btScalar(1.f);

	m_ci.m_additionalDamping=false;
	
	m_RigidBody = new btRigidBody(m_ci);
	collisionObj = m_RigidBody;
	setAutomaticCCD();
	m_RBtype = CAM;
	
	//Apply the proper animator
	cam->removeAnimators();
	anim = new CSceneNodeAnimatorCameraFPS(this,dev->getCursorControl());
	cam->addAnimator(anim);
	
}
