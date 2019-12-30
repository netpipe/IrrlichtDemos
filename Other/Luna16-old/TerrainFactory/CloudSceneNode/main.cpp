#include <stdio.h>
#include <irrlicht.h>
#include <stdlib.h>

// cloud scene node demo by Gaz Davidson - gaz@bitplane.net

using namespace irr;

#include "CCloudSceneNode.h"

using namespace scene;
using namespace core;

#pragma comment(lib, "Irrlicht.lib")

scene::ISceneNode* node = 0;
IrrlichtDevice* device = 0;

int main()
{

    // create the device, as usual
    device = createDevice(video::EDT_OPENGL, core::dimension2du(800, 600),
        16, false, false, false);

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();


    // now lets add a camera...
    ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS (0, 100.0f, 5.1067f, -1);

    // turn off the cursor
    device->getCursorControl()->setVisible(false);

    int lastFPS = -1;

    f32 rot=1.0;

    // for some reason, the zorder is messed up if there are no meshes.

    // lets add a skybox
	smgr->addSkyBoxSceneNode(
		driver->getTexture("media/box_up.jpg"),
		driver->getTexture("media/box_dn.jpg"),
		driver->getTexture("media/box_lf.jpg"),
		driver->getTexture("media/box_rt.jpg"),
		driver->getTexture("media/box_ft.jpg"),
		driver->getTexture("media/box_bk.jpg"));


//    clouds->setSkyClouds(true, 0.2f);

    //   clouds->OnRegisterSceneNode();
    // create a cloud node
    scene::CCloudSceneNode* clouds = new scene::CCloudSceneNode(
            smgr->getRootSceneNode(), smgr,
                device->getTimer(), 666, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(1,1,1));

    // load the texture
    video::ITexture * txture = driver->getTexture("media/cloud4.png");

    // set any old random seed
    srand(1928);

    // set the level of details to draw at what distance
    clouds->setLOD(1);

    // set the maximum detail level when recursing
    clouds->setMaxDepth(1);

    // we dont use lighting yet. all the normals face the camera and it looks wierd
    // we could edit the colour of the particles in the list in realtime though!
    clouds->setMaterialFlag(video::EMF_LIGHTING, false);


    clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    // set any old random seed
    srand(rand());

    // call a "here's one I made earlier" function

    clouds->setLOD(1.0f);
    clouds->setMaxDepth(5);
    clouds->setSkyClouds(1,30);

    clouds->setMaterialTexture( 0, txture );



	// add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"media/terrain-heightmap.bmp");

	terrain->setScale(core::vector3df(40, 3, 40));
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));

	terrain->setPosition(vector3df(0,0,0));
	camera->setPosition(vector3df(0,0,0));


    camera->setFarValue (20000.0f);


    clouds->setPosition(vector3df(0,0,0));

    clouds->makeRandomCloud(800);
    clouds->makeRandomCloud((u32)890);

    while(device->run())
    {
        driver->beginScene(true, true, video::SColor(255,153,153,253));


        rot-= 0.02f;
 		// silly way to do rotation, depends on speed
        clouds->setRotation(vector3df(0,rot,0));

        smgr->drawAll();
device->getGUIEnvironment()->drawAll();
     clouds->render();
        driver->endScene();



        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            wchar_t tmp[1024];
            swprintf(tmp, 1024, L"Cloudy day fps:%d clouds:%d",
                fps,clouds->getCurrentCloudCount());

            device->setWindowCaption(tmp);
            lastFPS = fps;
        }
    }


    /*
    In the end, delete the Irrlicht device.
    */
    device->drop();

    return 0;
}
