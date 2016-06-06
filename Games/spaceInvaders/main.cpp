
// Written by Michael Zeilfelder (aka CuteAlien)
// This software is using the zlib license

#include <irrlicht.h>
#include <iostream>

#ifdef __GNUWIN32__
#undef swprintf
#define   swprintf   _snwprintf
#endif  // __GNUWIN32__

using namespace irr;

const s32 INVADER_ROWS = 16;
const s32 INVADER_LINES = 5;
const s32 MAX_SHOTS_INVADERS = 3;
const s32 MAX_SHOTS_PLAYER = 3;
const float ROOM_RADIUS = 1200.f;
const float ROOM_BOTTOM = -800.f;
const float UPPER_LINE = 300.f;
const float LINE_OFFSET = 120.f;
const float LEFT_ROW = -900.f;
const float ROW_OFFSET = 120.f;

video::IVideoDriver* driver = 0;
scene::ISceneManager* sceneManager = 0;
gui::IGUIEnvironment* env = 0;

// The floor is drawn by a number of vertices. Those are extruded in direction of extrude_.
// Will work *only* for convex base areas
scene::SMeshBuffer* CreateExtrudedPolyLineMeshBuffer(s32 numVertices_, const core::vector3df * vertices_, const core::vector3df &extrude_)
{
    if ( numVertices_ < 3 || !vertices_ )
        return NULL;

   video::SMaterial material;
   material.DiffuseColor = video::SColor(200, 255, 200, 255);
    material.AmbientColor = video::SColor(255, 255, 255, 255);
   material.EmissiveColor = video::SColor(255, 255, 255, 255);
    material.MaterialType = video::EMT_SOLID;
   material.Lighting = true;
   material.BackfaceCulling = false;
   // material.Wireframe = true;   // hm, can't decide if it's nicer with or without wireframe :)

    scene::SMeshBuffer * buffer = new scene::SMeshBuffer();
    buffer->Material = material;

    // sides
    for ( s32 i=0; i<numVertices_; ++i)
    {
        video::S3DVertex v;
        v.Pos = vertices_[i];
        buffer->Vertices.push_back(v);
        v.Pos = vertices_[i] + extrude_;
        buffer->Vertices.push_back(v);

        buffer->Indices.push_back( ((i+1)*2+1) % (numVertices_*2) );
        buffer->Indices.push_back( ((i+1)*2) % (numVertices_*2) );
        buffer->Indices.push_back(i*2);

        buffer->Indices.push_back(i*2);
        buffer->Indices.push_back(i*2+1);
        buffer->Indices.push_back( ((i+1)*2+1) % (numVertices_*2) );
    }


    for ( s32 i=1; i<numVertices_-1; ++i)
    {
        buffer->Indices.push_back(0);
        buffer->Indices.push_back(i*2);
        buffer->Indices.push_back(i*2+2);

        buffer->Indices.push_back(i*2+3);
        buffer->Indices.push_back(i*2+1);
        buffer->Indices.push_back(1);
    }

   //    scene::IMeshManipulator * mm = mSceneManager->getMeshManipulator();
   //    mm->recalculateNormals(buffer);

    buffer->recalculateBoundingBox();

    return buffer;
}

scene::SMeshBuffer* CreateShipMeshBuffer()
{
   float depth = 30;
    core::vector3df vertices[8];
    vertices[0].set(-50, 0.f, -depth);
   vertices[1].set(50, 0.f, -depth);
   vertices[2].set(50, 15, -depth);
   vertices[3].set(20, 20, -depth);
   vertices[4].set(5, 30, -depth);
   vertices[5].set(-5, 30, -depth);
   vertices[6].set(-20, 20, -depth);
   vertices[7].set(-50, 15, -depth);

   return CreateExtrudedPolyLineMeshBuffer(8, vertices, core::vector3df(0, 0, depth));
}

