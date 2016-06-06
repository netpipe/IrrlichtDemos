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


class CirrPhysicsObject
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

	CirrPhysicsObject()
	{
		reset( true );
	}

	~CirrPhysicsObject()
	{
		reset( true );
	}
};



class CirrPhysics : public scene::ISceneNode
{

	core::aabbox3d<f32> Box;
	video::SMaterial Material;

	vector3df Gravity;					// gravity with which to affect all objects every frame
	array<CirrPhysicsObject> OBJECT;	// all objects irrPhysics knows about

	f32 dt;
	u32 thisTime;
	u32 lastTime;

	IrrlichtDevice* device;

	bool bSimActive;


public:

	CirrPhysics(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IrrlichtDevice* idevice, const vector3df &defaultGravity )
		: scene::ISceneNode(parent, mgr, id, vector3df(0,0,0) )
	{

	// irrphysics vars

		device = idevice;
		Gravity = defaultGravity;

		dt = 1;
		thisTime = device->getTimer()->getTime();
		lastTime = thisTime;

		bSimActive = true;


	// misc
		video::S3DVertex Vertices[4];
		Vertices[0] = video::S3DVertex(0,0,10000, 1,1,0, video::SColor(255,0,255,255), 0, 1);
		Vertices[1] = video::S3DVertex(10000,0,-10000, 1,0,0, video::SColor(255,255,0,255), 1, 1);
		Vertices[2] = video::S3DVertex(0,20000,0, 0,1,1, video::SColor(255,255,255,0), 1, 0);
		Vertices[3] = video::S3DVertex(-10000,0,-10000, 0,0,1, video::SColor(255,0,255,0), 0, 0);

		Box.reset(Vertices[0].Pos);
		for (s32 i=1; i<4; ++i) Box.addInternalPoint(Vertices[i].Pos);
	}

	virtual void OnRegisterSceneNode()
	{
		if( bSimActive ) SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}


	virtual void render()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);


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


	virtual const core::aabbox3d<f32>& getBoundingBox() const { return Box; }
	virtual u32 getMaterialCount() const { return 1; }
	virtual video::SMaterial& getMaterial(u32 i) { return Material; }




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



	u32 addNodeForPhysics( ISceneNode* node, const f32 &mass )	// mass in KG
	{
		CirrPhysicsObject tObject;
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

	
	
	void resetPhysicsForObject( const u32 &objectID, bool eraseNode )
	{
		if( objectID >= OBJECT.size() ) return;
		OBJECT[ objectID ].reset( eraseNode );
	}
	

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
};
