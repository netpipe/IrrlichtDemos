#ifndef CIrrBP_CYL_BODY_H_
#define CIrrBP_CYL_BODY_H_
#include "body/CIrrBPRigidBody.h"

class CIrrBPCylinderBody : public CIrrBPRigidBody
{
	public:
	virtual void drop() { delete this;}
	CIrrBPCylinderBody(irr::scene::ISceneNode * node,irr::f32 mass, irr::s32 bodyId = -1,BODY_OR bodyOrientationAxis=AUTO);

};
#endif