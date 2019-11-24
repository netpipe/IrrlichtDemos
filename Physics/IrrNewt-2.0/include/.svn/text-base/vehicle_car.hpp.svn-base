// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef IRRNEWT_LIBRARY_PHYSICS_VEHICLE_CAR_HPP_CODE_161698ASDVAVADFSADFWF168165168ASVASDAD
#define IRRNEWT_LIBRARY_PHYSICS_VEHICLE_CAR_HPP_CODE_161698ASDVAVADFSADFWF168165168ASVASDAD

#include "dll_exports.hpp"
#include "vehicle_simple.hpp"
#include "vehicle_tire.hpp"
#include "pos_rot.hpp"
#include "joint.hpp"
#include "structures.hpp"

namespace irr{
	namespace newton{

		class IVehicleTire;
		class IWorld;
		struct SPosRot;
		struct SCar;

		//!Class to management a vehicle
		class IRRNEWT_API ICar:public irr::newton::IVehicleSimple{

			irr::f32 steer_angle_increment;
			irr::f32 torque_increment;

			irr::newton::IVehicleTire* fr_tire;
			irr::newton::IVehicleTire* fl_tire;
			irr::newton::IVehicleTire* br_tire;
			irr::newton::IVehicleTire* bl_tire;

		public:

			//!
			//!costructor, don't call this directly
			ICar(irr::newton::IWorld* world,SCar car);

			//destructor
			virtual ~ICar() {}

            void createWheels(irr::newton::IWorld* world, SCar car);

			//!Get front right tire
			inline irr::newton::IVehicleTire* getFRTire() {return this->fr_tire;}

			//!Get front left tire
			inline irr::newton::IVehicleTire* getFLTire() {return this->fl_tire;}

			//!Get back right tire
			inline irr::newton::IVehicleTire* getBRTire() {return this->br_tire;}

			//!Get back left tire
			inline irr::newton::IVehicleTire* getBLTire() {return this->bl_tire;}

			//!Set the max value steer angle can have for all tires
			inline void setMaxSteerAngle(irr::f32 value) {
				this->fr_tire->setMaxSteerAngle(value);
				this->fl_tire->setMaxSteerAngle(value);
				this->fr_tire->setMinSteerAngle();
				this->fl_tire->setMinSteerAngle();
			}

			//!Set the max value torque caqm have for all tires
			inline void setMaxTorque(irr::f32 value) {
				this->br_tire->setMaxTorque(value);
				this->bl_tire->setMaxTorque(value);
				this->br_tire->setMinTorque();
				this->bl_tire->setMinTorque();
			}

			//!set steer angle increment
			inline void setSteerAngleIncrement(irr::f32 value) {this->steer_angle_increment = value;}

			//!set car accelleration
			inline void setTorqueIncrement(irr::f32 value) {this->torque_increment = value;}

			inline void turnRight() {

				irr::f32 factor = this->getWorld()->getTimeElapsed();

				this->fr_tire->addSteerAngle( this->steer_angle_increment * factor );
				this->fl_tire->addSteerAngle( this->steer_angle_increment * factor );

			}

			inline void turnLeft() {

				irr::f32 factor = this->getWorld()->getTimeElapsed();

				this->fr_tire->addSteerAngle( -this->steer_angle_increment * factor );
				this->fl_tire->addSteerAngle( -this->steer_angle_increment * factor );

			}

			//!set sterr angle to 0
			//!	param drastic if false set steer angle gradually to 0. how gradually depends on steer angle increment. If true set steer angle directly to 0
			inline void noTurn(bool drastic = false) {

				irr::f32 steer = this->getFRTire()->getSteerAngle();
				irr::f32 steer_increment = this->steer_angle_increment * this->getWorld()->getTimeElapsed();

				if(steer<0) steer += steer_increment;
				if(steer>0) steer -= steer_increment;
				steer -= this->getFRTire()->getSteerAngle();

				if( !drastic ) {
					this->fr_tire->addSteerAngle(steer);
					this->fl_tire->addSteerAngle(steer);
				}

				else {
					this->fr_tire->setSteerAngle(0);
					this->fl_tire->setSteerAngle(0);
				}

			}

			//!accelerate
			inline void forward() {
				this->br_tire->addTorque(-this->torque_increment);
				this->bl_tire->addTorque(-this->torque_increment);
			}

			//!decellerate
			inline void backward() {
				this->br_tire->addTorque(this->torque_increment);
				this->bl_tire->addTorque(this->torque_increment);
			}

			//!set speed to 0
			inline void noMove() {
				this->br_tire->setTorque(0);
				this->bl_tire->setTorque(0);
			}

		};

	}//newton
}//irr

#endif
