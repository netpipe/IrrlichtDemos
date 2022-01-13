#ifndef CIrrBP_CT_CONST_H_
#define CIrrBP_CT_CONST_H_

#include "constraint/CIrrBPConstraint.h"

class CIrrBPConeTwistConstraint: public CIrrBPConstraint
{
public:
	CIrrBPConeTwistConstraint(CIrrBPRigidBody * bodyA,CIrrBPRigidBody * bodyB,const irr::core::vector3df & pivotInA=irr::core::vector3df(0,0,0),const irr::core::vector3df & pivotInB=irr::core::vector3df(0,0,0));
	CIrrBPConeTwistConstraint(CIrrBPRigidBody * bodyA,const irr::core::vector3df & pivotInA=irr::core::vector3df(0,0,0));
	void drop() { delete this; }
	void setLimit (int limitIndex, irr::f32 limitValue) { m_fixedConstraint->setLimit(limitIndex,limitValue); }
	void setLimit (irr::f32 _swingSpan1, irr::f32 _swingSpan2, irr::f32 _twistSpan, irr::f32 _softness=1.f, irr::f32 _biasFactor=0.3f, irr::f32 _relaxationFactor=1.0f) 
	{		m_fixedConstraint->setLimit( _swingSpan1, _swingSpan2, _twistSpan, _softness, _biasFactor, _relaxationFactor);	}
private:
	btConeTwistConstraint * m_fixedConstraint;
};

#endif