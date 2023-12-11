#ifndef NPC_H_
#define NPC_H_

#include "../core/GameObject.h"

/**
 * The NPC class. <no description yet>
 */
class NPC : public GameObject
{

public:

	NPC(Game* game,
		const irr::core::stringc& name,
		const irr::core::stringc& meshPath,
		const irr::core::stringc& texture,
		const irr::core::vector3df& position = irr::core::vector3df(0.0f, 0.0f, 0.0f),
		const irr::core::vector3df& rotation = irr::core::vector3df(0.0f, 0.0f, 0.0f),
		const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f),
		
		const irr::f32 density = 1.0f,
		const irr::f32 weight = 1.0f
	) :
		GameObject(game, name, meshPath, texture, position, rotation, scale, density, weight, false, false)
	{
		
	}
};

#endif /*NPC_H_*/
