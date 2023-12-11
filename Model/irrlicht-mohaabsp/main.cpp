//to see the demo place the compiled exe and map in the mohaa/main folder


#include <irrlicht.h>
    #include <iostream>

using namespace irr;
using namespace scene;
using namespace gui;
using namespace video;
using namespace core;
using namespace quake3;
using namespace io;

    #include "CMOHAALevelMesh.h"
#include "CBSPMeshFileLoader.h"

	Q3LevelLoadParameter loadParam;


    int main( ) {
            video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

            IrrlichtDevice *device = createDevice( driverType, core::dimension2d<u32>(640,480), 32, false, false, false, NULL );

            if ( !device ) {
                    std::cout << "Driver failed.\n";
                    return 1;
            }

            device->setWindowCaption( L"Irrlicht: Allied Assault" );
           // device->getFileSystem()->addFileArchive("main",true,false,io::EFAT_FOLDER,"");
            device->getFileSystem()->addFileArchive("./Pak0.pk3");
             device->getFileSystem()->addFileArchive("./Pak1.pk3");
            device->getFileSystem()->addFileArchive("./Pak2.pk3");
  device->getFileSystem()->addFileArchive("./pak6.pk3");


            video::IVideoDriver *driver = device->getVideoDriver();
            scene::ISceneManager *smgr = device->getSceneManager();


            scene::IAnimatedMesh *mesh = 0;
            scene::ISceneNode *node = 0;

	// default Quake3 loadParam
	loadParam.defaultLightMapMaterial = EMT_LIGHTMAP;
	loadParam.defaultModulate = EMFN_MODULATE_1X;
	loadParam.defaultFilter = EMF_ANISOTROPIC_FILTER;
	loadParam.verbose = 2;
	loadParam.mergeShaderBuffer = 1;		// merge meshbuffers with same material
	loadParam.cleanUnResolvedMeshes = 1;	// should unresolved meshes be cleaned. otherwise blue texture
	loadParam.loadAllShaders = 1;			// load all scripts in the script directory
	loadParam.loadSkyShader = 0;			// load sky Shader
	loadParam.alpharef = 1;


        io::IFileSystem *filesys;

        filesys=device->getFileSystem();

     //  irr::io::path path2("mohdm6.bsp");
      // irr::io::IReadFile *file;
       //  file=filesys->createAndOpenFile("mohdm6.bsp");
      //  file->getFileName();
            //  CBSPMeshFileLoader2 cmesh(smgr, device->getFileSystem()); //createMesh
              CMOHAALevelMesh cmesh(filesys,smgr,loadParam); //createMesh

//        mesh=cmesh.createMesh( filesys->createAndOpenFile("mohdm6.bsp"));
//        mesh=cmesh.createMesh( filesys->createAndOpenFile("m1l1.bsp"));

        cmesh.loadFile(filesys->createAndOpenFile("mohdm6.bsp"));
        //mesh=cmesh.getMesh(0,0,0,1);
      //  IMesh* CMOHAALevelMesh::getMesh(s32 frameInMs, s32 detailLevel, s32 startFrameLoop, s32 endFrameLoop)
       // cmesh.getMesh( "mohdm6.bsp");

//        mesh =cmesh.getMesh();
        //cmesh->loadFile
       // CMOHAALevelMesh mesh2=new CMOHAALevelMesh(filesys,smgr,cmesh.LoadParam);

     //   CMOHAALevelMesh *mesh2;
       // s32
           // mesh2->getMesh(L"./mohdm6.bsp");

//            if ( cmesh )
                    node = smgr->addOctreeSceneNode( cmesh.getMesh(0,0,0,1) );

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

