
#include "Map.h"
#include "Events.h"


Map::MapLocation &Map::GetLocation(core::vector3di coord)
{
	return map[coord.X][coord.Y][coord.Z];
}

bool Map::LocationExists(core::vector3di coord)
{
	if ( map.count(coord.X)
			&& map[coord.X].count(coord.Y)
			&& map[coord.X][coord.Y].count(coord.Z)
			)
	{
		return true;
	}
	else
		return false;
}

void Map::EraseLocation(core::vector3di coord)
{
	//map[coord.X][coord.Y].count(coord.Z)
	map[coord.X][coord.Y].erase(coord.Z);
}


Map::Map()
{
}

Map::~Map()
{
	// Objects are removed by Level.
}

std::vector<core::vector3di> Map::GetAllEvents()
{
	std::vector<core::vector3di> eventCoords;
	
	for (std::map<int, std::map<int, std::map<int, MapLocation> > >::const_iterator i = map.begin(); i != map.end(); i ++)
	{
		const std::map<int, std::map<int, MapLocation> > &map2 = i->second;
		
		for (std::map<int, std::map<int, MapLocation> >::const_iterator j = map2.begin(); j != map2.end(); j ++)
		{
			const std::map<int, MapLocation> &map3 = j->second;
			
			for (std::map<int, MapLocation>::const_iterator k = map3.begin(); k != map3.end(); k ++)
			{
				//k->second is MapLocation
				
				if (k->second.event)
					eventCoords.push_back(core::vector3di(i->first, j->first, k->first));
			}
		}
	}
	
	return eventCoords;
}

std::vector<core::vector3di> Map::GetAllObjects()
{
	std::vector<core::vector3di> objectCoords;
	
	for (std::map<int, std::map<int, std::map<int, MapLocation> > >::const_iterator i = map.begin(); i != map.end(); i ++)
	{
		const std::map<int, std::map<int, MapLocation> > &map2 = i->second;
		
		for (std::map<int, std::map<int, MapLocation> >::const_iterator j = map2.begin(); j != map2.end(); j ++)
		{
			const std::map<int, MapLocation> &map3 = j->second;
			
			for (std::map<int, MapLocation>::const_iterator k = map3.begin(); k != map3.end(); k ++)
			{
				//k->second is MapLocation
				
				if (k->second.containsObject)
					objectCoords.push_back(core::vector3di(i->first, j->first, k->first));
			}
		}
	}
	
	return objectCoords;
}

std::vector<core::vector3di> Map::GetAllMapLocations()
{
	std::vector<core::vector3di> mapLocationCoords;
	
	for (std::map<int, std::map<int, std::map<int, MapLocation> > >::const_iterator i = map.begin(); i != map.end(); i ++)
	{
		const std::map<int, std::map<int, MapLocation> > &map2 = i->second;
		
		for (std::map<int, std::map<int, MapLocation> >::const_iterator j = map2.begin(); j != map2.end(); j ++)
		{
			const std::map<int, MapLocation> &map3 = j->second;
			
			for (std::map<int, MapLocation>::const_iterator k = map3.begin(); k != map3.end(); k ++)
			{
				//k->second is MapLocation
				
				mapLocationCoords.push_back(core::vector3di(i->first, j->first, k->first));
			}
		}
	}
	
	return mapLocationCoords;
}

bool Map::StartObjectMove(core::vector3di source, core::vector3di dest, bool forceMove)
{
	if (!LocationExists(source))
		return false;
	
	MapLocation &sourceLoc = GetLocation(source);
	
	if (sourceLoc.containsObject && (sourceLoc.object.movable || forceMove) && !sourceLoc.traversing)
	{
		MapLocation &destLoc = GetLocation(dest);
		
		if (!destLoc.containsObject)
		{
			// Move object from source to dest!
			// Event and other attributes should not be affected.
			destLoc.object = sourceLoc.object;
			
			sourceLoc.containsObject = false;
			destLoc.containsObject = true;
			
			destLoc.traversing = true;	// Must call FinishObjectMove to set this false.
										// (i.e. after any animation is finished)
			
			// Store the previous coordinate of this object
			// for use by lift among other things.
			destLoc.object.previousCoord = source;
			destLoc.object.previousCoordSet = true;
			
			// Remove old location if no longer used
			// (not used by event or anything either)
			if (!sourceLoc.containsObject && !sourceLoc.event)
			{
				// Remove sourceLoc.
				EraseLocation(source);
			}
			
			// update sliding state??
			// only slide in the horizontal
			// this is cleared in DefaultEvent::OnMapEvent
			if ( (dest-source).Y == 0 )
			{
				destLoc.object.is_sliding = true;
			}
			
			return true;
		}
	}
	return false;
}

