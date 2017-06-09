#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "irrlicht.h"
#include "irrKlang.h"


#include "ship.h"
#include "player.h"
#include "hud.h"
#include "turret.h"
#include "projectile.h"
#include "keylistener.h"
#include "planet.h"
#include "gamemanager.h"
#include "explosion.h"
#include "menu.h"



#include "vector"


using namespace irr;
using namespace irrklang;
using namespace scene;



class GameLoop
{
public:
	//mun conference at edison :(


	bool GamePaused;
	bool fired;
	bool isHudVisible;
	float velocity;
	u32 then;
	f32 frameDeltaTime;

	CShip *player_target;


	Player *CPlayer;
	Hud *CHud;
	gameManager *Manager;

	//lots of variables
	irr::scene::ISceneNodeAnimator *CPlayerAnimator;
	irr::scene::ICameraSceneNode *pCam;
	irr::scene::IMeshSceneNode *ship;
	irr::scene::ISceneNode *XYcircle;
	irr::scene::ISceneNode *ZYcircle;
	irr::scene::ISceneNode *TurningArrow;
	irr::scene::ITriangleSelector *selector;


	irr::IrrlichtDevice *graphics;



	irrklang::ISoundEngine *sound;
	KeyListener *receiver;


	GameLoop(irr::IrrlichtDevice *graphics,KeyListener *receiver, irrklang::ISoundEngine *sound);
	~GameLoop();
	void Run();	//main game func
	void SceneManager();
	void playerControl(KeyListener *receiver, f32 frameDeltaTime,irrklang::ISoundEngine *sound);
	void cameraControl(KeyListener *receiver, f32 frameDeltaTime);

	void setTurnCircleGUI(bool is_visible);
	void selectTarget();
	void manageTurrets();



};


#endif
