#ifndef LASER_H_
#define LASER_H_

#include <ode/ode.h>
#include <vector3d.h>
#include <SMaterial.h>
#include "Weapon.h"

class Laser : public Weapon
{
private:


	irr::video::SMaterial mat;

	irr::video::SColor color;

	const void draw(const irr::core::vector3df& start, const irr::core::vector3df& end) const;

public:

	Laser(const Game* game);

	const void fire(const Ship* ship, const Terrain* terrain) const;
};

#endif /*LASER_H_*/
