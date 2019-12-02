//SERVER MAIN FILE
//VERSION 0.2

#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include "irrlicht.h"
#include <fstream>
#include <vector>
//#include "libIrrlicht.a"
//#pragma comment(lib, "libIrrlicht.a")

#define MAXPENDING 10    /* Max connection requests */
#define BUFFSIZE 500
#define maxconnections 100
void Die(char *mess) { perror(mess); exit(1); }

int clientsock[maxconnections];//sockets to clients
int socketsused[maxconnections];//keeps track if the socket is in use
struct sockaddr_in echoclient[maxconnections];//holds information for each socket in use
using namespace std;
using namespace irr;
using namespace core;

float mapMaxSide=100, vertexDistance=.5;
float mapSize=240;
int serversock, versionId=0;
bool runServer=true;
long globalplayerid=0;
long globalitemid=0;
long globalmountid=0;
long globalmonsterid=0;
long globalnpcid=0;
double globalq=5; //position after globalq seconds is sent to other clients so they move there, also used to force an
//update if too much time has elapsed
time_t globalUpdateDelay =.1; // Seconds delayed before updating the client
void getLocalPlayersFromMap(vector<int> &currentList, int mapx, int mapy, int mapz, int dx, int dy, int dz, int currentUser, int type);
void addMessageHeader(stringc &netMessage);
/**
	
 * C++ version char* style "itoa":
	
 */	
char* itoa( int value, char* result, int base ) {
	
	// check that the base if valid
	
	if (base < 2 || base > 16) { *result = 0; return result; }
	

	
	char* out = result;
	
	int quotient = value;
	

	
	do {
	
		*out = "0123456789abcdef"[ std::abs( quotient % base ) ];
	
		++out;
	
		quotient /= base;
	
	} while ( quotient );
	

	
	// Only apply negative sign for base 10
	
	if ( value < 0 && base == 10) *out++ = '-';
	

	
	std::reverse( result, out );
	
	*out = 0;
	
	return result;
	
}
//get SQUARED distance across maps
double getCrossMapDistance(double aposx,double aposy,double aposz,double bposx,double bposy,double bposz, int amapx, int amapy, int amapz, int bmapx, int bmapy, int bmapz)
{
//get map differences
	if(amapx!=bmapx)
	{
		if(amapx>bmapx)
		{
		aposx+=240*abs(amapz-bmapz);
		}
		else
		{
		bposx+=240*abs(amapz-bmapz);
		}
	}
	if(amapy!=bmapy)
	{
		if(amapy>bmapy)
		{
		aposy+=240*abs(amapz-bmapz);
		}
		else
		{
		bposy+=240*abs(amapz-bmapz);
		}
	}
	if(amapz!=bmapz)
	{
		if(amapz>bmapz)
		{
		aposz+=240*abs(amapz-bmapz);
		}
		else
		{
		bposz+=240*abs(amapz-bmapz);
		}
	}
//get distance from corrected positions
return (pow((aposx-bposx),2)+pow((aposy-bposy),2)+pow((aposz-bposz),2));
}

//get vector between to points across maps from a to b
void getCrossMapVector(double aposx,double aposy,double aposz,double bposx,double bposy,double bposz, int amapx, int amapy, int amapz, int bmapx, int bmapy, int bmapz, double &vecx, double &vecy, double &vecz)
{
//get map differences
	if(amapx!=bmapx)
	{
		if(amapx>bmapx)
		{
		aposx+=240*abs(amapz-bmapz);
		}
		else
		{
		bposx+=240*abs(amapz-bmapz);
		}
	}
	if(amapy!=bmapy)
	{
		if(amapy>bmapy)
		{
		aposy+=240*abs(amapz-bmapz);
		}
		else
		{
		bposy+=240*abs(amapz-bmapz);
		}
	}
	if(amapz!=bmapz)
	{
		if(amapz>bmapz)
		{
		aposz+=240*abs(amapz-bmapz);
		}
		else
		{
		bposz+=240*abs(amapz-bmapz);
		}
	}
vecx=bposx-aposx;
vecy=bposy-aposy;
vecz=bposz-aposz;
}
//**********************************
// ITEM CLASS
//**********************************

class itemClass
{
public:
void loadFromFile(int fileId);
void sendData(int sock);
void saveFile();
int getId();
double getLastUsed();
void unloadItem();
itemClass();
void copyData(double &damage, double &defense);
private:
int id, type, eqslot, iconId, modelId, openSlots, quality, setId, cost;
stringc name, description;
double damage, defense, lastUsed;
int lastUsedI;
bool usable, stackable, quest, multiOwner, loaded;
//orbs
//reqs
vector<int> reqs[2];
};

void itemClass::copyData(double &damageOut, double &defenseOut)
{
damageOut=damage;
defenseOut=defense;
}

itemClass::itemClass()
{
lastUsed=0;
name="";
description="";
id=-1;
type=0;
eqslot=0;
iconId=0;
modelId=0;
openSlots=0;
quality=0;
setId=0;
cost=0;
damage=0;
defense=0;
usable=false;
stackable=false;
quest=false;
multiOwner=false;
loaded=false;
lastUsedI=1;
}
void itemClass::unloadItem()
{
//save item
saveFile();
//remove from active memory
type=0;eqslot=-1;iconId=-1;modelId=-1;openSlots=0;quality=0;setId=0;cost=0;
name="DNE";description="DNE";
damage=0; defense=0;lastUsed=0;
usable=false;stackable=false;quest=false;multiOwner=false;
//mark as open slot
id=-1;
loaded=false;
}

void itemClass::sendData(int sock)
{
//get time
cout <<"SENDING ITEM1"<<endl;
timeval newTimea;
gettimeofday(&newTimea, NULL);
cout <<"SENDING ITEM2"<<endl;
double time= ((.001*newTimea.tv_usec)+(newTimea.tv_sec*1000));
cout <<id<<endl;
cout <<"SENDING ITEM3"<<endl;
lastUsed=time;
cout <<"SENDING ITEM4"<<endl;
if(loaded)
{
	stringc sendData="";
	//populate sendData with all needed data
	char charbuffer[500];
	cout <<"SENDING ITEM"<<endl;
	sprintf(charbuffer,"30|%i|%i|%i|%i|%i|%i|%i|%i|%s|%f|%f|%i|", id, type,  modelId, iconId, openSlots, setId,cost,quality,name.c_str(), damage, defense, quest);
	sendData=charbuffer;
	//add header info
	addMessageHeader(sendData);
	//send data
	cout <<"SENDING ITEM"<<endl;
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
	//send description
	sprintf(charbuffer,"31|%i|%s|", id, description.c_str());
	sendData=charbuffer;
	//add header info
	cout <<"SENDING ITEM"<<endl;
	addMessageHeader(sendData);
	//send data
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
	cout <<"SENT ITEM"<<endl;
}
}

