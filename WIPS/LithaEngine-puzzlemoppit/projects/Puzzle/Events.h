
#include "Litha.h"
#include "Enums.h"

class Level;
class Map;


// NOTE: All new events need to be added to mapEventInstances list in
// Level constructor.
class IMapEventOwner : public virtual IReferenceCounted
		// need to be reference counted so can drop
		// (since DefaultEvent inherits IPausable, and thus must be dropped and not deleted)
{
protected:
	Level *level;
	Map *map;
	
public:
	IMapEventOwner(Level *level);
	
	// This is called when an object enters a map location.
	// (after all animation and movement is complete)
	virtual void OnMapEvent(core::vector3di coord) = 0;
	
	// Called as soon as an object starts to enter a new location.
	// I.e. is just leaving the old location.
	// Old location is cached in map. coord is the new location.
	virtual void OnMapMoveEvent(core::vector3di coord) = 0;
	
	// Hacked in.
	// Called whenever an event location is left. (an object moves off this location)
	// coord - the event location that previously had an object in it.
	// newCoord - the location that now should contain the object.
	//		I have not actually verified that the object will always be at newCoord...
	virtual void OnMapLeaveEvent(core::vector3di coord, core::vector3di newCoord) = 0;
	
	// When an object is on this event square, it must request permission of this
	// event owner to perform actions.
	// THIS SHOULD ONLY BE CALLED ON DefaultEvent. Default event in turn calls this for
	// map location specific events.
	virtual bool RequestActionPermission(core::vector3di coord, E_ACTION_TYPE action) = 0;
	
	virtual E_EVENT_TYPE GetType() = 0;
	
	virtual core::stringc GetIconMeshName() = 0;
};

// NOTE: There is only ever one instance of an event; state is kept in map and other places.

class DefaultEvent : public IMapEventOwner, public IPausable
{
	// use a circular based system for fallblock sounds
	ISound3D *fallblockSounds[3];
	u8 fallblockSoundSlot;
	
	// belowStack - all squares above this location are moved
	// moveDir - direction of movement
	void MoveStack(const core::vector3di &belowStack, const core::vector3di &moveDir);
	
protected:
	void OnPause();
	void OnResume();
	
public:
	DefaultEvent(Level *level);
	~DefaultEvent();
	
	void OnMapEvent(core::vector3di coord);
	
	void OnMapMoveEvent(core::vector3di coord);
	
	void OnMapLeaveEvent(core::vector3di coord, core::vector3di newCoord);
	
	bool RequestActionPermission(core::vector3di coord, E_ACTION_TYPE action);
	
	E_EVENT_TYPE GetType() { return EET_DEFAULT_EVENT; }
	
	core::stringc GetIconMeshName() { FAIL << "This should not be called."; }
};

class FanEvent : public IMapEventOwner
{
public:
	FanEvent(Level *level) : IMapEventOwner(level) {}
	
	void OnMapEvent(core::vector3di coord);
	
	void OnMapMoveEvent(core::vector3di coord) {}
	
	void OnMapLeaveEvent(core::vector3di coord, core::vector3di newCoord);
	
	bool RequestActionPermission(core::vector3di coord, E_ACTION_TYPE action);
	
	E_EVENT_TYPE GetType() { return EET_FAN_EVENT; }
	
	core::stringc GetIconMeshName() { return core::stringc("event_fan_draft.b3d"); }
};

class LiftEvent : public IMapEventOwner
{
	enum E_MOVE_DIRECTION
	{
		EMD_NONE,
		EMD_UP,
		EMD_DOWN
	};
	
	enum E_LIFT_LOCATION
	{
		ELL_BOTTOM,
		ELL_TOP,
		ELL_BOTH,	// both bottom and top, a rather pointless lift.
		ELL_MIDDLE	// somewhere in the middle
	};
	
	// assuming a lift is at the specified location, determine if that location
	// is an end location. (checks for adjacent LiftEvents)
	bool IsLiftEndLocation(core::vector3di coord);
	
	// assuming a lift is at the liftCoord, move that lift.
	// moves all objects resting on lift too.
	// may fail if moves fail (e.g. an immovable object blocks lift path)
	void MoveInDirection(core::vector3di liftCoord, E_MOVE_DIRECTION moveDir);
	
public:
	
	LiftEvent(Level *level) : IMapEventOwner(level) {}
	
	void OnMapEvent(core::vector3di coord);
	
	void OnMapMoveEvent(core::vector3di coord) {}
	void OnMapLeaveEvent(core::vector3di coord, core::vector3di newCoord) {}
	
	bool RequestActionPermission(core::vector3di coord, E_ACTION_TYPE action);
	
	E_EVENT_TYPE GetType() { return EET_LIFT_EVENT; }
	
	core::stringc GetIconMeshName() { return core::stringc("event_lift.b3d"); }
};

// Dummy event, does nothing. Used to indicate a position that the player can start the level from.
class PlayerStartEvent : public IMapEventOwner
{
public:
	PlayerStartEvent(Level *level) : IMapEventOwner(level) {}
	
	void OnMapEvent(core::vector3di coord){}
	void OnMapMoveEvent(core::vector3di coord) {}
	void OnMapLeaveEvent(core::vector3di coord, core::vector3di newCoord) {}
	
	bool RequestActionPermission(core::vector3di coord, E_ACTION_TYPE action) { return true; }
	
	E_EVENT_TYPE GetType() { return EET_PLAYER_START_EVENT; }
	
	core::stringc GetIconMeshName() { return core::stringc("event_player_start.b3d"); }
};