scene::SMeshBuffer* CreateAlienMeshBuffer()
{
   float depth = 30;
    core::vector3df vertices[5];
    vertices[0].set(0, 0.f, -depth);
   vertices[1].set(-50, 30.f, -depth);
   vertices[2].set(-30, 50, -depth);
   vertices[3].set(30, 50, -depth);
   vertices[4].set(50, 30, -depth);

   return CreateExtrudedPolyLineMeshBuffer(5, vertices, core::vector3df(0, 0, depth));
}

scene::SMeshBuffer* CreateShotMeshBuffer()
{
   float depth = 10;
    core::vector3df vertices[4];
   vertices[0].set(-10, 0.f, -depth);
   vertices[1].set(10, 0, -depth);
   vertices[2].set(10, 10, -depth);
   vertices[3].set(-10, 10, -depth);

   return CreateExtrudedPolyLineMeshBuffer(4, vertices, core::vector3df(0, 0, depth));
}


struct Invaders
{
   Invaders();
   scene::ISceneNode * mModel;
};

Invaders::Invaders()
{
   mModel = NULL;
}

struct Controller
{
   Controller();
   void Reset();

   s32 mMove;
   s32 mShoot;
};

Controller::Controller()
{
   Reset();
}

void Controller::Reset()
{
   mMove = 0;
   mShoot = 0;
}

struct Player
{
    Player();

   s32 mLives;
   s32 mScore;
   scene::ISceneNode * mModel;
};

Player::Player()
{
   mLives = 0;
   mScore = 0;
   mModel = 0;
}

struct Shot
{
   Shot();
   scene::ISceneNode *mModel;
};

Shot::Shot()
{
   mModel = 0;
}

enum GameMode
{
   GM_MAINSCREEN,
   GM_RUNNING,
};

struct Game
{
   Game();
   void CreateMeshes();
   void CreateModels();
   void UpdateRunning(u32 time_);
   void StartGame();
   void SpawnInvaders();
   bool ShootPlayer( const core::vector3df &pos_ );
   bool ShootInvader( const core::vector3df &pos_ );
   bool mQuitGame;
   GameMode mGameMode;
   s32 mLevel;
   s32 mInvaderMoveDir;
   s32 mInvadersLiving;
   s32 mCountPlayerShots;
   Invaders mInvaders[INVADER_ROWS*INVADER_LINES];
   Controller mController;
   s32 mPain;
   s32 mNoStartTime;

   Player mPlayer;
   Shot mShotsInvaders[MAX_SHOTS_INVADERS];
   Shot mShotsPlayer[MAX_SHOTS_PLAYER];

   scene::SMesh * mMeshShip;
   scene::SMesh * mMeshAlien;
   scene::SMesh * mMeshShot;
};

Game::Game()
{
   mQuitGame = false;
   mGameMode = GM_MAINSCREEN;
   mMeshShip = NULL;
   mMeshAlien = NULL;
   mCountPlayerShots = 0;
   mPain = 0;
   mNoStartTime = 0;
}

void Game::CreateMeshes()
{
   scene::SMeshBuffer * meshBuffer = CreateShipMeshBuffer();
   mMeshShip = new scene::SMesh();
   mMeshShip->addMeshBuffer(meshBuffer);

   meshBuffer = CreateAlienMeshBuffer();
   mMeshAlien = new scene::SMesh();
   mMeshAlien->addMeshBuffer(meshBuffer);

   meshBuffer = CreateShotMeshBuffer();
   mMeshShot = new scene::SMesh();
   mMeshShot->addMeshBuffer(meshBuffer);
}

void Game::CreateModels()
{
   mPlayer.mModel = sceneManager->addMeshSceneNode( mMeshShip );
   mPlayer.mModel->setPosition( core::vector3df( 0, ROOM_BOTTOM, 0 ) );
   mPlayer.mModel->setVisible(true);

   for ( s32 y = 0; y < INVADER_LINES; ++y )
   {
      for ( s32 x = 0; x < INVADER_ROWS; ++ x)
      {
         mInvaders[y*INVADER_ROWS+x].mModel = sceneManager->addMeshSceneNode( mMeshAlien);
         mInvaders[y*INVADER_ROWS+x].mModel->setVisible(false);
      }
   }

   for ( s32 i=0; i<MAX_SHOTS_INVADERS; ++i )
   {
      mShotsInvaders[i].mModel = sceneManager->addMeshSceneNode( mMeshShot );
      mShotsInvaders[i].mModel->setVisible(false);
   }
   for ( s32 i=0; i<MAX_SHOTS_PLAYER; ++i )
   {
      mShotsPlayer[i].mModel = sceneManager->addMeshSceneNode( mMeshShot );
      mShotsPlayer[i].mModel->setVisible(false);
   }
}

