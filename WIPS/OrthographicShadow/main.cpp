#include <irrlicht.h>
#include "effectWrapper.h"
#include "IOrthoControllerAnimator.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

int main()
{
	IrrlichtDevice* device = createDevice(EDT_OPENGL, dimension2di(800,600), 32);
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();
	
	IMeshSceneNode* ground = smgr->addCubeSceneNode();
	ground->setScale(vector3df(200,1,200));
	smgr->getMeshManipulator()->makePlanarTextureMapping(ground->getMesh(),2);

	ground->getMaterial(0).setTexture(0,driver->getTexture("media/rockwall.bmp"));
	ground->getMaterial(0).Lighting = true;

	ICameraSceneNode* orthoCam = smgr->addCameraSceneNode();
	orthoCam->setPosition(vector3df(300,400,0));
	orthoCam->setTarget(vector3df(0,0,0));

	matrix4 projMat;
	projMat.buildProjectionMatrixOrthoLH(650,500,10,1000);
	orthoCam->setProjectionMatrix(projMat);

	orthoCam->setIsOrthogonal(true);

	IAnimatedMeshSceneNode* dwarf = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/dwarf.ms3d"));
	dwarf->setPosition(vector3df(0,8,0));
	dwarf->getMaterial(0).Lighting = false;

	IOrthoControllerAnimator* orthoAnimator = new IOrthoControllerAnimator(smgr);
	dwarf->addAnimator(orthoAnimator);
	device->setEventReceiver(orthoAnimator);

	effectHandler* effect = new effectHandler(device,dimension2di(1024,1024));
	effect->setShadowDarkness(1.0f);

	effect->addShadowToNode(dwarf, EFT_8PCF, ESM_BOTH);
	effect->addShadowToNode(ground, EFT_8PCF, ESM_RECEIVE);
	effect->setMaxShadowDistanceFromLight(1.0f);
	projMat.buildProjectionMatrixOrthoLH(800,850,100,600);
	effect->getLightCamera()->setProjectionMatrix(projMat);
	

	for(u32 i = 0;i < 3;++i)
	{
		IMeshSceneNode* house = smgr->addMeshSceneNode(smgr->getMesh("media/house.3ds"));
		house->setScale(vector3df(0.2f,0.2f,0.2f));
		house->setPosition(vector3df(200 * i,0,-200));
		house->setRotation(vector3df(0,90,0));
		house->getMaterial(0).setTexture(0,driver->getTexture("media/fw12b.jpg"));
		house->setAutomaticCulling(EAC_FRUSTUM_BOX);
		house->getMaterial(0).Lighting = false;
		effect->addShadowToNode(house,EFT_8PCF,ESM_BOTH);
	}

	for(u32 i = 0;i < 3;++i)
	{
		IMeshSceneNode* house = smgr->addMeshSceneNode(smgr->getMesh("media/house.3ds"));
		house->setScale(vector3df(0.2f,0.2f,0.2f));
		house->setPosition(vector3df(200,0,200 * i));
		house->getMaterial(0).setTexture(0,driver->getTexture("media/fw12b.jpg"));
		house->setAutomaticCulling(EAC_FRUSTUM_BOX);
		house->getMaterial(0).Lighting = false;
		effect->addShadowToNode(house,EFT_8PCF,ESM_BOTH);
	}
	
	int lastFPS = 0;

	while(device->run())
	{
		driver->beginScene(true,true,SColor(0,0,0,0));
		
		if(driver->getFPS() != lastFPS)
		{
			stringw title = "Orthographic shadow test FPS: ";
			title += driver->getFPS();

			device->setWindowCaption(title.c_str());
		}

		orthoCam->setPosition(dwarf->getPosition() + vector3df(200,400,0));
		orthoCam->setTarget(dwarf->getPosition());

		effect->setLightPosition(dwarf->getPosition() + vector3df(-200,400,0));
		effect->setLightTarget(dwarf->getPosition());

		effect->update();

		driver->endScene();
	}

	device->drop();
	device = NULL;

	return 0;
}