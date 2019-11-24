// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef PHYSICS_IRRLICHT_LIB_JOINT_USER_HPP_16ASDAS
#define PHYSICS_IRRLICHT_LIB_JOINT_USER_HPP_16ASDAS

#include "joint.hpp"

namespace irr {
	namespace newton {


		//!NOT IMPLEMENTED
		//Create a custom joint defined by the user
		class IRRNEWT_API IJointCustom:public irr::newton::IJoint{
		protected:

			irr::newton::IBody* body0;
			irr::newton::IBody* body1;
			int maxDOF;

			void reserved_destroy() {this->~IJointCustom();}
			static void  JointDestructor (const NewtonJoint* me);
			static void  NewtonSubmitConstrainst (const NewtonJoint* me);

		public:

			//!
			//!default constructor. don't call this directly
			IJointCustom(
				int MaxDOF,
				irr::newton::IBody* body0,
				irr::newton::IBody* body1);


			//!
			//!destructor
			virtual ~IJointCustom(){}

			//!
			//!get world
			irr::newton::IWorld* getWorld();

			virtual void submitConstrainst() = 0;

		};		//IJOintUser


	}//namespace newton
}//namespace irr

#endif
