#ifndef CIrrBP_P2P_CONST_H_
#define CIrrBP_P2P_CONST_H_
#include "constraint/CIrrBPConstraint.h"


class CIrrBPP2PConstraint : public CIrrBPConstraint
{
public:
	CIrrBPP2PConstraint(CIrrBPRigidBody * bodyA,const irr::core::vector3df & pivotInA);
	CIrrBPP2PConstraint(CIrrBPRigidBody * bodyA, CIrrBPRigidBody * bodyB, const irr::core::vector3df & pivotInA, const irr::core::vector3df & pivotInB);
	void drop(){delete this;}
};
#endif