/*
IrrNewt irrlicht\newton OOP framework
Copyright (C) 2006 Mancuso Raffaele

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

IrrNewt e-mail: ares_support@virgilio.it
*/

#pragma once
#ifndef IRRNEWT_IRRLICHT_NEWTON_WRAPPER_165168
#define IRRNEWT_IRRLICHT_NEWTON_WRAPPER_165168

#include <irrlicht.h>
#include <Newton.h>

#include "dll_exports.hpp"
#include "hidden.hpp"
#include "world.hpp"
#include "intersection_point.hpp"
//#include "body.hpp"
#include "tree_body.hpp"
#include "collision.hpp"
#include "irrtonewt.hpp"
#include "material.hpp"
#include "irrnewt_base.hpp"
//#include "material_collision_callback.hpp"
#include "material_pair.hpp"
#include "material_pair_and_contact.hpp"
#include "body_controller.hpp"
#include "vehicle_simple.hpp"
#include "vehicle_tire.hpp"
#include "pos_rot.hpp"
#include "collision_manager.hpp"
#include "structures.hpp"
#include "ragdoll.hpp"
#include "utils.hpp"
#include "vehicle_car.hpp"
#include "camera_fps.hpp"

//joints
#include "joint.hpp"
#include "joint_universal.hpp"
#include "joint_upvector.hpp"
#include "joint_hinge.hpp"
#include "joint_corkscrew.hpp"
#include "joint_slider.hpp"
#include "joint_ball.hpp"
#include "joint_custom.hpp"
#include "joint_custom_gear.hpp"


namespace irr{
	namespace newton{

		//!This create the physics world. Call this to initialize newton and irrnewt
		//\param device It is the current irrlicht device
		IRRNEWT_API irr::newton::IWorld* createPhysicsWorld(irr::IrrlichtDevice* device);

		//!a shortcut for createPhysicsWorld()
		inline irr::newton::IWorld* createWorld(irr::IrrlichtDevice* device) {	return createPhysicsWorld(device);	}

		//!IrrNewt print various informations on the log. Using setLogStatus(false) to tell to IrrNewt that you wouldn't like that IrrNewt print informations on the log
		//!viceversa use setLogStatus(true) to tell newton to rpint log informations
		//!\param value true for show informations, false for hide it
		//!\return none
		IRRNEWT_API void setLogStatus(bool value);

		//!Get return log status. see setLogStatus for more informations
		//\return current log status
		//\see irr::newton::setLogStatus()
		IRRNEWT_API bool getLogStatus();

	}//namespace Newt
}//namespace raf

#endif
