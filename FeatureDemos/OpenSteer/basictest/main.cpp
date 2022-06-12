//Second...
//and a put a comment " My doubt is here" in the problematic.
//how to use:
 //  clock.update ();
 //  obj->update();
 //  setPosition();
 //  setRotation();

#include <irrlicht.h>

#include "main.h"

using namespace irr;
using namespace gui;

int main()
{
    //make a opengl device without keymap
    IrrlichtDevice *device =
    device = createDevice(video::EDT_OPENGL, // Render com openGL
       core::dimension2d<u32>(800, 600),     // resolucao 800X600
       32,                                   // colors
       false,                                // full screen
       false,                                // stencilbuffer
       false,                                // vsync
       0);                                   // keymap

    device->setWindowCaption(L"Irrlicht X Opensteer");
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

       //get mesh
    scene::IAnimatedMesh * mesh = smgr->getMesh("../../../media/dwarf.x");
    scene::IAnimatedMeshSceneNode * enemynode = smgr->addAnimatedMeshSceneNode( mesh );

    //make a scene node
    MyObject * obj;
    if (enemynode )
    {
        enemynode->setMaterialTexture( 0, driver->getTexture("../../../media/crate.png") );
        enemynode->setPosition(core::vector3df(30, 0, 30));
        enemynode->setScale(core::vector3df(1,1,1));
                   enemynode->setMaterialFlag(video::EMF_LIGHTING, false);
                enemynode->setRotation(core::vector3df(-90,0,0) ) ;
                obj = new MyObject(enemynode);
    }

       // add camera
    scene::ICameraSceneNode * camera = smgr->addCameraSceneNodeFPS();
    camera->setPosition(core::vector3df(-15.0f,0.0f, -40));

    float angulo;
    static OpenSteer::Clock clock;
    while(device->run())
    {
        driver->beginScene(true, true,  video::SColor(0,0,0,0));
        smgr->drawAll();
        angulo+= 10.0f;

        if (obj) {
             // ************************************************
             ///           My doubt is here
                     // ************************************************
              clock.update ();
              obj->update(clock.getTotalSimulationTime(), clock.getElapsedSimulationTime ());

              obj->MeshNode->setPosition(core::vector3df( obj->position().x, obj->position().y, obj->position().z ) );
              obj->MeshNode->setRotation(core::vector3df( -90, -40, 0 ) );

        }

        printf("to %d ( %f, %f, %f ) !\n", obj->serialNumber, obj->position().x, obj->position().y, obj->position().z );
        driver->endScene();
    }

    device->drop();
    return 0;
}
