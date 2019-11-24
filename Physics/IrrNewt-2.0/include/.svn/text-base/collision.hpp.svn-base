// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_COLLISION_HPP
#define IRRNEWT_FRAMEWORK_COLLISION_HPP
#pragma once

#include <Newton.h>
#include "dll_exports.hpp"
#include <irrlicht.h>
#include "irrnewt_base.hpp"
#include "intersection_point.hpp"
#include "stdio.h" //NULL declaration

namespace irr{
	namespace newton{

		//struct SIntersectionPoint;
		class IBody;

		//!Class to check collision and raycasting
		class IRRNEWT_API ICollision:public irr::newton::IrrNewt_Base {

			void reserved_destroy() {this->~ICollision();}

			void create(newton::SBodyFromNode data);
			void create(core::array<newton::ICollision*> collisions);

			friend class IWorld;
			friend class IBody;

			unsigned SkippedFace, PolyCount;

			newton::IWorld* world;
			NewtonCollision* n_collision;
			newton::SBodyFromNode creationData;

			bool isReleased;
			bool isCompound;
			core::vector3df size;

		public:

			ICollision(newton::IWorld* iWorld) {
				this->isReleased=false;
				this->isCompound = false;
				this->world = iWorld;
			}

			virtual ~ICollision() {}

			newton::SBodyFromNode getCreationData() {return this->creationData;}

			newton::E_BODY_TYPE getType() {return this->creationData.Type;}

			core::vector3df getSize() {return this->size * NewtonToIrr;}

			NewtonCollision* getNewtonCollision() {return this->n_collision;}

			inline u32 getSkippedFaceCount() {return this->SkippedFace;}

			inline u32 getPolyCount() {return this->PolyCount;}

			void release();

			irr::f32 calculateVolume();

			void setUserID(u32 ID);
			u32 getUserID();

			void calculateInertialMatrix(
				core::vector3df& inertia,
				core::vector3df& origin);

		};
	}

}



#endif

