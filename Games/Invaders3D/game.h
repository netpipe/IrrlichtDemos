//Game.h
#ifndef __GAME_H__
#define __GAME_H__
#include <irrlicht.h>
#include <audiere.h>
using namespace audiere;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#include <fstream>
using namespace std;
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <list>

//#include <conio.h>
#include <string.h>
#include <stdlib.h>
//#include "tinystr.h"
//#include "tinyxml.h"
#include "settings.h"
#include "math.h"
//Custom Tools
#include "chatQue.h"
#include "HighScore.h"
//Custom Objects
#include "bullet.h"
#include "alien.h"
#include "StarField.h"
#include "explosion.h"

#define PHYSICS_UPDATE_FPS         100.0f
#define PHYSICS_THREAD_SLEEPTIME   (1000.0f / PHYSICS_UPDATE_FPS)

class Game : public IEventReceiver {
protected:
        //Ingame Audio functions
        AudioDevicePtr* Audio_Device;
        OutputStreamPtr* Sound_Hit;
        //Explosion List
        std::list<Explosion> exList;
        //Score
        HighScore mHighScore;
        int mScore;
        std::string mPlayerName;
        //Starfield
        StarField *strField;
        //Round Logic
        unsigned short mRound;
        bool mStartNewRound;
        float mRoundDelay;
        void UpdateRoundLogic();
        void startGame();
        void killPlayer();
        bool mAlive;
        bool mAliveWindow;
        //Gui objecgt
        ChatQue* chatQue;
        //Bullets List
        std::list<Bullet> buList;
        //Enemy List
        std::list<Alien> alList;
        void spawnAliens();
        //Ship
        float fireSleep;
        scene::ISceneNode* ship;
        float shipX;
        float shipY;
        float shipZ;
        scene::IParticleSystemSceneNode* ps;

        void AlwaysUpdate();
        ISceneNode* pointerNode;
        bool quitMenu;
        unsigned gameType;
        bool UpdateAliens();

        bool keys[KEY_KEY_CODES_COUNT];
        bool getKeyState(EKEY_CODE key);
        int lastFPS;
        int gHEIGHT;
        int gWIDTH;
        u32 gDEPTH;
        video::E_DRIVER_TYPE gMODE;
        bool gSHADOWS;
        bool gFULLSCREEN;
        ICursorControl *curs;
        ITimer* ptime;
        u32 pold;
        float m_AccumlativeTimeSlice;
        float old_time;
        IParticleEmitter* em;
        bool show_fps;
        bool shaders;
    //GUI Code
        //GUI Variables
    	ICameraSceneNode *gamecam;
        IGUIStaticText* txtFPS;
        IGUIStaticText* txtSCORE;
        //GUI Functions
	    //GameFlow Functions
    	    void Input();
    	    void Update();
    	    void Render();
public:
	scene::ILightSceneNode* light1;
	scene::ILightSceneNode* light2;

	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	ILogger* logger;

	//////////////////////////////////////////////////////////////////////////
	// Irrlicht Callbacks
	virtual bool OnEvent(const SEvent& event);
    Game();
    IrrlichtDevice *gdevice;
	//////////////////////////////////////////////////////////////////////////
	// Game Functions
    void Init(settings conf);
	void Prepare();
	void MainLoop();
    u32 getrtime();
	unsigned Finish();
	void UpdateGame();
};

#endif
