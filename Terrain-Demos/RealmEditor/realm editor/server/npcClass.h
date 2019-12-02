//***************************************************************************************
//NPC CLASS
//stores all npc data for every npc
//***************************************************************************************
class npcClass
{
public:
void updateHealth(int dhp);
void update();
void sendStartData(int sock);
void sendUpdate(int sock);
void loadFromFile(int fileId);
void setSpawnData(double iposx, double iposy, double iposz, double imapx, double imapy, double imapz, double ifreq, bool temp);
bool getSpawnData(stringc &dataOut);
void respawn();
int getId();
int getFactionId();
npcClass();
int getDefense();
int getHealth();
void getPositionInfo(double &oposx, double &oposy, double &oposz, int &omapx, int &omapy, int &omapz);
private:
int id,modelId, health, maxhealth, damage, defense, level, equipment[20],  expDrop, goldDrop, mapx, mapy, mapz, smapx, smapy, smapz,type,  targetId, targetType, dataFileId, factionId;
vector<int> localPlayerList;
vector<int> skillList;
vector<int> skillFreqList;
vector<int> dropList;
vector<int> dropFreqList;
double posx, posy, posz, rotation, attackRadius, targetRadius, moveRadius, spawnx, spawny, spawnz, visibility, speed, attackSpeed, skillFreq, lastAttackTime, lastSkillTime, movx, movy, movz, lastRegen, regenRate, spawnFreq;
double lastUpdate, lastMovTime, lastSend;
bool moveable, talkable, temporary, loaded;
stringc name, title, description;
};
void npcClass::getPositionInfo(double &oposx, double &oposy, double &oposz, int &omapx, int &omapy, int &omapz)
{
oposx=posx;
oposy=posy;
oposz=posz;
omapx=mapx;
omapy=mapy;
omapz=mapz;
}
int npcClass::getHealth()
{
	//gets defense
	return health;
}
int npcClass::getDefense()
{
	//gets defense
	return defense;
}
int npcClass::getId()
{
	//gets id
	return id;
}
int npcClass::getFactionId()
{
	//gets factionId
	return factionId;
}
npcClass::npcClass()
{
	//constructor for npc class, can be used for things
}
void npcClass::respawn()
{
	//respawns an npc, resets health to max, and position to spawn point, and movement to nothing and target to nothing
health=maxhealth;
mapx=smapx;
mapy=smapy;
mapz=smapz;
posx=spawnx;
posy=spawny;
posz=spawnz;
movx=0;
movy=0;
movz=0;
targetId=-1;
}
void npcClass::updateHealth(int dhp)
{
	//updates health, reversed! subtracts dhp from health, pass negative to increase health
	health-=dhp;
}

