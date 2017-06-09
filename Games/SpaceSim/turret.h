#ifndef _TURRET_H_
#define _TURRET_H_

#include "irrlicht.h"
#include "ship.h"

#include "vector"



using namespace irr;
using namespace core;
using namespace scene;

class turret
{
public:
	irr::IrrlichtDevice *graphics;
	irr::scene::IAnimatedMeshSceneNode *t_model;
	core::vector3df offset;

	turret_class type;
	int turret_number;
	int rotate_speed;
	int reload_time;
	int max_range;

	turret(irr::IrrlichtDevice *graphics, core::vector3df position, core::vector3df rotation, core::vector3df offset, int turret_number, turret_class type);
	~turret();

	void aimTurret(core::vector3df newrot, f32 frameDeltaTime);
	void setPos(core::vector3df newpos);
	core::vector3df getRot() const;
	core::vector3df getPos() const;
	core::vector3df getOffset();

	int getTurretNum() const;
};

#endif
