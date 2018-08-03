/*
 * Target.cpp
 *
 *  Created on: 08-12-2011
 *      Author: Marcin Załuski
 */

#include "Target.h"
#include <iostream>

void Target::show() {
	// Add the billboard.
	if (isVisible) {
		return;
	} else {

		isVisible = false;
		marker = device->getSceneManager()->addBillboardSceneNode();
		marker->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		marker->setMaterialTexture(0,
				device->getVideoDriver()->getTexture("media/particle.bmp"));
		marker->setMaterialFlag(video::EMF_LIGHTING, false);
		marker->setMaterialFlag(video::EMF_ZBUFFER, false);
		marker->setSize(core::dimension2d<f32>(20.0f, 20.0f));
		marker->setPosition(position);
		std::cout << "MZ " << __FILE__<<" : " << __LINE__ << " target "
				<< this->position.X << ", " << position.Y << ", " << position.Z
				<< std::endl;
		isVisible = true;
	}

}
core::vector3df Target::getPosition() {
	return position;
}

core::vector3df Target::getAbsolutePosition() {
	return marker->getAbsolutePosition();
}

Target::~Target() {
	// TODO Auto-generated destructor stub
}
void Target::setPosition(const core::vector3df &nposition) {
	position = nposition;
//	std::cout << "MZ " << __FILE__<<" : " << __LINE__ << " target "
//			<< this->position.X << ", " << position.Y << ", " << position.Z
//			<< std::endl;
	marker->setPosition(position);
}
