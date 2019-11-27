/*

LightEmitter - cpp file

*/

#include "LightBillboard.h"


// constructor
cLightBillboard::cLightBillboard() {
}
	
// destructor
cLightBillboard::~cLightBillboard() {
}
	
// Initialization
void cLightBillboard::Initialize(IrrlichtDevice *setDevice) {
	device=setDevice;
}

// add a new light
void cLightBillboard::addLight(core::vector3df position, core::vector3df normal, f32 angle) {
	
	scene::ISceneManager *smgr = device->getSceneManager();
	scene::IBillboardSceneNode *billboard = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(18,18));
	
	billboard->setMaterialFlag(video::EMF_LIGHTING, false);
	billboard->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	billboard->setMaterialTexture(0, device->getVideoDriver()->getTexture("media/effects/bloom.png"));
	billboard->setPosition(position);	

	lightArray.push_back(billboard);
	lightNormal.push_back(normal);
	lightAngle.push_back(angle);
}

// draw light billboards
void cLightBillboard::drawLight(scene::ICameraSceneNode *camera) {

	core::vector3df cameraLook = camera->getTarget()-camera->getPosition();

	f32 alpha;
	for (u32 i=0; i<lightArray.size(); i++) {

		f32 f = cameraLook.dotProduct(lightNormal[i]);
		alpha = acos(f / (lightNormal[i].getLength() * cameraLook.getLength()));
		alpha*=180/3.14159f;
		
		u32 intensity;
		if (alpha <= lightAngle[i])
			intensity=0;
		else {
			alpha-=lightAngle[i];
			intensity = (u32) (255 * alpha/lightAngle[i]);
		}
		
		lightArray[i]->setColor(video::SColor(255,intensity,intensity,intensity),
								 video::SColor(255,intensity,intensity,intensity));
	}

}
