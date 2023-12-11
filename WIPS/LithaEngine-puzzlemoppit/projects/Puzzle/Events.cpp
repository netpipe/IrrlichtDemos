
#include "Events.h"
#include "Level.h"
#include "Map.h"

IMapEventOwner::IMapEventOwner(Level *level)
{
	this->level = level;
	this->map = level->GetMap();
}


// ****************** Default Event ************************
// This default event is kind of hacked in so more advanced
// stuff like sliding boxes is possible.
// Gravity could probably have been put here too...

DefaultEvent::DefaultEvent(Level *level) : IMapEventOwner(level)
{
	fallblockSoundSlot = 0;
	
	fallblockSounds[0] = GetEngine()->GetSoundSystem()->CreateSound3D();
	fallblockSounds[1] = GetEngine()->GetSoundSystem()->CreateSound3D();
	fallblockSounds[2] = GetEngine()->GetSoundSystem()->CreateSound3D();
}

DefaultEvent::~DefaultEvent()
{
	fallblockSounds[0]->drop();
	fallblockSounds[1]->drop();
	fallblockSounds[2]->drop();
}

void DefaultEvent::MoveStack(const core::vector3di &belowStack, const core::vector3di &moveDir)
{
	if (moveDir.Y == 0) // only move stack when moving in horizontal plane. (not when falling)
	{
		core::vector3di stackPos = belowStack;
		
		// For each stacked block
		while (true)
		{
			stackPos.Y ++;
			
			// No object, end.
			if (!map->GetObject(stackPos))
				break;
			
			// Hit some ground or other immovable object.
			if (!map->IsObjectMovable(stackPos))
				break;
			
			// Hit a balloon! These are considered weightless, and may support other blocks on top of
			// themselves, so we don't move them.
			if (map->GetObjectType(stackPos) == EOT_BALLOON)
				break;
			
			// Ok, it's a block we can move.
			// Try to move it.
			// If the move fails, (e.g. the destination contains an object), we don't move
			// any more of the stack above it.
			if (!level->StartMapObjectMove(stackPos, stackPos+moveDir))
				break;
		}
	}
}

void DefaultEvent::OnMapEvent(core::vector3di coord)
{
	if (map->GetObject(coord) && map->GetIsObjectSliding(coord))
	{
		// We are currently sliding, but now clear sliding state.
		// It will be set again within map.StartObjectMove if a horizontal
		// plane movement succeeds.
		map->SetIsObjectSliding(coord, false);
		
		switch (map->GetObjectType(coord))
		{
		// Sliding block logic
		case EOT_SLIDING_BLOCK:
			// If already moving...
			if (map->ObjectHasPreviousCoord(coord))
			{
				E_EVENT_TYPE eventType = EET_UNKNOWN;
				
				if (map->GetEvent(coord))
					eventType = map->GetEvent(coord)->GetType();
				
				// Don't slide on fan or lift event squares
				if ( !(eventType == EET_FAN_EVENT || eventType == EET_LIFT_EVENT) )
				{
					core::vector3di slideDir = coord - map->GetObjectPreviousCoord(coord);
					
					if (slideDir.Y == 0) // no vertical sliding...
					{
						if (level->StartMapObjectMove(coord, coord+slideDir))
							MoveStack(coord, slideDir);
					}
				}
			}
			break;
		
		// BALLOON.
		// Same as sliding block?
		// EXCEPT, we allow sliding across lift event squares.
		case EOT_BALLOON:
			// If already moving...
			if (map->ObjectHasPreviousCoord(coord))
			{
				E_EVENT_TYPE eventType = EET_UNKNOWN;
				
				if (map->GetEvent(coord))
					eventType = map->GetEvent(coord)->GetType();
				
				// Don't slide on fan event squares
				if ( !(eventType == EET_FAN_EVENT) )// || eventType == EET_LIFT_EVENT) )
				{
					core::vector3di slideDir = coord - map->GetObjectPreviousCoord(coord);
					
					if (slideDir.Y == 0) // no vertical sliding...
					{
						if (level->StartMapObjectMove(coord, coord+slideDir))
							MoveStack(coord, slideDir);
					}
				}
			}
			break;
		}
	}
}

