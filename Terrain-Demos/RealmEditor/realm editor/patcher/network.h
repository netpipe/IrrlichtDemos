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
						//*************
						//patcher calls
						//*************
						case 100:
							{
								//get number of files, or up to date, new version id
								stringc brokenString[3];
								breakCode(serverString, brokenString,3);
								if(brokenString[1]=="0")
								{
									//up to date, add play button
									envId=103;
									buildNewEnv=true;
									//ShellExecute(GetDesktopWindow(), "open", "rofh.exe", NULL, NULL, SW_SHOWNORMAL);
									//runloop=false;
								}else{
									//get variables
									totalFiles=atoi(brokenString[1].c_str());
									newVersionId=atoi(brokenString[2].c_str());
								}
								break;
							}
						case 101:
							{
								//get news, title, post date, description
								stringc brokenString[4];
								breakCode(serverString, brokenString,4);
								break;
							}
						case 102:
							{
								//file info
								stringc brokenString[4];
								breakCode(serverString, brokenString,4);
								fileSize=atoi(brokenString[1].c_str());
								currentSize=0;
								fileLocation=brokenString[2].c_str();
								fileLocationFinal=brokenString[3].c_str();
								fileContents="";
								fileContents.reserve(fileSize);
								fileContents="";
								break;
							}
						case 103:
							{
								//file contents
								stringc brokenString[2];
								breakCode(serverString, brokenString,2,false);
								printf("serverString size: %i\nadding %i to %s\n",serverString.size(), brokenString[0].size()+5, brokenString[0].c_str());
								fileInString="";//=serverString.subString(brokenString[0].size()+brokenString[1].size(),atoi(brokenString[0].c_str())-(brokenString[0].size()+brokenString[1].size()));
								printf("fileInString size: %i\n",fileInString.size());
								int x=0;
								for(x=brokenString[0].size()+5;x<atoi(brokenString[0].c_str())-1;x++)
								{
									fileInString.append(serverString[x]);
								}
								printf("fileInString size: %i    X: %i\n",fileInString.size(),x);
								//printf("fileInString: %s\n",fileInString.c_str());
								currentSize+=fileInString.size();
								fileContents.append(fileInString);
								printf("current file size: %i\n",fileContents.size());
								break;
							}
						case 104:
							{
								//end of file
								
								if(currentSize==fileSize)
								{
									//save file
									ofstream saveFile;
									saveFile.open(fileLocation.c_str(),ios::binary);
									if(saveFile.is_open())
									{
										printf("saving file\nname: %s  :: size: %i\n", fileLocation.c_str(), fileContents.size());
										//save the new file
										char* charForm;
										charForm= new char[fileContents.size()];
										for(int x=0;x<fileContents.size();x++)
										{
											charForm[x]=fileContents[x];
										}
										for(int x=0;x<fileContents.size();x++)
										{
											saveFile << *(charForm+x);
										}
										
										saveFile.close();
										//decompress the file
										En_Decode Coder;
										Coder.SetInputFileName(fileLocation.c_str(),"");
										Coder.SetOutputFileName(fileLocationFinal.c_str());
										Coder.Decode();
										//send you got it
										stringc sendCode="101|1|";
										send(serverSocket,sendCode.c_str(), sendCode.size(),NULL);
									}else
									{
										//failed to save file, send error screen
										envId=104;
										buildNewEnv=true;
									}
								}else
								{
									printf("not all of file\n");
									//send failed to get all of file, retry
									stringc sendCode="101|0|";
									send(serverSocket,sendCode.c_str(), sendCode.size(),NULL);
									failCounter++;
								}
								break;
							}
						case 105:
							{
								//up to date, add play button
								//save new id
								ofstream saveFile;
								saveFile.open("idFile.txt");
								if(saveFile.is_open())
								{
									char idChar[20];
									stringc idString;
									sprintf(idChar,"%i",newVersionId);
									idString=idChar;
									saveFile<<idString.c_str();
								}
								saveFile.close();
								envId=103;
								buildNewEnv=true;
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
		envId=100;
		buildNewEnv=true;
		//pauseLength=5000;
		//pauseScreen=true;
		//if not connected
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != NO_ERROR)
		{
			//report error occured on computer
			envId=101;
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
				envId=101;
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
					envId=102;
					buildNewEnv=true;
				}else{
					//report error occured on connecting to server
					envId=101;
					buildNewEnv=true;
					pauseLength=5000;
					pauseScreen=true;
					WSACleanup();
				}
			}
		}
		recvdata = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)recvdatafunc, NULL, 0, &thid);
		//send version id
		char sendChar[200];
		stringc sendCode;
		sprintf(sendChar,"100|%i|",versionId);
		sendCode=sendChar;
		send(serverSocket,sendCode.c_str(), sendCode.size(),NULL);
	}
}