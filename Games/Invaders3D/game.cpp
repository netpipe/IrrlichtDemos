//Author Rambus

#include "game.h"
//#include "BitStream.h"
//#include "HighScore.h"

Game::Game(){}

bool Game::OnEvent(const SEvent& event){
     //Keyboard events
   switch(event.EventType){
       case EET_KEY_INPUT_EVENT:
            keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
            return false;
            break;
       //InGame GUI events
       case EET_GUI_EVENT:{
     		      s32 id = event.GUIEvent.Caller->getID();
			      IGUIEnvironment* env = gdevice->getGUIEnvironment();
                  switch(event.GUIEvent.EventType){
                  	case EGET_BUTTON_CLICKED:
                        switch(id){
                           case 123: //Exit
                               gdevice->closeDevice();
                           //case 202: //Say
                            // say();
                        }
                    case EGET_MESSAGEBOX_YES:
                    {
                            s32 id = event.GUIEvent.Caller->getID();
                            quitMenu = false;
                            switch(id)
                            {
                            case 123: // Exut
                               Finish();
                             //  gdevice->closeDevice();
                               break;
                            case 124:
                                 quitMenu = false;
                                 gdevice->getCursorControl()->setVisible(false);
                                 break;
                            case 129:
                                 startGame();
                            }


                            break;
                    }
                    case EGET_MESSAGEBOX_CANCEL:
                         {
                            s32 id = event.GUIEvent.Caller->getID();
                            switch(id)
                            {
                            case 123: // Exut
                                 quitMenu = false;
                                 gdevice->getCursorControl()->setVisible(false);
                                 break;
                            case 124:
                                 quitMenu = false;
                                 gdevice->getCursorControl()->setVisible(false);
                                 break;
                            }
                          break;
                          }
                    case EGET_MESSAGEBOX_NO:
                         {
                            s32 id = event.GUIEvent.Caller->getID();
                            switch(id)
                            {
                            case 129:
                                 Finish();
                                 break;
                            case 124:
                                 quitMenu = false;
                                 gdevice->getCursorControl()->setVisible(false);
                                 break;
                            }

                            break;
                          }
         break;


                  }
            break;}
       //Log file events
       case EET_LOG_TEXT_EVENT:
            std::fstream LogFile("LogFile.log",std::ios::out|std::ios::app);
            LogFile << (event.LogEvent.Text) << std::endl;
            LogFile.close();
            break;
   }

	return false;
}

void Game::Init(settings conf)
{
    //Format config depth bool,  to u32
    mAliveWindow = false;
    u32 depth;
    if(conf.depth == true)
      depth = 32;
    else
      depth = 16;
    mPlayerName = "Space Invader";
     //Set Screen globals
     gHEIGHT = conf.height;
     gWIDTH = conf.width;
     show_fps = conf.fps;
     shaders = conf.shaders;
     gameType = conf.gameType;
     Audio_Device = conf.Audio_Device;
    gdevice = createDevice(conf.mode,dimension2du(conf.width,conf.height),depth,conf.fullscreen,false,false,0);
    gdevice->getFileSystem()->addZipFileArchive("Data.pk",false,false);
    ptime = gdevice->getTimer();
    gdevice->setEventReceiver(this);
   	gdevice->setWindowCaption(L"Space Invaders");
    curs = gdevice->getCursorControl();
	driver = gdevice->getVideoDriver();
	smgr = gdevice->getSceneManager();
	guienv = gdevice->getGUIEnvironment();
	guienv->setUserEventReceiver(this);
	logger = gdevice->getLogger();
 	gamecam = smgr->addCameraSceneNode(0,vector3df(0,100,0),vector3df(0,0,0),-1);
    gamecam->setFarValue(800);
	gamecam->setInputReceiverEnabled(true);
}

