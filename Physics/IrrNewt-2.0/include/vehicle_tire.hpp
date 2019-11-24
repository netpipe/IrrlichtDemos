// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef IRRNEWT_LIBRARY_PHYSICS_VEHICLE_TIRE_HPP_CODE_161698ASDVAVADFSADFWF168165168ASVASDAD
#define IRRNEWT_LIBRARY_PHYSICS_VEHICLE_TIRE_HPP_CODE_161698ASDVAVADFSADFWF168165168ASVASDAD

#include "dll_exports.hpp"
#include "vehicle_simple.hpp"

#include "world.hpp"


namespace irr{
	namespace newton{

		namespace Hidden{
			void VehicleTireCallBack(const NewtonJoint* vehicle_joint);
		}

		class IPosRot;

		//!Class to management a vehicle tire
		class IRRNEWT_API IVehicleTire:public irr::newton::IrrNewt_Base{
		public:

			//!costructor. Don't call it directly
			IVehicleTire(const IVehicleTire& t);

			//!costructor. Don't call it directly
			IVehicleTire(
				irr::newton::IVehicleSimple* vehicle,
				irr::core::vector3df pin,
				irr::f32 mass,
				irr::f32 width,
				irr::f32 radius,
				irr::f32 sus_shock,
				irr::f32 sus_spring,
				irr::f32 sus_length,
				int colID,
				irr::scene::ISceneNode* tire_scene_node,
				irr::newton::SPosRot body_scene_node_offset
				);

			//destructor
			virtual ~IVehicleTire();

			//!Get the tire torque applyed to the tire axis
			//!\return tire torque
			inline irr::f32 getTorque() {return this->torque;}

			//!Set the tire torque
			//!This function is useful to simulate normal vehicles with wheels that propel
			//by applying torque to the tire axis in order to move
			//!\param value new torque value
			//!\return none
			inline void setTorque(irr::f32 value) {

				this->torque = value;

				if(this->torque > this->max_torque) this->torque = max_torque;
				if(this->torque < this->min_torque) this->torque = min_torque;

			}

			inline void addTorque(irr::f32 value) {	this->setTorque( this->getTorque() + value );	}

			//!Set the max value torque can have
			inline void setMaxTorque(irr::f32 value) {this->max_torque = value;}

			//!Set the min value torque can have
			inline void setMinTorque(irr::f32 value) {this->min_torque = value;}

			//!
			//!set min torque as -max_torque
			inline void setMinTorque() {this->min_torque = -this->max_torque;}


			//steer angle functions


			//!Get the tire steering angle in radians
			//!\return the steering angle in radians
			inline irr::f32 getSteerAngle() {return this->steer_angle;}

			//!Set the tire steering angle
			//!\param value new tire steering angle in radians. can be negative
			inline void setSteerAngle(irr::f32 value) {

				this->steer_angle = value;

				if(this->steer_angle > this->max_steer_angle) this->steer_angle = max_steer_angle;
				if(this->steer_angle < this->min_steer_angle) this->steer_angle = min_steer_angle;
			}

			inline void addSteerAngle(irr::f32 value) {this->setSteerAngle(this->getSteerAngle() + value);}

			//!Set the max value steer angle can have
			inline void setMaxSteerAngle(irr::f32 value) {this->max_steer_angle = value;}

			//!Set the min value steer angle can have
			inline void setMinSteerAngle(irr::f32 value) {this->min_steer_angle = value;}

			//!
			//!set min steer angle as -max_steer_angle
			inline void setMinSteerAngle() {this->min_steer_angle = (-this->max_steer_angle) ; }

			//!Get tire position in global space
			irr::core::vector3df getPosition();
			//!Get tire rotation in global space
			irr::core::vector3df getRotation();
			//!Get tire position and rotation in global space
			irr::newton::SPosRot getPositionRotation();
			//!Get tire matrix in global space
			irr::core::matrix4 getMatrix();

			//!Detach and destroy a tire from the vehicle
			//\return none
			void remove();

			//brakes
			//!This function calculate every time IWorld::update() is called the exact
			//!acceleration needed to be applied to a tire axis in order to bring it to full
			//!stop in one time step
			//! and apply it. You can increase\\decrease this value every loop by using VehicleTire::setBrakeAcceleration() function
			//!\param value true for auto calculate tire max brake acceleration and apply it every loop, false if not
			inline void setCalculateMaxBrakeAcceleration(bool value) {this->auto_calculate_max_brake_acceleration=value;}

			//!Is tire max beake acceleration calculation enabled?
			//!\return true if it is enabled, false if not
			//!\see VehicleTire::setCalculateMaxBrakeAcceleration()
			inline bool getCalculateMaxBrakeAcceleration() {return this->auto_calculate_max_brake_acceleration;}

