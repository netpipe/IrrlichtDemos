#include "ProjectiveTextures.h" // Projector.
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
	device->setWindowCaption(L"Projective Textures [ Copyright (C) 2009 VIZ.fuerte ]");

	// Create pointers.
	pVideo     = device->getVideoDriver();
	pScene     = device->getSceneManager();
	pGUI       = device->getGUIEnvironment();
	pGPU       = pVideo->getGPUProgrammingServices();
	pCollision = pScene->getSceneCollisionManager();
	pLog       = device->getLogger();

	pScene->getParameters()->setAttribute(B3D_LOADER_IGNORE_MIPMAP_FLAG,true);

	// Loads the font that will be used.
	IGUIFont* font01 = pGUI->getFont("./media/courier.xml");

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

// =================================================================================
// PROJECTORS
// =================================================================================

	// Create the projector "1".
	CProjectiveTextures* projector1 = new CProjectiveTextures(0,pScene,-1,
															  vector3df(-11.0f,45.0f,-32.0f),
															  vector3df(-11.0f,0.0f,-32.0f));
	projector1->texture = pVideo->getTexture("./media/Batman.tga");// texture !!
	projector1->setFOV(70.0f);

	// Create the projector "2".
	CProjectiveTextures* projector2 = new CProjectiveTextures(0,pScene,-1,
															  vector3df(11.0f,45.0f,-32.0f),
															  vector3df(11.0f,0.0f,-32.0f));
	projector2->texture = pVideo->getTexture("./media/grate_project.tga");
	projector2->setFOV(70.0f);

	// Create the projector "3".
	CProjectiveTextures* projector3 = new CProjectiveTextures(0,pScene,-1,
															  vector3df(-11.0f,45.0f,32.0f),
															  vector3df(-11.0f,0.0f,32.0f));
	projector3->texture = pVideo->getTexture("./media/grate_project.tga");
	projector3->setFOV(70.0f);

	// Create the projector "4".
	CProjectiveTextures* projector4 = new CProjectiveTextures(0,pScene,-1,
															  vector3df(11.0f,45.0f,32.0f),
															  vector3df(11.0f,0.0f,32.0f));
	projector4->texture = pVideo->getTexture("./media/grate_project.tga");
	projector4->setFOV(70.0f);

