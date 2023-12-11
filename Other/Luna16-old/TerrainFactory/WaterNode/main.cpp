//
// Reflected Water Demo.
// sio2 'at' users.sourceforge.net
// Converted to C++ from the .NET code written by DeusXL (Irrlicht forum)
//

#include <stdio.h>
#include <iostream>
#include <wchar.h>
#include <irrlicht.h>

#include "CReflectedWater.h"

using namespace irr;

using namespace irr::core;
using namespace irr::video;
using namespace irr::scene;

#pragma comment(lib, "Irrlicht.lib")

IrrlichtDevice* _device = 0;
bool _bExit = false;

class CMainOnSetConstants : public irr::video::IShaderConstantSetCallBack
{
public:
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, s32 userData)
	{
		float fVal;
		SColorf col(0.0f,0.0f,0.0f,0.0f);
		if (userData == 1) {
			fVal = 5000000.0f;
			services->setPixelShaderConstant("Radius", &fVal, 1);
			services->setPixelShaderConstant("Ambient", reinterpret_cast<f32*>(&col), 4);
			fVal = 0.0f;
			services->setPixelShaderConstant("Texture", &fVal, 1);
		}
		fVal = 5000000.0f;
		services->setPixelShaderConstant("range", &fVal, 1);
		fVal = 0.0f;
		services->setPixelShaderConstant("decalMap", &fVal, 1);
		fVal = 1.0f;
		services->setPixelShaderConstant("heightMap", &fVal, 1);
	};
};

class CEventReceiver : public IEventReceiver
{
	bool OnEvent(SEvent Event)
	{
		if ((Event.EventType == irr::EET_KEY_INPUT_EVENT) &&
			Event.KeyInput.PressedDown && (Event.KeyInput.Key==KEY_ESCAPE))
		{
			_bExit = true;
			return true;
		}
		return false;
	}
};

int main()
{
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

//	CEventReceiver *_EventReceiver = new CEventReceiver();

	_device = createDevice(driverType, core::dimension2di(800, 600),
		32, false, false, false, 0);

	video::IVideoDriver* _driver = _device->getVideoDriver();
	scene::ISceneManager* _scene = _device->getSceneManager();

	_device->getFileSystem()->changeWorkingDirectoryTo("data");
	_device->getCursorControl()->setVisible(false);

	// wasd navigation
	SKeyMap keyMap[4] = {
		{EKA_MOVE_FORWARD,	KEY_KEY_W},
		{EKA_MOVE_BACKWARD,	KEY_KEY_S},
		{EKA_STRAFE_LEFT,	KEY_KEY_A},
		{EKA_STRAFE_RIGHT,	KEY_KEY_D},
	};
	_scene->addCameraSceneNodeFPS(0, 100.0f, 1000.0f, -1, keyMap, 4);
	_scene->getActiveCamera()->setPosition(core::vector3df(0, 400, -5000));
	float fFar = _scene->getActiveCamera()->getFarValue();
	_scene->getActiveCamera()->setFarValue(fFar *= 1000); // TODO: revise this

	_scene->setShadowColor(video::SColor(150,0,0,0));

	// Mesh "castl3ds/a3dcastl.3ds" converted to Blitz3D format using MilkShape3D
	scene::IMesh *castlemesh = _scene->getMesh("castl3ds/a3dcastl.b3d")->getMesh(0);
	scene::ISceneNode *castlenode = _scene->addMeshSceneNode(castlemesh);
	castlenode->setMaterialFlag(video::EMF_LIGHTING, false);
	castlenode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	castlenode->setScale(core::vector3df(10, 10, 10));
	castlenode->setMaterialTexture(0, _driver->getTexture("rockwall.bmp"));

	CMainOnSetConstants *_MainOnSetConstants = new CMainOnSetConstants;

	int ppxshading = _driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		"ppxshading.vert", "main", irr::video::EVST_VS_1_1, "ppxshading.frag", "main",
		irr::video::EPST_PS_1_1, _MainOnSetConstants, EMT_SOLID, 1);



	core::array<IAnimatedMeshSceneNode*> tinies;
	IAnimatedMesh *tinymesh = _scene->getMesh("tiny/tiny.x");
	/*
	for (int i = -1000; i <= 1000; i += 500)
	{
		IAnimatedMeshSceneNode *tiny = _scene->addAnimatedMeshSceneNode(tinymesh);
		tiny->setMaterialFlag(video::EMF_LIGHTING, false);
		tiny->setAnimationSpeed(5000);
		tiny->setMaterialType((E_MATERIAL_TYPE)ppxshading);
		tiny->setPosition(vector3df((float)i, 300, -2200));
		ISceneNode *hand = tiny->getXJointNode("Bip01_L_Hand");
		IMesh *clevemesh = _scene->getMesh("cleve3ds/a3dcleve.3ds")->getMesh(0);
		ISceneNode *clevenode = _scene->addMeshSceneNode(clevemesh, hand, -1);
		clevenode->setMaterialFlag(video::EMF_LIGHTING, false);
		clevenode->setScale(vector3df(5, 5, 5));
		clevenode->setRotation(vector3df(0, 20, 90));
		clevenode->setPosition(vector3df(70, 0, -40));
		clevenode->setMaterialType((E_MATERIAL_TYPE)ppxshading);
		hand = tiny->getXJointNode("Bip01_R_Hand");
		clevemesh = _scene->getMesh("cleve3ds/a3dcleve.3ds")->getMesh(0);
		clevenode = _scene->addMeshSceneNode(clevemesh, hand, -1);
		clevenode->setMaterialFlag(video::EMF_LIGHTING, false);
		clevenode->setScale(vector3df(5, 5, 5));
		clevenode->setRotation(vector3df(0, 160, 90));
		clevenode->setPosition(vector3df(70, 0, 40));
		clevenode->setMaterialType((E_MATERIAL_TYPE)ppxshading);
		tinies.push_back(tiny);
	}
*/
	tinymesh = _scene->getMesh("dwarf/Dwarf.x");
	for (int i = -1000; i <= 1000; i += 500)
	{
		IAnimatedMeshSceneNode *tiny = _scene->addAnimatedMeshSceneNode(tinymesh);
		tiny->setMaterialFlag(video::EMF_LIGHTING, false);
		tiny->setAnimationSpeed(15);
		tiny->setMaterialType((E_MATERIAL_TYPE)ppxshading);
		tiny->setPosition(vector3df((float)i, 50, -4000));
		tiny->setRotation(vector3df(0, 180, 0));
		tiny->setScale(vector3df(7, 7, 7));
		tinies.push_back(tiny);
	}


	int parallax = _driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		"opengl2.vert", "main", EVST_VS_1_1, "opengl2.frag", "main",
		EPST_PS_1_1, _MainOnSetConstants, EMT_SOLID, 0);

		castlenode->setMaterialType((E_MATERIAL_TYPE)parallax);
