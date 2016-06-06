#ifndef __Object__
#define __Object__
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>


using namespace irr;

class Object 
{
public:
	Object(const btScalar& radius, const btTransform& anglePos, const btScalar& mass, scene::ISceneNode* node);
	Object();

	virtual void createPhysicsInfo(const btScalar& radius, const btTransform& anglePos, const btScalar& mass);
	virtual btMotionState* getMotionState();

	virtual btRigidBody* getBody(); //returns the rigidbody
	virtual scene::ISceneNode* getNode(); //returns the irrlicht node

	//~Object();
protected:

	btCollisionShape* m_collisionShape;
	btMotionState* m_motionState;
	btRigidBody* m_rigidBody;

	scene::ISceneNode* m_node;



};

#endif 