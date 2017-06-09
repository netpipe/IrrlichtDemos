#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "irrlicht.h"
#include "irrKlang.h"
#include "projectile.h"
#include "ship.h"
#include "player.h"
#include "planet.h"
#include "keylistener.h"
#include "explosion.h"

#include "vector"


//class code
class gameManager
{
public:
	//vectors, how could i live without you
	std::vector<projectile*> projectile_manager;
	std::vector<CShip*> ship_manager;
	std::vector<planet*> planet_manager;
	std::vector<explosion*> explosion_manager;

	bool playerDead;

	gameManager(irr::IrrlichtDevice *graphics, KeyListener* receiver);
	~gameManager();
	irr::IrrlichtDevice *graphics;
	KeyListener* receiver;
	void gameManagerLoop(f32 frameDeltaTime, irrklang::ISoundEngine *sound, Player* CPlayer);
	void addProjectile(projectile* shot);
	void createProjectile();
	void addShip(CShip* ship);
	void createShip();
	void addPlanet(planet* p);
	void planetAnimationManager(f32 frameDeltaTime);
	void explosionAnimationManager();
	void playerShoot(irr::IrrlichtDevice *graphics,Player *CPlayer, irrklang::ISoundEngine *sound);
	CShip* getTarget();

};

#endif
