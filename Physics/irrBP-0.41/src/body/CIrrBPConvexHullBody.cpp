#include "body/CIrrBPConvexHullBody.h"

CIrrBPConvexHullBody::CIrrBPConvexHullBody(irr::scene::IAnimatedMeshSceneNode * node,irr::f32 mass, irr::s32 bodyId)
{
	m_IrrSceneNode = node;
	m_BodyId = bodyId;
	initializeMesh(node->getMesh(),node->getPosition(),node->getScale(),node,mass);
}
CIrrBPConvexHullBody::CIrrBPConvexHullBody(irr::scene::IMeshSceneNode * node,irr::f32 mass, irr::s32 bodyId)
{
	m_IrrSceneNode = node;
	m_BodyId = bodyId;
	initializeMesh(node->getMesh(),node->getPosition(),node->getScale(),node,mass);
}
CIrrBPConvexHullBody::~CIrrBPConvexHullBody()
{
	delete this->m_hullShape;
	this->m_hullShape = NULL;
	delete this->m_MotionState;
	this->m_MotionState = NULL;
	delete this->m_RigidBody;
	this->m_RigidBody = NULL;

}
void CIrrBPConvexHullBody::initializeMesh(irr::scene::IMesh * pMesh,const irr::core::vector3df & pos, const irr::core::vector3df & scale, void * nodePtr,irr::f32 mass)
{
	m_hullShape = new btConvexHullShape();
	
	  btVector3 vertices[3];
	  irr::u32 i,j,k,index,numVertices,numIndices;
	  irr::u16* mb_indices;
	  
	  for (i=0; i<pMesh->getMeshBufferCount(); i++)
	  {
		irr::scene::IMeshBuffer* mb=pMesh->getMeshBuffer(i);
		if(mb->getVertexType()==irr::video::EVT_STANDARD)
		{
		  irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();
		  mb_indices = mb->getIndices();
		  numVertices = mb->getVertexCount();
		  numIndices = mb->getIndexCount();
		  for(j=0;j<numIndices;j+=3)
		  {
			for (k=0;k<3;k++)
			{
			  index = mb_indices[j+k];
			  vertices[k] = btVector3(mb_vertices[index].Pos.X*scale.X, mb_vertices[index].Pos.Y*scale.Y, mb_vertices[index].Pos.Z*scale.Z);
			}
			m_hullShape->addPoint(vertices[0]);
			m_hullShape->addPoint(vertices[1]);
			m_hullShape->addPoint(vertices[2]);
		  }
		}
		else if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
		{
		  irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();
		  mb_indices = mb->getIndices();
		  numVertices = mb->getVertexCount();
		  numIndices = mb->getIndexCount();
		  for(j=0;j<numIndices;j+=3)
		  {
			for (k=0;k<3;k++)
			{
			  index = mb_indices[j+k];
			  vertices[k] = btVector3(mb_vertices[index].Pos.X*scale.X, mb_vertices[index].Pos.Y*scale.Y, mb_vertices[index].Pos.Z*scale.Z);
			}
			m_hullShape->addPoint(vertices[0]);
			m_hullShape->addPoint(vertices[1]);
			m_hullShape->addPoint(vertices[2]);
		  }
		}
	  }

	  m_MotionState = new CMotionState(this,bullet::getTransformFromIrrlichtNode(m_IrrSceneNode));
		
	//Calculate Inertia
	btVector3 LocalInertia;
    m_hullShape->calculateLocalInertia(mass, LocalInertia);

	m_RigidBody = new btRigidBody(mass, m_MotionState, m_hullShape,LocalInertia);
	
    m_RigidBody->setUserPointer(nodePtr);
	collisionObj = m_RigidBody;
	setAutomaticCCD();
	m_RBtype = CONVEXHULL;
}