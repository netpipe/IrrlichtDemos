#include "constraint/CIrrBPConstraint.h"
#include "body/CIrrBPRigidBody.h"

btTypedConstraint * CIrrBPConstraint::getConstraintPtr()
{
	return m_Constraint;
}

CIrrBPConstraint::CIrrBPConstraint()
{
	m_bodyA = NULL;
	m_bodyB = NULL;
	m_Constraint = NULL;
}

CIrrBPConstraint::~CIrrBPConstraint()
{
	if(m_Constraint)
		delete m_Constraint;
}

