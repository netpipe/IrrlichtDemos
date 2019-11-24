// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef PHYSICS_IRRLICHT_LIB_JOINT_HPP_16468716AGFADVAVEF21658186165ASDVAEVA5168651
#define PHYSICS_IRRLICHT_LIB_JOINT_HPP_16468716AGFADVAVEF21658186165ASDVAEVA5168651
#pragma once

#include <irrlicht.h>
#include <Newton.h>
#include "dll_exports.hpp"
#include "material_collision_callback.hpp"
#include "irrnewt_base.hpp"

namespace irr{
	namespace newton{

		class MaterialContactCallback;

		//!Class to management joints
		class IRRNEWT_API IJoint:public irr::newton::IrrNewt_Base {
			//!reserved
			inline virtual void reserved_destroy() {this->~IJoint();}

		protected:
			//!
			//!the newton joint
			NewtonJoint* n_joint;

			//!
			//!the newton world
			NewtonWorld* n_world;

		public:
			//!
			//!constructor. don't call this directly
			IJoint();

			//!
			//!destructor
			virtual ~IJoint() {}

			//!Enable or disable collision between the two bodies linked by this joint.
			//!The default state is collision disable when the joint is created.
			//!\param value true to enable collision between linked bodies, false to not
			//!\return none
			void setCollisionState(bool value);

			//!Get the collision state of the two bodies linked by the joint
			//!
			//!usually when two bodies are linked by a joint, the application wants collision
			//!between this two bodies to be disabled. This is the default behavior of joints
			//!when they are created, however when this behavior is not desired the application
			//!can change it by setting collision on. If the application decides to enable
			//!collision between jointed bodies, the application should make sure the collision
			//!geometry do not collide in the work space of the joint.
			//!\return the collision state
			bool getCollisionState();

			//!Set the strength coeficient to be applied to the joint reaction forces
			//!
			//!Constraint keep bodies together by calculating the exact force necessary to
			//!cancel the relative acceleration between one or more common points fixed in the
			//!two bodies. The problem is that when the bodies drift apart due to numerical
			//!integration inaccuracies, the reaction force work to pull eliminated the error
			//!but at the expense of adding extra energy to the system, does violating the rule
			//!that constraint forces must be work less. This is a inevitable situation and the
			//!only think we can do is to minimize the effect of the extra energy by dampening
			//!the force by some amount. In essence the stiffness coefficient tell Newton
			//!calculate the precise reaction force by only apply a fraction of it to the joint
			//!point. And value of 1.0 will apply the exact force, and a value of zero will apply
			//!only 10 percent.
			//!
			//!The stiffness is set to a all around value that work well for most situation,
			//!however the application can play with these parameter to make finals adjustment.
			//!A high value will make the joint stronger but more prompt to vibration of
			//!instability; a low value will make the joint more stable but weaker.
			//!\param value stiffness coeficient, a value between 0, and 1.0, the default value for most joint is 0.9
			void setStiffness(irr::f32 value);

			//!Get the strength coefficient bing applied to the joint reaction forces
			//!See Joint::setStiffness()
			//!\return stiffness coeficient
			//!\see Joint::setStiffness()
			irr::f32 getStiffness();

			//!
			//!get newton joint
			//!\return the newton joint
			inline const NewtonJoint* getNewtonJoint() const {return n_joint;}

		};//class ARES_API Joint


	}//namespace newton{
}//namespace irr{

#endif
