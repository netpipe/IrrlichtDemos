#include <iostream>
#include <irrlicht.h>
#include <vector>

#include "IOctree.h"

///TODO: remove nodes from oct, dinamic objs (movement/scale/rot)

// also distance based culling / occlusion alt -tecan

using namespace std;

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

ICameraSceneNode* cam1;
ICameraSceneNode* cam2;

int resX = 1200;
int resY = 600;

bool useOctree = true;
bool splitScreen = false;
int cubesSpace = 300;

int main()
{
    IrrlichtDevice *device = createDevice(EDT_OPENGL,dimension2du(resX,resY), 32, false,false, false);

    ISceneManager *smgr = device->getSceneManager();
    IVideoDriver *driver = device->getVideoDriver();

    cam1 = smgr->addCameraSceneNode(0, vector3df(0,cubesSpace,-cubesSpace), vector3df(0,0,0));
    cam1->setAspectRatio( (resX/2)/resY );

    //cam2 = smgr->addCameraSceneNode(0, vector3df(20,10,0), vector3df(0,0,0));
    cam2 = smgr->addCameraSceneNodeFPS(0,100,.01);
    cam2->setAspectRatio( (resX/2)/resY );
    //cam2->setFarValue(20);

    device->getCursorControl()->setVisible(false);

    IOctree* octree = new IOctree(cam2);

    //---------------------------------cubes

    vector<ISceneNode*> cubes;
    //for(int i=-30;i<30;i+=5)
    //    for(int j=-30;j<30;j+=5)
    for(int i=-cubesSpace;i<cubesSpace;i+=5)
        for(int j=-cubesSpace;j<cubesSpace;j+=5)
        {
            ISceneNode* c = smgr->addCubeSceneNode(1.1);
            c->setPosition(vector3df(i+0.5*(rand()%10),0,j+0.5*(rand()%10)));
            c->setScale(vector3df(0.5*(rand()%10),0.5*(rand()%10),0.5*(rand()%10)));

            //c->setPosition(vector3df(i,0,j));
            //f32 s=(1+rand()%3);
            //c->setScale(vector3df( s,s,s ));

            cubes.push_back(c);

            if(useOctree) octree->addSceneNode(c);
        }
    //octree->addSceneNode(cubes[0]);

    smgr->setAmbientLight(SColorf(.1,.1,.1,.1));
    smgr->addLightSceneNode(0,vector3df(100,100,100));

    if(!splitScreen) cam2->setPosition(vector3df(-cubesSpace,0,-cubesSpace));

    s32 lastFPS = -1;

    while(device->run())
    {
        driver->beginScene(true,true,SColor(255,0,0,0));

        if(splitScreen)
        {
            smgr->setActiveCamera(cam1);
            driver->setViewPort(rect<s32>(0,0,resX/2,resY));

            smgr->drawAll();
            octree->draw(driver);

            smgr->setActiveCamera(cam2);
            driver->setViewPort(rect<s32>(resX/2,0,resX,resY));
            cam2->setAspectRatio( (resX/2)/resY );
        }
        else
        {
            cam2->setAspectRatio( (resX)/resY );
        }

        if(useOctree) octree->update();

        smgr->drawAll();
/*
        const SViewFrustum* frustum = cam2->getViewFrustum();
        driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
        driver->draw3DLine(frustum->cameraPosition,frustum->getFarLeftDown(),SColor(255,255,255,255));
        driver->draw3DLine(frustum->cameraPosition,frustum->getFarLeftUp(),SColor(255,255,255,255));
        driver->draw3DLine(frustum->cameraPosition,frustum->getFarRightDown(),SColor(255,255,255,255));
        driver->draw3DLine(frustum->cameraPosition,frustum->getFarRightUp(),SColor(255,255,255,255));
*/
        driver->endScene();

        const s32 fps = driver->getFPS();
        if (lastFPS != fps)
        {
                core::stringw str = L"Irrlicht Octree Culling - By Andres J. Porfirio [";
                str += driver->getName();
                str += "] FPS:";
                str += fps;

                device->setWindowCaption(str.c_str());
                lastFPS = fps;
        }
    }

    device->drop();

    return 0;
}
