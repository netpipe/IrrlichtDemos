
// Pong.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
//#include "stdafx.h"
#include <irrlicht.h>
#include <iostream>

// irrlicht namespace
using namespace irr; using namespace core; using namespace scene;
using namespace video; using namespace io; using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

const f32 VELOCITY  =  0.5f;
const s32 UPLIMIT   =    30;
const s32 DOWNLIMIT =   -30;
f32 velocity  =  VELOCITY;
s32 hits1 = 0;
s32 hits2 = 0;

class MyEventReceiver: public IEventReceiver
{
   public:
       virtual bool OnEvent(const SEvent& event)
       {
           if (event.EventType == EET_KEY_INPUT_EVENT)
               KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            return false;
       }

       virtual bool IsKeyDown(EKEY_CODE keyCode) const
       {
         return KeyIsDown[keyCode];
       }

       MyEventReceiver()
       {
            for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
              KeyIsDown[i] = false;
       }
  private:
       bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

//detect collision
bool isCollision(ISceneNode* n1, ISceneNode* n2)
{
   aabbox3d<f32> box1 = n1->getTransformedBoundingBox();
   aabbox3d<f32> box2 = n2->getTransformedBoundingBox();

   if (box1.intersectsWithBox(box2))
      return true;
    else
      return false;
}

//move the bars according to user input
void moveBar(ISceneNode* bar1, ISceneNode* bar2, const MyEventReceiver& receiver)
{
   const f32 SPEEDFACTOR = 2.0f;
    vector3df poscorr = bar1->getPosition();
   if (receiver.IsKeyDown(KEY_KEY_W) && poscorr.Y < UPLIMIT)
    {
      poscorr.Y = poscorr.Y + SPEEDFACTOR*velocity;
      bar1->setPosition(poscorr);
   }
    else if (receiver.IsKeyDown(KEY_KEY_S) && poscorr.Y > DOWNLIMIT)
    {
      poscorr.Y = poscorr.Y - SPEEDFACTOR*velocity;
      bar1->setPosition(poscorr);
   }

   vector3df poscorr2 = bar2->getPosition();
   if (receiver.IsKeyDown(KEY_KEY_O) && poscorr2.Y < UPLIMIT)
    {
      poscorr2.Y = poscorr2.Y + SPEEDFACTOR*velocity;
      bar2->setPosition(poscorr2);
   }
    else if (receiver.IsKeyDown(KEY_KEY_L) && poscorr2.Y > DOWNLIMIT)
    {
      poscorr2.Y = poscorr2.Y - SPEEDFACTOR*velocity;
      bar2->setPosition(poscorr2);
   }
}

//move the ball according to the current direction
void moveBall(ISceneNode* ball, float coeff, int dir)
{
   vector3df pos = ball->getPosition();
    if (dir == 1)
    {
      pos.X -= velocity;
      pos.Y += coeff*velocity;
    }
    else
    {
      pos.X += velocity;
      pos.Y += coeff*velocity;
    }

    if ( (pos.X < -50) || (pos.X > 50) )
   {
       if (pos.X < -50) ++hits1;
       if (pos.X >  50) ++hits2;
      pos.X = 0;
       dir   = -dir;
    }
    ball->setPosition(pos);
}

void updateDirectionAfterCollison(ISceneNode* ball, ISceneNode* bar1, ISceneNode* bar2, ISceneNode* uplimit,
ISceneNode* downlimit, const MyEventReceiver& receiver, float* coeff, int* dir)
{
   //Collision with bar1
   if (isCollision(ball, bar1) && !(receiver.IsKeyDown(KEY_KEY_W) || receiver.IsKeyDown(KEY_KEY_S)))
    {
      *dir = 1;
   }
   if (isCollision(ball, bar1) && receiver.IsKeyDown(KEY_KEY_W))
    {
      *dir = 1;
      *coeff = -1;
   }
   if (isCollision(ball, bar1) && receiver.IsKeyDown(KEY_KEY_S))
    {
      *dir = 1;
      *coeff = 1;
   }

   //Collision with bar2
   if (isCollision(ball, bar2) && !(receiver.IsKeyDown(KEY_KEY_O)
         || receiver.IsKeyDown(KEY_KEY_L)))
    {
      *dir = 0;
   }
   if (isCollision(ball, bar2) && receiver.IsKeyDown(KEY_KEY_O))
    {
      *dir = 0;
      *coeff = -1;
   }
   if (isCollision(ball, bar2) && receiver.IsKeyDown(KEY_KEY_L))
    {
      *dir = 0;
      *coeff = 1;
   }

   if (isCollision(ball, uplimit))
    {
       *coeff = -1;
          std::cout << "collision uplimit"   << std::endl;
   }
   if (isCollision(ball, downlimit))
    {
       *coeff = 1;
       std::cout << "collision downlimit" << std::endl;
   }
}

int main()
{
   E_DRIVER_TYPE driverType = EDT_DIRECT3D9;

    std::cout << "Please select the driver you want for this example:\n"
   << " (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"
   << " (d) Software Renderer\n (e) Burning's Software Renderer\n"
    << " (f) NullDevice\n (otherKey) exit\n\n" << std::endl;

   char i;
   std::cin >> i;

   switch (i)
    {
       case 'a': driverType = EDT_DIRECT3D9;         break;
       case 'b': driverType = EDT_DIRECT3D8;         break;
       case 'c': driverType = EDT_OPENGL;             break;
       case 'd': driverType = EDT_SOFTWARE;            break;
       case 'e': driverType = EDT_BURNINGSVIDEO;   break;
       case 'f': driverType = EDT_NULL;               break;
       default:    return 0;
   }

   // create device
   MyEventReceiver receiver;

   IrrlichtDevice* device = createDevice(driverType, dimension2d<s32>(420, 300),
       16, false, false, true, &receiver);
   if (device == 0) return 1; // could not create selected driver.
   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr  = device->getSceneManager();

   ISceneNode* ball = smgr->addSphereSceneNode();
   if (ball)
    {
      ball->setPosition(vector3df(0.0f, 0.0f, 1.0f));
      ball->setScale(vector3df(0.3f, 0.3f, 0.3f));
      ball->setMaterialFlag(EMF_LIGHTING, false);
   }

   ISceneNode* bar1 = smgr->addCubeSceneNode();
   if (bar1)
    {
      bar1->setPosition(vector3df(48.0f, 0.0f, 1.0f));
      bar1->setScale(vector3df(0.1f, 1.0f, 0.1f));
      bar1->setMaterialFlag(EMF_LIGHTING, false);
   }

   ISceneNode* bar2 = smgr->addCubeSceneNode();
   if (bar2)
    {
      bar2->setPosition(vector3df(-48.0f, 0.0f, 1.0f));
      bar2->setScale(vector3df(0.1f, 1.0f, 0.1f));
      bar2->setMaterialFlag(EMF_LIGHTING, false);
   }

   ISceneNode* uplimit   = smgr->addCubeSceneNode();
   ISceneNode* downlimit = smgr->addCubeSceneNode();

   if (uplimit)
    {
      uplimit->setPosition(vector3df(0.0f, 35.0f, 1.0f));
      uplimit->setScale(vector3df(10.0f, 0.1f, 0.1f));
      uplimit->setMaterialFlag(EMF_LIGHTING, false);
   }
   if (downlimit)
    {
      downlimit->setPosition(vector3df(0.0f, -35.0f, 1.0f));
      downlimit->setScale(vector3df(10.0f, 0.1f, 0.1f));
      downlimit->setMaterialFlag(EMF_LIGHTING, false);
   }

   ICameraSceneNode* cam = smgr->addCameraSceneNode(0, vector3df(0,0,50), vector3df(0,0,0), 1);

   float coeff = 0;
   int dir = 1;
   while (device->run())
    {
     driver->beginScene(true, true, SColor(255, 113, 113, 133));
            updateDirectionAfterCollison(ball, bar1, bar2, uplimit, downlimit, receiver, &coeff, &dir);
            moveBall(ball, coeff, dir);
            moveBar(bar1, bar2, receiver);
            smgr->drawAll(); // draw the 3d scene
     driver->endScene();

       s32 fps = driver->getFPS();
       stringw str = L"PONG   FPS: ";
       str += fps;
       str += L"   Left:  ";
       str += hits1;
       str += L"   Right: ";
       str += hits2;
       device->setWindowCaption(str.c_str());
   }
   device->drop();

   return 0;
}
