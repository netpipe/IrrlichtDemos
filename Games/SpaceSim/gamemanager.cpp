//Here is the gameManager code
//The purpose of this object is to manage the all the projectiles and AI in the scene
//Operates the AI code by checking distances and hostility to run the AI
//Manages projectile collisions and planet rotations

#include "stdafx.h"
#include "gamemanager.h"

using namespace irr;
using namespace scene;
using namespace core;


gameManager::gameManager(irr::IrrlichtDevice *graphics, KeyListener *receiver)
{
	this->graphics=graphics;
	this->receiver = receiver;
	playerDead=false;
}
gameManager::~gameManager()
{
	//delete it? blasphemy
}
void gameManager::gameManagerLoop(f32 frameDeltaTime, Player* CPlayer)
{

	//AI ship loop
	//Controls the ship AI
	//
	//Heres how the projectile shooting works.
	//Its not the best way to do it I'm sure
	//In order to have projectiles be able to collide with other objects without resorting to the inbuild irrlicht collision system because I don't understand it
	//You need to create the projectile in the gameManager class so it can be added to the projectile array
	//Since there is no way to send objects 'up' a heirachy level, the gameManager class needs to be constantly checking the ship classes if it wants to shoot
	//If the ship wants to shoot, the ship returns true for the primary_shoot variable
	//The ship itself does not create the projectile, the gameManager class does
	//So the gameManager object constantly scans each ship object to see if it wants to shoot, and then creates the projectile in a higher hierchy
	//Not the best way to do it, but I can't figure out any other ways.
	//
	//Also, the ship_manager loop MUST go before the projectile manager loop

	for(int i=0;i<ship_manager.size();i++)
	{
		if(ship_manager[i]->getHullPoints()>0)
		{
			ship_manager[i]->AIRun(frameDeltaTime);
			core::vector3df pos = ship_manager[i]->getPos();
			if(ship_manager[i]->hostile==true)
			{
				if(pos.getDistanceFrom(CPlayer->getPos())<8000)
				{
					ship_manager[i]->engagePlayer(CPlayer->getPos());
				}
			}
			if(ship_manager[i]->cannon.primary_shoot==true)
			{
				projectile *p = ship_manager[i]->addShot(ship_manager[i]->getPos());
				projectile_manager.push_back(p);
				projectile *p2 = ship_manager[i]->addShot(core::vector3df(ship_manager[i]->getPos().X+10,ship_manager[i]->getPos().Y+10,ship_manager[i]->getPos().Z));
				projectile_manager.push_back(p2);
				projectile *p3 = ship_manager[i]->addShot(ship_manager[i]->getPos());
				projectile_manager.push_back(p3);
			}
		}
		else
		{
			explosion* e = new explosion(graphics,ship_manager[i]->getPos());
			explosion_manager.push_back(e);
			ship_manager[i]->drop();
			ship_manager.erase(ship_manager.begin()+i);
		}
	}


	//This function scans through each projectile and runs them
	//irrlicht collision detection is NOT used
	//I do not understand it
	//So, heres my ghetto collision detection
	for(int i=0;i<projectile_manager.size();++i)
	{
		core::vector3df dist = (projectile_manager[i]->bullet->getPosition());
		bool tmp;
		tmp = false;
		if(projectile_manager[i]->checkRange()<projectile_manager[i]->range)
		{
			projectile_manager[i]->projRun(frameDeltaTime);

			//Make sure the projectile created by the player doesn't destroy the players own ship
			if(projectile_manager[i]->getShip()!=CPlayer->ship)
			{
				core::vector3df dist = (projectile_manager[i]->bullet->getPosition());
				if(dist.getDistanceFrom(CPlayer->getPos())<300)
				{
//					////sound->play3D("res/sounds/hit.wav",CPlayer->getPos());
					CPlayer->damagePlayer(projectile_manager[i]->damage);
					tmp = true;
				}
			}
			for(int l=0;l<ship_manager.size();l++)
			{
				//Make sure AI doesn't destroy itself
				if(ship_manager[l]->ship!=projectile_manager[i]->getShip())
				{
					if(ship_manager[l]->getHullPoints()>0)
					{
						//distance to destroy is 300 right now
						//might have to change it as frigates get implemented
						if(dist.getDistanceFrom(ship_manager[l]->getPos())<300)
						{
							////sound->play3D("res/sounds/hit.wav",ship_manager[l]->getPos());
							ship_manager[l]->damageShip(projectile_manager[i]->damage);
							tmp = true;
						}
					}
				}
			}

			//the tmp bool MUST be used, or else the program can crash
			//If it is not used, then projectile_manager might call a projectile object that was recently deleted
			//giving an out of range error

			if(tmp==true)
			{
				//delete the projectile if its hit something
				projectile_manager[i]->drop();
				projectile_manager.erase(projectile_manager.begin()+i);
			}
		}
		else
		{
			//delete projectiles that have exceeded their maximum range
			projectile_manager[i]->drop();
			projectile_manager.erase(projectile_manager.begin()+i);
		}
	}


	//Player function, see below
	planetAnimationManager(frameDeltaTime);
	explosionAnimationManager();
	playerShoot(graphics,CPlayer);
}

