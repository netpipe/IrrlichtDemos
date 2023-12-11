#ifndef __objectWithNode__
#define __objectWithNode__
#include "object.h"
#include "regularMotionState.h"
#include <irrlicht.h>
using namespace irr; 
class ObjectWithNode : public Object
{
public:
	ObjectWithNode(scene::ISceneManager* smgr, const btVector3& size, const btTransform& anglePos, const btScalar& mass);
	virtual void createNode(scene::ISceneManager* smgr, btVector3 size);
	
	virtual void createPhysicsInfo(const btVector3 &size, const btTransform &anglePos, const btScalar &mass);
	~ObjectWithNode();

};

#endif 