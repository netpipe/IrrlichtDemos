#include "actions/CIrrBPRaycastVehicle.h"
#include "actions/wheel.h"

CIrrBPRaycastVehicle::CIrrBPRaycastVehicle(CIrrBPRigidBody *chassis, CIrrBPWorld *world)
{
	m_chassis = chassis;
	m_world = world;

	m_raycaster = new btDefaultVehicleRaycaster(world->getBulletWorldPtr());
	if(!m_raycaster)
		assert(!m_raycaster);

	m_vehicle = new btRaycastVehicle(m_tuning,m_chassis->getBodyPtr(),m_raycaster);
	if(!m_vehicle)
		assert(!m_vehicle);

	m_vehicle->setCoordinateSystem(0,1,2);
	
	m_action = m_vehicle;
}
CIrrBPRaycastVehicle::~CIrrBPRaycastVehicle()
{
	if(m_vehicle)
		delete m_vehicle;
	m_vehicle = NULL;

	if(m_raycaster)
		delete m_raycaster;
	m_raycaster = NULL;
}

const irrBPWheelInfo & CIrrBPRaycastVehicle::addWheel(const irrBPWheelInfo & wheel)
{
	btWheelInfo newwheel = bullet::irrBPWheelInfotoBulletWheelInfo(wheel,wheel.m_cinfo);
	
	m_vehicle->addWheel(newwheel.m_chassisConnectionPointCS,newwheel.m_wheelDirectionCS,
		newwheel.m_wheelAxleCS,newwheel.m_suspensionRestLength1,newwheel.m_wheelsRadius,m_tuning,newwheel.m_bIsFrontWheel);

	return wheel;
	
}
const irrBPWheelInfo & CIrrBPRaycastVehicle::addWheel(const irrBPWheelInfoConstructionInfo & wheelCI)
{
	irrBPWheelInfo wheel(wheelCI);
	return addWheel(wheel);
}
void CIrrBPRaycastVehicle::editWheel(irr::u32 id, const irrBPWheelInfo & wheel)
{
	btWheelInfo & exwheel = m_vehicle->getWheelInfo(id);
	bullet::irrBPWheelInfotoBulletWheelInfo(wheel,exwheel);
}
irrBPWheelInfo CIrrBPRaycastVehicle::getWheel(irr::u32 id)
{
	
	irrBPWheelInfo info;
	bullet::bulletWheelInfotoirrBPWheelInfo(m_vehicle->getWheelInfo(id),info);
	return info;
}

void CIrrBPRaycastVehicle::setBrake(irr::f32 brake, irr::u32 wheelID)
{
	m_vehicle->setBrake(brake,wheelID);
}
void CIrrBPRaycastVehicle::applyEngineForce(irr::f32 force, irr::u32 wheelID)
{
	m_vehicle->applyEngineForce(force,wheelID);
}
irr::f32 CIrrBPRaycastVehicle::getCurrentSpeedKmHour()
{
	return m_vehicle->getCurrentSpeedKmHour();
}
void CIrrBPRaycastVehicle::resetSuspension()
{
	m_vehicle->resetSuspension();
}
irr::f32 CIrrBPRaycastVehicle::getSteeringValue(int wheel) 
{
	return m_vehicle->getSteeringValue(wheel);
}
void CIrrBPRaycastVehicle::setSteeringValue(irr::f32 steering,int wheel)
{
	m_vehicle->setSteeringValue(steering,wheel);
}

void CIrrBPRaycastVehicle::setPitchControl(irr::f32 pitch)
{
	m_vehicle->setPitchControl(pitch);
}
