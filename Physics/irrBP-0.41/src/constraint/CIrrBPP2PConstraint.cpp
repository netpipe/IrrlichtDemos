#include "constraint/CIrrBPP2PConstraint.h"
#include "body/CIrrBPRigidBody.h"

CIrrBPP2PConstraint::CIrrBPP2PConstraint(CIrrBPRigidBody *  bodyA,const irr::core::vector3df & pivotInA)
{
	m_Constraint = new btPoint2PointConstraint(*(bodyA->getBodyPtr()),bullet::irrVectorToBulletVector(pivotInA));
	this->m_bodyA = bodyA;
	this->m_pivotA = pivotInA;
}
CIrrBPP2PConstraint::CIrrBPP2PConstraint(CIrrBPRigidBody * bodyA, CIrrBPRigidBody * bodyB, const irr::core::vector3df & pivotInA, const irr::core::vector3df & pivotInB)
{
	m_Constraint = new btPoint2PointConstraint(*(bodyA->getBodyPtr()),*(bodyB->getBodyPtr()),bullet::irrVectorToBulletVector(pivotInA),bullet::irrVectorToBulletVector(pivotInB));
	this->m_bodyA = bodyA;
	this->m_pivotA = pivotInA;
	this->m_bodyB = bodyB;
	this->m_pivotB = pivotInB;
}
