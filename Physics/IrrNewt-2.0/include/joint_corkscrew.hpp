// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef PHYSICS_IRRLICHT_LIB_JOINT_CORKSCREW_HPP_16468716AGFA
#define PHYSICS_IRRLICHT_LIB_JOINT_CORKSCREW_HPP_16468716AGFA
#pragma once

namespace irr {
	namespace newton {

		//class IJointCorkscrew
		class IRRNEWT_API IJointCorkscrew:public irr::newton::IJoint{

		protected:
			void reserved_destroy() {this->~IJointCorkscrew();}

		public:
			//!
			//!default constructor. don't call this directly
			IJointCorkscrew():IJoint() {}
						//!
			//!constructor. don't call this directly
			IJointCorkscrew(
				irr::newton::IWorld* world,
				irr::newton::SJointCorkscrew jointData);

			//!
			//!destructor
			virtual ~IJointCorkscrew(){}
		};
		//corkscrew joint
	}
}

#endif
