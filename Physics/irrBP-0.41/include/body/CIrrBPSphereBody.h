#ifndef CIrrBP_SPH_BODY_H_
#define CIrrBP_SPH_BODY_H_
#include "body/CIrrBPRigidBody.h"

class CIrrBPSphereBody : public CIrrBPRigidBody
{
public:
	virtual void drop() { delete this;}
	CIrrBPSphereBody(irr::scene::ISceneNode * node,irr::f32 mass, irr::s32 bodyId = -1);

};

#endif