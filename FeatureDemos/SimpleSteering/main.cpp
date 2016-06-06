/** (c) 2006/07 by Acki B.
  !¡!¡!!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡
  !¡!¡! Code::Blocks-Template - 2006 by Acki B. !¡!¡!
  !¡!¡!¡!¡!¡¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡!¡
**/

// !!! define here your Irrlicht media directory !!!
#define irrMediaDir "./Media"
#define anzEnemies 10

#include "stdDefs.h"
#include "cPathFinder.h"
#include "myEventReceiver.h"

#include <stdlib.h>

IrrlichtDevice* device;
IVideoDriver* driver;
ISceneManager* smgr;
IGUIEnvironment* guienv;
IGUIFont* font;

datPlayer Player;
array<datEnemy> Enemies;
array<datWaypoint> Waypoints;


void createMap();
void createWayPoints();
void createEnemies();
void createPlayer();
void updateEnemies();
// functions for debug
void addDebugBillboard(datWaypoint* wp);
void drawDebugLines();

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
  switch(event.EventType){
    // Tastatureingabe
  	case EET_KEY_INPUT_EVENT:{
 	  	if(!event.KeyInput.PressedDown){
  	   	switch(event.KeyInput.Key){
          case KEY_ESCAPE:{
            Player.prgEnd = true;
            return true;
          }break;
          case KEY_F1:{
            Player.useEnemyCam = !Player.useEnemyCam;
            if(!Player.useEnemyCam){
              smgr->setActiveCamera(Player.cam);
              Enemies[Player.selEnemy].node->setVisible(true);
            }else{
              smgr->setActiveCamera(Enemies[Player.selEnemy].cam);
              Enemies[Player.selEnemy].node->setVisible(false);
            }
            return true;
          }break;
          case KEY_F2:{
            Player.showWaypoints = !Player.showWaypoints;
            return true;
          }break;
          case KEY_ADD:{
            Player.speedEnemies += 0.1;
            if(Player.speedEnemies > 10.0) Player.speedEnemies = 10.0;
            return true;
          }break;
          case KEY_KEY_0 :
          case KEY_KEY_1 :
          case KEY_KEY_2 :
          case KEY_KEY_3 :
          case KEY_KEY_4 :
          case KEY_KEY_5 :
          case KEY_KEY_6 :
          case KEY_KEY_7 :
          case KEY_KEY_8 :
          case KEY_KEY_9 :{
            Player.selEnemy = (int)event.KeyInput.Key - (int)KEY_KEY_0;
            return true;
          }break;
          case KEY_SUBTRACT:{
            Player.speedEnemies -= 0.1;
            if(Player.speedEnemies < 0.1) Player.speedEnemies = 0.1;
            return true;
          }break;
        }
	    }
    }break;
	}
  return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};
int main(){
  MyEventReceiver rcv;
  device = createDevice(EDT_OPENGL, dimension2d<s32>(800, 600), 16, false, false, false, &rcv );
  driver = device->getVideoDriver();
  smgr = device->getSceneManager();
  guienv = device->getGUIEnvironment();

  // craete info text
  font = guienv->getFont(irrMediaDir"/fonthaettenschweiler.bmp");
	IGUIStaticText* txt = guienv->addStaticText(L""
    "+/-  - inc/dec enemy speed\n"
    "0-9  - select enemy\n"
    "F1    - switch camera\n"
    "F2    - show/hide waypoints\n"
    "\nEsc - close program"
  , rect<int>(10,10,400,200), false);
  txt->setOverrideFont(font);
  txt->setOverrideColor(SColor(255, 255, 255, 255));
  // create status text
	txt = guienv->addStaticText(L""
    "enemy selected: #0\n"
    "enemies speed: 1.5\n"
	, rect<int>(650,560,800,600), false);
  txt->setOverrideFont(font);
  txt->setOverrideColor(SColor(255, 255, 255, 255));

  createMap();
  createWayPoints();
  createEnemies();
  createPlayer();

  smgr->setActiveCamera(Player.cam);
	Player.cam->setPosition(vector3df(-50, 1000, -1000));
	Player.cam->setTarget(vector3df(-50, 0, -250));
	Player.cam->updateAbsolutePosition();
  device->getCursorControl()->setVisible(false);
	while(device->run() && !Player.prgEnd){
		driver->beginScene(true, true, SColor(255,100,100,150));
    drawDebugLines(); // only for debug mode (visualisation) used
		smgr->drawAll();
		guienv->drawAll();
    driver->endScene();
    updateEnemies();
//    rcv.updateText(txt);
	}

	device->drop();
	return 0;
}

