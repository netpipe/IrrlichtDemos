#include "body/CIrrBPSphereBody.h"

CIrrBPSphereBody::CIrrBPSphereBody(irr::scene::ISceneNode *node, irr::f32 mass, irr::s32 bodyId)
{
	m_IrrSceneNode = node;
   m_BodyId = bodyId;

      
   irr::core::vector3df Extent;
   Extent = node->getBoundingBox().getExtent();

	//Calculate the estimated radius.
   irr::f32 estimatedradius =  (Extent.Y * node->getScale().Y)/2.0f;

   m_MotionState = new CMotionState(this,bullet::getTransformFromIrrlichtNode(node));

   m_Shape = new btSphereShape(estimatedradius);
   
   btVector3 LocalInertia;
   m_Shape->calculateLocalInertia(mass, LocalInertia);

   m_RigidBody = new btRigidBody(mass, m_MotionState, m_Shape, LocalInertia);
   
   collisionObj = m_RigidBody;
   setAutomaticCCD();
   m_RBtype = SPHERE;
}
