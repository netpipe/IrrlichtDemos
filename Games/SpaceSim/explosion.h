#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "irrlicht.h"
//#include "irrKlang.h"

using namespace irr;

class explosion
{
public:
	int last_time;
	bool end;
	irr::IrrlichtDevice *graphics;
	scene::ISceneNode *node;
	scene::IParticleSystemSceneNode *ps;
	scene::IParticleSystemSceneNode *debris;
	scene::ISceneNode *shockwave;
	void drop();
	explosion(irr::IrrlichtDevice *graphics, core::vector3df pos);
	void loop();
	~explosion();
};

#endif
