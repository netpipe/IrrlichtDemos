// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef PHYSICS_IRRLICHT_LIB_JOINT_SLIDER_HPP_16468716AGFA
#define PHYSICS_IRRLICHT_LIB_JOINT_SLIDER_HPP_16468716AGFA
#pragma once

namespace irr {
	namespace newton {

			//class IJointSlider
		class IRRNEWT_API IJointSlider:public irr::newton::IJoint{

		protected:
			friend irr::u32 irr::newton::Hidden::defJointSliderCallback(const NewtonJoint* n_slider, NewtonHingeSliderUpdateDesc* desc);
			irr::f32 n_joint_posit,n_joint_veloc;
			irr::core::vector3df n_joint_force;

			void reserved_destroy() {this->~IJointSlider();}

		public:
			//!
			//!default constructor. don't call this directly
			IJointSlider():IJoint() {}
						//!
			//!constructor. don't call this directly
			IJointSlider(
				irr::newton::IWorld* world,
				irr::newton::SJointSlider jointData);

			//!
			//!destructor
			virtual ~IJointSlider(){}
		};
		//slider joint


	}
}

#endif