void itemClass::saveFile()
{
ofstream itemFile;
stringc dataFileS;
char dataFile[200];
sprintf(dataFile,"database/items/%i.rofh", id);
dataFileS=dataFile;
itemFile.open(dataFileS.c_str());
if(itemFile.is_open())
{
	itemFile << type;
	itemFile << '\n';
	itemFile << eqslot;
	itemFile << '\n';
	itemFile << iconId;
	itemFile << '\n';
	itemFile << modelId;
	itemFile << '\n';
	itemFile << openSlots;
	itemFile << '\n';
	itemFile << quality;
	itemFile << '\n';
	itemFile << setId;
	itemFile << '\n';
	itemFile << cost;
	itemFile << '\n';
	itemFile << name.c_str();
	itemFile << '\n';
	itemFile << description.c_str();
	itemFile << '\n';
	itemFile << damage;
	itemFile << '\n';
	itemFile << defense;
	itemFile << '\n';
	itemFile << usable;
	itemFile << '\n';
	itemFile << stackable;
	itemFile << '\n';
	itemFile << quest;
	itemFile << '\n';
	itemFile << multiOwner;
	itemFile << '\n';
	//reqs	
	itemFile << reqs[0].size();
	itemFile << '\n';
	for(int x=0;x<reqs[0].size();x++)
	{
	itemFile << reqs[0][x];
	itemFile << '\n';
	itemFile << reqs[1][x];
	itemFile << '\n';
	}
	//orbs
	itemFile << '0';
	itemFile << '\n';
itemFile.close();
}
}

double itemClass::getLastUsed()
{
return lastUsed;
}

void itemClass::loadFromFile(int fileId)
{
cout << "loading item "<< fileId <<endl;
//get time
timeval newTime;
gettimeofday(&newTime, NULL);
double time= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
lastUsed=time;
//load from file
ifstream itemFile;
char dataFile[200];
sprintf(dataFile, "database/items/%i.rofh", fileId);
stringc dataFileS=dataFile;
itemFile.open(dataFileS.c_str());
id=fileId;
	if(itemFile.is_open())
	{
cout << "file open"<<endl;
	
	std::string infromfile;
	getline(itemFile, infromfile);
	type=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	eqslot=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	iconId=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	modelId=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	openSlots=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	quality=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	setId=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	cost=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	name=infromfile.c_str();
	getline(itemFile, infromfile);
	description=infromfile.c_str();
	getline(itemFile, infromfile);
	damage=atof(infromfile.c_str());
	getline(itemFile, infromfile);
	defense=atof(infromfile.c_str());
	getline(itemFile, infromfile);
	usable=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	stackable=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	quest=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	multiOwner=atoi(infromfile.c_str());
	getline(itemFile, infromfile);
	int numReqs=atoi(infromfile.c_str());
for(int x=0;x<numReqs;x++)
{
	getline(itemFile, infromfile);
	reqs[0].push_back(atoi(infromfile.c_str()));
	getline(itemFile, infromfile);
	reqs[1].push_back(atoi(infromfile.c_str()));
}

	itemFile.close();
	loaded=true;
	}
else
{
//file does not exist default options
type=0;eqslot=-1;iconId=-1;modelId=-1;openSlots=0;quality=0;setId=0;cost=0;
name="DNE";description="DNE";
damage=0; defense=0;lastUsed=0;
usable=false;stackable=false;quest=false;multiOwner=false;
loaded=true;
}
}

int itemClass::getId()
{
return id;
}





//***********************************
class itemSystem
{
public:
int findItem(int uid);
itemClass getData(int position);
void sendItem(int uid, int sock);
void update();
private:
vector<itemClass> items;
};
itemClass itemSystem::getData(int position)
{
return items[position];
}
int itemSystem::findItem(int uid)
{
	int x=-1;
	for(x=0;x<items.size();x++)
	{
		if(items[x].getId()==uid)
		{
			return x;
		}
	}
//item does not exist in list add new item and refind
cout <<"new item"<<endl;
itemClass newItem;
newItem.loadFromFile(uid);
items.push_back(newItem);
	for(x=items.size();x>0;x--)
	{
		if(items[x-1].getId()==uid)
		{
			return x-1;
		}
	}
return -1;
}

void itemSystem::sendItem(int uid, int sock)
{
	int loc=findItem(uid);
cout <<"loc " << loc<<endl;
	items[loc].sendData(sock);
}

void itemSystem::update()
{
	//get time
	timeval newTime;
	gettimeofday(&newTime, NULL);
	double time= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
	for(int x=0;x<items.size();x++)
	{
		if(items[x].getLastUsed()+600000<time)
		{
			//unload after 10 mins
			items[x].unloadItem();
		}
	}
}

itemSystem itemSys;

/****************************************************
// PLAYER CLASS
****************************************************/

class playerClass
{
public:
void moveInventory(int location1, int location2);
void equip(int slot, int loc);
void deequip(int slot, int loc);
bool checkReqs(int slot, int loc);
int getAttackDamage();
playerClass();
stringc email, inGameName, userName, password, title;
double  speed, posx, posy, posz, rotation, qsecposx, qsecposy, qsecposz, lastAttackTime, lastSkillTime, attackRadius, attackDelay, damage, defense;
float movx, movy, movz;
int level, clan, str, dex, con, curhp, maxhp, intel, expOn, expTot, skillLevel, unusedSkillPoints, equipmentList[19], inventory[40], mountId, raceId, modelId,pvp, showeq, visibility,mapx, mapy, mapz, rotationDir, id, targetType,targetId, targetIndex, targetKey;
bool criminal, jump, loaded, mounted, updateMov[maxconnections], updatePos[maxconnections];
vector<int> localPlayersR, skillList, faction[2];
};
int playerClass::getAttackDamage()
{
return damage;
}

playerClass::playerClass()
{
damage=0;
defense=0;
}
void playerClass::equip(int slot, int loc)
{
	//add bonuses and stats from item located in loc
	if(loc==0)
	{
		//equipmentList
		double damageOut=0, defenseOut=0;
		itemSys.getData(itemSys.findItem(equipmentList[slot])).copyData(damageOut, defenseOut);
		damage+=damageOut;
		defense+=defenseOut;
	}
	else if(loc==1)
	{
		//inventory
		double damageOut=0, defenseOut=0;
		itemSys.getData(itemSys.findItem(inventory[slot])).copyData(damageOut, defenseOut);
		damage+=damageOut;
		defense+=defenseOut;
	}
}
void playerClass::deequip(int slot, int loc)
{
	//remove bonuses and stats from item in loc
	if(loc==0)
	{
		//equipmentList
		double damageOut=0, defenseOut=0;
		itemSys.getData(itemSys.findItem(equipmentList[slot])).copyData(damageOut, defenseOut);
		damage-=damageOut;
		defense-=defenseOut;
	}
	else if(loc==1)
	{
		//inventory
		double damageOut=0, defenseOut=0;
		itemSys.getData(itemSys.findItem(inventory[slot])).copyData(damageOut, defenseOut);
		damage-=damageOut;
		defense-=defenseOut;
	}
}
bool playerClass::checkReqs(int slot, int loc)
{
	//check reqs for item in loc
return true;
}
void playerClass::moveInventory(int location1, int location2)
{
//move slot1 to slot2, swap if needed, combine if possible
if(inventory[location2]!=-1)
{
//item in slot 2
if(inventory[location1]==inventory[location2])
{
//same id, combineable
}
else
{
//swap
int holder=inventory[location2];
inventory[location2]=inventory[location1];
inventory[location1]=holder;
}
}else
{
//no item in slot 2 just move
inventory[location2]=inventory[location1];
inventory[location1]=-1;
}
} 

//***************************************************************************************
//NPC CLASS
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
npcClass();
private:
int id,modelId, health, maxhealth, damage, defense, level, equipment[20],  expDrop, goldDrop, mapx, mapy, mapz, smapx, smapy, smapz, targetId, targetType, dataFileId, type, factionId;
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
npcClass::npcClass()
{

}
void npcClass::respawn()
{
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
health-=dhp;
}