			//!Set vehicle brake acceleration. If max brake acceleration autmatic calculation is enabled (see VehicleTire::setCalculateMaxBrakeAcceleration())
			//!this value will be added to the calculate max brake acceleration (or substracted if this is a negative value)
			//!
			//!This is a multipurpose function. The more common use is to apply hand or soft
			//! brakes to a vehicle. To apply brakes the application may use the function
			//! VehicleTire::setCalculateMaxBrakeAcceleration() to determine the exact
			//! acceleration needed
			//! to stop the tire from continue to spin in one frame. To simulated the variable
			//!brakes strength the application can use a nominal maximum friction torque (just
			//!like in real life any device will withstand a max value) and modulate this value
			//!with an analog control. For hand brakes the application set the control to
			//!the maximum and for soft brakes it can just modulate the variable friction.
			//!Another use for this function is to simulate rolling friction, For this effect
			//!the application apply the acceleration to stop but with a friction value set to a
			//!minimum non zero fixed value. Note that brakes and tire torque are not mutually
			//!exclusive, the application can apply then simultaneously. As a matter of fact
			//!doing so is quite a satisfying test showing how the vehicles rocks forth and back
			//!due to the engine torque, while the tire prevent it from moving. Another use for
			//!this function is the simulation of track based vehicles. For this the application
			//!apply an arbitrary fix acceleration to the tires on each side of the vehicle.
			//!A function as simple as A = Ad minus Ks x Omega can do the trick, where Ad is the
			//!desire acceleration controlled by the application joystick, Ks is some viscous
			//! velocity damping, and omega is the current tire angular velocity reported by the
			//!	function VehicleTire::getOmega(). To make the vehicle take turns the application
			//!	can elaborate the equation like A = Ad + At minus Ks x Omega where At is the
			//!	differential acceleration supplied by the steering control, for the tires on the
			//!	right side At is positive while for tires of the left side At is negative.
			//\param value the brake acceleration
			//\return none
			inline void setBrakeAcceleration(irr::f32 value) {this->brake_acceleration=value;}

			//!get vehicle brake acceleration. See VehicleTire::setBrakeAcceleration()
			//!\return vehicle brake acceleration
			//!\see VehicleTire::setBrakeAcceleration()
			inline irr::f32 getBrakeAcceleration() {return this->brake_acceleration;}

			//!set brake max friction torque to the tire axis
			//!\param value maximum friction torque the tire brake, or tire motor can withstand
			//!\return none
			//!\see VehicleTire::setBrakeAcceleration()
			inline void setBrakeMaxFrictionTorque(irr::f32 value) {this->brake_max_friction_torque=value;}

			//!get brake max friction torque to the tire axis
			//!\return maximum friction torque the tire brake, or tire motor can withstand
			//!\see VehicleTire::setBrakeAcceleration()
			inline irr::f32 getBrakeMaxFrictionTorque() {return this->brake_max_friction_torque;}
			//end of brakes

			//!Return a boolean value that tells the application if this tire is touching the ground
			//!\return false if the tire touching the ground (it is air borne), true if not
			inline bool isAirBorne() {return this->is_air_borne;}

			//!get newton tire id
			inline const void* getNewtonTireId() const {return this->tire_id;}

			//!get irrlicht scene node associated with this tire
			inline const irr::scene::ISceneNode* getIrrlichtNode() const {return this->tire_node;}

		private:
			void reserved_destroy();

			//!
			//!intialize tire variables to default values
			void Initialize();

			//!
			//!newton vehicle callback
			friend void irr::newton::Hidden::VehicleTireCallBack(const NewtonJoint* vehicle_joint);

			//!
			//!newton tire id
			void* tire_id;

			//!
			//!is air borne taken from newton
			bool is_air_borne;

			//!
			//!tire torque
			irr::f32 torque;

			//!
			//!tire steer angle
			irr::f32 steer_angle;

			//!
			//!max steer angle that can be set
			irr::f32 max_steer_angle;

			//!
			//!min steer angle that can be set
			irr::f32 min_steer_angle;

			irr::f32 max_torque,min_torque;

			irr::newton::IWorld* world;

			bool auto_calculate_max_brake_acceleration;
			irr::f32 brake_acceleration;
			irr::f32 brake_max_friction_torque;

			//!
			//!tire scene node
			irr::scene::ISceneNode* tire_node;

			irr::newton::SPosRot body_node_offset;

			//!
			//!newton vehicle of the tire
			NewtonJoint* n_vehicle;
		};

	}//newton
}//irr

#endif
