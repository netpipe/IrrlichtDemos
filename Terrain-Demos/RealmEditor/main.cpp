#include "stdafx.h"
int main()
{
	//starts the device, direct x9, attaches receiver
	device = irr::createDevice(video::EDT_OPENGL, core::dimension2du(width, height), 32, false, false, false, &receiver);
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
	camera->setFarValue(400);
	camera->setPosition(core::vector3df(1000*.5/2, 0, 1000*.5/2));


	Player.id=-1;
	// fog

///	driver->setFog(video::SColor(255,100,101,140), true , .5*200/4, .5*(200-4)/2, 0.05f);

	// movement speed per second
    f32 speed = 5.0f;
	Player.rotateSpeed=2;
	wchar_t tmp[1024];
	// time
	f32 time = 0;
	f32 oldtime;
	// time between frames
	f32 deltaT;
	vector3df camRot, lastMovVec=Player.movVec;
	core::position2d<s32> pressedPosition, curpos, mousepositionold, mousepositionnew, mouseResetPos, mouseOldHighlighter, mouseNewHighlighter;
	ICursorControl* cursorcon=device->getCursorControl();
	f32 camZoom=5;
	int oldRotDir=0, rotTest=0, lastRotation=Player.rotation, pressedId=-1;
	double camYPos=3.5, updateRot=0, updateDelay=0, forceUpdate=5, globalq=5;
	bool sentupdate[6]={false, false, false, false, false, false}, displayInfo=false, guiBuilt=false;
	bool pressedChangeable=false, pressedSwap=false;

	//vars that need to be set
	Player.speed=4;
	Player.rotateSpeed=1;
	Player.movVec=vector3df(0,0,0);
	Player.posx=0;
	Player.posy=0;
	Player.posz=0;
	Player.mapX=0;
	Player.mapY=0;
	Player.mapZ=0;
	Player.rotation=0;

	while(device->run())
		//if (device->isWindowActive())//)
		{
			// calculate time between frames
			oldtime = time;
			time = (f32)timer->getTime() / 1000.0f;
			deltaT = (time - oldtime);
			if(buildNewEnv)
			{
				buildNewEnv=false;
				env->clear();
				theMap.setMap(0,0,0);
				theMap.buildMap();
				Player.playerModel=smgr->addEmptySceneNode();
				Player.playerModel->setPosition(vector3df(0,2,0));
				GUISystem.buildMapEditor();
				guiBuilt=true;
				camera->setPosition(Player.playerModel->getPosition()+vector3df(10,10,10));
				camera->setTarget(Player.playerModel->getPosition());
				camRot.X=-cos(Player.playerModel->getRotation().Y*PI/180);
				camRot.Y=0;
				camRot.Z=-sin(Player.playerModel->getRotation().Y*PI/180);
				Player.modelHeight=Player.playerModel->getBoundingBox().getCenter().Y;
				Player.playerModel->setRotation(vector3df(sin(Player.rotation),0,cos(Player.rotation)));
				displayInfo=true;
///				maper = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)maperFunc, 0, 0, &thid);
                maperFunc();
                printf ("mapper 1");
				//initiate controls and let them play
				keyControl=2;
				getevents=3;
				drawScene=true;
			}
			switch(keyControl)
			{
			case 1:
				break;
			case 2:
				{
				//play mode, WASD work same with hot keys and jump aswell as all buttons
				mouseNewHighlighter=cursorcon->getPosition();
				if(mouseNewHighlighter!=mouseOldHighlighter)
				{
					mouseOldHighlighter=mouseNewHighlighter;
					theMap.selectVertex(smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(mouseNewHighlighter));
				}
				camera->setPosition(Player.playerModel->getPosition()+(camRot*camZoom));
				camera->setTarget(Player.playerModel->getPosition());
				//in game controls
				Player.movVec=vector3df(0,0,0);

				if(keys[irr::KEY_KEY_W])
				{
					Player.movVec.Z+=sin(Player.rotation*PI/180);
					Player.movVec.Y+=sin(camRot.Y);
					Player.movVec.X+=-cos(Player.rotation*PI/180);
				}
				if(keys[irr::KEY_KEY_A])
				{
					Player.movVec.Z+=sin((Player.rotation+270)*PI/180);
					Player.movVec.X+=-cos((Player.rotation+270)*PI/180);
				}
				if(keys[irr::KEY_KEY_S])
				{
					Player.movVec.Z+=sin((Player.rotation+180)*PI/180);
					Player.movVec.Y+=sin(camRot.Y+PI);
					Player.movVec.X+=-cos((Player.rotation+180)*PI/180);
				}
				if(keys[irr::KEY_KEY_D])
				{
					Player.movVec.Z+=sin((Player.rotation+90)*PI/180);
					Player.movVec.X+=-cos((Player.rotation+90)*PI/180);
				}
				//Mouse use
				if(mouseButtonState[0]==1)
				{
					//left button clicked
					position2d<s32> mpos=cursorcon->getPosition();
					if(mpos.X<985)
					{
						theMap.useTool(deltaT);
					}
					theMap.selectVertex(smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(mouseNewHighlighter));
				}

				if(mouseButtonState[2]==1){
					mousepositionnew=cursorcon->getPosition();
					vector3df rottest=Player.playerModel->getRotation();
					mouseButtonState[2]=2;
				}

				if(mouseButtonState[2]==2){
					mousepositionold=cursorcon->getPosition();
					mouseResetPos=mousepositionnew;
					cursorcon->setPosition(mouseResetPos);
					if((mousepositionnew.X-mousepositionold.X)>0)
					{
						mousepositionnew.X=mousepositionold.X+(Player.rotateSpeed);
					}else if((mousepositionnew.X-mousepositionold.X)<0)
					{
						updateRot=1;
						mousepositionnew.X=mousepositionold.X-(Player.rotateSpeed);
					}
					if((mousepositionnew.Y-mousepositionold.Y)>20)
					{
						mousepositionnew.Y=mousepositionold.Y+20;
					}else if((mousepositionnew.Y-mousepositionold.Y)<-20)
					{
						mousepositionnew.Y=mousepositionold.Y-20;
					}
					if(mousepositionnew!=mousepositionold)
					{
						updateRot++;
						Player.rotation-=(mousepositionnew.X-mousepositionold.X);//*(deltaT*100);
						if(Player.rotation<0)
						{
							Player.rotation=360+Player.rotation;
						}else if(Player.rotation>360)
						{
							Player.rotation=Player.rotation-360;
						}
						Player.playerModel->setRotation(vector3df(0,Player.rotation,0));
						camYPos-=mousepositionnew.Y-mousepositionold.Y;
						if(camYPos>90)
						{
							camYPos=90;
						}else if(camYPos<-90)
						{
							camYPos=-90;
						}
						camRot.X=-cos(Player.playerModel->getRotation().Y*PI/180);
						camRot.Y=sin(camYPos*PI/180);
						camRot.Z=sin(Player.playerModel->getRotation().Y*PI/180);
							//cursorcon->setPosition(mouseResetPos);
					}
					mousepositionnew=cursorcon->getPosition();
					waitformouseevent=5;
				}

				//update position
				double diagonalControl=1;
				diagonalControl=Player.movVec.X+Player.movVec.Z;
				if(diagonalControl==0)
				{
					diagonalControl=1;
				}
				//Player.movVec.X=Player.movVec.X/diagonalControl;
				//Player.movVec.Z=Player.movVec.Z/diagonalControl;
				Player.movVec*=-1;
				/*if(!Player.flying)
				{
					for(int x=0; x<4;x++)
					{
						if(theMap[x]->playerOnMap)
						{
							Player.movVec.Y=3*((theMap[x]->terrain->getHeight(Player.playerModel->getPosition().X*2, Player.playerModel->getPosition().Z*2)-Player.playerModel->getPosition().Y)-Player.modelHeight);
							//Player.playerModel->setPosition(Player.playerModel->getPosition()+vector3df(0,theMap[x]->terrain->getHeight(Player.playerModel->getPosition().X*2, Player.playerModel->getPosition().Z*2)-Player.playerModel->getPosition().Y-Player.modelHeight,0));
						}
					}
				}*/
				Player.playerModel->setPosition(Player.playerModel->getPosition()+(Player.movVec*Player.speed*deltaT));
				Player.movVec.Y=0;
				break;
				}
				case 3:
					{
						//for chat boxes... return will send the box to server
					if(keys[irr::KEY_RETURN])
					{
						keyControl=2;
						getevents=3;
						env->setFocus(0);
					}

					break;
					}
			}// end switch

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
				if(env->hasFocus(blueEditBox)||env->hasFocus(greenEditBox)||env->hasFocus(redEditBox)||env->hasFocus(sphereRadiusEditBox)||env->hasFocus(heightRateEditBox))
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
			if(displayInfo)
			{
            swprintf(tmp, 1024, L"frames per second: %i :: POS: X %f Y %f Z %f", fps, Player.playerModel->getPosition().X, Player.playerModel->getPosition().Y, Player.playerModel->getPosition().Z);
			font->draw(tmp, core::rect<s32>( 200, 50, 600, 60 ), video::SColor(255,255,255,255));
			}
			if(drawScene)
			{
			printf ("drawing");
				smgr->drawAll();
			}
			env->drawAll();

			driver->endScene();

			//				maperFunc();
		}
	return 0;

}
