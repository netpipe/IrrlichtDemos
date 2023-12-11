#include "constraint/CIrrBPConeTwistConstraint.h"
#include "body/CIrrBPRigidBody.h"

CIrrBPConeTwistConstraint::CIrrBPConeTwistConstraint(CIrrBPRigidBody * bodyA,CIrrBPRigidBody * bodyB,const irr::core::vector3df & pivotInA,const irr::core::vector3df & pivotInB)
{
	/*Setting object's members...*/
	this->m_bodyA = bodyA;
	this->m_pivotA = pivotInA;
	this->m_bodyB = bodyB;
	this->m_pivotB = pivotInB;
	
	//Body Transformation
	btTransform transf1,transf2;

	transf1 = transf1.getIdentity();
	transf2 = transf2.getIdentity();

	//Set origins...
	transf1.setOrigin(bullet::irrVectorToBulletVector(pivotInA));
	transf2.setOrigin(bullet::irrVectorToBulletVector(pivotInB));


	m_fixedConstraint = new btConeTwistConstraint(*bodyA->getBodyPtr(),*bodyB->getBodyPtr(),transf1,transf2);
	m_Constraint = m_fixedConstraint;
}
CIrrBPConeTwistConstraint::CIrrBPConeTwistConstraint(CIrrBPRigidBody * bodyA,const irr::core::vector3df & pivotInA)
{
	/*Setting object's members...*/
	this->m_bodyA = bodyA;
	this->m_pivotA = pivotInA;
	
	//Body Transformation
	btTransform transf1;

	transf1 = transf1.getIdentity();

	//Set origins...
	transf1.setOrigin(bullet::irrVectorToBulletVector(pivotInA));


	m_fixedConstraint = new btConeTwistConstraint(*bodyA->getBodyPtr(),transf1);
	m_Constraint = m_fixedConstraint;
}