#include "CWater.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace core;

CWater::CWater(	irr::IrrlichtDevice* irrlichtDevice, 
				const irr::core::dimension2d<irr::f32>& tileSize,
				const irr::core::dimension2d<irr::u32>& tileCount)
				:	device(irrlichtDevice), smgr(device->getSceneManager()), 
					driver(device->getVideoDriver()), mesh(tileSize, tileCount)
{
	waterMat = driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
		"shaders/WaterV.glsl", "main", EVST_VS_2_0, "shaders/WaterP.glsl", "main",
		EPST_PS_2_0, this);

	s32 patchCount = 3;

	vector3df tSize = vector3df(mesh.getTileCount().Width * mesh.getTileSize().Width - 20.0f, 0.0f,
								mesh.getTileCount().Height * mesh.getTileSize().Height - 20.0f);

	vector3df offSet = tSize * (f32)patchCount / 2.0f;

	parentNode = smgr->addEmptySceneNode();

	for(s32 x = 0;x < patchCount;++x)
	{
		for(s32 y = 0;y < patchCount;++y)
		{
			IMeshSceneNode* sceneNode = smgr->addMeshSceneNode(mesh.getMesh(), parentNode);
			sceneNode->getMaterial(0).DiffuseColor = SColor(255, 0, 100, 200);
			sceneNode->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)waterMat;
			sceneNode->getMaterial(0).setTexture(0, driver->getTexture("media/Water2Normal3.png"));
			sceneNode->setPosition(tSize * vector3df((f32)x, 0, (f32)y) - offSet);
		}
	}
}

CWater::~CWater()
{
	parentNode->remove();
}

void CWater::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
	matrix4 wvp = driver->getTransform(ETS_PROJECTION);
	wvp *= driver->getTransform(ETS_VIEW);

	services->setVertexShaderConstant("ViewProj", wvp.pointer(), 16);

	services->setVertexShaderConstant("World", driver->getTransform(ETS_WORLD).pointer(), 16);

	const f32 time = device->getTimer()->getTime() / 500.0f;
	services->setVertexShaderConstant("Time", &time, 1);

	const f32 tCoordAdjuster = mesh.getTileCount().Width / 200.0f * mesh.getTileSize().Width;
	services->setVertexShaderConstant("TCoordAdjuster", &tCoordAdjuster, 1);

	if(smgr->getActiveCamera())
		services->setVertexShaderConstant("CamPos",	&smgr->getActiveCamera()->getPosition().X, 3);
}

void CWater::update()
{
	if(smgr->getActiveCamera())
	{
		vector3df newPos = smgr->getActiveCamera()->getPosition();
		newPos.Y = 0.0f;
		parentNode->setPosition(newPos);
	}
}
