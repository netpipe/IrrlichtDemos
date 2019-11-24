// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef IRRNEWT_LIBRARY_PHYSICS_VEHICLE_SIMPLE_HPP_CODE_161698ASDVAVADFSADFWF168165168ASVASDAD
#define IRRNEWT_LIBRARY_PHYSICS_VEHICLE_SIMPLE_HPP_CODE_161698ASDVAVADFSADFWF168165168ASVASDAD

#include "dll_exports.hpp"
#include "body.hpp"
#include "pos_rot.hpp"
#include "joint.hpp"
#include "structures.hpp"

namespace irr{
	namespace newton{

		class IVehicleTire;
		struct SPosRot;

		namespace Hidden {
                  void VehicleTireCallBack(const NewtonJoint* vehicle);
        }

		//!Class to management a vehicle
		class IRRNEWT_API IVehicleSimple : public irr::newton::IJoint{

		public:
			//!
			//!costructor, don't call this directly
			IVehicleSimple(const IVehicleSimple& v);

			//!
			//!costructor, don't call this directly
			IVehicleSimple(
				irr::newton::IWorld* world,
				irr::newton::IBody* chassis,
				irr::core::vector3df up_dir);

			//destructor
			virtual ~IVehicleSimple();

			/*
			susShock = 30.0;
			susSpring = 200.0;
			susLength = 1.2;
			*/

			/*!
			add a tire to the vehicle
			After the application creates the vehicle joint, it must add the tires.
			the application should calculate the correct tire parameters, like tire mass,
			position, width height, spring and damper constants.
			\param tire_node the scene node associated with the tire
			\param pin the rotation axis of the tire, in the space of the tire. It may be (0,0,1)
			\param mass tire mass, must be much smaller than the vehicle body. ratio of 50:1 to 100:1 are the recommended values
			\param width width of the tire, must be smaller than the tire radius
			\param radius tire radius
			\param body_scene_node_offset the position and the rotation offset of the tire relative to the tire scene node (tire_node)
			\param sus_shock parametrized damping constant for a spring, mass, damper system. A value of one corresponds to a critically damped system.
			\param sus_spring parametrized spring constant for a spring, mass, damper system. A value of one corresponds to a critically damped system.
			\param sus_length distance from the tire set position to the upper stop on the vehicle body frame. The total suspension length is twice that.
			\return the new tire
			*/
			irr::newton::IVehicleTire* createTire(
				irr::scene::ISceneNode* tire_node,
				irr::core::vector3df pin,
				irr::f32 mass,
				irr::f32 width,
				irr::f32 radius,
				irr::newton::SPosRot body_scene_node_offset=irr::newton::SPosRot(),
				irr::f32 sus_shock=30.0f,
				irr::f32 sus_spring=200.0f,
				irr::f32 sus_length=1.2f);

			irr::newton::IVehicleTire* createTire(irr::newton::STire tire);

			//!get first tire of the vehicle tire set
			//\return the first tire
			irr::newton::IVehicleTire* getFirstTire();

			//!Get the next tire on the tire set
			//!\param current_tire the current tire
			//!\return the tire after the current tire
			irr::newton::IVehicleTire* getNextTire(irr::newton::IVehicleTire* current_tire);

			//!Get all tires of the vehicle
			//!\return an array contain all vehicle tires. an empty array indicates that you don't add tires to the vehicle. To add a tire see VehicleSimple::addTire()
			irr::core::array<irr::newton::IVehicleTire*> getAllTires();

			//!Reset all tires velocities to zero.
			//!This function is useful for reposition the vehicle
			//!\return none
			void reset();

			void removeWheels();

			//get chassis
			inline irr::newton::IBody* getChassis() {return this->chassis;}

			inline irr::newton::IWorld* getWorld() {return (irr::newton::IWorld*)NewtonWorldGetUserData(this->n_world);}

			inline NewtonWorld* getNewtonWorld() {return this->n_world;}


		private:

			//!
			//!newton vehicle callback
			friend void irr::newton::Hidden::VehicleTireCallBack(const NewtonJoint* vehicle);
			friend class IVehicleTire;

			void reserved_destroy();

			//!
			//!intialize vehicle variables to default values
			void Initialize();

			//!
			//!the chassis body
			irr::newton::IBody* chassis;

			//!
			//!the newton world
			NewtonWorld* n_world;

			//!
			//!total numbers of tires
			unsigned char tires_num;

		};

	}//newton
}//irr

#endif
