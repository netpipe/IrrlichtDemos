#ifndef _CIRRBPVEHICLEWHEEL_H
#define _CIRRBPVEHICLEWHEEL_H

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>

struct	irrBPWheelInfoConstructionInfo
{
	irr::core::vector3df	m_chassisConnectionCS;
	irr::core::vector3df	m_wheelDirectionCS;
	irr::core::vector3df	m_wheelAxleCS;
	irr::f32	m_suspensionRestLength;
	irr::f32	m_maxSuspensionTravelCm;
	irr::f32	m_wheelRadius;
	
	irr::f32		m_suspensionStiffness;
	irr::f32		m_wheelsDampingCompression;
	irr::f32		m_wheelsDampingRelaxation;
	irr::f32		m_frictionSlip;
	irr::f32		m_maxSuspensionForce;
	bool m_bIsFrontWheel;
	
};

struct irrBPWheelInfo
{
	struct RaycastInfo
	{

		irr::core::vector3df	m_contactNormalWS;
		irr::core::vector3df	m_contactPointWS;
		irr::f32	m_suspensionLength;
		irr::core::vector3df	m_hardPointWS;
		irr::core::vector3df	m_wheelDirectionWS; 
		irr::core::vector3df	m_wheelAxleWS; 
		bool		m_isInContact;
	};

	RaycastInfo	m_raycastInfo;
	irrBPWheelInfoConstructionInfo m_cinfo;
	btTransform	m_worldTransform;
	
	irr::core::vector3df	m_chassisConnectionPointCS;
	irr::core::vector3df	m_wheelDirectionCS;
	irr::core::vector3df	m_wheelAxleCS; 
	irr::f32	m_suspensionRestLength1;
	irr::f32	m_maxSuspensionTravelCm;
	irr::f32	m_wheelsRadius;
	irr::f32	m_suspensionStiffness;
	irr::f32	m_wheelsDampingCompression;
	irr::f32	m_wheelsDampingRelaxation;
	irr::f32	m_frictionSlip;
	irr::f32	m_steering;
	irr::f32	m_rotation;
	irr::f32	m_deltaRotation;
	irr::f32	m_rollInfluence;
	irr::f32	m_maxSuspensionForce;

	irr::f32	m_engineForce;

	irr::f32	m_brake;
	
	bool m_bIsFrontWheel;

	//ONLY FOR INTERNAL USE. DON'T USE THIS CONSTRUCTOR
	irrBPWheelInfo()
	{
	}
	irrBPWheelInfo(const irrBPWheelInfoConstructionInfo& ci)

	{

		m_suspensionRestLength1 = ci.m_suspensionRestLength;
		m_maxSuspensionTravelCm = ci.m_maxSuspensionTravelCm;

		m_wheelsRadius = ci.m_wheelRadius;
		m_suspensionStiffness = ci.m_suspensionStiffness;
		m_wheelsDampingCompression = ci.m_wheelsDampingCompression;
		m_wheelsDampingRelaxation = ci.m_wheelsDampingRelaxation;
		m_chassisConnectionPointCS = ci.m_chassisConnectionCS;
		m_wheelDirectionCS = ci.m_wheelDirectionCS;
		m_wheelAxleCS = ci.m_wheelAxleCS;
		m_frictionSlip = ci.m_frictionSlip;
		m_steering = irr::f32(0.);
		m_engineForce = irr::f32(0.);
		m_rotation = irr::f32(0.);
		m_deltaRotation = irr::f32(0.);
		m_brake = irr::f32(0.);
		m_rollInfluence = irr::f32(0.1);
		m_bIsFrontWheel = ci.m_bIsFrontWheel;
		m_maxSuspensionForce = ci.m_maxSuspensionForce;
		m_cinfo = ci;

	}

	irr::f32	m_clippedInvContactDotSuspension;
	irr::f32	m_suspensionRelativeVelocity;
	irr::f32	m_wheelsSuspensionForce;
	irr::f32	m_skidInfo;

};
#endif