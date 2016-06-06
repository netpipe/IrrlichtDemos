// --------------------------------------------------------------------------
// MotionBlur shaderr Aplication By TGM
// feel free to use this as you want!
// --------------------------------------------------------------------------

#include <irrlicht.h>
#include "PostProcessMotionBlur.h"

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

    IPostProcessMotionBlur *Blur = new IPostProcessMotionBlur(smgr->getRootSceneNode(), smgr, 666);
    Blur->initiate(1024,512,0.8,smgr);
    //every Post process has to be initiated, how to do this depends on the node.
    //this time we hav to feed the function just with the strength, a value that should lay between 0.7&0.9
    //the first two values are the size of the Rendertarget, the last the pointer to the sceneManager

	int lastFPS = -1;

	while(device->run())
	if (device->isWindowActive())
	{
	driver->beginScene(true, true, 0);  //This time the setup is a little bit harder than normal.
    Blur->render();                     //For to hold it simple you just have to call two functions:
    driver->setRenderTarget(0);         //render(), which does the mainwork(blending the differen Frames together)
    Blur->renderFinal();                //and renderFinal(); which will render the result into the given renderTarget.
    driver->endScene();                 //remark, that Render() will automticly set the right Rendertargets. so no need setting
    int fps = driver->getFPS();         //tehm befor calling render().

		if (lastFPS != fps)
		{
			core::stringw str = L"MotionShader Demo - TGMs Shader Pack[";
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