void npcClass::update()
{
	//updates the npc
	if(loaded)
	{
		//make sure npc is loaded otherwise it could be bad to run this
		//get the change in time from last update
		timeval newTime;
		gettimeofday(&newTime, NULL);
		double time= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
		double dtime=time-lastUpdate;
		//make sure lastMovTime is set
		if(lastMovTime==0)
		{
			lastMovTime=time;
		}
		//make sure lastSend is set
		if(lastSend==0)
		{
			lastSend=time;
		}
		//reset lastUpdate
		lastUpdate=time;
		//update localList
		getLocalPlayersFromMap(localPlayerList,mapx,mapy, mapz, 2,2,1, id, 2);
		//random movement check
		if(moveable&&targetId==-1)
		{
			//update every 5 seconds
			if(time>lastMovTime+10000)
			{
				//reset lastMovTime
				lastMovTime=time;
				//move the npc
				//check if too far from spawn
				double distance =getCrossMapDistance(posx, posy,posz,spawnx,spawny,spawnz,mapx,mapy,mapz,smapx,smapy,smapz);
				if(distance>=moveRadius)
				{
					//get vector to spawn
					getCrossMapVector(posx, posy,posz,spawnx,spawny,spawnz,mapx,mapy,mapz,smapx,smapy,smapz, movx,movy,movz);
					//add to get mov between 0 and 1
					double totalVector=movx+movy+movz;
					//divide by total to get the % per say, 0-1
					movx/=totalVector;
					movy/=totalVector;
					movz/=totalVector;
				}
				else
				{
					//move randomly
					int randomnum=rand();
					//get degrees
					randomnum=randomnum%360;
					rotation=randomnum;
					//get mov
					movx=cos(randomnum);
					movz=sin(randomnum);
					movy=0;
				}
			}
		}
		//check if target is set
		if(targetId!=-1)
		{
			//check if still in range
		}
		//move the npc
		posx+=movx*speed*dtime;
		posy+=movy*speed*dtime;
		posz+=movz*speed*dtime;
		//check if enough time has passed to regen
		if(lastUpdate>lastRegen+regenRate)
		{
			//regenfunction
		}
		//update users in local list if its been x ms
		if(time>lastSend+5000)
		{
			//reset lastSend
			lastSend=time;
			//go through all players in local list
			for(int x=0;x<localPlayerList.size();x++)
			{
				//send them an update
				sendUpdate(localPlayerList[x]);
			}
		}
	}
}

