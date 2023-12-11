#include <irrlicht.h>
#include <iostream>


using namespace irr;
using namespace scene;


#pragma comment(lib, "Irrlicht.lib")


typedef struct
{
    scene::IMesh *geometry;
	scene::ISceneNode *node;

} Quake3_Tp;


/*
  Load a QUAKE3 Map with internal objects (Lamps, Lights, etc) + Collisions

  By Colombian Developers
*/

Quake3_Tp LOAD_Q3SHADERS(char *ruta, IMetaTriangleSelector * plistacol,scene::ISceneManager* smgr)
{
	scene::IQ3LevelMesh *mesh = (scene::IQ3LevelMesh*) smgr->getMesh(ruta);
 	scene::ISceneNode *node= 0;
	scene::IMesh *geometry= 0;

	if ( mesh )
	{
		geometry = mesh->getMesh(quake3::E_Q3_MESH_GEOMETRY );
		node = smgr->addMeshSceneNode   (  geometry );

		if (plistacol)
	      plistacol->addTriangleSelector(smgr->createOctTreeTriangleSelector( geometry,node));
	}

	/*
		now construct SceneNodes for each Shader
		The Objects are stored in the quake mesh scene::E_Q3_MESH_ITEMS
		and the Shader ID is stored in the MaterialParameters
		mostly dark looking skulls and moving lava.. or green flashing tubes?
	*/
	if ( mesh )
	{
		// the additional mesh can be quite huge and is unoptimized
		scene::IMesh * additional_mesh = mesh->getMesh ( quake3::E_Q3_MESH_ITEMS );

		for ( u32 i = 0; i!= additional_mesh->getMeshBufferCount (); ++i )
		{
			IMeshBuffer *meshBuffer = additional_mesh->getMeshBuffer ( i );
			const video::SMaterial &material = meshBuffer->getMaterial();

			//! The ShaderIndex is stored in the material parameter
			s32 shaderIndex = (s32) material.MaterialTypeParam2;

			// the meshbuffer can be rendered without additional support, or it has no shader
			const quake3::SShader *shader = mesh->getShader ( shaderIndex );
			if ( 0 == shader )
			{
				continue;
			}

			// we can dump the shader to the console in it's original but already parsed layout
			// in a pretty printers way.. commented out, because the console would be full...
			// quake3::dumpShader ( Shader );

			// Now add the MeshBuffer(s) with the current Shader to the Manager


			scene::ISceneNode *modelo= smgr->addQuake3SceneNode ( meshBuffer, shader );

			if (plistacol) //collisions with Q3items?
			{
				scene::SMesh *modelom= new scene::SMesh;
				modelom->addMeshBuffer (meshBuffer);

			    plistacol->addTriangleSelector (smgr->createOctTreeTriangleSelector(modelom,modelo));

				delete modelom;
			}


		}

		// original mesh is not needed anymore
		mesh->releaseMesh ( quake3::E_Q3_MESH_ITEMS );
	}

   Quake3_Tp ret;
   ret.geometry = geometry;
   ret.node = node;

   return  ret;
}



/*
Ok, lets start.
*/

int IRRCALLCONV main(int argc, char* argv[])
{


    bool Colisiones=false;
	printf("Enable collisions with Q3 map? (y/n) \n");
	//std::cin >> i;
//	if (i=='y')
	 Colisiones=true;


	// create device and exit if creation failed
	core::dimension2di videoDim ( 800,600 );

	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, videoDim, 32, false );

	if (device == 0)
		return 1; // could not create selected driver.


	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

    //create a collision list
    IMetaTriangleSelector *lista= NULL;
	if (Colisiones)	lista= smgr->createMetaTriangleSelector() ;


	/*
	To display the Quake 3 map, we first need to load it. Quake 3 maps
	are packed into .pk3 files wich are nothing other than .zip files.
	So we add the .pk3 file to our FileSystem. After it was added,
	we are able to read from the files in that archive as they would
	directly be stored on disk.
	*/

	device->getFileSystem()->addZipFileArchive("ikzdm1.pk3");


	Quake3_Tp ret= LOAD_Q3SHADERS("ikzdm1.bsp",lista, smgr);

    //create a camera
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	//camera->setPosition(core::vector3df(891,678,1719) );

	//create a auto response animator
	if (Colisiones) //enable Q3 collisions
	  camera->addAnimator( smgr->createCollisionResponseAnimator(
		   lista,
		   camera,
		   core::vector3df(30,30,30),core::vector3df(0,-1,0),core::vector3df(0,30,0)
		   ) ) ;
	else //normal irr-collisions, q3 items not exist
		camera->addAnimator( smgr->createCollisionResponseAnimator(
		   smgr->createOctTreeTriangleSelector( ret.geometry,ret.node),
		   camera,
		   core::vector3df(30,30,30),core::vector3df(0,-1,0),core::vector3df(0,30,0)
		   ) );


	device->getCursorControl()->setVisible(false);


	while(device->run())
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, video::SColor(255,20,20,40));
		smgr->drawAll();
		driver->endScene();
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();

	return 0;
}


