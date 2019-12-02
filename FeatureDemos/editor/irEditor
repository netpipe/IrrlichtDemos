

// IrrEditor.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#pragma comment(lib, "Irrlicht.lib")

IrrlichtDevice         *device         = 0;
IVideoDriver         *driver         = 0;
ISceneManager         *smgr         = 0;
IGUIEnvironment         *guienv         = 0;
ICameraSceneNode      *camera         = 0;
ISceneCollisionManager   *CollMan      = 0;

enum EditorMode
{
   MODE_NONE,
   MODE_QUIT,
   MODE_TRANSLATE,
   MODE_ROTATE,
   MODE_SCALE,
};

position2di            initialCursorPosition;      // where mouse cursor started
position2di            initialObjectPosition;      // where ray from camera to object intersected screen
ISceneNode*            SelectedNode      = 0;   // the selected node
bool               ShiftIsPressed      = 0;   // is the shift key pressed?
bool               LMouseButtonDown   = 0;   // is the left mouse button down?
EditorMode            Mode            = MODE_TRANSLATE;
float               RotateSpeed         = 0.01f;
float               ScaleSpeed         = 0.01f;

#define ID_QUIT         101
#define ID_TRANSLATE   102
#define ID_ROTATE      103
#define ID_SCALE      104

class MyEventReceiver : public IEventReceiver
{
public:
  virtual bool OnEvent(const SEvent& anevent)
  {
       switch(anevent.EventType)
       {
         case EET_GUI_EVENT :
         {
            switch (anevent.GUIEvent.EventType)
               {
                  case EGET_BUTTON_CLICKED:
                     {
                        switch (anevent.GUIEvent.Caller->getID())
                          {
                             case ID_QUIT      :
                                {
                                   Mode = MODE_QUIT;
                                } break;
                             case ID_TRANSLATE   :
                                {
                                   Mode = MODE_TRANSLATE;
                                } break;
                             case ID_ROTATE   :
                                {
                                   Mode = MODE_ROTATE;
                                } break;
                             case ID_SCALE      :
                                {
                                   Mode = MODE_SCALE;
                                } break;
                          } break;
                     } break;
               }
         }  break;

         case EET_KEY_INPUT_EVENT :
         {
            if (anevent.KeyInput.Shift) ShiftIsPressed = true;
            else ShiftIsPressed = false;
         } break;

         // if it is a mouse event
         case EET_MOUSE_INPUT_EVENT :
         {
            // based upon the mouse event type
            switch (anevent.MouseInput.Event)
            {
               case EMIE_LMOUSE_PRESSED_DOWN :
                {
                  // what a freakin hack
                  LMouseButtonDown = true;

                   // check for a node being selected
                  SelectedNode = CollMan->getSceneNodeFromScreenCoordinatesBB(device->getCursorControl()->getPosition(),0,true);

                   // If we selected a node
                   if(SelectedNode)
                   {
                     // Remember where the node and cursor were when it was clicked on
                     initialCursorPosition = device->getCursorControl()->getPosition();
                     initialObjectPosition = CollMan->getScreenCoordinatesFrom3DPosition(SelectedNode->getAbsolutePosition(), camera);
                   }

                   // we used this message?
                   return (SelectedNode != 0);
                } break;

               case EMIE_LMOUSE_LEFT_UP :
                {
                  LMouseButtonDown = false;
                  return false;
                } break;

               case EMIE_MOUSE_MOVED :
               {
                  if (!LMouseButtonDown) return false;

                  if (Mode == MODE_TRANSLATE)
                  {
                     if (SelectedNode)
                     {
                        vector3df p = SelectedNode->getPosition();

                        if (ShiftIsPressed)
                         {
                           plane3df const planeXY(SelectedNode->getAbsolutePosition(), vector3df(1.f, 0.f, 0.f));
                           position2di currentCursorPosition(device->getCursorControl()->getPosition());
                           position2di effectiveObjectPosition = initialObjectPosition + currentCursorPosition - initialCursorPosition;
                           line3df ray(CollMan->getRayFromScreenCoordinates(effectiveObjectPosition, camera));
                           vector3df intersectWithPlane;
                           vector3df oldPos = SelectedNode->getAbsolutePosition();

                           if(planeXY.getIntersectionWithLine(ray.start, ray.getVector(), intersectWithPlane))
                           {
                              intersectWithPlane.X = oldPos.X;
                              intersectWithPlane.Z = oldPos.Z;
                              SelectedNode->setPosition(intersectWithPlane);
                           }
                         }
                         else
                         {
                           plane3df const planeXZ(SelectedNode->getAbsolutePosition(), vector3df(0.f, 1.f, 0.f));
                           position2di currentCursorPosition(device->getCursorControl()->getPosition());
                           position2di effectiveObjectPosition = initialObjectPosition + currentCursorPosition - initialCursorPosition;
                           line3df ray(CollMan->getRayFromScreenCoordinates(effectiveObjectPosition, camera));
                           vector3df intersectWithPlane;
                           if(planeXZ.getIntersectionWithLine(ray.start, ray.getVector(), intersectWithPlane))
                           {
                              SelectedNode->setPosition(intersectWithPlane);
                           }
                         }
                     }
                  } // end mode translate
                  else
                  if (Mode == MODE_ROTATE)
                  {
                     if (SelectedNode)
                     {
                        vector3df rot = SelectedNode->getRotation();
                        position2di currentCursorPosition(device->getCursorControl()->getPosition());
                        if (ShiftIsPressed)
                        {
                           if (initialObjectPosition.X < currentCursorPosition.X) rot.X -= RotateSpeed;
                           if (initialObjectPosition.X > currentCursorPosition.X) rot.X += RotateSpeed;
                        }
                        else
                        {
                           if (initialObjectPosition.X < currentCursorPosition.X) rot.Y -= RotateSpeed;
                           if (initialObjectPosition.X > currentCursorPosition.X) rot.Y += RotateSpeed;
                           if (initialObjectPosition.Y < currentCursorPosition.Y) rot.Z -= 1;
                           if (initialObjectPosition.Y > currentCursorPosition.Y) rot.Z += 1;
                        }
                        initialObjectPosition = currentCursorPosition;
                        SelectedNode->setRotation(rot);
                     }
                  }
                  else
                  if (Mode == MODE_SCALE)
                  {
                     if (SelectedNode)
                     {
                        vector3df scale = SelectedNode->getScale();
                        position2di currentCursorPosition(device->getCursorControl()->getPosition());
                        if (initialObjectPosition.X > currentCursorPosition.X)
                        {
                           scale.X -= ScaleSpeed;
                           scale.Y -= ScaleSpeed;
                           scale.Z -= ScaleSpeed;
                        }
                        if (initialObjectPosition.X < currentCursorPosition.X)
                        {
                           scale.X += ScaleSpeed;
                           scale.Y += ScaleSpeed;
                           scale.Z += ScaleSpeed;
                        }
                        initialObjectPosition = currentCursorPosition;
                        SelectedNode->setScale(scale);
                     }
                  }

               } break;
            }
         }
      }
      return 0;
   }
};

