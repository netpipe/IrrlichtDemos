/*

LightEmitter - header file

*/

#ifndef LightBillboard_h
#define LightBillboard_h

#include <irrlicht.h>

using namespace irr;


class cLightBillboard{
public:
	
	// constructor
	cLightBillboard();
	
	// destructor
	~cLightBillboard();
	
	// Initialization
	void Initialize(IrrlichtDevice *setDevice);
	
	// add a new light
	void addLight(core::vector3df position, core::vector3df normal, f32 angle);
	
	// draw light billboards
	void drawLight(scene::ICameraSceneNode *camera);
	
	
private:
	
	IrrlichtDevice *device;
	
	core::array<scene::IBillboardSceneNode*> lightArray;
	core::array<core::vector3df> lightNormal;
	core::array<f32> lightAngle;

};

#endif
