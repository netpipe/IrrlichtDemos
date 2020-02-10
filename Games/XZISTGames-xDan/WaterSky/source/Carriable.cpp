
#include "Carriable.h"
#include "Actor.h"
#include "SoundAnimator.h"

Carriable::Carriable(WorldBase *world)
    : Object(world)
{
    beingCarried = false;
}

void Carriable::update(f32 dt)
{
    if (!beingCarried)
    {
        Object *actor = getNearestObject(OBJECT_TYPE_PLAYER);
        if (actor)
        {
            if (getBoundsDistance(actor) < 0.0)
            {
                ((Actor *)actor)->carry(this);
                beingCarried = true;

//                if (soundAnimator && pickUpSound.size())
//                {
//                    soundAnimator->play(pickUpSound, 0);
//                }
            }
        }
    }
}