int main()
{
   // create the device
   device =  createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16, false, false, false, 0);
    if (!device) return 1;

   MyEventReceiver receiver;
    device->setEventReceiver(&receiver);

   // get a pointer to all of the irr items
   driver      = device->getVideoDriver();
    smgr      = device->getSceneManager();
    guienv      = device->getGUIEnvironment();
   CollMan      = smgr->getSceneCollisionManager();

   // add my little plug
   int x      = 10;
   int y      = 10;
   int width   = 100;
   int height   = 20;
   int space   = 5;

   guienv->addButton(core::rect<s32>(x,y,x+width,y+height),0,ID_QUIT,L"QUIT");
   y += height + space;
   guienv->addButton(core::rect<s32>(x,y,x+width,y+height),0,ID_TRANSLATE,L"Translate");
   y += height + space;
   guienv->addButton(core::rect<s32>(x,y,x+width,y+height),0,ID_ROTATE,L"Rotate");
   y += height + space;
   guienv->addButton(core::rect<s32>(x,y,x+width,y+height),0,ID_SCALE,L"Scale");

   // load a mesh
   IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
    if (!mesh) return 1;

   // create a node
   IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
   if (node)
    {
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setMD2Animation(scene::EMAT_STAND);
      node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
    }

   // add a camera
   camera = smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
   camera->setIsDebugObject(true);

   // run the scne
   while(Mode != MODE_QUIT)
   {
      device->run();
      driver->beginScene(true, true, SColor(255,100,101,140));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
        device->sleep(11,0);
    }

   // kill the device
   device->drop();

   return 0;
}