void updateEnemies(){
  // update only one enemy per loop
  static unsigned int enr = 0;
  line3d<f32> line;
  if(Enemies[enr].route.size() > 0){
    // check if waypoint 0 was reached
    vector3df wp = Waypoints[Enemies[enr].route[0]].pos;
    vector3df ep = Enemies[enr].cam->getPosition();
    wp.Y = ep.Y;
    if(ep.getDistanceFrom(wp) < 5){
      // 1st waypoint reached
      Enemies[enr].wpLast = Enemies[enr].route[0];
      Enemies[enr].route.erase(0);
    }else{
      // update enemy rotation
      Enemies[enr].cam->setTarget(wp);
      Enemies[enr].cam->updateAbsolutePosition();
      // move enemy
      line.start = Enemies[enr].cam->getPosition();
      line.end = line.start + (wp - line.start).normalize() * Player.speedEnemies;
      Enemies[enr].cam->setPosition(line.end);
    }
  }else{
    // calculate new route
    Enemies[enr].node->setMD2Animation(EMAT_STAND);
    int wpt = rand() % Waypoints.size();
    cPathFinder pfinder;
    pfinder.getRoute(Enemies[enr].wpLast, wpt, enr);
    if(Enemies[enr].route.size() > 0) Enemies[enr].node->setMD2Animation(EMAT_RUN);
    if(Enemies[Player.selEnemy].route.size() > 0){
      Player.bbDest->setPosition(Waypoints[Enemies[Player.selEnemy].route[Enemies[Player.selEnemy].route.size() - 1]].pos + vector3df(0,50,0));
    }
  }
  enr = (enr + 1) % Enemies.size();
}

