// WorldObject.h: interface for the CWorldObject class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "BulletPhysicsObject.h"
#include "IrrlichtObject.h"

class CWorldObject : public CBulletPhysicsObject, public CIrrlichtObject
{
private:


protected:

public:
	CWorldObject();
	virtual ~CWorldObject();
	virtual void Cleanup();

	virtual void SetRotation(core::vector3df &v) {

		//clamp
		if (fabs(v.X) > 360) v.X -= ((int) v.X / 360) * 360;
		if (fabs(v.Y) > 360) v.Y -= ((int) v.Y / 360) * 360;
		if (fabs(v.Z) > 360) v.Z -= ((int) v.Z / 360) * 360;
		CBulletPhysicsObject::SetRotation(v);
		CIrrlichtObject::SetRotation(v);
	};

	virtual void SetPosition(const core::vector3df &v){
		CBulletPhysicsObject::SetPosition(v);
		CIrrlichtObject::SetPosition(v);
	};

	//we don't need get position and rotation, since these are only implemented
	//in CBulletPhysicsObject


	virtual void UpdateTransform(void);
};

