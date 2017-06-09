#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "irrlicht.h"
#include "irrKlang.h"
#include "math.h"

using namespace irr;
using namespace scene;

class projectile
{
public:
	irr::IrrlichtDevice *graphics;
	core::vector3df pos;
	core::vector3df rot;
	core::vector3df pPos;
	irr::scene::IMeshSceneNode *bullet;

	irr::scene::IAnimatedMeshSceneNode *ship;
	scene::IParticleSystemSceneNode *ps;
	f32 frameDeltaTime;

//	projectile():graphics(0){};
	projectile(irr::IrrlichtDevice *graphics, core::vector3df pos = core::vector3df(0,0,0), core::vector3df rot = core::vector3df(0,0,0), irr::scene::IAnimatedMeshSceneNode *ship = 0);
	~projectile();

	void projRun(f32 frameDeltaTime);
	int damage;
	int range;
	virtual int checkRange();
	virtual void drop();

	core::vector3df getRot()
	{
		return bullet->getRotation();
	}
	IAnimatedMeshSceneNode *getShip()
	{
		return ship;
	}

protected:
	int velocity;

};

//other projectile classes
//railgun
class railgunShot : public projectile
{
public:
	railgunShot(irr::IrrlichtDevice *graphics, core::vector3df pos,  core::vector3df rot,irr::scene::IAnimatedMeshSceneNode *ship);
};

//photon cannon
class photonCannonShot : public projectile
{
public:
	photonCannonShot(irr::IrrlichtDevice *graphics, core::vector3df pos,  core::vector3df rot,irr::scene::IAnimatedMeshSceneNode *ship);
};


class ionCannonShot : projectile
{
public:

};

#endif