void createMap(){
  // load map data from XML file
  //! using TinyXML, because irrXML sucks... ;)

  // create a ground
  ISceneNode* ground = smgr->addCubeSceneNode(10);

  ground->setScale(vector3df(500, 1, 500));
  ground->setMaterialTexture( 0, driver->getTexture(irrMediaDir"/wall.bmp") );
  ground->setMaterialFlag(EMF_LIGHTING, false);

  // this should be loaded from a data file
  char map[15][16];
  sprintf(map[14], "###############");
  sprintf(map[13], "#             #");
  sprintf(map[12], "# ### ### ### #");
  sprintf(map[11], "# ###     ### #");
  sprintf(map[10], "#     ###     #");
  sprintf(map[ 9], "# ### ### ### #");
  sprintf(map[ 8], "#             #");
  sprintf(map[ 7], "# ## ### #### #");
  sprintf(map[ 6], "# #           #");
  sprintf(map[ 5], "# # #### #### #");
  sprintf(map[ 4], "# # ####      #");
  sprintf(map[ 3], "# #      #### #");
  sprintf(map[ 2], "# # #### #### #");
  sprintf(map[ 1], "#             #");
  sprintf(map[ 0], "###############");

  for(int y = 0; y < 15; y++){
    for(int x = 0; x < 15; x++){
      if(map[y][x] == '#'){
        ground = smgr->addCubeSceneNode(10);
        ground->setScale(vector3df(10, 5, 10));
        ground->setPosition(vector3df(x * 100, 50, y * 100) - vector3df(750, 20, 750));
        ground->setMaterialTexture(0, driver->getTexture(irrMediaDir"/wall.jpg"));
      }
    }
  }
}
void createWayPoints(){
  datWaypoint wpNeu;
  datNextWP nxNeu;

  Waypoints.clear();
  // this should be loaded from a data file
/** map of waypoints
. . . . . . . . . . . . . . .
. 24      25      26      27.
.   . . .   . . .   . . .   .
.   . . . 22      23. . .   .
. 18      19. . . 20      21.
.   . . .   . . .   . . .   .
. 12    1314    1516      17.
.   . .   . . .   . . . .   .
.   .  8 9      10        11.
.   .   . . . .   . . . .   .
.   .   . . . .  6         7.
.   .  4         5. . . .   .
.   .   . . . .   . . . .   .
.  0   1         2         3.
. . . . . . . . . . . . . . .
**/
  //! waypoint #0
  wpNeu.pos = vector3df(-650, 50, -650);
  wpNeu.next.clear();
  nxNeu.ID = 12;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 1;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #1
  wpNeu.pos = vector3df(-450, 50, -650);
  wpNeu.next.clear();
  nxNeu.ID = 0;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 4;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 2;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #2
  wpNeu.pos = vector3df(50, 50, -650);
  wpNeu.next.clear();
  nxNeu.ID = 1;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 5;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 3;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #3
  wpNeu.pos = vector3df(550, 50, -650);
  wpNeu.next.clear();
  nxNeu.ID = 2;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 7;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #4
  wpNeu.pos = vector3df(-450, 50, -450);
  wpNeu.next.clear();
  nxNeu.ID = 1;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 8;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 5;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #5
  wpNeu.pos = vector3df(50, 50, -450);
  wpNeu.next.clear();
  nxNeu.ID = 4;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 6;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 2;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #6
  wpNeu.pos = vector3df(50, 50, -350);
  wpNeu.next.clear();
  nxNeu.ID = 10;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 5;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 7;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #7
  wpNeu.pos = vector3df(550, 50, -350);
  wpNeu.next.clear();
  nxNeu.ID = 11;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 6;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 3;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #8
  wpNeu.pos = vector3df(-450, 50, -150);
  wpNeu.next.clear();
  nxNeu.ID = 9;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 4;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #9
  wpNeu.pos = vector3df(-350, 50, -150);
  wpNeu.next.clear();
  nxNeu.ID = 13;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 8;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 10;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #10
  wpNeu.pos = vector3df(50, 50, -150);
  wpNeu.next.clear();
  nxNeu.ID = 9;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 15;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 11;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 6;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #11
  wpNeu.pos = vector3df(550, 50, -150);
  wpNeu.next.clear();
  nxNeu.ID = 10;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 17;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 7;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #12
  wpNeu.pos = vector3df(-650, 50, 50);
  wpNeu.next.clear();
  nxNeu.ID = 18;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 13;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 0;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #13
  wpNeu.pos = vector3df(-350, 50, 50);
  wpNeu.next.clear();
  nxNeu.ID = 12;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 14;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 9;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #14
  wpNeu.pos = vector3df(-250, 50, 50);
  wpNeu.next.clear();
  nxNeu.ID = 13;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 19;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 15;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #15
  wpNeu.pos = vector3df(50, 50, 50);
  wpNeu.next.clear();
  nxNeu.ID = 14;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 10;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 16;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #16
  wpNeu.pos = vector3df(150, 50, 50);
  wpNeu.next.clear();
  nxNeu.ID = 15;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 20;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 17;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #17
  wpNeu.pos = vector3df(550, 50, 50);
  wpNeu.next.clear();
  nxNeu.ID = 16;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 21;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 11;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #18
  wpNeu.pos = vector3df(-650, 50, 250);
  wpNeu.next.clear();
  nxNeu.ID = 12;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 24;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 19;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #19
  wpNeu.pos = vector3df(-250, 50, 250);
  wpNeu.next.clear();
  nxNeu.ID = 18;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 22;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 14;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #20
  wpNeu.pos = vector3df(150, 50, 250);
  wpNeu.next.clear();
  nxNeu.ID = 23;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 16;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 21;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #21
  wpNeu.pos = vector3df(550, 50, 250);
  wpNeu.next.clear();
  nxNeu.ID = 20;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 27;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 17;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #22
  wpNeu.pos = vector3df(-250, 50, 350);
  wpNeu.next.clear();
  nxNeu.ID = 25;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 19;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 23;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #23
  wpNeu.pos = vector3df(150, 50, 350);
  wpNeu.next.clear();
  nxNeu.ID = 22;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 20;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 26;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #24
  wpNeu.pos = vector3df(-650, 50, 550);
  wpNeu.next.clear();
  nxNeu.ID = 18;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 25;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #25
  wpNeu.pos = vector3df(-250, 50, 550);
  wpNeu.next.clear();
  nxNeu.ID = 24;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 22;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 26;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #26
  wpNeu.pos = vector3df(150, 50, 550);
  wpNeu.next.clear();
  nxNeu.ID = 25;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 23;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 27;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);
  //! waypoint #27
  wpNeu.pos = vector3df(550, 50, 550);
  wpNeu.next.clear();
  nxNeu.ID = 26;
  wpNeu.next.push_back(nxNeu);
  nxNeu.ID = 21;
  wpNeu.next.push_back(nxNeu);
  addDebugBillboard(&wpNeu);
  Waypoints.push_back(wpNeu);

  // calculate distances for path finding
  for(unsigned int t = 0; t < Waypoints.size(); t++){
    for(unsigned int r = 0; r < Waypoints[t].next.size(); r++){
      Waypoints[t].next[r].Distanz = (int)Waypoints[Waypoints[t].next[r].ID].pos.getDistanceFrom(Waypoints[t].pos);
    }
  }
}
void createPlayer(){
  // create player camera
  SKeyMap keyMap[8];
  keyMap[0].Action = EKA_MOVE_FORWARD;
  keyMap[0].KeyCode = KEY_UP;
  keyMap[1].Action = EKA_MOVE_FORWARD;
  keyMap[1].KeyCode = KEY_KEY_W;
  keyMap[2].Action = EKA_MOVE_BACKWARD;
  keyMap[2].KeyCode = KEY_DOWN;
  keyMap[3].Action = EKA_MOVE_BACKWARD;
  keyMap[3].KeyCode = KEY_KEY_S;
  keyMap[4].Action = EKA_STRAFE_LEFT;
  keyMap[4].KeyCode = KEY_LEFT;
  keyMap[5].Action = EKA_STRAFE_LEFT;
  keyMap[5].KeyCode = KEY_KEY_A;
  keyMap[6].Action = EKA_STRAFE_RIGHT;
  keyMap[6].KeyCode = KEY_RIGHT;
  keyMap[7].Action = EKA_STRAFE_RIGHT;
  keyMap[7].KeyCode = KEY_KEY_D;
	Player.cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.10f, -1, keyMap, 8, false);
  Player.selEnemy = 0;
  Player.speedEnemies = 3.0;
  Player.useEnemyCam = false;
  Player.prgEnd = false;
  Player.showWaypoints = false;
  Player.bbDest = smgr->addBillboardSceneNode();
	Player.bbDest->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	Player.bbDest->setMaterialTexture(0, driver->getTexture(irrMediaDir"/particle.bmp"));
	Player.bbDest->setMaterialFlag(video::EMF_LIGHTING, false);
	Player.bbDest->setMaterialFlag(video::EMF_ZBUFFER, true);
	Player.bbDest->setSize(dimension2d<f32>(50.0f, 50.0f));
	Player.txtSel = smgr->addTextSceneNode(font, L"selected", SColor(255, 0, 0, 155), 0, vector3df(0, 50, 0));

}
void createEnemies(){
	// create enemy
	datEnemy neu;
  Enemies.clear();
  srand(device->getTimer()->getRealTime());
  for(int y = 0; y < anzEnemies; y++){
    neu.cam = smgr->addCameraSceneNodeFPS();
    // set position near a random choosen waypoint
    int wpn;
    wpn = rand() % Waypoints.size();
    neu.cam->setPosition(vector3df(Waypoints[wpn].pos.X + 10, 45, Waypoints[wpn].pos.Z + 10));
    neu.cam->setInputReceiverEnabled(false);
    // set waypoint as first target
    neu.route.clear();
    neu.route.push_back(wpn);
    IAnimatedMesh* mesh = smgr->getMesh(irrMediaDir"/faerie.md2");
    neu.node = smgr->addAnimatedMeshSceneNode(mesh, neu.cam);
    neu.node->setMaterialTexture( 0, driver->getTexture(irrMediaDir"/faerie2.bmp") );
    neu.node->setMaterialFlag(EMF_LIGHTING, false);
    neu.node->setMD2Animation(EMAT_RUN);
    // rotation offset for faerie, because she's looking to the side
    neu.node->setRotation(vector3df(0, -90, 0));
    // position offset to get faerie to the ground
    neu.node->setPosition(vector3df(0, -16, 0));
    Enemies.push_back(neu);
  }
}

