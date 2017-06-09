//'main' gameloop object
//checks for player input
//creates objects and sets up level
//updates all the other objects in the scene
//and manages the camera control code (for now)

#include "stdafx.h"
#include "gameloop.h"
#include "GFramework.h"


using namespace irr;
using namespace scene;
using namespace video;
using namespace core;



//ew, gloabls
float angle;
float angle2;
float distance;

int tMouseX;
int tMouseY;
int tMouseW;


//NOTE: THIS IS HOW THE PLANE INGAME WORKS, MODIFY ROTATION AND POSITION ACCOORDING TO THIS!!!
//CARTESIAN PLANE
//POSITION
//X = FRONT AND BACK
//Y = UP AND DOWN
//Z = LEFT AND RIGHT
//				|
//				|
//				|Y
//				/\
//			   /  \
//			 X/    \Z
//
//ROTATION
//X = UP AND DOWN
//Y = LEFT AND RIGHT
//Z = TILT, NOT USED
//

GameLoop::GameLoop(irr::IrrlichtDevice *graphics, KeyListener *receiver, irrklang::ISoundEngine *sound):graphics(0),GamePaused(0), pCam(0),then(0),velocity(0)
{
	//set up game


	//establish game object variables initialized in the init.cpp file
	this->graphics = graphics;
	this->sound = sound;
	this->receiver = receiver;

	//add spacebox, skybox, whatever the hell you call it
	scene::ISceneNode *skybox = graphics->getSceneManager()->addSkyBoxSceneNode(
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_top3.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_bottom4.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_left2.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_right1.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_front5.jpg"),
		graphics->getVideoDriver()->getTexture("res/textures/skyboxes/1/space_back6.jpg"));


	//some flat lighting
	//TODO: add real lighting
	//maybe a sun
	//ok just did a sun
	graphics->getSceneManager()->setAmbientLight(video::SColor(64,64,64,64));
	irr::scene::ILightSceneNode *sun = graphics->getSceneManager()->addLightSceneNode(0,core::vector3df(-20000,0,-20000),video::SColor(255,255,255,255),50000);
	if(sun)
	{
		//sun->setLightType(video::ELT_DIRECTIONAL);
		sun->setRotation(core::vector3df(0,90,0));
		scene::ISceneNode *corona= graphics->getSceneManager()->addBillboardSceneNode(sun,core::dimension2d<f32>(5000,5000));
		corona->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/particlewhite.bmp"));
		corona->setMaterialFlag(video::EMF_LIGHTING, false);
		corona->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	}

	//create game manager object
	Manager = new gameManager(graphics,receiver);

	//player create
	CPlayer = new Player(graphics,sound,receiver,core::vector3df(0,0,0));


	//create the hud object
	CHud = new Hud(graphics,CPlayer);


	//PLACEHOLDER
	CShip *ai = new CShip(graphics,sound, core::vector3df(-5000,0,5000),TERR_PRAETORIAN_CRUISER,FACTION_PROVIAN_CONSORTIUM,TYPE_SHIP);
	Manager->addShip(ai);
	CShip *test = new CShip(graphics,sound, core::vector3df(-2000,0,4000),TERR_PRAETORIAN_CRUISER,FACTION_PROVIAN_CONSORTIUM,TYPE_SHIP);
	Manager->addShip(test);

	//player camera create
	pCam = graphics->getSceneManager()->addCameraSceneNode();
	if(pCam)
	{
		pCam->setFarValue(100000);
	}

	//important GUI stuff
	//shows what direction the ship wants to face
	XYcircle = graphics->getSceneManager()->addCubeSceneNode();
	if(XYcircle)
	{
		XYcircle->setPosition(CPlayer->getPos());
		XYcircle->setScale(core::vector3df(60,0,60));
		XYcircle->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/circle.png"));
		XYcircle->setMaterialFlag(video::EMF_LIGHTING,false);
		XYcircle->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		XYcircle->setMaterialFlag(video::EMF_ZBUFFER,true);
		XYcircle->setVisible(false);
	}
	ZYcircle = graphics->getSceneManager()->addCubeSceneNode();
	if(ZYcircle)
	{
		ZYcircle->setPosition(CPlayer->getPos());
		ZYcircle->setScale(core::vector3df(0,60,60));
		ZYcircle->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/circle.png"));
		ZYcircle->setMaterialFlag(video::EMF_LIGHTING,false);
		ZYcircle->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		ZYcircle->setMaterialFlag(video::EMF_ZBUFFER,true);
		ZYcircle->setVisible(false);
	}
	TurningArrow=graphics->getSceneManager()->addCubeSceneNode();
	if(TurningArrow)
	{
		TurningArrow->setScale(core::vector3df(60,0,60));
		TurningArrow->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/arrow.png"));

		TurningArrow->setMaterialFlag(video::EMF_LIGHTING, false);

		TurningArrow->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		TurningArrow->setVisible(false);
	}


	//temporary stuff
	//turn this into functioning station later
	scene::IMesh *mesh = graphics->getSceneManager()->getMeshManipulator()->createMeshWithTangents(graphics->getSceneManager()->getMesh("res/station.obj"));
	scene::ISceneNode *n=graphics->getSceneManager()->addMeshSceneNode(mesh);
    if (n)
    {

		  n->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/station_color.jpg"));
		  n->setMaterialTexture(1,graphics->getVideoDriver()->getTexture("res/station_bump.jpg"));
		  n->setScale(core::vector3df(50,50,50));
		  n->setPosition(core::vector3df(5000,0,5500));
          n->setMaterialFlag(video::EMF_LIGHTING, true);
		  n->getMaterial(0).NormalizeNormals=true;
		  n->getMaterial(0).SpecularColor.set(0,0,0,0);

    }
	mesh->drop();
	planet *p = new planet(graphics,graphics->getSceneManager()->getMesh("res/earth.x"),core::vector3df(10000,-10000,10000),core::vector3df(5000,5000,5000));
	Manager->addPlanet(p);

	//setup variables
	player_target =0;
	fired = false;
	u32 then = graphics->getTimer()->getTime();
    angle = 0;
    angle2 = 0;
    distance = 300.0;
	velocity=0;
}

