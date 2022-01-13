#ifndef CIrrBP_6DOF_CONST_H_
#define CIrrBP_6DOF_CONST_H_

#include "constraint/CIrrBPConstraint.h"

class CIrrBP6DOFConstraint: public CIrrBPConstraint
{
public:
	CIrrBP6DOFConstraint(CIrrBPRigidBody * bodyA,CIrrBPRigidBody * bodyB,const irr::core::vector3df & pivotInA=irr::core::vector3df(0,0,0),const irr::core::vector3df & pivotInB=irr::core::vector3df(0,0,0));
	CIrrBP6DOFConstraint(CIrrBPRigidBody * bodyA,const irr::core::vector3df & pivotInA=irr::core::vector3df(0,0,0));
	
	irr::core::vector3df getAxis(int axis_index) const { return bullet::bulletVectorToIrrVector(m_fixedConstraint->getAxis(axis_index));}
	irr::f32 getAngle(int axis_index) const { return m_fixedConstraint->getAngle(axis_index);}

	irr::f32 getRelativePivotPosition(int axis_index) const { return m_fixedConstraint->getRelativePivotPosition(axis_index);}

	bool testAngularLimitMotor(int axis_index) { return m_fixedConstraint->testAngularLimitMotor(axis_index);}

    void	setLinearLowerLimit(const irr::core::vector3df& linearLower)
    {
    	m_fixedConstraint->setLinearLowerLimit(bullet::irrVectorToBulletVector(linearLower));
    }

    void	setLinearUpperLimit(const irr::core::vector3df& linearUpper)
    {
    	m_fixedConstraint->setLinearUpperLimit(bullet::irrVectorToBulletVector(linearUpper));
    }

    void	setAngularLowerLimit(const irr::core::vector3df& angularLower)
    {
		m_fixedConstraint->setAngularLowerLimit(bullet::irrVectorToBulletVector(angularLower));
    }

    void	setAngularUpperLimit(const irr::core::vector3df& angularUpper)
    {
		m_fixedConstraint->setAngularUpperLimit(bullet::irrVectorToBulletVector(angularUpper));
    }


    void setLimit(int axis, irr::f32 lo, irr::f32 hi)
    {
    	m_fixedConstraint->setLimit(axis,lo,hi);
    }

    bool	isLimited(int limitIndex)
    {
    	return m_fixedConstraint->isLimited(limitIndex);
    }


	bool getUseFrameOffset() { return m_fixedConstraint->getUseFrameOffset(); }
	void setUseFrameOffset(bool frameOffsetOnOff) { m_fixedConstraint->setUseFrameOffset(frameOffsetOnOff); }

	void drop() { delete this; }
private:
	btGeneric6DofConstraint* m_fixedConstraint;
};

#endif