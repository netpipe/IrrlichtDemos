#include "ShadowMap.h" // Shadow map effect.
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
	device->setWindowCaption(L"Depth Shadow Mapping [ Copyright (C) 2009 VIZ.fuerte ]");

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

	// Creates the shadow map.
	CShadowMap* effect = new CShadowMap(dimension2du(1024,1024));

	CGrid* grid = new CGrid(0,pScene,-1,2,128,SColor(255,0,255,0),4,SColor(255,0,64,0),true);

	// Creates a light.
	ILightSceneNode* light = pScene->addLightSceneNode();
	light->setPosition(vector3df(0,128.0f,128.0f));

	// Add light to effect. For now, it only supports a light
	// (remember that this is only an example ^^).
	effect->globalLight = light;

	// Create an animator, so that the light of turns in circles.
	ISceneNodeAnimator* anim = pScene->createFlyCircleAnimator(vector3df(0,128.0f,0),128.0f);
	light->addAnimator(anim);
	anim->drop();

	// Create a billboard for use as a sun.
	IBillboardSceneNode* sun = pScene->addBillboardSceneNode(light);
	sun->setMaterialTexture(0,pVideo->getTexture("media/sol.dds"));
	sun->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	sun->setMaterialFlag(EMF_LIGHTING,false);
	sun->setSize(dimension2d<f32>(100.0f,100.0f));

	// Load the effect of illumination (per-pixel lighting).
	s32 ppl = pGPU->addShaderMaterialFromFiles("shader/light.vs","shader/light.fs");

	// Load the scene.
	IAnimatedMeshSceneNode* map = pScene->addAnimatedMeshSceneNode(pScene->getMesh("media/scene.b3d"));
	map->getMesh()->setHardwareMappingHint(EHM_STATIC);
	map->setMaterialType((E_MATERIAL_TYPE)ppl);// Per-pixel light

	// Add mesh to shadow map, cast and receive shadows.
	effect->meshArray.push_back(map);

	// The Ninja !!!.
	IAnimatedMeshSceneNode* ninja = pScene->addAnimatedMeshSceneNode(pScene->getMesh("media/ninja.b3d"));
	ninja->setScale(vector3df(2.5f));
	ninja->setPosition(vector3df(8.0f,3.0f,17.0f));
	ninja->setMaterialType((E_MATERIAL_TYPE)ppl);// Per-pixel light

	// Add mesh to shadow map, cast and receive shadows.
	effect->meshArray.push_back(ninja);

	// Now to draw everything ;-)
	bool info = false;
	while(device->run())
	{
		if(device->isWindowActive())
		{
			eventReceiver.endEventProcess();
			pVideo->beginScene(true,true,SColor(0,128,128,128));

			// FOV control (changing the value of this, you can gain more quality of the shadow)
			if(eventReceiver.keyDown(KEY_KEY_1))
				effect->fov += 0.1f * DEGTORAD;

			if(eventReceiver.keyDown(KEY_KEY_2))
				effect->fov -= 0.1f * DEGTORAD;

			// Render the graphic effect.
			effect->update();
			grid->render();

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
				text2 += L"Far (light radius): ";
				text2 += effect->farPlane;
				text2 += L"\n";
				text2 += L"FOV: ";
				text2 += effect->fov;

				font01->draw(text2.c_str(),rect<s32>(25,75,25,75),SColor(255,255,255,255));

				font01->draw(L"Shadow Map",rect<s32>(65,250,65,250),SColor(255,0,64,64),true,true);
				pVideo->draw2DImage(effect->shadowTex,rect<s32>(0+25,0+265,128+25,128+265),rect<s32>(0,0,1024,1024));
				pVideo->draw2DRectangleOutline(recti(25,265,153,393));
			}

			pVideo->endScene();
			eventReceiver.startEventProcess();
		}
	}

	// Remove all buffers, textures...
	pVideo->removeAllHardwareBuffers();
	pVideo->removeAllTextures();

	device->drop();

	return 0;
}

// Compile and test ;-)