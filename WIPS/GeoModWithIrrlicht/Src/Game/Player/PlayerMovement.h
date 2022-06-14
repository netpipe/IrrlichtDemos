/*
===============================================================================
	File:	PlayerMovement.h
	Desc:	
===============================================================================
*/

#ifndef __MX_GAME_PLAYER_MOVEMENT_H__
#define __MX_GAME_PLAYER_MOVEMENT_H__

namespace mix {

//
//	EPlayerMovementType
//
enum EPlayerMovementType
{
	PM_NORMAL,				// normal physics
	PM_DEAD,				// no acceleration or turning, but free falling
	PM_SPECTATOR,			// flying without gravity but with collision detection
	PM_FREEZE,				// stuck in place without control
	PM_NOCLIP				// flying without collision detection nor gravity
};

//
//	EPlayerMotionState
//
enum EPlayerMotionState
{
	Player_On_Ground,
	Player_In_Air,
};

//
//	CharacterSettings
//
struct CharacterSettings
{
	f32		mass;		// Specifies how much the character pushes down on objects it stands on.
	f32		strength;	// Specifies how much the character is able to push other objects around,
};

//
// EKeyAction - basic game action ( like player movement ) associated with a key.
//
enum EKeyAction
{
	// Movement

	MOVE_FORWARD,
	MOVE_BACKWARD,

	STRAFE_LEFT	,
	STRAFE_RIGHT,

	DUCK,
	JUMP_UP,

	//------------------------------------
	// Add your custom key actions here.

	//------------------------------------
	NUM_ACTIONS	= 256 // marker
};

}//end of namespace mix

#endif /* ! __MX_GAME_PLAYER_MOVEMENT_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//


