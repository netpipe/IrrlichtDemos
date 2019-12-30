
#include <irrlicht.h>
#include "decalscenenode.h"
#define MAX_DECALS 50
DecalSceneNode* decals[MAX_DECALS];

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


int main()
{

	IrrlichtDevice *device =    createDevice( video::EDT_OPENGL, dimension2du(640, 480), 16,
                                                false, false, false, 0);
	if (!device)    return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);

	IAnimatedMesh* mesh = smgr->getMesh("./media/sydney.md2");
	if (!mesh)
		return 1;
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("./media/sydney.bmp") );
	}

    //Camera
    scene::ICameraSceneNode* camera =  smgr->addCameraSceneNodeFPS();


   //Point light
   scene::ISceneNode* lnode = smgr->addLightSceneNode(0, core::vector3df(-200,200,-200),
      video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
   scene::ISceneNodeAnimator* anim2 = 0;
   anim2 = smgr->createFlyCircleAnimator (core::vector3df(500,350,200),50.0f);
   node->addAnimator(anim2);
      lnode->addAnimator(anim2);
   anim2->drop();



   // device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");
    //scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("20kdm2.bsp");
    scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("./data/3/aa.irrmesh");
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    scene::ISceneNode*  q3node =0;

    if(q3levelmesh) q3node = smgr->addOctTreeSceneNode(q3levelmesh->getMesh(0));
    scene::ITriangleSelector* selector = 0;
    q3node->setMaterialFlag(video::EMF_LIGHTING, true);
    if(q3node)
    {
        q3node->setScale(core::vector3df(1.0, 1.0, 1.0));
        selector = smgr->createOctTreeTriangleSelector(q3levelmesh->getMesh(0), q3node, 128);
        q3node->setTriangleSelector(selector);
        selector->drop();
    }

    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    video::ITexture* image = driver->getTexture("data/textures/bullet.png");
    //  video::ITexture* image = driver->getTexture("media/bullet.png");

    for(int i=0; i<MAX_DECALS; i++){
        decals[i] = new DecalSceneNode(smgr->getRootSceneNode(), smgr, image, 2.2f);
    //decals[i]->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    //decals[i]->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
    //decals->setMaterialFlag(video::EMF_LIGHTING, true);
    //decals[i]->getMaterial(0)->setFlag();

    }
    int nextDecal=0;

ISceneNode* outNode;
	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

///start of bullet stuff
        // shootTime = device->getTimer()->getTime() + 100;
         //Find intersection point
         core::line3d<f32> line;
         core::triangle3df tri;
         core::vector3df intersection;
         line.start = camera->getPosition();
         line.end = line.start + (camera->getTarget() - line.start).normalize() * 1000.0f;

         if (smgr->getSceneCollisionManager()->getCollisionPoint(line, selector, intersection, tri,outNode))
         {
            //Setup decal sprite
            decals[nextDecal]->Setup(tri,intersection);
            nextDecal++;
            if (nextDecal >= MAX_DECALS)
               nextDecal = 0;
         }
/// end

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
