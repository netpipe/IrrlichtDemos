#include <irrlicht.h>


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

// gameplay
const f32 Thrust = 0.00008f;
const f32 SideThrust = 0.00006f;
const f32 WalkSpeed = 0.0025f;
const f32 Resistance = 0.991f;
const vector2df MaxSpeed(0.007f, 0.01f);
const f32 Gravity = 0.002f;
const s32 MaxEnemies = 15;
const s32 MaxBullets = 6;
const f32 PhaserSpeed = 0.02f;
const u32 TimeBetweenShots = 100;

// states
enum STATES
{
	STATE_MENU, // the menu
	STATE_GAMEOVER, // game over screen
	STATE_PLAYING, // playing the game
	STATE_LOST_LIFE, // just lost a life, waiting...
	STATE_LAUNCH, // launching
	STATE_PAUSED, // pause game (P key)
	STATE_QUIT // the game is exiting
};

// zx spectrum colours from wikipedia, all 15
enum ZXCOLOUR
{
	Black = 0,
	Blue0 = 0xFF0000be,
	Blue1 = 0xFF0000FF,
	Red0 = 0xFFBE0000,
	Red1 = 0xFFFF00FF,
	Magenta0 = 0xFFbe00be,
	Magenta1 = 0xFFFF00FF,
	Green0 = 0xFF00be00,
	Green1 = 0xFF00ff00,
	Cyan0 = 0xFF00bebe,
	Cyan1 = 0xFF00FFFF,
	Yellow0 = 0xFFbebe00,
	Yellow1 = 0xFFFFFF00,
	White0 = 0xFFbebebe,
	White1 = 0xFFFFFFFF,

	// for fuel in tank
	Magenta2 = 0xA0BE00BE
};



// layout
const f32 MaxY = 6.0f;
const f32 MaxX = 5.0f;

// gfx
const f32 PhaserWidth = 0.01f;

// laziness
IMesh *billboardMesh=0;



class GameEntity : public ISceneNode
{
public:
	GameEntity(ISceneManager* mgr, vector3df position=vector3df(0,0,0),vector3df scale=vector3df(1,1,1))
	 : ISceneNode(mgr->getRootSceneNode(), mgr, -1, position),
	 Velocity(0,0,0), ExpiryTime(0),
	 HasGravity(false), IsDestructable(true), IsAlive(true), IsFragile(false),
	 Expires(false), Resting(false), LastTime(0), IsCarryable(false),
	 Score(0), PickUpStage(0), LevelUp(false), LeftFacing(false)
	{
		setAutomaticCulling(EAC_OFF);
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const { return Box; }
	virtual void OnAnimate(u32 timeMs)
	{
		if (!LastTime)
			LastTime=timeMs;

		f32 t = (f32)(timeMs-LastTime);
		LastTime = timeMs;
		if (!t)
			return;

		vector3df newP = getPosition();

		if (HasGravity && !Resting)
		{
			newP -= vector3df(0,Gravity*t,0);
			Velocity -= Velocity * (1-powf(Resistance,t));
		}

		newP += Velocity*t;

		// is the position off the screen?
		if (newP.X > MaxX || newP.X < -MaxX)
		{
			newP.X += (MaxX*2) * ( newP.X < 0 ? 1 : -1);
		}
		if (newP.Y < -MaxY)
		{
			newP.Y = MaxY *0.25f;
		}

		setPosition(newP);
		ISceneNode::OnAnimate(timeMs);
	}
	virtual void render() { }
	void setLeftFacing(bool facing)
	{
		LeftFacing = facing;
		setRotation(vector3df(0, LeftFacing ? 180.0f : 0, 0));
	}

	vector3df Velocity;
	bool HasGravity, // does it fall
		 IsDestructable, // can it be destroyed?
		 IsAlive, // is it playing?
		 IsFragile, // does it break if it hits something solid?
		 Expires, // does it expire?
		 Resting, // has it landed?
		 IsCarryable, // is it something you carry round?
		 LevelUp, // when it hits the ground, does the ship get more complete?
		 LeftFacing; // is it flipped?
	u32 ExpiryTime;
	s32 Score, PickUpStage;
	aabbox3d<f32> Box;
	u32 LastTime;
};

class Billboard : public GameEntity
{
public:
	 Billboard(ISceneManager* mgr, ITexture* textures, s32 tcount=1, vector3df pos=vector3df(0,0,0), vector3df scale = vector3df(0.5,0.5,0.5))
		 : GameEntity(mgr, pos), Bill(0), Textures(textures), Count(tcount)
	 {
		Bill = mgr->addMeshSceneNode(billboardMesh, this, -1, pos, vector3df(0,0,0));
		setScale(scale);
		updateAbsolutePosition();
		Bill->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
		Bill->setMaterialTexture(0, textures);
		Box = Bill->getTransformedBoundingBox();
		Box.MaxEdge -= getPosition();
		Box.MinEdge -= getPosition();
		Bill->updateAbsolutePosition();
	}

