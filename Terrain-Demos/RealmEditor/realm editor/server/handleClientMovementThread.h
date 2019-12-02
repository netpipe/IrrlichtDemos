//*************************//
//Handle for client updates//
//This runs in a thread, one for each client, updates their position on server, does attack checks, map changes, and updates clients
//*************************//
void *HandleClientMovement(void *ptr) 
{
	//get starting data, used in the loop
	int sock=(int) ptr;
	char buffer[BUFFSIZE];
	int received = 1, oldmapx, oldmapy;
	timeval newtime,oldtime;
	double deltaT=0, lastUpdate=0, globalqForce=0;
	//globalPlayers[sock].speed=5;
	
	int counter=0;
	//loop untill player is unloaded (id=-1) or server stops
	while (globalPlayers[sock].loaded&&runServer) 
	{
		//get time to find change in time, move based off of that time
		oldtime=newtime;
		gettimeofday(&newtime,NULL);
		double time= (.001*newtime.tv_usec)+(newtime.tv_sec*1000);
		double otime= (.001*oldtime.tv_usec)+(oldtime.tv_sec*1000);
		deltaT =(time-otime);
		//update local player list
		getLocalPlayersFromMap(globalPlayers[sock].localPlayersR,globalPlayers[sock].mapx,globalPlayers[sock].mapy, globalPlayers[sock].mapz, 2,2,1, sock, 1);
		//************
		//attack check
		//************
		//check if their is a target set
		
		if(globalPlayers[sock].targetId!=-1)
		{
			//check the type of target, human or npc
			if(globalPlayers[sock].targetType==0)
			{
				//HUMAN
				//get index position if not already found, used to quickly find in local player list
				if(globalPlayers[sock].targetIndex==-1)
				{
					for(int x=0;x<globalPlayers[sock].localPlayersR.size();x++)
					{
						if(globalPlayers[globalPlayers[sock].localPlayersR[x]].id==globalPlayers[sock].targetId)
						{
							//store index
							globalPlayers[sock].targetIndex=globalPlayers[sock].localPlayersR[x];
							x=globalPlayers[sock].localPlayersR.size();
						}
					}
				}
				if(globalPlayers[sock].targetKey==1)
				{
					//control pressed
					//aggressive (attack)
					//check if player is close enough
					//get distance from player NOT SQUARE ROOTED! saves time, target radius stuff must be squared aswell.
					double distance=getCrossMapDistance(globalPlayers[sock].posx,globalPlayers[sock].posy,globalPlayers[sock].posz,globalPlayers[globalPlayers[sock].targetIndex].posx,globalPlayers[globalPlayers[sock].targetIndex].posy,globalPlayers[globalPlayers[sock].targetIndex].posz,globalPlayers[sock].mapx,globalPlayers[sock].mapy,globalPlayers[sock].mapz,globalPlayers[globalPlayers[sock].targetIndex].mapx,globalPlayers[globalPlayers[sock].targetIndex].mapy,globalPlayers[globalPlayers[sock].targetIndex].mapz);
					//check if close enough and enough time has passed
					if(distance<globalPlayers[sock].attackRadius&&newtime.tv_usec>=globalPlayers[sock].lastAttackTime+globalPlayers[sock].attackDelay)
					{
						//get damage
						int damage=attack(globalPlayers[sock].damage, globalPlayers[globalPlayers[sock].targetIndex].defense);
						//subtract damage from health
						globalPlayers[globalPlayers[sock].targetIndex].curhp-=damage;
						//send health change to all near players
						for(int x=0;x<globalPlayers[sock].localPlayersR.size();x++)
						{
							sendHealth(globalPlayers[sock].localPlayersR[x], 1, globalPlayers[globalPlayers[sock].targetIndex].id, globalPlayers[globalPlayers[sock].targetIndex].curhp);
							distance=getCrossMapDistance(globalPlayers[globalPlayers[sock].targetIndex].posx,globalPlayers[globalPlayers[sock].targetIndex].posy,globalPlayers[globalPlayers[sock].targetIndex].posz,globalPlayers[globalPlayers[sock].localPlayersR[x]].posx,globalPlayers[globalPlayers[sock].localPlayersR[x]].posy,globalPlayers[globalPlayers[sock].localPlayersR[x]].posz,globalPlayers[globalPlayers[sock].targetIndex].mapx,globalPlayers[globalPlayers[sock].targetIndex].mapy,globalPlayers[globalPlayers[sock].targetIndex].mapz,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapx,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapy,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapz);
							//if within 75 units (distance is not square rooted so 75^2 is used
							if(distance<5625)
							{
								//send text for health change
							}
						}
					}
				}
				else if(globalPlayers[sock].targetKey==0)
				{
					//no key
					//friendly
				}
			}else
			{
				//NPC
				//check if npc is close enough
				if(globalPlayers[sock].targetIndex==-1)
				{
					for(int x=0;x<npcsystem.size();x++)
					{
						if(npcsystem[x].getId()==globalPlayers[sock].targetId)
						{
							globalPlayers[sock].targetIndex=x;
							x=npcsystem.size();
						}
					}
				}cout<<"GOOD"<<endl;
				switch(npcsystem[globalPlayers[sock].targetIndex].getFactionId())
				{
				case 0:
					//monster faction, not friendly ever
					//aggressive
					//check if player is close enough
					//get distance from player NOT SQUARE ROOTED! saves time, target radius stuff must be squared aswell.
					double posx=0, posy=0, posz=0;
					int mapx=0, mapy=0, mapz=0;
					npcsystem[globalPlayers[sock].targetIndex].getPositionInfo(posx,posy,posz,mapx,mapy,mapz);
					double distance=getCrossMapDistance(globalPlayers[sock].posx,globalPlayers[sock].posy,globalPlayers[sock].posz,posx,posy,posz,globalPlayers[sock].mapx,globalPlayers[sock].mapy,globalPlayers[sock].mapz,mapx,mapy,mapz);
					//check if close enough and enough time has passed
					cout<<"GOOD2:"<<distance<<endl;
					cout << globalPlayers[sock].posx<< " " <<globalPlayers[sock].posy<< " " <<globalPlayers[sock].posz<< " " <<posx<< " " <<posy<< " " <<posz<< " " <<globalPlayers[sock].mapx<< " " <<globalPlayers[sock].mapy<< " " <<globalPlayers[sock].mapz<< " " <<mapx<< " " <<mapy<< " " <<mapz<<endl;
					if(distance<globalPlayers[sock].attackRadius&&newtime.tv_usec>=globalPlayers[sock].lastAttackTime+globalPlayers[sock].attackDelay)
					{
						cout<<"DAMAGE"<<endl;
						//attack to be done later
						//get damage
						int damage=attack(globalPlayers[sock].damage, npcsystem[globalPlayers[sock].targetIndex].getDefense());
						//subtract damage from health
						npcsystem[globalPlayers[sock].targetIndex].updateHealth(damage);
						//send health change to all near players
						
						for(int x=0;x<globalPlayers[sock].localPlayersR.size();x++)
						{
							sendHealth(globalPlayers[sock].localPlayersR[x], 0, npcsystem[globalPlayers[sock].targetIndex].getId(), npcsystem[globalPlayers[sock].targetIndex].getHealth());
							distance=getCrossMapDistance(posx,posy,posz,globalPlayers[globalPlayers[sock].localPlayersR[x]].posx,globalPlayers[globalPlayers[sock].localPlayersR[x]].posy,globalPlayers[globalPlayers[sock].localPlayersR[x]].posz,mapx,mapy,mapz,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapx,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapy,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapz);
							//if within 75 units (distance is not square rooted so 75^2 is used
							if(distance<5625)
							{
								//send text for health change
							}
						}
					}
					break;
				}
			}
		}
		//****************
		//end attack check
		//****************

		//update player position on server
		//new position=old position+movement rate*speed*change in time/1000 (get to ms)
		if(globalPlayers[sock].movx!=0)
		{
			globalPlayers[sock].posx+=globalPlayers[sock].movx*globalPlayers[sock].speed*(deltaT/1000);
		}
		if(globalPlayers[sock].movy!=0)
		{
			globalPlayers[sock].posy+=globalPlayers[sock].movy*globalPlayers[sock].speed*(deltaT/1000);
		}
		if(globalPlayers[sock].movz!=0)
		{
			globalPlayers[sock].posz+=globalPlayers[sock].movz*globalPlayers[sock].speed*(deltaT/1000);
		}

		//change map check
		//needs to be added, if pos > mapsize map++ pos=pos-mapsize, check for all maps and moving both directions

		for(int x=0;x<globalPlayers[sock].localPlayersR.size();x++)
		{
			//if Movement has changed then update
			//update is set to true when a movement change is received from the client
			if(globalPlayers[globalPlayers[sock].localPlayersR[x]].updateMov[sock])
			{
				//set update to false
				globalPlayers[globalPlayers[sock].localPlayersR[x]].updateMov[sock]=false;
				//send movement info to the client
sendMov(globalPlayers[globalPlayers[sock].localPlayersR[x]].id, 
globalPlayers[globalPlayers[sock].localPlayersR[x]].posx+(globalPlayers[globalPlayers[sock].localPlayersR[x]].movx*globalPlayers[globalPlayers[sock].localPlayersR[x]].speed*globalq), 
globalPlayers[globalPlayers[sock].localPlayersR[x]].posy+(globalPlayers[globalPlayers[sock].localPlayersR[x]].movy*globalPlayers[globalPlayers[sock].localPlayersR[x]].speed*globalq),
globalPlayers[globalPlayers[sock].localPlayersR[x]].posz+(globalPlayers[globalPlayers[sock].localPlayersR[x]].movz*globalPlayers[globalPlayers[sock].localPlayersR[x]].speed*globalq), 
globalPlayers[globalPlayers[sock].localPlayersR[x]].rotation,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapx,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapy,
globalPlayers[globalPlayers[sock].localPlayersR[x]].mapz, 
sock);
			}
		}
	}//end while
	pthread_exit(NULL);
}
//declare threads for handling clients
pthread_t clients2[maxconnections];
