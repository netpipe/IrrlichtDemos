
#include <irrlicht.h>
//#include "CBeamSceneNode.h"
#include "CFireSceneNode.h"
//#include "decalscenenode.h"
#define MAX_DECALS 50
//DecalSceneNode* decals[MAX_DECALS];

using namespace irr;


using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


int main()
{

	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16,
			false, false, false, 0);
	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();


	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);


	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	if (!mesh)
		return 1;
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
	}

scene::ICameraSceneNode* camera =  smgr->addCameraSceneNodeFPS();
//! Code To Impliment for thise demo of demo's


//device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");
//	scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("20kdm2.bsp");
//	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
//	scene::ISceneNode* q3node = 0;
//	if(q3levelmesh)
//		q3node = smgr->addOctTreeSceneNode(q3levelmesh->getMesh(0));
//	scene::ITriangleSelector* selector = 0;
//	if(q3node){
//		q3node->setScale(core::vector3df(1.0, 1.0, 1.0));
//		selector = smgr->createOctTreeTriangleSelector(q3levelmesh->getMesh(0), q3node, 128);
//		q3node->setTriangleSelector(selector);
//		selector->drop();
//	}



ISceneNode* fire = new CFireSceneNode(
    smgr->getRootSceneNode(), //Parent
    smgr, //Scene Manager
    device->getTimer(), //ITimer
    "irrlichtlogoalpha2.tga", //Fadeout texture
    SColor(255, 255, 128, 100), //Starting color
    2, //Fadeout color (2 - red, 1 - green, 0 - blue)
    0.95f, //Fadeout rate (0.0f - 1.0f -> the higher, the less the
             //color fades into the fadeout color)
    64, //Texture X size
    64, //Texture Y size
    30, //Width
    70, //Height
    20, //Update speed
    -1, //id
    vector3df(0.0f, 0.0f, 0.0f), //Position
    vector3df(0.0f, 0.0f, 0.0f) //Rotation
);

fire

	while(device->run())
	{
        core::line3d<f32> line;
        line.start = camera->getPosition();
        line.end = line.start + (camera->getTarget() - line.start).normalize() * 1000.0f;
        core::vector3df intersection;
        core::triangle3df tri;
        // hit the level !?!?!
//      if(smgr->getSceneCollisionManager()->getCollisionPoint(line, selector, intersection, tri)){
          // create decal
//
//    video::ITexture* image = driver->getTexture("../../media/lightFalloff.png");
//    driver->makeColorKeyTexture(image, core::position2d<s32>(0,0));
//    image->regenerateMipMapLevels();
//    smgr->addLightSceneNode(NULL, core::vector3df(0,10,0), video::SColorf(5.0f, 255.0f, 255.0f), 250, -1);
//
//      }

		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene() call clears the screen with a color and
		the depth buffer, if desired. Then we let the Scene Manager and
		the GUI Environment draw their content. With the endScene()
		call everything is presented on the screen.
		*/
		driver->beginScene(true, true, SColor(255,100,101,140));



		smgr->drawAll();
		guienv->drawAll();
fire->render();
		driver->endScene();
	}

	device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