	IMeshSceneNode *Bill;
	ITexture *Textures;
	s32 Count;
};


class Player : public Billboard, public IEventReceiver
{
public:
	Player(ISceneManager* mgr, ITexture* textures, s32 tcount=1)
		: Billboard(mgr, textures, tcount, vector3df(0,0,0),vector3df(0.25,0.5,0.5)),
		Boost(false), Direction(false), Shooting(false)
	{
		HasGravity = true;
		IsAlive = true;
		IsDestructable = true;
		PickUp = 0;

        //JETPAC Exhaust
		Particles = mgr->addParticleSystemSceneNode(false, this, -1, vector3df(-0.7f,-0.3f,0));
		Particles->setParticlesAreGlobal(false);
		IParticleEmitter *e= Particles->createPointEmitter(vector3df(0,-0.002f,0), 50, 200,
			SColor(255,0,0,0), SColor(255,0,0,0),200,600,30 ,core::dimension2df(.020f,.020f),core::dimension2df(.15f,0.15f));
		Particles->setEmitter(e);
		e->drop();

		reset();
	}


	void reset()
	{
		setPosition(vector3df(0,MaxY*0.25f,0));
		updateAbsolutePosition();
		Velocity = vector3df(0,0,0);
		LastTime = 0;
		Boost = false;
		Direction = false;
		Shooting = false;
		setVisible(true);
		IsAlive = true;
	}

	virtual void OnAnimate(u32 timeMs)
	{
		f32 t = (f32)(timeMs-LastTime);

		Particles->setVisible(!Resting && (Boost || Direction));
		if (Resting)
		{
			if (Direction)
				Velocity.X = LeftFacing ? -WalkSpeed : WalkSpeed;
			else
				Velocity.X = 0;
		}
		else
		{
			if (Boost)
				Velocity += vector3df(0,Thrust*t,0);
			if (Direction)
				Velocity += vector3df((SideThrust*t) * (LeftFacing ? -1 : 1 ),0,0);
		}


		if (Velocity.X > MaxSpeed.X) Velocity.X = MaxSpeed.X;
		if (Velocity.X < -MaxSpeed.X) Velocity.X = -MaxSpeed.X;
		if (Velocity.Y > MaxSpeed.Y) Velocity.Y = MaxSpeed.Y;
		if (Velocity.Y < -MaxSpeed.Y) Velocity.Y = -MaxSpeed.Y;

		vector3df p = getPosition();
		if (p.Y > MaxY)
		{
			p.Y=MaxY;
			setPosition(p);
			Velocity.Y*= -0.3f;
		}
		GameEntity::OnAnimate(timeMs);

		if (PickUp)
			PickUp->setPosition(getAbsolutePosition());

	}

	void setPickUp(GameEntity * pickup)
	{
		PickUp = pickup;
	}

	virtual bool OnEvent(const SEvent& e)
	{
		// process keyboard input
		switch(e.KeyInput.Key)
		{
		case KEY_UP:
			Boost = e.KeyInput.PressedDown;
			if (Boost && Resting)
			{
				setPosition(getPosition() + vector3df(0,0.05f,0));
				Resting=false;
			}
			return true;
		case KEY_RIGHT:
			if (LeftFacing && e.KeyInput.PressedDown)
				setLeftFacing(false);

			Direction=e.KeyInput.PressedDown;

			return true;
		case KEY_LEFT:
			if (!LeftFacing && e.KeyInput.PressedDown)
				setLeftFacing(true);

			Direction=e.KeyInput.PressedDown;
			return true;

		case KEY_LSHIFT:
			Shooting=e.KeyInput.PressedDown;

			return true;

		}
		return false;
	}

