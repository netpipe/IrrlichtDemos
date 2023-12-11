#ifndef CIrrBP_CONVHB_BODY_H_
#define CIrrBP_CONVHB_BODY_H_

#include "body/CIrrBPRigidBody.h"
class CIrrBPConvexHullBody : public CIrrBPRigidBody
{
public:
	virtual void drop() { delete this;}
	~CIrrBPConvexHullBody();
	CIrrBPConvexHullBody(irr::scene::IMeshSceneNode * node,irr::f32 mass, irr::s32 bodyId = -1);
	CIrrBPConvexHullBody(irr::scene::IAnimatedMeshSceneNode * node,irr::f32 mass, irr::s32 bodyId = -1);
	
private:
	void initializeMesh(irr::scene::IMesh * pMesh,const irr::core::vector3df & pos, const irr::core::vector3df & scale, void * nodePtr,irr::f32 mass);
	btConvexHullShape * m_hullShape;
};

#endif