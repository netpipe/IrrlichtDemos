//multithreaded, handles a single client
//handles data received from clients
void *HandleClient(void *ptr) 
{
	//set start data
	int sock=(int) ptr;
	int received = 1;
	time_t lastMov=time(NULL)+((globalq-1)*1000), newTime;
	//loop untill socket closes or error occurs, or server stops
	while (received > 0&&runServer) 
	{
		//if loaded a character (logged in)
		if(globalPlayers[sock].id>-1)
		{
			//get time
			newTime=time(NULL);
			//if enough time has passed
			if(lastMov<newTime)
			{
				//update lastMov time
				lastMov=newTime+((globalq-1)*1000);
				//update movement for everyone
				for(int q=0;q<maxconnections;q++)
				{
					globalPlayers[sock].updateMov[q]=true;
				}
			}
		}
		//receive data
		char buffer[BUFFSIZE]="";
		//if an error occured
		if ((received = recv(clientsock[sock], buffer, BUFFSIZE, 0)) < 0) 
		{
			//error occured, loop will end due to condition
		    //end while loop, an error occured in connection
			break;
		} else 
		{
			//received data, time to decide what to do with it
			//display data
			cout << inet_ntoa(echoclient[sock].sin_addr) << " sent: " << buffer<< endl;
			//get function id by breaking up the code using the | as break points
			stringc tobreak;
			tobreak.append(buffer);
			stringc funccode;
			int strpos = tobreak.find("|");
			int strpos2;
			funccode=tobreak.subString(0,strpos);
			//print function id
			cout << funccode.c_str()<<'\n';
			//use the correct function
			switch(atoi(funccode.c_str()))
			{	
			case 1: 
				{
				//EVERYTHING TO DO WITH LOGIN.
				//PASSWORD, USER NAME, AND OTHER STUFF
				//login
				printf("logging in");
				ifstream playerfile;
				stringc loginData[3];
				//break the received info and store in loginData
				breakCode(tobreak, loginData, 3);
				//get the file location
				stringc userfile;
				userfile.append("database/players/");
				userfile.append(loginData[1].c_str());
				//open player file
				cout << "Opening file: " << userfile.c_str() << endl;
				playerfile.open(userfile.c_str());
				if(!playerfile.is_open())
				{
					//if file fails to open return code 1 0 wrong pw or not registered
					stringc toclient="1|0|";
					//add header
					addMessageHeader(toclient);
					//send
					send(clientsock[sock], toclient.c_str(), toclient.size(),0);
					cout << "opening file failed, must register"<< endl;
				} else 
				{
					//account is registered, check password
					cout << "file opened getting data" << endl;
					char filepass[21];
					//get password from file
					std::string infromfile;
					//get id
					getline(playerfile, infromfile);
					globalPlayers[sock].id=atoi(infromfile.c_str());
					//get pass
					getline(playerfile, infromfile);
					stringc filepassstr=infromfile.c_str();
					//check pass from the file with one from client
					if(filepassstr==loginData[2])
					{
						// passwords match, log them in
						// load data into character info class
						globalPlayers[sock].loadFromFile(userfile.c_str());
						// if name ==new send 2 else 1
						// if 2 wait for more info (character creation screen launches
						// if 1 send player info to client
						stringc toclient="1|1|";
						//add header
						addMessageHeader(toclient);
						//send
						send(clientsock[sock], toclient.c_str(), toclient.size(),0);
						//send char info
						globalPlayers[sock].sendStart(sock);
						//make thread to handle movement, and things
						pthread_create( &clients2[sock], NULL, &HandleClientMovement, (void*) sock);
						//add player to map system
						virtualMapSystem[globalPlayers[sock].mapx+middleMapX][globalPlayers[sock].mapy+middleMapY][globalPlayers[sock].mapz+middleMapZ].addPlayer(sock);
						cout << "login successful"<<endl;
					} else 
					{
						//passwords don't match, login failed return code 0001 0, code for wrong pw or username
						stringc toclient="1|0|";
						//add header
						addMessageHeader(toclient);
						//send data
						send(clientsock[sock], toclient.c_str(), toclient.size(),0);
						cout <<"wrong pw"<<endl;
					}
				}
				break;
				}
			case 2:
				{
				//REGISTRATION
				//USER CREATES USERNAME AND PASSWORD WITH EMAIL ADDRESS
				printf("registering");
				//break data from client
				stringc data[4];
				breakCode(tobreak, data, 4);
				//get file location
				ifstream playerFile;
				stringc databasestr="database/players/";
				databasestr.append(data[1]);
				playerFile.open(databasestr.c_str());
				//try to open file
				if(playerFile.is_open())
				{
					//account username exists
					stringc toclient="2|0|";
					//add header
					addMessageHeader(toclient);
					//send data
					send(clientsock[sock], toclient.c_str(), toclient.size(), 0);
					//close the file
					playerFile.close();
				}else{
					//username not in use
					//check email list
					//create the player file
					ofstream newplayer;
					newplayer.open(databasestr.c_str());
					//write the new id to the file
					newplayer<< globalplayerid << '\n';
					//update id
					globalplayerid++;
					//saves id file, makes sure not to have same id's even if server crashes
					saveId();
					//build player file
					newplayer<<data[2].c_str()<< '\n'<< data[3].c_str()<< '\n'<<data[1].c_str() << '\n';
					newplayer<< "new\n0\n0\n10\n10\n10\n10\n0\n0\n0\n0\n0\n0\n0\n0\n0\n1\n1\n1\n100\n2\n0\n0\n0\n0\n0\n0\n100\n100";
					//build player class with start info
					globalPlayers[sock].loadFromFile(databasestr);
					//send char creation page (later)
					//for now send login info
					//close file
					newplayer.close();
					//must say I don't know what the below code is for... I will try removing it later and see if it changes anything
					stringc toclient;
					addMessageHeader(toclient);
					send(clientsock[sock], toclient.c_str(), toclient.size(),0);
				}
				break;
				}
			case 3:
				{
					//MOVEMENT id, (Position:) x, y, z, rotation, (movVec:) x, y, z
					//break the code up, and set vars in player class
					stringc brokencode[8];
					breakCode(tobreak, brokencode,8);
					globalPlayers[sock].posx=atoi(brokencode[1].c_str());
					globalPlayers[sock].posy=atoi(brokencode[2].c_str());
					globalPlayers[sock].posz=atoi(brokencode[3].c_str());
					globalPlayers[sock].rotation=atoi(brokencode[4].c_str());
					globalPlayers[sock].movx=atof(brokencode[5].c_str());
					globalPlayers[sock].movy=atof(brokencode[6].c_str());
					globalPlayers[sock].movz=atof(brokencode[7].c_str());
					//update last movement time
					lastMov=newTime+((globalq-1)*1000);
					//tell everyone they need to update
					for(int q=0;q<maxconnections;q++)
					{
						globalPlayers[sock].updateMov[q]=true;
					}
					break;
				}
			case 4:
				{
				//STATS
				break;
				}
			case 5:
				{
				//EQUIPMENT
				break;
				}
			case 6:
				{
				//MOUNTS
				break;
				}
			case 7:
				{
				//SPELLS
				break;
				}
			case 8:
				{
				//SKILLS
				break;
				}
			case 9:
				{
					
					//TARGETING
					//working on this still
					stringc brokencode[4];
					breakCode(tobreak, brokencode,4);
					globalPlayers[sock].targetId=atoi(brokencode[1].c_str());
					globalPlayers[sock].targetType=atoi(brokencode[2].c_str());
					globalPlayers[sock].targetKey=atoi(brokencode[3].c_str());
					globalPlayers[sock].targetIndex=-1;
					cout <<"targetting "<< globalPlayers[sock].targetId <<endl;
					//targetKey 
					//0=no keys
					//1=ctrl
					//decide how to handle and send data back
					if(globalPlayers[sock].targetType==1)
					{
						//human
						if(globalPlayers[sock].targetKey==1)
						{
							//control pressed
							//aggressive (attack)
						}
						else if(globalPlayers[sock].targetKey==0)
						{
							//no key
							//friendly
						}
					}
					else if(globalPlayers[sock].targetType==2)
					{
						//npc
						//needs faction check
						//find targetIndex
						for(int x=0;x<npcsystem.size();x++)
						{
							if(npcsystem[x].getId()==globalPlayers[sock].targetId)
							{
								globalPlayers[sock].targetIndex=x;
							}
						}
						switch(npcsystem[globalPlayers[sock].targetIndex].getFactionId())
						{
						case 0:
							//monster faction, not friendly ever
							//aggressive
							break;
						}
					}
					break;
				}
			case 10:
				{
				//SCROLL OVER ID
				break;
				}
			case 11:
				{
					//button move confirm
					stringc brokencode[5];
					//break up the code
					breakCode(tobreak, brokencode,5); 
					//put the code into there vars
					int fromType=atoi(brokencode[1].c_str());
					int toType=atoi(brokencode[3].c_str());
					int fromId=atoi(brokencode[2].c_str());
					int toId=atoi(brokencode[4].c_str());
					//type list
					//4 equipment
					//1 inventory
					if(toType==4)
					{
						//to equipment
						if(fromType==4)
						{
							//from equipment
							//swap if of same item type
							//so rings can swap or move to each other if empty
						}
						else if(fromType==1)
						{
							//from inventory
							//check reqs, swap or replace if empty
							if(globalPlayers[sock].equipmentList[toId]==0)
							{
								//no item in slot, check reqs
								if(globalPlayers[sock].checkReqs(fromId,1))
								{
									//equip
									globalPlayers[sock].equip(fromId,1);
									globalPlayers[sock].equipmentList[toId]=globalPlayers[sock].inventory[fromId];
									globalPlayers[sock].inventory[fromId]=0;
									updateInventory(fromType, fromId, 0, sock);
									updateInventory(toType, toId, globalPlayers[sock].equipmentList[toId], sock);
								}
							}
							else
							{
								//item in slot swap?
							}
						}
					}
					else if(toType==1)
					{
						//move to inventory
						//swap if possible if slot taken do and can't equip, don't swap
						if(fromType==4)
						{
							//de-equip
							globalPlayers[sock].deequip(fromId,0);
							if(globalPlayers[sock].inventory[toId]==0)
							{
								//nothing in slot move
								globalPlayers[sock].inventory[toId]=globalPlayers[sock].equipmentList[fromId];
								globalPlayers[sock].equipmentList[fromId]=0;
								updateInventory(fromType, fromId, 0, sock);
								updateInventory(toType, toId, globalPlayers[sock].inventory[toId], sock);
							}
							else
							{
								//item in slot swap?
							}
						}
						else if(fromType==1)
						{
							//move from inv to inv
							int item1=globalPlayers[sock].inventory[fromId];
							int item2=globalPlayers[sock].inventory[toId];
							globalPlayers[sock].inventory[fromId]=item2;
							globalPlayers[sock].inventory[toId]=item1;
							updateInventory(fromType, fromId, globalPlayers[sock].inventory[fromId], sock);
							updateInventory(toType, toId, globalPlayers[sock].inventory[toId], sock);
						}
					}
					break;
				}
			case 12:
				{
					//CHAT Local
					stringc brokencode[2];
					breakCode(tobreak, brokencode,2);
					//check if admin code by breaking at first space and check if first part is /admin
					//if it is then break the rest of the code as needed based off of the next word, quit, add, remove, kick, ban, etc.
					if(brokencode[1]=="/admin quit server")
					{
						printf("quitting server");
						runServer=false;
					}
					else if(brokencode[1]=="/admin quit server")
					{
						printf("quitting server");
						runServer=false;
					}
					else
					{
						//send to players in local list for now. Later will send to either local list or to members of channel based off of its Id
						for(int x=0;x<globalPlayers[sock].localPlayersR.size();x++)
						{
							//send
							sendChat(globalPlayers[sock].id,globalPlayers[sock].inGameName,brokencode[1],globalPlayers[sock].localPlayersR[x]);
						}
						//send to player himself, so they know they said something
						sendChat(globalPlayers[sock].id,globalPlayers[sock].inGameName,brokencode[1],sock);
					}
					break;
				}
			case 13:
				{
				//CHAT Channel
				break;
				}
			case 14:
				{
				//CHAT Whisper
				break;
				}
			case 15:
				{
				//CHAT Clan
				break;
				}
			case 16:
				{
					//item request
					//check if item is in active database
					cout <<"ITEM REQUESTED"<<endl;
					int uid=0;
					//break code
					stringc brokencode[2];
					breakCode(tobreak, brokencode,2);
					//get id
					uid=atoi(brokencode[1].c_str());
					//send the item to the sock, checks in sendItem if it is loaded
					itemSys.sendItem(uid, sock);
					break;
				}
			case 100:
				{
					//get version ID from client
					cout <<"got version\n";
					stringc brokencode[2];
					breakCode(tobreak, brokencode,2);
					//check if is latest version
					if(brokencode[1]==versionId)
					{
						//up to date
						cout <<"up tp date\n";
						//tell the client they are good to go
						stringc sendCode="105|";
						//add header
						addMessageHeader(sendCode);
						//send
						send(clientsock[sock], sendCode.c_str(), sendCode.size(),MSG_NOSIGNAL);
					}else
					{
						//get files needed to upload
						vector<stringc> fileList;
						//get next version they need
						int currentVersion=atoi(brokencode[1].c_str())+1;
						cout <<"updating: "<< currentVersion <<"\n";
						while(currentVersion<=versionId)
						{
							//open the version file and get the files from it
							fstream fileIn;
							stringc fileLocation;
							char fileLocChar[200];
							sprintf(fileLocChar,"database/patcher/%i",currentVersion);
							fileLocation=fileLocChar;
							fileIn.open(fileLocation.c_str());
							//open file
							if(fileIn.is_open())
							{
								//loop till end of file
								while(!fileIn.eof())
								{						
									char fileOut[200];
									fileIn.getline(fileOut,200);
									stringc fileOutStr=fileOut;
									//add file to list
									fileList.push_back(fileOutStr);
									//if file was a blank line remove it from the list
									if(fileList[fileList.size()-1]=="")
									{
										fileList.pop_back();
									}
								}
							}
							//go to next version
							currentVersion++;
						}
						//send number of files and new version
						stringc sendCode;
						char convert[200];
						int totalSize=0;
						sprintf(convert,"100|%i|%i|",fileList.size(),versionId);
						sendCode.append(convert);
						//add header
						addMessageHeader(sendCode);
						//send
						send(clientsock[sock], sendCode.c_str(), sendCode.size(),MSG_NOSIGNAL);
						cout <<"got files\n";
						// remove duplicate files
						vector<stringc> fileCheck(0);
						bool found=false;
						//loop through all files
						for(int x=0;x<fileList.size();x++)
						{
							//check if in the fileCheck vector
							for(int y=0;y<fileCheck.size();y++)
							{
								//if it is say it was found
								if(fileList[x]==fileCheck[y])
								{
									found=true;
								}
							}
							//if it was not found add it to the list
							if(!found)
							{
								fileCheck.push_back(fileList[x]);
							}
						}
						//save new list without multiples
						fileList=fileCheck;
						cout <<"sending "<< fileList.size()<<" files\n";
						//start sending files
						int failCounter=0;
						//loop through each file
						for(int x=0;x<fileList.size();x++)
						{
							bool fileSent;
							cout <<"sending "<< fileList[x].c_str()<<"\n";
							//send the file
							sendFile(sock, fileList[x], fileSent);
							//wait until received confirmation
							received = recv(clientsock[sock], buffer, BUFFSIZE, 0);
							if(received==-1)
							{
								//error occured, stop sending
								x=fileList.size();
								failCounter=6;
							}
							//break code
							stringc sendCheck=buffer;
							breakCode(sendCheck, brokencode,2);
							//code 0 from client, failed, anything else means it was good, so go onto the next
							if(brokencode[1]==0)
							{
								//resend
								x--;
								failCounter++;
							}
							//do a failing check
							if(failCounter>5)
							{
								//failed 5 times something isn't working right
								x=fileList.size();
								//send error occured
							}
						}
						//done sending
						
						if(failCounter<6)
						{
						sendCode="105|";
						addMessageHeader(sendCode);
						send(clientsock[sock], sendCode.c_str(), sendCode.size(),MSG_NOSIGNAL);
						}
					}
					cout <<"sent files\n";
					break;
				}

			}//END SWITCH
		} 
	}//end while loop
	//save
	globalPlayers[sock].savePlayer();
	//reset globalPlayers slot
	
	//state the socket is closing
	cout << "Closing Socket: "<< inet_ntoa(echoclient[sock].sin_addr) << endl;
	//close socket
	close(clientsock[sock]);
	//open socket for new connections
	socketsused[sock]=0;
	//remove player from map system
	virtualMapSystem[globalPlayers[sock].mapx+middleMapX][globalPlayers[sock].mapy+middleMapY][globalPlayers[sock].mapz+middleMapZ].removePlayer(sock);
	//open player position
	globalPlayers[sock].id=-1;	
	globalPlayers[sock].loaded=false;
	//exit thread
	pthread_exit(NULL);
}

//threads for each client
pthread_t clients[maxconnections];
