// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_MATERIAL_COLLISION_CALLBACK_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#define IRRNEWT_FRAMEWORK_MATERIAL_COLLISION_CALLBACK_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#pragma once

#include <stddef.h>
#include "dll_exports.hpp"

namespace irr{
	namespace newton{
              
              class IBody;
              class IMaterialPair;
              class IMaterialPairAndContact;

		//!Callbacks for collision between two materials
		class IRRNEWT_API IMaterialCollisionCallback{
		public:

			//!The 2 bodies will collide soon.
			//!\param material_pair the materials of the two bodies
			//!\param body0 the first body wich collide
			//!\param body1 the second body wich collide
			//!\return 1 to accept collision, 0 to refuse collision
			virtual int ContactBegin(
				irr::newton::IMaterialPair* material_pair,
				irr::newton::IBody* body0,
				irr::newton::IBody* body1) {return 1;}

			//!The 2 bodies is colliding
			//!\param material_pair_and_contact the materials of the two bodies and the material contact object
			//!\return 1 to accept collision, 0 to refuse collision
			virtual int ContactProcess(
				irr::newton::IMaterialPairAndContact* material_pair_and_contact
				) {return 1;}

			//!The 2 bodies has collided but now they don't collide
			//!\param material_pair the materials of the two bodies
			//!\return none
			virtual void ContactEnd(
				irr::newton::IMaterialPair* material_pair
				) {}
		};
	}
}

#endif