void DefaultEvent::OnMapMoveEvent(core::vector3di coord)
{
	if (map->GetObject(coord))
	{
		ASSERT( map->ObjectHasPreviousCoord(coord) );
		
		switch (map->GetObjectType(coord))
		{
		// Sliding block or balloon just started moving.
		// So we want to find all blocks stacked above it on the previous location,
		// and move them too.
		// (all blocks stacked until we reach an immovable block or a weightless balloon)
		case EOT_SLIDING_BLOCK:
		case EOT_BALLOON:
			{
				core::vector3di prevCoord = map->GetObjectPreviousCoord(coord);
				
				MoveStack(prevCoord,
						coord - prevCoord);
			}
			break;
		}
	}
}

// A special emitter that can wrap a built in Irrlicht emitter.
// This emitter causes an entire seconds worth of particles to be emitted at once,
// after which the emitter stops emitting.
class SingleEmissionEmitter : public scene::IParticleEmitter
{
	scene::IParticleEmitter *baseEmitter;
	
	bool firstEmit;
	
public:
	SingleEmissionEmitter(scene::IParticleEmitter *baseEmitter)
	{
		this->baseEmitter = baseEmitter;
		baseEmitter->grab();
		
		firstEmit = true;
	}
	
	~SingleEmissionEmitter()
	{
		baseEmitter->drop();
	}
	
	s32 emitt(u32 now, u32 timeSinceLastCall, scene::SParticle*& outArray)
	{
		s32 emitted = 0;
		
		if (firstEmit)
		{
			// We ignore timeSinceLastCall, instead setting it to 1000 to emit
			// an entire second worth of particles.
			emitted = baseEmitter->emitt(now, 1000, outArray);
			
			// And then stop emitting.
			firstEmit = false;
		}
		
		return emitted;
	}
	
	void setDirection( const core::vector3df& newDirection )
	{
		baseEmitter->setDirection(newDirection);
	}

	void setMinParticlesPerSecond( u32 minPPS )
	{
		baseEmitter->setMinParticlesPerSecond(minPPS);
	}

	void setMaxParticlesPerSecond( u32 maxPPS )
	{
		baseEmitter->setMaxParticlesPerSecond(maxPPS);
	}

	void setMinStartColor( const video::SColor& color )
	{
		baseEmitter->setMinStartColor(color);
	}

	void setMaxStartColor( const video::SColor& color )
	{
		baseEmitter->setMaxStartColor(color);
	}

	void setMaxStartSize( const core::dimension2df& size )
	{
		baseEmitter->setMaxStartSize(size);
	}

	void setMinStartSize( const core::dimension2df& size )
	{
		baseEmitter->setMinStartSize(size);
	}

	const core::vector3df& getDirection() const
	{
		return baseEmitter->getDirection();
	}

	u32 getMinParticlesPerSecond() const
	{
		return baseEmitter->getMinParticlesPerSecond();
	}

	u32 getMaxParticlesPerSecond() const
	{
		return baseEmitter->getMaxParticlesPerSecond();
	}

	const video::SColor& getMinStartColor() const
	{
		return baseEmitter->getMinStartColor();
	}

	const video::SColor& getMaxStartColor() const
	{
		return baseEmitter->getMaxStartColor();
	}

	const core::dimension2df& getMaxStartSize() const
	{
		return baseEmitter->getMaxStartSize();
	}

	const core::dimension2df& getMinStartSize() const
	{
		return baseEmitter->getMinStartSize();
	}

	scene::E_PARTICLE_EMITTER_TYPE getType() const { return scene::EPET_POINT; }
};

void DefaultEvent::OnPause()
{
	fallblockSounds[0]->Pause();
	fallblockSounds[1]->Pause();
	fallblockSounds[2]->Pause();
}

void DefaultEvent::OnResume()
{
	fallblockSounds[0]->Resume();
	fallblockSounds[1]->Resume();
	fallblockSounds[2]->Resume();
}

