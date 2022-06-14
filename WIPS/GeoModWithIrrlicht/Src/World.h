/*
===============================================================================
	File:	World.h
	Desc:	Core classes for presenting game worlds to the player.
===============================================================================
*/

#ifndef __MX_WORLD_H__
#define __MX_WORLD_H__

#define USE_D3DX9		// Needed for portals.



namespace mix {

//
// mxSystemWorld
//
class mxSystemWorld : public mxMemory {
public:
	::irr::ILogger *				Logger;
	::irr::io::IFileSystem *		FileSystem;
	::irr::gui::ICursorControl *	CursorControl;

public:
			// Returns the current real time ( in milliseconds ).
			// NOTE: This value does not start with 0 when the application starts.
	u32		GetRealTime() const;

			// Returns current virtual time in milliseconds.
			// This value starts with 0 when the application starts.
	u32		GetTime() const;

	void	SetMouseCursorVisible( bool bVisible );

	//--- Memory management. -------------------------------------------------------

	// overrides ( mxMemory )
	void *	Allocate( size_t  Num );
	void *	Reallocate( void* old, size_t  Num );
	void	Free( void* mem );

	u32		GetNumBytesAvailable();

private:
	friend class mxWorld;

	// called by mxWorld.
	//
	mxSystemWorld();
	~mxSystemWorld() {}

	void Spawn();
	void Shutdown();

private:
	::irr::ITimer *		m_timer;
};

#include "SystemWorld.inl"

extern mxSystemWorld *	GSystem;
//==============================================================================

//
//	E_Colour
//
enum E_Colour
{
	Color_White,
	Color_Grey,
	Color_Black,
	Color_Blue,
	Color_Green,
	Color_Yellow,
	Color_Red,
};

//
//	E_Font
//
enum E_Font
{
	Font_Small,
	Font_Medium,
	Font_Large,
};

//
//	mxConsole - game console.
//
class mxConsole {
public:
	void	Show();
	void	Hide();

	mxConsole &		Print( const char* text, ... );

	mxConsole &		operator << ( const char* string );
	mxConsole &		operator << ( const Vec3D& vector );

private:
	friend class mxWorld;

			mxConsole();
			~mxConsole();
};

//==============================================================================

// Forward declarations.
class mxRenderMesh;
class mxRenderEntity;
class mxPortal;

class IPostProcessEffect;

//
//	mxOnScreenText - static text on the screen.
//
class mxOnScreenText {
public:
	void	ChangeColor( E_Colour newColor );
	void	SetText( const char* format, ... );

private:
	TArray< wchar_t >		m_Wstring;
	::irr::gui::IGUIStaticText * m_pStaticText;

	enum { MAX_BUFFER_LENGTH = 256 };
	char	m_buffer[ MAX_BUFFER_LENGTH ];

	friend class mxRenderWorld;
	friend class TArray< mxOnScreenText >;

	mxOnScreenText()
		: m_pStaticText( NULL )
		, m_Wstring( 256 )
	{
		m_Wstring.SetNum( 256 );
	}

	~mxOnScreenText()
	{
		if ( m_pStaticText ) {
			m_pStaticText->remove();
		}
	}
};

//
//	mxView
//
class mxView {
public:
	enum E_FrustumType
	{
		CLIPPED_PYRAMID,
		CONE_FRUSTUM,
	};

	TViewFrustum		Frustum;
	//...
	// TODO: additional parameters (for frustum clipping with portal frames, visibility culling, etc)
	//TConeFrustum		ConeFrustum;
	//TConvexShape		ConvexFrustum;
};

//
// mxRenderWorld - all drawing is done to this world.
//
class mxRenderWorld {
public:
	::irr::video::IVideoDriver *	Driver;
	::irr::scene::ISceneManager *	SceneManager;
	::irr::scene::ISceneNode *		RootSceneNode;
	::irr::gui::IGUIEnvironment *	GUI;

	u32		DesiredFrameRate;	// Minimum desired frame rate ( frames per second ).
	u32		MaxFrameRate;		// Maximum frame rate.


#ifdef USE_D3DX9

//	LPDIRECT3D9             pD3D;		// Used to create the D3DDevice.
//	LPDIRECT3DDEVICE9       pD3DDevice; // Rendering device.

#endif // USE_D3DX9


public:
	::irr::video::E_DRIVER_TYPE		GetDriverType() const;

	bool		IsFullScreen() const;
	u32			GetScreenWidth() const;
	u32			GetScreenHeight() const;

				// Returns approximate current FPS.
	int32		GetFPS() const;

	void	TakeScreenShot( const char* filename );

	void	AddEntity( mxRenderEntity* entity );
	void	RemoveEntity( mxRenderEntity* entity );

