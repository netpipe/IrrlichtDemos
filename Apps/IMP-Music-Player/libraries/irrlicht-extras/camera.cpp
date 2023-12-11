#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
bool up    = false;
bool down  = false;
bool right = false;
bool left  = false;
int speed=1;
#pragma comment(lib, "Irrlicht.lib")
class MyEventReceiver : public IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent& event)
   {

         if (event.EventType == EET_KEY_INPUT_EVENT)
      {
         switch(event.KeyInput.Key)
         {
         case KEY_KEY_W:

            up = event.KeyInput.PressedDown;
            break;

         case KEY_KEY_S:

            down = event.KeyInput.PressedDown;
            break;

         case KEY_KEY_A:

                     left = event.KeyInput.PressedDown;
            break;

         case KEY_KEY_D:

            right = event.KeyInput.PressedDown;
            break;

         }

         //return true;
      }

      return false;
   }

};
int main(int argc, char** argv)
{
MyEventReceiver receiver;
    IrrlichtDevice *device =
        createDevice(EDT_OPENGL, dimension2d<s32>(640, 480), 16,
            false, false, false, &receiver);

    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");


    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();
ISceneNode* hero = smgr->addCubeSceneNode();
IAnimatedMesh * movingplane;

movingplane = smgr->addHillPlaneMesh("floor",
      core::dimension2d<f32>(20,20),
      core::dimension2d<u32>(40,40), 0, 0,
      core::dimension2d<f32>(0,0),
      core::dimension2d<f32>(10,10));


 IAnimatedMeshSceneNode *floor=smgr->addAnimatedMeshSceneNode(movingplane);
    if (hero)
    {
        hero->setMaterialFlag(EMF_LIGHTING, false);
    }
ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));
 while(device->run())
    {
core::vector3df c = hero->getPosition();
         core::vector3df d= hero->getRotation();
float diry = ((d.Y+90)*3.14)/180;
        if (up)
         {

            c.X += speed * cos((d.Y) * 3.14 / 180);
  c.Z -= speed * sin((d.Y) * 3.14 / 180);

         }
         if (down)
         {
                     c.X -= speed * cos((d.Y) * 3.14 / 180);
  c.Z += speed * sin((d.Y) * 3.14 / 180);
         }
         if (left)
         {
            d.Y -= 0.1;
         }
         if (right)
         {
            d.Y += 0.1;
         }
hero->setRotation(d);
    int xf = (c.X-sin(diry)*125);
int yf =(c.Z-cos(diry)*125);
int zf =100;

hero->setPosition(c);


         camera->setTarget(c);

         c.Y +=200.f;
         c.Z -= 150.f;

         camera->setPosition(vector3df(xf,zf,yf));
driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }     //device->drop();

    return 0;
}
