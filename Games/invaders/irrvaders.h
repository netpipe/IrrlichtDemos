
/*

	Circular Irrvaders

	Quite a bit longer than I expected it to be, and not as nice as if I'd actually planned it!
	It's a great big horrible copy and paste job, but it was fun and quick to write
	Too much memory management. shouldnt have made everything derive from IUnknown, but too late now!

*/

#ifndef __IRRVADERS_H__
#define __IRRVADERS_H__

#include <irrlicht.h>
#include "CircularSpriteCanvasSceneNode.h"

namespace irr
{
namespace game
{

	using namespace core;
	using namespace scene;
	using namespace video;
	using namespace io;
	using namespace gui;

	class CGameEntity : public IReferenceCounted
	{
	public:

		CGameEntity(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos)
			:	Canvas(canvas), Position(pos), Texture(tex), Hits(1),
				hasSprite(false), SpritesX(1), SpritesY(1), StartSprite(0), Frames(1), Speed(500), CurrentTime(0)
		{
			// grab texture
			if (tex)
				tex->grab();
		}
		~CGameEntity()
		{
			if (Texture)
				Texture->drop();
		}

		virtual void createSprite(u32 spritesx, u32 spritesy, u32 startSprite,
								  u32 frames=1, u32 speed=500, bool flipx=false, bool flipy=false)
		{
			hasSprite = true;
			SpritesX = spritesx;
			SpritesY = spritesy;
			StartSprite = startSprite;
			Frames = frames;
			if (!Frames)
				Frames=1;
			Speed = speed;
			if (!Speed)
				Speed++;
			FlipX = flipx;
			FlipY = flipy;
		}

		virtual void move(f32 x, f32 y)
		{
			Position.UpperLeftCorner.X += x;
			Position.UpperLeftCorner.Y += y;
			Position.LowerRightCorner.X += x;
			Position.LowerRightCorner.Y += y;
		}

		virtual void draw(u32 timePassed)
		{
			if (hasSprite)
			{
				u32 cx, cy, cs;
				CurrentTime += timePassed;
				cs = ((CurrentTime / Speed) % Frames) + StartSprite;
				cx = cs % SpritesX;
				cy = cs / SpritesX;
				Canvas->drawSprite(Texture, SpritesX, SpritesY, cx, cy, Position, FlipX, FlipY);
			}
		}

		virtual bool checkCollision(CGameEntity *other)
		{
			return Position.isRectCollided(other->Position);
		}
		// returns true if it was destroyed
		virtual bool hit(CGameEntity *other)
		{
			if (--Hits <0 )
				Hits =0;

			return Hits == 0;
		}

		rect<f32> Position;
		CircularSpriteCanvasSceneNode *Canvas;
		video::ITexture *Texture;
		s32 Hits;
		bool hasSprite;
		u32 SpritesX, SpritesY, StartSprite, Frames, Speed, CurrentTime;
		bool FlipX, FlipY;
	};

	class CBaseSegment : public CGameEntity
	{
	public:
		CBaseSegment(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos,
						s32 area, bool flipx, bool flipy)
			: CGameEntity(canvas, tex, pos)
		{
			Hits=4;
			createSprite(4, 2, area, 0, 100, flipx, flipy);
		}

		virtual bool hit(CGameEntity *other)
		{
			// change sprite
			if (Hits > 1)
				StartSprite++;
			return CGameEntity::hit(other);
		}

	};