	void	AddPortal( mxPortal* portal );
	void	RemovePortal( mxPortal* portal );

			// Called each frame to render the world and can be invoked by portals.
	void	DrawEntities( const mxView& view );

	//--- Testing & Debugging ---------------------------------------

	// Creates a static text on the screen at the specified position. NOTE: x and y must be in the range [0..1].
	//mxOnScreenText *	CreateText( f32 x, f32 y, E_Colour color = E_Colour::Color_White, E_Font font = E_Font::Font_Medium );
	//mxOnScreenText *	CreateText( u16 x, u16 y, u16 width, u16 height, E_Colour color = E_Colour::Color_White, E_Font font = E_Font::Font_Medium );
	//void				RemoveText( mxOnScreenText* pText );

	// Get the corresponding SColor.
	::irr::video::SColor	GetColor( E_Colour color );

	// For debugging.
//	void	CreatePermMark( const Vec3D& position, E_Colour color = E_Colour::Color_Black );	// Creates a permanent mark.
//	void	CreateTempMark( const Vec3D& position, u32 timeMs = 10000 );

public:
	// TODO: remove this:
	static const AABB	UniverseExtent;	// (very huge) bounding box of the entire game universe.

	//--- Materials for Testing & Debugging ------------------------
	//
	static const TRenderMaterial	NoLightMaterial;	// For debug drawing ( bounding boxes, etc. ).
	static const TRenderMaterial	WireframeMaterial;
	static const TRenderMaterial	AABBMaterial;		// For drawing AABBs.

	static TRenderMaterial		DefaultSolidMaterial;	// For drawing solid objects.
	static TRenderMaterial		DecalMaterial;			// For decals.

private:
	friend class mxWorld;

	// called by mxWorld
	mxRenderWorld();
	~mxRenderWorld() {}

	void Spawn();
	void Shutdown();

	// called by mxWorld each frame
	void RenderFrame();

	void UpdateView();
	void DrawPortals( const mxView& view );

private:
	::irr::video::E_DRIVER_TYPE  m_eDriverType;

	u32		m_uScreenWidth;
	u32		m_uScreenHeight;
	bool	m_bFullScreen;

	TArray< mxOnScreenText* >		m_staticTexts;

	TArray< IPostProcessEffect* >	m_postProcessEffects;

	// TODO: make a priority queue (visible entities) and insert entities into that queue and render them;
	// make queues for solid, transparent objects, etc.;
	// employ more algorithms (occlusion culling, PVS, etc)
	TCircularList< mxRenderEntity >		m_renderEntities;

    TCircularList< mxRenderEntity >		m_portals;

	mxView		m_view;
};

#include "RenderWorld.inl"

extern mxRenderWorld *	GRenderWorld;
//==============================================================================


//
//	Size of the world.
//

#define WORLD_EXTENT			524288.0	// Maximum size of the world.
#define HALF_WORLD_EXTENT		262144.0	// Half the maximum size of the world.

//==============================================================================

// Forward declarations.
class mxEntity;
class mxPhysicsEntity;
	class CBulletPhysicsEntity;
	class CPlayerPhysicsEntity;

class mxMaterialSystem;

// TODO: remove GPhysicsWorld, use Environment in each entity.

//
//	mxPhysicsWorld
//
class mxPhysicsWorld {
public:
	// TODO: use a hash table for collision shapes and save memory.

	// These automaticaly add new physics entities to the simulation.
	// NOTE: doesn't take ownership of the returned object.
	mxPhysicsEntity *	GetSpherePhysicsEntity( f32 inRadius, f32 inMass, const Vec3D& position );
	mxPhysicsEntity *	GetBoxPhysicsEntity( const Vec3D& dimensions, f32 inMass, const Vec3D& position );

	mxPhysicsEntity *	GetStaticMeshPhysicsEntity( const ::irr::scene::IMesh* pMesh, const Vec3D& position = Vec3D(0,0,0) );

	// For testing & debugging.
	mxPhysicsEntity *	GetStaticTriangleMeshPhysicsEntity( const mxRenderMesh& rMesh );

	mxPhysicsEntity *	GetStaticPlanePhysicsEntity( const TPlane3D& plane );

	mxPhysicsEntity *	CreatePlayerPhysics( const Vec3D& initialPosition, f32 mass = 90.0f );
	void	TracePhysicsEntity( const Vec3D& vFrom, const Vec3D& vTo, const mxPhysicsEntity* playerPhysics, Vec3D &OutNewPos );

	// Take responsibility for deleting this physics entity.
	//void	Acquire( mxPhysicsEntity* physEnt );

	void	DeletePhysicsEntity( mxPhysicsEntity* physEnt );

