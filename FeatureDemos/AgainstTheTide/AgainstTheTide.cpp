#include "CWaterMesh.h"
#include "CWater.h"
#include "CWaveParticle.h"
#include "XEffects/effectWrapper.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace core;

#pragma comment(lib, "irrlicht.lib")

// Context specific animator.
class CShipSwayAnimator : public irr::scene::ISceneNodeAnimator
{
public:
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs)
	{
		const f32 dTime = timeMs / 500.0f;

		vector3df currRot = node->getRotation();
		currRot.Z = sinf(dTime / 2.0f) * 5.0f + 2.5f;
		currRot.Y = sinf(dTime / 3.0f) * 3.0f;
		node->setRotation(currRot);

		vector3df currPos = node->getPosition();
		currPos.Y = 700.0f + sinf(dTime / 2.0f) * 20.0f;
		node->setPosition(currPos);
	}

	virtual irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node,
		irr::scene::ISceneManager* newManager)
	{
		return new CShipSwayAnimator();
	}
};

// Context specific animator.
class CLightSwingAnimator : public irr::scene::ISceneNodeAnimator
{
public:
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs)
	{
		const f32 dTime = timeMs / 200.0f;

		vector3df currRot = node->getRotation();
		currRot.Z = sinf(dTime / 2.0f) * 6.0f + 2.5f;
		currRot.Y = sinf(dTime / 3.0f) * 6.0f;
		node->setRotation(currRot);
	}

	virtual irr::scene::ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node,
		irr::scene::ISceneManager* newManager)
	{
		return new CLightSwingAnimator();
	}
};

