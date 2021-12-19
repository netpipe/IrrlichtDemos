//to see the demo place the compiled exe and map in the mohaa/main folder


#include <irrlicht.h>
    #include <iostream>
    #include "CMOHAALevelMesh.h"
#include "CBSPMeshFileLoader.h"

    using namespace irr;
using namespace scene;

    int main( ) {
            video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

            IrrlichtDevice *device = createDevice( driverType, core::dimension2d<u32>(640,480), 32, false, false, false, NULL );

            if ( !device ) {
                    std::cout << "Driver failed.\n";
                    return 1;
            }

            device->setWindowCaption( L"Irrlicht: Allied Assault" );
            device->getFileSystem()->addFileArchive("main",true,false,io::EFAT_FOLDER,"");

            video::IVideoDriver *driver = device->getVideoDriver();
            scene::ISceneManager *smgr = device->getSceneManager();


            scene::IAnimatedMesh *mesh = 0;
            scene::ISceneNode *node = 0;


        CBSPMeshFileLoader cmesh(smgr, device->getFileSystem()); //createMesh

      //  irr::io::path path2("mohdm6.bsp");
        irr::io::IReadFile *file("mohdm6.bsp");
     //   file->getFileName(path2);
        cmesh.createMesh( file);

       // cmesh.getMesh( "mohdm6.bsp");

      //  mesh =cmesh.getMesh();
        //cmesh->loadFile
      //  CMOHAALevelMesh mesh=new CMOHAALevelMesh(,smgr,"mohdm6.bsp");

      //  CMOHAALevelMesh *mesh;

            //mesh->getMesh("mohdm6.bsp")

            if ( mesh )
                    node = smgr->addOctreeSceneNode( mesh->getMesh(0) );

            if (node) {
             //     node->setMaterialFlag(video::EMF_LIGHTING,false);
             //     node->setMaterialFlag(video::EMF_WIREFRAME,true);
            }

            smgr->addCameraSceneNodeFPS();
            device->getCursorControl()->setVisible(false);

            int lastFPS = -1;
            while ( device->run() ) {
                    driver->beginScene(true, true, video::SColor(0,200,200,200));

                    smgr->drawAll();
                    driver->endScene();

                    int fps = driver->getFPS();

                    if ( lastFPS != fps ) {
                            core::stringw str = L"Irrlicht Engine - MOHAA test [";
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

