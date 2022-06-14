/*
===============================================================================
	File:	IrrSceneNodes.h
	Desc:	Irrlicht scene node classes.
===============================================================================
*/

#ifndef __MX_IRR_SCENE_NODES_H__
#define __MX_IRR_SCENE_NODES_H__

// Forward declarations.
namespace mix {
	class mxGamePlayer;

	class mxGeomoddableSolid;

	class CPlayerPhysicsEntity;
}//end of namespace mix


namespace irr {
namespace scene {

//===========================================================================================================

using namespace ::mix;

//
//	mxPlayerController
//
//	Handles user input events, animates FPS camera, provides collision detection and response.
//	Most of the code swiped from Irrlicht.
//
class mxPlayerController : public ISceneNodeAnimatorCameraFPS {
public:
					mxPlayerController( mxGamePlayer & player );
					~mxPlayerController();

	//----- overrides ( ISceneNodeAnimatorCameraFPS ) --------------------------------------------

	f32		getMoveSpeed() const;
	void	setMoveSpeed( f32 moveSpeed );

	f32		getRotateSpeed() const;
	void	setRotateSpeed( f32 rotateSpeed );

	void	setKeyMap( SKeyMap *map, u32 count );
	void	setVerticalMovement( bool allow );

	//----- overrides ( ISceneNodeAnimator ) ----------------------------------------------------

	void animateNode( ISceneNode* node, u32 timeMs );

	ISceneNodeAnimator * createClone( ISceneNode* node, ISceneManager * newManager = NULL );

	bool	isEventReceiverEnabled() const { return true; }

	bool	OnEvent( const SEvent& event );

	ESCENE_NODE_ANIMATOR_TYPE	getType() const
	{
		return ESNAT_CAMERA_FPS;
	}

	//--------------------------------------------------------------------------------------------------
	void	Update();	// Called by mxPlayer.

	void	Warp( const Vec3D& newOrigin );

	bool	CanJump() const;

			// Returns true if the character is on ground.
	bool	HasGroundContacts() const;

	bool	CheckForGroundSupport();

	void	DoJump();

	EPlayerMovementType		GetMovementType() const						{ return m_eMovementType; }
	void					SetMovementType( EPlayerMovementType type )	{ m_eMovementType = type; }

private:
	mxGamePlayer &		m_player;	// the player entity we're controlling

	EPlayerMovementType	m_eMovementType;

	//---------------------------------------------------------------------
	gui::ICursorControl *	m_pCursorControl;

	//
	//	KeyMap_s
	//	associates a key with an action.
	//
	struct KeyMap_s
	{
		KeyMap_s()
		{}

		KeyMap_s( u32 action, EKEY_CODE keyCode )
			: m_Action( action ), m_KeyCode( keyCode )
		{}

		u32			m_Action;
		EKEY_CODE	m_KeyCode;
	};

	TFixedArray< KeyMap_s, NUM_ACTIONS >	m_keyMaps;

	// Variables to hold whether cursor keys are pressed or not.
	TFixedArray< bool,NUM_ACTIONS >		m_cursorKeys;

	core::position2d< f32 >		m_cursorPosition;
	core::position2d< f32 >		m_cursorCenter;

	u32			m_iLastAnimationTime;

	f32			m_fMaxVerticalAngle;

	f32			m_fMoveSpeed;
	f32			m_fRotateSpeed;
	f32			m_fJumpForce;
	f32			m_fCrouchSpeed;

	//-------------------------------
	Vec3D		m_currPos;

	enum { X = 0, Y = 1, Z = 2 };
	btVector3	m_vSpeed;
	btScalar	m_maxSpeed;

	btCollisionShape *	m_shape;
	btMotionState *		m_motionState;	// <= for garbage collection
	btRigidBody *		m_rigidBody;

	//bool	bRotationChanged;		// True if mouse cursor moved and camera target should be changed.
	bool	m_bHasGroundContacts;	// True if the character is on ground.

	u32		m_uLastTimeJumped;

	struct SoundData_s {
		enum { MAX_NUM_WALK_SOUNDS = 4 };
	};
	SoundData_s		m_soundData;
	u32				m_uLastTimePlayedSound;
};

//
//	CBox_SceneNode
//
class CBox_SceneNode : public ISceneNode {
public:
			CBox_SceneNode( f32 dimX, f32 dimY, f32 dimZ );
			~CBox_SceneNode();

	// overrides ( ISceneNode )
	void			OnRegisterSceneNode();
	void			render();

	const core::aabbox3df &	getBoundingBox() const;

	TRenderMaterial &	getMaterial( u32 );
	u32					getMaterialCount() const;

	ESCENE_NODE_TYPE	getType() const;

private:
	float	m_DimX, m_DimY, m_DimZ;

	SMeshBuffer		m_meshBuffer;
};

inline const core::aabbox3df & CBox_SceneNode::getBoundingBox() const {
	return m_meshBuffer.BoundingBox;
}

inline TRenderMaterial & CBox_SceneNode::getMaterial( u32 ) {
	return m_meshBuffer.Material;
}

inline u32 CBox_SceneNode::getMaterialCount() const
{ return 1; }

inline ESCENE_NODE_TYPE CBox_SceneNode::getType() const {
	return ESNT_CUBE;
}

}//end of namespace scene
}//end of namespace irr

#endif /* ! __MX_IRR_SCENE_NODES_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