void gameManager::planetAnimationManager(f32 frameDeltaTime)
{
	for(int i = 0;i<planet_manager.size();i++)
	{
		planet_manager[i]->rotate(frameDeltaTime);
	}
}
void gameManager::explosionAnimationManager()
{
	for(int i = 0;i<explosion_manager.size();i++)
	{
		if(explosion_manager[i]->end==false)
		{
			explosion_manager[i]->loop();
		}
		else
		{
			explosion_manager[i]->drop();
			explosion_manager.erase(explosion_manager.begin()+i);
		}
	}
}

void gameManager::playerShoot(irr::IrrlichtDevice *graphics,Player* CPlayer)
{

	//The same thing that applies to ships applies to the player
	//However, the Player projectiles must also come from the turret
	//and shoot different projectiles depending on what type of turret the player has installed

	if(CPlayer->cannonFired.primary==true)
	{
		//Scan through the turrets that the player has
		for(int i=0;i<CPlayer->turret_manager.size();i++)
		{
			if(CPlayer->turret_manager[i]->type==TYPE_TURRET_RAIL)
			{
				////sound->play3D("res/rail.wav",CPlayer->turret_manager[i]->getPos());
				projectile *p  = new railgunShot(graphics, CPlayer->turret_manager[i]->getPos(), CPlayer->turret_manager[i]->getRot(),CPlayer->ship);
				projectile_manager.push_back(p);
			}
		}
	}

	if(CPlayer->getHull()<1)
	{
		if(playerDead==false)
		{
			explosion* e = new explosion(graphics,CPlayer->getPos());
			explosion_manager.push_back(e);
			playerDead=true;
		}
	}
}


//legacy code, probably won't be used
void gameManager::addProjectile(projectile* shot)
{
	projectile_manager.push_back(shot);
}

//used by the gameloop object to add ships to the ship manager
void gameManager::addShip(CShip* ship)
{
	ship_manager.push_back(ship);
}

void gameManager::addPlanet(planet* p)
{
	planet_manager.push_back(p);
}

CShip* gameManager::getTarget()
{
	//Some pretty basic code here
	//Just scan through all the ships inside the ship_manager array
	//and then get the position of the mouse
	//get the distance between the target icon on the ship and the mouse using trig
	//then if the mouse is closer than 32 pixels to the target icon thing
	//select the ship then return it to the player_target variable inside the gameloop object
	vector2d<int> t;
	t.X = receiver->mouseX();
	t.Y = receiver->mouseY();

	for(int i=0;i<ship_manager.size();i++)
	{
		vector2d<int> pos;
		pos = ship_manager[i]->array_pos;
		const float x = t.X-pos.X;
		const float y = t.Y-pos.Y;
		float dist = sqrt(x*x + y*y);
		if(dist<32)
		{
			return ship_manager[i];
		}
	}

	//Makes sure game doesn't crash
	return 0;
}
