/*
 * Target.h
 *
 *  Created on: 08-12-2011
 *      Author: Marcin Załuski
 */

#ifndef TARGET_H_
#define TARGET_H_

#include <irrlicht.h>

using namespace irr;

class Target {
public:
	IrrlichtDevice *device;
	core::vector3df position;
	bool isVisible;
	scene::IBillboardSceneNode * marker;

	Target(IrrlichtDevice *dev, const core::vector3df &nposition) :
			device(dev), position(nposition), isVisible(false), marker(NULL) {
	}
	;
	Target(IrrlichtDevice *dev) :
			device(dev), isVisible(false), marker(NULL) {
	}
	;
	Target(IrrlichtDevice *dev, float nx, float ny, float nz) :
			device(dev), position(nx, ny, nz), isVisible(false), marker(NULL) {
	}
	;
	void setPosition(const core::vector3df &nposition);
	core::vector3df getPosition();
	core::vector3df getAbsolutePosition();
	void show();

	virtual ~Target();
};

#endif /* TARGET_H_ */
