#ifndef _SHIP_H
#define _SHIP_H


//ship base class
//use this to make ships
//ai and player ships

#include "irrlicht.h"
#include "irrKlang.h"
#include "sTypes.h"
#include "projectile.h"
#include "explosion.h"


#include "time.h"
#include "vector"

using namespace irr;
using namespace scene;

enum AI_STATE
{
	STATE_ALERT,
	STATE_PATROL,
	STATE_FLEE,
	STATE_REARM
};

class CShip
{
public:

	//disgusting

	int last_time;


	AI_STATE state;
	struct turningCircle
	{
		float X;
		float Y;
		float Z;
	};
	struct turningCircle turn;

	struct cannonFired
	{
		int primary;
		bool primary_shoot;
		int secondary;
		bool secondary_shoot;
	};
	struct cannonFired cannon;

	struct fireC
	{
		float X;
		float Y;
		float Z;
	};
	struct fireC fireRot;

	std::vector<projectile*> projectile_manager;

	//ship stat variables
	int hullPoints;
	int armorPoints;
	int shieldPoints;

	int maxVelocity;
	float velocity;
	float maxTurn;

	bool hostile;
	bool shoot;

	char name;

	int maxShieldPoints;
	int maxArmorPoints;
	int maxHullPoints;

	gui::IGUIImage *target_array;

	core::vector3df *ship_position;
	core::vector3df *ship_rotation;

	core::vector3df sRot;
	core::vector3df sPos;
	core::vector3df rotSlow;
	core::vector3df posSlow;

	core::vector2d<s32> array_pos;

	scene::IBoneSceneNode *turret1_node;
	scene::IBoneSceneNode *turret2_node;
	scene::IBoneSceneNode *turret3_node;
	scene::IBoneSceneNode *turret4_node;

	scene::IBoneSceneNode *exhaust_01;
	scene::IBoneSceneNode *exhaust_02;
	scene::IBoneSceneNode *exhaust_03;


	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;
	irr::scene::IAnimatedMeshSceneNode *ship;
	f32 frameDeltaTime;

	ship_class s_class;
	ship_faction faction;
	ship_types type;


	CShip(irr::IrrlichtDevice *graphics=(irr::IrrlichtDevice *)0,
		irrklang::ISoundEngine *sound = (irrklang::ISoundEngine *) 0,
		const core::vector3df& ship_position = core::vector3df(0,0,0),
		ship_class s_class= TERR_IMPERIUM_FRIGATE,
		ship_faction faction= FACTION_TERRAN_ALLIANCE,
		ship_types type=TYPE_SHIP,
		const wchar_t *name = (const wchar_t*)0);


	virtual ~CShip();

	float getDistToPoint(core::vector3df& point);
	void AIRun(f32 frameDeltaTime);
	void movement(f32 frameDeltaTime);
	void damageShip(int damage);
	void drop();

	void rotateToPoint(core::vector3df point);
	void rotateAwayFromPoint(core::vector3df& point);
	void shootAtPoint(core::vector3df point);
	void engagePlayer(core::vector3df playerpos);
	void dodgeFire();

	void ableToShoot();
	projectile *addShot(core::vector3df pos);

	int getHullPoints() const;
	int getArmorPoints() const;
	int getShieldPoints() const;
	bool getHostileToPlayer() const;
	float getVelocity();
	core::vector3df getPos() const;
	core::vector3df getRot() const;
	ship_class getShipClass();
	ship_faction getShipFaction();



};

#endif _SHIP_H
