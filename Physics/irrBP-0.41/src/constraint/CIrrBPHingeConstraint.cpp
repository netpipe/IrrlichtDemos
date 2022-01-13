#include "constraint/CIrrBPHingeConstraint.h"
#include "body/CIrrBPRigidBody.h"

CIrrBPHingeConstraint::CIrrBPHingeConstraint(CIrrBPRigidBody * bodyA,const irr::core::vector3df & pivotInA,const irr::core::vector3df & axisInA)
{
	m_Constraint = new btHingeConstraint(*(bodyA->getBodyPtr()),bullet::irrVectorToBulletVector(pivotInA),bullet::irrVectorToBulletVector(axisInA));
	this->m_bodyA = bodyA;
	this->m_axisA = axisInA;
	this->m_pivotA = pivotInA;
}

CIrrBPHingeConstraint::CIrrBPHingeConstraint(CIrrBPRigidBody * bodyA,CIrrBPRigidBody * bodyB,const irr::core::vector3df & pivotInA,const irr::core::vector3df & pivotInB,const irr::core::vector3df & axisInA,const irr::core::vector3df & axisInB)
{
	m_Constraint = new btHingeConstraint(*(bodyA->getBodyPtr()),*(bodyB->getBodyPtr()),
											bullet::irrVectorToBulletVector(pivotInA),bullet::irrVectorToBulletVector(pivotInB),
											bullet::irrVectorToBulletVector(axisInA),bullet::irrVectorToBulletVector(axisInB));
	this->m_bodyA = bodyA;
	this->m_axisA = axisInA;
	this->m_pivotA = pivotInA;
	this->m_bodyB = bodyB;
	this->m_axisB = axisInB;
	this->m_pivotB = pivotInB;
}
