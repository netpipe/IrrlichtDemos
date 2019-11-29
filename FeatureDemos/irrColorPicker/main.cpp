#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

//http://irrlicht.sourceforge.net/forum/viewtopic.php?f=9&t=50473&hilit=colorpicker
#include <irrlicht.h>

#include "exampleHelper.h"

using namespace irr;

/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look at them,
*/
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#include "IGUIColorPicker.h"

/*
This is the main method. We can now use main() on every platform.
*/
int main()
{

	#ifdef __EMSCRIPTEN__
			IrrlichtDevice *device =
		createDevice( video::EDT_OGLES2, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;
	#else
	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;
	#endif
	/*
	Set the caption of the window to some nice text. Note that there is an
	'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	/*
	Get a pointer to the VideoDriver, the SceneManager and the graphical
	user interface environment, so that we do not always have to write
	device->getVideoDriver(), device->getSceneManager(), or
	device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	/*
	We add a hello world label to the window, using the GUI environment.
	The text is placed at the position (10,10) as top left corner and
	(260,22) as lower right corner.
	*/
	guienv->addStaticText(L"Hello World! This is Irrlicht with the burnings software renderer!",
		rect<s32>(10,10,260,22), true);

	const io::path mediaPath = getExampleMediaPath();

	IAnimatedMesh* mesh = smgr->getMesh( "sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture( "sydney.bmp") );
	}

	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));



	IGUIColorPicker *iguicolorpicker = new IGUIColorPicker(guienv, 0, -1,  true);

	iguicolorpicker->setEnabled(1);

	#ifdef __EMSCRIPTEN__
 		emscripten_set_main_loop(main_loop,0,1);
 	#else

	while(device->run())
	{

		driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(255,100,101,140));


		smgr->drawAll();
		guienv->drawAll();
iguicolorpicker->draw();
		driver->endScene();
	}
	#endif
	device->drop();

	return 0;
}

