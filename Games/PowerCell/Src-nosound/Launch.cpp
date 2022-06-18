#include "Globals.hpp"
#include "Output.hpp"



IrrlichtDevice *device = 0;
MyEventReceiver receiver;
u32 idleTime = 0;

vector3df target;
vector3df location;

triangle3df   triangle;
line3d<f32> raytrace;
ITriangleSelector* selector;
ISceneCollisionManager* colmgr;

position2d<s32> Cursor_Position;

namespace FPS
{
  f32
    m_fps = 0,
    m_lastFPS = 0;

  void Update(void)
  {
    m_fps = device->getVideoDriver()->getFPS();
    if(m_lastFPS != m_fps)
    {
      stringw str = L"PowerCell [";
      str += device->getVideoDriver()->getName();
      str += "] FPS:";
      str += m_fps;

      device->setWindowCaption(str.c_str());
      m_lastFPS = m_fps;
    }
  }
};

void SetDefaultValues(void)
{
  Output::Instance()->w("Asking classes to set default values.\n");
  LocalTimer::Instance()->SetDefaultValues();
  Player::Instance()->SetDefaultValues();
  PlayerHelper::Instance()->SetDefaultValues();
  Scene::Instance()->SetDefaultValues();
  Elevator::Instance()->SetDefaultValues();
  Stage::Instance()->SetDefaultValues();
  Sound::Instance()->SetDefaultValues();
}

void TimedEvents(void)
{
  LocalTimer::Instance()->Update();

  if(Scene::Instance()->GetState() == STATE_INGAME)
  {
    //! ----------------------------------------------
    //! Checks done less regularly.
    //! ----------------------------------------------
    if(LocalTimer::Instance()->Tick())
    {
        FPS::Update();

        PlayerHelper::Instance()->DecreaseHealth(1);

        GameGUI::Instance()->updateTimeInfo(LocalTimer::Instance()->GetTime());
        GameGUI::Instance()->updateHealthInfo(PlayerHelper::Instance()->GetHealth());

        Elevator::Instance()->CheckCollisionWithPlayer();

        Player::Instance()->CheckFalling();
    }

    //! ----------------------------------------------
    //! Checks done every frame.
    //! ----------------------------------------------
		PowerCell::Instance()->CheckCollisionWithPlayer();
		HealthCell::Instance()->CheckCollisionWithPlayer();

        Obstacle::Instance()->CheckCollisionWithPlayer();

        Elevator::Instance()->UpdatePlayerPosition();

//    if(moving) Sound::Instance()->PlayFootStepSound();
  // Sound::Instance()->UpdateListener(Player::Instance()->getPlayerNode()->getPosition(),Player::Instance()->getPlayerNode()->getRotation());

  }

  if(Scene::Instance()->GetState() == STATE_GAME_OVER)
  {
    if(LocalTimer::Instance()->TickFast())
    {
      FPS::Update();
    }


   // Scene::Instance()->UpdateEndCamera(Player::Instance()->getPlayerNode()->getPosition());
   // Sound::Instance()->UpdateListener(Player::Instance()->getPlayerNode()->getPosition(),Player::Instance()->getPlayerNode()->getRotation());
  }

}

