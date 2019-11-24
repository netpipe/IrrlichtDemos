// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_CONTACT_HPP_ASDBASEGAEF52616816AVEAWEWASFAS
#define IRRNEWT_FRAMEWORK_CONTACT_HPP_ASDBASEGAEF52616816AVEAWEWASFAS
#pragma once

#include "stdio.h" //NULL declaration and size_t (newton.h require size_t declaration)
#include <Newton.h>
#include "dll_exports.hpp"

namespace irr{
	namespace newton{

		//!Contact material class. Used only in material callbacks
		class IRRNEWT_API IMaterialContact {
			NewtonContactsProcess* newton_contact;
		public:
			IMaterialContact():newton_contact(NULL) {}
			IMaterialContact(NewtonContactsProcess* contact_to_set):newton_contact(contact_to_set){}

			//!get newton material contact
			inline NewtonContactsProcess* getNewtonMaterialContact() {return this->newton_contact;}
		};

	}
}

#endif