void Game::startGame()
{
         //Was already dead
         if(mAliveWindow)
         {
             //Kill Aliens
             std::list<Alien>::iterator iterA;
             for (iterA=alList.begin(); iterA != alList.end(); iterA++)
                        iterA->drop();
             alList.erase(alList.begin(),alList.end());
             //Kill Bullets
             std::list<Bullet>::iterator iterB;
             for (iterB=buList.begin(); iterB != buList.end(); iterB++)
                        iterB->drop();
             buList.erase(buList.begin(),buList.end());
         }
         mAliveWindow = false;
         mAlive = true;
         mScore = 0;
         mRound = 1;
         shipX = -40;
         shipY = 30;
         shipZ = 0;
         ship->setPosition(core::vector3df(shipX,shipY,shipZ));
         ship->setVisible(true);
         mStartNewRound = true;
         mRoundDelay = ptime->getTime() + 1600;
}

void Game::killPlayer()
{
     mAlive = false;
     ship->setVisible(false);
}

void Game::Prepare()
{
     old_time = ptime->getTime();
     fireSleep = old_time;
     //Init variables
     quitMenu = false;
    for (s32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
     keys[i] = false;
    //Sound
   // OutputStreamPtr* Sound_Hit;
    Sound_Hit = new OutputStreamPtr(OpenSound(*Audio_Device, "Media/Audio/bomb.wav", false));

	//Gui code
	gdevice->getCursorControl()->setVisible(false);
    IGUISkin* skin = guienv->getSkin();
   	IGUIFont* font = guienv->getFont("Media/Fonts/font.bmp");
   	if (font)
       skin->setFont(font);
    IGUIImage* img = guienv->addImage(driver->getTexture("Media/Gui/score.png"),position2d<int>(0,5));
    txtSCORE = guienv->addStaticText(L"Score: ",rect<int>(10,10,400,60), false);
    txtSCORE->setOverrideColor(SColor(0,230,230,230));
    txtFPS = guienv->addStaticText(L"",rect<int>(10,gHEIGHT-30,300,gHEIGHT-10), false);
    txtFPS->setOverrideColor(SColor(0,230,230,230));
    //ChatQue

    IGUIFont* guiFont;
    guiFont = gdevice->getGUIEnvironment()->getFont("Media/Fonts/gamefont.bmp");
    chatQue = new ChatQue();
    chatQue->initialise(gdevice,core::rect<s32>(10,100,gWIDTH-100,gHEIGHT-100),guiFont,8000,500);
    chatQue->setDefaultStyle(CQS_PLAIN);
        chatQue->addMessage( L"Irrlicht Space Invaders",SColor(255,55,255,55),SColor(255,60,60,60), CQS_SHADOW );
        chatQue->addMessage( L"By: Mark Laprairie",SColor(255,55,255,55),SColor(255,60,60,60), CQS_SHADOW );
        chatQue->addMessage( L"www.g0dsoft.com",SColor(255,55,255,55),SColor(255,60,60,60), CQS_SHADOW );
 	strField = new StarField(smgr,50);

    m_AccumlativeTimeSlice = 0;
    ptime->setTime(0);
    video::ITexture* bkg_texture = 0;
    ship = smgr->addAnimatedMeshSceneNode(smgr->getMesh("Media/Models/ship.MY3D"));
    ship->setRotation(vector3df(0,90,0));
    ship->setScale(vector3df(.2,.2,.16));

    //Background Starfield
	scene::ISceneNode* stars = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(200, 200));
    if ( stars )
    {
    	stars->setMaterialFlag(video::EMF_LIGHTING, false);
        stars->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    	stars->setMaterialTexture(0, driver->getTexture("Media/Models/starfield.png"));
        stars->setPosition(core::vector3df(0,10,0));
    }

    light1 = smgr->addLightSceneNode(gamecam, core::vector3df(90,100,0),
		video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 1400.0f);

	light2 = smgr->addLightSceneNode(0, core::vector3df(0,10,0),
		video::SColorf(0.7f, 0.0f, 0.0f, 0.0f), 50.0f,-1);

	startGame();

    logger->log(L"Game has finished loading.");//Random Debug Statment
}