GameLoop::~GameLoop()
{
	//destructor
	//dont do anything here
}


void GameLoop::Run()
{
	//main game loop


	//determine fps difference for frame independent movement
	u32 now = graphics->getTimer()->getTime();
	frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	then = now;


	sound->setListenerPosition(pCam->getPosition(),pCam->getRotation());

	//constantly update camera position
	//to track player
	pCam->setTarget(CPlayer->getPos());


	//constantly update position of turning circle GUI
	XYcircle->setPosition(CPlayer->getPos());
	XYcircle->setRotation(core::vector3df(0,CPlayer->turn.Y,0));
	ZYcircle->setPosition(CPlayer->getPos());
	ZYcircle->setRotation(core::vector3df(CPlayer->getRot().X,CPlayer->turn.Y,0));

	TurningArrow->setRotation(core::vector3df(CPlayer->turn.X,CPlayer->turn.Y,0));
	TurningArrow->setPosition(CPlayer->getPos());


	//run functions
	playerControl(receiver,frameDeltaTime,sound);					//player key input
	cameraControl(receiver,frameDeltaTime);							//camera movement input
	CHud->updateHud(player_target);									//continually update hud
	CPlayer->playerRun(frameDeltaTime);								//run through player funcs
	selectTarget();													//select target for player
	Manager->gameManagerLoop(frameDeltaTime,sound, CPlayer);		//update nodes and game scene ai
	CPlayer->manageTurrets(player_target,frameDeltaTime);			//gives player target to turrets
}

void GameLoop::selectTarget()
{
	//If the player clicks with the left mouse button
	//Check the gameManger object if there is any ship that the mouse is near
	//if there is a ship, return the ship object and set player_target to that ship
	//if there is no ship, return 0
	if(receiver->mouseLButtonDown())
	{
		player_target = Manager->getTarget();
	}
	if(player_target!=0)
	{
		if(player_target->getHullPoints()<1)
		{
			player_target=0;
		}
	}

}


//turns the turning circle on or off
void GameLoop::setTurnCircleGUI(bool is_visible)
{
	TurningArrow->setVisible(is_visible);
	XYcircle->setVisible(is_visible);
	ZYcircle->setVisible(is_visible);
}

