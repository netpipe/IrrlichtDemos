#include "CMotionState.h"
#include "body/CIrrBPRigidBody.h"

CMotionState::CMotionState(CIrrBPRigidBody * body,const btTransform &startTrans, const btTransform &centerOfMassOffset)
{
	m_startWorldTrans = startTrans;
	m_graphicsWorldTrans = startTrans;
	m_centerOfMassOffset = centerOfMassOffset;
	m_body = body;
	m_irrNode = m_body->getIrrlichtNode();
	if(!m_irrNode)
		assert("Error during motion state creation. No irrlicht node engaged");
}

void CMotionState::getWorldTransform(btTransform &worldTrans)
{
	worldTrans = 	m_centerOfMassOffset.inverse() * m_graphicsWorldTrans ;

}
void CMotionState::setWorldTransform(const btTransform &worldTrans)
{
	if(m_irrNode)
	{
		m_irrNode->setPosition(bullet::bulletVectorToIrrVector(worldTrans.getOrigin()));
		//Seems to be more precise
		m_irrNode->setRotation(bullet::bulletTransformToIrrRotation(worldTrans));
		//m_irrNode->setRotation(QuaternionToIrrEuler(worldTrans.getRotation()));
		m_graphicsWorldTrans = worldTrans * m_centerOfMassOffset ;

	}
	else
	{
		assert("Error during motion state updating.");
	}

}

CMotionState::~CMotionState()
{
}
