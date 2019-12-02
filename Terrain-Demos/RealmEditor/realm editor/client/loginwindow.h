/*void loginWindow(){
//ends the while loops
	runloop=true;
	receiver.getKeyboardEvents=false;
	receiver.getMouseEvents=false;
	receiver.getEnterPress=true;
	env->clear();
	//set background image
	IGUIImage* loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
	globaltxtbox1= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2));
	globaltxtbox2= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+40));
	env->addButton(rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2-25, (driver->getScreenSize().Height)/2+80),0,1,L"Login");
	env->addButton(rect<s32>((driver->getScreenSize().Width)/2+25,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+80),0,2,L"Quit");
	env->addButton(rect<s32>((driver->getScreenSize().Width)/2-20,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+20, (driver->getScreenSize().Height)/2+80),0,3,L"Register");
	env->addStaticText(L"Username", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-20));
	env->addStaticText(L"Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+20));

	printf("%d", receiver.getMouseEvents);
	//draw(display) login page
	while(device->run() && driver && runloop)
		if (device->isWindowActive()) 
		{
			if(keys[13]==PRESSED){
				keys[13]=DOWN;
				//login
						//remove background
						//env->clear();
						IGUIImage* loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
						if(connected==0){
							//if connected
							env->addStaticText(L"Sending Information", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
							driver->beginScene(true, true, SColor(0,122,65,171));
							env->drawAll();
							driver->endScene();
							stringc username;
							stringc password;
							stringw boxtxt;
							boxtxt=globaltxtbox1->getText();
							convertString(username, boxtxt, 1);
							stringc loginclient="1|";
							loginclient.append(username);
							loginclient.append('|');
							boxtxt=globaltxtbox2->getText();
							convertString(password, boxtxt, 1);
							loginclient.append(password);
							loginclient.append('|');
							send(serverSocket,loginclient.c_str(), loginclient.size(),0);
						}else{
							//if not connected
							env->addStaticText(L"Connecting to Server", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
							driver->beginScene(true, true, SColor(0,122,65,171));
							env->drawAll();
							driver->endScene();
							int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
							if (iResult != NO_ERROR)
							{
								//report error occured on computer
								loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
								env->addStaticText(L"Connection Failed Initialize Network Returning to login", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
								driver->beginScene(true, true, SColor(0,122,65,171));
								env->drawAll();
								driver->endScene();
								u32 startTime=timer->getRealTime();
								while(startTime+5000>timer->getRealTime())
								{
									driver->beginScene(true, true, SColor(0,122,65,171));
									env->drawAll();
									driver->endScene();
								}
								//rebuild login area
								env->clear();
								loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
								globaltxtbox1= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2));
								globaltxtbox2= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+40));
								env->addButton(rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2-25, (driver->getScreenSize().Height)/2+80),0,1,L"Login");
								env->addButton(rect<s32>((driver->getScreenSize().Width)/2+25,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+80),0,2,L"Quit");
								env->addButton(rect<s32>((driver->getScreenSize().Width)/2-20,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+20, (driver->getScreenSize().Height)/2+80),0,3,L"Register");
								env->addStaticText(L"Username", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-20));
								env->addStaticText(L"Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+20));
								driver->beginScene(true, true, SColor(0,122,65,171));
								env->drawAll();
								driver->endScene();
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
									loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
									env->addStaticText(L"Connection Failed Initialize Network Returning to login", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
									driver->beginScene(true, true, SColor(0,122,65,171));
									env->drawAll();
									driver->endScene();
									u32 startTime=timer->getRealTime();
									while(startTime+5000>timer->getRealTime())
									{
										driver->beginScene(true, true, SColor(0,122,65,171));
										env->drawAll();
										driver->endScene();
									}
									//rebuild login area
									env->clear();
									loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
									globaltxtbox1= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2));
									globaltxtbox2= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+40));
									env->addButton(rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2-25, (driver->getScreenSize().Height)/2+80),0,1,L"Login");
									env->addButton(rect<s32>((driver->getScreenSize().Width)/2+25,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+80),0,2,L"Quit");
									env->addButton(rect<s32>((driver->getScreenSize().Width)/2-20,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+20, (driver->getScreenSize().Height)/2+80),0,3,L"Register");
									env->addStaticText(L"Username", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-20));
									env->addStaticText(L"Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+20));
									driver->beginScene(true, true, SColor(0,122,65,171));
									env->drawAll();
									driver->endScene();
									WSACleanup();
								}
								else
								{
									//if success
									// Connect to a server.
									clientService.sin_family = AF_INET;
									// Just test using the localhost, you can try other IP address
									// more information needed on how this works.
									clientService.sin_addr.s_addr = inet_addr("192.168.1.115");
									clientService.sin_port = htons(65271);
									connected = connect(serverSocket, (SOCKADDR*)&clientService, sizeof(clientService));
									//if success
									if(connected==0){
										loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
										env->addStaticText(L"Connected to Server, Sending Information", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
										driver->beginScene(true, true, SColor(0,122,65,171));
										env->drawAll();
										driver->endScene();
										
										recvdata = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvdatafunc, NULL, 0, &thid);
										thid++;
										stringc username;
										stringc password;
										stringw boxtxt;
										boxtxt=globaltxtbox1->getText();
										convertString(username, boxtxt, 1);
										stringc loginclient="1|";
										loginclient.append(username);
										loginclient.append('|');
										boxtxt=globaltxtbox2->getText();
										convertString(password, boxtxt, 1);
										loginclient.append(password);
										loginclient.append('|');
										printf("test %s test",loginclient);
										send(serverSocket,loginclient.c_str(), loginclient.size(),0);
									}else{
										//report error occured on connecting to server
										loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
										env->addStaticText(L"Failed to Connect to Server Returning to login", rect<s32>((driver->getScreenSize().Width)/2-40,(driver->getScreenSize().Height)/2-10, (driver->getScreenSize().Width)/2+40, (driver->getScreenSize().Height)/2+10));
										driver->beginScene(true, true, SColor(0,122,65,171));
										env->drawAll();
										driver->endScene();
										u32 startTime=timer->getRealTime();
										while(startTime+5000>timer->getRealTime())
										{
											driver->beginScene(true, true, SColor(0,122,65,171));
											env->drawAll();
											driver->endScene();
										}
										//rebuild login area
										loginbackground = env->addImage(driver->getTexture("images/loginbg.jpg"),position2d<int>(0,0), false, 0, -1);
										globaltxtbox1= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2));
										globaltxtbox2= env->addEditBox(L"", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+40, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+40));
										env->addButton(rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2-25, (driver->getScreenSize().Height)/2+80),0,1,L"Login");
										env->addButton(rect<s32>((driver->getScreenSize().Width)/2+25,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+80),0,2,L"Quit");
										env->addButton(rect<s32>((driver->getScreenSize().Width)/2-20,(driver->getScreenSize().Height-40)/2+80, (driver->getScreenSize().Width)/2+20, (driver->getScreenSize().Height)/2+80),0,3,L"Register");
										env->addStaticText(L"Username", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2-20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2-20));
										env->addStaticText(L"Password", rect<s32>((driver->getScreenSize().Width)/2-50,(driver->getScreenSize().Height-40)/2+20, (driver->getScreenSize().Width)/2+50, (driver->getScreenSize().Height)/2+20));
										driver->beginScene(true, true, SColor(0,122,65,171));
										env->drawAll();
										driver->endScene();
										WSACleanup();
									}
								}
							}
						}
			}else if(keys[13]==RELEASED){
				keys[13]=UP;
			}
			driver->beginScene(true, true, SColor(0,122,65,171));
			env->drawAll();
			driver->endScene();
		}
}*/