//big func to move player and stuff
void GameLoop::playerControl(KeyListener *receiver, f32 frameDeltaTime,irrklang::ISoundEngine *sound)
{
	if(CPlayer->getHull()>0)
	{
		//get input and do stuff with it
		if(receiver->IsKeyDown(irr::KEY_KEY_A))
		{
			CPlayer->turn.Y-=35*frameDeltaTime;
		}
		if(receiver->IsKeyDown(irr::KEY_KEY_D))
		{
			CPlayer->turn.Y += 35*frameDeltaTime;
		}
		if(receiver->IsKeyDown(irr::KEY_KEY_W))
		{
			if(CPlayer->turn.X<70)
				CPlayer->turn.X+=35*frameDeltaTime;
		}
		if(receiver->IsKeyDown(irr::KEY_KEY_S))
		{
			if(CPlayer->turn.X>-70)
				CPlayer->turn.X-=35*frameDeltaTime;
		}

		if(receiver->IsKeyDown(irr::KEY_KEY_Q))
		{
			CPlayer->turn.Y=CPlayer->getRot().Y;
			CPlayer->turn.X=CPlayer->getRot().X;
			CPlayer->turn.Z=CPlayer->getRot().Z;
		}

		//get input to add velocity to player
		if(receiver->IsKeyDown(irr::KEY_KEY_X))
		{
			if(CPlayer->getVelocity() < CPlayer->getMaxSpeed())	//dont let velocity get above maxspeed
			{
				velocity+=(2+velocity/2)*frameDeltaTime;
				CPlayer->setVelocity(velocity);
			}
		}
		if(receiver->IsKeyDown(irr::KEY_KEY_Z))
		{
			if(CPlayer->getVelocity() > -10)
			{
				velocity-=(2+velocity/2)*frameDeltaTime;
				CPlayer->setVelocity(velocity);
			}

		}


		//self explainatory
		if(receiver->IsKeyDown(irr::KEY_SPACE))
		{
			CPlayer->shoot();
		}

		//show turning circle only when turning
		if(CPlayer->getRot().Y-3>CPlayer->turn.Y | CPlayer->getRot().Y+3<CPlayer->turn.Y | CPlayer->getRot().X-3>CPlayer->turn.X | CPlayer->getRot().X+3<CPlayer->turn.X)
			setTurnCircleGUI(true);
		else
			setTurnCircleGUI(false);
	}


}

void GameLoop::cameraControl(KeyListener *receiver, f32 frameDeltaTime)
{
	//move camera by determining if mouse moved
	//check pixels the mouse moved
	if(receiver->mouseRButtonDown()==true)
	{
		if(receiver->mouseX()!=tMouseX)
		{
			int i=receiver->mouseX()-tMouseX;
			tMouseX=receiver->mouseX();
			angle+=i;
		}
		if(receiver->mouseY()!=tMouseY)
		{
			int t=receiver->mouseY()-tMouseY;
			tMouseY=receiver->mouseY();
			angle2+=t;
		}
	}
	//change distance of camera to player
	if(receiver->IsKeyDown(irr::KEY_MINUS))
	{
		if(distance<CAMERA_DISTANCE_MAX)
			distance+=10;
	}
	if(receiver->IsKeyDown(irr::KEY_PLUS))
	{
		if(distance>CAMERA_DISTANCE_MIN)
			distance-=10;
	}


	//if angle is too big or too small
	//put it back in 360 limit
	if (angle > 360)
		angle -= 360;
	else if (angle < 0)
		angle += 360;

	if(angle2>360)
		angle-=360;
	if(angle<0)
		angle+=360;

	//distance cannot be less than zero, or else there will be inversion
	if(distance<0)
		distance=0;


	tMouseX=receiver->mouseX();
	tMouseY=receiver->mouseY();
	tMouseW=receiver->mouseWheel();	//this doesnt fucking work


	core::vector3df targ = pCam->getTarget();
	core::vector3df playerCameraPos;



	//3d trig
	//this math is to determine the location of orbiting camera
	playerCameraPos.Y = sin(angle2*3.14/180)*distance;
	playerCameraPos.Y += targ.Y;


	float temp;
	temp=cos(angle2*3.14/180);


	//some code to calculate position
	core::vector3df old;
	playerCameraPos.X = (sin((angle) * 3.141 / 180) * (distance));
	playerCameraPos.X = playerCameraPos.X*temp;
	playerCameraPos.X  += targ.X;

	playerCameraPos.Z  = (cos((angle) * 3.141/ 180) * (distance));
	playerCameraPos.Z = playerCameraPos.Z*temp;
	playerCameraPos.Z += targ.Z;


	//smooth out camera motion
	old = pCam->getPosition();
	old=old*0.9+playerCameraPos*0.1;
	pCam->setPosition(old);
}