void Game::StartGame()
{
   mLevel = 0;
   mInvaderMoveDir = 1;
   mGameMode = GM_RUNNING;
   mPlayer.mScore = 0;
   mPlayer.mLives = 3;
   SpawnInvaders();
}

void Game::SpawnInvaders()
{
   mInvadersLiving = INVADER_LINES*INVADER_ROWS;
   for ( s32 y = 0; y < INVADER_LINES; ++y )
   {
      for ( s32 x = 0; x < INVADER_ROWS; ++ x)
      {
         mInvaders[y*INVADER_ROWS+x].mModel->setPosition( core::vector3df( LEFT_ROW + x*ROW_OFFSET, UPPER_LINE+y*LINE_OFFSET, 0 ) );
         mInvaders[y*INVADER_ROWS+x].mModel->setVisible(true);
      }
   }
}

bool Game::ShootPlayer( const core::vector3df &pos_ )
{
   for ( s32 i=0; i < MAX_SHOTS_PLAYER; ++i )
   {
      if ( !mShotsPlayer[i].mModel->isVisible() )
      {
         mShotsPlayer[i].mModel->setPosition( pos_ + core::vector3df(0, 10, 0) );
         mShotsPlayer[i].mModel->setVisible(true);
         return true;
      }
   }
   return false;
}

bool Game::ShootInvader( const core::vector3df &pos_ )
{
   for ( s32 i=0; i < MAX_SHOTS_INVADERS; ++i )
   {
      if ( !mShotsInvaders[i].mModel->isVisible() )
      {
         mShotsInvaders[i].mModel->setPosition( pos_ + core::vector3df(0, -10, 0) );
         mShotsInvaders[i].mModel->setVisible(true);
         return true;
      }
   }
   return false;
}


