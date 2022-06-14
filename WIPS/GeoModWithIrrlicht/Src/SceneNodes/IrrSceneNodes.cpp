/*
===============================================================================
	File:	IrrSceneNodes.cpp
	Desc:
===============================================================================
*/

#include "stdafx.h"
#pragma hdrstop
#include <Include/IrrMix.h>

namespace irr {
namespace scene {

/*=========================================
	class mxPlayerController
=========================================*/

// HACK: tmp:
::mix::mxOnScreenText *  DbgSpeed = NULL;
::mix::mxOnScreenText *  DbgState = NULL;


const btScalar  CHARACTER_MASS		= 30.f;
const btScalar  CHARACTER_HEIGHT	= 2.00f;
const btScalar  CHARACTER_WIDTH		= 1.75f;




mxPlayerController::mxPlayerController( ::mix::mxGamePlayer & player )
	: m_player( player )
	, m_eMovementType( ::mix::PM_NORMAL ) //PM_NOCLIP;
{
	m_pCursorControl = ::mix::GSystem->CursorControl;
	m_pCursorControl->grab();

	m_iLastAnimationTime = 0;

	//--------------------------------
	m_fMaxVerticalAngle = 85.0f;

	m_fRotateSpeed = 100.0f;
	m_fMoveSpeed = 3.0f;
	m_fJumpForce = 60.0f;

	//---------------------------------
	m_cursorKeys.Zero();
	m_keyMaps.Zero();

	using namespace ::mix;

	m_keyMaps[ MOVE_FORWARD ].m_KeyCode		= KEY_KEY_W;
	m_keyMaps[ MOVE_FORWARD ].m_Action		= MOVE_FORWARD;

	m_keyMaps[ MOVE_BACKWARD ].m_KeyCode	= KEY_KEY_S;
	m_keyMaps[ MOVE_BACKWARD ].m_Action		= MOVE_BACKWARD;

	m_keyMaps[ STRAFE_LEFT ].m_KeyCode		= KEY_KEY_A;
	m_keyMaps[ STRAFE_LEFT ].m_Action		= STRAFE_LEFT;

	m_keyMaps[ STRAFE_RIGHT ].m_KeyCode		= KEY_KEY_D;
	m_keyMaps[ STRAFE_RIGHT ].m_Action		= STRAFE_RIGHT;

	m_keyMaps[ DUCK ].m_KeyCode				= KEY_KEY_C;
	m_keyMaps[ DUCK ].m_Action				= DUCK;

	m_keyMaps[ JUMP_UP ].m_KeyCode			= KEY_SPACE;
	m_keyMaps[ JUMP_UP ].m_Action			= JUMP_UP;

	//------------------------------------------------------

	m_uLastTimeJumped = 0;



	btScalar  characterMass = CHARACTER_MASS;
	btScalar  characterHeight = CHARACTER_HEIGHT;
	btScalar  characterWidth  = CHARACTER_WIDTH;
	m_shape = new btCapsuleShape( characterWidth,characterHeight );

	btTransform   transform;
	transform.setIdentity();
	transform.setOrigin( Convert( player.GetOrigin() ) );

	//m_constantRotation = transform.getRotation();

	m_motionState = new btDefaultMotionState( transform );
	btRigidBody::btRigidBodyConstructionInfo   cInfo( characterMass, m_motionState, m_shape );

	m_rigidBody = new btRigidBody( cInfo );
	m_rigidBody->setSleepingThresholds( 0.0, 0.0 );
	m_rigidBody->setAngularFactor( 0.0 );

	m_rigidBody->setRestitution( 0.f );
	m_rigidBody->setFriction( 0.3f );

	m_rigidBody->setDamping( 0.0f, 1e10 );

//	m_rigidBody->setGravity( btVector3( 0, -5000, 0 ) );

	m_rigidBody->setUserPointer( & player );

	GPhysicsWorld->m_pDynamicsWorld->addRigidBody( m_rigidBody );




	//------------------------------------------------

	// Init. sound.
	//m_soundData.walkSounds[0] = ::mix::GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Footsteps/foot_stone01.ogg" );
	//m_soundData.walkSounds[1] = ::mix::GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Footsteps/foot_stone02.ogg" );
	//m_soundData.walkSounds[2] = ::mix::GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Footsteps/foot_stone03.ogg" );
	//m_soundData.walkSounds[3] = ::mix::GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Footsteps/foot_stone04.ogg" );
	//m_soundData.jumpSound = ::mix::GSoundWorld->CacheSound( PATH_TO_MEDIA "Sounds/Footsteps/jump_stone01.ogg" );

	//for ( u32 i=0; i < SoundData_s::MAX_NUM_WALK_SOUNDS; i++ ) {
//		m_soundData.walkSounds[i]->setDefaultVolume( 0.5f );
//	}
//	m_soundData.jumpSound->setDefaultVolume( 0.7f );
//	m_uLastTimePlayedSound = 0;

	//------------------------------------------------

//	DbgSpeed = ::mix::GRenderWorld->CreateText( 0.1f, 0.8f, ::mix::E_Colour::Color_Yellow );
//	DbgState = ::mix::GRenderWorld->CreateText( 0.1f, 0.9f, ::mix::E_Colour::Color_Yellow );
}
//const = 0
	//	virtual const core::array<SKeyMap>& mxPlayerController::getKeyMap(){};

//virtual void mxPlayerController::setInvertMouse(bool invert) {};

mxPlayerController::~mxPlayerController()
{
	m_pCursorControl->drop();

	//--------------------------
	delete m_shape;
	delete m_motionState;
	delete m_rigidBody;
}

bool mxPlayerController::OnEvent( const SEvent& rEvent )
{
	using namespace ::mix;

	switch ( rEvent.EventType )
	{
	case EET_KEY_INPUT_EVENT :
		{
			for ( u32 i = 0; i < NUM_ACTIONS; i++ )
			{
				if ( m_keyMaps[i].m_KeyCode == rEvent.KeyInput.Key )
				{
					m_cursorKeys[ m_keyMaps[i].m_Action ] = rEvent.KeyInput.PressedDown;
					return true;
				}
			}
			// Any custom events.
			//switch ( rEvent.KeyInput.Key )
			//{
			//}
			break;
		}//End case of keyboard events

	//
	//	Mouse input event
	//
	case EET_MOUSE_INPUT_EVENT :
		{
			if ( rEvent.MouseInput.Event == EMIE_MOUSE_MOVED )
			{
				m_cursorPosition = m_pCursorControl->getRelativePosition();
				return true;
			}

			if ( EMIE_LMOUSE_PRESSED_DOWN == rEvent.MouseInput.Event )
			{
				m_player.GetCurrentWeapon().PrimaryFire();
				return true;
			}
			if ( EMIE_LMOUSE_LEFT_UP == rEvent.MouseInput.Event )
			{
				m_player.GetCurrentWeapon().EndAttack();
				return true;
			}

			if ( EMIE_RMOUSE_PRESSED_DOWN == rEvent.MouseInput.Event )
			{
				m_player.GetCurrentWeapon().SecondaryFire();
				return true;
			}
			if ( EMIE_RMOUSE_LEFT_UP == rEvent.MouseInput.Event )
			{
				m_player.GetCurrentWeapon().EndAttack();
				return true;
			}

			//
			// Check middle mouse button.
			//
			if ( EMIE_MMOUSE_LEFT_UP == rEvent.MouseInput.Event)
			{
				return true;
			}

			//
			// Check mouse wheel.
			//
			if ( EMIE_MOUSE_WHEEL == rEvent.MouseInput.Event)
			{
				if ( rEvent.MouseInput.Wheel < 0 ) {
					m_player.SelectNextWeapon();	// wheel down
					return true;
				} else {
					m_player.SelectPrevWeapon();	// wheel up
					return true;
				}
				return true;
			}

			break;
		}//End case of mouse events

	default:
		break;
	}

	return false;
}

void mxPlayerController::animateNode( ISceneNode* node, u32 timeMs )
{
	using namespace ::mix;

	ICameraSceneNode * camera = static_cast< ICameraSceneNode* >( node );

	const f32  timeDiff = ( f32 ) (timeMs - m_iLastAnimationTime);
	m_iLastAnimationTime = timeMs;

	// Update position.
	Vec3D  pos = camera->getPosition();

	// Save previous position.
	m_currPos = pos;

	// Update rotation.
	Vec3D  target = ( camera->getTarget() - camera->getAbsolutePosition() );
	Vec3D  relativeRotation = target.getHorizontalAngle();


	bool	bRotationChanged = false;

	if ( m_cursorPosition != m_cursorCenter )
	{
		relativeRotation.Y -= (0.5f - m_cursorPosition.X) * m_fRotateSpeed;
		relativeRotation.X -= (0.5f - m_cursorPosition.Y) * m_fRotateSpeed;

		// X < m_fMaxVerticalAngle or X > 360-m_fMaxVerticalAngle

		if ( relativeRotation.X > (m_fMaxVerticalAngle * 2) &&
			relativeRotation.X < 360.0f - m_fMaxVerticalAngle )
		{
			relativeRotation.X = 360.0f - m_fMaxVerticalAngle;
		}
		else
		{
			if ( relativeRotation.X > m_fMaxVerticalAngle &&
				relativeRotation.X < 360.0f - m_fMaxVerticalAngle )
			{
				relativeRotation.X = m_fMaxVerticalAngle;
			}

			// Reset cursor position.
			m_pCursorControl->setPosition( 0.5f, 0.5f );
			m_cursorCenter = m_pCursorControl->getRelativePosition();

			// Needed to avoid problems when the event receiver is disabled.
			m_cursorPosition = m_cursorCenter;
		}
		bRotationChanged = true;
	}

	target.set( 0,0, core::max_( 1.f, pos.getLength() ) );
	Vec3D  movedir = target;

	TMatrix4  mat;
	mat.setRotationDegrees( Vec3D( relativeRotation.X, relativeRotation.Y, 0 ) );
	mat.transformVect( target );

	movedir = target;
	movedir.normalize();

	// Strafing.
	Vec3D  strafevect( target );
	strafevect = strafevect.crossProduct( camera->getUpVector() );

	strafevect.normalize();

	// Write the right target.
	if ( bRotationChanged )	{
		camera->setTarget( target );
	}

	//-------------------------------------------------------------------


	// HACK:
	m_rigidBody->applyCentralForce( btVector3( 0,-1400,0 ) );	// <- apply 'gravity'



	// Check for ground support.
	m_bHasGroundContacts = CheckForGroundSupport();

	if ( m_bHasGroundContacts ) {
		DbgState->SetText( "State: On Ground" );
	} else {
		DbgState->SetText( "State: In Air" );
	}


	//-------------------------------------------------------------------
	btVector3   newVelocity = btVector3( 0.0, 0.0, 0.0 );

	btVector3	vStrafe( strafevect.X, strafevect.Y, strafevect.Z );
	btVector3	vMoveDir( movedir.X, movedir.Y, movedir.Z );


	vStrafe /= 5;
	vMoveDir /= 5;


	bool bMoved = false;

	// Forward/backward moving.
	if ( m_cursorKeys[ MOVE_FORWARD ] ) {
		newVelocity += vMoveDir * timeDiff * m_fMoveSpeed;
		bMoved = true;
	}
	if ( m_cursorKeys[ MOVE_BACKWARD ] ) {
		newVelocity -= vMoveDir * timeDiff * m_fMoveSpeed;
		bMoved = true;
	}

	// Strafing.
	if ( m_cursorKeys[ STRAFE_LEFT ] ) {
		newVelocity += vStrafe * timeDiff * m_fMoveSpeed;
		bMoved = true;
	}
	if ( m_cursorKeys[ STRAFE_RIGHT ] ) {
		newVelocity -= vStrafe * timeDiff * m_fMoveSpeed;
		bMoved = true;
	}

	// No vertical movement with movement keys (WASD).
	newVelocity.setY( 0 );


	btVector3  currLinearVelocity = m_rigidBody->getLinearVelocity();
	DbgSpeed->SetText( "Curr. player speed: %f", currLinearVelocity.length() );




	// HACK: To avoid getting stuck while having contacts with other dynamic objects.
	const btScalar  THRESHOLD = 0.001f;
	if ( ! m_bHasGroundContacts && m_rigidBody->getLinearVelocity().length2() < THRESHOLD )
	{
		m_bHasGroundContacts = true;
	}


	// Move only if the player is on ground.
	if ( m_bHasGroundContacts )
	{
		// Dampen when on ground and not being moved by the player.
		if ( ! bMoved ) {
			m_rigidBody->setLinearVelocity( 0.2f * currLinearVelocity );
		}

		// Jumping.
		if ( m_cursorKeys[ JUMP_UP ] )
		{
			if ( CanJump() ) {
				DoJump();
				//bMoved = true;
			}
		}
		// Ducking.
	//	else if ( m_cursorKeys[ DUCK ] ) {
	//		// ...
	//	}
		else
		{
			// Normal walking.
			m_rigidBody->setLinearVelocity( newVelocity );

			// Play sound of footsteps.
			const f32  SOUND_THRESHOLD = 8.0f;
			if (/* newVelocity.length2()*/currLinearVelocity.length2() > (SOUND_THRESHOLD * SOUND_THRESHOLD) )
			{
				idRandom  random1( m_iLastAnimationTime );
				if ( int(m_iLastAnimationTime - m_uLastTimePlayedSound) > (320 + random1.RandomInt(80))
					&& newVelocity.length2() > 10 )
				{
					idRandom  random2( (int)m_uLastTimePlayedSound );
				//	::mix::GSoundWorld->Play2D( m_soundData.walkSounds[ random2.RandomInt( SoundData_s::MAX_NUM_WALK_SOUNDS ) ] );
					m_uLastTimePlayedSound = m_iLastAnimationTime;
				}
			}
		}

		//btVector3  velocity( m_rigidBody->getLinearVelocity() );
		//velocity *= 1.4f;
		//m_rigidBody->setLinearVelocity( velocity );
	}
	else
	{
		// If in mid-air.
		btVector3  velocity = currLinearVelocity + newVelocity/10;
		m_rigidBody->setLinearVelocity( velocity );
	}


	//---------------------------------------------------
	const btScalar  MAX_LINEAR_VELOCITY_RUN = 13; // m/s

#if 1
	{
		btScalar  speedSquared = m_rigidBody->getLinearVelocity().length2();
		if ( speedSquared >= (MAX_LINEAR_VELOCITY_RUN * MAX_LINEAR_VELOCITY_RUN) )
		{
			btVector3   dir( m_rigidBody->getLinearVelocity() );
			dir.normalize();
			m_rigidBody->setLinearVelocity( dir * MAX_LINEAR_VELOCITY_RUN );
		}
	}
#endif

}

void mxPlayerController::setRotateSpeed( f32 speed )
{
	m_fRotateSpeed = speed;
}

void mxPlayerController::setMoveSpeed( f32 speed )
{
	m_fMoveSpeed = speed;
}

f32 mxPlayerController::getRotateSpeed() const
{
	return m_fRotateSpeed;
}

f32 mxPlayerController::getMoveSpeed() const
{
	return m_fMoveSpeed;
}

void mxPlayerController::setKeyMap( SKeyMap *map, u32 count )
{
/*	using namespace irr;

	// clear the keymap
	KeyMap.clear();

	// add actions
	for (u32 i=0; i<count; ++i)
	{
		switch(map[i].Action)
		{
		case EKA_MOVE_FORWARD: KeyMap.push_back(SCamKeyMap(EKA_MOVE_FORWARD, map[i].KeyCode));
			break;
		case EKA_MOVE_BACKWARD: KeyMap.push_back(SCamKeyMap(EKA_MOVE_BACKWARD, map[i].KeyCode));
			break;
		case EKA_STRAFE_LEFT: KeyMap.push_back(SCamKeyMap(EKA_STRAFE_LEFT, map[i].KeyCode));
			break;
		case EKA_STRAFE_RIGHT: KeyMap.push_back(SCamKeyMap(EKA_STRAFE_RIGHT, map[i].KeyCode));
			break;
		case EKA_JUMP_UP: KeyMap.push_back(SCamKeyMap(EKA_JUMP_UP, map[i].KeyCode));
			break;
		default:
			break;
		}
	}
*/
}

void mxPlayerController::setVerticalMovement( bool bAllow )
{
	(void) bAllow;
}

ISceneNodeAnimator *
	mxPlayerController::createClone( ISceneNode* node, ISceneManager* newManager )
{
//	mxPlayerController * newAnimator = new mxPlayerController( NULL );
//	newAnimator->setKeyMap( NULL, NULL );
//	return newAnimator;
	Unreachable();
	return NULL;
}

//
//	Update the character position.
//
void mxPlayerController::Update()
{
	m_player.GetCamera()->setPosition( Convert( m_rigidBody->getWorldTransform().getOrigin() ) );
}

void mxPlayerController::Warp( const Vec3D& newOrigin )
{
	btTransform  & rTransform = m_rigidBody->getWorldTransform();
	rTransform.setOrigin( btVector3( newOrigin.X, newOrigin.Y, newOrigin.Z ) );
}

bool mxPlayerController::CanJump() const
{
	return HasGroundContacts()
		&& (GWorld->GetTime() - m_uLastTimeJumped > 1250);
}

bool mxPlayerController::HasGroundContacts() const
{
	return m_bHasGroundContacts;
}

bool mxPlayerController::CheckForGroundSupport()
{
	// Shoot a ray downward and see if it intersects something.
	// A single ray cast. Just cast a ray from the character's feet to the ground and any object that is contacted is stepped onto.

	class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
	{
	public:
		ClosestNotMe (btRigidBody* me) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
		{
			m_me = me;
		}

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject == m_me)
				return 1.0;

			return ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace
		);
	}
	protected:
		btRigidBody* m_me;
	};

	ClosestNotMe     rayCallback( m_rigidBody );


	btTransform  xform;
	m_rigidBody->getMotionState()->getWorldTransform (xform);

	btVector3   rayFrom;
	rayFrom = xform.getOrigin();



	btVector3   rayTo( rayFrom );
	btVector3   vDown( Convert(m_player.GetCamera()->getUpVector() * (-1)) );
	vDown.normalize();
	rayTo += vDown * (CHARACTER_HEIGHT + 1);



	::mix::GPhysicsWorld->m_pDynamicsWorld->rayTest( rayFrom, rayTo, rayCallback );
	return rayCallback.hasHit();// && (rayCallback.m_closestHitFraction < 1.0f);
}

