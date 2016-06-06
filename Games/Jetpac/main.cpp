
#include <irrlicht.h>
#include "Jetpac.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;



	// business as usual
	IrrlichtDevice *device;
	ISceneManager *smgr;
	IVideoDriver *driver;
	IGUIEnvironment *env;

	// game state stuff
	s32 Lives, Score, HiScore, Level;
	u32 NextFuelTime, NextBonusTime, NextEnemyTime,
		NextMenuTime, NextShootTime, NextFlashTime, NextExtraLife;

	// game resources
	IMesh *rocketBaseMesh, *rocketBodyMesh, *rocketConeMesh, *fuelMesh, *platformMesh;
	array<ITexture*> BadGuyTextures;
	array<ITexture*> ExplosionTextures;
	array<ITexture*> PlayerTextures;
	array<IMesh*> BonusMeshes;
	IGUIFont *font;

	// scene stuff
	ICameraSceneNode *Camera;
	Player *player;
	Rocket *ship;
	array<Ledge*> platforms;
	array<Enemy*> enemies;
	array<GameEntity*> collectables;
	array<PhaserFire*> bullets;

	// gui
	IGUIStaticText *ScoreText, *HiScoreText, *LivesText, *GameOverText, *MenuText;

	STATES State;

	void setState(STATES state)
	{
		State = state;
		MenuText->setVisible(State == STATE_MENU);
		GameOverText->setVisible(State == STATE_GAMEOVER);

		switch(state)
		{
		case STATE_PAUSED:
			{
			device->getTimer()->stop();
			break;
			}
		case STATE_PLAYING:
		{
			// remove enemies
			for (u32 i=0; i<enemies.size(); ++i)
			{
				enemies[i]->remove();
				enemies[i]->drop();
			}
			enemies.clear();
			// respawn player
			player->reset();
			NextShootTime = 0;
			NextFlashTime = 0;
			break;
		}
		case STATE_LAUNCH:
		{
			ship->setStage(10); // we have ignition!
			player->setVisible(false);
			player->IsAlive = false;
			break;
		}
		default:
			break;

		}
	}



	void setLives(s32 l)
	{
		Lives = l;
		stringw txt = L"";
		if (Lives > 4)
		{
//			txt == Lives;
			txt += L"x ~";
		}
		else
		{
			for (s32 i=0; i<Lives; ++i)
				txt += L"~ ";
		}
		LivesText->setText(txt.c_str());
	}

	void setHiScore(s32 newScore)
	{
		HiScore = newScore;

		stringw txt = L"High: ";
		txt += HiScore;
		HiScoreText->setText(txt.c_str());
	}
	void setScore(s32 newScore)
	{
		if (!newScore)
			NextExtraLife = 1;

		Score = newScore;
		if (Score > HiScore)
			setHiScore(Score);

		stringw txt = L"Score: ";
		txt += Score;
		if ((u32)Score > NextExtraLife*20000)
		{
			NextExtraLife ++;
			setLives(Lives+1);
		}
		ScoreText->setText(txt.c_str());
	}

	void spawnThings()
	{
		u32 t = device->getTimer()->getTime();

		if (State == STATE_LOST_LIFE && t > NextMenuTime)
		{
			if (Lives > 0)
				setState(STATE_PLAYING);
			else
			{
				setState(STATE_GAMEOVER);
				NextMenuTime = t + 3000;
			}

		}

		if (NextFuelTime && t > NextFuelTime)
		{
			// spawn fuel
			NextFuelTime = 0;
			f32 p = (rand() % (s32)(MaxX*2-2)) - MaxX + 1;
			collectables.push_back( new RocketPart(smgr, fuelMesh, ship->Stage, vector3df(p, MaxY+3, 0)));
		}

		if (t > NextEnemyTime)
		{
			NextEnemyTime = t + rand() % (1000 - 900 / ( Level > 19 ? 1 : 20-Level ));
			if (enemies.size() < (u32)(MaxEnemies+Level))
			{
				// spawn new enemy

				ZXCOLOUR c;
				u32 p = rand() % 4;
				switch (p)
				{
				case 0: c = Red0; break;
				case 1: c = Cyan0; break;
				case 2: c = Magenta0; break;
				case 3: c = Green0; break;
				}
				f32 height = (rand() % (s32)MaxY) + 2.f;
				vector3df pos(-MaxX,height,0);

				enemies.push_back( new Enemy(smgr, BadGuyTextures[(Level-1) % BadGuyTextures.size()],
					pos, c, (Level-1) % BadGuyTextures.size()) );
			}
		}

		if (NextBonusTime && t > NextBonusTime)
		{
			// spawn new bonus
			u32 no = rand() % BonusMeshes.size();
			ZXCOLOUR c;
			switch (no)
			{
			case 0: c = Cyan0; break; // triyarble
			case 1: c = Yellow1; break; // gold bar
			case 2: c = White0; break; // diamond
			//case 3: c = White0; break; // pearl
			}
			f32 p = (rand() % (s32)(MaxX*2-2)) - MaxX + 1;
			collectables.push_back( new Bonus(smgr, BonusMeshes[no], 1000*(no+1), c, vector3df(p, MaxY+3, 0)));
			NextBonusTime = 0;
		}
	}

	bool startLevel()
	{
		u32 t = device->getTimer()->getTime();
		// clear enemies
		u32 i;
		for (i=0; i<enemies.size(); ++i)
		{
			enemies[i]->remove();
			enemies[i]->drop();
		}
		enemies.clear();

		// clear pickups
		for (i=0; i<collectables.size(); ++i)
		{
			collectables[i]->remove();
			collectables[i]->drop();
		}
		collectables.clear();

		// launch enemies!!
		NextEnemyTime = t + rand() % 1000;
		NextBonusTime = t + rand() % 60000;

		// reset ship. every 4th level you have to start again
		if (Level%4 == 1)
		{
			ship->setStage(1);
			RocketPart* rp = new RocketPart(smgr, rocketBodyMesh, 1, vector3df(0, MaxY, 0));
			collectables.push_back( rp );
			rp = new RocketPart(smgr, rocketConeMesh, 2, vector3df(-MaxX*0.5f, MaxY, 0));
			collectables.push_back( rp );

			ship->Velocity.Y = 0.0f;
			vector3df p = ship->getPosition();
			p.Y = ship->OrigY;
			ship->setPosition(p);
			ship->updateAbsolutePosition();

			NextFuelTime = 0;
			return true;
		}
		else
		{
			ship->setStage(3);
			NextFuelTime = t + rand() % 20000;
			return false;
		}

	}

	void logicAndCollisions()
	{

		u32 now = device->getTimer()->getTime();
		s32 i, j;
		aabbox3df b = player->getTransformedBoundingBox();
		vector3df p = player->getPosition();
		bool collision =false;

		// dead players can't do any of this-
		if (player->IsAlive)
		{

			// shoot
			if (player->Shooting && now > NextShootTime)
			{
				for (u32 i=0; i<bullets.size(); ++i)
				{
					if (bullets[i]->Finished)
					{
						bullets[i]->shoot(player->getPosition() + vector3df(0,-0.04f,0), player->LeftFacing ? -1 : 1, 350);
						NextShootTime = now + TimeBetweenShots;
						break;
					}
				}
			}

			// collide player with world
			for (i=0; i<platforms.size(); ++i)
			{
				aabbox3df b2 = platforms[i]->getTransformedBoundingBox();

				if (b.intersectsWithBox(b2))
				{
					collision = true;

					if (b.MinEdge.Y > platforms[i]->getPosition().Y)
					{
						if (p.X > b2.MinEdge.X && p.X < b2.MaxEdge.X)
						{
							// landed
							player->Resting = true;
						}
						else
						{
							player->Resting = false;
						}
						p.Y += b2.MaxEdge.Y - b.MinEdge.Y;
						player->Velocity.Y = 0;
						player->setPosition(p);
					}
					else
					{
						player->Resting = false;
						if (p.X > b2.MinEdge.X && p.X < b2.MaxEdge.X)
						{
							// roof
							player->Velocity.Y *= -0.3f;
						}
						else
						{
							// wall
							if (p.X > b2.MaxEdge.X)
							{
								p.X += b2.MaxEdge.X - b.MinEdge.X + 0.02f;
								player->Velocity.X = 0.002f;
							}
							else
							{
								p.X += b2.MinEdge.X - b.MaxEdge.X - 0.02f;
								player->Velocity.X = -0.002f;
							}
							player->setPosition(p);
						}
					}
				}
			}

			if (!collision && player->Resting)
				player->Resting = false;

			// player and pickups
			for (j=0; j<collectables.size(); ++j)
			{
				if (!collectables[j]->IsCarryable || (!collectables[j]->LevelUp && !player->PickUp && (ship->Stage == collectables[j]->PickUpStage || collectables[j]->PickUpStage > 9) ))
				{
					aabbox3df b2 = collectables[j]->getTransformedBoundingBox();
					if (b.intersectsWithBox(b2))
					{
						if (collectables[j]->IsCarryable)
						{
							collectables[j]->Resting = true;
							collectables[j]->LevelUp = true; // for when it gets dropped
							player->setPickUp(collectables[j]);
						}
						else
						{
							setScore(Score + collectables[j]->Score);
							collectables[j]->remove();
							collectables[j]->drop();
							collectables.erase(j--);
							NextBonusTime = now + 30000 + rand() % 30000;
						}
					}
				}
			}

			// player dropping pickup
			vector3df p2 = ship->getPosition();
			p2.Y = p.Y;
			if (player->PickUp && b.isPointInside(p2))
			{
				// drop pickup
				player->PickUp->setPosition(p2);
				player->PickUp->Resting = false;
				player->setPickUp(0);
			}

			// player entering spaceship
			if (ship->Stage == 9 && player->Resting && b.isPointInside(ship->getPosition()))
			{
				setState(STATE_LAUNCH);
			}

		} // <- dead players can't do any of that

		// enemies

		for (i=0; i<enemies.size(); ++i)
		{
			b = enemies[i]->getTransformedBoundingBox();
			p = enemies[i]->getAbsolutePosition();
			s32 currenti=i;
			aabbox3df b2;


			// enemy ai
			if (enemies[i]->AI == 2)
			{
				if (!enemies[i]->NextTime || now > enemies[i]->NextTime)
				{
					enemies[i]->NextTime = now + rand() % 1000 + 500;
					// direction change

					if (enemies[i]->Velocity.Y < 0 || enemies[i]->Velocity.Y > 0)
					{
						enemies[i]->Velocity.Y = 0;
					}
					else
					{
						f32 dir = player->getAbsolutePosition().Y < p.Y ? -1.f : 1.f;
						enemies[i]->Velocity.Y = 0.0015f*dir;
					}
				}
			}
			else if (enemies[i]->AI == 4)
			{
				if (!enemies[i]->NextTime || now > enemies[i]->NextTime)
				{
					enemies[i]->NextTime = now + rand() % + 500;
					// direction change
					core::vector3df pp = player->getAbsolutePosition() - p;
					pp.setLength(0.002f);
					enemies[i]->Velocity += pp;
					enemies[i]->Velocity.setLength(0.003f);
				}
			}


			// enemies going too far out of bounds
			if (b.MaxEdge.Y > MaxY)
			{
				if (enemies[i]->AI == 0)
				{
					// remove it
					enemies[i]->drop();
					enemies[i]->remove();
					enemies.erase(i--);
					continue;
				}
				else
				{
					// change direction
					enemies[i]->Velocity.Y = -fabs(enemies[i]->Velocity.Y);
				}
			}

			if (player->IsAlive)
			{
				// enemies and player
				b2 = player->getTransformedBoundingBox();
				if (b.intersectsWithBox(b2))
				{
					Explosion *e = new Explosion(smgr, ExplosionTextures, b.getCenter());
					e->drop();
					enemies[i]->drop();
					enemies[i]->remove();
					enemies.erase(i--);

					e = new Explosion(smgr, ExplosionTextures, b2.getCenter());
					e->drop();

					setLives(Lives-1);
					player->setVisible(false);
					player->IsAlive = false;
					if (player->PickUp)
					{
						player->PickUp->LevelUp = false;
						player->PickUp->Resting = false;
						player->setPickUp(0);
					}

					NextEnemyTime = now + 10000;
					NextMenuTime = now + 5000;

					State = STATE_LOST_LIFE;
					return;
				}
			}

			// enemies and scenery
			for (j=0; j < platforms.size(); ++j)
			{

				b2 = platforms[j]->getTransformedBoundingBox();

				if (b.intersectsWithBox(b2))
				{
					if (enemies[i]->IsFragile)
					{
						// destroy enemy
						Explosion *e = new Explosion(smgr, ExplosionTextures, b.getCenter(), enemies[i]->Colour);
						e->drop();
						enemies[i]->drop();
						enemies[i]->remove();
						enemies.erase(i--);
						break;
					}
					else
					{
						// change direction
						if ((p.X > b2.MaxEdge.X && enemies[i]->Velocity.X < 0) ||
						    (p.X < b2.MinEdge.X && enemies[i]->Velocity.X > 0))
						{
							if (enemies[i]->AI == 3)
							{
								// these boys dont like head on collisions
								Explosion *e = new Explosion(smgr, ExplosionTextures, b.getCenter(), enemies[i]->Colour);
								e->drop();
								enemies[i]->drop();
								enemies[i]->remove();
								enemies.erase(i--);
							}
							else
								enemies[i]->Velocity.X *=-1.f;
						}
						else
						if (b.MaxEdge.Y > b2.MinEdge.Y && b.MinEdge.Y < b2.MaxEdge.Y)
						{
							if (enemies[i]->AI == 2)
								enemies[i]->Velocity.Y = 0;
							else
								enemies[i]->Velocity.Y *=-1.f;


						}
					}
				}
			}

			if (i != currenti)
				continue;

			// enemies and bullets
			for (j=0; j < bullets.size(); ++j)
			{
				if (!bullets[j]->Finished)
				{
					b2 = bullets[j]->getBoundingBox();
					bool destroy = false;

					if (b.intersectsWithBox(b2))
						destroy=true;

					aabbox3df b3;

					if (!destroy && b2.MaxEdge.X > MaxX)
					{
						b3 = b2;
						b3.MaxEdge.X -= MaxX*2;
						b3.MinEdge.X -= MaxX*2;

						if (b.intersectsWithBox(b3))
							destroy=true;
					}
					if (!destroy && b2.MinEdge.X < -MaxX)
					{
						b3 = b2;
						b3.MaxEdge.X += MaxX*2;
						b3.MinEdge.X += MaxX*2;
						if (b.intersectsWithBox(b3))
							destroy=true;
					}

					if (destroy)
					{
						setScore( Score + enemies[i]->Score );
						// destroy enemy
						Explosion *e = new Explosion(smgr, ExplosionTextures, b.getCenter(), enemies[i]->Colour);
						e->drop();
						enemies[i]->drop();
						enemies[i]->remove();
						enemies.erase(i--);
						break;
					}
				}
			}
		}

		// todo: weapons and scenery?

		// pickups and scenery

		for (j=0; j<collectables.size(); ++j)
		{
			if (collectables[j]->Resting)
				continue;

			b = collectables[j]->getTransformedBoundingBox();

			for (i=0; i<platforms.size(); ++i)
			{
				aabbox3df b2 = platforms[i]->getTransformedBoundingBox();

				if (b.intersectsWithBox(b2))
				{
					collectables[j]->Resting = true;
					if (collectables[j]->LevelUp)
					{
						ship->setStage(ship->Stage +1);
						if (ship->Stage > 2 && ship->Stage < 9)
						{
							NextFuelTime = device->getTimer()->getTime() + rand() % 20000;
						}

						setScore(Score + collectables[j]->Score);

						collectables[j]->drop();
						collectables[j]->remove();
						collectables.erase(j--);

					}
					break;
				}
			}
		} // pickups + scenery

		// ship flashing
		if (ship->Stage == 9 && now > NextFlashTime )
		{
			ship->setStage(9);
			NextFlashTime = now + 400;
		}
		if (State == STATE_LAUNCH)
		{
			if (ship->Stage == 10) // up
			{
				if (ship->getTransformedBoundingBox().MinEdge.Y > MaxY+2)
				{
					Level = Level + 1;
					setScore(Score + 2000);
					if (!startLevel())
					{
						ship->setStage(11);
					}
					else
					{
						setState(STATE_PLAYING);
					}
				}
			}
			else if (ship->Stage == 11) // down
			{
				if (ship->getTransformedBoundingBox().MinEdge.Y <
					platforms[0]->getTransformedBoundingBox().MaxEdge.Y)
				{
					ship->setStage(3);
					ship->Velocity.Y = 0.0f;
					setState(STATE_PLAYING);
				}
			}
		}
		// game over to menu
		if (State == STATE_GAMEOVER && now > NextMenuTime)
		{
			setState(STATE_MENU);
		}
	}

	void newGame()
	{
		setScore(0);
		setLives(3);
		Level = 1;
		startLevel();

		setState(STATE_PLAYING);
	}
