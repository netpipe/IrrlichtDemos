#ifndef _CIRRBPVEHICLE_H
#define _CIRRBPVEHICLE_H

#include <btBulletDynamicsCommon.h>
#include "body/CIrrBPRigidBody.h"
#include "CIrrBPWorld.h"
#include "actions/wheel.h"
#include "actions/CIrrBPActionInterface.h"

class CIrrBPRaycastVehicle : public CIrrBPActionInterface
{
public:
	CIrrBPRaycastVehicle(CIrrBPRigidBody * chassis, CIrrBPWorld * world);

	/*!
		Adds a wheel to the vehicle
		@param wheel wheel to add
		@return the wheel added
	*/
	const irrBPWheelInfo & addWheel(const irrBPWheelInfo & wheel);
	
	/*!
		Adds a wheel to the vehicle using the constructor info
		@param wheelCI wheel to add
		@return the wheel added
	*/
	const irrBPWheelInfo & addWheel(const irrBPWheelInfoConstructionInfo & wheelCI);

	/*!
		Modifies a wheel.
		@param id id of the wheel
		@param wheel new wheel infos
	*/
	void editWheel(irr::u32 id, const irrBPWheelInfo & wheel);

	/*!
		Gets a wheel
		@param id id of the wheel
		@return the id-th wheel
	*/
	irrBPWheelInfo getWheel(irr::u32 id);

	/*!
		Applies a brake force to a wheel
		@param brake brake force
		@param wheelID id of the wheel
	*/
	void setBrake(irr::f32 brake, irr::u32 wheelID);

	/*!
		Applies an engine force to a wheel
		@param force engine force
		@param wheelID id of the wheel
	*/
	void applyEngineForce(irr::f32 force, irr::u32 wheelID);

	/*!
		Gets the current vehicle speed (in km/h)
		@return vehicle speed
	*/
	irr::f32 getCurrentSpeedKmHour();

	/*!
		Resets the suspension status.
	*/
	void resetSuspension();

	/*!
		Gets the steering value of a wheel
		@param wheel id of the wheel
		@return steering value
	*/
	irr::f32 getSteeringValue(int wheel) ;

	/*!
		Sets the steering value of a wheel
		@param steering steering value
		@param wheel id of the wheel
	*/
	void setSteeringValue(irr::f32 steering,int wheel);

	void setPitchControl(irr::f32 pitch);
	
	void drop() { delete this;}

	btRaycastVehicle * getVehiclePtr(){return m_vehicle;}
	~CIrrBPRaycastVehicle();
private:
	btRaycastVehicle * m_vehicle;
	btRaycastVehicle::btVehicleTuning m_tuning;
	btVehicleRaycaster * m_raycaster;
	CIrrBPRigidBody * m_chassis;
	CIrrBPWorld * m_world;

};

#endif