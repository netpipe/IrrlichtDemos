// --------------------------------------------------------------------------
// Glass Shader Aplication By TGM
// feel free to use this as you want!
// --------------------------------------------------------------------------

#include <irrlicht.h>
#include "PostProcessGlass.h"





// --------------------------------------------------------------------------

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// --------------------------------------------------------------------------

IrrlichtDevice *device;


const stringc vertGlassShader =
"varying vec3 fvNormal;"
"varying vec3 fvViewVec;"
"void main( void )"
"{"
"float dist =0.00087;" //maybe this value must be changed depending on the world size
"   gl_Position = ftransform();"
"   fvNormal    = gl_NormalMatrix * gl_Normal;"
"   fvViewVec = vec3(dist * gl_ModelViewMatrix * gl_Vertex);"
"}"
;
// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1
const stringc fragGlassShader =
"varying vec3 fvNormal;"
"varying vec3 fvViewVec;"
""
"void main( void )"
"{"
"  vec4 color;"
"  vec4 temp;"
"  temp.xyz=-normalize(fvNormal);"
"  color.r=temp.r+0.5;"
"  color.g=temp.g+0.5;"
"  color.b=temp.b+0.5;"
"  float t=(1.0-fvViewVec.z)/2.0;"
"  if(t<0.0){t=0.0;}"
"  gl_FragColor = color*t;"
"}"
;



int main()
{

	// create device and exit if creation failed

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<s32>(1024, 768),
		16, false, false);


	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();
    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

	// add fps camera
	scene::ICameraSceneNode* fpsCamera = smgr->addCameraSceneNodeFPS();
	fpsCamera->setPosition(core::vector3df(-50,50,-150));

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	// create test cube
	scene::ISceneNode* test = smgr->addCubeSceneNode(6);
	test->setPosition(core::vector3df(0,0,0));
	test->setMaterialFlag(video::EMF_LIGHTING, false);

device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");
scene::IAnimatedMesh* mesh = smgr->getMesh("../../media/20kdm2.bsp");
scene::ISceneNode* node = 0;

if (mesh)
    node = smgr->addOctTreeSceneNode(mesh->getMesh(0));
if (node)
    node->setPosition(core::vector3df(-1300,-144,-1249));

//add the disortion mesh
   s32 mtlGlassShader = video::EMT_SOLID; // Fallback material type
   mtlGlassShader = gpu->addHighLevelShaderMaterial(
               vertGlassShader.c_str(), "main", video::EVST_VS_1_1,
               fragGlassShader.c_str(), "main", video::EPST_PS_1_1,
               NULL, video::EMT_SOLID);
   mesh = smgr->getMesh("../../media/dwarf.x");


   ISceneNode* Geometrie = smgr->addAnimatedMeshSceneNode(mesh);
   if (Geometrie)
   {
      Geometrie->setMaterialFlag(EMF_LIGHTING, false);
      Geometrie->setMaterialType((video::E_MATERIAL_TYPE)mtlGlassShader); // Override material type
   }
   //Geometrie->setRotation(vector3df(270,0,0));
   Geometrie->setPosition(vector3df(100,-80,100));



    IPostProcessGlass *Glass = new IPostProcessGlass(smgr->getRootSceneNode(), smgr, 666);
    Glass->initiate(1024,512,0.0505f,smgr);

	int lastFPS = -1;

	while(device->run())
	if (device->isWindowActive())
	{
	driver->beginScene(true, true, 0);
	driver->setRenderTarget(Glass->rt0, true, true, video::SColor(0,0,0,0));
	node->setVisible(true);
	//test->setVisible(true);
	Geometrie->setVisible(false);
	smgr->drawAll();
	Geometrie->setVisible(true);
    node->setVisible(false);
    test->setVisible(false);
    driver->setRenderTarget(Glass->rt1, true, true, video::SColor(0,0,0,0));
    smgr->drawAll();
	driver->setRenderTarget(0);
	Glass->render();
	env->drawAll();
	driver->endScene();


			int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw str = L"Glass Shader Demo WIP - TGMs Shader Pack[";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}
	device->drop(); // drop device
	return 0;
}



