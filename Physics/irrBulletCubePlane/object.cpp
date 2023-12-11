#include "object.h"
#include "regularMotionState.h"


Object::Object(const btScalar& radius, const btTransform& anglePos, const btScalar& mass, irr::scene::ISceneNode* node)
{
	this->m_node = node;
	this->createPhysicsInfo(radius, anglePos, mass);
}

Object::Object(){}
//I originally didn't store the nodes inside of the class and used spheres.
void Object::createPhysicsInfo(const btScalar &radius, const btTransform &anglePos, const btScalar &mass)
{
	this->m_collisionShape = new btSphereShape(radius);
	this->m_motionState = new regularMotionState(anglePos, m_node);
	btVector3 inertia(0,0,0);
	this->m_collisionShape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(mass, this->m_motionState, this->m_collisionShape, inertia);
	this->m_rigidBody = new btRigidBody(rigidBodyInfo);
}

btRigidBody* Object::getBody()
{
	return this->m_rigidBody;
}

btMotionState* Object::getMotionState()
{
	return this->m_motionState;
}

irr::scene::ISceneNode* Object::getNode()
{
	return this->m_node;
}

//Object::~Object()
//{
//	delete this->m_collisionShape;
//	delete this->m_motionState;
//	delete this->m_rigidBody;
//	delete this->m_node;
//
//	
//
//
//};