// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef PHYSICS_IRRLICHT_LIB_JOINT_BALL_HPP_16468716AGFA
#define PHYSICS_IRRLICHT_LIB_JOINT_BALL_HPP_16468716AGFA
#pragma once

namespace irr {
	namespace newton {

		//class IJointBall
		class IRRNEWT_API IJointBall:public irr::newton::IJoint{

		protected:
			void reserved_destroy() {this->~IJointBall();}

		public:
			//!
			//!default constructor. don't call this directly
			IJointBall():IJoint() {}
						//!
			//!constructor. don't call this directly
			IJointBall(
				irr::newton::IWorld* world,
				irr::newton::SJointBall jointData);

			//!
			//!destructor
			virtual ~IJointBall(){}

			//!
			//!Set the ball and socket cone and twist limits
			//!\param pin a unit vector defining the cone axis in global space
			//!\param maxConeAngle max angle in radians the attached body is allow to swing relative to the pin axis, a value of zero will disable this limits
			//!\param maxTwistAngle max angle in radians the attached body is allow to twist relative to the pin axis, a value of zero will disable this limits.
			//!\remarks	limits are disabled at creation time. A value of zero for maxConeAngle disable the cone limit, a value of zero for maxTwistAngle disable the twist limit all non-zero value for maxConeAngle are clamped between 5 degree and 175 degrees
			void setConeLimits(
				core::vector3df pin,
				irr::f32 maxConeAngle,
				irr::f32 maxTwistAngle
				);
		};
		//ball joint

	}
}

#endif
