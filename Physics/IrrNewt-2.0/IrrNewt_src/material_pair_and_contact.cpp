// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/material_pair_and_contact.hpp"

/*
void irr::newton::IMaterialPairAndContact::setContactElasticity(float value) {
	NewtonMaterialSetContactElasticity(
		this->getNewtonMaterialPair(),
		value);
}
*/

irr::f32 irr::newton::IMaterialPairAndContact::getContactNormalSpeed() {
	// EIGEN FIX THIS
	/*return NewtonMaterialGetContactNormalSpeed(
		this->getNewtonMaterialPair(),
		this->getNewtonMaterialContact());*/

  return 0.f;
}

irr::f32 irr::newton::IMaterialPairAndContact::getContactTangentSpeed(bool index) {
	/*return NewtonMaterialGetContactTangentSpeed(
		this->getNewtonMaterialPair(),
		this->getNewtonMaterialContact(),
		index);*/

  return 0.f;
}

