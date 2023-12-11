#include "PointLights.h" // Bloom effect.
#include "EventReceiver.h" // Keyboard Controls, mouse..

// Pointers.
IVideoDriver		    * pVideo;
ISceneManager		    * pScene;
IrrlichtDevice		    * device;
IFileSystem			    * pFile;
IGUIEnvironment		    * pGUI;
IGPUProgrammingServices * pGPU;
ILogger				    * pLog;
ISceneCollisionManager	* pCollision;

// Main.
int main(int argc,char* argv[])
{
	EventReceiver eventReceiver;
	eventReceiver.init();

	// Graphical configuration.
	SIrrlichtCreationParameters params;
	params.DriverType    = EDT_OPENGL;
	params.WindowSize    = dimension2d<u32>(640,480);
	params.Bits          = 32;
	params.EventReceiver = &eventReceiver;
	device = createDeviceEx(params);

	// Title of the window.
	device->setWindowCaption(L"Per-Pixel Point lights [ Copyright (C) 2009 VIZ.fuerte ]");

	// Create pointers.
	pVideo     = device->getVideoDriver();
	pScene     = device->getSceneManager();
	pGUI       = device->getGUIEnvironment();
	pGPU       = pVideo->getGPUProgrammingServices();
	pCollision = pScene->getSceneCollisionManager();
	pLog       = device->getLogger();

	pScene->getParameters()->setAttribute(B3D_LOADER_IGNORE_MIPMAP_FLAG,true);

	// Loads the font that will be used.
	IGUIFont* font01 = pGUI->getFont("media/courier.xml");

	// Creates a camera of type "FPS".
	SKeyMap keyMap[8];
	keyMap[0].Action  = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_UP;
	keyMap[1].Action  = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[2].Action  = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_DOWN;
	keyMap[3].Action  = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;

	keyMap[4].Action  = EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = KEY_LEFT;
	keyMap[5].Action  = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;

	keyMap[6].Action  = EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = KEY_RIGHT;
	keyMap[7].Action  = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;

	ICameraSceneNode* camera = pScene->addCameraSceneNodeFPS(0,100,0.5,-1,keyMap,8);
	camera->setPosition(vector3df(0.0f,16.0f,-32.0f));

	// Creates the effect "Bloom".
	CPointLights* effect = new CPointLights();

	pScene->setAmbientLight(SColorf(0.0f,0.0f,0.0f,0.0f));

	effect->addLight(SColor(255,0,255,0),vector3df(0.0f,32.0f,96.0f),40);
	effect->addLight(SColor(255,255,255,0),vector3df(-96.0f,32.0f,0.0f),40);
	effect->addLight(SColor(255,0,255,255),vector3df(96.0f,32.0f,0.0f),40);
	effect->addLight(SColor(255,255,0,255),vector3df(0.0f,32.0f,-96.0f),40);

	effect->addLight(SColor(255,255,255,255),vector3df(-32.0f,16.0f,-32.0f),35);
	effect->addLight(SColor(255,255,255,255),vector3df(-32.0f,16.0f,32.0f),35);
	effect->addLight(SColor(255,255,255,255),vector3df(32.0f,16.0f,32.0f),35);
	effect->addLight(SColor(255,255,255,255),vector3df(32.0f,16.0f,-32.0f),35);

	effect->addLight(SColor(255,255,255,255),vector3df(-32.0f,48.0f,-32.0f),35);
	effect->addLight(SColor(255,255,255,255),vector3df(-32.0f,48.0f,32.0f),35);
	effect->addLight(SColor(255,255,255,255),vector3df(32.0f,48.0f,32.0f),35);
	effect->addLight(SColor(255,255,255,255),vector3df(32.0f,48.0f,-32.0f),35);

	// Load the scene.
	IMesh* mesh = pScene->getMesh("media/scene.b3d");
	((ISkinnedMesh*)mesh)->convertMeshToTangents();

	IMeshSceneNode* map = pScene->addMeshSceneNode(mesh);
	map->setMaterialType(EMT_SOLID);
	map->setMaterialFlag(EMF_LIGHTING,true);
	effect->meshArray.push_back(map);

	// Now to draw everything ;-)
	bool info = false;
	bool wire = false;
	while(device->run())
	{
		if(device->isWindowActive())
		{
			eventReceiver.endEventProcess();
			pVideo->beginScene();

			// Render the graphic effect.
			pScene->drawAll();
			effect->render();

			// Draw two black rectangles on the screen.
			pVideo->draw2DRectangle(SColor(196,0,0,0),rect<s32>(0,0,640,60));
			pVideo->draw2DRectangle(SColor(196,0,0,0),rect<s32>(0,420,640,480));

			// Show info !!!
			// Draw the text (above shows the frames per second, triangles,...).
			if(eventReceiver.keyPressed(KEY_KEY_I))
				info = info ? false : true;

			if(eventReceiver.keyPressed(KEY_KEY_1))
				wire = wire ? false : true;

			map->setMaterialFlag(EMF_WIREFRAME,wire);

			if(info)
			{
				stringw text2 = L"FPS: ";
				text2 += pVideo->getFPS();
				text2 += L"\n";
				text2 += L"Triangles: ";
				text2 += pVideo->getPrimitiveCountDrawn();
				text2 += L"\n\n";
				text2 += L"Light Count: ";
				text2 += effect->lightConf.size();

				font01->draw(text2.c_str(),rect<s32>(25,75,25,75),SColor(255,255,255,255));
			}

			pVideo->endScene();
			eventReceiver.startEventProcess();
		}
	}

	pVideo->removeAllHardwareBuffers();
	pVideo->removeAllTextures();

	device->drop();

	return 0;
}

// Compile and test ;-)