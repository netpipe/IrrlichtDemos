// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/material_pair.hpp"
#include "../include/hidden.hpp"

void irr::newton::IMaterialPair::setContactElasticity(irr::f32 value) {
	NewtonMaterialSetContactElasticity(
		this->getNewtonMaterialPair(),
		value);
}

void irr::newton::IMaterialPair::setContactSoftness(irr::f32 value) {
	NewtonMaterialSetContactSoftness(
		this->getNewtonMaterialPair(),
		value);
}

void irr::newton::IMaterialPair::setContactFrictionState(
	irr::s32 state,
	bool index) {

	NewtonMaterialSetContactFrictionState(
		this->getNewtonMaterialPair(),
		state,index);

}

void irr::newton::IMaterialPair::disableContact() {
	//NewtonMaterialDisableContact(this->getNewtonMaterialPair());
}

unsigned irr::newton::IMaterialPair::getContactFaceAttribute() {
	return NewtonMaterialGetContactFaceAttribute(this->getNewtonMaterialPair());
}

irr::f32 irr::newton::IMaterialPair::getCurrentTimeStep() {
	return 0.f; //NewtonMaterialGetCurrentTimestep(this->getNewtonMaterialPair());
}

irr::core::vector3df irr::newton::IMaterialPair::getContactPosition() {
	float position_array[3],normal_array[3];

	NewtonMaterialGetContactPositionAndNormal(
		this->getNewtonMaterialPair(),
		position_array,
		normal_array);

	return irr::newton::Hidden::Vector3dfFromArray(position_array);
}

irr::core::vector3df irr::newton::IMaterialPair::getContactNormal() {

	float position_array[3],normal_array[3];

	NewtonMaterialGetContactPositionAndNormal(
		this->getNewtonMaterialPair(),
		position_array,
		normal_array);

	return irr::newton::Hidden::Vector3dfFromArray(normal_array);
}

irr::core::vector3df irr::newton::IMaterialPair::getContactForce() {
	float force_array[3];

	NewtonMaterialGetContactForce(
		this->getNewtonMaterialPair(),
		force_array);

	return irr::newton::Hidden::Vector3dfFromArray(force_array);
}

void irr::newton::IMaterialPair::getContactTangentDirections(
	irr::core::vector3df& primary,
	irr::core::vector3df& secondary) {

	float primary_array[3],secondary_array[3];

	NewtonMaterialGetContactTangentDirections(
		this->getNewtonMaterialPair(),
		primary_array,
		secondary_array);

	primary=irr::newton::Hidden::Vector3dfFromArray(primary_array);
	secondary=irr::newton::Hidden::Vector3dfFromArray(secondary_array);

}
