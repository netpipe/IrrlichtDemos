/*!-----------------------------------------------------------------------------
| Turn to target tutorial                                                      |
| author: arras                                                                |
| date:   06.02.2009                                                           |
|                                                                              |
| use arrow keys + PgUp/PgDn or A,D,S,W,E,C keys to move target around         |
------------------------------------------------------------------------------*/

#include <irrlicht.h>
using namespace irr;

// return relative position of target given origin and rotation
core::vector3d<f32> getRelativePosition(const core::vector3d<f32> &origin, const core::vector3d<f32> &rotation, const core::vector3d<f32> &target)
{
   core::matrix4 m;
   m.setRotationDegrees(rotation);

   core::vector3d<f32> relative = target - origin;
   m.inverseRotateVect(relative);

   return relative;
}

// return relative position of target given node position and rotation
core::vector3d<f32> getRelativePosition(scene::ISceneNode *node, const core::vector3d<f32> &target)
{
   node->updateAbsolutePosition(); //this will update AbsoluteTransformation of node
   core::matrix4 m = node->getAbsoluteTransformation();

   core::vector3d<f32> relative = target - node->getPosition();
   m.inverseRotateVect(relative);

   return relative;
}

// use above functions to rotate node towards target
// rotation is done around local X and Y axis of node not global axis, if you want
//    different behavior you need to code it yourself
// code is susceptible to gimbal lock
void rotateTo(scene::ISceneNode *node, f32 speed, const core::vector3df &target)
{
   core::vector3df pos = getRelativePosition(node, target);
   core::vector3df rot = pos.getHorizontalAngle();

   if(rot.X <= 180 && rot.X > speed) rot.X = speed;
   if(rot.X > 180 && rot.X < 360 - speed) rot.X = -speed;

   if(rot.Y <= 180 && rot.Y > speed) rot.Y = speed;
   if(rot.Y > 180 && rot.Y < 360 - speed) rot.Y = -speed;

   core::matrix4 m;
   m.setRotationDegrees( node->getRotation() );

   core::matrix4 m2;
   m2.setRotationDegrees(rot);

   m *= m2;

   node->setRotation( m.getRotationDegrees() );
}

// just detects keyboard input
class MEventReceiver : public IEventReceiver
{
   bool Left;
   bool Right;
   bool Up;
   bool Down;
   bool Forward;
   bool Back;

public:

   MEventReceiver()
   {
      Left = false;
      Right = false;
      Up = false;
      Down = false;
      Forward = false;
      Back = false;
   }

   virtual bool OnEvent(const SEvent &event)
   {
      if(event.EventType == irr::EET_KEY_INPUT_EVENT)
      {
         switch(event.KeyInput.Key)
         {
            case KEY_RIGHT: Right = event.KeyInput.PressedDown; break;
            case KEY_LEFT: Left = event.KeyInput.PressedDown; break;

            case KEY_UP: Forward = event.KeyInput.PressedDown; break;
            case KEY_DOWN: Back = event.KeyInput.PressedDown; break;

            case KEY_PRIOR: Up = event.KeyInput.PressedDown; break;
            case KEY_NEXT: Down = event.KeyInput.PressedDown; break;

            case KEY_KEY_D: Right = event.KeyInput.PressedDown; break;
            case KEY_KEY_A: Left = event.KeyInput.PressedDown; break;

            case KEY_KEY_W: Forward = event.KeyInput.PressedDown; break;
            case KEY_KEY_S: Back = event.KeyInput.PressedDown; break;

            case KEY_KEY_E: Up = event.KeyInput.PressedDown; break;
            case KEY_KEY_C: Down = event.KeyInput.PressedDown; break;

            default: break;
         }

         return true;
      }
      return false;
   }

   bool leftPressed(){return Left;}
   bool rightPressed(){return Right;}

   bool forwardPressed(){return Forward;}
   bool backPressed(){return Back;}

   bool upPressed(){return Up;}
   bool downPressed(){return Down;}

} Receiver;


// moves node around scene given commands detected by receiver
void control(scene::ISceneNode *node, f32 speed)
{
   if(Receiver.leftPressed()) node->setPosition(node->getPosition() + core::vector3df(0,0,-speed));
   if(Receiver.rightPressed()) node->setPosition(node->getPosition() + core::vector3df(0,0,speed));

   if(Receiver.forwardPressed()) node->setPosition(node->getPosition() + core::vector3df(-speed,0,0));
   if(Receiver.backPressed()) node->setPosition(node->getPosition() + core::vector3df(speed,0,0));

   if(Receiver.upPressed()) node->setPosition(node->getPosition() + core::vector3df(0,speed,0));
   if(Receiver.downPressed()) node->setPosition(node->getPosition() + core::vector3df(0,-speed,0));
}

int main()
{
   IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
      core::dimension2d<s32>(640, 480), 16, false, false, false, &Receiver);

   device->setWindowCaption(L"Irrlicht Vzor");

   video::IVideoDriver* driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();

   // add arrow to the scene which will represent object rotating towards target
   scene::IMesh *mesh = smgr->addArrowMesh("arrow", video::SColor(180,255,0,0),
      video::SColor(180,0,0,255), 4,8, 20,15, 0.5f,1 )->getMesh(0);
   // arrow mesh points along Y axis originaly we have to change that
   core::matrix4 m;
   m.setRotationDegrees(core::vector3df(90,0,0));
   smgr->getMeshManipulator()->transformMesh(mesh, m);
   scene::ISceneNode *node = smgr->addMeshSceneNode(mesh);
   node->setMaterialFlag(video::EMF_LIGHTING, false);
   node->setPosition(core::vector3df(0,0,10));

   // target marker
   mesh = smgr->addSphereMesh("sphere", 1)->getMesh(0);
   smgr->getMeshManipulator()->setVertexColors(mesh, video::SColor(255,255,0,0));
   scene::ISceneNode *target = smgr->addMeshSceneNode(mesh);
   target->setPosition(core::vector3df(0,10,30));
   target->setMaterialFlag(video::EMF_LIGHTING, false);

   // camera
   smgr->addCameraSceneNode(0, node->getPosition()+core::vector3df(20,20,-20),node->getPosition());

   while(device->run())
   {
      driver->beginScene(true, true, video::SColor(0,200,200,200));

      control(target, 0.01f); // set diferent speed if its too fast/slow
      rotateTo(node, 0.01f, target->getPosition()); // set diferent speed if its too fast/slow

      smgr->drawAll();

      driver->endScene();
   }

   device->drop();

   return 0;
}