void GameLoop(void)
{
  bool moving=0;
  if(Scene::Instance()->GetState() == STATE_INGAME)
  {
		Camera::Instance()->moveCameraControl();

    if(receiver.getKeyState(KEY_SPACE))
    {
      Player::Instance()->jump();
    }


    if(!receiver.getKeyState(KEY_LSHIFT)){
      Player::Instance()->setSpeed(400);//normal run
    //  idleTime = 0;
    //   moving=1;
    }
    else{
        //speed boost run << todo change the anim speed
        Player::Instance()->setSpeed(600);
    }
    if(receiver.getKeyState(KEY_KEY_W)){
      Player::Instance()->moveForward();
      idleTime = 0;
    moving=1;
    }
    if(receiver.getKeyState(KEY_KEY_S)){
      Player::Instance()->moveBackwards();
      idleTime = 0;
      moving=1;
    }
    if (receiver.getKeyState(KEY_KEY_A)){
      Player::Instance()->strafeLeft();
      idleTime = 0;
      moving=1;
    }
    if (receiver.getKeyState(KEY_KEY_D)){
      Player::Instance()->strafeRight();
      idleTime = 0;
      moving=1;
    }
    if(receiver.getKeyState(KEY_KEY_C))
    {
      LocalTimer::Instance()->SetTime(30000);
      PlayerHelper::Instance()->IncreaseAquiredPowerCells(1);
      GameGUI::Instance()->updatePowerCellInfo(
        PlayerHelper::Instance()->GetPowerCellsAquired(),
        PlayerHelper::Instance()->GetPowerCellsTotal());
      idleTime = 0;
    }
    else if(receiver.getKeyState(KEY_KEY_I))
    {   PlayerHelper::Instance()->IncreaseHealth(1);    }

    else if(receiver.getKeyState(KEY_KEY_U))
    {   PlayerHelper::Instance()->DecreaseHealth(111);  }

    if(moving)  { }//Sound::Instance()->PlayFootStepSound(0); }
        else {
			if(Player::Instance()->getIsRunning())
				Player::Instance()->setIsRunning(false);
			idleTime += 1;
			if(idleTime>500){
				srand(LocalTimer::Instance()->GetTick());
				// int rnd = (rand()%10)+1;    stringc file = "c";     file += rnd;
				// file += ".wav";     Sound::PlaySound(file);
				Player::Instance()->getPlayerNode()->setMD2Animation("jump");
				idleTime = 0;
			}
			 else if(receiver.getKeyState(KEY_ESCAPE))
			 {
				HealthCell::Instance()->Clear();
				PowerCell::Instance()->Clear();
				Elevator::Instance()->Clear();
				Obstacle::Instance()->Clear();
				device->getSceneManager()->clear();
				device->getGUIEnvironment()->clear();
				SetDefaultValues();
				MenuGUI::Instance()->Init();
				Scene::Instance()->SetState(STATE_MENU_MAIN);
			    Sound::Instance()->PlayBackgroundMusic(MUSIC_MENU);
			 }
		}
    TimedEvents();
  }
  else if(Scene::Instance()->GetState() == STATE_GAME_OVER)
  {
    Obstacle::Instance()->CheckCollisionWithPlayer();
    Elevator::Instance()->CheckCollisionWithPlayer();
    Elevator::Instance()->UpdatePlayerPosition();

    if(receiver.getKeyState(KEY_ESCAPE))
    {
        Player::Instance()->getPlayerNode()->setMD2Animation("stand");
        HealthCell::Instance()->Clear();
        PowerCell::Instance()->Clear();
        Elevator::Instance()->Clear();
        Obstacle::Instance()->Clear();
        device->getSceneManager()->clear();
        device->getGUIEnvironment()->clear();
        SetDefaultValues();
        MenuGUI::Instance()->Init();
        Scene::Instance()->SetState(STATE_MENU_MAIN);
        Sound::Instance()->PlayBackgroundMusic(MUSIC_MENU);
    }
  }
  else
  {
       device->sleep(20,0); //! state game over sleep while not active
    TimedEvents();
  }
}


int main(int argc, char** argv)
{
    Output::Instance()->w("Creating device.\n");

    device = createDevice(EDT_OPENGL, dimension2du(800, 600), 32, false, false, false, 0);
    device->setWindowCaption(L"PowerCell");

    Output::Instance()->w("Passing device to classes.\n");
    Camera::Instance()->setDevice(device);
    Collision::Instance()->setDevice(device);
    GameGUI::Instance()->setDevice(device);
    HealthCell::Instance()->setDevice(device);
    Player::Instance()->setDevice(device);
    PlayerHelper::Instance()->setDevice(device);
    PowerCell::Instance()->setDevice(device);
    Scene::Instance()->setDevice(device);
    LocalTimer::Instance()->setDevice(device);
    MenuGUI::Instance()->setDevice(device);

    SetDefaultValues();

    Sound::Instance()->Create();
    Sound::Instance()->PlayBackgroundMusic(MUSIC_MENU);

		device->getFileSystem()->changeWorkingDirectoryTo("media");
    //! Create main menu, and set state to match.
    Scene::Instance()->SetState(STATE_MENU_MAIN);
    MenuGUI::Instance()->Init();
    //! Sound INIT





    //! ----------------------------------------------------
    //! Event receiver
    //! ----------------------------------------------------
    Output::Instance()->w("Setting event receiver.\n");

    receiver.setDevice(device);
    device->setEventReceiver(&receiver);

//device->getCursorControl()->setVisible(false);

    Output::Instance()->w("Entering main loop.\n");
    while(device->run())
    {        Sound::Instance()->PlayAll();
      if(device->isWindowActive())
       {


                GameLoop();
        device->getVideoDriver()->beginScene(true, true, SColor(0,200,200,200));
        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();
        device->getVideoDriver()->endScene();

        device->sleep(5,0);
       }
      else
       {
        device->sleep(50,0);
       }
    }
    //PowerCell::Clear();
    Sound::Instance()->Drop();
    Elevator::Instance()->Clear();
    Obstacle::Instance()->Clear();
    device->drop();
    return 0;
}

