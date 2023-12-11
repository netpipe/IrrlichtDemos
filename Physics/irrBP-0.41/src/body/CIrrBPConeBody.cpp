#include "body/CIrrBPConeBody.h"

CIrrBPConeBody::CIrrBPConeBody(irr::scene::ISceneNode * node,irr::f32 mass, irr::s32 bodyId,BODY_OR bodyOrientationAxis)
{
   m_IrrSceneNode = node;
   m_BodyId = bodyId;
   irr::core::vector3df edges[8];
   node->getBoundingBox().getEdges(edges);
   
   irr::core::vector3df TScale = node->getScale();

   
   irr::core::vector3df Extent;
   Extent = node->getBoundingBox().getExtent();

   irr::f32 radius,height;
   m_MotionState = new CMotionState(this,bullet::getTransformFromIrrlichtNode(node));

   BODY_OR coneOr = bodyOrientationAxis;

   if(coneOr == AUTO)
   {
   		   if(Extent.X == Extent.Y && Extent.X == Extent.Z) //not recognizable. so take default axis
				coneOr = Y;
		   else if(Extent.Y == Extent.Z)
				coneOr =  X;
		   else if(Extent.X == Extent.Z)
				coneOr = Y;
		   else if(Extent.X == Extent.Y)
				coneOr = Z;
		   else
				coneOr = Y;
   }

   switch(coneOr)
   {
		case X:
			radius = fabs(edges[1].Y-edges[0].Y)/2.0f;
			height = fabs(edges[4].X-edges[0].X);

			radius *= TScale.Y;
			height *= TScale.X;

			m_Shape = new btConeShapeX(radius,height);
			break;
		case Y:

		    radius = fabs(edges[4].X-edges[0].X)/2.0f;
			height = fabs(edges[1].Y-edges[0].Y);

			radius *= TScale.X;
			height *= TScale.Y;
			
			m_Shape = new btConeShape(radius,height);
			break;
		case Z:

			radius = fabs(edges[4].X-edges[0].X)/2.0f;
			height = fabs(edges[2].Z-edges[0].Z);

			radius *= TScale.X;
			height *= TScale.Z;

			m_Shape = new btConeShapeZ(radius,height);
			break;
   }
  
   
   btVector3 LocalInertia;
   m_Shape->calculateLocalInertia(mass, LocalInertia);

   m_RigidBody = new btRigidBody(mass, m_MotionState, m_Shape, LocalInertia);
  
   collisionObj = m_RigidBody;
   setAutomaticCCD();
   m_RBtype = CONE;

}
