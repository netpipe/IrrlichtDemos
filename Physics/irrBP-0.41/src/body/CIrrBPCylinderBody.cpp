#include "body/CIrrBPCylinderBody.h"
CIrrBPCylinderBody::CIrrBPCylinderBody(irr::scene::ISceneNode * node,irr::f32 mass, irr::s32 bodyId, BODY_OR bodyOrientationAxis)
{
   m_IrrSceneNode = node;
   m_BodyId = bodyId;
   
   irr::core::vector3df Extent;
   Extent = node->getBoundingBox().getExtent();

   irr::core::vector3df TScale = node->getScale();
   	//Calculate the half edges
   irr::core::vector3df HalfEdges(Extent.X/2.0f,Extent.Y/2.0f,Extent.Z/2.0f);

   m_MotionState = new CMotionState(this,bullet::getTransformFromIrrlichtNode(node));

   
   btVector3 HalfExtents(TScale.X * HalfEdges.X, TScale.Y * HalfEdges.Y, TScale.Z * HalfEdges.Z);
   
   /*Searching Cone orientation*/
   BODY_OR cylOr = bodyOrientationAxis;
   if(cylOr == AUTO)
   {
	   if(HalfEdges.X == HalfEdges.Y && HalfEdges.X == HalfEdges.Z) //not recognizable. so take default axis (irrlicht' X , bullet' Z)
		   cylOr = X;
	   else if(HalfEdges.Y ==  HalfEdges.Z)
		   cylOr = X;
	   else if(HalfEdges.X == HalfEdges.Y)
		   cylOr = Z;
	   else if(HalfEdges.X == HalfEdges.Z)
		   cylOr = Y;
	   else //Also not recognizable. take default axis
		   cylOr = X;

   }
   switch(cylOr)
   {
	   case X:
		   m_Shape = new btCylinderShapeZ(HalfExtents);
		   break;
	   case Y:
		   m_Shape = new btCylinderShape(HalfExtents);
		   break;
	   case Z:
		   m_Shape = new btCylinderShapeX(HalfExtents);
		   break;
   }
   

   btVector3 LocalInertia;
   m_Shape->calculateLocalInertia(mass, LocalInertia);

   m_RigidBody = new btRigidBody(mass, m_MotionState, m_Shape, LocalInertia);

   collisionObj = m_RigidBody;
   setAutomaticCCD();
   m_RBtype = CYLINDER;
}
