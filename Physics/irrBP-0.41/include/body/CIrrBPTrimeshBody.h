#ifndef CIrrBP_TRM_BODY_H_
#define CIrrBP_TRM_BODY_H_
#include "body/CIrrBPRigidBody.h"

class CIrrBPTrimesh : public CIrrBPRigidBody 
{
public:
	virtual void drop() { delete this;}
	~CIrrBPTrimesh();
	CIrrBPTrimesh(irr::scene::IAnimatedMeshSceneNode * node,irr::f32 mass, irr::s32 bodyId = -1);
	CIrrBPTrimesh(irr::scene::IMeshSceneNode * node,irr::f32 mass, irr::s32 bodyId = -1);
	const btGImpactMeshShape * getTrimeshShape(){return m_trimeshShape;}
	const btTriangleMesh * getVertexArrays(){return m_indexVertexArrays;}
private:
	btGImpactMeshShape * m_trimeshShape;
	btTriangleMesh* m_indexVertexArrays;

	void initializeMesh(irr::scene::IMesh * mesh,const irr::core::vector3df & pos, const irr::core::vector3df & scale, void * nodePtr,irr::f32 mass);
};
#endif