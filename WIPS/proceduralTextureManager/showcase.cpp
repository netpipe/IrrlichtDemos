#include <irrlicht.h>
#include <math.h>
#include "CCrossPlaneSceneNode.h"
#include "CProceduralTextureManager.h"
#include "CGUIDigitViewer.h"

using namespace irr;

int main(){
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL,
                core::dimension2d<s32>(640,480));

        video::IVideoDriver* driver = device->getVideoDriver();
        scene::ISceneManager* scenemgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	ITimer* timer = device->getTimer();
	s32 fps;

	scene::CProceduralTextureManager* ptmgr = new scene::CProceduralTextureManager(scenemgr->getRootSceneNode(), scenemgr, device->getTimer(), -1);
	u8 tid = ptmgr->addFireTexture("textures/fire.tga", 64, 64, 0.95f, 2, 30);
	u8 tid2 = ptmgr->addFireTexture("textures/fire.tga", 64, 64, 0.95f, 1, 30);
	u8 tid3 = ptmgr->addFireTexture("textures/fire.tga", 64, 64, 0.95f, 0, 30);
	u8 tid4 = ptmgr->addFireTexture("textures/arrow.tga", 16, 64, 0.99f, 2, 15);
	u8 tid5 = ptmgr->addFireTexture("textures/stripe.tga", 16, 64, 0.98f, 0, 25);
	u8 tid6 = ptmgr->addTestTexture(2,128,15);
	u8 tid7 = ptmgr->addFireTexture("textures/water.tga", 32, 32, 1.0f, 4, 20);

	gui::CGUIDigitViewer* fpscounter = new gui::CGUIDigitViewer(guienv, guienv->getRootGUIElement(), -1, &fps, 5, "digits/", "tga", core::rect<s32>(0,0,40,16));
	fpscounter->drop();

	scene::ISceneNode* nodea = new scene::CCrossPlaneSceneNode(scenemgr->getRootSceneNode(), scenemgr, -1, 0.5f);
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(10.0f, 25.0f, 10.0f));
		nodea->setRotation(core::vector3df(0.0f, 0.0f, 0.0f));
		if (tid < 8) {
			printf("yes\n");
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid));
		}

	} nodea->drop();
	nodea = new scene::CCrossPlaneSceneNode(scenemgr->getRootSceneNode(), scenemgr, -1, 0.5f);
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(10.0f, 25.0f, 10.0f));
		nodea->setRotation(core::vector3df(0.0f, 45.0f, 0.0f));
		if (tid2 < 8) {
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid));
		}
	} nodea->drop();
	nodea = new scene::CCrossPlaneSceneNode(scenemgr->getRootSceneNode(), scenemgr, -1, 0.5f, core::vector3df(-20.0f, 0.0f, 0.0f));
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(10.0f, 25.0f, 10.0f));
		nodea->setRotation(core::vector3df(0.0f, 0.0f, 0.0f));
		if (tid2 < 8) {
			printf("yes\n");
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid2));
		}
	} nodea->drop();
	nodea = new scene::CCrossPlaneSceneNode(scenemgr->getRootSceneNode(), scenemgr, -1, 0.5f, core::vector3df(-20.0f, 0.0f, 0.0f));
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(10.0f, 25.0f, 10.0f));
		nodea->setRotation(core::vector3df(0.0f, 45.0f, 0.0f));
		if (tid2 < 8) {
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid2));
		}
	} nodea->drop();
	nodea = new scene::CCrossPlaneSceneNode(scenemgr->getRootSceneNode(), scenemgr, -1, 0.5f, core::vector3df(20.0f, 0.0f, 0.0f));
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(10.0f, 25.0f, 10.0f));
		nodea->setRotation(core::vector3df(0.0f, 0.0f, 0.0f));
		if (tid2 < 8) {
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid3));
		}
	} nodea->drop();
	nodea = new scene::CCrossPlaneSceneNode(scenemgr->getRootSceneNode(), scenemgr, -1, 0.5f, core::vector3df(20.0f, 0.0f, 0.0f));
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(10.0f, 25.0f, 10.0f));
		nodea->setRotation(core::vector3df(0.0f, 45.0f, 0.0f));
		if (tid2 < 8) {
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid3));
		}
	} nodea->drop();
	nodea = new scene::CCrossPlaneSceneNode(scenemgr->getRootSceneNode(), scenemgr, -1, 3.0f, core::vector3df(10.0f, 0.0f, 20.0f));
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(10.0f, 50.0f, 10.0f));
		nodea->setRotation(core::vector3df(0.0f, 45.0f, 0.0f));
		if (tid2 < 8) {
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid4));
		}
	} nodea->drop();
	nodea = new scene::CCrossPlaneSceneNode(scenemgr->getRootSceneNode(), scenemgr, -1, 1.0f, core::vector3df(10.0f, 0.0f, -20.0f));
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(10.0f, 50.0f, 10.0f));
		nodea->setRotation(core::vector3df(0.0f, 45.0f, 0.0f));
		if (tid2 < 8) {
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid5));
		}
	} nodea->drop();

	nodea = scenemgr->addCubeSceneNode(10.0f, 0, -1, core::vector3df(-10.0f, 5.0f, -20.0f));
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setRotation(core::vector3df(0.0f, 45.0f, 0.0f));
		if (tid2 < 8) {
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid6));
		}
	}

	nodea = scenemgr->addCubeSceneNode(10.0f, 0, -1, core::vector3df(-10.0f, 25.0f, 20.0f));
	if (nodea){
		nodea->setMaterialFlag(video::EMF_LIGHTING, false);
		nodea->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		nodea->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		nodea->setScale(core::vector3df(1.0f, 5.0f, 1.0f));
		nodea->setRotation(core::vector3df(180.0f, 0.0f, 0.0f));
		if (tid2 < 8) {
			nodea->setMaterialTexture(0, ptmgr->getTexture(tid7));
		}
	}
	scene::ICameraSceneNode* camera = scenemgr->addCameraSceneNode(0, core::vector3df(0.0f,0.0f,0.0f), core::vector3df(0.0f,15.0f,0.0f));

	f32 omega = 0.0005f;
	core::vector3df campos = core::vector3df(0.0f, 0.0f, 0.0f);
	f32 dist = 45.0f;
	u32 dt = 0;
	f32 alpha = 0.0f;
	u32 prevtime = timer->getTime();

	while (device->run() && driver){

		dt = timer->getTime() - prevtime;
		prevtime += dt;

		fps = driver->getFPS();

		alpha += dt * omega;
		campos = core::vector3df(cos(alpha)*dist, 25.0f, sin(alpha)*dist);
		camera->setPosition(campos);

		driver->beginScene(true, true, video::SColor(255,0,0,0));
		scenemgr->drawAll();
		guienv->drawAll();
		driver->endScene();
	}
	ptmgr->drop();
}
