
/*
void main()
{
  vec3 lightDir = (gl_ModelViewMatrix * gl_Vertex
                   - gl_LightSource[0].position).xyz;

   // if the vertex is lit
   float dt = dot(lightDir, gl_NormalMatrix * gl_Normal);
   if ( dt <0.01 )
   {
      // don't move it
       gl_Position = ftransform();
   }
   else
   {
      // move it far, is the light direction
      vec4 fin = gl_ProjectionMatrix * (
                 gl_ModelViewMatrix * gl_Vertex
                 + vec4(normalize(lightDir) * 100000.0, 0.0)
               );

      // if fin is behind the far plane
      if ( fin.z > fin.w )
            fin.z = fin.w; // move to the far plane (needed for z-fail algo.)

      gl_Position = fin;
   }
}
*/



#include <irrlicht.h>
#pragma comment(lib,"irrlicht.lib")

#define WIN32_LEAN_AND_MEAN
//#include <windows.h>

#define GL_GLEXT_LEGACY 1
#include <GL/gl.h>
#include "glext.h"
#pragma comment(lib, "OpenGL32.lib")

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice          * pDevice    = 0;
IVideoDriver            * pVideo     = 0;
ISceneManager           * pScene     = 0;
IGUIEnvironment         * pGUI       = 0;
ISceneCollisionManager  * pCollision = 0;
IGPUProgrammingServices * pGPU       = 0;
IFileSystem             * pFile      = 0;

int main(int argc,char* argv[])
{
	SIrrlichtCreationParameters params;
	params.DriverType    = EDT_OPENGL;
	params.WindowSize    = dimension2du(800,600);
	params.Bits		     = 32;
	params.Stencilbuffer = true;
	pDevice = createDeviceEx(params);

	pVideo     = pDevice->getVideoDriver();
	pScene     = pDevice->getSceneManager();
	pGUI       = pDevice->getGUIEnvironment();
	pCollision = pScene->getSceneCollisionManager();
	pGPU       = pVideo->getGPUProgrammingServices();
	pFile      = pDevice->getFileSystem();

	IGUIFont* font = pGUI->getFont("bigfont.png");

	// CAMERA
	// ============================================
	ICameraSceneNode* camera = pScene->addCameraSceneNode();
	camera->setPosition(vector3df(0,256,-256.0f));
	camera->setTarget(vector3df(0,0,0));

	// LIGHT
	// ============================================
	ILightSceneNode* light = pScene->addLightSceneNode(0,vector3df(128,128,128),SColorf(1,1,1,1),256.f);
	light->setLightType(video::ELT_POINT);
	light->setDebugDataVisible(EDS_BBOX);
	//light->enableCastShadow(true);

	// ANIMATOR FOR THE LIGHT
	// ============================================
	ISceneNodeAnimator* anm = pScene->createFlyCircleAnimator(vector3df(0,128,0),128.f,0.0015f);
	light->addAnimator(anm);

	// MATERIAL AND SHADER
	// ============================================
	int shader = pGPU->addShaderMaterialFromFiles("shadow.vp","");

	SMaterial mat;
	mat.MaterialType = (E_MATERIAL_TYPE)shader;
	mat.ZBuffer = ECFN_LESSEQUAL;
	mat.ColorMask = ECP_NONE;
	mat.ZWriteEnable = 0;
	mat.Lighting = false;
	mat.FogEnable = false;

	// MESH
	// ============================================
	IAnimatedMeshSceneNode* mesh = pScene->addAnimatedMeshSceneNode(pScene->getMesh("barrel.b3d"));
	//mesh->addShadowVolumeSceneNode();

	for(u32 i=0; i<mesh->getMesh()->getMeshBufferCount(); ++i)
	mesh->getMesh()->getMeshBuffer(i)->setHardwareMappingHint(EHM_STATIC);

	// PLANE
	// ============================================
	IAnimatedMeshSceneNode* plane = pScene->addAnimatedMeshSceneNode(pScene->getMesh("plane.b3d"));

	while(pDevice->run())
	{
		if(pDevice->isWindowActive())
		{
			pVideo->beginScene(true,true,SColor(255,128,128,128));

			pScene->drawAll();

			// store current OpenGL state
			glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT |
			GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);

			glEnable(GL_STENCIL_TEST);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(0.0f, 1.0f);

			glStencilMask(~0);
			glStencilFunc(GL_ALWAYS, 0, ~0);

			// ZFAIL Method
			/*glCullFace(GL_FRONT);*/
			mat.BackfaceCulling = 0;
			mat.FrontfaceCulling = 1;
			glStencilOp(GL_KEEP,GL_INCR,GL_KEEP);

			////////////////////
			pVideo->setTransform(ETS_WORLD,mesh->getAbsoluteTransformation());
			pVideo->setMaterial(mat);

			for(u32 i=0; i<mesh->getMesh()->getMeshBufferCount(); ++i)
			pVideo->drawMeshBuffer(mesh->getMesh()->getMeshBuffer(i));
			////////////////////////

			/* glCullFace(GL_BACK); */
			mat.BackfaceCulling = 1;
			mat.FrontfaceCulling = 0;
			glStencilOp(GL_KEEP,GL_DECR,GL_KEEP);

			////////////////////
			pVideo->setTransform(ETS_WORLD,mesh->getAbsoluteTransformation());
			pVideo->setMaterial(mat);

			for(u32 i=0; i<mesh->getMesh()->getMeshBufferCount(); ++i)
			pVideo->drawMeshBuffer(mesh->getMesh()->getMeshBuffer(i));
			////////////////////////

			glPopAttrib();

			pVideo->drawStencilShadow(1,SColor(100,0,0,0),SColor(100,0,0,0),SColor(100,0,0,0),SColor(100,0,0,0));

			// Debug
			stringw dsd = L"FPS: ";
			dsd += pVideo->getFPS();
			dsd += "\n";
			dsd += "TRIANGLES: ";
			dsd += pVideo->getPrimitiveCountDrawn();
			font->draw(dsd,rect<s32>(1,1,1,1),SColor(255,255,255,255));

			pVideo->endScene();
			pDevice->sleep(4,0);
		}
	}

	pVideo->removeAllHardwareBuffers();
	pVideo->removeAllTextures();
	pDevice->drop();
	pDevice = 0;

	return 0;
}
