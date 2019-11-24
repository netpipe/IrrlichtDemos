// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef PHYSICS_IRRLICHT_LIB_JOINTCUSTOM_GEAR_HPP
#define PHYSICS_IRRLICHT_LIB_JOINTCUSTOM_GEAR_HPP

#include "joint_custom.hpp"

namespace irr {
	namespace newton {

		//!NOT IMPLEMENTED
		class IRRNEWT_API IJointCustomGear:public irr::newton::IJointCustom{
		protected:

			void reserved_destroy() {this->~IJointCustomGear();}

		public:

			//!
			//!default constructor. don't call this directly
			IJointCustomGear(
				f32 gearRatio,
				core::vector3df childPin,
				core::vector3df parentPin,
				newton::IBody* child,
				newton::IBody* parent);


			//!
			//!destructor
			virtual ~IJointCustomGear(){}

			virtual void submitConstrainst() = 0;

		};


	}//namespace newton
}//namespace irr

#endif
