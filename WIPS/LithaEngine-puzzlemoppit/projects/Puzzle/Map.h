
#include "Litha.h"
#include "Enums.h"
#include <map>
#include <vector>

class IMapEventOwner;

struct MapObject
{
	MapObject()
	{
		object = NULL;
		movable = false;
		type = EOT_UNKNOWN;
		previousCoordSet = false;
		is_sliding = false;
	}
	
	// Changed from IObject to IEntity so player (ICharacter) can also be set here.
	ITransformable *object;
	
	// Whether this object can be pushed by other objects.
	// (the object may still move of its own accord however this flag is set)
	bool movable;
	
	E_OBJECT_TYPE type;
	
	// remember the previous location this object was at, before the Map.StartMoveObject
	// was called.
	// Teleportation by SetObject does not set this.
	core::vector3di previousCoord;
	bool previousCoordSet;
	
	// hacked in to fix undo bug
	// (otherwise objects may slide after undo, if the stopping object has since moved)
	bool is_sliding;
};

class Map
{
	struct MapLocation
	{
		// MapLocation may ONLY contain an object and an event.
		// Nothing else (otherwise would require changing logic elsewhere)
		
		MapLocation()
		{
			event = NULL;
			traversing = false;
			containsObject = false;
			eventType = EET_UNKNOWN;
		}
		
		MapObject object; // may or may not be used (see containsObject flag below)
		
		E_EVENT_TYPE eventType;
		IMapEventOwner *event;
		
		// Is an Object currently traversing into this location?
		// Should not modify a map location when it is being traversed into
		bool traversing;
		
		// this map location actually contains an object, and
		// this->object contents is actually valid.
		bool containsObject;
	};

	std::map<int, std::map<int, std::map<int, MapLocation> > > map;
	
	MapLocation &GetLocation(core::vector3di coord);
	
	bool LocationExists(core::vector3di coord);
	
	void EraseLocation(core::vector3di coord);
	
public:
	Map();
	~Map();
	
	// Returns all coordinates that contain an event.
	// Mostly used by Editor.
	std::vector<core::vector3di> GetAllEvents();
	
	std::vector<core::vector3di> GetAllObjects();
	
	// Get all existing map locations.
	std::vector<core::vector3di> GetAllMapLocations();
	
	void Clear()
	{
		map.clear();
	}
	
	// moves the object in source to the position dest, provided:
	// - there is an object at source
	// - it is movable and is not currently traversing (into source)
	// - and there is no object at dest
	// forceMove forces the object to move even if it is immovable.
	// should only be used for move logic of a specific type of object.
	// IMPORTANT NOTE: This method should only really be used by Level::StartMapObjectMove,
	// so that moves applied to the player's square can be intercepted and given special case treatment.
	bool StartObjectMove(core::vector3di source, core::vector3di dest, bool forceMove = false);
	
	// hacked in for undo system.
	// same as StartObjectMove but doesn't actually do the move.
	bool ObjectMoveWillComplete(core::vector3di source, core::vector3di dest, bool forceMove = false);
	
	// Call this to finalise the move. An animation can be performed between start and finish.
	// (during which time the object is flagged as "traversing" and cannot be affected in an way)
	void FinishObjectMove(core::vector3di dest);
	
	// Put an object at a set coordinate.
	// Must not set NULL. (Currently it is not possible to remove an object).
	void SetObject(core::vector3di coord, ITransformable *object, bool movable, E_OBJECT_TYPE type);
	
	// Set event. Can be NULL to clear.
	void SetEvent(core::vector3di coord, IMapEventOwner *event);
	
	ITransformable *GetObject(core::vector3di coord);
	IMapEventOwner *GetEvent(core::vector3di coord);
	
	// Get object type.
	// Will fail (assert) if there is no object at the coordinate.
	// So ensure there is first with GetObject
	E_OBJECT_TYPE GetObjectType(core::vector3di coord);
	void SetObjectType(core::vector3di coord, E_OBJECT_TYPE objectType);
	
	// see GetObjectType
	bool IsObjectMovable(core::vector3di coord);
	void SetObjectIsMovable(core::vector3di coord, bool movable);
	
	// must be an object there...
	bool GetIsObjectSliding(core::vector3di coord);
	void SetIsObjectSliding(core::vector3di coord, bool sliding);
	
	// see GetObjectType
	// gets the previously occupied coordinate of the object at the specified coordinate
	// will fail if the object has never moved
	// an immovable object will (probably) never have a previous coord set.
	core::vector3di GetObjectPreviousCoord(core::vector3di coord);
	bool ObjectHasPreviousCoord(core::vector3di coord);
	
	// hacked in for Undo function
	void SetObjectPreviousCoord(core::vector3di coord, core::vector3di prevCoord);
	
	// is an object traversing into this location?
	// (object does not have to exist for this)
	bool IsTraversing(core::vector3di coord);
};
