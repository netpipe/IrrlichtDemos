int WINAPI recvdatafunc()
{
	int bytesRecv = SOCKET_ERROR;
	int totalRecv=0;
	bool test=true;
	while(runloop)
	{
		//while(bytesRecv == SOCKET_ERROR)
		char recvbuf[51250];
		for(int x=0;x<51250;x++)
		{
			recvbuf[x]=0;
		}
	    bytesRecv = recv(serverSocket, recvbuf, 51250, 0);
		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
		{
			printf("Client: Connection Closed.\n");
			connected=-1;
			return 0;
			break;
		}
		
		if (bytesRecv < 0){
			connected=-1;
			return 0;
		}
	    else
		{
			totalRecv+=bytesRecv;
			vector<stringc> netString;
			stringc fullServerString="";
			int nextVert=0, indexPos=0, stuckCounter=0;
			bool done=false;
			//get buffer into string format
			for(int x=0;x<bytesRecv;x++)
			{
				fullServerString.append(recvbuf[x]);
			}
			printf("SIZE: %i\n", fullServerString.size());
			//loop through string
			while(indexPos!=bytesRecv&&stuckCounter<201)//stuckCounter<201||!done||
			{
				//get to next | and save position
				nextVert=fullServerString.findNext('|',indexPos);
				//get size of message
				int sizeOfMessage=atoi(fullServerString.subString(indexPos,nextVert-indexPos).c_str());
				if(sizeOfMessage!=0)
				{
					printf("new netString size: %i\n",sizeOfMessage);
					netString.push_back(fullServerString.subString(indexPos,sizeOfMessage));
					indexPos+=sizeOfMessage;
				}else
				{
					stuckCounter++;
					if(stuckCounter>200)
					{
						printf("STUCK: %i :: %i\n",indexPos,bytesRecv );
					}
				}
			}
			/*if(test&&fullServerString.size()>200)
			{
				cout << "TEST "<<fullServerString.size()<<" :: ";
				for(int x=0;x<fullServerString.size();x++)
				{
					cout << *(fullServerString.c_str()+x);
				}
				cout <<"::END\n";
				test=false;
			}*/
			printf("done");
			printf("totRecv %i",totalRecv);
			//Handle each code sent to client
			for(int x=0; x<netString.size();x++)
			{
				stringc fileInString;
				stringc serverString=netString[x];
				stringc code;
				//printf("BUFFER ");
				//printf(serverString.c_str(), '\n');
				cout << endl;
				int start=serverString.findNext('|',0)+1;
				code.append(serverString.subString(start,serverString.findNext('|',start)-start));
				//printf("code: %s", code.c_str());
				switch(atoi(code.c_str()))
				{
					case 1:
						{
							if(Player.id<0)
							{
								returnEnvId=1;
								stringc code2;
								s32 breakPos=0, breakPos2=0;
								breakPos = serverString.findNext('|',0);
								breakPos++;
								breakPos2 = serverString.findNext('|',breakPos);
								code2=serverString.subString(breakPos, breakPos2-breakPos);
								switch(atoi(code2.c_str()))
								{
								case 0:
									//login failed
									envId=10;
									buildNewEnv=true;
									pauseLength=5000;
									pauseScreen=true;
									break;
								case 1:
									//login success
									//has character, wait for data from server
									envId=12;
									buildNewEnv=true;
									pauseScreen=false;
									break;
								case 2:
									//make new char window
									break;
								case 3:
									//banned
									envId=11;
									buildNewEnv=true;
									pauseLength=99999999999999;
									pauseScreen=true;
									break;
								}
							}
							break;
						}
					case 13:
						{
							//get start data
							stringc brokenString[31];
							breakCode(serverString, brokenString,31);
							Player.inGameName=brokenString[1];
							Player.title=brokenString[2];
							Player.id=atoi(brokenString[3].c_str());
							Player.mapX=atoi(brokenString[4].c_str());
							Player.mapY=atoi(brokenString[5].c_str());
							Player.mapZ=atoi(brokenString[6].c_str());
							Player.posx=atoi(brokenString[7].c_str());
							Player.posy=atoi(brokenString[8].c_str());
							Player.posz=atoi(brokenString[9].c_str());
							Player.level=atoi(brokenString[10].c_str());
							Player.intel=atoi(brokenString[11].c_str());
							Player.con=atoi(brokenString[12].c_str());
							Player.dex=atoi(brokenString[13].c_str());
							Player.str=atoi(brokenString[14].c_str());
							Player.rotation=atoi(brokenString[15].c_str());
							Player.raceId=atoi(brokenString[16].c_str());
							Player.mountId=atoi(brokenString[17].c_str());
							Player.modelId=atoi(brokenString[18].c_str());
							Player.expOn=atoi(brokenString[19].c_str());
							Player.expTot=atoi(brokenString[20].c_str());
							Player.skillLevel=atoi(brokenString[21].c_str());
							Player.unusedSkillPoints=atoi(brokenString[22].c_str());
							Player.pvp=atoi(brokenString[23].c_str());
							Player.showeq=atoi(brokenString[24].c_str());
							Player.visibility=atoi(brokenString[25].c_str());
							Player.criminal=atoi(brokenString[26].c_str());
							Player.maxhp=atoi(brokenString[27].c_str());
							Player.curhp=atoi(brokenString[28].c_str());
							Player.mounted=atoi(brokenString[29].c_str());
							Player.speed=atoi(brokenString[30].c_str());
							envId=13;
							buildNewEnv=true;
							pauseScreen=false;
							break;
						}
					
						case 20:
						{
							//find out what this is for
							stringc brokenString[9];
							breakCode(serverString, brokenString,9);
							for(int x=0;x<500;x++)
							{
								if(charDat[x].id==atoi(brokenString[1].c_str())&&charDat[x].loaded)
								{
									charDat[x].posx=atoi(brokenString[2].c_str());
									charDat[x].posy=atoi(brokenString[3].c_str());
									charDat[x].posz=atoi(brokenString[4].c_str());
									charDat[x].mapX=atoi(brokenString[5].c_str());
									charDat[x].mapY=atoi(brokenString[6].c_str());
									charDat[x].mapZ=atoi(brokenString[7].c_str());
									charDat[x].rotation=atoi(brokenString[8].c_str());
									charDat[x].playerModel->setRotation(vector3df(0,charDat[x].rotation,0));
									charDat[x].playerModel->setPosition(vector3df(charDat[x].posx,charDat[x].posy,charDat[x].posz));
									x=500;
								}
							}
							break;
						}
						case 22:
						{
							//get player movement
							printf("NET22:: %s\n", serverString.c_str());
							stringc brokenString[9];
							breakCode(serverString, brokenString,9);
							for(int x=0;x<charDat.size();x++)
							{
								printf("net22\n");
								if(charDat[x].id==atoi(brokenString[1].c_str())&&charDat[x].loaded)
								{
									charDat[x].startMovTime=timer->getRealTime();
									printf("!Moving player %f from %f %f %f to %f %f %f! ", charDat[x].id,charDat[x].playerModel->getPosition().X,charDat[x].playerModel->getPosition().Y,charDat[x].playerModel->getPosition().Z, atoi(brokenString[2].c_str()), atoi(brokenString[3].c_str()), atoi(brokenString[4].c_str()));
									charDat[x].movVec.X=(atoi(brokenString[2].c_str())-charDat[x].playerModel->getPosition().X)/5;
									charDat[x].movVec.Y=(atoi(brokenString[3].c_str())-charDat[x].playerModel->getPosition().Y)/5;
									charDat[x].movVec.Z=(atoi(brokenString[4].c_str())-charDat[x].playerModel->getPosition().Z)/5;
									charDat[x].rotation=atoi(brokenString[5].c_str());
									charDat[x].mapX=atoi(brokenString[6].c_str());
									charDat[x].mapY=atoi(brokenString[7].c_str());
									charDat[x].mapZ=atoi(brokenString[8].c_str());
									charDat[x].playerModel->setRotation(vector3df(0,charDat[x].rotation,0));
									x=charDat.size();
								}
							}  
							break;

						}
						case 23:
							{
								//get chat message
								stringc brokenString[3];
								breakCode(serverString, brokenString,3);
								stringw wstr;
								convertString(brokenString[2],wstr, 2);
								chatBox->addItem(wstr.c_str());
								break;
							}
						case 25:
							{
								//get equipment
								stringc brokenString[20];
								breakCode(serverString, brokenString,20);
								//assign equipment to item system and equipment list
								for(int x=1;x<20;x++)
								{
									//loop through equipment sent
									int id=atoi(brokenString[x].c_str());
									itemSystem.addItem(id);
									Player.setEquipment(x-1,id);
									
								}
								break;
							}
						case 26:
							{
								//get inventory
								stringc brokenString[41];
								breakCode(serverString, brokenString,41);
								//assign equipment to item system and equipment list
								for(int x=1;x<41;x++)
								{
									//loop through equipment sent
									int id=atoi(brokenString[x].c_str());
									itemSystem.addItem(id);
									Player.setInventory(x-1,id);
								}
								break;
							}
						case 27:
						{
							//get new char from server
							stringc brokenString[23];
							breakCode(serverString, brokenString,23);
							bool addedDat=false;
							for(int x=0;x<charDat.size();x++)
							{
								if(!charDat[x].inUse)
								{
									addedDat=true;
									charDat[x].inUse=true;
									charDat[x].inGameName=brokenString[1];
									charDat[x].title=brokenString[2];
									charDat[x].id=atoi(brokenString[3].c_str());
									charDat[x].mapX=atoi(brokenString[4].c_str());
									charDat[x].mapY=atoi(brokenString[5].c_str());
									charDat[x].mapZ=atoi(brokenString[6].c_str());
									charDat[x].posx=atoi(brokenString[7].c_str());
									charDat[x].posy=atoi(brokenString[8].c_str());
									charDat[x].posz=atoi(brokenString[9].c_str());
									charDat[x].level=atoi(brokenString[10].c_str());
									charDat[x].rotation=atoi(brokenString[11].c_str());
									charDat[x].raceId=atoi(brokenString[12].c_str());
									charDat[x].mountId=atoi(brokenString[13].c_str());
									charDat[x].modelId=atoi(brokenString[14].c_str());
									charDat[x].pvp=atoi(brokenString[15].c_str());
									charDat[x].showeq=atoi(brokenString[16].c_str());
									charDat[x].visibility=atoi(brokenString[17].c_str());
									charDat[x].criminal=atoi(brokenString[18].c_str());
									charDat[x].maxhp=atoi(brokenString[19].c_str());
									charDat[x].curhp=atoi(brokenString[20].c_str());
									charDat[x].mounted=atoi(brokenString[21].c_str());
									charDat[x].speed=atoi(brokenString[22].c_str());;
									charDat[x].buildModel=true;
									x=charDat.size();
								}
							}
							if(!addedDat)
							{
								Character newDat;
								newDat.inUse=true;
								newDat.inGameName=brokenString[1];
								newDat.title=brokenString[2];
								newDat.id=atoi(brokenString[3].c_str());
								newDat.mapX=atoi(brokenString[4].c_str());
								newDat.mapY=atoi(brokenString[5].c_str());
								newDat.mapZ=atoi(brokenString[6].c_str());
								newDat.posx=atoi(brokenString[7].c_str());
								newDat.posy=atoi(brokenString[8].c_str());
								newDat.posz=atoi(brokenString[9].c_str());
								newDat.level=atoi(brokenString[10].c_str());
								newDat.rotation=atoi(brokenString[11].c_str());
								newDat.raceId=atoi(brokenString[12].c_str());
								newDat.mountId=atoi(brokenString[13].c_str());
								newDat.modelId=atoi(brokenString[14].c_str());
								newDat.pvp=atoi(brokenString[15].c_str());
								newDat.showeq=atoi(brokenString[16].c_str());
								newDat.visibility=atoi(brokenString[17].c_str());
								newDat.criminal=atoi(brokenString[18].c_str());
								newDat.maxhp=atoi(brokenString[19].c_str());
								newDat.curhp=atoi(brokenString[20].c_str());
								newDat.mounted=atoi(brokenString[21].c_str());
								newDat.speed=atoi(brokenString[22].c_str());;
								//type, player, npc friendly, neutral, enemy
								newDat.buildModel=true;
								charDat.push_back(newDat);
							}
							break;
						}
						case 28:
							{
								//get npc start data
								npcClass npc;
								npc.loadFromServer(serverString);
								npcDat.push_back(npc);
								break;
							}
						case 29:
							{
								//get npc update
								stringc brokenString[10];
								breakCode(serverString, brokenString,10);
								for(int x=0;x<npcDat.size();x++)
								{
									if(npcDat[x].getId()==atoi(brokenString[1].c_str()))
									{
										npcDat[x].updateFromServer(serverString);
									}
								}
								break;
							}
						case 30:
							{
								//get item info
								printf("\nGOT ITEM INFO\n");
								stringc brokenString[13];
								breakCode(serverString, brokenString,13);
								itemSystem.setInfo(atoi(brokenString[1].c_str()),serverString);
								break;
							}
						case 31:
							{
								//get item description
								stringc brokenString[3];
								breakCode(serverString, brokenString,3);
								itemSystem.setDescription(atoi(brokenString[1].c_str()),serverString);
								break;
							}
						case 32:
							{
								//get inventory update
								
								stringc brokenString[4];
								breakCode(serverString, brokenString,4);
								int type=atoi(brokenString[1].c_str());
								int loc=atoi(brokenString[2].c_str());
								int id =atoi(brokenString[3].c_str());
								itemSystem.addItem(id);
								printf("!!UPDATING INVENTORY %i %i %i!!\n", type, loc, id);
								if(type==1)
								{
									Player.setInventory(loc,id);
								}
								else if(type==4)
								{
									Player.setEquipment(loc,id);
								}
								if(GUISystem.isInventoryBuilt())
								{
									if(type==1)
									{
										char imgloc[100];
										sprintf(imgloc,"images/inv/%i.png",itemSystem.getIcon(Player.inventoryList[loc]));
										GUISystem.changeButton(129+loc,imgloc,129+loc);
									}else if(type==4)
									{
										char imgloc[100];
										sprintf(imgloc,"images/inv/%i.png",itemSystem.getIcon(Player.equipmentList[loc]));
										GUISystem.changeButton(110+loc,imgloc,110+loc);
									}
								}
								break;
							}
						case 33:
							{
								//get target info, determines how to handle clicking from server
								//will add an indicator of some form to show how you will effect it
								//wether its attack or friendly
								break;
							}
						case 34:
							{
								//get number to display on given target, pending type it changes color
								//used for skills, attack, healing, poison, things like that to show
								//health changes
								break;
							}
						case 35:
							{
								//health change
								stringc brokenString[4];
								breakCode(serverString, brokenString,4);
								if(atoi(brokenString[1].c_str())==0)
								{
									//update npc
									int uid=atoi(brokenString[2].c_str());
									for(int x=0;x<npcDat.size();x++)
									{
										if(npcDat[x].getId()==uid)
										{
											npcDat[x].setHealth(atoi(brokenString[3].c_str()));
											x=npcDat.size();
										}
									}
								}
								else if(atoi(brokenString[1].c_str())==1)
								{
									//update other player
									int uid=atoi(brokenString[2].c_str());
									for(int x=0;x<charDat.size();x++)
									{
										if(charDat[x].id==uid)
										{
											charDat[x].curhp=atoi(brokenString[3].c_str());
											x=charDat.size();
										}
									}
								}
								else if(atoi(brokenString[1].c_str())==2)
								{
									//update self
									Player.curhp=atoi(brokenString[3].c_str());
								}
								break;
							}
				}//endswitch
			}//end loop
		}
	} 
	runloop=false;
}

