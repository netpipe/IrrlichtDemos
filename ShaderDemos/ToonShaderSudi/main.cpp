#include <irrlicht.h>

// --------------------------------------------------------------------------

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// --------------------------------------------------------------------------

#pragma comment(lib, "Irrlicht.lib")

// --------------------------------------------------------------------------

IrrlichtDevice* device = 0;
bool UseHighLevelShaders = true;

// OpenGL Vertex Program 1.1
const stringc vertToonShader =
"uniform vec3 lightPos;"
"varying vec3 lightDir;"
"varying vec3 normal;"
"varying vec2 Texcoord;"
"void main()"
"{"
"   normal = normalize(gl_Normal);"
"   Texcoord    = gl_MultiTexCoord0.xy;"
"   lightDir = normalize(lightPos-gl_Vertex.xyz);"
"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"}";

// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1
const stringc fragToonShader =
"varying vec3 lightDir;"
"varying vec3 normal;"
"uniform sampler2D Texture0;"
"varying vec2 Texcoord;"
"void main()"
"{"
"   vec3 mlightDir = normalize(lightDir);"
"   vec3 mnormal = normalize(normal);"
"   float intensity = dot(mnormal, mlightDir);"
"   vec4 color = texture2D(Texture0, Texcoord);"
"   if (intensity > 0.95)"
"      color = color-vec4(color.x*0.1, color.y*0.1, color.z*0.1, 0.0);"
"   else if (intensity > 0.50)"
"      color = color-vec4(color.x*0.3, color.y*0.3, color.z*0.3, 0.0);"
"   else if (intensity > 0.25)"
"      color = color-vec4(color.x*0.5, color.y*0.5, color.z*0.5, 0.0);"
"   else"
"      color = color-vec4(color.x*0.8, color.y*0.8, color.z*0.8, 0.0);"
"   gl_FragColor = color;"
"}";


const stringc vertCellShader =
"uniform float offset;"
"void main()"
"{"
   "vec3 Pos = gl_Vertex.xyz+normalize(gl_Normal)*offset;"
   "gl_Position = gl_ModelViewProjectionMatrix*vec4(Pos,1.0);"
"}";

// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1
const stringc fragCellShader =
"void main()"
"{"
 "  gl_FragColor = vec4(0.0,0.0,0.0,1.0);"
"}";

//
// --------------------------------------------------------------------------

class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services,
			s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		// set inverted world matrix
		// if we are using highlevel shaders (the user can select this when
		// starting the program), we must set the constants by name.

		core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);
		else
			services->setVertexShaderConstant(invWorld.pointer(), 0, 4);

		// set clip matrix

		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		else
			services->setVertexShaderConstant(worldViewProj.pointer(), 4, 4);

		// set camera position

		core::vector3df pos = device->getSceneManager()->
			getActiveCamera()->getAbsolutePosition();

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&pos), 3);
		else
			services->setVertexShaderConstant(reinterpret_cast<f32*>(&pos), 8, 1);

		// set light color

		video::SColorf col(0.0f,1.0f,1.0f,0.0f);

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mLightColor",
					reinterpret_cast<f32*>(&col), 4);
		else
			services->setVertexShaderConstant(reinterpret_cast<f32*>(&col), 9, 1);

		// set transposed world matrix

		core::matrix4 world = driver->getTransform(video::ETS_WORLD);
		world = world.getTransposed();

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);
		else
			services->setVertexShaderConstant(world.pointer(), 10, 4);
	}
};

int main()
{
	// Create device
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
	device = createDevice(driverType, dimension2du(800, 600), 32, false, false, false, 0);
	if (!device) {
		printf("Error creating Irrlicht device\n");
		return 0;
	}

	// Obtain device internals
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	// Set a window caption
	device->setWindowCaption(L"ToonShader - Irrlicht Engine Demo");

		MyShaderCallBack* mc = new MyShaderCallBack();



	// Create GLSL shaders
	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	s32 mtlToonShader = video::EMT_SOLID; // Fallback material type
		s32 mtlCellShader = video::EMT_SOLID; // Fallback material type

	bool bCanDoGLSL_1_1 = false;
	if (gpu && (driverType == video::EDT_OPENGL)) {
		bCanDoGLSL_1_1 = true; // provisionally accept
		if (!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1)){
		if (!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1)) {
			printf("queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) failed\n");
			bCanDoGLSL_1_1 = false;
		}
			printf("queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1) failed\n");
			bCanDoGLSL_1_1 = false;
		}
	}
	if (bCanDoGLSL_1_1) {
		// Shader uses built-in OpenGL GLSL uniforms, hence needs no explicit constants.
		// Therefore a callback function is not needed and is NULL.
		mtlToonShader = gpu->addHighLevelShaderMaterial(
					vertToonShader.c_str(), "main", video::EVST_VS_1_1,
					fragToonShader.c_str(), "main", video::EPST_PS_1_1,
					mc, video::EMT_SOLID);
							mtlCellShader = gpu->addHighLevelShaderMaterial(
					vertCellShader.c_str(), "main", video::EVST_VS_1_1,
					fragCellShader.c_str(), "main", video::EPST_PS_1_1,
					mc, video::EMT_SOLID);
	} else {
		// This demo is for OpenGL!
		printf("This demo requires OpenGL with GLSL High-Level shaders\n");
		mtlToonShader = video::EMT_SOLID;
	}

	    device->getFileSystem()->addFileArchive("../../media/map-20kdm2.pk3");

	// Add an animated mesh
	IAnimatedMesh* mesh = smgr->getMesh("../../media/ninja.b3d");
	//IAnimatedMesh* mesh = smgr->getMesh("maps/20kdm2.bsp");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{

	//	node->setFrameLoop(0, 14);
		node->setAnimationSpeed(10);
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );
		node->setMaterialTexture( 0, driver->getTexture("../../media/nskinbl.jpg") );
		node->setMaterialType((video::E_MATERIAL_TYPE)mtlToonShader); // Override material type
	}//node->setGenericT

/*
	// Add an animated mesh
//	IAnimatedMesh* mesh = smgr->getMesh("../media/sydney.md2");
	IAnimatedMeshSceneNode* node2 = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
	    node->setAnimationSpeed( 0);

	    node2->setScale(vector3df(1.f,1.f,1.f));
		node2->setMaterialFlag(EMF_LIGHTING, false);
	//	node2->setMD2Animation ( scene::EMAT_STAND );
		node2->setMaterialTexture( 0, driver->getTexture("../media/nskinbl.jpg") );
		node2->setMaterialType((video::E_MATERIAL_TYPE)mtlCellShader); // Override material type
	}
*/
	// Add a viewing camera
scene::ICameraSceneNode * cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.10f);

	scene::ISceneNode* node3 = 0;
	node3 = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
	scene::ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyCircleAnimator (core::vector3df(0,250,0),550.0f, 20.001f);
	node3->addAnimator(anim);
	anim->drop();

	// Main rendering loop
	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		driver->endScene();
		device->sleep(10,0);
	}

	device->drop();

	// Done
	return 0;
}
