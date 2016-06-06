//CWorldObject.cpp:
//Represents an object in the world, and can be managed by the World Object Manager

#include "utils.h"
#include "WorldObjectMgr.h"
#include "WorldObject.h"

#include "irrlicht.h"
using namespace irr;
using namespace core;

CWorldObject::CWorldObject()
{
	m_pNode = 0;
}

CWorldObject::~CWorldObject()
{
}



//do any scenemanager cleanup here
void CWorldObject::Cleanup(){
	if (m_pNode) m_pNode->remove();
}


void CWorldObject::UpdateTransform(void)
{
	vector3df pos = CBulletPhysicsObject::GetPosition();
	vector3df rot = CBulletPhysicsObject::GetRotation();
	CIrrlichtObject::SetPosition(pos);
	CIrrlichtObject::SetRotation(rot);
}


