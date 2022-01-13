#ifndef CIrrBP_CONE_BODY_H_
#define CIrrBP_CONE_BODY_H_
#include "body/CIrrBPRigidBody.h"

class CIrrBPConeBody : public CIrrBPRigidBody
{
	public:
	virtual void drop() { delete this;}
	CIrrBPConeBody(irr::scene::ISceneNode * node,irr::f32 mass, irr::s32 bodyId = -1,BODY_OR bodyOrientationAxis=AUTO);
};

#endif