	IParticleSystemSceneNode *Particles;
	GameEntity *PickUp;
	bool Boost, Direction, Shooting;
};

class Enemy : public Billboard
{
public:
	Enemy(ISceneManager* mgr, ITexture* texture, vector3df pos, ZXCOLOUR c, s32 ai=0)
		: Billboard(mgr, texture, 1),
		AI(ai), NextTime(0)
	{
		IsAlive = true;
		IsDestructable = true;
		Bill->getMaterial(0).AmbientColor = c;
		Colour = c;
		setPosition(pos);
		setScale(vector3df(0.3f,0.3f,0.3f));
		updateAbsolutePosition();
		Bill->updateAbsolutePosition();

		f32 direction =  (rand() % 2) ? 1.f : -1.f;
		f32 direction2=  (rand() % 2) ? 1.f : -1.f;

		Score = 50;

		switch(AI)
		{
		case 0: // floaty
			{
			IsFragile = true;
			f32 speed = (f32) (rand() % 4)+1;
			Velocity.Y = -0.0001f * speed;
			Velocity.X = 0.0005f * speed * direction;
			break;
			}
		case 1: // bouncey fuzzballs
			{
			Score = 60;
			Velocity.Y = -0.0025f * direction2;
			Velocity.X = 0.0025f * direction;
			break;
			}
		case 2: // bubbles
			{
			Score = 80;
			Velocity.Y = -0.0015f * direction2;
			Velocity.X = 0.0015f * direction;
			break;
			}
		case 3: // jet things
			{
			Score = 70;
			Velocity.Y = -0.0015f * direction2;
			Velocity.X = 0.003f;
			break;
			}
		case 4: // ufos
			{
			Score = 100;
			Velocity.Y = -0.003f * direction2;
			Velocity.X = 0.003f * direction;
			break;
			}
		default:

			break;
		}
		setLeftFacing(Velocity.X < 0);

	}
	s32 AI;
	u32 NextTime;
	ZXCOLOUR Colour;
};

class Bonus : public GameEntity
{
public:
	Bonus(ISceneManager* mgr, IMesh *part, s32 score, ZXCOLOUR col, vector3df pos)
		: GameEntity(mgr, pos)
	{
		HasGravity = true;
		IsDestructable = false;
		Score = score;
		Node = mgr->addMeshSceneNode(part, this, -1,
			vector3df(0,-part->getBoundingBox().getCenter().Y,0), vector3df(0,0,0));
		Node->getMaterial(0).AmbientColor = col;
		setScale(vector3df(0.3f,0.3f,0.3f));
		updateAbsolutePosition();
		Box = Node->getTransformedBoundingBox();
		Box.MaxEdge -= getPosition();
		Box.MinEdge -= getPosition();
		ISceneNodeAnimator *anm = mgr->createRotationAnimator( vector3df(0,1.0f,0));
		Node->addAnimator(anm);
		anm->drop();

	}
	IMeshSceneNode *Node;
};

class RocketPart : public GameEntity
{
public:
	RocketPart(ISceneManager* mgr, IMesh *part, s32 PartNo, vector3df pos) : GameEntity(mgr, pos)
	{
		HasGravity = true;
		IsCarryable = true;
		Score = 100;

		PickUpStage = PartNo;

		Node = mgr->addMeshSceneNode(part, this, -1,
			vector3df(0,-part->getBoundingBox().getCenter().Y,0), vector3df(0,0,0));
		setScale(vector3df(0.3f,0.3f,0.3f));
		Box = Node->getTransformedBoundingBox();
		updateAbsolutePosition();
		Node->updateAbsolutePosition();
		Box.MaxEdge -= getPosition();
		Box.MinEdge -= getPosition();

		if (PartNo > 2)
			setLeftFacing(true); // oops modeled the wrong way around

	}
	IMeshSceneNode* Node;
};


class Rocket : public GameEntity
{
public:
	Rocket(ISceneManager* mgr, IMesh *part, IMesh *part2, IMesh *part3, IVideoDriver *driver) : GameEntity(mgr)
	{
		Driver = driver;
		Target = driver->addRenderTargetTexture(dimension2du(256,256), "Rocket");

		Mesh1 = mgr->addMeshSceneNode(part, this);
		OriginalTex = Mesh1->getMaterial(0).getTexture(0);
		Mesh1->setMaterialTexture(0, Target);
		Mesh2 = mgr->addMeshSceneNode(part2, this);
		Mesh2->setMaterialTexture(0, Target);
		Mesh3 = mgr->addMeshSceneNode(part3, this);
		Mesh3->setMaterialTexture(0, Target);

        //Rocket Exhaust
		Particles = mgr->addParticleSystemSceneNode(false, this);
		Particles->setParticleSize(core::dimension2df(0.05f,0.05f));
//		Particles->setMinParticleStart(core::dimension2df(0.05f,0.05f));
//		Particles->setMaxParticleStart(core::dimension2df(0.05f,0.05f));

		Particles->setParticlesAreGlobal(false);

		IParticleEmitter *e= Particles->createPointEmitter(vector3df(0,-0.002f,0), 50, 200,
			SColor(255,0,0,0), SColor(255,0,0,0),100,300, 30 ,
			core::dimension2df(0.50f,0.5f),core::dimension2df(1.0f,1.0f));

		Particles->setEmitter(e);
		e->drop();
		Particles->setVisible(false);

		f32 Yoffset = Mesh1->getTransformedBoundingBox().MinEdge.Y - getPosition().Y;
		setStage(1);
		setScale( vector3df(0.3f, 0.3f, 0.3f));
		setPosition(vector3df(MaxX*0.3f, -Yoffset/2.0f, 0));
		OrigY = getPosition().Y;
		updateAbsolutePosition();
		Mesh1->updateAbsolutePosition();
		Mesh2->updateAbsolutePosition();
		Mesh3->updateAbsolutePosition();
		Flash=true;
	}
	~Rocket()
	{
		if (Target)
			Target->drop();
	}