class SailShaderCallback : public irr::video::IShaderConstantSetCallBack
{
public:
	SailShaderCallback(IrrlichtDevice* deviceIn) : device(deviceIn) {}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		f32 Time = device->getTimer()->getTime() / 500.0f;
		services->setVertexShaderConstant("Time", &Time, 1);
	}

	IrrlichtDevice* device;
};

	int main()
	{


	IrrlichtDevice* device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	ICameraSceneNode* camera;
	CWater* water;
	CWaveParticle* waveParticle;
		// Irrlicht Graphics init.
		device = createDevice(EDT_OPENGL, dimension2d<u32>(800, 600), 32);
		driver = device->getVideoDriver();
		smgr = device->getSceneManager();

		device->setWindowCaption(L"Against The Tide");
//		device->setEventReceiver(this);

		camera = smgr->addCameraSceneNodeFPS();
		camera->setFarValue(5000.0f);
		camera->setPosition(vector3df(-1000.0f, 1200.0f, 1000.0f));
		camera->setTarget(vector3df(0.0f, 600.0f, 0.0f));

		dimension2d<u32> tileCount(128, 128);
		water = new CWater(device, dimension2d<f32>(20.0f, 20.0f), tileCount);

		u32 oldFPS = 0;

		IMeshSceneNode* ship = smgr->addMeshSceneNode(smgr->getMesh("media/ship.b3d")->getMesh(0));

		for(u32 i = 0;i < ship->getMaterialCount();++i)
		{
			if(i != 0) ship->getMaterial(i).setTexture(0, driver->getTexture("media/wood.jpg"));
			ship->getMaterial(i).BackfaceCulling = false;
			ship->getMaterial(i).NormalizeNormals = true;
			ship->getMaterial(i).EmissiveColor = SColor(0xffdddddd);
		}

		smgr->getMeshManipulator()->makePlanarTextureMapping(ship->getMesh(), 1.0);
		ship->getMesh()->setHardwareMappingHint(EHM_STATIC);

		SailShaderCallback* sailCallback = new SailShaderCallback(device);
		s32 sailMat = driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
			"shaders/SailV.glsl", "main", EVST_VS_2_0, "shaders/SailP.glsl", "main", EPST_PS_2_0,
			sailCallback);
		sailCallback->drop();

		ship->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)sailMat;

		CShipSwayAnimator* swayAnim = new CShipSwayAnimator();
		ship->addAnimator(swayAnim);
		swayAnim->drop();

		waveParticle = new CWaveParticle(device, ship, -6.8f);

		ship->setScale(vector3df(100.0f, 100.0f, 100.0f));
		ship->setPosition(vector3df(0.0f, 700.0f, 0.0f));

		ISceneNode* lightHandle = smgr->addEmptySceneNode(ship);
		lightHandle->setPosition(vector3df(-1.1f, 1.4f, -1.5f));

		CLightSwingAnimator* lightSwingAnimator = new CLightSwingAnimator();
		lightHandle->addAnimator(lightSwingAnimator);
		lightSwingAnimator->drop();

		ISceneNode* lightShaft = smgr->addCubeSceneNode(0.1f, lightHandle);
		lightShaft->setScale(vector3df(0.5f, 30.0f, 0.5f));
		lightShaft->setPosition(vector3df(0.0f, -1.55f, 0.0f));

		ISceneNode* lightBox = smgr->addCubeSceneNode(0.3f, lightShaft);
		lightBox->setPosition(vector3df(0.0f, -0.05f, 0.0f));
		lightBox->setScale(vector3df(2.0f, 1.0f / 30.0f, 2.0f));
		lightBox->getMaterial(0).setTexture(0, driver->getTexture("media/wood.jpg"));
		lightBox->getMaterial(0).EmissiveColor = SColor(0xffaaaacc);

		ISceneNode* lightSpark = smgr->addBillboardSceneNode(lightBox, dimension2df(50.0f, 50.0f));
		lightSpark->setPosition(vector3df(0.0f, -0.2f, 0.0f));
		lightSpark->getMaterial(0).setTexture(0, driver->getTexture("media/particlewhite.bmp"));
		lightSpark->getMaterial(0).Lighting = false;
		lightSpark->getMaterial(0).MaterialType = EMT_TRANSPARENT_ADD_COLOR;

		effectHandler* effect = new effectHandler(device, dimension2du(1024, 1024), "shaders");
		effect->setClearColour(SColor(255, 32, 32, 64));

		effect->addShadowLight(
			SShadowLight(vector3df(), vector3df(), SColorf(3.0, 3.0, 3.0, 1.0), 0.01f, 600.0f));

		effect->addShadowToNode(ship, EFT_4PCF);
		effect->setAmbientColor(SColor(0xffaaaacc));
		effect->excludeLayer(0);

		ISceneNode* cube1 = smgr->addCubeSceneNode(0.5f, ship, -1, vector3df(0.4f, -5.4f, 0.0f));
		cube1->setRotation(vector3df(0.0f, 50.0f, 0.0f));
		ISceneNode* cube2 = smgr->addCubeSceneNode(0.5f, ship, -1, vector3df(0.8f, -5.9f, 0.1f));
		ISceneNode* cube3 = smgr->addCubeSceneNode(0.5f, ship, -1, vector3df(0.0f, -5.9f, 0.0f));

		cube1->getMaterial(0).setTexture(0, driver->getTexture("media/wood.jpg"));
		cube1->getMaterial(0).Lighting = false;
		cube2->getMaterial(0).setTexture(0, driver->getTexture("media/wood.jpg"));
		cube2->getMaterial(0).Lighting = false;
		cube3->getMaterial(0).setTexture(0, driver->getTexture("media/wood.jpg"));
		cube3->getMaterial(0).Lighting = false;

		effect->addShadowToNode(cube1, EFT_4PCF);
		effect->addShadowToNode(cube2, EFT_4PCF);
		effect->addShadowToNode(cube3, EFT_4PCF);

		while(device->run())
		{
			if(oldFPS != driver->getFPS())
			{
				stringw WindowTitle = "Against The Tide FPS: ";
				WindowTitle += driver->getFPS();
				device->setWindowCaption(WindowTitle.c_str());
			}

			driver->beginScene(true, true, SColor(255, 32, 32, 64));

			camera->OnAnimate(device->getTimer()->getTime());

			if(water) water->update();

			lightSpark->updateAbsolutePosition();
			effect->getShadowLight(0).setPosition(lightSpark->getAbsolutePosition());
			effect->getShadowLight(0).setTarget(
				lightSpark->getAbsolutePosition() +
				(lightSpark->getAbsolutePosition() - lightBox->getAbsolutePosition()));

			effect->update();

			driver->endScene();
		}



	//				driver->writeImageToFile(driver->createScreenShot(), "shot.jpg");

};