class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& e)
	{
		// process keyboard input

		if (e.EventType == EET_KEY_INPUT_EVENT)
		{

			switch(State)
			{
			case STATE_PAUSED:

			if ( e.KeyInput.Key == KEY_ESCAPE)
					{
                           State = STATE_QUIT;
					}


				if (!e.KeyInput.PressedDown)
				{
					State =STATE_PLAYING;
					device->getTimer()->start();
					return true;
				}
				device->sleep(50,1);
				break;
			case STATE_MENU:
				if (!e.KeyInput.PressedDown && e.KeyInput.Key == KEY_LSHIFT)
				{
					newGame();
					return true;
				}
				break;
			case STATE_PLAYING:
				if (player->IsAlive)
				{
					if (!e.KeyInput.PressedDown && e.KeyInput.Key == KEY_ESCAPE)
					{
						setState(STATE_PAUSED);
						return true;
					}
					else
					if (player->OnEvent(e))
						return true;
				}
				break;
			case STATE_GAMEOVER:
				if (!e.KeyInput.PressedDown)
				{
					setState(STATE_MENU);
					return true;
				}
				break;
			default:
				return true;
				break;
			}
		}

		return false;
	}
};







	void loadResources()
	{
		// add archive
		device->getFileSystem()->addZipFileArchive("media.zip");
//		device->getFileSystem->changeWorkingDirectoryTo("/media") ;

		// load font
		font = env->getFont("arialb.xml");
		env->getSkin()->setFont(font);

		// load platform
		platformMesh = smgr->getMesh("platform.obj")->getMesh(0);

		// load player
		PlayerTextures.push_back(driver->getTexture("jetpac.png"));

		// load rocket
		rocketBaseMesh = smgr->getMesh("rocket_base2.obj")->getMesh(0);
		rocketBodyMesh = smgr->getMesh("rocket_body2.obj")->getMesh(0);
		rocketConeMesh = smgr->getMesh("rocket_cone2.obj")->getMesh(0);

		// load fuel
		fuelMesh = smgr->getMesh("fuel.obj")->getMesh(0);
		billboardMesh = smgr->getMesh("billboard.obj")->getMesh(0);

		// load bad guys
		BadGuyTextures.push_back(driver->getTexture("comet.png"));
		BadGuyTextures.push_back(driver->getTexture("fuzzy.png"));
		BadGuyTextures.push_back(driver->getTexture("bubble.png"));
		BadGuyTextures.push_back(driver->getTexture("jet.png"));
		BadGuyTextures.push_back(driver->getTexture("ufo.png"));
		//BadGuyTextures.push_back(driver->getTexture("cross.png"));
		//BadGuyTextures.push_back(driver->getTexture("frog.png"));

		BonusMeshes.push_back( smgr->getMesh("bonus1.obj")->getMesh(0)); // yarble
		BonusMeshes.push_back( smgr->getMesh("bonus2.obj")->getMesh(0)); // gold
		BonusMeshes.push_back( smgr->getMesh("bonus3.obj")->getMesh(0)); // diamond
		BonusMeshes[BonusMeshes.size()-1]->getMeshBuffer(0)->getMaterial().MaterialType = EMT_TRANSPARENT_ADD_COLOR;
		//BonusMeshes.push_back( smgr->getMesh("bonus4.obj")->getMesh(0)); // pearl

		// load explosion
		ExplosionTextures.push_back(driver->getTexture("explosion0.png"));
		ExplosionTextures.push_back(driver->getTexture("explosion1.png"));
		ExplosionTextures.push_back(driver->getTexture("explosion2.png"));
		ExplosionTextures.push_back(driver->getTexture("explosion3.png"));
	}