void DefaultEvent::OnMapLeaveEvent(core::vector3di coord, core::vector3di newCoord)
{
	// There may not be any object on this coord since the object has just left it!
	// Object *may* be on newCoord, although I'm not 100% certain it will be, so
	// I must GetObject check before using.
	
	
	// Falling ground block.
	// Is block below the previous coord a EOT_GROUND_BLOCK_FALL?
	// And this object is not a balloon... (i.e. not weightless)
	// Then that block should fall...? Start falling, at least.
	
	core::vector3di below = coord + core::vector3di(0,-1,0);
	
	if (map->GetObject(below) && map->GetObjectType(below) == EOT_GROUND_BLOCK_FALL)
	{
		// And check that the object that just moved was not a weightless balloon...
		// (but if the object is not present in the new location, then continue regardless)
		if ( !map->GetObject(newCoord) || map->GetObjectType(newCoord) != EOT_BALLOON)
		{
			// Just remove...
			// falling is too complex :S
			level->RemoveObject(below);
			
			// sfx
			
			core::vector3df fallblockPos3d = level->GetPosFromCoord(below);
			fallblockSounds[fallblockSoundSlot]->SetPosition( fallblockPos3d );
			fallblockSounds[fallblockSoundSlot]->Play("../projects/Puzzle/media/sfx/fallblock.ogg");
			
			// circular slots
			// since we may want several fallblock sounds at once
			// i.e. so old sound isn't stopped as soon as the next falls.
			
			fallblockSoundSlot ++;
			
			if (fallblockSoundSlot >= 3)
				fallblockSoundSlot = 0;
			
			
			// Particle effects
			
			scene::ISceneManager *smgr = GetEngine()->GetIrrlichtDevice()->getSceneManager();
			video::IVideoDriver *driver = GetEngine()->GetIrrlichtDevice()->getVideoDriver();
			
			scene::IParticleSystemSceneNode *ps = smgr->addParticleSystemSceneNode(false);
			ps->setPosition(fallblockPos3d);
			ps->setMaterialFlag(video::EMF_LIGHTING, false);
			ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
			ps->setMaterialTexture(0, driver->getTexture("dust.png"));
			//ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
			ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
			
			u32 lifeTime = 1000;
			f32 minParticleSize = 0.5;
			f32 maxParticleSize = 0.8;
			
			ps->setScale( core::vector3df(1,1,1) * 0.1 );
			
			scene::IParticleEmitter *emitter = ps->createBoxEmitter(
					core::aabbox3df(-4,-4,-4, 5,5,5),
					core::vector3df(0,-0.00005,0),
					20,30,
					video::SColor(0,100,100,100),//colour lower bound
					video::SColor(0,200,200,200),//colour upper bound
					lifeTime,//min lifetime
					lifeTime,//max lifetime
					360,//max angular difference from velocity direction
					core::dimension2df(minParticleSize,minParticleSize),//minimum size
					core::dimension2df(maxParticleSize,maxParticleSize)//maximum size
					);
			
			// Emit entire one second's worth of particles all at once.
			// Then no more...!
			scene::IParticleEmitter *singleEmissionEmitter = new SingleEmissionEmitter(emitter);
			emitter->drop();
			
			ps->setEmitter(singleEmissionEmitter);
			singleEmissionEmitter->drop();
			
			
			scene::IParticleAffector *affector = ps->createFadeOutParticleAffector();
			ps->addAffector(affector);
			affector->drop();
			
			
			scene::ISceneNodeAnimator *deletionAnim = smgr->createDeleteAnimator(4000);
			ps->addAnimator(deletionAnim);
			deletionAnim->drop();
		}
	}
}

bool DefaultEvent::RequestActionPermission(core::vector3di coord, E_ACTION_TYPE action)
{
	// Handle any specific event first.
	// Denial overrides allowal.
	if (IMapEventOwner *event = map->GetEvent(coord))
	{
		if (!event->RequestActionPermission(coord, action))
			return false;
	}
	
	// Now default event stuff.
	
	// A balloon should not be able to fall. (no gravity)
	if (action == EAT_FALL && map->GetObject(coord) && map->GetObjectType(coord) == EOT_BALLOON)
	{
		return false;
	}
	
	// Can an object be moved by the player pushing it?
	if (action == EAT_MOVE_BY_PUSH)
	{
		core::vector3di above = coord + core::vector3di(0,1,0);
		
		// Can't be pushed if there is another object above this one.
		if (map->GetObject(above)
				// Except if it is a weightless balloon...
				&& map->GetObjectType(above) != EOT_BALLOON
				// And the object itself is NOT a sliding block or a balloon
				// (as they can be pushed when stuff is stacked on them)
				&& (!map->GetObject(coord) || (map->GetObjectType(coord) != EOT_SLIDING_BLOCK
					&& map->GetObjectType(coord) != EOT_BALLOON))
				)
		{
			return false;
		}
	}
	
	return true;
}

// ****************** Fan Event ************************

