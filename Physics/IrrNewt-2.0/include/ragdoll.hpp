// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#ifndef IRRNEWT_FRAMEWORK_RAGDOLL_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#define IRRNEWT_FRAMEWORK_RAGDOLL_HPP_416516816AVAVASDAWDW651686SDBRHSTSADFAWQEW
#pragma once

#include "irrnewt_base.hpp"
#include "dll_exports.hpp"
#include "body.hpp"

#include <irrlicht.h>
#include <Newton.h>

/*namespace irr{
	namespace newton{

		//class IWorld;
		class IRagDollBone;

		//!Class for ragdol
		class IRRNEWT_API IRagDoll:public irr::newton::IrrNewt_Base{

		protected:

			friend class IWorld;

			int last_id;
			int head_bone_id;

			NewtonRagDoll* n_ragdoll;
			irr::newton::IWorld* world;
			virtual void reserved_destroy() {this->~IRagDoll();}

			void BuildRadDoll(
				NewtonRagDoll* ragDoll,
				NewtonRagDollBone* parentDollBone,
				irr::scene::ISceneNode* bone,
				irr::core::vector3df veloc,
				irr::f32 boneMass);

			void build(
				 scene::ISceneNode* ragdoll,
				 irr::f32 bonesMass);

		public:

			virtual ~IRagDoll();
			IRagDoll(irr::newton::IWorld* world);

			//!get newton ragdoll
			inline const NewtonRagDoll* getNewtonRagDoll() const{return n_ragdoll;}

			//!get a bone from its id. All bones are numerated from 0
			IRagDollBone* findBone(int id);

			//!get all bones of this ragdoll
			core::array<irr::newton::IRagDollBone*> getBones();

			//!set a force to all bones bodies
			void setForceToBones(core::vector3df value);

			//!set torque to all bones bodies
			void setTorqueToBones(core::vector3df value);

			//!add recursively a force to all bone bodies
			void addForceContinuousToBones(core::vector3df value);

			//!set freeze to all bones bodies
			void setFreezeToBones(bool value);

			//!set auto frezee to all bone bodies
			void setAutoFreezeToBones(bool value);

			//!set a material to all bone bodies
			void setMaterialToBones(newton::IMaterial* material);

			//!get the total bone count of this ragdoll
			inline int getBoneCount() {return this->last_id - 1;}

		};

		//!Class to management a rag doll bone
		class IRRNEWT_API IRagDollBone:public irr::newton::IrrNewt_Base{
		protected:

			irr::newton::IBody* ragdoll_bone_body;
			NewtonRagDoll* n_ragdoll;
			NewtonRagDollBone* n_bone;

			friend class IRagDoll;

			void reserved_destroy() {this->~IRagDollBone();}

		public:

			IRagDollBone(NewtonRagDollBone* this_bone):n_bone(this_bone) {}
			IRagDollBone():n_bone(NULL) {}
			virtual ~IRagDollBone() {}

			//!get the newton ragdoll related with this bone
			inline NewtonRagDoll* getNewtonRagDoll() const{return n_ragdoll;}

			//!get the newton object for this ragdoll bone
			inline NewtonRagDollBone* getNewtonRagDollBone() const{return n_bone;}

			//!get the body of this rag doll bone
			irr::newton::IBody* getBody();

			//!ge tthe node of this ragdoll bone
			irr::scene::ISceneNode* getNode();


		};

	}
}*/

#endif