void Game::UpdateRunning(u32 time_)
{
   float movePlayer = (float)time_* 1.f;
   if ( mController.mMove > 0 )
   {
      if ( mPlayer.mModel->getPosition().X <= ROOM_RADIUS )
         mPlayer.mModel->setPosition( mPlayer.mModel->getPosition() + core::vector3df(movePlayer, 0, 0) );
   }
   else if ( mController.mMove < 0 )
   {
      if ( mPlayer.mModel->getPosition().X >= -ROOM_RADIUS )
         mPlayer.mModel->setPosition( mPlayer.mModel->getPosition() + core::vector3df(-movePlayer, 0, 0) );
   }

   if ( mController.mShoot )
   {
      ++mCountPlayerShots;
      ShootPlayer( mPlayer.mModel->getPosition() );
      for ( s32 i=0; i < MAX_SHOTS_PLAYER; ++i )
      {
         if ( !mShotsPlayer[i].mModel->isVisible() )
         {
            mShotsPlayer[i].mModel->setPosition( mPlayer.mModel->getPosition() + core::vector3df(0, 10, 0) );
            mShotsPlayer[i].mModel->setVisible(true);
            break;
         }
      }
      mController.mShoot = 0;
   }

   bool invadersHitFloor = false;
   float speedInvader = 0.1f + (float)(INVADER_ROWS*INVADER_LINES-mInvadersLiving)*0.005 + (float)mLevel*0.01;
   float moveInvader = (float)time_* speedInvader;
   moveInvader *= mInvaderMoveDir;
   bool flipDir = false;
   for ( s32 i=0; i < INVADER_ROWS*INVADER_LINES; ++i )
   {
      if ( mInvaders[i].mModel->isVisible() )
      {
         mInvaders[i].mModel->setPosition( mInvaders[i].mModel->getPosition() + core::vector3df(moveInvader, 0, 0) );
         if (    mInvaders[i].mModel->getPosition().X < -ROOM_RADIUS
              ||   mInvaders[i].mModel->getPosition().X > ROOM_RADIUS )
         {
            flipDir = true;
         }
      }
   }
   if ( flipDir )
   {
      mInvaderMoveDir *= -1;
      moveInvader *= -1;
      float moveDown = 50.f;
      for ( s32 i=0; i < INVADER_ROWS*INVADER_LINES; ++i )
      {
         if ( mInvaders[i].mModel->isVisible() )
         {
            mInvaders[i].mModel->setPosition( mInvaders[i].mModel->getPosition() + core::vector3df(moveInvader, -moveDown, 0) );
            if ( mInvaders[i].mModel->getPosition().Y < ROOM_BOTTOM )
            {
               invadersHitFloor = true;
            }
         }
      }
   }

   static u32 urgeToShoot = 0;
   urgeToShoot += time_;
   if ( urgeToShoot > 3000 )
   {
      s32 row = mCountPlayerShots % INVADER_ROWS;   // nearly random ;-)

      for ( s32 y = INVADER_LINES-1; y >= 0; --y )
      {
         if ( mInvaders[y*INVADER_ROWS+row].mModel->isVisible() )
         {
            if ( ShootInvader( mInvaders[y*INVADER_ROWS+row].mModel->getPosition() ) )
            {
               urgeToShoot = 0;
               break;
            }
         }
      }
   }

   float moveShotPlayer = (float)time_ * 1.f;
   for ( s32 i=0; i < MAX_SHOTS_PLAYER; ++i )
   {
      if ( mShotsPlayer[i].mModel->isVisible() )
      {
         mShotsPlayer[i].mModel->setPosition( mShotsPlayer[i].mModel->getPosition() + core::vector3df(0, moveShotPlayer, 0) );
         if ( mShotsPlayer[i].mModel->getPosition().Y > ROOM_RADIUS )
         {
            mShotsPlayer[i].mModel->setVisible(false);
         }
      }
   }

   float moveShotInvader = (float)time_ * 1.f;
   for ( s32 i=0; i < MAX_SHOTS_INVADERS; ++i )
   {
      if ( mShotsInvaders[i].mModel->isVisible() )
      {
         mShotsInvaders[i].mModel->setPosition( mShotsInvaders[i].mModel->getPosition() + core::vector3df(0, -moveShotInvader, 0) );
         if ( mShotsInvaders[i].mModel->getPosition().Y < -ROOM_RADIUS )
         {
            mShotsInvaders[i].mModel->setVisible(false);
         }
      }
   }

   const float HIT_DIST = 3000.f;
   // check collisions
   for ( s32 i=0; i < MAX_SHOTS_PLAYER; ++i )
   {
      if ( mShotsPlayer[i].mModel->isVisible() )
      {
         for ( s32 a=0; a < INVADER_ROWS*INVADER_LINES; ++a )
         {
            if ( mInvaders[a].mModel->isVisible() )
            {
               if ( (mShotsPlayer[i].mModel->getPosition()-mInvaders[a].mModel->getPosition()).getLengthSQ() < HIT_DIST )
               {
                  mShotsPlayer[i].mModel->setVisible(false);
                  mInvaders[a].mModel->setVisible(false);
                  --mInvadersLiving;
                  ++mPlayer.mScore;
               }
            }
         }
      }
   }

   for ( s32 i=0; i < MAX_SHOTS_INVADERS; ++i )
   {
      if ( mShotsInvaders[i].mModel->isVisible() )
      {
         if ( (mShotsInvaders[i].mModel->getPosition()-mPlayer.mModel->getPosition()).getLengthSQ() < HIT_DIST )
         {
            --mPlayer.mLives;
            mShotsInvaders[i].mModel->setVisible(false);
            mPain = 500;
         }
      }
   }

   if ( mPlayer.mLives <= 0 || invadersHitFloor )
   {
      mGameMode = GM_MAINSCREEN;
      mNoStartTime = 3000;
   }

   if ( !mInvadersLiving )
   {
      ++mLevel;
      SpawnInvaders();
   }
}

