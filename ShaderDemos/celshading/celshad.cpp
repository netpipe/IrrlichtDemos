#include <irrlicht.h>

using namespace irr;
using namespace video;
using namespace scene;
using namespace core;
using namespace io;
using namespace gui;

#pragma comment(lib,"irrlicht.lib")

class CST : public irr::video::IShaderConstantSetCallBack
{
public:

	CST()
	{
		device = createDevice(EDT_OPENGL, dimension2du(800, 600), 32);
		driver = device->getVideoDriver();
		smgr = device->getSceneManager();

		camera = smgr->addCameraSceneNodeFPS(0, 100.0, 0.005);
		camera->setPosition(vector3df(0.0f, 0.0f, -5.0f));

		ISceneNode* torus = smgr->addMeshSceneNode(smgr->getMesh("torus.b3d"));

		torus->getMaterial(0).setTexture(0, driver->getTexture("lialique.bmp"));
		torus->getMaterial(0).setTexture(1, driver->getTexture("cel.png"));
		torus->getMaterial(0).TextureLayer[1].BilinearFilter = false;

		u32 lastFPS = 0;

		video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

		s32 celMat = gpu->addHighLevelShaderMaterialFromFiles(
			"cel.vert", "main", EVST_VS_2_0,
			"cel.frag", "main", EPST_PS_2_0, this);

		torus->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)celMat;
		lightPosition = vector3df(100, 100, 100);

		while(device->run())
		{
			driver->beginScene(true, true, SColor(0xff88aadd));
			smgr->drawAll();
			driver->endScene();

			if(driver->getFPS() != lastFPS)
			{
				lastFPS = driver->getFPS();
				stringw windowCaption = "CST FPS: ";
				windowCaption += lastFPS;

				device->setWindowCaption(windowCaption.c_str());
			}
		}
	}

	~CST()
	{
		if(device)
		{
			device->closeDevice();
			device->run();
			device->drop();
			device = NULL;
		}
	}

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		matrix4 world = driver->getTransform(ETS_WORLD);

		matrix4 invWorld;
		world.getInverse(invWorld);

		vector3df lightPosOS;
		invWorld.transformVect(lightPosOS, lightPosition);
		services->setVertexShaderConstant("mLightPos", &lightPosOS.X, 3);

		vector3df camPosOS;
		invWorld.transformVect(camPosOS, camera->getPosition());
		services->setVertexShaderConstant("mCamPos", &camPosOS.X, 3);

		matrix4 wvp = driver->getTransform(ETS_PROJECTION);
		wvp *= driver->getTransform(ETS_VIEW);
		wvp *= world;

		services->setVertexShaderConstant("mWorldViewProj", wvp.pointer(), 16);
	}

	ICameraSceneNode* camera;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IrrlichtDevice* device;
	vector3df lightPosition;
};

int main()
{
	CST app;
	return 0;
}
