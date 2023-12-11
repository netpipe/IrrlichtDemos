// author Arras

#include "irrlicht.h"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice *device;

// free flight functions
void makeCockpit(irr::scene::ICameraSceneNode *camera,
                irr::scene::ISceneNode *node,
                irr::core::vector3df offset)
{
    irr::core::matrix4 m;
    m.setRotationDegrees(node->getRotation());

    irr::core::vector3df frv = irr::core::vector3df (0.0f, 0.0f, 1.0f);
    m.transformVect(frv);

    irr::core::vector3df upv = irr::core::vector3df (0.0f, 1.0f, 0.0f);
    m.transformVect(upv);

    m.transformVect(offset);

    offset += node->getPosition();
    camera->setPosition(offset);

    camera->setUpVector(upv);

    offset += frv;
    camera->setTarget(offset);

    camera->updateAbsolutePosition();
}

void move(irr::scene::ISceneNode *node, irr::core::vector3df vel)
{
    irr::core::matrix4 m;
    m.setRotationDegrees(node->getRotation());
    m.transformVect(vel);
    node->setPosition(node->getPosition() + vel);
    node->updateAbsolutePosition();
}

void rotate(irr::scene::ISceneNode *node, irr::core::vector3df rot)
{
    irr::core::matrix4 m;
    m.setRotationDegrees(node->getRotation());
    irr::core::matrix4 n;
    n.setRotationDegrees(rot);
    m *= n;
    node->setRotation( m.getRotationDegrees() );
    node->updateAbsolutePosition();
}

void turn(irr::scene::ISceneNode *node, irr::f32 rot)
{
    rotate(node, irr::core::vector3df(0.0f, rot, 0.0f) );
}

void pitch(irr::scene::ISceneNode *node, irr::f32 rot)
{
    rotate(node, irr::core::vector3df(rot, 0.0f, 0.0f) );
}

void roll(irr::scene::ISceneNode *node, irr::f32 rot)
{
    rotate(node, irr::core::vector3df(0.0f, 0.0f, rot) );
}



// event reciever
bool keys[KEY_KEY_CODES_COUNT];

class MyEventReceiver : public IEventReceiver
{

public:

    virtual bool OnEvent(const SEvent &event)
    {
        if(event.EventType == EET_KEY_INPUT_EVENT)
        {
            keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
            return true;
        }
        return false;
    }
};



int main()
{
    for(s32 i=0; i<KEY_KEY_CODES_COUNT; i++) {keys[i] = false;};
    MyEventReceiver receiver;
   device =
      createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 600), 32, false, false, false, &receiver);

//   device->setResizeAble(true);

   video::IVideoDriver* driver = device->getVideoDriver();
   scene::ISceneManager* smgr = device->getSceneManager();

   scene::ISceneNode* node1 = smgr->addCubeSceneNode(); //replace with what ever you want
   if (node1)
      node1->setMaterialFlag(video::EMF_LIGHTING, false);

   scene::ISceneNode* node2 = smgr->addCubeSceneNode(); //just for reference
   if (node2)
      node2->setMaterialFlag(video::EMF_LIGHTING, false);
   node2->setPosition(core::vector3df(0,0,100));

   scene::ICameraSceneNode *camera = device->getSceneManager()->addCameraSceneNode();

   while(device->run())
   {
        driver->beginScene(true, true, video::SColor(0,0,0,0));
        smgr->drawAll();

        // direction control
        if(keys[irr::KEY_LEFT])
        {
            turn(node1, 0.1);
        }
        if(keys[irr::KEY_RIGHT])
        {
            turn(node1, -0.1);
        }
        if(keys[irr::KEY_UP])
        {
            pitch(node1, 0.1);
        }
        if(keys[irr::KEY_DOWN])
        {
            pitch(node1, -0.1);
        }
        if(keys[irr::KEY_COMMA])
        {
            roll(node1, 0.1);
        }
        if(keys[irr::KEY_PERIOD])
        {
            roll(node1, -0.1);
        }

        // movement control
        if(keys[irr::KEY_KEY_W])
        {
            move(node1, core::vector3df(0,0,0.1));
        }
        if(keys[irr::KEY_KEY_S])
        {
            move(node1, core::vector3df(0,0,-0.1));
        }

        makeCockpit(camera, node1, core::vector3df(0,7,-30));

        driver->endScene();
    }

   device->drop();

   return 1;
}
