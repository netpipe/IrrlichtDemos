// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#pragma once
#ifndef RAFFAELE_MANCUSO_PHYSICS_IRRLICHT_LIB_MATERIALSFDASE_HPP_16468716AGFADVAVEF21658186165ASDVAEVA5168651
#define RAFFAELE_MANCUSO_PHYSICS_IRRLICHT_LIB_MATERIALSFDASE_HPP_16468716AGFADVAVEF21658186165ASDVAEVA5168651


//#include "dll_exports.hpp"
#include "irrnewt_base.hpp"
//#include "material_collision_callback.hpp"

#include <stddef.h>
#include <irrlicht.h>
#include <Newton.h>

namespace irr{
	namespace newton{

	    class IMaterialCollisionCallback;

		//!Class to management rigid bodies materials
		class IRRNEWT_API IMaterial:public irr::newton::IrrNewt_Base {
			//!
			//!newton material id
			int material_id;

			//!
			//!newton world
			NewtonWorld* n_world;

		public:

			//!costrucotr. don't call this directly. use World::createMaterial() to create
			//!a new material
			IMaterial(NewtonWorld* newton_world);

			//!
			//!destructor
			virtual ~IMaterial();

			//!get newton material id
			int getID() {return material_id;}

			//!Set the default softness coefficients
			//!
			//!softnessCoef must be a positive value. It is recommended that softnessCoef be set
			//!to value lower or equal to 1.0 A low value for softnessCoef will make the material
			//!soft. A typical value for softnessCoef is 0.15
			//!\param material_2 the second material wich the function takes effects
			//!\param value softness coefficient
			//!\return none
			void setSoftness(IMaterial* material_2,float value);

			//!Set the coefficients of restitution (elasticity)
			//!
			//!\param material_2 the second material wich the function takes effects
			//!\param value elasticity coefficients, must be a positive value. It is recommended
			//!that value be set to a value lower or equal to 1.0
	        void setElasticity(IMaterial* material_2,float value);


			void setCollidable(IMaterial* material_2,bool state);

			//!Set the default coefficients of friction for the material
			//!
			//!staticFriction and kineticFriction must be positive values. kineticFriction
			//!must be lower than staticFriction. It is recommended that staticFriction and
			//!kineticFriction be set to a value lower or equal to 1.0, however because some
			//!synthetic materials can have higher than one coeficient of friction Newton allows
			//!for the coeficient of friction to be as high as 2.0.
			//!\param material_2 the second material wich the function takes effects
			//!\param static_friction static friction coefient
			//!\param kinetic_friction dynamic friction coefficient
			//!\return none
			void setFriction(IMaterial* material_2,
				float static_friction,float kinetic_friction);

			//!Set material callbacks. When a body with this material colide with an other body of
			//!material_2 IrrNewt calls 3 user defined functions. The first is MaterialCollisionCallback::beginContact(),
			//!the second MaterialCollisionCallback::processContact() and the third MaterialCollisionCallback::endContact()
			//!These functions are virtual, so you can re-implement it to execute your code
			//!
			//!From newton documentation:"When the AABB extend of the collision geometry of two bodies overlap, Newton collision system retrieves the material interaction that defines the behavior between the pair of bodies. The material interaction is collected from a database of materials, indexed by the material gruopID assigned to the bodies. If the material is tagged as non collidable, then no action is taken and the simulation continues. If the material is tagged as collidable, and a beginCallback was set for this material, then the beginCallback function is called. If the function *beginCallback* returns 0, no further action is taken for this material (this can be use to ignore the interaction under certain conditions). If the function *beginCallback* returns 1, Newton proceeds to calculate the array of contacts for the pair of colliding bodies. If the function processCallback was set, the application receives a callback for every contact found between the two colliding bodies. Here the application can perform fine grain control over the behavior of the collision system. For example, rejecting the contact, making the contact frictionless, applying special effects to the surface etc. After all contacts are processed and if the function
			//!endCallback was set, Newton calls endCallback. Here the application can collect information gathered during the contact-processing phase and provide some feedback to the player. A typical use for the material callback is to play sound effects. The application passes the address of structure in the userData along with three event function callbacks. When the function beginCallback is called by Newton, the application resets a variable say maximumImpactSpeed. Then for every call to the function processCallback, the application compares the impact speed for this contact with the value of maximumImpactSpeed, if the value is larger, then the application stores the new value along with the position, and any other quantity desired. When the application receives the call to endCallback the application plays a 3d sound based in the position and strength of the contact."
			//!\param material_2 the second material wich the function takes effects
			//!\param contact_callback pointer to a MaterialCollisionCallback object wich contain the methods that are called if an collision between 2 materials happen
			void setCollisionCallback(
				IMaterial* material_2,
				irr::newton::IMaterialCollisionCallback* contact_callback);

			void setContinuousCollisionMode(IMaterial* material_2, bool state);

		private:
			//!
			//!reserved
			inline virtual void reserved_destroy() {this->~IMaterial();}

		};//class ARES_API Material
	}//namespace newton{
}//namespace irr{

#endif
