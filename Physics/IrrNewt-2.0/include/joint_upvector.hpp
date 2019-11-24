// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef PHYSICS_IRRLICHT_LIB_JOINT_UPVECTOR_HPP_16468716AGFA
#define PHYSICS_IRRLICHT_LIB_JOINT_UPVECTOR_HPP_16468716AGFA

//#include "joint.hpp"

namespace irr {
	namespace newton {

		//!This class is used to management a rotation constraint joint
		class IRRNEWT_API IJointUpVector:public irr::newton::IJoint{
		protected:
					  void reserved_destroy() {this->~IJointUpVector();}
		public:
			//!
			//!default constructor. don't call this directly
			IJointUpVector():IJoint() {}
						//!
			//!constructor. don't call this directly
			IJointUpVector(
				irr::newton::IWorld* world,
				irr::newton::SJointUpVector body);

			//!
			//!destructor
			virtual ~IJointUpVector(){}

			//!Get the up vector pin of this joint in global space.
			//!
			//!the application can call this function to change the joint up vector,
			//!this is useful to animate the up vector. if the application is going to animated
			//!the up vector, it must do so by applying only small rotation, too large rotation
			//!can cause vibration of the joint
			//!\return the up vector direction in global space
			irr::core::vector3df getPin();

			//!Set the up vector pin of this joint in global space.
			//!
			//!the application can call this function to change the joint up vector,
			//!this is useful to animate the up vector. if the application is going to animated
			//!the up vector, it must do so by applying only small rotation, too large rotation
			//!can cause vibration of the joint
			//!\param new_pin the up vector direction in global space.
			//!\return none
			void setPin(irr::core::vector3df new_pin);

		};


	}
}

#endif