	void setStage(s32 stage)
	{
		Stage = stage;
		Mesh1->setVisible(true);
		Mesh2->setVisible((Stage > 1));
		Mesh3->setVisible((Stage > 2));
		Particles->setVisible(Stage > 9);

		if (Stage==9)
			Flash =! Flash;

		if (Stage==10)
			Velocity.Y = 0.003f;

		if (Stage==11)
			Velocity.Y = -0.002f;

		NeedsUpdate = true;
	}


	virtual void OnRegisterSceneNode()
	{
		if (NeedsUpdate)
		{
			Driver->setRenderTarget(Target, true, true, video::SColor(255,255,255,255));
			Driver->draw2DImage(OriginalTex, position2di(0,0));
			if ( (Stage > 3 && Stage < 9) || (Stage==9 && Flash) )
			{
				s32 p = 256-( (256/6) * (Stage-3));
				rect<s32> pos(0,p,256,256);
				Driver->draw2DRectangle((SColor)Magenta2, pos);
			}
			Driver->setRenderTarget(0);
			NeedsUpdate = false;
		}
		ISceneNode::OnRegisterSceneNode();
	}

	IParticleSystemSceneNode *Particles;
	IMeshSceneNode *Mesh1, *Mesh2, *Mesh3;
	s32 Stage;
	bool Flash, NeedsUpdate;
	f32 OrigY;
	ITexture *Target, *OriginalTex;
	IVideoDriver *Driver;

};



class Ledge : public GameEntity
{
public:
	Ledge(ISceneManager* mgr, f32 width, vector3df pos, ZXCOLOUR col, IMesh *mesh) : GameEntity(mgr, pos)
	{
		IsAlive = true;

		updateAbsolutePosition();
		Node = mgr->addMeshSceneNode(mesh, this);
		Node->getMaterial(0).AmbientColor = (SColor)col;
		Node->setScale(vector3df(width*0.60f, 0.15f, 0.25f));
		Node->updateAbsolutePosition();
		Box = Node->getTransformedBoundingBox();
		Box.MaxEdge -= getPosition();
		Box.MinEdge -= getPosition();

	}

