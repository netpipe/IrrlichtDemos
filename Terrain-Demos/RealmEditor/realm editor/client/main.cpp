#include "stdafx.h"
int main(){
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


	Player.id=-1;
	// fog
	//driver->setFog(video::SColor(255,100,101,140), true, .5*200/4, .5*(200-4)/2, 0.05f);

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
				case 1:
					//login page
					keyControl=1;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					globaltxtbox1= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2));
					globaltxtbox2= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+40));
					env->addButton(rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2-25, (driver->getScreenSize().Height)/2+80),0,1,L"Login");
					env->addButton(rect<s32>((driver->getScreenSize().Width)/2+25,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+80),0,2,L"Quit");
					env->addButton(rect<s32>((driver->getScreenSize().Width)/2-20,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+20, (driver->getScreenSize().Height)/2+80),0,3,L"Register");
					env->addStaticText(L"Username", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-20));
					env->addStaticText(L"Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+20));
					break;
				case 2:
					//connection error page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Connection Failed Initialize Network Returning to login", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;
				case 3:
					//connecting to server page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Connecting to Server", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;
				case 4:
					//connected to server page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Connected to Server, Sending Information", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;
				case 5:
					//logging into server page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Logging in, please wait", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;
				case 6:
					//register page
					keyControl=1;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					globaltxtbox1= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-80));
					globaltxtbox2= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-40));
					globaltxtbox3= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2));
					globaltxtbox4= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+40));
					env->addButton(rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2-15, (driver->getScreenSize().Height)/2+80),0,5,L"Register");
					env->addButton(rect<s32>((driver->getScreenSize().Width)/2+25,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+80),0,2,L"Quit");
					env->addButton(rect<s32>((driver->getScreenSize().Width)/2-10,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+20, (driver->getScreenSize().Height)/2+80),0,4,L"Cancel");
					env->addStaticText(L"Confirm Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-20));
					env->addStaticText(L"Email", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+20));
					env->addStaticText(L"Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-60, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-60));
					env->addStaticText(L"Username", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-100, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-100));
					break;
				
				case 7:
					//registering page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Registering, please wait", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;

				case 8:
					//passwords don't match page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Passwords do not match", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;	
				case 9:
					//missing a field or invalid field page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Invalid field", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;	
				case 10:
					//failed to login page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Login failed, invalid password or you need to register", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;
				case 11:
					//banned page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Sorry but you are banned, be a little smarter next time", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;
				case 12:
					//login successful page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Logged in, awaiting data", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					break;
				case 13:
					{
					//Building Map page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Building Map, will take time", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					driver->beginScene(true, true, SColor(0,122,65,171));
					env->drawAll();
					driver->endScene();
					//build fake player
					Player.playerModel=smgr->addEmptySceneNode();
					theMap.setMap(Player.mapX, Player.mapY, Player.mapZ);
					//theMap.setNewColor(0,0,0);
					theMap.buildMap();				
					maper = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)maperFunc, 0, 0, &thid);

					thid++;
					envId=14;
					buildNewEnv=true;
					break;
					}
				case 14:
					{
					//building character page
					keyControl=0;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Map Loaded, building character", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					driver->beginScene(true, true, SColor(0,122,65,171));
					env->drawAll();
					driver->endScene();
					stringc meshName="models/";
					char buffer[32];
					meshName.append(itoa(Player.modelId, buffer, 10));
					meshName.append(".b3d");
					Player.playerModel=smgr->addAnimatedMeshSceneNode(smgr->getMesh("models/sydney.md2"));//meshName.c_str()));
					meshName="textures/";
					buffer[32];
					meshName.append(itoa(Player.modelId, buffer, 10));
					meshName.append(".png");
					Player.playerModel->setMaterialTexture(0, driver->getTexture("textures/sydney.bmp"));//meshName.c_str()));
					Player.playerModel->setMaterialFlag(video::EMF_LIGHTING, false);
					
				//equipment build
					Player.playerModel->setPosition(vector3df(Player.posx,Player.posy,Player.posz));
					Player.playerModel->setRotation(vector3df(sin(Player.rotation),0,cos(Player.rotation)));
					Player.playerModel->setScale(vector3df(.05,.05,.05));
					camera->setPosition(Player.playerModel->getPosition()+vector3df(10,10,10));
					camera->setTarget(Player.playerModel->getPosition());
					camRot.X=-cos(Player.playerModel->getRotation().Y*PI/180);
					camRot.Y=0;
					camRot.Z=-sin(Player.playerModel->getRotation().Y*PI/180);
					envId=15;
					Player.modelHeight=Player.playerModel->getBoundingBox().getCenter().Y;
					buildNewEnv=true;
					pauseScreen=false;
					displayInfo=true;
					break;
					}
				case 15:
					keyControl=0;
					//characters = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)charactersFunc, 0, 0, &thid);
					//SetThreadPriority(characters, THREAD_PRIORITY_LOWEST  );
					thid++;
					env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
					env->addStaticText(L"Character Built, building gui", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
					driver->beginScene(true, true, SColor(0,122,65,171));
					env->drawAll();
					driver->endScene();
					env->clear();
					//build gui
					//env->addImage(driver->getTexture("images/guibg.png"),position2d<s32>(0,0), true, 0, 101);
					GUISystem.buildInterface();
					GUISystem.buildChatBox();
					guiBuilt=true;
					//initiate controls and let them play
					keyControl=2;
					getevents=3;
					drawScene=true;
					break;
				}
			}
			switch(keyControl)
			{
			case 1:
				break;
			case 2:
				{
					//play mode, WASD work same with hot keys and jump aswell as all buttons
				camera->setPosition(Player.playerModel->getPosition()+(camRot*camZoom));
				camera->setTarget(Player.playerModel->getPosition());
				//in game controls
				Player.movVec=vector3df(0,0,0);
				if(keys[irr::KEY_KEY_W])
				{
					Player.movVec.Z+=sin(Player.rotation*PI/180);
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
					//see if clicked LMB on button/GUI or on a target
					pressedPosition=cursorcon->getPosition();
					pressedId=GUISystem.findButtonFromPoint(pressedPosition, pressedChangeable, pressedSwap);
					if(pressedId==-1)
					{
						//see if clicked on a target
						targetNode=smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(pressedPosition));
						//if target found
						int type=0;
						if(targetNode!=0)
						{
							int dataIndex=-1;
							for(int x=0;x<charDat.size();x++)
							{
								if(targetNode->getID()==charDat[x].nodeId)
								{
									dataIndex=x;
									type=1;
								}
							}
							for(int x=0;x<npcDat.size();x++)
							{
								if(targetNode->getID()==npcDat[x].getNodeId())
								{
									dataIndex=x;
									type=2;
								}
							}
							if(dataIndex>-1)
							{
								//send target data to server
								stringc sendCode;
								char dataIn[200];
								int key=0;
								if(keys[irr::KEY_CONTROL])
								{
									key=1;
								}
								if(type==1)
								{
									sprintf(dataIn,"9|%i|%i|%i|",charDat[dataIndex].id, type, key);
								}
								else if(type==2)
								{
									sprintf(dataIn,"9|%i|%i|%i|",npcDat[dataIndex].getId(), type, key);
								}
								sendCode.append(dataIn);
								send(serverSocket,sendCode.c_str(), sendCode.size(),NULL);
							}
						}else
						{
							//no target, no button. remove target if possible
							stringc sendCode;
							char dataIn[200];
							sprintf(dataIn,"9|-1|-1|2|");
							sendCode.append(dataIn);
							send(serverSocket,sendCode.c_str(), sendCode.size(),NULL);
						}
					}
					mouseButtonState[0]=2;
				}
				if(mouseButtonState[0]==2)
				{
					//left button held down
					//on release of LMB if clicked on button above then check what to do on release if mouse moved
					if(pressedId>-1)
					{
						position2d<s32> mpos=cursorcon->getPosition();
						if(mpos!=pressedPosition)
						{
							bool buttonChangeable=false, buttonSwap=false;
							int buttonId=GUISystem.findButtonFromPoint(mpos, buttonChangeable, buttonSwap);
							if(buttonId!=pressedId)
							{
								//attach image to cursor
								mouseButtonState[0]=4;
							}
						}
					}
				}
				if(mouseButtonState[0]==3)
				{
					//left button released
					//on release of LMB if clicked on button above then check what to do if mouse moved
					if(pressedId>-1)
					{
						position2d<s32> mpos=cursorcon->getPosition();
						if(mpos!=pressedPosition)
						{
							bool buttonChangeable=false, buttonSwap=false;
							int buttonId=GUISystem.findButtonFromPoint(mpos, buttonChangeable, buttonSwap);
							if(buttonId!=pressedId)
							{
								//send button data to server
								int toType=GUISystem.getButtonTypeFromId(buttonId);
								int fromType=GUISystem.getButtonTypeFromId(pressedId);
								//quick checks, can't go to skills, skills can't go to inv or eq, quick can only go to other quick
								printf("%i|%i\n", fromType, toType);
								if(((fromType==3||fromType==4||fromType==1||fromType==2)&&toType==2)||((fromType==4||fromType==1)&&(toType==1||toType==4))||toType==-1)
								{
									stringc sendCode;
									char dataIn[200];
									//change id's based of type
									if(fromType==1)
									{
										pressedId-=129;
									}else
									if(fromType==2)
									{
										pressedId-=300;
									}else
									if(fromType==3)
									{
										pressedId-=0;
									}else
									if(fromType==4)
									{
										pressedId-=110;
									}
									if(toType==1)
									{
										buttonId-=129;
									}else
									if(toType==2)
									{
										buttonId-=300;
									}else
									if(toType==3)
									{
										buttonId-=0;
									}else
									if(toType==4)
									{
										buttonId-=110;
									}
									//send data to server
									sprintf(dataIn,"11|%i|%i|%i|%i|",fromType,pressedId, toType, buttonId);
									sendCode.append(dataIn);
									send(serverSocket,sendCode.c_str(), sendCode.size(),NULL);
								}
							}
						}
						pressedId=-1;
					}
					mouseButtonState[0]=0;
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
						}else if(camYPos<0)
						{
							camYPos=0;
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
				if((Player.movVec!=lastMovVec||Player.rotation!=lastRotation)&&updateDelay<=timer->getRealTime())
				{
					//printf("test");
					//cout << "POSX: " << Player.speed << " :: 5SEC: " << Player.movVec.X*Player.speed*5 << endl;
					forceUpdate=timer->getRealTime()+1000;
					updateDelay=timer->getRealTime()+500;
					//update server
					stringc sendData="3|";
					char data[200]="";
					sprintf(data,"%f", Player.playerModel->getPosition().X); 
					sendData.append(data);
					sendData.append('|');
					sprintf(data,"%f", Player.playerModel->getPosition().Y); 
					sendData.append(data);
					sendData.append('|');
					sprintf(data,"%f", Player.playerModel->getPosition().Z); 
					sendData.append(data);
					sendData.append('|');
					sprintf(data,"%f", Player.rotation); 
					sendData.append(data);
					sendData.append('|');
					sprintf(data,"%f", Player.movVec.X); 
					sendData.append(data);
					sendData.append('|');
					sprintf(data,"%f", Player.movVec.Y); 
					sendData.append(data);
					sendData.append('|');
					sprintf(data,"%f", Player.movVec.Z); 
					sendData.append(data);
					sendData.append('|');
					send(serverSocket,sendData.c_str(), sendData.size(),NULL);
					//sendData.append();
					lastMovVec=Player.movVec;
					lastRotation=Player.rotation;
				}
				
				break;
				}
				case 3:
					{
						//for chat boxes... return will send the box to server
					if(keys[irr::KEY_RETURN])
					{
						if(env->hasFocus(chatEdit))
						{
							stringc chatMsg=chatEdit->getText();
							chatEdit->setText(L"");
							//send to server
							int location=0;
							while(location>-1)
							{
								location=chatMsg.find("|");
								if(location>-1)
								{
									chatMsg[location]=' ';
								}
							}
							stringc toServer="12|";
							toServer.append(chatMsg);
							toServer.append("|");
							send(serverSocket,toServer.c_str(), toServer.size(),NULL);
						}
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
			//update local item database
			itemSystem.update();
			//build other players and npcs
			for(int r=0;r<charDat.size();r++)
			{
				if(charDat[r].buildModel)
				{
					printf("building Model");
					charDat[r].buildModel=false;
					stringc meshName="models/";
					char buffer[32];
					meshName.append(itoa(charDat[r].modelId, buffer, 10));
					meshName.append(".b3d");
					charDat[r].playerModel=smgr->addAnimatedMeshSceneNode(smgr->getMesh("models/sydney.md2"),0,globalNodeId);//meshName.c_str()));
					charDat[r].nodeId=globalNodeId;
					globalNodeId++;
					meshName="textures/";
					buffer[32];
					meshName.append(itoa(charDat[r].modelId, buffer, 10));
					meshName.append(".png");
					charDat[r].playerModel->setMaterialTexture(0, driver->getTexture("textures/sydney.bmp"));//meshName.c_str()));
					charDat[r].playerModel->setMaterialFlag(video::EMF_LIGHTING, false);
					//equipment build
					charDat[r].playerModel->setPosition(vector3df(charDat[r].posx,charDat[r].posy,charDat[r].posz));
					charDat[r].playerModel->setRotation(vector3df(sin(charDat[r].rotation),0,cos(charDat[r].rotation)));
					charDat[r].playerModel->setScale(vector3df(.05,.05,.05));
					charCount++;
					charDat[r].loaded=true;
				}else 
				if(charDat[r].id>0&&charDat[r].loaded&&!charDat[r].buildModel)
				{
					//if the slot is in use
					//movement
					if(charDat[r].startMovTime+5000>timer->getRealTime())
					{
						charDat[r].playerModel->setPosition(charDat[r].playerModel->getPosition()+(charDat[r].movVec*deltaT));
					}
					//server wants to reset position
					/*if(charDat[r].changePosition==1)
					{
						//need maps to be done
					}
					//server wants to change animations
					if(charDat[r].changeAction==1)
					{
						//need animation file and complete model
					}*/
				}
			}
			for(int x=0;x<npcDat.size();x++)
			{
				if(npcDat[x].getBuildModel()==true)
				{
					npcDat[x].buildModelFunc();
				}else
				{
					npcDat[x].update(deltaT);
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
				smgr->drawAll();
			}
			env->drawAll();
			
			driver->endScene();
		}

		TerminateThread(maper,0);
	device->drop();
	return 0;
}