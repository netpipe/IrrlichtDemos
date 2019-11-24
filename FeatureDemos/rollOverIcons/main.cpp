#include <irrlicht.h>
#include "listFileSwitch.h"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#include <stdio.h>
#include <iostream>

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}
	   //pf is CNrpPictureFlow
 //  IGUIElement* elm = CNrpEngine::Instance().GetGuiEnvironment()->getFocus();
  //       if( elm == pf )
   //        elm->OnEvent( event );

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};



char *p;

int main()
{
    MyEventReceiver receiver;

	IrrlichtDevice *device =
		createDevice( video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
			false, false, false, &receiver);

	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);

	//IAnimatedMesh* mesh = smgr->getMesh("untitled.x");
	//if (!mesh)
	//{
	//	device->drop();
	//	return 1;
	//}
	//IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );


//	if (node)
//	{
	  //  node->setFrameLoop (0, 120);

//	    node->setAnimationSpeed(1333);
//	    node->setLoopMode(true);
	//	node->setMaterialFlag(EMF_LIGHTING, false);
	//	node->setMD2Animation("ObAction");
	//	node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
//	}

	smgr->addCameraSceneNode(0, vector3df(0,10,-10), vector3df(0,5,0));

   CNrp2DPictureFlow *pf = new CNrp2DPictureFlow( device , guienv,
                                                 guienv->getRootGUIElement(), core::recti( 0, 0, 400, 400 ), core::recti( 0, 0, 100, 100 ),-1 );


	int ammount =7;

stringw stexture;


// manual picture add from directory
//   for( int k=0; k < ammount; k++ ){
//		//   pf->addItem( driver->getTexture( "stones.jpg" ), L"" );
//		   stexture = "picture";
//		stexture += k;
//		stexture += ".jpg";
//            pf->addItem( driver->getTexture( stexture ), L"" );
//   }

int delay = 0;
int direction =0;
int icount=0;


 IFileSystem *fs = device->getFileSystem();
//	CurrentArchiveList.clear();
//	IFileSystem *fs = Device->getFileSystem();
//	for ( i = 0; i != fs->getFileArchiveCount(); ++i )
//	{
//		CurrentArchiveList.push_back ( fs->getFileArchive(i)->getFileList()->getPath() );
//	}

//	io::path* testpath;
	//io::IFileArchive* archive;
	//device->getFileSystem()->addFileArchive( "./pics.zip");
	//device->getFileSystem()->getWorkingDirectory();
	//		io::IFileSystem* archive ;
			//io::IFileArchive* archive ;
//			archive->addFolderFileArchive("media");

//                std::string fName = app->activity->internalDataPath + files->getFullFileName(x).c_str();
//                // If the file is not a directory and it doesn't already exist (you can use stat() or directly try fread() to check if the file exists)
//                if (!files->isDirectory(x) && !Operations::fileExists(fName))
//

    if (! fs->addFileArchive("pictures.zip",
                             false,     // case-insensitive
                             false)) { // require full path to get file
    //    fatalError("ResourceManager: Failed to open resource package \\"%s\"", p);
	printf("testing1");
    }

    const IFileList *l = fs->getFileArchive(fs->getFileArchiveCount()-1)->getFileList();
    for (u32 i=0; i<l->getFileCount(); i++) {
		pf->addItem( driver->getTexture( l->getFileName(i).c_str() ), L"" );
	//std::cout << "testing2";
    }
//    // make it so that files added last are searched first
//    u32 count = fs->getFileArchiveCount();
//    if (count > 1) {
//        fs->moveFileArchive(count-1, -((s32) count - 1));
//    }

//            IFileList* files = (IFileList*)archive->getFileList();
//            for (int x = 0; x < files->getFileCount(); x++)
//            {
//			std::cout << files->getFileName(x).c_str();
//			}

	while(device->run())
	{
		if (delay > 140){
			delay=0;
			if (pf->getcurrent() < ammount-1 & direction == 0){
					pf->Next(1);
					} else {pf->Next(-1); direction == 1;}
		} else {delay++;}

		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
		device->sleep(10);
	}


	device->drop();

	return 0;
}