//	IMesh *plane = _scene->addHillPlaneMesh("plane", dimension2df(6000, 6000), dimension2di(10, 10),
//		0, 0.0f, dimension2df(0, 0), dimension2df(100, 100))->getMesh(0);

			IMesh *plane = _scene->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(1000,1000),
		core::dimension2d<u32>(40,40), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));


	plane = _scene->getMeshManipulator()->createMeshWithTangents(plane);
	ISceneNode *parallaxplane = _scene->addMeshSceneNode(plane); //, null, -1);
	parallaxplane->setMaterialFlag(EMF_LIGHTING, false);
	parallaxplane->setMaterialTexture(0, _driver->getTexture("rockwall.bmp"));
	parallaxplane->setMaterialTexture(1, _driver->getTexture("rockwall_height.bmp"));
	parallaxplane->setMaterialType((E_MATERIAL_TYPE)parallax);

	CReflectedWater *water = new CReflectedWater("ReflectedWater", _device, _scene, -1, 180, 100, dimension2di(512,512));
	ISceneNode *waternode = water->m_waternode;
	waternode->setPosition(vector3df(0, 50, -3000));
	water->m_WaveDisplacement /= 1.5f;
	water->m_WaveHeight *= 12.0f;
	water->m_WaveSpeed *= 5.0f;
	water->m_RefractionFactor = 0.51f;

	ISceneNode *sky = _scene->addSkyDomeSceneNode(_driver->getTexture("sky.png"), 10, 10, 1, 1.3, NULL);

	int lastfps = -1;
	while (_device->run() && !_bExit)
	{
		if (_device->isWindowActive())
		{
			_driver->beginScene(true, true, SColor(255,100,100,100));
			water->updateRendertarget(_scene);
			_scene->drawAll();
			_driver->endScene();

_device->sleep(10);

			int fps = _driver->getFPS();
			if (fps != lastfps)
			{
				core::stringw str;
				str = L"Irrlicht .NET CP Reflective Water Demo (Conversion to C++) - FPS : ";
				str += fps;
				str += L" Polycount : ";
				str += (int)_driver->getPrimitiveCountDrawn();
				_device->setWindowCaption(str.c_str());
				lastfps = fps;
			}
		}
	}
	_device->drop();
}
