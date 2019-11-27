//#include "stdafx.h"
#include "SceneNodeAnimatorTA.h"
#define WORLD_SCALE 1.f;

#define PI 3.1415926f
#define DEG2RAD(a) ((a)*PI/180.0f) 
#define RAD2DEG(a) ((a)*180.0f/PI) 


CSceneNodeAnimatorTA::CSceneNodeAnimatorTA(irr::scene::ISceneNode *node, irr::scene::ISceneNode *skeleton, irr::f32 mass, irr::f32 friction, bool player, irr::s32 collissionID) {
	TA::Physics& physics = TA::Physics::GetInstance();

	irr::core::aabbox3df box = skeleton->getBoundingBox();
	irr::core::vector3df extent = box.getExtent() * skeleton->getScale() /2.2;
	irr::core::vector3df center = box.getCenter();
	
	TA::Vec3 start;
	TA::Vec3 end;
	irr::f32 radius;
	
	// max is y
	if (extent.Y > extent.X && extent.Y > extent.Z) {
		start = convertIrrlichtPos(center - irr::core::vector3df(0,extent.Y/1.7f,0));
		end = convertIrrlichtPos(center + irr::core::vector3df(0,extent.Y/1.7f,0));
		radius = (extent.X+extent.Z)/2.f;
	// max is x
	} else if (extent.X > extent.Z) {
		start = convertIrrlichtPos(center - irr::core::vector3df(extent.X/1.7f,0,0));
		end = convertIrrlichtPos(center + irr::core::vector3df(extent.X/1.7f,0,0));
		radius = (extent.Y+extent.Z)/2.f;
	// max is z	
	} else {
		start = convertIrrlichtPos(center - irr::core::vector3df(0,0,extent.Z/1.7f));
		end = convertIrrlichtPos(center + irr::core::vector3df(0,0,extent.Z/1.7f));
		radius = (extent.X+extent.Y)/2.f;
	}

	m_pDynamicObject=TA::DynamicObject::CreateNew();
	TA::AABB aabb(convertIrrlichtPos(center),convertIrrlichtPos(extent));

	if (player)	
		m_pDynamicObject->InitialiseAsACapsule(start, end, radius);
	else
		m_pDynamicObject->InitialiseAsABox(aabb);

	m_pDynamicObject->SetMass(mass);
	m_pDynamicObject->SetRotationDisabled(true);
	m_pDynamicObject->SetFriction(friction);

	irr::core::vector3df pos = node->getPosition();
	irr::core::vector3df rot = node->getRotation();

	TA::MFrame frame(convertIrrlichtPos(pos),convertIrrlichtRot(rot));
	m_pDynamicObject->SetFrame(frame);

	physics.AddDynamicObject(m_pDynamicObject);

	m_pDynamicObject->Release();
	
}

CSceneNodeAnimatorTA::~CSceneNodeAnimatorTA(void) {
}

void CSceneNodeAnimatorTA::animateNode(irr::scene::ISceneNode *node, irr::u32 timeMs) {
	const TA::MFrame &frame = m_pDynamicObject->GetFrame();
	TA::MFrame dynamicObjectFrame = frame;
	node->setPosition(convertTAPos(frame.v3Translation));
	node->setPosition(convertTAPos(m_pDynamicObject->GetPosition()));
	
	irr::core::vector3df rot = convertTARot(frame.m33Rotation);
	
	irr::core::matrix4 mat;
	mat.setRotationDegrees(rot);
	irr::core::vector3df vec = irr::core::vector3df(0,0,1);
	mat.rotateVect(vec);
	
	node->setRotation(irr::core::vector3df(0,vec.getHorizontalAngle().Y,0));
	rot = node->getRotation();

	dynamicObjectFrame.m33Rotation = convertIrrlichtRot(rot);

	m_pDynamicObject->SetFrame(dynamicObjectFrame);
}

TA::Vec3 CSceneNodeAnimatorTA::convertIrrlichtPos(irr::core::vector3df pos) {
	// convert to TA vector
	TA::Vec3 tokpos(pos.X,pos.Y,pos.Z);
	return tokpos * WORLD_SCALE;
}

irr::core::vector3df CSceneNodeAnimatorTA::convertTAPos(const TA::Vec3 &p) {
	//Now, we convert to irrlicht vector
	return irr::core::vector3df(p.x,p.y,p.z) / WORLD_SCALE;
}

TA::Mat33 CSceneNodeAnimatorTA::convertIrrlichtRot(irr::core::vector3df &r) {
	irr::core::matrix4 mat;
	mat.setRotationDegrees(r);
	TA::Mat33 tamat;

	tamat(0,0)=	mat(0,0) ;
	tamat(0,1)=	mat(0,1) ;
	tamat(0,2)=	mat(0,2) ;

	tamat(1,0)=	mat(1,0) ;
	tamat(1,1)=	mat(1,1) ;
	tamat(1,2)=	mat(1,2) ;

	tamat(2,0)=	mat(2,0) ;
	tamat(2,1)=	mat(2,1) ;
	tamat(2,2)=	mat(2,2) ;

	return tamat;
}

irr::core::vector3df CSceneNodeAnimatorTA::convertTARot(const TA::Mat33 &r) {
	irr::core::matrix4 mat;

	mat(0,0) = r(0,0);
	mat(0,1) = r(0,1);
	mat(0,2) = r(0,2);
	mat(0,3) = 0.0f;

	mat(1,0) = r(1,0);
	mat(1,1) = r(1,1);
	mat(1,2) = r(1,2);
	mat(1,3) = 0.0f;

	mat(2,0) = r(2,0);
	mat(2,1) = r(2,1);
	mat(2,2) = r(2,2);
	mat(2,3) = 0.0f;

	mat(3,0) = 0.0f;
	mat(3,1) = 0.0f;
	mat(3,2) = 0.0f;
	mat(3,3) = 1.0f;

	return mat.getRotationDegrees();
}
