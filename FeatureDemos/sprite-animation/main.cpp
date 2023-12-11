#include "irrlicht.h"
#include "CAnimSprite.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



int main()
{
     IrrlichtDevice* irrDevice = createDevice(EDT_OPENGL,dimension2d<u32>(640,480),32,false,false,false,0);
     IVideoDriver*   irrVideo  = irrDevice->getVideoDriver();
     ISceneManager*  irrSceneMgr = irrDevice->getSceneManager();

     CAnimSprite* Sprite = new CAnimSprite(irrSceneMgr->getRootSceneNode(), irrSceneMgr, 666, irrDevice->getTimer());
     Sprite->Load("sonwalk.jpg",0,0,40*8,40,40,40,true);
     Sprite->setSpeed(100);
    // Sprite->PlayBackward();
     Sprite->setScale(vector3df(2,2,0));
     Sprite->setPosition(vector3df(-0.5,0.1,0));
     f32 rt=0;

     while  (irrDevice->run())
       {
           irrVideo->beginScene(true, true, SColor(0,200,200,200));
           rt += 0.01;
           Sprite->setRotation(vector3df(0,0,rt));
           Sprite->OnAnimate();
           irrSceneMgr->drawAll();
           irrVideo->endScene();
       }
     irrDevice->drop();

     return 0;
}
