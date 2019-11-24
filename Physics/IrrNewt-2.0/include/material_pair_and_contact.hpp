// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_MATERIAL_PAIR_AND_CONTACT_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#define IRRNEWT_FRAMEWORK_MATERIAL_PAIR_AND_CONTACT_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#pragma once

#include "contact.hpp"
#include "material_pair.hpp"
#include <Newton.h>

namespace irr{
	namespace newton{

		//!material pair and contact. Using Multiple inheritance.
		class IRRNEWT_API IMaterialPairAndContact:public irr::newton::IMaterialPair,public irr::newton::IMaterialContact{
		public:
			//!
			//!constructor. don't call this directly
			IMaterialPairAndContact():IMaterialPair(),IMaterialContact(){}

			//!
			//!constructor. don't call this directly
			IMaterialPairAndContact(
				NewtonMaterial* material_to_set,
				NewtonContactsProcess* contact_to_set):
			IMaterialPair(material_to_set),IMaterialContact(contact_to_set){}

			//!Calculate the speed of this contact along the normal vector of the contact
			//!\return Contact speed. A positive value means the contact is repulsive
			irr::f32 getContactNormalSpeed();

			//!Calculate the speed of this contact along the tangent vector of the contact
			//!\param index to the tangent vector. This value can be 0 for primary tangent direction or 1 for the secondary tangent direction.
			//!\return Contact tangent speed
			irr::f32 getContactTangentSpeed(bool index);
		};
	}
}

#endif
