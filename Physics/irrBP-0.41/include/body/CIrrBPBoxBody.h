#ifndef CIrrBP_BOX_BODY_H_
#define CIrrBP_BOX_BODY_H_
#include "body/CIrrBPRigidBody.h"
class CIrrBPBoxBody : public CIrrBPRigidBody
{
	public:
	virtual void drop() { delete this;}
	CIrrBPBoxBody(irr::scene::ISceneNode * node,irr::f32 mass, irr::s32 bodyId = -1);
};
#endif