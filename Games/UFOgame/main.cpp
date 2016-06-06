// ufo game. code by gaz davidson
//
// lets keep this short and sweet eh?

// includes for game
#include <cstdlib>
#include <iostream>
#include <irrlicht.h>

#pragma comment(lib, "Irrlicht.lib")
using namespace irr;
using namespace scene;

#include "scrollergame.h"


// lots of the cpp code ploped into a header so it would compile
//#include "game.h"

int main()
{

    InputReceiver receiver;

    // do the standard irrlicht startup thing

    // create device
    device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(screenwidth, screenheight),
                 16, false, false, false, &receiver);

    if (device == 0)
          return 1; // could not create selected driver.

    driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

    driver->setAmbientLight( video::SColorf(0.5,0.5,0.5));

    // load textures and meshes
    loadResources();
    // create waves
    makeWaves();

    // exhaust
    exhaust = new scene::CParticleSystem2SceneNode( smgr->getRootSceneNode(),  smgr, 0, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(1, 1, 1) );
    exhaust->setAutomaticCulling( EAC_OFF );
    exhaust->setParticleSize(core::dimension2d<f32>(10,10));
	exhaust->setMaterialFlag(video::EMF_LIGHTING, false);
	exhaust->setMaterialTexture(0, gametextures[10]);
	exhaust->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

    // smoke
    smoke = new scene::CParticleSystem2SceneNode( smgr->getRootSceneNode(),  smgr, 0, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(1, 1, 1) );
    smoke->setAutomaticCulling( EAC_OFF );
    smoke->setParticleSize(core::dimension2d<f32>(30,30));
	smoke->setMaterialFlag(video::EMF_LIGHTING, false);
	smoke->setMaterialTexture(0, gametextures[10]);
	smoke->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

    // create player
    player = new CPlayerSceneNode( 0, smgr, 0);
    player->setPosition(core::vector3df(0,0,0));
    player->setScale(core::vector3df(20,20,20));

    // create level
    terrain = new CScrollingWorldSceneNode(0,smgr,0);

    // create camera
    camera = new CGameCamera(0,smgr,0); // smgr->addCameraSceneNode();
    camera->cam->setFarValue(8000);
    //camera->setNearValue(2); // zoooom in

    int lastFPS = -1;

    // mouse cursor
    core::position2d<s32> mcp, oldmcp;
    device->getCursorControl()->setVisible(false);

    mousesensitivity.X = 15.0;
    mousesensitivity.Y = 15.0;

    printf("let the games begin!\n");


        u32 time2;
    while(device->run())
     //   if (device->isWindowActive())
    {
        time2 = device->getTimer()->getTime();

        // draw scene
        driver->beginScene(true, true, video::SColor(255,0,100,200));

        mcp = device->getCursorControl()->getPosition();

        if (mcp != oldmcp)
        {
            device->getCursorControl()->setPosition(screenwidth/2,screenheight/2);
            cpos.X = f32(mcp.X)/f32(screenwidth) -0.5f;
            cpos.Y = f32(mcp.Y)/f32(screenheight) -0.5f;

            oldmcp = mcp;
        }

        smgr->drawAll();

        // frame limiter!
        //float x=0.001;
        //   for (int n=1;n<1000;n++)
        //       for (int m=1;m<500;m++) x+=0.0001;

        //   draw gui
        //   end scene

        player->OnPostRender(time2);
        terrain->OnPostRender(time2);
        camera->OnPostRender(time2);
smoke->OnPostRender(time2);


        camera->cam->updateAbsolutePosition();
        driver->endScene();

        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
          core::stringw str = L"Scrolling shooter";
          str += driver->getName();
          str += "] FPS:";
          str += fps;
          str += " mouse pos= ";
          str += int(cpos.X*100);
          str += ",";
          str += int(cpos.Y*100);

          device->setWindowCaption(str.c_str());
          lastFPS = fps;
        }

    }
    player->drop();
    terrain->drop();
    exhaust->drop();
    device->drop();

    return 0;
}
