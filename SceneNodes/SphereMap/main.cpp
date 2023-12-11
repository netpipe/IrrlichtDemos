
#include <irrlicht.h>

#include "CSphereMapRendererSceneNode.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{

	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2du(800, 600), 32, false, false, false, 0);

	device->setWindowCaption(L"SphereMap Renderer");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	// add a camera
	core::vector3df pos(900,550,1700);
	smgr->addCameraSceneNodeFPS()->setPosition(pos);
	//smgr->addCameraSceneNodeMaya()->setTarget(pos);

	// load environment
	device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	smgr->addMeshSceneNode(mesh->getMesh(0));


	//sphrMap->setPosition(pos);

	// add something to show the sphere map
	IAnimatedMesh* msh = smgr->getMesh("../../media/dwarf.x");

	//ISceneNode* node = smgr->addSphereSceneNode();
	ISceneNode* node = smgr->addCubeSceneNode();
	//node->setVisible(true);

	node->setMaterialType(video::EMT_SPHERE_MAP);
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setPosition(pos);
	node->setScale(vector3df(15.0,15.0,15.0));

	ISceneNodeAnimator* anmtr = smgr->createRotationAnimator(core::vector3df(0.3f,0.3f,0.3f));
	node->addAnimator(anmtr);
	node->updateAbsolutePosition();

	anmtr->drop();
	//node->setRotation(core::vector3df(45.0f, 45.0, 0));


	// create spheremap renderer
	CSphereMapRendererSceneNode* sphrMap =
		new CSphereMapRendererSceneNode(node, smgr);
	sphrMap->drop();
	//sphrMap->setPosition(pos);

	//node->setMaterialTexture(0, driver->getTexture("../../nice.jpg"));
	node->setMaterialTexture(0, sphrMap->getTexture(ESMRT_OUTPUT));
	node->setMaterialTexture(1, sphrMap->getTexture(ESMRT_OUTPUT));

	//node->setMaterialFlag(EMF_WIREFRAME, true);
	//node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	//sphrMap->setScale(vector3df(5,5,5));
	//node->setMaterialTexture(0, driver->getTexture("../../media/spheremap.jpg"));

	//sphrMap->setDebugDataVisible(EDS_SPHEREMAP_MESH);
	//sphrMap->setDebugDataVisible(EDS_SPHEREMAP_BILLBOARD);
	sphrMap->updateAbsolutePosition();

	//node->getMaterial(0).setTexture(0,driver->getTexture("../../media/t351sml.jpg"));

	device->getCursorControl()->setVisible(false);
	core::matrix4 flipMatrix;
	flipMatrix.buildTextureTransform(0,vector2df(0,0),vector2df(1,1), vector2df(-1,-1));

	//node->getMaterial(0).setTextureMatrix(0,zoomMatrix);
	//node->getMaterial(0).PointCloud = true;
	//node->getMaterial(0).TextureLayer[0].TextureWrap = ETC_MIRROR;

	//node->getMaterial(0).setTextureMatrix(0, flipMatrix);

	f32 f;
	u32 time=0;

	while(device->run())
	{
		time = device->getTimer()->getTime();

		f = f32(time % 3000) / 1500.0f;
		f = 1.0f/(fabs(f - 0.9999));

		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		//sphrMap->setMagnify(0.6f);//f);
		sphrMap->update(false);//, false);
		node->setVisible(true);

		driver->endScene();
	}
	device->drop();

	return 0;
}

