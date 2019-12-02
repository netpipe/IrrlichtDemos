#include "stdafx.h"
int main(){
	fstream loadFile;
	loadFile.open("idFile.txt");
	if(loadFile.is_open())
	{
		loadFile>>versionId;
	}else
	{
		runloop=false;
	}
	loadFile.close();
	envId=100;
	returnEnvId=101;
	width=300;
	height=200;
	//starts the device, direct x9, attaches receiver
	device = createDevice(video::EDT_DIRECT3D9, core::dimension2d<s32>(width, height), 32, false, false, false, &receiver);
	//gets the video driver, graphics card
	driver = device->getVideoDriver();
	//gets scene manager, lets you do things with scene nodes
	smgr = device->getSceneManager();
	//gets timer, lets us move uniform speed despite fps
	timer = device->getTimer();
	//gets gui environment, lets use make a gui
	env = device->getGUIEnvironment();
	gui::IGUIFont* font = env->getFont("media/cityblueprint.bmp");

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNode();
	camera->setNearValue(0.1f);
	//camera->setFarValue(400);
	camera->setPosition(core::vector3df(1000*.5/2, 0, 1000*.5/2));


	// fog
	//driver->setFog(video::SColor(255,100,101,140), true, .5*200/4, .5*(200-4)/2, 0.05f);

	// movement speed per second
    f32 speed = 5.0f;
	wchar_t tmp[1024];
	// time
	f32 time = 0;
	f32 oldtime;
	// time between frames
	f32 deltaT;
	core::position2d<s32> pressedPosition, curpos, mousepositionold, mousepositionnew, mouseResetPos, mouseOldHighlighter, mouseNewHighlighter;
	ICursorControl* cursorcon=device->getCursorControl();
	f32 camZoom=5;
	int pressedId=-1;
	double camYPos=3.5, updateRot=0, updateDelay=0, forceUpdate=5, globalq=5;
	bool sentupdate[6]={false, false, false, false, false, false}, displayInfo=false, guiBuilt=false;
	bool pressedChangeable=false, pressedSwap=false;
	while(device->run() && driver && runloop)
		if (device->isWindowActive())//) 
		{
			// calculate time between frames
			oldtime = time;
			time = (f32)timer->getTime() / 1000.0f;
			deltaT = (time - oldtime);
			if(buildNewEnv)
			{
				buildNewEnv=false;
				env->clear();
				switch(envId)
				{
					//********************
					//patcher environments
					//********************
				case 100:
					keyControl=0;
					//connecting to server
					env->addImage(driver->getTexture("images/updaterconnecting.png"),position2d<int>(0,0), false, 0, -1);
					//quit button
					env->addButton(rect<s32>(230,160,290,175),0,2,L"QUIT");
					driver->beginScene(true, true, SColor(0,122,65,171));
					env->drawAll();
					driver->endScene();
					connectToServer();
					break;
				case 101:
					keyControl=0;
					//connection failed
					env->addImage(driver->getTexture("images/updaterconnectfailed.png"),position2d<int>(0,0), false, 0, -1);
					//quit button
					env->addButton(rect<s32>(230,160,290,175),0,2,L"QUIT");
					break;
				case 102:
					keyControl=0;
					//connected, patching
					env->addImage(driver->getTexture("images/updaterbg.png"),position2d<int>(0,0), false, 0, -1);
					updatePatchInfo=true;
					//quit button
					env->addButton(rect<s32>(230,160,290,175),0,2,L"QUIT");
					break;
				case 103:
					keyControl=0;
					//patched add login button
					env->addImage(driver->getTexture("images/updaterbg.png"),position2d<int>(0,0), false, 0, -1);
					//quit button
					env->addButton(rect<s32>(230,160,290,175),0,2,L"QUIT");
					//play button
					env->addButton(rect<s32>(230,180,290,195),0,6,L"PLAY");
					break;
				case 104:
					keyControl=0;
					//patched add login button
					env->addImage(driver->getTexture("images/errorsaving.png"),position2d<int>(0,0), false, 0, -1);
					//quit button
					env->addButton(rect<s32>(230,160,290,175),0,2,L"QUIT");
					break;
				}
			}
			
			if(pauseScreen&&!buildNewEnv)
			{
				pauseScreen=false;
				u32 startTime=timer->getRealTime();
				while(startTime+pauseLength>timer->getRealTime()&&!buildNewEnv)
				{
					driver->beginScene(true, true, SColor(0,122,65,171));
					env->drawAll();
					driver->endScene();
				}
				envId=returnEnvId;
				buildNewEnv=true;
			}
			if(guiBuilt)
			{
				if(env->hasFocus(chatEdit)||env->hasFocus(blueEditBox)||env->hasFocus(greenEditBox)||env->hasFocus(redEditBox)||env->hasFocus(sphereRadiusEditBox)||env->hasFocus(heightRateEditBox))
				{
					keyControl=3;
					getevents=4;
				}else if(keyControl==3)
				{
					keyControl=2;
					getevents=3;
				}
			}
			driver->beginScene(true, true, SColor(255,100,101,140));
			s32 fps = driver->getFPS();
			if(drawScene)
			{
				smgr->drawAll();
			}
			env->drawAll();
			
			driver->endScene();
		}

	device->drop();
	return 0;
}