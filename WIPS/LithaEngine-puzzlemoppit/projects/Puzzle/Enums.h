
#ifndef ENUMS_H
#define ENUMS_H


// NOTE: When modifying this list, should also modify all implementations
// of IMapEventOwner.RequestActionPermission
enum E_ACTION_TYPE
{
	EAT_FALL,
	EAT_LOCOMOTE,	// when an object moves *itself* (not falling or getting pushed)
	EAT_MOVE_BY_PUSH // can an object be pushed by the player while at this location?
};

enum E_OBJECT_TYPE // WARNING: Ordering of these should not be changed as IDs are used in level files.
{
	// (SO ADD NEW TYPES TO THE END)
	EOT_UNKNOWN = 0,
	EOT_GROUND_BLOCK,
	EOT_MOVABLE_BLOCK,
	EOT_FAN,
	EOT_LIFT,
	EOT_PLAYER_CENTRE,			// a character centre pos (may actually be either player or some kind of enemy creature)
	EOT_PLAYER_INTERSECTING,	// a location intersected by a character, but not character centre
	EOT_EXIT_PORTAL,
	EOT_SLIDING_BLOCK,
	EOT_BALLOON,
	EOT_GROUND_BLOCK_FALL,
	
	// Number of items in this enum.
	EOT_COUNT
};

enum E_EVENT_TYPE // WARNING: Ordering of these should not be changed as IDs are used in level files.
{
	EET_UNKNOWN = 0,
	EET_FAN_EVENT,
	EET_LIFT_EVENT,
	EET_PLAYER_START_EVENT,
	EET_DEFAULT_EVENT,//this one is always present. And cannot be set in editor.
	
	// Number of items in this enum.
	EET_COUNT
};

// useful constants for animation
enum
{
	ANIM_IDLE = 0,
	ANIM_WALK,
	ANIM_FALL,
	ANIM_PUSH,
	ANIM_FALL_UPRIGHT
};

#endif

