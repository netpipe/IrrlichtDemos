
#include <irrlicht.h>
#pragma comment(lib, "Irrlicht.lib")

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
   MyEventReceiver()
   {
      u32 k;
      for (k = 0; k < sizeof(Keys) / sizeof(*Keys); ++k)
         Keys[k] = false;
   }

   virtual ~MyEventReceiver()
   {
   }

   // you may need to change the parameter type depending on your Irrlicht version
   virtual bool OnEvent(const SEvent& event)
   {
      if (event.EventType == EET_KEY_INPUT_EVENT)
      {
         Keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
         return true;
      }

      return false;
   }

public:
   bool Keys[KEY_KEY_CODES_COUNT];
};

int main()
{
   // create device and exit if creation failed
   IrrlichtDevice *device = createDevice(EDT_DIRECT3D9, core::dimension2d<s32>(800, 600), 32, false, false, false);
   if (device == 0)
      return 1; // could not create selected driver.

   MyEventReceiver receiver;
   device->setEventReceiver(&receiver);

   video::IVideoDriver* driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();

   // the thing we want to look at
   scene::ISceneNode* box =
      smgr->addCubeSceneNode();

   // add camera
   scene::ICameraSceneNode* cam =
      smgr->addCameraSceneNode(box);
   cam->setTarget( box->getAbsolutePosition() );

   // add a dynamic light
   smgr->addLightSceneNode(0, core::vector3df(-20, 0, 0), video::SColorf(1.f, 0.f, 0.f, 1.f));
   smgr->addLightSceneNode(0, core::vector3df(0, -20, 0), video::SColorf(0.f, 1.f, 0.f, 1.f)); 
   smgr->addLightSceneNode(0, core::vector3df(0, 0, -20), video::SColorf(0.f, 0.f, 1.f, 1.f));

   f32 Radius = 20.f;
   f32 Theta  = 180.f; // degrees
   f32 Phi    = 90.f; // degrees

   f32 LinearVelocity = 20.f;
   f32 AngularVelocity = 40.f;

   u32 oldNow = device->getTimer()->getRealTime();

   while(device->run())
   {
      // calculate elapsed time as fractional seconds
      u32 now = device->getTimer()->getRealTime();
      f32 Elapsed = (now - oldNow) / 1000.f;
      oldNow = now;

      // window is active, so render scene
      if (device->isWindowActive())
      {
         if (driver->beginScene(true, true, video::SColor(255, 100, 100, 140)))
         {
            smgr->drawAll();

            driver->endScene();
         }

         // adjust camera position based on current key input
         if (receiver.Keys[KEY_NUMPAD1])
            Radius -= (LinearVelocity * Elapsed);
         if (receiver.Keys[KEY_NUMPAD3])
            Radius += (LinearVelocity * Elapsed);
         if (receiver.Keys[KEY_NUMPAD4])
            Theta += (AngularVelocity * Elapsed);
         if (receiver.Keys[KEY_NUMPAD6])
            Theta -= (AngularVelocity * Elapsed);
         if (receiver.Keys[KEY_NUMPAD8])
            Phi += (AngularVelocity * Elapsed);
         if (receiver.Keys[KEY_NUMPAD2])
            Phi -= (AngularVelocity * Elapsed);
         if (receiver.Keys[KEY_NUMPAD5])
         {
            Theta = 180.f;
            Phi   = 90.f;
         }

         // prevent camera from walking into our box
         if (Radius < 10.f)
            Radius = 10.f;

         // lame ass gimble lock prevention. if you don't want to do this
         // you need to adjust the up vector of the camera so it never is
         // parallel to the look at vector
         if (Phi < 10.f)
            Phi = 10.f;
         else if (170.f < Phi)
            Phi = 170.f;

         f32 sinOfPhi = sinf(Phi * core::DEGTORAD);
         f32 cosOfPhi = cosf(Phi * core::DEGTORAD);

         f32 sinOfTheta = sinf(Theta * core::DEGTORAD);
         f32 cosOfTheta = cosf(Theta * core::DEGTORAD);

         core::vector3df offset;

         // these are switched around a little from what i posted above
         offset.X = Radius * sinOfTheta * sinOfPhi;
         offset.Y = Radius * cosOfPhi;
         offset.Z = Radius * cosOfTheta * sinOfPhi;

         // camera is a child of the cube, so our offset is actually
         // the position of the camera
         cam->setPosition(offset);
         cam->setTarget( box->getAbsolutePosition() );
         cam->updateAbsolutePosition();

         wchar_t caption[128];
         swprintf(caption, 128, L"r=%f theta=%f phi=%f",
            Radius, Theta, Phi);

         device->setWindowCaption(caption);
      }
   }

   device->drop();

   return 0;
} 