void drawDebugLines(){
  static bool showPrev = true;
  if(showPrev != Player.showWaypoints){
    for(unsigned int t = 0; t < Waypoints.size(); t++){
      if(Waypoints[t].bill) Waypoints[t].bill->setVisible(Player.showWaypoints);
    }
    showPrev = Player.showWaypoints;
  }
  SMaterial mat;
  mat.Lighting = false;
  driver->setMaterial(mat);
  driver->setTransform(ETS_WORLD, matrix4());
  if(Player.showWaypoints){
    for(unsigned int t = 0; t < Waypoints.size(); t++){
      for(unsigned int r = 0; r < Waypoints[t].next.size(); r++){
        if((Waypoints[t].next[r].ID < Waypoints.size()) && (Waypoints[t].next[r].ID > t)){
          SColor col(255, 155, 0, 0);
          // is way current path then draw green !!!
          if(Enemies[Player.selEnemy].route.size() > 0){
            if(((Enemies[Player.selEnemy].route[0] == Waypoints[t].next[r].ID) && (Enemies[Player.selEnemy].wpLast == t))
            || ((Enemies[Player.selEnemy].route[0] == t) && (Enemies[Player.selEnemy].wpLast == Waypoints[t].next[r].ID))){
              col = SColor(255, 0, 200, 0);
            }
          }
          driver->draw3DLine(Waypoints[t].pos, Waypoints[Waypoints[t].next[r].ID].pos, col);
        }
      }
    }
  }
  // show path of current enemy
  vector3df p1 = Enemies[Player.selEnemy].cam->getAbsolutePosition();
  for(unsigned int t = 0; t < Enemies[Player.selEnemy].route.size(); t++){
    vector3df p2 = Waypoints[Enemies[Player.selEnemy].route[t]].pos;
    driver->draw3DLine(p1, p2, SColor(255, 200, 150, 150));
    p1 = p2;
  }
}
void addDebugBillboard(datWaypoint* wp){
  // drawing a billboard just for seeing the waypoint
  wp->bill = smgr->addBillboardSceneNode();
	wp->bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	wp->bill->setMaterialTexture(0, driver->getTexture(irrMediaDir"/portal6.bmp"));
	wp->bill->setMaterialFlag(video::EMF_LIGHTING, false);
	wp->bill->setMaterialFlag(video::EMF_ZBUFFER, true);
	wp->bill->setSize(dimension2d<f32>(50.0f, 50.0f));
  wp->bill->setPosition(wp->pos);
}
