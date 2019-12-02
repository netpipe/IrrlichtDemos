#define NOMINMAX

//version
int versionId=0;

#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include <wchar.h>
#include <irrlicht.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
//#include <NxPhysics.h>
#include <fstream>
#include "GameFNS.h"
#include "convertString.h"


// Physics
/*static NxPhysicsSDK* gPhysicsSDK = NULL;
static NxScene*	gScene = NULL;
NxVec3 gDefaultGravity(0,-9.81f,0);
NxVec3 gForceVec(0,0,0);
NxReal gForceStrength = 75000000.0f; // some force big enough to toss them balls and boxes around
*/

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

ITimer* timer=0;
#include "breakCode.h"
SOCKET serverSocket;
#include "itemClass.h"
itemSys itemSystem;
#include "charclass.h"
Character Player;

#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "ws2_32.lib")

//#include "mapClass.h"

bool keys[irr::KEY_KEY_CODES_COUNT] ; 

/*keyPosition mouseButtonState[3]; //Left(0), Middle(1) and Right(2) Buttons.
// Mouse X/Y coordinates and Wheel data.
struct mouseData
{
int X;
int Y;
float wheel; //wheel is how far the wheel has moved
};*/
//struct mouseData mouse; 
int mouseButtonState[3]={0,0,0}; //Left(0), Middle(1) and Right(2) Buttons.
// Mouse X/Y coordinates and Wheel data.
   struct mouseData
   {
   int X;
   int Y;
   float wheel; //wheel is how far the wheel has moved
   };
   struct mouseData mouse; 
   int getevents=0;

int envId=1, keyControl=1, pauseLength=5000, returnEnvId=1, globalNodeId=1000, width=1024, height=768;
bool buildNewEnv=true, pauseScreen=false, maploop=true, drawScene=false, mapThread=false;

int waitformouseevent=0;
int charCount=0;
vector<Character> charDat(0);

//needed global vars for classes.h
IrrlichtDevice* device=0;
IVideoDriver* driver=0;
ISceneManager* smgr=0;
IGUIEnvironment* env =0;
bool runloop=true;
int connected=-1;
IGUIEditBox* globaltxtbox1;
IGUIEditBox* globaltxtbox2;
IGUIEditBox* globaltxtbox3;
IGUIEditBox* globaltxtbox4;
IGUIEditBox* redEditBox=0;
IGUIEditBox* greenEditBox=0;
IGUIEditBox* blueEditBox=0;
IGUIEditBox* sphereRadiusEditBox=0;
IGUIEditBox* heightRateEditBox=0;
IGUIEditBox* chatEdit=0;
IGUIListBox* chatBox;
//maps* theMap[4];
//network things
WSADATA wsaData;
sockaddr_in clientService;
unsigned long thid=0;

//NPC INFO
#include "npcClass.h"
vector<npcClass> npcDat(0);

//map variables
const int mapMaxX=256-16, mapMaxZ=256-16;
float vertexDist=.5;
float sphereRadius=5, heightRate=50;
int newVersionId=0, totalFiles=0, currentFile=0, fileSize=0,currentSize=0, failCounter=0;
stringc fileLocation;
stringc fileLocationFinal;
stringc fileContents;
#include "map.h"
mapData theMap;

ISceneNode* targetNode;

HANDLE recvdata;
HANDLE characters;
void loginWindow();
int WINAPI charactersFunc();
void buildGUI();
void createCharWindow();
void registerWindow();
int WINAPI recvdatafunc();

HANDLE maper;
int WINAPI maperFunc(LPVOID lpParam);
int WINAPI  maperFunc(LPVOID lpParam)
{
	mapThread=true;
	while(device->run() && driver && runloop)
	{
		theMap.updateMap();
	}
	mapThread=false;
	return 0;
}

#include "GUIClass.h"
GUIEnvironment GUISystem;
#include "network.h"
#include "classes.h"
s32* globalPushId;


//receives mouse keyboard and gui events, located in classes.h
MyEventReceiver receiver;


//#include "characters.h"