void FanEvent::OnMapEvent(core::vector3di coord)
{
	// Move block upwards. Unless top event.
	// How to determine if top event: This same event will not be set for the square above.
	
	core::vector3di above = coord + core::vector3di(0,1,0);
	
	
	// Change character's animation when in the fan updraft
	if (map->GetObject(coord) && map->GetObjectType(coord) == EOT_PLAYER_CENTRE)
	{
		if (IProxyTransformable *proxy = dynamic_cast<IProxyTransformable *>(map->GetObject(coord)))
		{
			if (ICharacter *character = dynamic_cast<ICharacter *>(proxy->GetProxiedTransformable()))
			{
				// Hacked in.
				// So it looks better when the player gets stuck with a block below
				// (if he's standing upright, why can't he just walk off the block!?)
				character->SetAnimations(ANIM_FALL, -1);
			}
		}
	}
	
	// this flag just used for the player, to detect when he has stopped moving upwards
	// in a fan updraft, so can send restart level event and make him bob.
	// Finished moving upwards either when there are no more fan events left, or if
	// the upwards move attempt fails (i.e. due to obstruction by another object that is
	// above the player in the fan vortex)
	bool finishedMovingUpwards = false;
	
	if (map->GetEvent(above) == this)
	{
		// Move block upwards.
		if (!level->StartMapObjectMove(coord, above))
			finishedMovingUpwards = true;
	}
	else
	{
		// Top event.
		finishedMovingUpwards = true;
	}
	
	if (finishedMovingUpwards)
	{
		// Just captured the player?
		// Make it bob!
		if (map->GetObject(coord) && map->GetObjectType(coord) == EOT_PLAYER_CENTRE)
		{
			// this would work for any Actor
			if (IProxyTransformable *proxy = dynamic_cast<IProxyTransformable *>(map->GetObject(coord)))
			{
				if (ICharacter *character = dynamic_cast<ICharacter *>(proxy->GetProxiedTransformable()))
				{
					IBobAnimator *anim = GetEngine()->GetWorld()->CreateBobAnimator(0.1, 1.0);
					character->AddAnimator(anim);
					anim->SetStartPos(character->GetPosition());
					anim->drop();
					
					NOTE << "Character got stuck in a fan.";
					
					// Send a RestartLevel event in 10 seconds time.
					Event event("ApplyUndo");
					level->TimedEvent(event, 7);
				}
			}
		}
	}
}

void FanEvent::OnMapLeaveEvent(core::vector3di coord, core::vector3di newCoord)
{
	// Object no longer here!
	// So should move any objects below upwards.
	// Providing "below" contains a fan event AND an object.
	
	// This move will in turn generate a OnMapLeaveEvent for the location below this one.
	// So all blocks will move up correctly.
	
	core::vector3di below = coord + core::vector3di(0,-1,0);
	
	if (map->GetEvent(below) == this && map->GetObject(below))
	{
		// Hack: player might be intersecting the destination of the block...
		// (we don't want player just slightly touching the fan events to stop
		// blocks moving upwards, it doesn't look right)
		level->ShovePlayerOutOfTheWay(coord);
		
		level->StartMapObjectMove(below, coord);
	}
}

bool FanEvent::RequestActionPermission(core::vector3di coord, E_ACTION_TYPE action)
{
	switch (action)
	{
	case EAT_FALL:
		return false;
	case EAT_LOCOMOTE:
		// May NOT locomote if on top most square.
		// OR if the square above is occupied
		return !(
				map->GetEvent(coord + core::vector3di(0,1,0)) != this
					|| map->GetObject(coord + core::vector3di(0,1,0))
				);
	case EAT_MOVE_BY_PUSH: // can be pushed if at the topmost location
		return map->GetEvent(coord + core::vector3di(0,1,0)) != this;
	};
	FAIL << "shouldn't have reached here";
}

// ****************** Lift Event ************************

bool LiftEvent::IsLiftEndLocation(core::vector3di coord)
{
	// Is lift at either top or bottom of its path?
	
	if (map->GetEvent( coord + core::vector3di(0,-1,0) ) != this)
		return true; // at bottom, there is not a LiftEvent below this square
	
	// There is a lift event one square above this, but isn't two squares above.
	// (at top)
	if (map->GetEvent( coord + core::vector3di(0,1,0) ) == this
			&& map->GetEvent( coord + core::vector3di(0,2,0) ) != this)
	{
		return true;
	}
	
	return false;
}

