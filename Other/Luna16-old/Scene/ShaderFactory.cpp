/*
#include <irrlicht.h>
#include <string>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int AddShader(){   //!Test Area for toon and cell shader

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


// OpenGL Fragment Program 1.1
const stringc fragCellShader =
    "void main()"
    "{"
    "  gl_FragColor = vec4(0.0,0.0,0.0,1.0);"
    "}";


	// Create GLSL shaders
	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	s32 mtlToonShader = video::EMT_SOLID; // Fallback material type
	bool bCanDoGLSL_1_1 = false;
	if (1){                     //(gpu && (driverType == video::EDT_OPENGL)) {
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


	IAnimatedMesh* mesh = smgr->getMesh("./data/sydney.md2");
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation ( scene::EMAT_STAND );
		node->setMaterialTexture( 0, driver->getTexture("./data/sydney.bmp") );
		node->setMaterialType((video::E_MATERIAL_TYPE)mtlToonShader); // Override material type
	}
};
*/
