// --------------------------------------------------------------------------
// RadialBlur Shader Aplication By TGM
// feel free to use this as you want!
// --------------------------------------------------------------------------

#include <irrlicht.h>
#include "PostProcessRadialBlur.h"

// --------------------------------------------------------------------------

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// --------------------------------------------------------------------------

IrrlichtDevice *device;

int main()
{

	// create device and exit if creation failed

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1024, 768),
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

    IPostProcessRadialBlur *Blur = new IPostProcessRadialBlur(smgr->getRootSceneNode(), smgr, 666);
    Blur->initiate(1024,512,0.8,2,smgr);
    //every Post process has to be initiated, how to do this depends on the node.
    //this time we hav to feed the function just with the sampleDist, and the samplestrength,
    //a value that determines how much of the screen will be blurred.
    //the first two values are the size of the Rendertarget, the last the pointer to the sceneManager

	int lastFPS = -1;

	while(device->run())
	if (device->isWindowActive())
	{
	driver->beginScene(true, true, 0);
	driver->setRenderTarget(Blur->rt0, true, true, video::SColor(0,0,0,0));  //First Render The entire scene into the renderTarget
	smgr->drawAll();                                                          //of the PostProcess
    driver->setRenderTarget(0);                                               //set The Rendertarget to the mainview
    Blur->render();                                                           //Then let the first Post Process Render himself                                                                                                          //So now let the second PPE render himself to the screen.
    driver->endScene();                                                       //thats all^^ quite simple isn´t it?
			int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw str = L"RadialBlur Shader Demo - TGMs Shader Pack[";
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