void connectToServer()
{
	if(connected!=0){
		env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
		env->addStaticText(L"Connecting to Server", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
		driver->beginScene(true, true, SColor(0,122,65,171));
		env->drawAll();
		driver->endScene();
		buildNewEnv=true;
		//pauseLength=5000;
		//pauseScreen=true;
		//if not connected
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != NO_ERROR)
		{
			//report error occured on computer
			envId=2;
			buildNewEnv=true;
			pauseLength=5000;
			pauseScreen=true;
			WSACleanup();
		}
		else
		{
			//if success
			// Create a socket.
			serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (serverSocket == INVALID_SOCKET)
			{
				//report error occured on computer
				envId=2;
				buildNewEnv=true;
				pauseLength=5000;
				pauseScreen=true;
				WSACleanup();
			}
			else
			{
				//if success
				// Connect to a server.
				clientService.sin_family = AF_INET;
				// Just test using the localhost, you can try other IP address
				// more information needed on how this works.
				clientService.sin_addr.s_addr = inet_addr("98.228.83.14");
				clientService.sin_port = htons(50000);
				connected = connect(serverSocket, (SOCKADDR*)&clientService, sizeof(clientService));
				//if success
				if(connected==0){
					//connected to server
					envId=4;
					buildNewEnv=true;
				}else{
					//report error occured on connecting to server
					envId=2;
					buildNewEnv=true;
					pauseLength=5000;
					pauseScreen=true;
					WSACleanup();
				}
			}
		}
		recvdata = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvdatafunc, NULL, 0, &thid);
	}
}