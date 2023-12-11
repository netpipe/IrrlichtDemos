#ifndef CIrrBP_CAP_BODY_H_
#define CIrrBP_CAP_BODY_H_
#include "body/CIrrBPRigidBody.h"

class CIrrBPCapsuleBody : public CIrrBPRigidBody
{
	public:
	virtual void drop() { delete this;}
	CIrrBPCapsuleBody(irr::scene::ISceneNode * node,irr::f32 mass, irr::s32 bodyId = -1,BODY_OR bodyOrientationAxis=AUTO);
};

#endif