// Do actual jumping.
//
void mxPlayerController::DoJump()
{
	//::mix::GSoundWorld->Play2D( m_soundData.jumpSound );
	m_uLastTimeJumped = ::mix::GWorld->GetTime();

	//--------------------------------------
	btTransform  xform;
	m_rigidBody->getMotionState()->getWorldTransform (xform);
	btVector3 up;

	//up = xform.getBasis()[1];
	Assign( up, m_player.GetCamera()->getUpVector() );

	up.normalize ();

#if 0
	const btScalar magnitude = (btScalar(1.0)/m_rigidBody->getInvMass()) * btScalar(10.0);
	m_rigidBody->applyCentralImpulse( up * magnitude );
#else
	btVector3  velocity( m_rigidBody->getLinearVelocity() );
	velocity += up * m_fJumpForce;
	m_rigidBody->setLinearVelocity( velocity );
#endif
}

/*================================
	class CBox_SceneNode
================================*/

CBox_SceneNode::CBox_SceneNode( f32 dimX, f32 dimY, f32 dimZ )
	: ISceneNode( ::mix::GRenderWorld->RootSceneNode, ::mix::GRenderWorld->SceneManager )
	, m_DimX( dimX )
	, m_DimY( dimY )
	, m_DimZ( dimZ )
{
	// indices
	const u16	u[36] = {   0,2,1,   0,3,2,   1,5,4,   1,2,5,   4,6,7,   4,5,6,
        7,3,0,   7,6,3,   9,5,2,   9,8,5,   0,11,10,   0,10,7};

	m_meshBuffer.Indices.set_used( 36 );
	for ( u32 i = 0; i < 36; ++i ) {
		m_meshBuffer.Indices[i] = u[i];
	}

	video::SColor	clr( 255, 255, 255, 255 );

	m_meshBuffer.Vertices.set_used( 12 );

	// UNDONE: recalculation of normals
	//
	m_meshBuffer.Vertices[0]  = video::S3DVertex( 0,				0,				0,			-1,-1,-1, clr, 0, 1);
	m_meshBuffer.Vertices[1]  = video::S3DVertex( m_DimX,			0,				0,			 1,-1,-1, clr, 1, 1);
	m_meshBuffer.Vertices[2]  = video::S3DVertex( m_DimX,			m_DimY,			0,			 1, 1,-1, clr, 1, 0);
	m_meshBuffer.Vertices[3]  = video::S3DVertex( 0,				m_DimY,			0,			-1, 1,-1, clr, 0, 0);
	m_meshBuffer.Vertices[4]  = video::S3DVertex( m_DimX,			0,				m_DimZ,		1,-1, 1, clr, 0, 1);
	m_meshBuffer.Vertices[5]  = video::S3DVertex( m_DimX,			m_DimY,			m_DimZ,		1, 1, 1, clr, 0, 0);
	m_meshBuffer.Vertices[6]  = video::S3DVertex( 0,				m_DimY,			m_DimZ,		-1, 1, 1, clr, 1, 0);
	m_meshBuffer.Vertices[7]  = video::S3DVertex( 0,				0,				m_DimZ,		-1,-1, 1, clr, 1, 1);
	m_meshBuffer.Vertices[8]  = video::S3DVertex( 0,				m_DimY,			m_DimZ,		-1, 1, 1, clr, 0, 1);
	m_meshBuffer.Vertices[9]  = video::S3DVertex( 0,				m_DimY,			0,			-1, 1,-1, clr, 1, 1);
	m_meshBuffer.Vertices[10] = video::S3DVertex( m_DimX,			0,				m_DimZ,		1,-1, 1, clr, 1, 0);
	m_meshBuffer.Vertices[11] = video::S3DVertex( m_DimX,			0,				0,			 1,-1,-1, clr, 0, 0);

	m_meshBuffer.BoundingBox.reset( 0, 0, 0 );

	for ( u32 i = 0; i < 12; ++i )
	{
		m_meshBuffer.Vertices[i].Pos -= core::vector3df( m_DimX/2, m_DimY/2, m_DimZ/2 );
		//m_meshBuffer.Vertices[i].Pos *= m_scale;
		m_meshBuffer.BoundingBox.addInternalPoint( m_meshBuffer.Vertices[i].Pos );
	}
}

CBox_SceneNode::~CBox_SceneNode() {
}

void CBox_SceneNode::OnRegisterSceneNode() {
	if ( IsVisible ) {
		SceneManager->registerNodeForRendering( this );
		ISceneNode::OnRegisterSceneNode();
	}
}

void CBox_SceneNode::render() {
	::mix::GRenderWorld->Driver->setMaterial( m_meshBuffer.Material );
	::mix::GRenderWorld->Driver->setTransform( video::ETS_WORLD, AbsoluteTransformation );
	::mix::GRenderWorld->Driver->drawMeshBuffer( & m_meshBuffer );
}

}//end of namespace scene
}//end of namespace irr

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