// another global...
Game GAME;

class MyEventReceiver : public IEventReceiver
{
public:
   virtual bool OnEvent(const SEvent &event)
   {
      // allow leaving program with ESC (otherwise fullscreentests suck)
      if ( event.EventType == irr::EET_KEY_INPUT_EVENT
          && !event.KeyInput.PressedDown
          && event.KeyInput.Key == KEY_ESCAPE )
         GAME.mQuitGame = true;

      if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
      {
         switch ( event.KeyInput.Key )
         {
            case KEY_SPACE:
               if ( event.KeyInput.PressedDown )
               {
                  if ( GAME.mGameMode == GM_MAINSCREEN )
                  {
                     if ( GAME.mNoStartTime <= 0 )   // prevent accidential restart (shoot and restart use same keys)
                     {
                        GAME.StartGame();
                     }
                  }
                  else
                     GAME.mController.mShoot = 1;
               }
            break;
            case  KEY_LEFT:
               if ( event.KeyInput.PressedDown )
                  GAME.mController.mMove = -1;
               else
                  GAME.mController.mMove = 0;
            break;
            case KEY_RIGHT:
               if ( event.KeyInput.PressedDown )
                  GAME.mController.mMove = 1;
               else
                  GAME.mController.mMove = 0;
            break;
         }
      }
      return false;
   }
};

int main()
{
   MyEventReceiver receiver;
   IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
      core::dimension2d<s32>(800, 600), 16, false, false, false, &receiver);
   if (device == 0)
      return 1; // could not create selected driver.

   driver = device->getVideoDriver();
   sceneManager = device->getSceneManager();
   env = device->getGUIEnvironment();

   driver->setAmbientLight(video::SColor(255,255,255,255));

   GAME.CreateMeshes();
   GAME.CreateModels();

   scene::ICameraSceneNode* cam = sceneManager->addCameraSceneNode();
   cam->setPosition( core::vector3df( 0, 0, -1500 ) );
   cam->setTarget( core::vector3df( 0, 0, 0 ) );
   sceneManager->setActiveCamera(cam);

   gui::IGUIStaticText * staticScore = env->addStaticText(L"SCORE: ", core::rect<s32>(35,35,240,50) );
   gui::IGUIStaticText * staticLives = env->addStaticText(L"LIVES: ", core::rect<s32>(35,65,240,80) );
   gui::IGUIStaticText * staticStart = env->addStaticText(L"PRESS SPACE TO START", core::rect<s32>(135,165,340,180) );

   while(device->run() && driver && !GAME.mQuitGame)
   {
      if (device->isWindowActive())
      {
         static u32 oldTime = device->getTimer()->getRealTime();
           u32 newTime = device->getTimer()->getRealTime();
         u32 timeDiff = newTime - oldTime;
         oldTime = newTime;

         if ( GAME.mNoStartTime > 0 )
         {
            GAME.mNoStartTime -= timeDiff;
         }

         if ( GAME.mPain > 0 )
         {
            driver->beginScene(true, true, video::SColor(100,255,0,0));
            GAME.mPain -= timeDiff;
         }
         else
         {
            driver->beginScene(true, true, video::SColor(100,100,100,100));
         }

         if ( GAME.mGameMode == GM_MAINSCREEN )
         {
            staticStart->setVisible(true);   // bad: shouldn't be done each frame
         }
         else if ( GAME.mGameMode == GM_RUNNING )
         {
            staticStart->setVisible(false); // bad: shouldn't be done each frame
            GAME.UpdateRunning(timeDiff);
         }

         sceneManager->drawAll();


         wchar_t str[200];
         swprintf( str, 200, L" SCORE: %d", GAME.mPlayer.mScore );
         staticScore->setText( str );

         swprintf( str, 200, L" LIVES: %d", GAME.mPlayer.mLives );
         staticLives->setText( str );

         env->drawAll();

         driver->endScene();
      }
   }
   device->drop();
   return 0;
}
