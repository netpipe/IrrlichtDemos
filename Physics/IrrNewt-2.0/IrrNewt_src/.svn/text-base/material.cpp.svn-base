// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/material.hpp"
#include "../include/material_collision_callback.hpp"
#include "../include/hidden.hpp"

using namespace irr;

irr::newton::IMaterial::IMaterial(NewtonWorld* newton_world) {
	this->n_world=newton_world;
	this->class_type.Type=ECT_MATERIAL;
	this->material_id=NewtonMaterialCreateGroupID(this->n_world);
}

irr::newton::IMaterial::~IMaterial() {}

void irr::newton::IMaterial::setSoftness(IMaterial* material_2,float value) {
	NewtonMaterialSetDefaultSoftness(this->n_world,
		this->getID(),material_2->getID(),value);
}
void irr::newton::IMaterial::setElasticity(IMaterial* material_2,float value){
	NewtonMaterialSetDefaultElasticity(this->n_world,
		this->getID(),material_2->getID(),value);
}
void irr::newton::IMaterial::setCollidable(IMaterial* material_2,bool statue){
	NewtonMaterialSetDefaultCollidable(this->n_world,
		this->getID(),material_2->getID(),statue);
}
void irr::newton::IMaterial::setFriction(IMaterial* material_2,float static_friction,float kinetic_friction){
	NewtonMaterialSetDefaultFriction(this->n_world,
		this->getID(),material_2->getID(),static_friction,kinetic_friction);
}
void irr::newton::IMaterial::setContinuousCollisionMode(IMaterial* material_2, bool state) {
		NewtonMaterialSetContinuousCollisionMode(
			this->n_world,
			this->getID(),material_2->getID(),
			state);
}


void irr::newton::IMaterial::setCollisionCallback(
	IMaterial* material_2,
	IMaterialCollisionCallback* contact_callback) {

	NewtonMaterialSetCollisionCallback(
		this->n_world,//newton world
		this->getID(),//first material
		material_2->getID(),//second material
		contact_callback,//user data,
		NULL,
		NULL
		);
}
