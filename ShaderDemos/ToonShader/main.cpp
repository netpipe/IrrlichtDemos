
// --------------------------------------------------------------------------
// Irrlicht Toon (Cel) Shader Demo
// sio2 'at' users.sourceforge.net
// --------------------------------------------------------------------------

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

// OpenGL Vertex Program 1.1
const stringc vertToonShader =
"varying vec3 normal;"
"void main()"
"{"
"	normal = gl_NormalMatrix * gl_Normal;"
"	gl_Position = ftransform();"
"}";

// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1
const stringc fragToonShader =
"varying vec3 normal;"
"void main()"
"{"
"	float intensity;"
"	vec4 color;"
"	vec3 n = normalize(normal);"
""
"	intensity = dot(vec3(gl_LightSource[0].position),n);"
""
"	if (intensity > 0.95)"
"		color = vec4(1.0,0.5,0.5,1.0);"
"	else if (intensity > 0.5)"
"		color = vec4(0.6,0.3,0.3,1.0);"
"	else if (intensity > 0.25)"
"		color = vec4(0.4,0.2,0.2,1.0);"
"	else"
"		color = vec4(0.2,0.1,0.1,1.0);"
""
"	gl_FragColor = color;"
"}";

// --------------------------------------------------------------------------

int main()
{
	// Create device
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
	IrrlichtDevice *device = createDevice(driverType, dimension2di(800, 600), 32, false, false, false, 0);
	if (!device) {
		printf("Error creating Irrlicht device\n");
		return 0;
	}

	// Obtain device internals
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	// Set a window caption
	device->setWindowCaption(L"ToonShader - Irrlicht Engine Demo");

	// Create GLSL shaders
	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	s32 mtlToonShader = video::EMT_SOLID; // Fallback material type
	bool bCanDoGLSL_1_1 = false;
	if (gpu && (driverType == video::EDT_OPENGL)) {
		bCanDoGLSL_1_1 = true; // provisionally accept
		if (!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1)) {
			printf("queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) failed\n");
			bCanDoGLSL_1_1 = false;
		}
		if (!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1)) {
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
					NULL, video::EMT_SOLID);
	} else {
		// This demo is for OpenGL!
		printf("This demo requires OpenGL with GLSL High-Level shaders\n");
		mtlToonShader = video::EMT_SOLID;
	}

	// Add an animated mesh
	IAnimatedMesh* mesh = smgr->getMesh("./data/sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );
		node->setMaterialTexture( 0, driver->getTexture("./data/sydney.bmp") );
		node->setMaterialType((video::E_MATERIAL_TYPE)mtlToonShader); // Override material type
	}

	// Add a viewing camera
scene::ICameraSceneNode * cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 1.0f);


	// Main rendering loop
	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		driver->endScene();
	}

	device->drop();

	// Done
	return 0;
}

// --------------------------------------------------------------------------