// this is copy pasted from above
bool Map::ObjectMoveWillComplete(core::vector3di source, core::vector3di dest, bool forceMove)
{
	if (!LocationExists(source))
		return false;
	
	MapLocation &sourceLoc = GetLocation(source);
	
	if (sourceLoc.containsObject && (sourceLoc.object.movable || forceMove) && !sourceLoc.traversing)
	{
		MapLocation &destLoc = GetLocation(dest);
		
		if (!destLoc.containsObject)
		{
			return true;
		}
	}
	return false;
}

void Map::FinishObjectMove(core::vector3di dest)
{
	if (LocationExists(dest))
	{
		MapLocation &loc = GetLocation(dest);
		loc.traversing = false;
	}
}

void Map::SetObject(core::vector3di coord, ITransformable *object, bool movable, E_OBJECT_TYPE type)
{
	MapLocation &loc = GetLocation(coord);
	
	// if setting a new object, then the location must NOT already contain an object
	ASSERT( (!object) || (!loc.containsObject) );
	
	loc.containsObject = (bool)object; // coud be NULL to clear object
	loc.traversing = false; // object is assumed to have been immediately positioned
	
	MapObject mapObject;
	loc.object = mapObject;	// Set MapObject to defaults
							// Note: do not set MapLocation to defaults as doing so would also clear event.
	
	loc.object.object = object;
	loc.object.movable = movable;
	loc.object.type = type;
	
	// Erase the location if no object and no event
	if (!loc.containsObject && !loc.event)
		EraseLocation(coord);
}

void Map::SetEvent(core::vector3di coord, IMapEventOwner *event)
{
	MapLocation &loc = GetLocation(coord);
	loc.event = event;
	
	// Erase the location if no object and no event
	if (!loc.containsObject && !loc.event)
		EraseLocation(coord);
}

ITransformable *Map::GetObject(core::vector3di coord)
{
	if (LocationExists(coord))
	{
		MapLocation &loc = GetLocation(coord);
		
		if (loc.containsObject)
			return loc.object.object;
		else
			return NULL;
	}
	else
		return NULL;
}

IMapEventOwner *Map::GetEvent(core::vector3di coord)
{
	if (LocationExists(coord))
		return GetLocation(coord).event;
	else
		return NULL;
}

E_OBJECT_TYPE Map::GetObjectType(core::vector3di coord)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	return loc.object.type;
}

void Map::SetObjectType(core::vector3di coord, E_OBJECT_TYPE objectType)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	loc.object.type = objectType;
}

bool Map::IsObjectMovable(core::vector3di coord)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	return loc.object.movable;
}

void Map::SetObjectIsMovable(core::vector3di coord, bool movable)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	loc.object.movable = movable;
}

bool Map::GetIsObjectSliding(core::vector3di coord)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	return loc.object.is_sliding;
}

void Map::SetIsObjectSliding(core::vector3di coord, bool sliding)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	loc.object.is_sliding = sliding;
}

core::vector3di Map::GetObjectPreviousCoord(core::vector3di coord)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	ASSERT (loc.object.previousCoordSet);
	
	return loc.object.previousCoord;
}

bool Map::ObjectHasPreviousCoord(core::vector3di coord)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	return loc.object.previousCoordSet;
}

void Map::SetObjectPreviousCoord(core::vector3di coord, core::vector3di prevCoord)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	ASSERT (loc.containsObject);
	
	loc.object.previousCoordSet = true;
	loc.object.previousCoord = prevCoord;
}

bool Map::IsTraversing(core::vector3di coord)
{
	ASSERT (LocationExists(coord));
	
	MapLocation &loc = GetLocation(coord);
	
	return loc.traversing;
}


