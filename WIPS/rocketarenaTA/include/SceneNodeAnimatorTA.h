#pragma once
#include <irrlicht.h>
#include "Physics.h"

enum TA_BODY_TYPE
{
	RIGID_BODY=0,
	ANIMATED_BODY,
	TERRAIN_BODY
};

class CSceneNodeAnimatorTA :
	public irr::scene::ISceneNodeAnimator
{
public:
	CSceneNodeAnimatorTA(irr::scene::ISceneNode *node, irr::scene::ISceneNode *skeleton, irr::f32 mass=.5f, irr::f32 friction=.5f, bool player=false, irr::s32 collissionID = 0);
	~CSceneNodeAnimatorTA(void);

	//! animates the scene node
	virtual void animateNode(irr::scene::ISceneNode *node, irr::u32 timeMs);
//	void OnCollide(neCollisionInfo &info);
	static TA::Vec3 convertIrrlichtPos(irr::core::vector3df pos);
	static irr::core::vector3df convertTAPos(const TA::Vec3 &p);
	static TA::Mat33 convertIrrlichtRot(irr::core::vector3df &rot);
	static irr::core::vector3df convertTARot(const TA::Mat33 &r);
	
//	neRigidBody *m_RigidBody;
//	neAnimatedBody *m_AnimatedBody;
	irr::u32 m_Collided;
	TA::DynamicObject* m_pDynamicObject;
	TA_BODY_TYPE m_Type;
};
