#include "constraint/CIrrBulletSpringConstraint.h"
#include "body/CIrrBulletRigidBody.h"


CIrrBulletSpringConstraint::CIrrBulletSpringConstraint(CIrrBulletRigidBody *bodyA, CIrrBulletRigidBody *bodyB)
{
	m_bodyA = bodyA;
	m_bodyB = bodyB;

	btTransform frameInA, frameInB;
	frameInA = btTransform::getIdentity();
	frameInA.setOrigin(btVector3(btScalar(10.), btScalar(0.), btScalar(0.)));
	frameInB = btTransform::getIdentity();
	frameInB.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));
	m_fixedConstraint = new btGeneric6DofSpringConstraint(*bodyA->getBodyPtr(),*bodyA->getBodyPtr(),frameInA,frameInB,true);
	btGeneric6DofSpringConstraint * pGen6DOFSpring = m_fixedConstraint;
	pGen6DOFSpring->setLinearUpperLimit(btVector3(5., 0., 0.));
		pGen6DOFSpring->setLinearLowerLimit(btVector3(-5., 0., 0.));

		pGen6DOFSpring->setAngularLowerLimit(btVector3(0.f, 0.f, -1.5f));
		pGen6DOFSpring->setAngularUpperLimit(btVector3(0.f, 0.f, 1.5f));


		pGen6DOFSpring->enableSpring(0, true);
		pGen6DOFSpring->setStiffness(0, 39.478f);
		pGen6DOFSpring->setDamping(0, 0.5f);
		pGen6DOFSpring->enableSpring(5, true);
		pGen6DOFSpring->setStiffness(5, 39.478f);
		pGen6DOFSpring->setDamping(0, 0.3f);
		pGen6DOFSpring->setEquilibriumPoint();
	m_Constraint = m_fixedConstraint;
}
