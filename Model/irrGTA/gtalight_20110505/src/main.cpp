// Copyright (C) 2007-2011 David Kment
// For license see license.txt

#include "main.h"

#include <irrlicht.h>
//#include <windows.h>

#include "CGTASceneNode.h"
#include "CDFFMeshFileLoader.h"
#include "CIMGArchiveLoader.h"
#include "CTXDArchiveLoader.h"
#include "CImageLoaderGTATexture.h"
#include "CEventReceiver.h"
#include "CRenderManager.h"

using namespace std;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char *argv[])
{
    int gameId = 0;

    path userWorkingDirectory = "";

    int i;
    for(i=1; i < argc; i++)
    {
        if(i==1)
        {
            userWorkingDirectory = argv[i];

            while(userWorkingDirectory.find("\"") != -1)
            {
                userWorkingDirectory.erase(userWorkingDirectory.find("\""));
            }
        }
    }

    CEventReceiver *receiver = new CEventReceiver();

	SIrrlichtCreationParameters creationParams;
	creationParams.Fullscreen = false;
	creationParams.WindowSize = core::dimension2d<u32>(1280, 720);
	creationParams.Stencilbuffer = false;
	creationParams.AntiAlias = 4;
	creationParams.DriverType = video::EDT_OPENGL;
	creationParams.EventReceiver = receiver;

    IrrlichtDevice *device = createDeviceEx(creationParams);

    device->setWindowCaption(L"GTALight");
    device->getCursorControl()->setVisible(false);

    IVideoDriver *driver = device->getVideoDriver();
    ISceneManager *smgr = device->getSceneManager();
    IGUIEnvironment *guienv = device->getGUIEnvironment();
    IFileSystem *fs = device->getFileSystem();

    CIMGArchiveLoader *imgloader = new CIMGArchiveLoader(fs);
    fs->addArchiveLoader(imgloader);

    CTXDArchiveLoader *txdloader = new CTXDArchiveLoader(fs);
    fs->addArchiveLoader(txdloader);

    CDFFMeshFileLoader dffloader = CDFFMeshFileLoader(device);
    smgr->addExternalMeshLoader(&dffloader);

    CImageLoaderGTATexture gtatextureloader = CImageLoaderGTATexture(device);
    driver->addExternalImageLoader(&gtatextureloader);

    // smgr->getParameters()->setAttribute("GTA_LOD_MESHES_ONLY", lodMode);
    smgr->getParameters()->setAttribute("GTA_TEXTURES_PATH", "");
    smgr->getParameters()->setAttribute("GTA_MODELS_PATH", "");
    smgr->getParameters()->setAttribute("GTA_TEXTURES_EXTENSION", "gtatexture");

    if(userWorkingDirectory.size() > 0)
    {
        if(device->getFileSystem()->changeWorkingDirectoryTo(userWorkingDirectory))
        {
            printf("changing to user data directory: %s\n", userWorkingDirectory.c_str());
        }
        else
        {
            printf("error, invalid user data directory supplied: %s\n", userWorkingDirectory.c_str());
			return -1;
        }
    }

    // detect game ID
    if(fs->existFile("gta3.exe"))
    {
        gameId = GAMEID_GTA3;
    }
    else if(fs->existFile("gta-vc.exe"))
    {
        gameId = GAMEID_GTAVC;
    }
    else if(fs->existFile("gta_sa.exe"))
    {
        gameId = GAMEID_GTASA;
    }

    if(!fs->addFileArchive("models/gta3.img", true, false))
    {
        printf("error, cannot register archive models gta3.img\n");
		return -1;
    }
 gameId = GAMEID_GTAVC;
	// add camera
    ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(NULL, 50.0f, 0.2f, -1, 0, 0, false, 0.0f);
	// camera->setPosition(core::vector3df(0.0f, 150.0f, 0.0f));
	camera->updateAbsolutePosition();

	// add GTA scene node
    CGTASceneNode *gtanode = new CGTASceneNode(device, gameId, NULL, smgr, -1, core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(0.0f, 0.0f, 0.0f), core::vector3df(1.0f, 1.0f, 1.0f));
    gtanode->initialise();

    printf("Worldmap initialised\n");

	// add camera
    ICameraSceneNode* camera2 = smgr->addCameraSceneNodeFPS(NULL, 50.0f, 0.2f, -1, 0, 0, false, 0.0f);

    CRenderManager* manager;

    if(DYNAMIC_LOD_MODE)
    {
        manager = new CRenderManager(gtanode);
    }

	IGUIStaticText *locationInfo = guienv->addStaticText(L"", rect<s32>(10,10,260,42), true);

    int fps = 0;
    int trisdrawn;

    stringw locationText = "";

	camera->setFarValue(50000);
	camera2->setFarValue(50000);

    while(device->run())
    {
        if (device->isWindowActive())
        {
			locationText = "Position: ";
			locationText += camera2->getAbsolutePosition().X;
			locationText += ", ";
			locationText += camera2->getAbsolutePosition().Y;
			locationText += ", ";
			locationText += camera2->getAbsolutePosition().Z;
			locationText += "\n";
			locationText += "Rotation: ";
			locationText += camera2->getRotation().X;
			locationText += ", ";
			locationText += camera2->getRotation().Y;
			locationText += ", ";
			locationText += camera2->getRotation().Z;

			locationInfo->setText(locationText.c_str());

            fps = driver->getFPS();
            trisdrawn = driver->getPrimitiveCountDrawn();

            driver->beginScene(true, true, SColor(255,100,101,140));

            smgr->drawAll();
            guienv->drawAll();

            driver->endScene();

            if(DYNAMIC_LOD_MODE)
            {
                manager->update(camera2->getAbsolutePosition());
            }
        }
    }

    device->drop();

    return 0;
}