int main(){


		unsigned int w=800, h=600;
		device =
			createDevice( video::EDT_OPENGL, dimension2du(w, h), 32,
				false, false, false, 0);

		device->setWindowCaption(L"JetPac");

		driver = device->getVideoDriver();
		smgr = device->getSceneManager();
		env = device->getGUIEnvironment();

        MyEventReceiver receiver;
		device->setEventReceiver(&receiver);
		smgr->setAmbientLight(SColorf(1.0, 1.0, 1.0, 1.0));

		loadResources();
		// create stuff
		Camera = smgr->addCameraSceneNode(0, vector3df(0,MaxY/2,-5), vector3df(0,MaxY/2,0));
		player = new Player(smgr, *PlayerTextures.pointer());
		player->setPosition(vector3df(0,MaxY*0.25f,0));

		// floor
        platforms.push_back( new Ledge(smgr, MaxX*2.0f, vector3df(0,0,0), Yellow0, platformMesh));
		// platforms
		platforms.push_back( new Ledge(smgr, MaxX*0.35f, vector3df(MaxX*0.65f,MaxY*0.8f,0), Green0, platformMesh));
		platforms.push_back( new Ledge(smgr, MaxX*0.25f, vector3df(0,MaxY/2.0f,0), Green0, platformMesh));
		platforms.push_back( new Ledge(smgr, MaxX*0.35f, vector3df(-MaxX*0.55f,MaxY*0.7f,0), Green0, platformMesh));

		// ship
		ship = new Rocket(smgr, rocketBaseMesh, rocketBodyMesh, rocketConeMesh, driver);

		// bullets
		for (u32 i=0; i<MaxBullets; ++i)
			bullets.push_back( new PhaserFire(smgr));

		// create GUI
		s32 q = w/3;
		ScoreText   = env->addStaticText(L"Score: 0", rect<s32>(0,0,q,80));
		ScoreText->setOverrideColor(Cyan0);
		ScoreText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
		LivesText = env->addStaticText(L"3~", rect<s32>(q,0,q*2,80));
		LivesText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
		LivesText->setOverrideColor(White0);
		HiScoreText = env->addStaticText(L"HiScore: 0", rect<s32>(q*2,0,q*3,80));
		HiScoreText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
		HiScoreText->setOverrideColor(Cyan0);
		MenuText = env->addStaticText(L"Keys:\n Thrust/Walk: Up, Left, Right\nFire: Shift\n\nPress fire to start!", rect<s32>(0,0,w,h));
		MenuText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
		MenuText->setOverrideColor(White0);
		GameOverText = env->addStaticText(L"GAME OVER", rect<s32>(0,0,w,h));
		GameOverText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
		GameOverText->setOverrideColor(White1);
		GameOverText->setVisible(false);
		setHiScore(0);
		setScore(0);
		setLives(3);

		setState(STATE_MENU);

		u32 now = device->getTimer()->getTime();

		while(State != STATE_QUIT)
		{
			//bool active = device->isWindowActive();
			//if (State == STATE_PAUSED)
			//setState(STATE_PAUSED);

			//if (active)
			//{
				if (!device->run())
				{
					setState(STATE_QUIT);
				}
				else
				{
					driver->beginScene(true, true, SColor(0,0,0,0));
					if (State != STATE_MENU)
						smgr->drawAll();

					env->drawAll();
					driver->endScene();
					if (State != STATE_MENU)
					{
						logicAndCollisions();
						spawnThings();
					}
					device->yield();
				}
				device->sleep(1,0);
			//}
		}




			player->drop();
		u32 i;
		for (i=0; i<platforms.size(); ++i)
			platforms[i]->drop();

		for (i=0; i<enemies.size(); ++i)
			enemies[i]->drop();

		for (i=0; i<collectables.size(); ++i)
			collectables[i]->drop();

		ship->drop();

		device->drop();
};