	class CBase : public CGameEntity
	{
	public:
		CBase(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos)
			: CGameEntity(canvas, tex, pos), Segments()
		{
			// create segments
			//
			rect<f32> p(pos);
			position2df	w(pos.getWidth() / 4.0f, 0),
								h(0, pos.getHeight() / 2.0f);
			p.LowerRightCorner.X = pos.UpperLeftCorner.X + w.X;
			p.LowerRightCorner.Y = pos.UpperLeftCorner.Y + h.Y;
			// top
			Segments.push_back( new CBaseSegment(canvas, tex, p, 0, false, false ) );
			p += w;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 4, false, false ) );
			p += w;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 4, false, false ) );
			p += w;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 0, true, false ) );
			// middle
			p -= w*3; p += h;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 4, false, false ) );
			p += w;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 0, true, true ) );
			p += w;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 0, false, true ) );
			p += w;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 4, false, false) );
			// bottom
			p -= w*3; p += h;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 4, false, false ) );
			p += w*3;
			Segments.push_back( new CBaseSegment(canvas, tex, p, 4, false, false ) );

		}

		virtual bool checkCollision(CGameEntity *other)
		{
			if (CGameEntity::checkCollision(other))
			{
				// check remaining base segments...
				for (u32 i=0; i<Segments.size(); ++i)
					if (Segments[i]->checkCollision(other))
						return true;
			}
			return false;
		}
		virtual void draw(u32 timePassed)
		{
			// draw all base segments
			for (u32 i=0; i<Segments.size(); ++i)
				Segments[i]->draw(timePassed);
		}

		virtual bool hit(CGameEntity *other)
		{
			// check remaining base segments...
			for (u32 i=0; i<Segments.size(); ++i)
				if (Segments[i]->checkCollision(other))
				{
					// remove segment if it is destroyed
					if (Segments[i]->hit(other))
					{
						Segments[i]->drop();
						Segments.erase(i--);
						recalculateSize();
					}
					if (Segments.size() == 0)
						return true;
				}
			return false;
		}

		void recalculateSize()
		{
			if (!Segments.size())
			{
				Position.LowerRightCorner = Position.UpperLeftCorner = position2df(0,0);
				return;
			}

			Position.LowerRightCorner = Position.UpperLeftCorner = Segments[0]->Position.UpperLeftCorner;

			for (u32 i=0; i<Segments.size(); ++i)
			{
				Position.addInternalPoint(Segments[i]->Position.UpperLeftCorner);
				Position.addInternalPoint(Segments[i]->Position.LowerRightCorner);
			}
		}

		void dropSegments()
		{
			for (u32 i=0; i< Segments.size(); ++i)
				Segments[i]->drop();

			Segments.clear();
		}

		~CBase()
		{
			dropSegments();
		}

		array <CBaseSegment*> Segments;
	};

	class CInvader : public CGameEntity
	{
	public:
		CInvader(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos, u32 start)
			: CGameEntity(canvas, tex, pos)
		{
			// create enemy sprite
			createSprite(2,2,start,2,500);
		}
		void setBeefCake(ITexture *newtex)
		{
			if (Texture)
				Texture->drop();
			Texture = newtex;
			Texture->grab();

			Hits = 5;
		}
	};

	class CUFO : public CGameEntity
	{
	public:
		CUFO(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos)
			: CGameEntity(canvas, tex, pos)
		{
			// create ufo
			createSprite(1,1,0);
		}

		virtual bool hit(CGameEntity *other)
		{
			if (CGameEntity::hit(other))
			{
				Canvas->explosion(Position.getCenter());
				return true;
			}
			return false;
		}
	};

	class CPowerUp : public CGameEntity
	{
	public:
		CPowerUp(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos, u32 num)
			: CGameEntity(canvas, tex, pos)
		{
			createSprite(4,4,num);
		}

		virtual bool hit(CGameEntity *other)
		{
			if (CGameEntity::hit(other))
			{
				Canvas->explosion(Position.getCenter());
				return true;
			}
			return false;
		}
	};

	class CInvaders : public CGameEntity
	{
	public:
		CInvaders(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos, u32 columns, u32 rows)
			: CGameEntity(canvas, tex, pos), Invaders(), Direction(0), StopY(0)
		{
			// create invaders
			//
			rect<f32> p(pos);
			position2df	w(pos.getWidth() / (f32)columns/2.0, 0),
						h(0, pos.getHeight() / (f32)rows/2.0f);
			p.LowerRightCorner.X = pos.UpperLeftCorner.X + w.X;
			p.LowerRightCorner.Y = pos.UpperLeftCorner.Y + h.Y;
			rect<f32> p2(p);

			u32 s=0;
			for (u32 y=0; y<rows; ++y)
			{
				if (y>=rows/2)
					s=2;
				p = p2;
				p += h * (2.f * (f32)y);
				for (u32 x=0; x<columns; ++x)
				{
					Invaders.push_back(new CInvader(Canvas, tex, p, s));
					p += w * 2.f;
				}
			}
		}
		void setBeefCake(ITexture *newtex)
		{
			// super hard invaders!

			if (Texture)
				Texture->drop();
			Texture = newtex;
			if (Texture)
				Texture->grab();

			for (u32 i=0; i<Invaders.size(); ++i)
				Invaders[i]->setBeefCake(newtex);
		}

		virtual bool checkCollision(CGameEntity *other)
		{
			if (CGameEntity::checkCollision(other))
			{
				// check remaining base segments...
				for (u32 i=0; i<Invaders.size(); ++i)
					if (Invaders[i]->checkCollision(other))
						return true;
			}
			return false;
		}
		virtual void draw(u32 timePassed)
		{
			// draw all base segments
			for (u32 i=0; i<Invaders.size(); ++i)
				Invaders[i]->draw(timePassed);
		}

		virtual bool hit(CGameEntity *other)
		{
			// check invaders...
			for (u32 i=0; i<Invaders.size(); ++i)
				if (Invaders[i]->checkCollision(other))
				{
					// remove invader if hit
					if (Invaders[i]->hit(other))
					{
						// explode!
						Canvas->explosion(Invaders[i]->Position.getCenter());

						Invaders[i]->drop();
						Invaders.erase(i--);
						recalculateSize();
					}
					if (Invaders.size() == 0)
						return true;
				}
			return false;
		}
		virtual void move(f32 x, f32 y)
		{
			// move all invaders
			for (u32 i=0; i<Invaders.size(); ++i)
				Invaders[i]->move(x,y);

			// move box
			CGameEntity::move(x,y);

		}

		void recalculateSize()
		{
			if (!Invaders.size())
			{
				return;
				Position.LowerRightCorner = Position.UpperLeftCorner = position2df(0,0);
			}

			Position.LowerRightCorner = Position.UpperLeftCorner = Invaders[0]->Position.UpperLeftCorner;

			for (u32 i=0; i<Invaders.size(); ++i)
			{
				Position.addInternalPoint(Invaders[i]->Position.UpperLeftCorner);
				Position.addInternalPoint(Invaders[i]->Position.LowerRightCorner);
			}
		}

		void dropInvaders()
		{
			for (u32 i=0; i< Invaders.size(); ++i)
				Invaders[i]->drop();

			Invaders.clear();
		}

		~CInvaders()
		{
			dropInvaders();
		}

		array <CInvader*> Invaders;
		u32 Direction;
		f32 StopY;
	};


	class CBullet : public CGameEntity
	{
	public:
		CBullet(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos, f32 speed)
			: CGameEntity(canvas, tex, pos), Speed(speed)
		{
			// create bullet sprite
			createSprite(1,1,0);
		}
		f32 Speed;
	};

	class CPlayer : public CGameEntity
	{
	public:
		CPlayer(CircularSpriteCanvasSceneNode *canvas, video::ITexture *tex, rect<f32> pos)
			: CGameEntity(canvas, tex, pos)
		{
			// create player sprite
			createSprite(1,1,1);
		}

		virtual bool hit(CGameEntity *other)
		{
			if (CGameEntity::hit(other))
			{
				Canvas->explosion(Position.getCenter());
				return true;
			}
			return false;
		}

	};



	// the game
	class CIrrvaders : public IReferenceCounted
	{
	public:

		enum E_GAME_MODE
		{
			EGM_MENU=0,
			EGM_PLAY,
			EGM_PAUSE
		};
		enum E_GAME_TEXTURES
		{
			EGT_PLAYER=0,
			EGT_INVADER,
			EGT_BASE,
			EGT_BULLET,
			EGT_UFO,
			EGT_POWERUP,
			EGT_BEEFCAKE,
			EGT_SIZE
		};

		enum E_GAME_POWERUP
		{
			EGPU_EXTRA_BULLET=0,
			EGPU_EXTRA_ENEMY_BULLET,
			EGPU_SHIELD,
			EGPU_ENEMY_BEEFCAKE, // lol
			EGPU_RESET_BASES,
			EGPU_1UP,
			EGPU_DOUBLEWIDTH,
			EGPU_HALFWIDTH,
			EGPU_SPEEDUP,
			EGPU_SLOWDOWN,
			EGPU_WIDEBULLETS,
			EGPU_WIDEENEMYBULLETS,
			EGPU_100PTS,
			EGPU_1000PTS,
			EGPU_10000PTS,
			EGPU_BACKOFF,
			EGPU_COUNT
		};

		class MyEvents : public IEventReceiver
		{
		public:

			MyEvents()
			{
				for(s32 i=0; i<1024; i++)
					keys[i] = false;
			}

			bool OnEvent(const SEvent& event)
			{
				// process game events here...
				if(event.EventType == irr::EET_KEY_INPUT_EVENT)
				{
					keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
					return true;
				}
				return false;
			}

			bool keys[1024];

		};

		CIrrvaders() :
			// Irrlicht
			device(0), driver(0), smgr(0), guienv(0),
			// GUI
			Canvas(0), GUIScore(0), GUIHiScore(0), GUILives(0), CanvasRotation(0,180,180),
			// input
			Events(),
			// Game states
			QuitGame(false), Mode(EGM_MENU), MenuMode(0),
			Lives(3), Score(0), HiScore(1000),
			GameWidth(1000), GameHeight(1000), PlayerY(GameHeight-200), BaseY(PlayerY-100),
			GameTime(0), LastGameTime(0), NextUFOTime (0),
			// constants
			StartLives(3),
			// game entities
			Invaders(0), PlayerShip(0), UFO(0), PowerUp(0),
			Bases(), PlayerBullets(), EnemyBullets()
		{

			/*
				start Irrlicht
			*/
			device = createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 16, false, false, false, 0);
			device->setWindowCaption(L"Irrvaders");
			device->setEventReceiver(&Events);

			driver = device->getVideoDriver();
			smgr = device->getSceneManager();
			guienv = device->getGUIEnvironment();

			/*
				load textures
			*/

			driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS,false);

			IGUIFont *font = guienv->getFont("media/ZXFont.png");
			if (!font)
				font = guienv->getBuiltInFont();
			guienv->getSkin()->setFont(font);

			c8 *tmptexnames[] = { "media/player.png", "media/invader.png", "media/base.png",
								  "media/bullet.png", "media/ufo.png", "media/powerup.png",
								  "media/beefcake.png", 0 };

			for (s32 i=0; i<EGT_SIZE; ++i)
				Textures.push_back( driver->getTexture(tmptexnames[i]) );

			/*
				create GUI
			*/

			// Score
			GUIScore	= guienv->addStaticText(L"", rect<s32>(5,5,150,20), false, false);
			GUIScore->setOverrideColor(SColor(255,200,255,200));
			// HiScore
			GUIHiScore	= guienv->addStaticText(L"", rect<s32>(175,5,325,20), false, false);
			GUIHiScore->setOverrideColor(SColor(255,200,255,200));
			// men
			GUILives	= guienv->addStaticText(L"", rect<s32>(350,5,475,20), false, false);
			GUILives->setOverrideColor(SColor(255,200,255,200));

			// menu text
			MenuText1 = guienv->addStaticText(L"Use Z and X to move, space to fire\n\nPress S to start", rect<s32>(300,100,500,200));
			MenuText1->setOverrideColor(SColor(255,200,255,200));
			//MenuText2
			//MenuText3

			setScore(0);
			setHiScore(1000);
			setLives(StartLives);

			// disable mouse
			device->getCursorControl()->setVisible(false);

			Canvas = new CircularSpriteCanvasSceneNode(smgr, GameWidth, GameHeight);
			Canvas->setPosition(vector3df(0,-550,700));
			Canvas->setRotation(CanvasRotation);
			smgr->addCameraSceneNode();

			/*
				Game setup
			*/

			device->getTimer()->tick();
			MenuStartTime = GameTime = LastGameTime = device->getTimer()->getTime();

			setMenuMode();
		}
		~CIrrvaders()
		{
			// clear game
			clearGame();

			if (Canvas)
				Canvas->drop();

			// event reciever is deleted here, but still referenced in device
			device->setEventReceiver(0);
			device->drop();
		}

		void makeBases(u32 count=0, f32 width=0.0f)
		{
			// destroy bases
			for (u32 i=0; i<Bases.size(); ++i)
				Bases[i]->drop();

			Bases.clear();

			if (!count)
				return;

			f32 bh=50,
				bg=GameWidth / ((f32)count+1.0f),
				bw=GameWidth*width,
				bp=bg-bw/2;

			// add bases
			for (u32 i=0; i<count; ++i)
			{
				Bases.push_back(new CBase(Canvas, Textures[EGT_BASE], rect<f32>(bp,BaseY-bh/2,bp+bw,BaseY+bh/2)));
				bp+=bg;
			}
		}

		void setLives(s32 l)
		{
			wchar_t tmp[30];
			Lives = l;
			if (Lives > 5)
				swprintf(tmp, 30, L"Lives: ~x %d", Lives);
			else
				swprintf(tmp, 30, L"Lives: %c%c%c%c%c",
								Lives > 0 ? L'~' : ' ',
								Lives > 1 ? L'~' : ' ',
								Lives > 2 ? L'~' : ' ',
								Lives > 3 ? L'~' : ' ',
								Lives > 4 ? L'~' : ' ');
			// update text
			GUILives->setText(tmp);
		}
		void setScore(s32 s)
		{
			// bonus life each 10,000 pts
			if (s/10000 > Score/10000)
				setLives(Lives+1);

			wchar_t tmp[30];
			Score = s;
			if (Score > HiScore)
				setHiScore(Score);
			swprintf(tmp, 30, L"Score: %d", Score);
			GUIScore->setText(tmp);
		}
		void setHiScore(s32 hs)
		{
			wchar_t tmp[30];
			HiScore = hs;
			swprintf(tmp, 30, L"Hi: %d", HiScore);
			GUIHiScore->setText(tmp);
		}

		bool mainLoop()
		{
			if (!device->run())
				return false;

			driver->beginScene(true, true, SColor(255,0,80,0));

			LastGameTime = GameTime;
			GameTime = device->getTimer()->getTime();
			u32 t = GameTime - LastGameTime;

			switch (Mode)
			{
			case EGM_MENU:
				menuLoop();
				break;
			case EGM_PLAY:
				gameLoop();
				break;
			case EGM_PAUSE:
				t=0;
				// do nothing
				break;
			}

			// draw everything
			for (u32 i=0; i<Bases.size(); ++i)
				Bases[i]->draw(t);
			for (u32 i=0; i<PlayerBullets.size(); ++i)
				PlayerBullets[i]->draw(t);
			for (u32 i=0; i<EnemyBullets.size(); ++i)
				EnemyBullets[i]->draw(t);

			if (PlayerShip)
				PlayerShip->draw(t);
			if (Invaders)
				Invaders->draw(t);
			if (UFO)
				UFO->draw(t);
			if (PowerUp)
			  PowerUp->draw(t);

			smgr->drawAll();
			guienv->drawAll();
			driver->endScene();

			return !QuitGame;
		}

		void gameLoop()
		{
			f32 t = (f32)(GameTime-LastGameTime);
			bool dead = false;

			// player movement
			if (PlayerShip)
			{
				if (Events.keys[KEY_KEY_P])
				{
					Mode = EGM_PAUSE;
					GameTime -=500;
					return;
				}

				if (Events.keys[KEY_KEY_Z])
					PlayerShip->move(t*-1.f*PlayerSpeed,0);
				if (Events.keys[KEY_KEY_X])
					PlayerShip->move(t*PlayerSpeed,0);

				if (PlayerShip->Position.getCenter().X > GameWidth)
					PlayerShip->move(-GameWidth,0);
				if (PlayerShip->Position.getCenter().X < 0)
					PlayerShip->move(GameWidth,0);


				if (Events.keys[KEY_SPACE] && PlayerBullets.size() < MaxPlayerBullets && GameTime > NextPlayerFireTime)
				{
					PlayerBullets.push_back(
						new CBullet(Canvas, Textures[EGT_BULLET], PlayerBulletSize +
							position2df(PlayerShip->Position.UpperLeftCorner.X + PlayerShip->Position.getWidth()/2.f,
										PlayerShip->Position.UpperLeftCorner.Y - PlayerBulletSize.getHeight()),
							-PlayerBulletSpeed));
					NextPlayerFireTime = GameTime + PlayerReloadTime;
				}
			}

			// player bullet movement
			for (u32 i=0; i<PlayerBullets.size(); ++i)
			{
				bool collision = false;

				PlayerBullets[i]->move(0,PlayerBullets[i]->Speed*t);

				// check base collision
				for (u32 j=0; j<Bases.size(); ++j)
					if (Bases[j]->checkCollision(PlayerBullets[i]))
					{
						ConsecutiveHits =0;
						// base destroyed?
						if(Bases[j]->hit(PlayerBullets[i]))
						{
							Bases[j]->drop();
							Bases.erase(j--);
						}
						collision=true;
						break;
					}

				// check invader collision
				if (Invaders)
					if (Invaders->checkCollision(PlayerBullets[i]))
					{
						ConsecutiveHits++;
						setScore(Score + ConsecutiveHits*10);

						if (Invaders->hit(PlayerBullets[i]))
						{
							Invaders->drop();
							Invaders=0;
						}
						collision=true;
					}

				// check invader bullet collision
				for (u32 j=0; j<EnemyBullets.size(); ++j)
					if (PlayerBullets[i]->checkCollision(EnemyBullets[j]))
					{
						collision=true;
						EnemyBullets[j]->drop();
						EnemyBullets.erase(j);
					}

				// powerup collision
				if (PowerUp && PowerUp->checkCollision(PlayerBullets[i]))
				{
					PowerUp->hit(PlayerBullets[i]);
					PowerUp->drop();
					PowerUp=0;
					collision=true;
				}

				// ufo collision
				if (UFO && UFO->checkCollision(PlayerBullets[i]))
				{
					ConsecutiveHits++;
					setScore(Score + 100 + (rand() % 200));
					collision=true;
					if (UFO->hit(PlayerBullets[i]))
					{
						NextUFOTime = GameTime + 20000 + (rand() % 20000);

						// powerup...
						if (!PowerUp)
							PowerUp = new CPowerUp(Canvas, Textures[EGT_POWERUP], UFO->Position, rand() % EGPU_COUNT);

						UFO->drop();
						UFO=0;
					}

				}

				// drop this bullet if it ran out of energy or went off the screen

				if (collision || PlayerBullets[i]->Position.LowerRightCorner.Y < 0)
				{
					if (!collision)
						ConsecutiveHits=0;

					PlayerBullets[i]->drop();
					PlayerBullets.erase(i);
				}
			}

			for (u32 i=0; i<EnemyBullets.size(); ++i)
			{
				bool collision = false;

				EnemyBullets[i]->move(0,EnemyBullets[i]->Speed*t);

				// check base collision
				for (u32 j=0; j<Bases.size(); ++j)
					if (Bases[j]->checkCollision(EnemyBullets[i]))
					{
						// base destroyed?
						if(Bases[j]->hit(EnemyBullets[i]))
						{
							Bases[j]->drop();
							Bases.erase(j--);
						}
						collision=true;
						break;
					}

				// player collision

				if (PlayerShip && EnemyBullets[i]->checkCollision(PlayerShip))
				{
					// in case we have armor or something
					if (PlayerShip->hit(EnemyBullets[i]))
					{
						// explode player
						dead = true;
						collision = true;
					}

				}


				if (collision || (PlayerShip && EnemyBullets[i]->Position.UpperLeftCorner.Y > PlayerShip->Position.LowerRightCorner.Y))
				{
					EnemyBullets[i]->drop();
					EnemyBullets.erase(i);
				}

			}

			// invader movement / drop bombs
			if (Invaders)
			{
				// movement

				switch (Invaders->Direction)
				{
				case 0:
					Invaders->move(InvaderSpeed*t,0);
					if (Invaders->Position.LowerRightCorner.X > GameWidth-10)
					{
						Invaders->Direction++;
						Invaders->StopY = Invaders->Position.LowerRightCorner.Y + InvaderStep;
					}
					break;
				case 1:
				case 3:
					Invaders->move(0,InvaderSpeed*t*5);
					if (Invaders->Position.LowerRightCorner.Y > Invaders->StopY)
					{
						Invaders->Direction++;
						InvaderSpeed += 0.0005;// + (Level/300.0f);
					}
					break;
				case 2:
					Invaders->move(-InvaderSpeed*t,0);
					if (Invaders->Position.UpperLeftCorner.X < 10)
					{
						Invaders->Direction=3;
						Invaders->StopY = Invaders->Position.LowerRightCorner.Y + InvaderStep;
					}
					break;
				}
				if (Invaders->Direction==4)
					Invaders->Direction=0;

				// bombs
				if (EnemyBullets.size() < MaxEnemyBullets) // && (rand() % ) // frequency
				{
					// shoot!
					position2df p = Invaders->Invaders[ rand() % Invaders->Invaders.size()]->Position.getCenter();
					EnemyBullets.push_back(
						new CBullet(Canvas, Textures[EGT_BULLET], EnemyBulletSize +
							position2df(p.X - EnemyBulletSize.getWidth()/2.f,p.Y),
							EnemyBulletSpeed));
				}
			}


			// check invader/base collision
			if (Invaders)
				for (u32 i=0; i<Bases.size(); ++i)
					if (Bases[i]->checkCollision(Invaders))
					{
						makeBases(); // reset bases
						break;
					}

			// UFO
			if (UFO)
			{
				UFO->move(0.06f*t,0);
				if (UFO->Position.LowerRightCorner.X > GameWidth)
				{
					UFO->drop();
					UFO = 0;
					NextUFOTime = GameTime + 20000 + (rand() %20000);
				}
			}
			else
			{
				if (GameTime > NextUFOTime)
				{
					UFO = new CUFO(Canvas, Textures[EGT_UFO],rect<f32>(0,-100,20,0)); //, rand() % 1 ? true : false);
				}

			}

			// power ups
			if (PowerUp && PlayerShip)
			{
				PowerUp->move(0,0.1f*t);
				if (PowerUp->Position.LowerRightCorner.Y > PlayerShip->Position.LowerRightCorner.Y)
				{
					PowerUp->drop();
					PowerUp = 0;
				}
				else
				{
					if (PowerUp->checkCollision(PlayerShip))
					{
						E_GAME_POWERUP p = (E_GAME_POWERUP)PowerUp->StartSprite;

						switch (p)
						{
						case EGPU_10000PTS:
							setScore(Score+10000);
							break;
						case EGPU_1000PTS:
							setScore(Score+1000);
							break;
						case EGPU_100PTS:
							setScore(Score+100);
							break;
						case EGPU_1UP:
							setLives(Lives+1);
							break;
						case EGPU_BACKOFF:
							if (Invaders)
								Invaders->move(0,-100);
							break;
						case EGPU_DOUBLEWIDTH:
							PlayerShip->Position.LowerRightCorner.X += PlayerShip->Position.getWidth();
							break;
						case EGPU_ENEMY_BEEFCAKE:
							if (Invaders)
								Invaders->setBeefCake(Textures[EGT_BEEFCAKE]);
							break;
						case EGPU_EXTRA_BULLET:
							MaxPlayerBullets++;
							break;
						case EGPU_EXTRA_ENEMY_BULLET:
							MaxEnemyBullets++;
							break;
						case EGPU_HALFWIDTH:
							PlayerShip->Position.LowerRightCorner.X -= PlayerShip->Position.getWidth()/2.0f;
							break;
						case EGPU_RESET_BASES:
							makeBases(4 + rand() % 4, 1.0f/12.0f); // extra bases!
							break;
						case EGPU_SHIELD:
							PlayerShip->Hits++;
							break;
						case EGPU_SLOWDOWN:
							PlayerSpeed-= 0.01f;
							break;
						case EGPU_SPEEDUP:
							PlayerSpeed+= 0.01f;
							break;
						case EGPU_WIDEBULLETS:
							PlayerBulletSize.LowerRightCorner.X+=PlayerBulletSize.getWidth();
							break;
						case EGPU_WIDEENEMYBULLETS:
							EnemyBulletSize.LowerRightCorner.X+=EnemyBulletSize.getWidth();
							break;
						}

						PowerUp->drop();
						PowerUp = 0;
					}
				}
			}




			// rotate canvas
			if (PlayerShip)
				Canvas->setRotation(CanvasRotation + vector3df(0,0,(PlayerShip->Position.getCenter().X/GameWidth))*360.f);


			// player won?
			if (!Invaders)
				setLevel(Level+1);

			if (Invaders && PlayerShip && Invaders->Position.LowerRightCorner.Y > PlayerShip->Position.UpperLeftCorner.Y)
				dead = true;

			// player lost?
			if (dead)
			{
				if (Lives)
				{
					makeBases(4, 1.0f/12.0f);
					setLives(--Lives);
					setLevel(Level);
				}
				else
					gameOver();

			}

		}

		void gameOver()
		{
			clearGame();
			Mode = EGM_MENU;
			MenuMode = 0;
			setMenuMode();
		}

		void menuLoop()
		{
			// process menu screen...
			if (GameTime > MenuStartTime + 20000)
			{
				// change menu mode
				MenuStartTime = GameTime + 20000;
				if (MenuMode++ > 2)
					MenuMode = 0;
				setMenuMode();
			}
			// start game
			if (Events.keys[KEY_KEY_S])
			{
				Mode = EGM_PLAY;
				setMenuMode();
				resetGame();
			}

		}
		void setMenuMode()
		{
			//MenuText1->setVisible(true);

			// hide everything
			MenuText1->setVisible(false);
			//MenuText2->setVisible(false);
			//MenuText3->setVisible(false);

			if (Mode == EGM_MENU)
				switch(MenuMode)
				{
				case 0:
					// show options
					MenuText1->setVisible(true);
					break;/*
				case 1:
					// show scoring
					MenuText2->setVisible(false);
					break;
				case 2:
					// show credits
					MenuText3->setVisible(false);
					break;*/
				}
		}
		void setGameSize(f32 width, f32 height)
		{
			GameWidth = width;
			GameHeight = height;
			if (Canvas)
			{
				Canvas->setSize(width, height);
			}
			// move bases
			// move player
			// clear bullets
		}

		void resetGame()
		{
			// clear game
			clearGame();

			// set score and lives
			setScore(0);
			setLives(StartLives);
			setGameSize(600, 700);

			PlayerWidth = 50.f;
			PlayerHeight = 20.f;
			PlayerSpeed = 0.2f;
			PlayerBulletSize = rect<f32>(0,0,5,30);
			EnemyBulletSize =  rect<f32>(0,0,5,30);
			EnemyBulletSpeed = 0.5f;
			PlayerBulletSpeed = 1.5f;
			MaxPlayerBullets = 1;
			MaxEnemyBullets = 1;
			NextPlayerFireTime=0;
			PlayerReloadTime=200;

			PlayerShip = new CPlayer(Canvas, Textures[EGT_PLAYER],
				rect<f32>(GameWidth/2.0f - PlayerWidth/2.0f, PlayerY,
						  GameWidth/2.0f + PlayerWidth/2.0f, PlayerY+PlayerHeight));

			// reset bases
			makeBases(4, 1.0f/12.0f);
			MaxPlayerBullets =1;

			// start level
			setLevel(1);
		}

		void clearGame()
		{
			// invaders
			if (Invaders)
				Invaders->drop();
			Invaders=0;
			// player
			if (PlayerShip)
				PlayerShip->drop();
			PlayerShip=0;
			// UFO
			if (UFO)
				UFO->drop();
			UFO=0;
			// PowerUps
			if (PowerUp)
				PowerUp->drop();
			PowerUp=0;

			// bases
			makeBases(0);

			// bullets
			for (u32 i=0; i < PlayerBullets.size(); ++i)
				PlayerBullets[i]->drop();
			PlayerBullets.clear();
			for (u32 i=0; i < EnemyBullets.size(); ++i)
				EnemyBullets[i]->drop();
			EnemyBullets.clear();
		}

		void setLevel(s32 l=1)
		{
			Level=l;

			if (Invaders)
				Invaders->drop();

			Invaders = new CInvaders(Canvas, Textures[EGT_INVADER],
									rect<f32>(0, 0, GameWidth*0.6, GameHeight*0.6),
									10,5);

			InvaderSpeed = (1.0f + Level) * 0.005;
			InvaderStep = 50; //+ (Level*2);
			MaxEnemyBullets = 1 + (Level / 5);
			ConsecutiveHits = 0;

			NextUFOTime = GameTime + 20000 + (rand() % 20000);

			// clear bullets
			for (u32 i=0; i < PlayerBullets.size(); ++i)
				PlayerBullets[i]->drop();
			PlayerBullets.clear();
			for (u32 i=0; i < EnemyBullets.size(); ++i)
				EnemyBullets[i]->drop();
			EnemyBullets.clear();

		}

		IrrlichtDevice *device;
		IVideoDriver* driver;
		ISceneManager* smgr;
		IGUIEnvironment* guienv;
		s32 Lives, Score, HiScore;
		s32 StartLives;
		f32 GameWidth, GameHeight;
		f32 PlayerY, BaseY, PlayerSpeed, PlayerWidth, PlayerHeight;
		vector3df CanvasRotation;

		u32 GameTime, LastGameTime, MenuStartTime, MenuMode;

		IGUIStaticText *GUIScore, *GUIHiScore, *GUILives, *MenuText1, *MenuText2, *MenuText3;
		CircularSpriteCanvasSceneNode *Canvas;

		MyEvents Events;

		E_GAME_MODE Mode;

		array<CBullet*> PlayerBullets;
		array<CBullet*> EnemyBullets;
		array<CBase*>	Bases;
		CPlayer*	PlayerShip;
		CInvaders*	Invaders;
		CUFO*		UFO;
		CPowerUp*	PowerUp;
		array<ITexture*> Textures;

		rect<f32> PlayerBulletSize, EnemyBulletSize;

		f32 PlayerBulletSpeed, EnemyBulletSpeed, InvaderSpeed, InvaderStep;
		u32 MaxPlayerBullets, MaxEnemyBullets, NextPlayerFireTime, NextUFOTime,
			PlayerReloadTime, Level, ConsecutiveHits;

		bool QuitGame;

	};

} // namespace game
} // namespace irr

#endif // __IRRVADERS_H__
