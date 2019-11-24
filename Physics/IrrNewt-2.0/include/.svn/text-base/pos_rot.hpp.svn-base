// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_POS_ROT_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#define IRRNEWT_FRAMEWORK_POS_ROT_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#pragma once

#include <irrlicht.h>
#include "dll_exports.hpp"

namespace irr{
	namespace newton{

		//!\brief This class is used to store position and rotation.
		//!
		//!I write it to management a thing that require position and 
		//!rotation but not scale. So using a matrix4 will be increase used RAM without reasons
		struct SPosRot{

			//!default constructor. set position and rotation to 0,0,0
			SPosRot(){}

			//!constructor
			//!\param pos the position
			//!\param rot the rotation
			SPosRot(irr::core::vector3df pos,irr::core::vector3df rot):Position(pos),Rotation(rot) {}

			//!copy contructor
			//!\param p the object to copy from
			SPosRot(const SPosRot& p):Position(p.Position),Rotation(p.Rotation){}

			//!destructor
			~SPosRot() {}

			//make an instance of this class equal to another istance of this class
			SPosRot operator =(SPosRot& p) {
				this->Position=p.Position;
				this->Rotation=p.Rotation;
				return (SPosRot(p));
			}
			
			//!get 2 vectors and add 1 to position and 1 to rotation 
			//!\param pos the vector to add to position
			//!\param rot the vector to add to rotation
			//!\return none
			inline void add(irr::core::vector3df pos,irr::core::vector3df rot) {
				this->Position+=pos;
				this->Rotation+=rot;
			}

			//!get a PosRot object and add it to the current position and rotation
			//!\param p the PosRot object to add
			//!\return none
			inline void add(SPosRot p){
				this->Position+=p.Position;
				this->Rotation+=p.Rotation;
			}

			irr::core::vector3df /*!the position*/Position,/*!the rotation*/Rotation;
		};

	}
}

#endif