void npcClass::update()
{
if(loaded)
{

timeval newTime;
gettimeofday(&newTime, NULL);
double time= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
double dtime=time-lastUpdate;
if(lastMovTime==0)
{
lastMovTime=time;
}
if(lastSend==0)
{
lastSend=time;
}
lastUpdate=time;
	//update localList
	getLocalPlayersFromMap(localPlayerList,mapx,mapy, mapz, 2,2,1, id, 2);
	//random movement check
	if(moveable&&targetId==-1)
	{
		//update every 5 seconds
		if(time>lastMovTime+10000)
		{
		lastMovTime=time;
		//move the npc
		//check if too far from spawn
		double distance =getCrossMapDistance(posx, posy,posz,spawnx,spawny,spawnz,mapx,mapy,mapz,smapx,smapy,smapz);
			if(distance>=moveRadius)
			{
			//get vector to spawn
			getCrossMapVector(posx, posy,posz,spawnx,spawny,spawnz,mapx,mapy,mapz,smapx,smapy,smapz, movx,movy,movz);
			double totalVector=movx+movy+movz;
			movx/=totalVector;
			movy/=totalVector;
			movz/=totalVector;
			}else
			{
			//move randomly
			int randomnum=rand();
			randomnum=randomnum%360;
			rotation=randomnum;
			movx=cos(randomnum);
			movz=sin(randomnum);
			movy=0;
			}
		}
	}
	if(targetId!=-1)
	{
		//check if still in range
	}
//mov the npc
//posx+=movx*speed*dtime;
//posy+=movy*speed*dtime;
//posz+=movz*speed*dtime;
if(lastUpdate>lastRegen+regenRate)
{
//regenfunction
}
//update users in local list
if(time>lastSend+5000)
{
lastSend=time;
for(int x=0;x<localPlayerList.size();x++)
{
	sendUpdate(localPlayerList[x]);
}
}
}
}

void npcClass::sendUpdate(int sock)
{
	stringc sendData="";
	//populate sendData with all needed data
	float fsecx=0, fsecy=0, fsecz=0;
	//calculate pos in 5 sec
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
dataFileId=fileId;
ifstream loadFile;
stringc fileString="";
char fileChar[200];
sprintf(fileChar,"database/monsters/%i",fileId);
fileString=fileChar;
loadFile.open(fileString.c_str());
if(loadFile.is_open())
{
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
	loaded=true;
	lastMovTime=0;
	lastSend=0;
}
}

void npcClass::setSpawnData(double iposx, double iposy, double iposz, double imapx, double imapy, double imapz, double ifreq, bool temp)
{
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
if(temporary)
{
return false;
}else
{
char buf[500];
sprintf(buf,"%i\n%f\n%f\n%f\n%i\n%i\n%i\n%f\n", dataFileId, spawnx, spawny, spawnz,smapx,smapy,smapz,spawnFreq);
dataOut=buf;
return true;
}
}

vector<npcClass> npcsystem(0);

//***************************************************************************************
// MAP CLASS
//***************************************************************************************
class mapClass
{
public:
int id, numPlayers, weatherId;
int* locations, spawnPoints;
vector <int> players;
void addPlayer(int socket);
void removePlayer(int socket);
mapClass();
};
mapClass::mapClass()
{
id=0; 
numPlayers=0; 
weatherId=0;
players.resize(0);
printf("%i: ", numPlayers);
}
void mapClass::addPlayer(int socket)
{
players.push_back(socket);
numPlayers++;
}
void mapClass::removePlayer(int socket)
{
//remove the given socket
vector<int> newplayers(0);
for(int x=0;x<players.size();x++)
{
	if(players[x]!=socket)
	{
		newplayers.push_back(players[x]);
	}
}
players=newplayers;
numPlayers--;
}

//init global map class
vector<vector<vector<mapClass> > > virtualMapSystem;
int middleMapX=0,middleMapY=0,middleMapZ=0;

//***************************************************************************************

//init global player variable
playerClass globalPlayers[maxconnections];

