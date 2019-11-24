// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef PHYSICS_IRRLICHT_LIB_JOINT_HINGE_HPP_16468716AGFA
#define PHYSICS_IRRLICHT_LIB_JOINT_HINGE_HPP_16468716AGFA
#pragma once

namespace irr {
	namespace newton {

		//class JointHinge
		class IRRNEWT_API IJointHinge:public irr::newton::IJoint{

		protected:
			//callback
			friend irr::u32 irr::newton::Hidden::defJointHingeCallback(const NewtonJoint* n_hinge, NewtonHingeSliderUpdateDesc* desc);
			//call destructor
			void reserved_destroy() {this->~IJointHinge();}

			irr::f32 n_joint_angle,n_joint_omega;
			irr::core::vector3df n_joint_force;

		public:
			//!
			//!default constructor. don't call this directly
			IJointHinge():IJoint() {}
						//!
			//!constructor. don't call this directly
			IJointHinge(
				irr::newton::IWorld* world,
				irr::newton::SJointHinge jointData);

			//!
			//!destructor
			virtual ~IJointHinge(){}

			//!get joint angle
			inline irr::f32 getAngle() {return this->n_joint_angle;}

			//!get joint omega
			inline irr::f32 getOmega() {return this->n_joint_omega;}

			//get force applied to the joint
			inline irr::core::vector3df getForce() {return this->n_joint_force;}
		};
		//hinge joint
	}
}

#endif
