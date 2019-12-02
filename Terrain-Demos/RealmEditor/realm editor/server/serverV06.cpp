//SERVER MAIN FILE
//VERSION 0.6

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
#include "irrlicht/irrlicht.h"
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
#include "itoa.h"
#include "attack.h"
#include "getCrossMapDistance.h"
#include "getCrossMapVector.h"
#include "itemClass.h"
#include "itemSystem.h"
#include "playerClass.h"
#include "npcClass.h"
#include "mapClass.h"

//***************************************************************************************

//init global player variable
playerClass globalPlayers[maxconnections];

#include "addMessageHeader.h"
#include "sendHealth.h"
/*
************************************************************************************
not used yet atleast, may be implemented elsewhere or changed or removed or combined
************************************************************************************


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
*/
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
		globalPlayers[newList[x]].sendPlayer(currentUser,12);
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

#include "npcThread.h"
#include "handleClientMovementThread.h"
#include "handleClientThread.h"

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
