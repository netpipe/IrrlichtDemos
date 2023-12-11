
#ifndef COLLISION_MATERIAL_INTERACTION_H
#define COLLISION_MATERIAL_INTERACTION_H

#include "litha_internal.h"
#include "collision_structs.h"

class IInteractionPreparator;

struct CollisionMaterialInteraction
{
	CollisionMaterialInteraction()
	{
		friction = 100.f;
		preparator = NULL;
	}
	
	f32 friction;
	
	// A preparator can be used to modify the interaction settings depending on the circumstances of the collision.
	// This is only useful in rare circumstances.
	// Remember to keep the pointer valid!
	IInteractionPreparator *preparator;
};

// a class that operates on an interaction
class IInteractionPreparator
{
public:
	// Provide custom interaction settings depending on the state of the Collision.
	// Changes to the interaction are only temporary.
	virtual void Prepare(CollisionMaterialInteraction &interaction, const Collision &collision) = 0;
};

#endif

