// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef RAFFAELE_MANCUSO_IRRNEWT_LIBRARY_PHYSICS_TREE_BODY_HPP_CODE_ASDGWE5615168168ASFADVFEF86516816ASFT
#define RAFFAELE_MANCUSO_IRRNEWT_LIBRARY_PHYSICS_TREE_BODY_HPP_CODE_ASDGWE5615168168ASFADVFEF86516816ASFT

//#include <irrlicht.h>
//#include <Newton.h>
//#include "dll_exports.hpp"

#include "body.hpp"

namespace irr{
	namespace newton{

  		//!Class to management a body with a tree optimized collision
		class IRRNEWT_API ITreeBody:public irr::newton::IBody {
		public:


			//!constructor. for ICollision
			//!don't call it directly. use World::createTreeBody()
			//!or World::deserializeTreeBody() to create a tree body
			ITreeBody(
				irr::newton::IWorld* world,
				irr::scene::ISceneNode* node,
				irr::newton::ICollision* collision);

			//!constructor for deserialization
			ITreeBody(
				irr::newton::IWorld* world,
				irr::scene::ISceneNode* node,
				core::stringc file);

			//!destructor
			virtual ~ITreeBody();

			//!save the tree body into a file. Use World::deserializeTreeBody() to load it
			//!\param file file name and path
			//!\return none
			void serialize(newton::IWorld* world, irr::core::stringc file);

			//!get the amount of faces skipped from the original mesh because they are too small
			//!you can manipulate this behaviour using IWorld::getInternalBehaviour()
			//!\return the amount of faces skipped
			inline irr::u32 getSkippedFaceCount() {return this->collision->getSkippedFaceCount();}

			//!get total faces in the tree
			inline irr::u32 getPolyCount() {return this->collision->getPolyCount();}

		private:

			inline virtual void reserved_destroy() {this->~ITreeBody();}
			bool deserialize(newton::IWorld* world, irr::core::stringc file);
			void CreateBodyTerrain(irr::s32 LOD);

		};
	}
}

#endif
