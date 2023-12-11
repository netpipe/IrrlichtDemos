#ifndef CIrrBP_HINGE_CONST_H_
#define CIrrBP_HINGE_CONST_H_

#include "constraint/CIrrBPConstraint.h"

class CIrrBPHingeConstraint : public CIrrBPConstraint
{
public:
		CIrrBPHingeConstraint(CIrrBPRigidBody * bodyA,const irr::core::vector3df & pivotInA,const irr::core::vector3df & axisInA);
		CIrrBPHingeConstraint(CIrrBPRigidBody * bodyA,CIrrBPRigidBody * bodyB,const irr::core::vector3df & pivotInA,const irr::core::vector3df & pivotInB,const irr::core::vector3df & axisInA,const irr::core::vector3df & axisInB);
		void drop(){delete this;}
protected:
	irr::core::vector3df m_axisA;
	irr::core::vector3df m_axisB;
};
#endif