// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef PHYSICS_IRRLICHT_LIB_JOINT_UNIVERSAL_HPP_16468716AGFAASFWAWEWE
#define PHYSICS_IRRLICHT_LIB_JOINT_UNIVERSAL_HPP_16468716AGFAASFWAWEWE

#include "joint.hpp"

namespace irr {
	namespace newton {

		//class JointUniversal
		class IRRNEWT_API IJointUniversal:public irr::newton::IJoint{

		protected:
			void reserved_destroy() {this->~IJointUniversal();}

		public:
			//!
			//!default constructor. don't call this directly
			IJointUniversal():IJoint() {}
						//!
			//!constructor. don't call this directly
			IJointUniversal(
				irr::newton::IWorld* world,
				irr::newton::SJointUniversal jointData);

			//!
			//!destructor
			virtual ~IJointUniversal(){}
		};
		//universal joint


	}
}

#endif
