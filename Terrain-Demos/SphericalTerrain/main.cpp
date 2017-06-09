#include <irrlicht.h>

#include "CSphericalTerrain/CSphericalTerrain.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device = 0;
ISceneManager* smgr = 0;
ILightSceneNode* sun = 0;

// thank to rogerborg for this, this is used to align stuff on the terrain from the normal
// source: http://irrlicht.sourceforge.net/forum/viewtopic.php?p=163896
void createRotations(core::vector3df const & fromDirection,
                core::vector3df const & fromUp,
                ISceneNode * testNode)
{
   core::vector3df masterDirection(fromDirection);
   masterDirection.normalize();

   core::vector3df masterUp(fromUp);
   masterUp.normalize();

   core::vector3df realUp = masterDirection.crossProduct(masterUp).normalize();
   realUp = realUp.crossProduct(masterDirection);

   core::quaternion quatDirection;
   quatDirection.rotationFromTo(vector3df(0, 0, 1), masterDirection);

   core::vector3df worldUp(0, 1, 0);
   core::matrix4 mat;
   mat = quatDirection.getMatrix();
   mat.rotateVect(worldUp);

   core::quaternion quatUp;
   quatUp.rotationFromTo(worldUp, realUp);

   core::quaternion quat = quatDirection * quatUp;

   core::vector3df eulers;
   quat.toEuler(eulers);
   eulers *= RADTODEG;

   testNode->setRotation(eulers);
   testNode->updateAbsolutePosition();
   mat = testNode->getAbsoluteTransformation();

   vector3df slaveDirection(0, 0, 1);
   mat.rotateVect(slaveDirection);

   vector3df slaveUp(0, 1, 0);
   mat.rotateVect(slaveUp);
}

// the triplanar/splatting/rim/light shader for the main planetoid
class CSphericalTerrainShader : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services,
			s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();

		services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);

		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);


		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		core::vector3df camPos = device->getSceneManager()->
			getActiveCamera()->getAbsolutePosition();
		services->setVertexShaderConstant("mCamPos", reinterpret_cast<f32*>(&camPos), 3);

		core::vector3df lightPos = sun->getAbsolutePosition();
		services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&lightPos), 3);

		SLight lightData = sun->getLightData();
		video::SColorf amb = lightData.AmbientColor;

		services->setVertexShaderConstant("mAmbientColor",
				reinterpret_cast<f32*>(&amb), 4);

		services->setVertexShaderConstant("mLightColor",
				reinterpret_cast<f32*>(&lightData.DiffuseColor), 4);

		core::matrix4 world = driver->getTransform(video::ETS_WORLD);
		world = world.getTransposed();

		services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

		f32 texScaleFactor = 0.5f;
		services->setVertexShaderConstant("texScaleFactor", reinterpret_cast<f32*>(&texScaleFactor), 1);

		s32 TextureLayerID = 0;
		services->setPixelShaderConstant("texture1", &TextureLayerID, 1);
		TextureLayerID = 1;
		services->setPixelShaderConstant("texture2", &TextureLayerID, 1);
		TextureLayerID = 2;
		services->setPixelShaderConstant("texture3", &TextureLayerID, 1);
		TextureLayerID = 3;
		services->setPixelShaderConstant("alphaDetailMap", &TextureLayerID, 1);
	}
};