	ISceneNode *Node;
};

class PhaserFire : public GameEntity
{
public:
	PhaserFire(ISceneManager* mgr) : GameEntity(mgr)
	{
		Finished = true;
	}

	void shoot(vector3df pos, s32 direction, u32 length)
	{
		u32 c = rand() % 3;
		switch (c)
		{
		case 0: Mat.AmbientColor = (SColor)Magenta1; break;
		case 1: Mat.AmbientColor = (SColor)White1; break;
		case 2: Mat.AmbientColor = (SColor)Blue1; break;
		//case 3: Mat.AmbientColor = (SColor)Yellow1; break;
		//case 4: Mat.AmbientColor = (SColor)Cyan1; break;
		}
		StartY = pos.Y;
		StartX = pos.X;
		EndX = pos.X;
		Direction = PhaserSpeed * (f32)direction;
		Length = length;
		Box.reset(pos);
		LastTime=0;
		Finished = false;
		Stopped = false;
	}

	virtual void OnAnimate(u32 timeMs)
	{
		if (!LastTime)
		{
			LastTime=timeMs;
			EndTime = timeMs+Length;
			return;
		}

		f32 tdif = (f32)(timeMs-LastTime);
		LastTime=timeMs;

		if (timeMs > EndTime)
		{
			// move the tail, check if we've finished
			StartX += Direction * tdif;
			if ((Direction > 0 && StartX > EndX) || (Direction < 0 && EndX > StartX) )
			{
				Finished = true;
			}

		}
		else if (!Stopped)
		{
			EndX += Direction * tdif;
		}

		Box.reset(vector3df(StartX, StartY-PhaserWidth,-PhaserWidth ));
		Box.addInternalPoint(vector3df(EndX, StartY+PhaserWidth,PhaserWidth ));

		ISceneNode::OnAnimate(timeMs);

	}

	virtual void OnRegisterSceneNode()
	{
		if (!Finished)
		{
			SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
		}
		ISceneNode::OnRegisterSceneNode();
	}
	virtual void render()
	{
		SceneManager->getVideoDriver()->setMaterial(Mat);
		SceneManager->getVideoDriver()->setTransform(ETS_WORLD, AbsoluteTransformation);

		SceneManager->getVideoDriver()->draw3DBox(Box);
		aabbox3df b2;
		if (Box.MaxEdge.X > MaxX)
		{
			b2 = Box;
			b2.MaxEdge.X -= MaxX*2;
			b2.MinEdge.X -= MaxX*2;
			SceneManager->getVideoDriver()->draw3DBox(b2);
		}
		if (Box.MinEdge.X < -MaxX)
		{
			b2 = Box;
			b2.MaxEdge.X += MaxX*2;
			b2.MinEdge.X += MaxX*2;
			SceneManager->getVideoDriver()->draw3DBox(b2);
		}
	}

	SMaterial Mat;
	f32 StartY, StartX, EndX;
	bool Stopped, Finished;
	u32 Length;
	f32 Direction;
	u32 LastTime, EndTime;
};

class Explosion : public Billboard
{
public:
	Explosion(ISceneManager* mgr, array<ITexture*> &textures, vector3df pos, ZXCOLOUR col=White1, u32 Length=300)
		: Billboard(mgr, textures[0], 1)
	{
		IsAlive = true;
		IsDestructable = false;
		Expires = true;
		// create texture animator
		ISceneNodeAnimator *anm = mgr->createTextureAnimator( textures, Length / textures.size(), false);
		Bill->addAnimator(anm);
		anm->drop();
		Bill->getMaterial(0).AmbientColor = col;
		// deletion animator
		anm = mgr->createDeleteAnimator(Length);
		addAnimator(anm);
		anm->drop();

		setPosition(pos);
		updateAbsolutePosition();
		Bill->updateAbsolutePosition();

	}
};
