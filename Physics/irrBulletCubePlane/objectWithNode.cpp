#include "objectWithNode.h"
#include <iostream>


ObjectWithNode::ObjectWithNode(scene::ISceneManager* smgr, const btVector3 &size, const btTransform &anglePos, const btScalar &mass)
{
	this->createNode(smgr, size);
	createPhysicsInfo(size, anglePos, mass);
}


void ObjectWithNode::createNode(scene::ISceneManager* smgr, btVector3 size)
{
	//The size must be a cube or this will not work
	this->m_node = smgr->addCubeSceneNode(size.getX()*2); //Bullet uses half of the size, tell irrlicht it is actually 10
	this->m_node->setMaterialFlag(video::EMF_LIGHTING, true);

}

void ObjectWithNode::createPhysicsInfo(const btVector3 &size, const btTransform &anglePos, const btScalar &mass)
{
	this->m_collisionShape = new btBoxShape(size); 
	this->m_motionState = new regularMotionState(anglePos, m_node);// use the customized motion state
	btVector3 inertia(0,0,0); 
	this->m_collisionShape->calculateLocalInertia(mass, inertia);// yay calculate inertia for us
	btRigidBody::btRigidBodyConstructionInfo rigidBodyInfo(mass, this->m_motionState, this->m_collisionShape, inertia);
	this->m_rigidBody = new btRigidBody(rigidBodyInfo); //new rigid body!!!



}


//Doesn't work
//ObjectWithNode::~ObjectWithNode()
//{
//	delete this->m_node;
//	delete this->m_collisionShape;
// 	delete this->m_motionState;
//	delete this->m_rigidBody;
//};