

//Modify Makefile: Add the 2 new files  Sources := main.cpp GUIHtm.c SocketMngr.c

#include <irrlicht.h>
///HTMGUI
#include "GUIHtm.h"
///HTMGUI

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


///HTMGUI
void ExitApp(char * Tag,char * ParamStr);
void YRotate(char * Tag,char * ParamStr);
void XRotate(char * Tag,char * ParamStr);
static char fExit =0,VRotate=0,ORotate=0 ;
#define NO_FUNCTION_BEFORE_REWRITE_WIDGET NULL
///HTMGUI

video::ITexture* rt =NULL;
IVideoDriver* driver = NULL;


int main()
{

///HTMGUI
  InitGUI(20000);
  AddHtmlPage(0,"Default_template.txt", NO_FUNCTION_BEFORE_REWRITE_WIDGET);
  AddUserButton(0,"Exit",ExitApp);
  AddUserButton(0,"XRotate",XRotate);
  AddUserButton(0,"YRotate",YRotate);
  AddImage(0,"HelloImage","Hello.bmp",NO_FUNCTION_BEFORE_REWRITE_WIDGET);
///HTMGUI

	IrrlichtDevice *device =
#ifdef _IRR_OSX_PLATFORM_
		createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16,
			false, false, false, 0);
#else
		createDevice( video::EDT_SOFTWARE, dimension2d<s32>(640, 480), 16,
			false, false, false, 0);
#endif
	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();

	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	if (!mesh)
		return 1;

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
	}

	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));


	while(device->run())
	{
		///HTMGUI
		CheckGUI();
		if(fExit){
			break;
		}
                if(VRotate){
                   VRotate =0;
                   vector3df objRot = node->getRotation();
                    objRot.Y += 20.5f;
                    node->setRotation(objRot);
                 }
                if(ORotate){
                   ORotate =0;
                   vector3df objRot = node->getRotation();
                    objRot.Z += 20.5f;
                    node->setRotation(objRot);
                 }
		///HTMGUI

		rt = driver->addRenderTargetTexture(core::dimension2d<s32>(256,256), "RTT1");
		driver->setRenderTarget(rt, true, true, video::SColor(0,0,0,255));


		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		driver->endScene();

                video::IImage* image = driver->createImageFromData (rt->getColorFormat(),rt->getSize(),rt->lock(),true);
                rt->unlock();
                driver->writeImageToFile(image,"Hello.bmp");
                image->drop();

		device->sleep(15);

	}

	device->drop();

        ///HTMGUI
        ExitGUI();
        ///HTMGUI

	return 0;
}

///HTMGUI
void YRotate(char * Tag,char * ParamStr){
    VRotate=1;
    return;
}
void XRotate(char * Tag,char * ParamStr){
    ORotate=1;
    return;
}
void ExitApp(char * Tag,char * ParamStr){
 fExit =1;
 return;
}
///HTMGUI


