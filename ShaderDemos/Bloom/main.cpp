#include "Bloom.h" // Bloom effect.
#include "EventReceiver.h" // Keyboard Controls, mouse..
#include "Grid.h" // Grid.

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
	device->setWindowCaption(L"Bloom [ Copyright (C) 2009 VIZ.fuerte ]");

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
	CBloom* effect = new CBloom();

	CGrid* grid = new CGrid(0,pScene,-1,2,128,SColor(255,0,255,0),4,SColor(255,0,64,0),true);

	// Creates a light.
	ILightSceneNode* light = pScene->addLightSceneNode();
	light->setPosition(vector3df(0,128.0f,128.0f));

	// Create a billboard for use as a sun.
	IBillboardSceneNode* sun = pScene->addBillboardSceneNode(light);
	sun->setMaterialTexture(0,pVideo->getTexture("media/sol.dds"));
	sun->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	sun->setMaterialFlag(EMF_BILINEAR_FILTER,false);
	sun->setMaterialFlag(EMF_TRILINEAR_FILTER,true);
	sun->setMaterialFlag(EMF_LIGHTING,false);
	sun->setSize(dimension2d<f32>(100.0f,100.0f));

	// Load the effect of illumination (per-pixel lighting).
	s32 ppl = pGPU->addShaderMaterialFromFiles("shader/light.vs","shader/light.fs");

	// Load the scene.
	IMeshSceneNode* map = pScene->addMeshSceneNode(pScene->getMesh("media/scene.b3d"));
	map->setMaterialType((E_MATERIAL_TYPE)ppl);

	// Load the pirate xD.
	IMeshSceneNode* pirate = pScene->addMeshSceneNode(pScene->getMesh("media/pirate.b3d"));
	pirate->getMaterial(0).Shininess = 10.0f;
	pirate->setMaterialFlag(video::EMF_GOURAUD_SHADING,false);
	pirate->setPosition(vector3df(0.0f,2.0f,0.0f));
	pirate->setMaterialType((E_MATERIAL_TYPE)ppl);

	// Load the duck.
	IMeshSceneNode* duck = pScene->addMeshSceneNode(pScene->getMesh("media/duck.b3d"));
	duck->getMaterial(0).Shininess = 10.0f;
	duck->setPosition(vector3df(18.0f,2.0f,-18.0f));
	duck->setScale(vector3df(10.0f));
	duck->setMaterialType((E_MATERIAL_TYPE)ppl);

	// Loads the banana.
	IMeshSceneNode* banana = pScene->addMeshSceneNode(pScene->getMesh("media/banana.b3d"));
	banana->getMaterial(0).Shininess = 50.0f;
	banana->setPosition(vector3df(-28.0f,2.0f,28.0f));
	banana->setMaterialType((E_MATERIAL_TYPE)ppl);

	// Loading torture.
	IAnimatedMeshSceneNode* turtle = pScene->addAnimatedMeshSceneNode(pScene->getMesh("media/turtle.b3d"));
	turtle->getMaterial(0).Shininess = 30.0f;
	turtle->setScale(vector3df(0.45f));
	turtle->setPosition(vector3df(16.0f,2.0f,16.0f));
	turtle->setRotation(vector3df(0.0f,45.0f,0.0f));
	turtle->setMaterialType((E_MATERIAL_TYPE)ppl);

	// Now to draw everything ;-)
	bool info = false;
	while(device->run())
	{
		if(device->isWindowActive())
		{
			eventReceiver.endEventProcess();
			pVideo->beginScene(true,true,SColor(0,128,128,128));

			// Brightness control.
			if(eventReceiver.keyDown(KEY_KEY_1) && effect->brightnessValue < 0.5f)
				effect->brightnessValue += 0.001f;

			if(eventReceiver.keyDown(KEY_KEY_2) && effect->brightnessValue > 0.0f)
				effect->brightnessValue -= 0.001f;

			// Blurring control.
			if(eventReceiver.keyDown(KEY_KEY_3) && effect->blurValue < 0.5f)
				effect->blurValue += 0.0001f;

			if(eventReceiver.keyDown(KEY_KEY_4) && effect->blurValue > 0.0f)
				effect->blurValue -= 0.0001f;

			// Multiplier control.
			if(eventReceiver.keyDown(KEY_KEY_5) && effect->multiValue < 1.0f)
				effect->multiValue += 0.001f;

			if(eventReceiver.keyDown(KEY_KEY_6) && effect->multiValue > 0.0f)
				effect->multiValue -= 0.001f;

			// Render the graphic effect.
			effect->render(grid);

			// Draw two black rectangles on the screen.
			pVideo->draw2DRectangle(SColor(196,0,0,0),rect<s32>(0,0,640,60));
			pVideo->draw2DRectangle(SColor(196,0,0,0),rect<s32>(0,420,640,480));

			// Show info !!!
			// Draw the text (above shows the frames per second, triangles,...).
			if(eventReceiver.keyPressed(KEY_KEY_I))
				info = info ? false : true;

			if(info)
			{
				stringw text2 = L"FPS: ";
				text2 += pVideo->getFPS();
				text2 += L"\n";
				text2 += L"Triangles: ";
				text2 += pVideo->getPrimitiveCountDrawn();
				text2 += L"\n\n";
				text2 += L"Brightness: ";
				text2 += effect->brightnessValue;
				text2 += L"\n";
				text2 += L"Blur: ";
				text2 += effect->blurValue;
				text2 += L"\n";
				text2 += L"Multiplier: ";
				text2 += effect->multiValue;

				font01->draw(text2.c_str(),rect<s32>(25,75,25,75),SColor(255,255,255,255));

				font01->draw(L"Scene",rect<s32>(45,250,45,250),SColor(255,0,64,64),true,true);
				pVideo->draw2DImage(effect->tex0,rect<s32>(0+25,0+265,128+25,128+265),rect<s32>(0,0,1024,1024));
				pVideo->draw2DRectangleOutline(recti(25,265,153,393));

				font01->draw(L"Filter",rect<s32>(216,250,216,250),SColor(255,0,64,64),true,true);
				pVideo->draw2DImage(effect->tex1,rect<s32>(0+200,0+265,128+200,128+265),rect<s32>(0,0,512,512));
				pVideo->draw2DRectangleOutline(recti(200,265,328,393));

				font01->draw(L"Blur",rect<s32>(388,250,388,250),SColor(255,0,64,64),true,true);
				pVideo->draw2DImage(effect->tex3,rect<s32>(0+375,0+265,128+375,128+265),rect<s32>(0,0,256,256));
				pVideo->draw2DRectangleOutline(recti(375,265,503,393));
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