// add custom message header to outgoing messages
void addMessageHeader(stringc &netMessage)
{
//add size to front
int startSize=netMessage.size();
bool sizeChanged=false;
if(startSize+2<10&&!sizeChanged)
{
startSize+=2;
sizeChanged=true;
}else if(startSize+3<100&&!sizeChanged)
{
startSize+=3;
sizeChanged=true;
}else if(startSize+4<1000&&!sizeChanged)
{
startSize+=4;
sizeChanged=true;
}else if(startSize+5<10000&&!sizeChanged)
{
startSize+=5;
sizeChanged=true;
}
char sizeAttach[10];
sprintf(sizeAttach,"%i|",startSize);
stringc sizeAttachment=sizeAttach;
sizeAttachment.append(netMessage);
netMessage=sizeAttachment;
}
void sendInventory(playerClass player,  int sock);
//builds player class from given file
void buildPlayerClass(playerClass &thePlayerClass, stringc dataFile)
{
	ifstream playerFile;
	playerFile.open(dataFile.c_str());
	if(playerFile.is_open())
	{
printf("buildPlayerClass");
		std::string infromfile;
		getline(playerFile, infromfile);
		thePlayerClass.id=atoi(infromfile.c_str());
		
		getline(playerFile, infromfile);
		thePlayerClass.password.append(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.email.append(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.userName.append(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.inGameName.append(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.title.append(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.clan=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.str=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.dex=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.con=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.intel=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.level=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.pvp=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.expOn=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.expTot=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.skillLevel=atoi(infromfile.c_str());
		int skillCount=0;
		getline(playerFile, infromfile);
		skillCount=atoi(infromfile.c_str());
		thePlayerClass.skillList.resize(skillCount);
		for(int x=0;x<skillCount;x++)
		{
				getline(playerFile, infromfile);
				thePlayerClass.skillList[x,0]=atoi(infromfile.c_str());
				getline(playerFile, infromfile);
				thePlayerClass.skillList[x,1]=atoi(infromfile.c_str());
		}
		int eqCount=0;
		getline(playerFile, infromfile);
		eqCount=atoi(infromfile.c_str());
		for(int x=0;x<eqCount;x++)
		{
			int eqPos=0;
			getline(playerFile, infromfile);
			eqPos=atoi(infromfile.c_str());
			getline(playerFile, infromfile);
			thePlayerClass.equipmentList[eqPos]=atoi(infromfile.c_str());
		}
		int invCount=0;
		getline(playerFile, infromfile);
		invCount=atoi(infromfile.c_str());
		for(int x=0;x<invCount;x++)
		{
			int invPos=0;
		getline(playerFile, infromfile);
		invPos=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.inventory[invPos]=atoi(infromfile.c_str());
		}
		getline(playerFile, infromfile);
		thePlayerClass.mountId=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.raceId=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.modelId=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.showeq=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.visibility=atoi(infromfile.c_str());;
		getline(playerFile, infromfile);
		thePlayerClass.speed=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.mapx=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.mapy=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.mapz=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.posx=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.posy=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.posz=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.curhp=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		thePlayerClass.maxhp=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		int factionTot=atoi(infromfile.c_str());
		for(int x=0;x<factionTot;x++)
		{
			getline(playerFile, infromfile);
			thePlayerClass.faction[0].push_back(atoi(infromfile.c_str()));
			getline(playerFile, infromfile);
			thePlayerClass.faction[1].push_back(atoi(infromfile.c_str()));
		}
		thePlayerClass.jump=false;
		thePlayerClass.loaded=true;
	}
}

//send stat data to client code 10
void sendStat(double statid, double newNumber, int sock)
{
	stringc sendData;
	sendData="10|";
	sendData.append(statid);
	sendData.append('|');
	sendData.append(newNumber);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send skill data to client code 11
void sendSkill(double skillid, double newNumber, int sock)
{
	stringc sendData;
	sendData="11|";
	sendData.append(skillid);
	sendData.append('|');
	sendData.append(newNumber);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send health data to client code 12
void sendHP(double hpType, double newHealth, int sock)
{
	stringc sendData;
	sendData="12|";
	sendData.append(hpType);
	sendData.append('|');
	sendData.append(newHealth);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send start data to client code 13
void sendStart(playerClass thePlayer, int sock)
{
	//player info list
	stringc sendData;
	sendData="13|";
	sendData.append(thePlayer.inGameName.c_str());
	sendData.append('|');
	sendData.append(thePlayer.title);
	sendData.append('|');
	char thebuffer[100];
	sendData.append(itoa(thePlayer.id,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mapx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mapy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mapz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.posx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.posy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.posz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.level,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.intel,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.con,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.dex,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.str,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.rotation,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.raceId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mountId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.modelId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.expOn,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.expTot,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.skillLevel,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.unusedSkillPoints,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.pvp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.showeq,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.visibility,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.criminal,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.maxhp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.curhp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mounted,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.speed,thebuffer,10));
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
	//skill list

	//equipment list
	sendInventory(thePlayer, sock);
}

//send exp data to client code 14
void sendEXP(double expType, double newNumber, int sock)
{
	stringc sendData;
	sendData="14|";
	sendData.append(expType);
	sendData.append('|');
	sendData.append(newNumber);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send skill timer data to client code 15
void sendSkillTimer(double skillid, double curTime, double endTime, int sock)
{
	stringc sendData;
	sendData="15|";
	sendData.append(skillid);
	sendData.append('|');
	sendData.append(curTime);
	sendData.append('|');
	sendData.append(endTime);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send skill use data to client code 16
void sendSkillUse(double skillid, int sock)
{
	stringc sendData;
	sendData="16|";
	sendData.append(skillid);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send damage data to client code 17
void sendDamage(double damageType, double damageTargetId, double damageAmount, int sock)
{
	stringc sendData;
	sendData="17|";
	sendData.append(damageType);
	sendData.append('|');
	sendData.append(damageTargetId);
	sendData.append('|');
	sendData.append(damageAmount);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send attack timer data to client code 18
void sendAttackTimer(double curTime, double endTime, int sock)
{
	stringc sendData;
	sendData="18|";
	sendData.append(curTime);
	sendData.append('|');
	sendData.append(endTime);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send animation data to client code 19
void sendAnimation(double targetId, double animationId, int sock)
{
	stringc sendData;
	sendData="19|";
	sendData.append(targetId);
	sendData.append('|');
	sendData.append(animationId);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send pos data to client code 20
void sendPos(double targetId, double targetX, double targetY, double targetZ,double targetMX,double targetMY, double rotation, int sock)
{
	stringc sendData;
	sendData="20|";
	char thebuffer[22];
	sendData.append(itoa(targetId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(targetX,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(targetY,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(targetZ,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(targetMX,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(targetMY,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(rotation,thebuffer,10));
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send rot data to client code 21
void sendRot(double targetId, double rotation, int sock)
{
	stringc sendData;
	sendData="21|";
	sendData.append(targetId);
	sendData.append('|');
	sendData.append(rotation);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send mov data to client code 22
void sendMov(double targetId, double qsecx, double qsecy, double qsecz,  double rotation, int mapx, int mapy, int mapz, int sock)
{
	stringc sendData;
	sendData="22|";
	char thebuffer[22];
	sendData.append(itoa(targetId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(qsecx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(qsecy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(qsecz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(rotation,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapz,thebuffer,10));
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send chat data to client code 23
void sendChat(double senderId, stringc senderName, stringc message, int sock)
{
	char thebuffer[22];
	stringc sendData;
	sendData="23|";
	sendData.append(itoa(senderId,thebuffer,10));
	sendData.append('|');
	stringc newmsg="[local - ";
	newmsg.append(senderName);
	newmsg.append("] ");
	newmsg.append(message);
	sendData.append(newmsg);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send admin data to client code 24
void sendAdmin(double adminId, stringc message,  int sock)
{
	stringc sendData;
	sendData="24|";
	sendData.append(adminId);
	sendData.append('|');
	sendData.append(message);
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send inventory data to client code 25 and 26
void sendInventory(playerClass player,  int sock)
{
	stringc sendData;
	sendData="25|";
	for(int x=0;x<19;x++)
	{
	//send equipment
	char item[400];
	sprintf(item,"%i|", player.equipmentList[x]);
	cout <<"e" << player.equipmentList[x]<< endl;
	sendData.append(item);
	}
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);

	sendData="26|";

	for(int x=0;x<40;x++)
	{
	//send inventory
	char item[800];
	sprintf(item,"%i|", player.inventory[x]);
	cout <<"i" << player.inventory[x]<< endl;
	sendData.append(item);
	}
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send player data to client code 27
void sendPlayer(playerClass thePlayer,  int sock, int from)
{
printf("@sendPlayer %i to %i @ ", from, sock);
	stringc sendData;
	sendData="27|";
	sendData.append(thePlayer.inGameName.c_str());
	sendData.append('|');
	sendData.append(thePlayer.title);
	sendData.append('|');
	char thebuffer[100];
	sendData.append(itoa(thePlayer.id,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mapx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mapy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mapz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.posx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.posy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.posz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.level,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.rotation,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.raceId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mountId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.modelId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.pvp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.showeq,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.visibility,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.criminal,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.maxhp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.curhp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.mounted,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(thePlayer.speed,thebuffer,10));
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}
// update item slot code 32
void updateInventory(int type, int loc, int id,  int sock)
{
	//change type at loc to id for client sock
	char buffer[300];
	sprintf(buffer, "32|%i|%i|%i|", type, loc, id);
	stringc sendData=buffer;
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

/****************************************/
/* End of send data to client functions */
/****************************************/


//breaks stringtobreak into numbreaks parts and stores it in brokenString[]
void breakCode(stringc stringtobreak, stringc brokenString[], int numbreaks)
{
	int strpos=0, strpos2=0;
	//print string to screen
	//cout << endl << stringtobreak.c_str() << endl;
	//loop untill you don't need to break it anymore
	for(int x=0;x<numbreaks;x++)
	{
		//get new start position
		strpos=strpos2;
		//get end position, first | symbol after the start position
		strpos2=stringtobreak.findNextString("|", strpos);
		//break string and store it in brokenString
		brokenString[x]=stringtobreak.subString(strpos, strpos2-strpos);
		//print start, end, and string to screen (debugging)
		//cout << "strpos " << strpos << " strpos2 " << strpos2 << " string " << brokenString[x].c_str() << endl;
		//add 1 to end position for next loop
		strpos2+=1;
	}//end for loop
}

int savePlayer(int sock)
{
printf("opening file ");
	ofstream playerFileS;
	stringc filetoopen="database/players/";
filetoopen.append(globalPlayers[sock].userName);
	playerFileS.open(filetoopen.c_str());
	if(playerFileS.is_open())
	{
printf("saving ");
		playerFileS << globalPlayers[sock].id;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].password.c_str();
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].email.c_str();
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].userName.c_str();
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].inGameName.c_str();
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].title.c_str();
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].clan;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].str;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].dex;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].con;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].intel;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].level;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].pvp;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].expOn;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].expTot;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].skillLevel;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].skillList.size();
		playerFileS << '\n';
		for(int x=0;x<globalPlayers[sock].skillList.size();x++)
		{
			playerFileS << globalPlayers[sock].skillList[x,0];
		playerFileS << '\n';
			playerFileS << globalPlayers[sock].skillList[x,1];
		playerFileS << '\n';
		}
		int eqCount=19;
		playerFileS << eqCount;
		playerFileS << '\n';
		for(int x=0;x<eqCount;x++)
		{
			int eqPos=x;
			playerFileS << eqPos;
		playerFileS << '\n';
			playerFileS << globalPlayers[sock].equipmentList[eqPos];
		playerFileS << '\n';
		}
		int invCount=40;
		playerFileS << invCount;
		playerFileS << '\n';
		for(int x=0;x<invCount;x++)
		{
			int invPos=x;
			playerFileS << invPos;
		playerFileS << '\n';
			playerFileS << globalPlayers[sock].inventory[invPos];
		playerFileS << '\n';
		}
		playerFileS << globalPlayers[sock].mountId;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].raceId;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].modelId;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].showeq;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].visibility;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].speed;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].mapx;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].mapy;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].mapz;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].posx;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].posy;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].posz;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].curhp;
		playerFileS << '\n';
		playerFileS << globalPlayers[sock].maxhp;
	}
}
int saveItem();
int saveMount();
void saveSpawns()
{
	//open spawn file
	ofstream spawnFile;
	spawnFile.open("database/spawnData");
	if(spawnFile.is_open())
	{
	//loop through monsters saving if not temporary
		for(int x=0;x<npcsystem.size();x++)
		{
			stringc dataOut="";
			bool temp=npcsystem[x].getSpawnData(dataOut);
			if(temp)
			{
			spawnFile<<dataOut.c_str();
			}
		}
	}
}
int saveId()
{
	ofstream idFile;
	stringc filetoopen="database/idfile.idfile.rofh";
	idFile.open(filetoopen.c_str());
	if(idFile.is_open())
	{
		idFile << globalplayerid;
		idFile << '\n';
		idFile << globalitemid;
		idFile << '\n';
		idFile << globalmountid;
		idFile << '\n';
		idFile << globalmonsterid;
		idFile << '\n';
		idFile << globalnpcid;
		idFile << '\n';
		idFile << versionId;
idFile << '\n';
		idFile << virtualMapSystem.size();
idFile << '\n';
		idFile << virtualMapSystem[0].size();
idFile << '\n';
		idFile << virtualMapSystem[0][0].size();
idFile << '\n';
		idFile << middleMapX;
idFile << '\n';
		idFile << middleMapY;
idFile << '\n';
		idFile << middleMapZ;
	}
}


void sendFile(int sock, stringc fileLoc, bool& fileSent)
{
stringc fileLocFull=fileLoc;
int length=fileLoc.findLast('.');
fileLoc=fileLoc.subString(0,length);
cout << fileLoc.c_str();
	char converter[200];
	int sendResults=0;
	ifstream fileIn;
	stringc fullFileLoc="database/patcher/";
	fullFileLoc.append(fileLoc);
	fileIn.open(fullFileLoc.c_str(), ios::binary);
	if(fileIn.is_open())
	{
		int actualSize=0;
		//send start info to client
		int begin=fileIn.tellg();
		fileIn.seekg(0,ios::end);
		int end=fileIn.tellg();
		fileIn.seekg(0,ios::beg);
		int sizeOfFile=end;
		stringc sendInfo;
		char sendCharInfo[200];
		sprintf(sendCharInfo,"102|%i|%s|%s|",sizeOfFile, fileLoc.c_str(), fileLocFull.c_str());
		sendInfo.append(sendCharInfo);
		addMessageHeader(sendInfo);
		send(clientsock[sock], sendInfo.c_str(), sendInfo.size(),MSG_NOSIGNAL);
		char* fromFile;
		fromFile=new char[sizeOfFile];
		fileIn.read(fromFile,sizeOfFile);
		//loop through file sending all content
		double deltaT=0;
		int indexPos=0;
		while(indexPos!=sizeOfFile&&sendResults!=-1)
		{
			while(deltaT<600000)
			{
			deltaT++;
			}
			deltaT=0;
			cout <<"a";
			stringc sendCode="103|";
			for(int x=0;x<200;x++)
			{
				sendCode.append(*(fromFile+indexPos));
				indexPos++;
				if(indexPos==sizeOfFile)
				{
					x=200;
				}
			}cout <<"b";
			//sendCode.append(converter);
			sendCode.append("|");
			//cout << sendCode.size()<<endl;
			actualSize+=sendCode.size()-5;
			addMessageHeader(sendCode);cout <<"c";
			cout << sendCode.size() << "d";
			sendResults=send(clientsock[sock], sendCode.c_str(), sendCode.size(),MSG_NOSIGNAL);
	
		}
		if(sendResults!=-1)
		{
			cout <<"actualSize: "<< actualSize<<'\n';
			delete[] fromFile;
			while(deltaT<600000)
			{
				deltaT++;
			}
			deltaT=0;
			//send end of file to client
			stringc sendCode="104|";
			addMessageHeader(sendCode);
			send(clientsock[sock], sendCode.c_str(), sendCode.size(),MSG_NOSIGNAL);
			fileSent=true;
			cout << "file send succesful\n";
		}
	}else{
		cout << "file send failed!\n";
		fileSent=false;
	}

}

//get local players from map system from given map and for certain distance x y z check against current list if populated
void getLocalPlayersFromMap(vector<int> &currentList, int mapx, int mapy, int mapz, int dx, int dy, int dz, int currentUser, int type)
{
vector<int> newList(0);
for(int x=-dx;x<=dx;x++)
for(int y=-dy;y<=dy;y++)
for(int z=-dz;z<=dz;z++)
	for(int q=0;q<virtualMapSystem[x+middleMapX+mapx][y+middleMapY+mapy][z+middleMapZ+mapz].players.size();q++)
	{
		if(virtualMapSystem[x+middleMapX+mapx][y+middleMapY+mapy][z+middleMapZ+mapz].players[q]!=currentUser)
		{
		newList.push_back(virtualMapSystem[x+middleMapX+mapx][y+middleMapY+mapy][z+middleMapZ+mapz].players[q]);
		}
	}

bool found=false;

for(int x=0;x<currentList.size();x++)
{
	found=false;
	for(int y=0;y<newList.size();y++)
	{
		if(currentList[x]==newList[y])
		{
		found=true;
		}
	}
	if(!found)
	{
		//send remove code
	}
}
for(int x=0;x<newList.size();x++)
{
	found=false;
	for(int y=0;y<currentList.size();y++)
	{
		if(newList[x]==currentList[y])
		{
		found=true;
		}
	}
	if(!found)
	{
		//send start code
		if(type==1)
		{
		sendPlayer(globalPlayers[newList[x]], currentUser,12);
		}else
		{
cout << "currentUser " << currentUser<< endl;
		npcsystem[currentUser].sendStartData(newList[x]);
		}
	}
}
currentList=newList;
//currentList.resize(0);
//for(int x=0;x<newList.size();x++)
//{
//currentList.push_back(newList[x]);
//}
}

pthread_t npcThread;
//**************************
//Handle for NPC updaters***
//**************************
void *HandleNPCUpdates(void *ptr)
{
int counter=0;
double time=0, oldtime=0;
timeval newTime;
gettimeofday(&newTime, NULL);
oldtime= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
time=oldtime;
while(runServer)
{
oldtime=time;
while(time-oldtime<50)
{
gettimeofday(&newTime, NULL);
time= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
}
	for(counter=0;counter<npcsystem.size();counter++)
	{
		npcsystem[counter].update();
	}
}
pthread_exit(NULL);
}


//*************************//
//Handle for client updates//
//*************************//
void *HandleClientMovement(void *ptr) 
{
	int sock=(int) ptr;
	char buffer[BUFFSIZE];
	int received = 1, oldmapx, oldmapy;
	timeval newtime,oldtime;
	double deltaT=0, lastUpdate=0, globalqForce=0;
	//globalPlayers[sock].speed=5;
	
int counter=0;
	while (globalPlayers[sock].id>-1&&runServer) 
	{
		oldtime=newtime;
		gettimeofday(&newtime,NULL);
		double time= (.001*newtime.tv_usec)+(newtime.tv_sec*1000);
		double otime= (.001*oldtime.tv_usec)+(oldtime.tv_sec*1000);
		deltaT =(time-otime);
getLocalPlayersFromMap(globalPlayers[sock].localPlayersR,globalPlayers[sock].mapx,globalPlayers[sock].mapy, globalPlayers[sock].mapz, 2,2,1, sock, 1);
	//************
	//attack check
	//************
	if(globalPlayers[sock].targetId!=-1)
	{
		if(globalPlayers[sock].targetType==0)
		{
			//get index position if not already found	
			if(globalPlayers[sock].targetIndex==-1)
			{
				for(int x=0;x<globalPlayers[sock].localPlayersR.size();x++)
				{
					if(globalPlayers[globalPlayers[sock].localPlayersR[x]].id==globalPlayers[sock].targetId)
					{
						//store index
						globalPlayers[sock].targetIndex=globalPlayers[sock].localPlayersR[x];
					}
				}
			}
			//check if player is close enough
		//get distance from player NOT SQUARE ROOTED! saves time
			double distance=getCrossMapDistance(globalPlayers[sock].posx,globalPlayers[sock].posy,globalPlayers[sock].posz,globalPlayers[globalPlayers[sock].targetIndex].posx,globalPlayers[globalPlayers[sock].targetIndex].posy,globalPlayers[globalPlayers[sock].targetIndex].posz,globalPlayers[sock].mapx,globalPlayers[sock].mapy,globalPlayers[sock].mapz,globalPlayers[globalPlayers[sock].targetIndex].mapx,globalPlayers[globalPlayers[sock].targetIndex].mapy,globalPlayers[globalPlayers[sock].targetIndex].mapz);
			if(distance<globalPlayers[sock].attackRadius&&newtime.tv_usec>=globalPlayers[sock].lastAttackTime+globalPlayers[sock].attackDelay)
			{
			}

		}else
		{
		//check if npc is close enough
		}
	}
	//****************
	//end attack check
	//****************

//update player position on server
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
//if moved on x axis
/*
if(globalPlayers[sock].posx<0)
{
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].removePlayer(sock);
globalPlayers[sock].mapx--;
globalPlayers[sock].posx+=mapMaxSide*vertexDistance;
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].addPlayer(sock);
}
else if(globalPlayers[sock].posx>mapMaxSide*vertexDistance)
{
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].removePlayer(sock);
globalPlayers[sock].mapx++;
globalPlayers[sock].posx-=mapMaxSide*vertexDistance;
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].addPlayer(sock);
}
//if moves on y axis
if(globalPlayers[sock].posy<0)
{
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].removePlayer(sock);
globalPlayers[sock].mapy--;
globalPlayers[sock].posy+=mapMaxSide*vertexDistance;
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].addPlayer(sock);
}
else if(globalPlayers[sock].posy>mapMaxSide*vertexDistance)
{
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].removePlayer(sock);
globalPlayers[sock].mapy++;
globalPlayers[sock].posy-=mapMaxSide*vertexDistance;
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].addPlayer(sock);
}
//if moved on z axis
if(globalPlayers[sock].posz<0)
{
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].removePlayer(sock);
globalPlayers[sock].mapz--;
globalPlayers[sock].posz+=mapMaxSide*vertexDistance;
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].addPlayer(sock);
}
else if(globalPlayers[sock].posz>mapMaxSide*vertexDistance)
{
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].removePlayer(sock);
globalPlayers[sock].mapz++;
globalPlayers[sock].posz-=mapMaxSide*vertexDistance;
globalMaps[globalPlayers[sock].mapx][globalPlayers[sock].mapy][globalPlayers[sock].mapz].addPlayer(sock);
}
*/
		//if globalUpdateDelay Seconds have gone by, update Client
		//if(lastUpdate+globalUpdateDelay<=newtime)
		//{
		//	lastUpdate=newtime;
			for(int x=0;x<globalPlayers[sock].localPlayersR.size();x++)
			{
				//if Movement has changed then update
				if(globalPlayers[globalPlayers[sock].localPlayersR[x]].updateMov[sock])
				{printf("yay");
					globalPlayers[globalPlayers[sock].localPlayersR[x]].updateMov[sock]=false;

cout << "5SEC: " << globalPlayers[globalPlayers[sock].localPlayersR[x]].movx * globalPlayers[globalPlayers[sock].localPlayersR[x]].speed * globalq << " :: MOVX: " << globalPlayers[globalPlayers[sock].localPlayersR[x]].movx << " :: globalq : " << globalq << " :: speed : " << globalPlayers[globalPlayers[sock].localPlayersR[x]].speed << endl;
					
sendMov(globalPlayers[globalPlayers[sock].localPlayersR[x]].id, 
globalPlayers[globalPlayers[sock].localPlayersR[x]].posx+(globalPlayers[globalPlayers[sock].localPlayersR[x]].movx*globalPlayers[globalPlayers[sock].localPlayersR[x]].speed*globalq), 
globalPlayers[globalPlayers[sock].localPlayersR[x]].posy+(globalPlayers[globalPlayers[sock].localPlayersR[x]].movy*globalPlayers[globalPlayers[sock].localPlayersR[x]].speed*globalq),
globalPlayers[globalPlayers[sock].localPlayersR[x]].posz+(globalPlayers[globalPlayers[sock].localPlayersR[x]].movz*globalPlayers[globalPlayers[sock].localPlayersR[x]].speed*globalq), 
globalPlayers[globalPlayers[sock].localPlayersR[x]].rotation,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapx,globalPlayers[globalPlayers[sock].localPlayersR[x]].mapy,
globalPlayers[globalPlayers[sock].localPlayersR[x]].mapz, 
sock);
				}
			}
		//}
	}//end while
	pthread_exit(NULL);
}

pthread_t clients2[maxconnections];
//multithreaded, handles a single client
void *HandleClient(void *ptr) 
{
	int sock=(int) ptr;
	int received = 1;
	time_t lastMov=time(NULL)+((globalq-1)*1000), newTime;
	while (received > 0&&runServer) 
	{
		if(globalPlayers[sock].id>-1)
		{
			newTime=time(NULL);
			if(lastMov<newTime)
			{
				lastMov=newTime+((globalq-1)*1000);
				for(int q=0;q<maxconnections;q++)
				{
					globalPlayers[sock].updateMov[q]=true;
				}
			}
		}
		//receive data
		char buffer[BUFFSIZE]="";
		if ((received = recv(clientsock[sock], buffer, BUFFSIZE, 0)) < 0) 
		{
		        //Die("Failed to receive additional bytes from client");
			//end while loop, an error occured in connection
			break;
		} else 
			{
			cout << inet_ntoa(echoclient[sock].sin_addr) << " sent: " << buffer<< endl;
			//get function id
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
				stringc userfile;
				userfile.append("database/players/");
				userfile.append(loginData[1].c_str());
				//open player file
				cout << "Opening file: " << userfile.c_str() << endl;
				playerfile.open(userfile.c_str());
				if(!playerfile.is_open())
				{
					//if file fails to open return code 0001 0 wrong pw or not registered
					stringc toclient="1|0|";
addMessageHeader(toclient);
					send(clientsock[sock], toclient.c_str(), toclient.size(),0);
					cout << "opening file failed, must register"<< endl;
				} else 
					{
					//account is registered, check password
					cout << "file opened getting data" << endl;printf("TEST");

					char filepass[21];
					//get password
					playerfile >> globalPlayers[sock].id;
					playerfile >> filepass;
					stringc filepassstr=filepass;
					if(filepassstr==loginData[2])
					{
						// passwords match, log them in
						// load data into character info class
						buildPlayerClass(globalPlayers[sock], userfile.c_str());
						// if name ==new send 2 else 1
						// if 2 wait for more info (character creation screen launches
						// if 1 send player info to client
						stringc toclient="1|1|";
addMessageHeader(toclient);
						send(clientsock[sock], toclient.c_str(), toclient.size(),0);
						//send char info
						sendStart(globalPlayers[sock], sock);
						pthread_create( &clients2[sock], NULL, &HandleClientMovement, (void*) sock);
						//add player to game
						virtualMapSystem[globalPlayers[sock].mapx+middleMapX][globalPlayers[sock].mapy+middleMapY][globalPlayers[sock].mapz+middleMapZ].addPlayer(sock);
						cout << "login successful"<<endl;
					} else 
						{
						//passwords don't match, login failed return code 0001 0
						stringc toclient="1|0|";
addMessageHeader(toclient);
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
				stringc data[4];
				breakCode(tobreak, data, 4);
				ifstream playerFile;
				stringc databasestr="database/players/";
				databasestr.append(data[1]);
				playerFile.open(databasestr.c_str());
				if(playerFile.is_open())
				{
					//account username exists
					stringc toclient="2|0|";
addMessageHeader(toclient);
					send(clientsock[sock], toclient.c_str(), toclient.size(), 0);
					playerFile.close();
				}else{
					//username not in use
					//check email list

					//create the player file
					ofstream newplayer;
					newplayer.open(databasestr.c_str());
					newplayer<< globalplayerid << '\n';
					globalplayerid++;
					saveId();
					newplayer<<data[2].c_str()<< '\n'<< data[3].c_str()<< '\n'<<data[1].c_str() << '\n';
					newplayer<< "new\n0\n0\n10\n10\n10\n10\n0\n0\n0\n0\n0\n0\n0\n0\n0\n1\n1\n1\n100\n2\n0\n0\n0\n0\n0\n0\n100\n100";
					//build player class with start info
					buildPlayerClass(globalPlayers[sock], databasestr);
					//send char creation page (later)
					//for now send login info
					newplayer.close();
					//threadPlayerData;
					stringc toclient;
addMessageHeader(toclient);
					send(clientsock[sock], toclient.c_str(), toclient.size(),0);
				}
				break;
				}
			case 3:
				{
				//MOVEMENT id, (Position:) x, y, z, rotation, (movVec:) x, y, z
				stringc brokencode[8];
				breakCode(tobreak, brokencode,8);
				globalPlayers[sock].posx=atoi(brokencode[1].c_str());
				globalPlayers[sock].posy=atoi(brokencode[2].c_str());
				globalPlayers[sock].posz=atoi(brokencode[3].c_str());
				globalPlayers[sock].rotation=atoi(brokencode[4].c_str());
				globalPlayers[sock].movx=atof(brokencode[5].c_str());
				globalPlayers[sock].movy=atof(brokencode[6].c_str());
				globalPlayers[sock].movz=atof(brokencode[7].c_str());
				lastMov=newTime+((globalq-1)*1000);
cout << endl;
				printf("!!MOVX, %f!!", globalPlayers[sock].movx);
cout << endl;
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
				stringc brokencode[4];
				breakCode(tobreak, brokencode,4);
				globalPlayers[sock].targetType=atoi(brokencode[2].c_str());
				globalPlayers[sock].targetId=atoi(brokencode[1].c_str());
				globalPlayers[sock].targetIndex=-1;
				globalPlayers[sock].targetKey=atoi(brokencode[3].c_str());
//action 
//0=no keys
//1=ctrl
//decide how to handle and send data back

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
				breakCode(tobreak, brokencode,5); 
int fromType=atoi(brokencode[1].c_str());
int toType=atoi(brokencode[3].c_str());
int fromId=atoi(brokencode[2].c_str());
int toId=atoi(brokencode[4].c_str());
if(toType==4)
{
	if(fromType==4)
	{
	//from equipment
	//swap if of same item type
	}else if(fromType==1)
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
}else if(toType==1)
{
	//move to inventory
	//swap if possible if slot taken do and can't equip, don't swap
	if(fromType==4)
	{cout<<"ASDF" <<globalPlayers[sock].inventory[toId] <<endl;
		//de-equip
		globalPlayers[sock].deequip(fromId,0);
		if(globalPlayers[sock].inventory[toId]==0)
		{
		//nothing in slot move
		globalPlayers[sock].inventory[toId]=globalPlayers[sock].equipmentList[fromId];
		globalPlayers[sock].equipmentList[fromId]=0;
updateInventory(fromType, fromId, 0, sock);
updateInventory(toType, toId, globalPlayers[sock].inventory[toId], sock);
		}else
		{
		//item in slot swap?
		}
	}else if(fromType==1)
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
}else if(brokencode[1]=="/admin quit server")
{
printf("quitting server");
	runServer=false;
}else{
	for(int x=0;x<globalPlayers[sock].localPlayersR.size();x++)
	{
		sendChat(globalPlayers[sock].id,globalPlayers[sock].inGameName,brokencode[1],globalPlayers[sock].localPlayersR[x]);
	}
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
stringc brokencode[2];
				breakCode(tobreak, brokencode,2);
uid=atoi(brokencode[1].c_str());
itemSys.sendItem(uid, sock);
				break;
				}
			case 100:
				{
				//get version ID from client
cout <<"got version\n";
				stringc brokencode[2];
				breakCode(tobreak, brokencode,2);
				if(brokencode[1]==versionId)
				{
					//up to date
cout <<"up tp date\n";
					stringc sendCode="105|";
addMessageHeader(sendCode);
					send(clientsock[sock], sendCode.c_str(), sendCode.size(),MSG_NOSIGNAL);
				}else
				{

					//get files needed to upload
					vector<stringc> fileList;
					int currentVersion=atoi(brokencode[1].c_str())+1;
cout <<"updating: "<< currentVersion <<"\n";
					while(currentVersion<=versionId)
					{
						fstream fileIn;
						stringc fileLocation;
						char fileLocChar[200];
						sprintf(fileLocChar,"database/patcher/%i",currentVersion);
						fileLocation=fileLocChar;
						fileIn.open(fileLocation.c_str());
						if(fileIn.is_open())
						{
						while(!fileIn.eof())
						{						
							char fileOut[200];
							fileIn.getline(fileOut,200);
							stringc fileOutStr=fileOut;
							fileList.push_back(fileOutStr);
if(fileList[fileList.size()-1]=="")
{
fileList.pop_back();
}
						}
						}else
						{
						//send error occured
						for(int z=0;z<fileList.size();z++)
						{
						fileList.pop_back();
						}
						}
						currentVersion++;
					}
					//send number of files and new version
					stringc sendCode;
					char convert[200];
					int totalSize=0;
					sprintf(convert,"100|%i|%i|",fileList.size(),versionId);
					sendCode.append(convert);
					addMessageHeader(sendCode);
					send(clientsock[sock], sendCode.c_str(), sendCode.size(),MSG_NOSIGNAL);
cout <<"got files\n";
				// remove duplicate files
cout <<"sending "<< fileList.size()<<" files\n";
					//start sending files
					int failCounter=0;
					for(int x=0;x<fileList.size();x++)
					{
						bool fileSent;
cout <<"sending "<< fileList[x].c_str()<<"\n";
						sendFile(sock, fileList[x], fileSent);
						received = recv(clientsock[sock], buffer, BUFFSIZE, 0);
						if(received==-1)
						{
							x=fileList.size();
						}
						stringc sendCheck=buffer;
						breakCode(sendCheck, brokencode,2);
						if(brokencode[1]==0)
						{
							//resend
							x--;
							failCounter++;
						}
						if(failCounter>5)
						{
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
savePlayer(sock);
//reset globalPlayers slot
	
	//state the socket is closing
	cout << "Closing Socket: "<< inet_ntoa(echoclient[sock].sin_addr) << endl;
	//close socket
	close(clientsock[sock]);
	//open socket for new connections
	socketsused[sock]=0;
	virtualMapSystem[globalPlayers[sock].mapx+middleMapX][globalPlayers[sock].mapy+middleMapY][globalPlayers[sock].mapz+middleMapZ].removePlayer(sock);
	globalPlayers[sock].id=-1;	
	//exit thread
	pthread_exit(NULL);
}



//threads for each client
pthread_t clients[maxconnections];

/*************/
/* MAIN PART */
/*************/
int main()
{
	//set all sockets used to 0
	for(int z=0;z<maxconnections;z++)
	{
		socketsused[z]=0;
		globalPlayers[z].id=-1;
	}
	int argc=2;
	char *argv[2];
            
	struct sockaddr_in echoserver;


	if (argc != 2) 
	{
		fprintf(stderr, "USAGE: echoserver <port>\n");
		exit(1);
	}
	/* Create the TCP socket */
	if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
	{
		Die("Failed to create socket");
	}

	/* Construct the server sockaddr_in structure */
	memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
	echoserver.sin_family = AF_INET;                  /* Internet/IP */
	echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
	echoserver.sin_port = htons(50000);//atoi(argv[1]));       /* server port */


	/* Bind the server socket */
	if (bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) 
	{
		Die("Failed to bind the server socket");
	}
	/* Listen on the server socket */
	if (listen(serversock, MAXPENDING) < 0) 
	{
		Die("Failed to listen on server socket");
	}

	cout << "Server V 0.5"<< endl;
	cout << "Loading start info"<< endl;
	//load in last id sets
	ifstream idfile;
	idfile.open("database/idfile.idfile.rofh");
	if(idfile.is_open()){
		std::string infromfile;
		getline(idfile, infromfile);
		globalplayerid=atoi(infromfile.c_str());
		getline(idfile, infromfile);
		globalitemid=atoi(infromfile.c_str());
		getline(idfile, infromfile);
		globalmountid=atoi(infromfile.c_str());
		getline(idfile, infromfile);
		globalmonsterid=atoi(infromfile.c_str());
		getline(idfile, infromfile);
		globalnpcid=atoi(infromfile.c_str());
		getline(idfile, infromfile);
		versionId=atoi(infromfile.c_str());
	cout << "Building virtual map system"<< endl;
int tempHolder=0;
getline(idfile, infromfile);
		tempHolder=atoi(infromfile.c_str());
		virtualMapSystem.resize(tempHolder);
getline(idfile, infromfile);
		tempHolder=atoi(infromfile.c_str());
for(int x=0;x<virtualMapSystem.size();x++)
		virtualMapSystem[x].resize(tempHolder);
getline(idfile, infromfile);
		tempHolder=atoi(infromfile.c_str());
		for(int x=0;x<virtualMapSystem.size();x++)
		for(int y=0;y<virtualMapSystem[x].size();y++)
		virtualMapSystem[x][y].resize(tempHolder);
getline(idfile, infromfile);
		middleMapX=atoi(infromfile.c_str());
getline(idfile, infromfile);
		middleMapY=atoi(infromfile.c_str());
getline(idfile, infromfile);
		middleMapZ=atoi(infromfile.c_str());
idfile.close();
	cout << "populating maps with npcs"<< endl;
	//add npc's and start npc thread
ifstream spawnData;
spawnData.open("database/spawnData");
if(spawnData.is_open())
{
while(!spawnData.eof())
{
//get spawn information and add to npcsystem
int fileId;
double posx,posy,posz,mapx,mapy,mapz,freq;
//load npc from file and set spawn
getline(spawnData, infromfile);
fileId=atoi(infromfile.c_str());
getline(spawnData, infromfile);
posx=atof(infromfile.c_str());
getline(spawnData, infromfile);
posy=atof(infromfile.c_str());
getline(spawnData, infromfile);
posz=atof(infromfile.c_str());
getline(spawnData, infromfile);
mapx=atof(infromfile.c_str());
getline(spawnData, infromfile);
mapy=atof(infromfile.c_str());
getline(spawnData, infromfile);
mapz=atof(infromfile.c_str());
getline(spawnData, infromfile);
freq=atof(infromfile.c_str());
npcClass newNpc;
newNpc.loadFromFile(fileId);
newNpc.setSpawnData(posx,posy,posz,mapx,mapy,mapz,freq, false);
newNpc.respawn();
npcsystem.push_back(newNpc);
}
pthread_create(&npcThread, NULL, &HandleNPCUpdates, NULL);
}else
{
//don't start
runServer=false;
cout << "failed to load npc's not starting\n";
}

	
/* Run until cancelled */
		int x=0, y=0;
		while (runServer) 
		{
			//find an open socket
			y=-1;
			for(x=0;x<maxconnections-2;x++)
			{
				if(socketsused[x]==0&&x!=99)
				{
			     		y=x;
				}
		    	}
			      
		/* Wait for client connection */
		     
		if(y>-1)
		{
			//if there is an open socket accept the connection
			unsigned int clientlen = sizeof(echoclient[y]);
			clientsock[y] = accept(serversock, (struct sockaddr *) &echoclient[y], &clientlen);
			fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient[y].sin_addr));
			//multithread the connection
			pthread_create( &clients[y], NULL, &HandleClient, (void*) y);
			//set the socket to used
			socketsused[y]=1;
		}else
			{
				//server is full accept socket in reserved socket
				y=maxconnections-1;
				unsigned int clientlen = sizeof(echoclient[y]);
				clientsock[y] = accept(serversock, (struct sockaddr *) &echoclient[y],&clientlen);
				fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient[y].sin_addr));
				char buffer[BUFFSIZE];
				int received = recv(clientsock[y], buffer, BUFFSIZE, 0);
				//tell the user the server is full
		 		send(clientsock[y], "0001", received, 0);
				//close the socket
				close(clientsock[y]);
			}
		}//end while loop
		//save everything	
		printf("quitting the server");
		saveId();
		saveSpawns();
		for(int x=0;x<maxconnections;x++)
		{
			close(clientsock[x]);
		}
		bool allPlayersKicked=false;
		while(!allPlayersKicked)
		{
			allPlayersKicked=true;
			for(int x=0;x<maxconnections;x++)
			{
				if(globalPlayers[x].id>0)
				{
					allPlayersKicked=false;
					x=maxconnections;
				}
			}
		}
	}else{
		printf("failed to open idfile, not starting server as that would not be healthy");
	}
	cout << "Closing server" << endl;

	return 0;
}
