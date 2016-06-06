//!
//! DEMO FOR SCAPEMAKELOADER, CLODSCENENODE, QUADTREESCENENODE, SCENEREADER AND MYMATERIAL
//!
//! NOTE THAT THIS WORKS ONLY WITH DirectX due to custom material for mesh fading
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.

#include <irrlicht.h>

#include <string.h>
#include "SceneReader.h"

#include "CQuadTreeSceneNode.h"
#include "CLODSceneNode.h"
#include "ScapeMakerLoader.h"

using namespace irr;

int main(int argc, char *argv[])
{
    IrrlichtDevice* device = createDevice( video::EDT_OPENGL, core::dimension2d<s32>( 800, 600 ), 32, false );

    scene::ISceneManager* manager = device->getSceneManager();
    video::IVideoDriver* driver = device->getVideoDriver();

    ScapeMakeLoader* loader = new ScapeMakeLoader( device );
    loader->LoadScape( "river", 30 );

    scene::ITerrainSceneNode* terrain = loader->getTerrain();

    CSimpleWaterSceneNode* wat = new CSimpleWaterSceneNode( device, -1, 140, terrain, "river" );

    manager->addSkyBoxSceneNode(
        driver->getTexture( "river//engine//river_sb_top.jpg" ),
        driver->getTexture( "river//engine//river_sb_bottom.jpg" ),
        driver->getTexture( "river//engine//river_sb_right.jpg" ),
        driver->getTexture( "river//engine//river_sb_left.jpg" ),
        driver->getTexture( "river//engine//river_sb_front.jpg" ),
        driver->getTexture( "river//engine//river_sb_back.jpg" ),
        0,
        -1 );

    scene::ICameraSceneNode* cam = manager->addCameraSceneNodeFPS();
    cam->setFarValue(2000);
    cam->setFOV(2.0f);

    int lastFPS = -1;
    while ( device->run() )
    {
        driver->beginScene( true, true, video::SColor(0,0,0,0) );

        manager->drawAll();

        driver->endScene();

        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw str = L"Irrlicht Engine [";
            str += driver->getName();
            str += "] FPS:";
            str += fps;
            str += "  Total triangles: ";
            s32 primcount = driver->getPrimitiveCountDrawn();
            str += primcount;
            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
    }

    delete loader;
    delete wat;

    device->drop();

    return 0;
}