void Game::UpdateRoundLogic()
{
     float lCurTime = ptime->getTime();
     if(mStartNewRound && mRoundDelay < lCurTime)
     {
         mRoundDelay = lCurTime + 1500;
         mStartNewRound = false;
         core::stringw str = L"[ Alien Wave #";
	     str += mRound;
	     str += " ]";
	     txtSCORE->setText(str.c_str());
         chatQue->addMessage( str.c_str(),SColor(255,55,255,55),SColor(0,0,0,0), CQS_PULSE );
         mRound++;
         //Aliens
         spawnAliens();
     }
}
void Game::spawnAliens()
{
     int lAlienLines;
     float lMoveSpeed = 0.02 + ((float)mRound*(0.006));
     switch(mRound){
         case 1:
              lAlienLines = 3;
              break;
         case 2:
              lAlienLines = 4;
              break;
         default:
              lAlienLines = 5;
              break;
     }

     for(int i = 1;i<11;i++) //i = left to right
     {
             for(int j = 1;j<lAlienLines;j++) //j = up/down
             {
                    Alien* a = new Alien(smgr,i,j,&buList,lMoveSpeed);
                    alList.push_front(*a);
             }
     }
}
u32 Game::getrtime()
{
     return ptime->getRealTime();
}

void Game::MainLoop()
{
   Update();
   m_AccumlativeTimeSlice = (ptime->getTime() * 1.0f);
   while( m_AccumlativeTimeSlice > (old_time))
   {
     AlwaysUpdate();
     if(mAlive)
     {
        UpdateGame();
        UpdateRoundLogic();
     }

     old_time += (PHYSICS_THREAD_SLEEPTIME*1.0f);
   }
   Render();
}


void Game::AlwaysUpdate()
{
     strField->update();
     //Update Explosions
     std::list<Explosion>::iterator iter;
     for (iter=exList.begin(); iter != exList.end(); iter++)
     {
         if(iter->update())
         {
              iter->drop();
              exList.erase(iter);
              //delete &iter;
              return;
            //Then Dead!
         }
     }

}
void Game::Update()
{
    Input();
}

bool Game::UpdateAliens()
{
     bool changeDir = false;
     std::list<Alien>::iterator iterA;
     mStartNewRound = true;
     for (iterA=alList.begin(); iterA != alList.end(); iterA++)
     {
           mStartNewRound = false;
           std::list<Bullet>::iterator blst;
           for(blst=buList.begin(); blst != buList.end(); blst++)
           {
                if(blst->getTarget() && blst->collidesWith(iterA->mPos.Z,iterA->mPos.X,2))
                {
                    //Death sound
                    OutputStream* Out_Shot = OpenSound(*Audio_Device, "Media/Audio/bomb.wav", true);
                    Out_Shot->setVolume(0.3);
                    (Out_Shot)->play();
                    //Spawn Explosion
                    Explosion* e = new Explosion(gdevice,iterA->mPos);
                    exList.push_front(*e);
                    //Kill bullet
                    blst->drop();
                    //delete **blst;
                    buList.erase(blst);
                    //delete &blst;
                    iterA->drop();
                    //delete iterA;
                    alList.erase(iterA);
                    mScore += 80 + (mRound*10);
                    //delete &iterA;
                    return changeDir;
                }
           }
           if(iterA->update())
              changeDir = true;
     }
     return changeDir;
}

