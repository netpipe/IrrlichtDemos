//
// irrPhysics v0.001
//
// created by FMX for the Irrlicht community
// 26 Jan 2009
//
// licence: whatever, irrlicht compatible. zlib, etc, etc
//
// use at your own risk! No warranties, guarantees or any sort of reconciliating services available
//


class CirrSPObject
{
public:
	ISceneNode	*node;

	float		Mass;

	vector3df	userForce;		// extra force to apply next frame, get's reset afterwards
	vector3df	userVel;		// extra velocity to apply next frame, get's reset afterwards

	vector3df	Velocity;		// cumulative velocity

	bool		bIsFrozen;		// if TRUE, object doesn't get updated	[ default = false]
	bool		bGravity;		// does Gravity affect it?				[ default = true ]


	void reset( bool clearObject )
	{
		bGravity  = true;
		bIsFrozen = false;

		userForce = vector3df(0,0,0);
		userVel	  = vector3df(0,0,0);

		Velocity  = vector3df(0,0,0);

		if( clearObject )
		{
			node = 0;
			Mass = 1;
		}
	}

	CirrSPObject()
	{
		reset( true );
	}

	~CirrSPObject()
	{
		reset( true );
	}
};




class CirrSimplePhysics
{

	vector3df Gravity;					// gravity with which to affect all objects every frame
	array<CirrSPObject> OBJECT;	// all objects irrPhysics knows about

	f32 dt;
	u32 thisTime;
	u32 lastTime;

	IrrlichtDevice* device;

	bool bSimActive;


public:

	CirrSimplePhysics( IrrlichtDevice* idevice = 0, const vector3df &defaultGravity = vector3df(0,0,0) )
	{
	// irrphysics vars

		device = idevice;
		Gravity = defaultGravity;

		dt = 1;
		thisTime = device->getTimer()->getTime();
		lastTime = thisTime;

		OBJECT.set_used( 0 );

		bSimActive = true;
	}

	
	~CirrSimplePhysics()
	{
	// Cleanup
		OBJECT.clear();
	}


	void UPDATE()
	{
		if( !bSimActive ) return;


	// get dt
		thisTime = device->getTimer()->getTime();
		dt = (f32)(thisTime - lastTime) / 1000.f; // Time in seconds
		lastTime = thisTime;


	// Go through all objects and apply Physics!

		for( u16 i = 0;  i < OBJECT.size();  i++ )
		{
		// any point?
			if( (!OBJECT[ i ].node) || (OBJECT[ i ].bIsFrozen) ) continue;

			vector3df newForce( 0,0,0 );

		// Gravity
			if( OBJECT[ i ].bGravity )
			{ newForce += Gravity; }

		// User Force
			newForce += OBJECT[ i ].userForce;

		// Calculate Acceleration and Velocity from Force
			OBJECT[ i ].Velocity += newForce / OBJECT[ i ].Mass;

		// User Velocity
			OBJECT[ i ].Velocity += OBJECT[ i ].userVel;

		// Move the node
			vector3df nPos = OBJECT[ i ].node->getAbsolutePosition();
				nPos += OBJECT[ i ].Velocity * dt;
			OBJECT[ i ].node->setPosition( nPos );

		// Reset for Next frame
			OBJECT[ i ].userForce = vector3df( 0,0,0 );
			OBJECT[ i ].userVel = vector3df( 0,0,0 );
		}
	}



	void setSimPaused( bool pauseIt )
	{
		bSimActive = pauseIt;
	}

	bool getSimPaused()
	{
		return bSimActive;
	}

	
	
	void setGravity( const vector3df &grav )
	{
		Gravity = grav;
	}


//////////

	u32 addNodeForPhysics( ISceneNode* node, const f32 &mass )	// mass in KG
	{
		CirrSPObject tObject;
			tObject.Mass = mass;
			tObject.node = node;

		OBJECT.push_back( tObject );

		return( OBJECT.size() - 1 );
	}

	ISceneNode* getNodeFromObject( const u32 &objectID )
	{
		if( objectID >= OBJECT.size() ) return 0;
		return( OBJECT[ objectID ].node );
	}

//////////

	vector3df &getObjectVelocity( const u32 &objectID )
	{
		static vector3df n( 0,0,0 );
		if( objectID >= OBJECT.size() ) return n;
		return OBJECT[ objectID ].Velocity;
	}
	
	void setObjectVelocity( const u32 &objectID, const vector3df &vel  )
	{
		if( objectID >= OBJECT.size() ) return;
		OBJECT[ objectID ].Velocity = vel;
	}
	
//////////
	
	void resetPhysicsForObject( const u32 &objectID, bool eraseNode )
	{
		if( objectID >= OBJECT.size() ) return;
		OBJECT[ objectID ].reset( eraseNode );
	}
	
//////////

	void setObjectUserForce( const u32 &objectID, const vector3df &force )
	{
		if( objectID >= OBJECT.size() ) return;
		OBJECT[ objectID ].userForce = force;
	}
	
	void setObjectUserVelocity( const u32 &objectID, const vector3df &velocity )
	{
		if( objectID >= OBJECT.size() ) return;
		OBJECT[ objectID ].userVel = velocity;
	}

	void setObjectAffectedByGravity( const u32 &objectID, const bool doesIt )
	{
		if( objectID >= OBJECT.size() ) return;
		OBJECT[ objectID ].bGravity = doesIt;
	}


//////////
	
	void addObjectUserForce( const u32 &objectID, const vector3df &force )
	{
		if( objectID >= OBJECT.size() ) return;
		OBJECT[ objectID ].userForce += force;
	}

//////////


};
