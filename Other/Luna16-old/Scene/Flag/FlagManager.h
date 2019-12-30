
#ifdef FLAG
// flag scene node
	#include "SimpleFlag.h"
	SimpleFlagNode	*irrFlagNode = 0;

// demo veriables
	vector3df	GravityVec(0, -9.8f, 0);
	bool	doWind 			= true;
	bool	showWire		= false;
	bool	pausePhysics	= false;

SMaterial Mat;
		Mat.Lighting		= false;
		Mat.BackfaceCulling = false;

	// Create the Flag Scene Node (dont forget to drop() it afterwards)
		irrFlagNode = new SimpleFlagNode
		(
			10, 20,		// flag dimensions (change to whatever size required, or scale the node later)
			10, 20,		// flag tesselation (good default value)

			Mat,		// initial material properties
			GravityVec,	// gravity (depends on the scale of the scene and the tesselation)

			2,			// iterations (change at your own peril)
			1.99f,		// spring-stiffness (change at your own peril)

			smgr->getRootSceneNode(), smgr, -1	// parent, irrlicht scene manager, ID
		);

		if( !irrFlagNode ) return 0; // unknown error

	// assign a texture and enable alpha transparency (id is always 0)
		irrFlagNode->getMaterial(0).setTexture( 0, driver->getTexture( "media/Flags/agnostechdemo1200912132.png" ) );
		irrFlagNode->getMaterial(0).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;


	// node can be manipulated like any other irrlicht node
		irrFlagNode->setScale( vector3df(10) );
#endif

#ifdef FLAG2
#include "CFlagSceneNode.h"
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

	ILogger* output = device->getLogger();

	path vsFileName = "flag_shader.vert";
	path psFileName = "flag_shader.frag";

	if(!driver->queryFeature(EVDF_PIXEL_SHADER_1_1) && !driver->queryFeature(EVDF_ARB_FRAGMENT_PROGRAM_1))
	{
		output->log("WARNING: Pixel shaders disabled because of missing driver/hardware support.");
		psFileName = "";
	}

	if(!driver->queryFeature(EVDF_VERTEX_SHADER_1_1) && !driver->queryFeature(EVDF_ARB_VERTEX_PROGRAM_1))
	{
		output->log("WARNING: Vertex shaders disabled because of missing driver/hardware support.");
		vsFileName = "";
	}

	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	int materialType = 0;
	FlagShaderCallBack* cb = new FlagShaderCallBack();
	if(gpu)
	{
// add your own light settings here
		cb->setAmbient(SColorf(0.1f,0.1f,0.1f,1.0f));
		cb->setDiffuse(SColorf(1.0f,1.0f,1.0f,1.0f));
		cb->setTranslucency(SColorf(0.0f,0.0f,0.0f,0.0f));
		cb->setLight(0);	// use the first light
		cb->setSampler(0);

		materialType = gpu->addHighLevelShaderMaterialFromFiles(vsFileName, "main", EVST_VS_1_1,
												psFileName, "main", EPST_PS_1_1,cb, EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
		cb->drop();
	}

	IImage* vertweightmap = driver->createImageFromFile("media/Flags/weightmap.tga");

// create a couple of perlin generators
	Perlin1D* pgen1 = new Perlin1D(128,5,0.05f,2.0f,rand());
	Perlin1D* pgen2 = new Perlin1D(128,6,0.04f,2.0f,rand());

// create a wind generator, there maybe issues/bugs with the direction handling
	CWindGen* wind = new CWindGen(vector3df(0.0f,0.0f,1.0f),30.0,pgen1,0.5f,1.5f,pgen2);

// create the flag
	CFlagSceneNode* flag = new CFlagSceneNode(smgr->getRootSceneNode(),smgr,345,wind,20.0f,16,12,20.0f,0.994f,
						0.05f,vector3df(0.0f,-0.08f,0.0f),vertweightmap);
	flag->getMaterial(0).setTexture(0, driver->getTexture("media/Flags/agnostechdemo1200912132.png"));
	flag->getMaterial(0).MaterialType = (E_MATERIAL_TYPE)materialType;
	flag->setPosition(vector3df(0.0f,0.0f,0.0f));

// drop the weight map

	vertweightmap->drop();

	ILightSceneNode* lightnode = smgr->addLightSceneNode();
	lightnode->setLightType(ELT_DIRECTIONAL);
	lightnode->setRotation(vector3df(30.0f, -25.0f, 0.0f));
	lightnode->setPosition(vector3df(6.7f, 32.0f,-11.0f));

//	ICameraSceneNode* cam = smgr->addCameraSceneNodeMaya();
//	cam->setPosition(vector3df(150,0,0));
//	cam->setTarget(vector3df(0,0,0));
#endif