// =================================================================================

	CGrid* grid = new CGrid(0,pScene,-1,2,128,SColor(255,0,255,0),4,SColor(255,0,64,0),true);

	// Creates a light.
	ILightSceneNode* light = pScene->addLightSceneNode();
	light->setPosition(vector3df(0,128.0f,128.0f));

	// Create a billboard for use as a sun.
	IBillboardSceneNode* sun = pScene->addBillboardSceneNode(light);
	sun->setMaterialTexture(0,pVideo->getTexture("./media/sol.tga"));
	sun->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	sun->setMaterialFlag(EMF_BILINEAR_FILTER,false);
	sun->setMaterialFlag(EMF_TRILINEAR_FILTER,true);
	sun->setMaterialFlag(EMF_LIGHTING,false);
	sun->setSize(dimension2d<f32>(100.0f,100.0f));

	// Load the scene.
	IMeshSceneNode* map = pScene->addMeshSceneNode(pScene->getMesh("./media/scene.b3d"));
	projector1->nodeArray.push_back(map);
	projector2->nodeArray.push_back(map);
	projector3->nodeArray.push_back(map);
	projector4->nodeArray.push_back(map);
	map->getMaterial(2).Lighting = false;

	IMeshSceneNode* glow = pScene->addMeshSceneNode(pScene->getMesh("./media/glow.b3d"));
	glow->setMaterialFlag(EMF_LIGHTING,false);
	glow->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);

	// Load the audi.
	IMeshSceneNode* audi = pScene->addMeshSceneNode(pScene->getMesh("./media/audi.b3d"));
	projector1->nodeArray.push_back(audi);
	projector2->nodeArray.push_back(audi);
	projector4->nodeArray.push_back(audi);

	ISceneNodeAnimator* anim = pScene->createFlyStraightAnimator(vector3df(20.0f,0,45.0f),
																 vector3df(20.0f,0,-128.0f),
																 3000,true,true);
	audi->addAnimator(anim);
	anim->drop();

	// Load the jeep.
	IMeshSceneNode* jeep = pScene->addMeshSceneNode(pScene->getMesh("./media/jeep.b3d"));
	projector1->nodeArray.push_back(jeep);
	projector3->nodeArray.push_back(jeep);

	anim = pScene->createFlyStraightAnimator(vector3df(-20.0f,0,25.0f),
											 vector3df(-20.0f,0,-128.0f),
											 4000,true,true);
	jeep->addAnimator(anim);
	anim->drop();

	// The projector (mesh).
	IMeshSceneNode* projector = pScene->addMeshSceneNode(pScene->getMesh("./media/cam.b3d"),0,-1,
														 vector3df(-11.0f,45.0f,-32.0f));

	// Now to draw everything ;-)
	bool info = false;
	while(device->run())
	{
		if(device->isWindowActive())
		{
			eventReceiver.endEventProcess();
			pVideo->beginScene(true,true,SColor(0,128,128,128));

			// Render the graphic effect.
			grid->render();
			pScene->drawAll();

			// Update the projector.
			projector1->render();
			projector2->render();
			projector3->render();
			projector4->render();

			// Turn the projector mesh.
			vector3df toTarget = projector1->getTarget() - vector3df(-11.0f,45.0f,-32.0f);
			projector->setRotation(toTarget.getHorizontalAngle());

			// Control of the projector (only the 1)
			if(eventReceiver.keyDown(KEY_KEY_1))
				projector1->setTarget(projector1->getTarget() + vector3df(0,0,0.1f));

			if(eventReceiver.keyDown(KEY_KEY_2))
				projector1->setTarget(projector1->getTarget() - vector3df(0,0,0.1f));

			if(eventReceiver.keyDown(KEY_KEY_3))
				projector1->setTarget(projector1->getTarget() + vector3df(0.1f,0,0));

			if(eventReceiver.keyDown(KEY_KEY_4))
				projector1->setTarget(projector1->getTarget() - vector3df(0.1f,0,0));

			if(eventReceiver.keyDown(KEY_KEY_5))
				projector1->setFOV(projector1->getFOV() + 0.001f);

			if(eventReceiver.keyDown(KEY_KEY_6))
				projector1->setFOV(projector1->getFOV() - 0.001f);

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
				text2 += L"Target X: ";
				text2 += projector1->getTarget().X;
				text2 += L"\n";
				text2 += L"Target Y: ";
				text2 += projector1->getTarget().Y;
				text2 += L"\n";
				text2 += L"Target Z: ";
				text2 += projector1->getTarget().Z;
				text2 += L"\n";
				text2 += L"FOV: ";
				text2 += projector1->getFOV();
				text2 += L"\n";
				font01->draw(text2.c_str(),rect<s32>(25,75,25,75),SColor(255,255,255,255));

				font01->draw(L"Projector 1",rect<s32>(60,250,60,250),SColor(255,0,64,64),true,true);
				pVideo->draw2DImage(projector1->texture,rect<s32>(0+25,0+265,128+25,128+265),rect<s32>(0,0,256,256));
				pVideo->draw2DRectangleOutline(recti(25,265,153,393));

				font01->draw(L"Projector 2,3 and 4",rect<s32>(260,250,260,250),SColor(255,0,64,64),true,true);
				pVideo->draw2DImage(projector2->texture,rect<s32>(0+200,0+265,128+200,128+265),rect<s32>(0,0,256,256));
				pVideo->draw2DRectangleOutline(recti(200,265,328,393));
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