void Game::UpdateGame()
{
     //Frame Time
     u32 lFrameTime = ptime->getTime();
     ///Update Controls
     vector3df tmpRot = ship->getRotation();
     tmpRot.Y = 90;
     //Movement
     if(getKeyState(KEY_KEY_A)){
         shipZ += 0.6;
         if(shipZ > 55)
            shipZ = 55;
         tmpRot.Y = 80;
         ship->setPosition(core::vector3df(shipX,shipY,shipZ));
     }
     if(getKeyState(KEY_KEY_D)){
         shipZ -= 0.6;
         if(shipZ < -55)
            shipZ = -55;
         tmpRot.Y = 100;
         ship->setPosition(core::vector3df(shipX,shipY,shipZ));
     }
     ship->setRotation(tmpRot);
     //Fire
     if(getKeyState(KEY_SPACE)){
        if(fireSleep < lFrameTime){
            Bullet* b = new Bullet(smgr,vector3df(shipX+5,shipY,shipZ),1);
            buList.push_front(*b);
            fireSleep = lFrameTime + 450;
            //Play Fire Sound
            OutputStream* Out_Shot = OpenSound(*Audio_Device, "Media/Audio/lazer.wav", true);
            Out_Shot->setVolume(0.3);
            (Out_Shot)->play();
        }
     }

     ///Update Enemys
     bool hasDied = false;
     if(UpdateAliens()){
         std::list<Alien>::iterator iter;
         for (iter=alList.begin(); iter != alList.end(); iter++)
         {
               iter->changeDirection();
               if(iter->killsPlayer())
               {
                //  mHighScore.addScore(mScore,"Invader");
                  hasDied = true;
                  //Play death sound
                  if(Sound_Hit)
                     (*Sound_Hit)->play();
               }

         }
     }
     ///Update Bullets
     std::list<Bullet>::iterator iterB;
     for (iterB=buList.begin(); iterB != buList.end(); iterB++)
     {
           //Submit high score
           //Collide bullets with player
           if(iterB->collidesWith(shipZ,shipX,2))
           {
              //mHighScore.addScore(mScore,"Invader");
              hasDied = true;
              Explosion* e = new Explosion(gdevice,iterB->getPosition());
              //Play death sound
              if(Sound_Hit)
                 (*Sound_Hit)->play();
              exList.push_front(*e);
              iterB->drop();
              buList.erase(iterB);
              break;
           }

           if(iterB->update())//Remove lazer
           {
              iterB->drop();
              buList.erase(iterB);
              //delete &iterB;
              break;
           }
     }
     //Has the player died?
     if(hasDied)
        killPlayer();
}
void Game::Input()
{
     if (quitMenu == false){
       if (getKeyState(KEY_ESCAPE) || getKeyState(KEY_KEY_Q)){
            guienv->addMessageBox(L"Are you sure you want to exit?",L"Yes?",true,EMBF_YES | EMBF_CANCEL,0,123);
             gdevice->getCursorControl()->setVisible(true);
            quitMenu = true;
       }
     }
     if(mAlive == false && mAliveWindow == false){
               mAliveWindow = true;
               guienv->addMessageBox(L"You have failed!",L"Start a new game?",true,EMBF_YES | EMBF_NO,0,129);
               gdevice->getCursorControl()->setVisible(true);
     }
 return;
}
void Game::Render()
{
    driver->beginScene(true, true, video::SColor(0,0,0,0));

	if (show_fps && lastFPS != driver->getFPS()){
       lastFPS = driver->getFPS();
	   core::stringw str = L"FPS:";
	   str += lastFPS;
       txtFPS->setText(str.c_str());
	}
    core::stringw str = L"Score: ";
	str += mScore;
	txtSCORE->setText(str.c_str());
	// Render the scene
	smgr->drawAll();
	guienv->drawAll();
	// Chat Queue
	chatQue->draw();
	driver->endScene();
}

unsigned Game::Finish()
{
   guienv->grab();
   guienv->drop();
   smgr->grab();
   smgr->drop();
   gdevice->grab();
   gdevice->closeDevice();
   gdevice->drop();
   return 0;
}

//KeyCode
bool Game::getKeyState(EKEY_CODE key){
     return keys[key];
}