	// TODO: rename these to GetGlobalGravity() or remove these.
	const Vec3D &	GetGravity() const;
	void			SetGravity( const Vec3D& newGravity );

	mxMaterialSystem &	GetMaterialSystem();

			// Called by RenderWorld.
	void	DebugDrawWorld();

private:
	friend class mxWorld;

	mxPhysicsWorld();
	~mxPhysicsWorld();

	void Spawn();
	void Shutdown();

	void	RunFrame( const REAL fElapsedTime );

//HACK: temp., remove that: !!!
public://private:
	btDiscreteDynamicsWorld *	m_pDynamicsWorld;

	btCollisionConfiguration *	m_pCollisionConfiguration;
	btCollisionDispatcher *		m_pCollisionDispatcher;
	btBroadphaseInterface *		m_pBroadphaseInterface;
	btConstraintSolver *		m_pConstraintSolver;
	btIDebugDraw *				m_pDebugDrawer;

	REAL	m_fInternalTimeStep;	// The size of internal step.

	mxMaterialSystem *			m_pMaterialSystem;

private:
	// For garbage collection. TODO: use a map (hashtable) for sharing collision shapes among bodies.
	TArray< btCollisionShape* >			m_collisionShapes;	// <- these will be deleted upon destroying the world
	TArray< btDefaultMotionState* >		m_motionStates;		// <- these will be deleted upon destroying the world
	TArray< mxPhysicsEntity* >			m_physicsEntities;	// <- these will be deleted upon destroying the world
	TArray< btTriangleMesh* >		m_triangleMeshes;	// <- these will be deleted upon destroying the world
};

#include "PhysicsWorld.inl"

extern mxPhysicsWorld *	GPhysicsWorld;
//==============================================================================

// Forward declarations.
class mxGamePlayer;
class mxEnvironment;
class mxWeaponsManager;

//==============================================================================

//
//	mxAction
//
class mxAction {
public:
	virtual		~mxAction() {}

	virtual void	Execute() = 0;
};

//
//	mxActionChangeBool
//
class mxAction_Bool : public mxAction {
public:
	mxAction_Bool( bool bInitialValue = true )
		: m_boolean( bInitialValue )
	{}

	// overrides ( mxAction )
	virtual void Execute()
	{
		m_boolean ^= 1;
	}

	void SetBool( bool value )
	{ m_boolean = value; }

	bool GetBool() const
	{ return m_boolean; }

private:
	bool	m_boolean;
};

//==============================================================================

//
//	mxWorld
//
class mxWorld : public ::irr::IEventReceiver {
public:
						mxWorld();
						~mxWorld();

	void				Spawn();
	void				Run();
	void				Shutdown();

	mxEnvironment *		GetEnvironment();
	mxGamePlayer *		GetLocalPlayer();

	mxConsole &			GetConsole();

	u32					GetTime() const;	// Returns the current game time.

	bool				IsPaused() const;

	mxGamePlayer *		SpawnHumanPlayer( const Vec3D& initialPosition );	// Creates a new human player.

public:
	mxEnvironment *		Environment;


public:
	void	AddAction( mxAction& rAction, ::irr::EKEY_CODE  key );

protected:
	// override irr:IEventReceiver
	bool	OnEvent( const ::irr::SEvent & event );

private:
	void	RunFrame( const REAL fDeltaTime );

	//--- Testing & Debugging -----------------------
	void	LoadDemoLevel();
	void	RunDebugInfo();

	// called in constructor
	void PreInit();

private:
	::irr::IrrlichtDevice *	m_pIrrlichtDevice;

	mxSystemWorld		m_systemWorld;
	mxRenderWorld		m_renderWorld;

	mxPhysicsWorld		m_physicsWorld;
	mxConsole			m_gameConsole;

	bool			m_bInitialized;

private:
	u32				m_uFrameNumber;
	u32				m_uLastFrameTime;	// time since last update in milliseconds
	u32				m_uCurrentTime;		// in milliseconds
	bool			m_bPaused;

	mxGamePlayer *	m_localPlayer;	// will be deleted by mxEnvironment

#ifdef MX_DEBUG
	// Testing & Debugging.
	struct DebugData {
		mxOnScreenText *	statsText;
		mxOnScreenText *	playerPosText;
		mxOnScreenText *	numObjectsText;
	};
	DebugData	m_debugData;
#endif // MX_DEBUG
	//------------------------------------

//!	TFixedArray< TArray< mxAction* >, ::irr::EKEY_CODE::KEY_KEY_CODES_COUNT >		m_keyActions;
};

#include "World.inl"

extern mxWorld *	GWorld;

}//end of namespace mix

#endif /* ! __MX_WORLD_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
