#ifndef SHIP_H_
#define SHIP_H_
#include <irrlicht.h>
#include "../core/IHoverCraft.h"

class Terrain;

class Ship : public IHoverCraft
{

private:

	irr::scene::ILightSceneNode* light;

protected:

	///Adds additional Irrlicht stuff.
	const void addIrrlichtStuff();

public:

	Ship(Game* game,
		const irr::core::stringc& name,
		const irr::core::stringc& meshPath,
		const irr::core::stringc& texture,
		const irr::core::vector3df& position = irr::core::vector3df(0.0f, 0.0f, 0.0f),
		const irr::core::vector3df& rotation = irr::core::vector3df(0.0f, 0.0f, 0.0f),
		const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f),

		const dReal density = 1.0f, const dReal weight = 1.0f);

	~Ship();

	///Move ship acording to its rotation.
	const void move(const irr::f32 elapsed) const;
};

#endif /*SHIP_H_*/