void LiftEvent::MoveInDirection(core::vector3di liftCoord, E_MOVE_DIRECTION moveDir)
{
	if (moveDir == EMD_DOWN)
	{
		// Just move the lift down one square. Gravity will move any objects resting on it later.
		level->StartMapObjectMove( liftCoord, liftCoord + core::vector3di(0,-1,0), true );
	}
	else
	{
		// Moving up is more complicated.
		
		// iterate upwards from the square above the lift, until either an immovable object,
		// or an empty (no object) square is found.
		
		core::vector3di coord = liftCoord;
		
		while (1)
		{
			coord.Y ++;
			
			ITransformable *object = map->GetObject(coord);
			
			if (!object)
			{
				// An empty square was found
				// So iterate back downwards moving each object up by one
				// (including the lift itself)
				
				while (1)
				{
					coord.Y --;
					
					// forceMove is set to enable moving of the usually immovable Lift
					level->StartMapObjectMove(coord, coord + core::vector3di(0,1,0), true);
					
					if (coord.Y == liftCoord.Y)
						break;
				}
				
				break;
			}
			else if (!map->IsObjectMovable(coord))
			{
				// an immovable object was found, lift movement is obstructed.
				// so do nothing.
				break;
			}
		}
	}
}

void LiftEvent::OnMapEvent(core::vector3di coord)
{
	/*
	(A)
	
	- does this square contain an object
	- is this square object NOT a lift
	- is this square above a lift (square below this is contains a lift object)
	- is the lift on the square below at an end location (either bottom event square or one from top, see diagram)
	
	So attempt to move.
	
	Determine direction to move, up or down:
	- if the square below the lift is NOT a lift event (not event == this), then should move upwards
	- if the square two above the lift is NOT a lift event, then should move downwards
	- [if both upwards and downwards, then do nothing...]
	
	Actual move:
	- if moving down, then just move the lift down one square. Gravity will do the rest.
	- if moving up, do as follows.
	
	Moving up:
	- iterate upwards from the square above the lift, until either an immovable object, or an empty (no object)
		square is found.
	- if an immovable object is found, cannot move, so do nothing.
	- if an empty square is found, then iterate downwards again, moving each object upwards by one, including
		the lift when it is reached. (end iteration at the lift)
	
	TEST THIS BEFORE IMPLEMENTING OTHER CASES.
	- test with lift at top and bottom of path, with zero, one, two, and more objects stacked on it(???)
	- correct operation should be for the lift to move one square (up or down as the case may be)
		and then stop. All objects resting on the lift to move as well.
	
	*/
	
	ITransformable *object = map->GetObject(coord);
	
	// ensure this square contains an object
	//ASSERT(object);
	
	// removed assertion, as sometimes this *is* getting called on a square with no event.
	// not sure how...
	if (!object)
		return;
	
	core::vector3di coordBelow = coord + core::vector3di(0,-1,0);
	
	
	if (map->GetObjectType(coord) != EOT_LIFT	// is this square NOT a lift
			&& map->GetObject(coordBelow)		// square below contains an object
			&& map->GetObjectType(coordBelow) == EOT_LIFT	// and object below IS a lift
			&& IsLiftEndLocation(coordBelow)	// and the lift is at an end location
			)
	{
		core::vector3di liftCoord = coordBelow;
		
		// Determine move direction.
		
		// Square below lift is NOT a LiftEvent, at bottom so move upwards.
		bool moveUp = map->GetEvent(liftCoord + core::vector3di(0,-1,0)) != this;
		
		// Square two above lift is NOT a LiftEvent, at top so move downwards.
		bool moveDown = map->GetEvent(liftCoord + core::vector3di(0,2,0)) != this;
		
		
		if (moveUp && moveDown)
		{
			// Cannot move both up and down, so do nothing.
		}
		else
		{
			// need also to check that the lift has not only just reached its end location
			// if going up, check that object last position was NOT one below
			// if going down, check that object last position was NOT one block above
			// if object has no previous coordinate, i.e. was positioned initially on this location, then will move.
			
			if ( moveUp && ( !map->ObjectHasPreviousCoord(coord) || map->GetObjectPreviousCoord(coord) != coord + core::vector3di(0,1,0) ) )
			{
				MoveInDirection(liftCoord, EMD_UP);
				level->LiftMoveInc();
			}
			else if ( moveDown && ( !map->ObjectHasPreviousCoord(coord) || map->GetObjectPreviousCoord(coord) != coord + core::vector3di(0,-1,0) ) )
			{
				MoveInDirection(liftCoord, EMD_DOWN);
				level->LiftMoveInc();
			}
		}
	}
	
	/*
	(B)
	
	- does this square contain an object
	- is the square object a lift
	- is this NOT an end location (detect end location as in (A))
	
	We can assume as the lift is not at an end location that it has moved at least once on a journey from
	one end to the other. So we must determine the direction to continue moving in...
	
	- determine move direction (using lift GetPreviousCoordinate. May need to implement the Map method for this).
	- move in that direction (moving as in (A))
	
	CheckForEndlocation and MoveInDirection could be implemented as separate methods as the functionality
	is common to both (A) and (B).
	
	TEST.
	- test similar to (A) except now the lift should continue to its destination.
	- it must also be usable again after reaching its destination
	
	*/
	
	if (map->GetObject(coord)	// Does this square contain an object
			&& map->GetObjectType(coord) == EOT_LIFT	// and object IS a lift
			&& !IsLiftEndLocation(coord))				// and is NOT at an end location
	{
		// We can assume since the lift is not at an end location it has moved at least once on a journey
		// from one end to another
		
		// So need to determine lift move direction.
		
		//ASSERT( map->ObjectHasPreviousCoord(coord) );
		
		if (!map->ObjectHasPreviousCoord(coord))
			return;
		
		core::vector3di previousCoord = map->GetObjectPreviousCoord(coord);
		
		if ( coord.Y > previousCoord.Y )
			MoveInDirection(coord, EMD_UP);
		else
			MoveInDirection(coord, EMD_DOWN);
	}
	
	/*
	(C)
	
	This deals with if the lift's path is blocked by another object.
	
	- does this square contain an object	]
	- is this square object NOT a lift		] These three like (A)
	- is this square above a lift			]
	- is the lift on the square below NOT at an end location	] this differs from (A)
	
	Determine if the lift can move in EITHER direction:
	- for going down, check if the square below the lift is empty. If so, it can move ok.
	- for going up, do iteration upwards as in (A). If an empty square is reached BEFORE an immovable object, can move ok.
	
	If the lift can only move in one direction, then this is the direction it should move in!
	- MoveInDirection as in both (A) and (B).
	
	Note that in theory (B) and (C) could both be called here. This is not a problem as moves will just fail
	if the objects have already had their moves started.
	
	TESTING.
	- have an object or objects (movable or not), placed in the lifts path...
	- this (C) probably needs thinking over again to ensure it is 100% correct.
	- (then again, (A) and (B) probably do too)
	
	*/
	
	if (map->GetObject(coord) // need to first check for object again, in case it was moved in (B)
			&& map->GetObjectType(coord) != EOT_LIFT		// is this square NOT a lift
			&& map->GetObject(coordBelow)					// square below contains an object
			&& map->GetObjectType(coordBelow) == EOT_LIFT	// and object below IS a lift
			&& !IsLiftEndLocation(coordBelow)				// and the lift is NOT at an end location
			)
	{
		core::vector3di liftCoord = coordBelow;
		
		// Can move up?
		// Iterate upwards, if an empty square is reached before an immovable object, then can move upwards.
		
		bool moveUp = false;
		
		core::vector3di probeCoord = liftCoord;
		
		while (1)
		{
			probeCoord.Y ++;
			
			ITransformable *object = map->GetObject(probeCoord);
			
			// if empty square, can move ok
			if (!object)
			{
				moveUp = true;
				break;
			}
			// if immovable object, cannot move
			else if (!map->IsObjectMovable(probeCoord))
			{
				break;
			}
		}
		
		// Can move down if the square below lift is empty
		bool moveDown = map->GetObject(liftCoord + core::vector3di(0,-1,0)) == NULL;
		
		if (moveUp && moveDown)
		{
			// Cannot move both up and down, so do nothing.
		}
		else
		{
			if (moveUp)
				MoveInDirection(liftCoord, EMD_UP);
			else if (moveDown)
				MoveInDirection(liftCoord, EMD_DOWN);
		}
	}
}

bool LiftEvent::RequestActionPermission(core::vector3di coord, E_ACTION_TYPE action)
{
	switch (action)
	{
	case EAT_FALL:
		return true;
	case EAT_LOCOMOTE:
		// Need to make this true if the object is above an end location.
		//return IsLiftEndLocation( coord + core::vector3di(0,-1,0) );
		
		// FIX: Need this otherwise player can get trapped when standing on a block that is
		// on a lift.
		// But was there a good reason for the old behaviour???!
		return true;
	case EAT_MOVE_BY_PUSH: // objects can always be pushed off a lift
		return true;
	};
	FAIL << "shouldn't have reached here";
}