int main()
{
	SIrrlichtCreationParameters parameters;
	parameters.AntiAlias = 2;
	parameters.Bits = 32;
	parameters.DriverType = video::EDT_OPENGL;
	parameters.Vsync = true;
	parameters.Stencilbuffer = true;
	parameters.WindowSize = dimension2d<u32>(640, 480);

	device = createDeviceEx(parameters);

	if (!device)
		return 1;

	device->setWindowCaption(L"Spherical Terrain");

	IVideoDriver* driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	s32 sphericalTerrainMaterial = 0;
	if (gpu)
	{
		CSphericalTerrainShader* sts = new CSphericalTerrainShader();

		sphericalTerrainMaterial = gpu->addHighLevelShaderMaterialFromFiles(
				"media/spherical_terrain.vert", "vertexMain", video::EVST_VS_2_a,
				"media/spherical_terrain.frag", "pixelMain", video::EPST_PS_2_a,
				sts, EMT_SOLID, 0, EGSL_DEFAULT);
		sts->drop();
	}

	// the main planetoid
    scene::CSphericalTerrain* sphericalTerrainSceneNode = new scene::CSphericalTerrain("media/heightmap.png", smgr, 50.0f, 2.0f, smgr->getRootSceneNode(), -1);
	if (sphericalTerrainSceneNode)
	{
	    sphericalTerrainSceneNode->setMaxHeight(2.0f);
		sphericalTerrainSceneNode->setPosition(core::vector3df(0,0,30));
		sphericalTerrainSceneNode->setMaterialTexture(0, driver->getTexture("media/low_height.png"));
		sphericalTerrainSceneNode->setMaterialTexture(1, driver->getTexture("media/mid_height.png"));
		sphericalTerrainSceneNode->setMaterialTexture(2, driver->getTexture("media/high_height.png"));
		sphericalTerrainSceneNode->setMaterialTexture(3, driver->getTexture("media/alphamap.png"));
		sphericalTerrainSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
		sphericalTerrainSceneNode->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
		sphericalTerrainSceneNode->setMaterialType((video::E_MATERIAL_TYPE)sphericalTerrainMaterial);

		// now we place stuff on its surface
		// coins billboards
		for (u32 i = 0; i < 256; i+=8) {
			scene::ISceneNode* coinMeshNode = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(0.5, 0.5));
			coinMeshNode->setMaterialTexture(0, driver->getTexture("media/coin.png"));
			coinMeshNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

			float hover = 1.05f; // this is used so coins are a bit above the surface
			core::vector3df surfacePosition = sphericalTerrainSceneNode->getSurfacePosition(i, 36, hover);
			coinMeshNode->setPosition(surfacePosition);
		}

		// trees
		scene::IMesh* treeMesh = smgr->getMesh("media/tree/tree.3ds");
		if (treeMesh)
		{
			for (unsigned int i = 0; i < 256; i+=16) {
				scene::IMeshSceneNode* treeMeshNode = smgr->addMeshSceneNode(treeMesh);
				treeMeshNode->setScale(vector3df(0.0125f, 0.0125f, 0.0125f));
				treeMeshNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

				// place the tree on the surface
				int yPos = 28;
				core::vector3df surfacePosition = sphericalTerrainSceneNode->getSurfacePosition(i, yPos);
				treeMeshNode->setPosition(surfacePosition);

				// align the tree on the surface
				core::vector3df normal = sphericalTerrainSceneNode->getSurfaceNormal(i, yPos);
				core::vector3df dir(0, 1, 0);
				core::vector3df axis = dir.crossProduct(normal);
				axis.normalize();
				createRotations(axis, normal, treeMeshNode);

				treeMeshNode->getMaterial(0).AmbientColor = SColor(255, 9, 32, 2);
				treeMeshNode->getMaterial(0).SpecularColor = SColor(255, 0, 0, 0);
				treeMeshNode->getMaterial(0).Shininess = 0.0f;
			}
		}

		// grass
		scene::IMesh* grassMesh = smgr->getMesh("media/grass/grass_low_poly.obj");
		if (grassMesh)
		{
			for (unsigned int i = 0; i < 256; i++) {
				scene::IMeshSceneNode* grassMeshNode = smgr->addMeshSceneNode(grassMesh);
				grassMeshNode->setScale(vector3df(0.035f, 0.035f, 0.035f));
				grassMeshNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
				grassMeshNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

				// place the grass on the surface
				int x = ((float)rand()/RAND_MAX)*255;
				int y = ((float)rand()/RAND_MAX)*18;
				grassMeshNode->setPosition(sphericalTerrainSceneNode->getSurfacePosition(x, y));

				// align the grass on the surface
				core::vector3df normal = sphericalTerrainSceneNode->getSurfaceNormal(x, y);
				core::vector3df dir(0, 1, 0);
				core::vector3df axis = dir.crossProduct(normal);
				axis.normalize();
				createRotations(axis, normal, grassMeshNode);

				grassMeshNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
				grassMeshNode->getMaterial(0).AmbientColor = SColor(255, 9, 32, 2);
				grassMeshNode->getMaterial(0).SpecularColor = SColor(255, 0, 0, 0);
				grassMeshNode->getMaterial(0).Shininess = 0.0f;
			}
		}
	}

	// asteroids (no shaders so no triplanar texturing so textures distorsions happen on the pole)
	for (int i = 0; i < 2; i++) {
		scene::CSphericalTerrain* sphericalTerrainSceneNode2 = new scene::CSphericalTerrain("media/heightmap2.png", smgr, 1.0f, 0.5f, smgr->getRootSceneNode(), -1);
		if (sphericalTerrainSceneNode2)
		{
			float sx = 0.5f+((float)rand()/RAND_MAX)*1.25f;
			float sy = 0.5f+((float)rand()/RAND_MAX)*1.25f;
			float sz = 0.5f+((float)rand()/RAND_MAX)*1.25f;
			float cx = 10.0f * cosf(M_PI/2+i*M_PI);
			float cy = 10.0f * sinf(M_PI/2+i*M_PI);
			sphericalTerrainSceneNode2->setPosition(core::vector3df(cx,0,cy+30));
			sphericalTerrainSceneNode2->setScale(core::vector3df(sx,1.0f,sz));
			sphericalTerrainSceneNode2->setMaterialFlag( video::EMF_NORMALIZE_NORMALS, true );
			sphericalTerrainSceneNode2->setRotation(core::vector3df(sx * 72.0f, sy * 72.0f, sz * 72.0f));
			sphericalTerrainSceneNode2->setMaterialTexture(0, driver->getTexture("media/craters.jpg"));
			sphericalTerrainSceneNode2->setMaterialTexture(1, driver->getTexture("media/detail.jpg"));
			sphericalTerrainSceneNode2->setMaterialFlag(video::EMF_LIGHTING, true);
			sphericalTerrainSceneNode2->setMaterialType(video::EMT_DETAIL_MAP);
			sphericalTerrainSceneNode2->getMaterial(0).getTextureMatrix(0).setTextureScale(3.0f, 3.0f);
			sphericalTerrainSceneNode2->getMaterial(0).getTextureMatrix(1).setTextureScale(10.0f, 10.0f);
		}
	}

	// light
    sun =
        smgr->addLightSceneNode(0, core::vector3df(0,0,0),
        video::SColorf(1.0f, 1.0f, 1.0f, 1.0f), 800.0f);
	sun->getLightData().AmbientColor = video::SColorf(0.0f, 0.0f, 0.0f, 0.0f);

    scene::ISceneNodeAnimator* sunOrbitAnim =
        smgr->createFlyCircleAnimator(core::vector3df(16,32,0),800.0f, -0.0002f);
    sun->addAnimator(sunOrbitAnim);
    sunOrbitAnim->drop();

	smgr->setAmbientLight(video::SColorf(0.4f,0.4f,0.4f,1.0f));

	// the tube
    scene::IMesh* tubeMesh = smgr->getMesh("media/tube_mario.dae");
	scene::IMeshSceneNode* tubeMeshNode = 0;
    if (tubeMesh)
    {
		tubeMeshNode = smgr->addMeshSceneNode(tubeMesh);
		tubeMeshNode->setScale(vector3df(0.565f, 4.75f, 0.565f));
		tubeMeshNode->setPosition(core::vector3df(0,0,30));
		tubeMeshNode->setRotation(core::vector3df(0,0,90));
		tubeMeshNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

		tubeMeshNode->getMaterial(0).AmbientColor = SColor(255, 9, 32, 2);
		tubeMeshNode->getMaterial(0).SpecularColor = SColor(255, 0, 0, 0);
		tubeMeshNode->getMaterial(0).Shininess = 0.0f;
	}

	// skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    smgr->addSkyBoxSceneNode(
        driver->getTexture("media/spacebox/spacebox_top3.png"),
        driver->getTexture("media/spacebox/spacebox_bottom4.png"),
		driver->getTexture("media/spacebox/spacebox_front5.png"),
		driver->getTexture("media/spacebox/spacebox_back6.png"),
		driver->getTexture("media/spacebox/spacebox_right1.png"),
		driver->getTexture("media/spacebox/spacebox_left2.png"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    scene::ICameraSceneNode *camera;
	camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.004f);
	camera->setNearValue(0.005f);

	s32 lastFPS = -1;

	while(device->run())
	{
		driver->beginScene(true, true, 0);

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

        const s32 fps = driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw str = L"Spherical Terrain - [";
            str += driver->getName();
            str += "] FPS:";
            str += fps;

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
        device->sleep(15);
	}

	device->drop();

	return 0;
}
