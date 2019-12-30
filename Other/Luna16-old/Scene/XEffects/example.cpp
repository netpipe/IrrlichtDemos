//#include <windows.h>
#include <irrlicht.h>
#include <iostream>
#include "effectWrapper.h"


using namespace irr;
using namespace scene;
using namespace video;
using namespace core;
using namespace io;
using namespace gui; // Just for the hell of it...

bool disp2d = 0;

class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver(scene::ICameraSceneNode* cammy)
	{
		Cam = cammy;
	}

	bool OnEvent(const SEvent &event)
	{
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_KEY_T:
				Cam->setFOV(Cam->getFOV() + 0.05f);
				return true;
			case irr::KEY_KEY_G:
				Cam->setFOV(Cam->getFOV() - 0.05f);
				return true;
			case irr::KEY_KEY_Y:
				Cam->setAspectRatio(Cam->getAspectRatio() + 0.05f);
				return true;
			case irr::KEY_KEY_H:
				Cam->setAspectRatio(Cam->getAspectRatio() - 0.05f);
				return true;
			case irr::KEY_KEY_W:
				Cam->setPosition(Cam->getPosition() + vector3df(-5,0,0));
				return true;
			case irr::KEY_KEY_S:
				Cam->setPosition(Cam->getPosition() + vector3df(5,0,0));
				return true;
			case irr::KEY_KEY_A:
				Cam->setPosition(Cam->getPosition() + vector3df(0,0,-5));
				return true;
			case irr::KEY_KEY_D:
				Cam->setPosition(Cam->getPosition() + vector3df(0,0,5));
				return true;

			case irr::KEY_KEY_R:
				disp2d = !(disp2d);
				return true;
			}
		}

		return false;
	}

private:
	scene::ICameraSceneNode* Cam;
};

int main()
{


/*		shadowDimen = dimension2du(1024,1024);
		shadowDimen = dimension2du(2048,2048);
		shadowDimen = dimension2du(4096,4096);
		shadowDimen = dimension2du(2048,2048);
		filterType = EFT_NONE;
		filterType = EFT_4PCF;
		filterType = EFT_8PCF;
		filterType = EFT_4PCF;
*/
	dimension2du shadowDimen = dimension2du(1024,1024);
    E_FILTER_TYPE filterType = EFT_4PCF;
    filterType = EFT_NONE;
////


IrrlichtDevice* device = createDevice(EDT_OPENGL,dimension2du(800,600),32);

ISceneManager* smgr = device->getSceneManager();
IVideoDriver* driver = device->getVideoDriver();

ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,100,.5);
cam->setPosition(vector3df(0,10,0));

// Create the effect handler, passing the device and RTT size (as a dimension2d) .
effectHandler* effect = new effectHandler(device,shadowDimen);


ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("media/terrain.bmp",0,-1,vector3df(0,0,0),vector3df(0,0,0),vector3df(1,1,1),SColor(255,255,255,255),1,ETPS_33);
terrain->setMaterialTexture(0,driver->getTexture("media/terrain.jpg"));
terrain->setMaterialTexture(1, driver->getTexture("media/detailmap.jpg"));
terrain->scaleTexture(1,10);
terrain->setScale(core::vector3df(4, 0.01f, 4));
terrain->setPosition(vector3df(-120,-1.5f,-120));
terrain->setMaterialType(EMT_DETAIL_MAP);

// Make the terrain recieve a shadow with the specified filter type.
// (NOTE: 12PCF is only available in Direct3D9, all others work with OpenGL)
effect->addShadowToNode(terrain,filterType);

IAnimatedMeshSceneNode* sydney;
IAnimatedMesh* sydneymesh = smgr->getMesh("media/dwarf.x");

for(int g = 0;g < 1;g++)
{
	for(int v = 0;v < 3;v++)
	{
		sydney = smgr->addAnimatedMeshSceneNode(sydneymesh);
		sydney->setScale(vector3df(0.05f,0.05f,0.05f));
		sydney->setPosition(vector3df(g * 4,0.5f,v * 4));
		sydney->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);

		sydney->setAutomaticCulling(EAC_FRUSTUM_BOX);

		sydney->setMaterialType(EMT_SOLID);

		// Add the nodes to the depth pass so that they cast a shadow.
		effect->addNodeToDepthPass(sydney);

	}
}


effect->getLightCamera()->addAnimator(smgr->createFlyCircleAnimator(sydney->getPosition() + vector3df(0,15,0),25,0.0002f));

effect->getLightCamera()->setNearValue(5);

effect->setMaxShadowDistanceFromLight(120);

smgr->addLightSceneNode(effect->getLightCamera());


MyEventReceiver receiver(cam);
device->setEventReceiver(&receiver);

// Parent a sphere to the light camera so we can see its position.
smgr->addSphereSceneNode(1,8,effect->getLightCamera())->setMaterialFlag(EMF_LIGHTING,false);



while(device->run())
{
	wchar_t tmp[255];
	swprintf(tmp,255,L"Shadow Map Demo (FPS: %d)",driver->getFPS());
	device->setWindowCaption(tmp);

	driver->beginScene(true,true,SColor(0,0,0,0));

	// Point the light camera at one of the nodes as it rotates around them.
	effect->setLightTarget(sydney->getPosition());

	// Update the effect  handler, remember to always do this BEFORE smgr->drawAll and AFTER beginscene!
	effect->update();

	smgr->drawAll();

	// If user wishes we can display the shadow map to the screen. (May not work in OpenGL)
	if(disp2d)
		driver->draw2DImage(effect->getShadowMapTexture(),position2d<s32>(0,0));

	driver->endScene();
	//	device->sleep(5);
}

device->drop();
return 0;
}