void npcClass::sendUpdate(int sock)
{
	//send update of position and movement and rotation, health
	//sends position in 5 seconds, clients move npc to that position
	//makes it an auto correcting system, if client is too far off in one position
	//then it moves them back on target
	stringc sendData="";
	//populate sendData with all needed data
	float fsecx=0, fsecy=0, fsecz=0;
	//calculate pos in 5 seconds
	fsecx=movx*speed*5;
	fsecy=movy*speed*5;
	fsecz=movz*speed*5;
	char charbuffer[500];
	sprintf(charbuffer,"29|%i|%i|%f|%f|%f|%i|%i|%i|%f|", id, health,  fsecx, fsecy, fsecz, mapx,mapy,mapz,rotation);
	sendData=charbuffer;
	//add header info
	addMessageHeader(sendData);
	//send data
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

void npcClass::sendStartData(int sock)
{
	stringc sendData="";
	//populate sendData with all needed data
	float fsecx=0, fsecy=0, fsecz=0;
	//calculate pos in 5 sec
	//get faction name
	stringc factionName="";
	//build equipment list
	stringc equiplist="";
	char charbufferas[50000];
	sprintf(charbufferas,"28|%i|%i|%i|%i|%f|%f|%f|%f|%f|%f|%i|%i|%i|%i|%s|%s|%s|%i|%f|%f|%s|%i|", id, health, maxhealth, modelId, posx, posy, posz, fsecx, fsecy, fsecz, mapx,mapy,mapz,factionId,  factionName.c_str(), name.c_str(), title.c_str(), level, rotation, visibility, equiplist.c_str(), type);
	sendData=charbufferas;
	//add header info
	addMessageHeader(sendData);
	//send data
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

void npcClass::loadFromFile(int fileId)
{
	//naturally this loads an npc from a file
	//get file string from fileId
	dataFileId=fileId;
	ifstream loadFile;
	stringc fileString="";
	char fileChar[200];
	sprintf(fileChar,"database/monsters/%i",fileId);
	fileString=fileChar;
	//open file
	loadFile.open(fileString.c_str());
	if(loadFile.is_open())
	{
		//get data from file, each var is on its own line
		//below gets each var and stores it, I will make a
		//list to show file order on the forums link is below
		//http://rofhonline.com/forums/viewtopic.php?f=8&t=201
		std::string fileData;
		getline(loadFile,fileData);
		id=atoi(fileData.c_str());
		getline(loadFile,fileData);
		name=fileData.c_str();
		getline(loadFile,fileData);
		title=fileData.c_str();
		getline(loadFile,fileData);
		description=fileData.c_str();
		getline(loadFile,fileData);
		maxhealth=atoi(fileData.c_str());
		getline(loadFile,fileData);
		factionId=atoi(fileData.c_str());
		getline(loadFile,fileData);
		modelId=atoi(fileData.c_str());
		getline(loadFile,fileData);
		int eqtot=atoi(fileData.c_str());
		for(int x=0;x<eqtot;x++)
		{
				getline(loadFile,fileData);
				int eqpos=atoi(fileData.c_str());
				getline(loadFile,fileData);
				equipment[eqpos]=atoi(fileData.c_str());
		}
		getline(loadFile,fileData);
		int sktot=atoi(fileData.c_str());
		for(int x=0;x<sktot;x++)
		{
				getline(loadFile,fileData);
				skillList.push_back(atoi(fileData.c_str()));
		}
		for(int x=0;x<sktot;x++)
		{
				getline(loadFile,fileData);
				skillFreqList.push_back(atoi(fileData.c_str()));
		}
		getline(loadFile,fileData);
		skillFreq=atof(fileData.c_str());
		getline(loadFile,fileData);
		attackSpeed=atof(fileData.c_str());
		getline(loadFile,fileData);
		speed=atof(fileData.c_str());
		getline(loadFile,fileData);
		defense=atoi(fileData.c_str());
		getline(loadFile,fileData);
		damage=atoi(fileData.c_str());
		getline(loadFile,fileData);
		int droptot=atof(fileData.c_str());
		for(int x=0;x<droptot;x++)
		{
				getline(loadFile,fileData);
				dropList.push_back(atoi(fileData.c_str()));
		}
		for(int x=0;x<droptot;x++)
		{
				getline(loadFile,fileData);
				dropFreqList.push_back(atoi(fileData.c_str()));
		}
		getline(loadFile,fileData);
		level=atoi(fileData.c_str());
		getline(loadFile,fileData);
		expDrop=atoi(fileData.c_str());
		getline(loadFile,fileData);
		visibility=atof(fileData.c_str());
		getline(loadFile,fileData);
		moveRadius=atof(fileData.c_str());
		getline(loadFile,fileData);
		attackRadius=atof(fileData.c_str());
		getline(loadFile,fileData);
		targetRadius=atof(fileData.c_str());
		getline(loadFile,fileData);
		moveable=atoi(fileData.c_str());
		getline(loadFile,fileData);
		int chattot=atoi(fileData.c_str());
		for(int x=0;x<chattot;x++)
		{
				//getline(loadFile,fileData);
				//chatList.push_back(atoi(fileData.c_str()));
		}
		//init some vars
		loaded=true;
		lastMovTime=0;
		lastSend=0;
	}
}

void npcClass::setSpawnData(double iposx, double iposy, double iposz, double imapx, double imapy, double imapz, double ifreq, bool temp)
{
	//sets spawn data, map and pos, aswell as frequency and if the npc is temporary or permanent, allows for events and such
	//this is taken from the spawn file, or from an admin command if creating a spawn point
	spawnx=iposx;
	spawny=iposy;
	spawnz=iposz;
	smapx=imapx;
	smapy=imapy;
	smapz=imapz;
	spawnFreq=ifreq;
	temporary=temp;
}

bool npcClass::getSpawnData(stringc &dataOut)
{
	//used to get spawn data to save to file
	//if the npc is temporary return false 
	if(temporary)
	{
		return false;
	}
	else
	{
		//build a string and set dataOut to it, includes npc file Id, and spawn data as seen in sprintf function
		char buf[500];
		sprintf(buf,"%i\n%f\n%f\n%f\n%i\n%i\n%i\n%f\n", dataFileId, spawnx, spawny, spawnz,smapx,smapy,smapz,spawnFreq);
		dataOut=buf;
		return true;
	}
}
//set up npcsystem vector to store all npcs
vector<npcClass> npcsystem(0);

