#include <irrlicht.h>

using namespace irr;

#include "player.h"

scene::ISceneNodeAnimatorCollisionResponse* camCollisionResponse( IrrlichtDevice * device )
{
	scene::ICameraSceneNode *camera = device->getSceneManager()->getActiveCamera();
	scene::ISceneNodeAnimatorCollisionResponse *a = 0;

	core::list<scene::ISceneNodeAnimator*>::ConstIterator it = camera->getAnimators().begin();
	for (; it != camera->getAnimators().end(); ++it)
	{
		a = (scene::ISceneNodeAnimatorCollisionResponse*) (*it);
		if ( a->getType() == scene::ESNAT_COLLISION_RESPONSE )
			return a;
	}

	return 0;
}


//! internal Animation
void setTimeFire ( TimeFire *t, u32 delta, u32 flags )
{
	t->flags = flags;
	t->next = 0;
	t->delta = delta;
}

void checkTimeFire ( TimeFire *t, u32 listSize, u32 now )
{
	u32 i;
	for ( i = 0; i < listSize; ++i )
	{
		if ( now < t[i].next )
			continue;

		t[i].next = core::max_ ( now + t[i].delta, t[i].next + t[i].delta );
		t[i].flags |= FIRED;
	}
}
