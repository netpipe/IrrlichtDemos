#include "irrlicht.h"
#include "fmod.hpp"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;
using namespace FMOD;

#pragma comment(lib, "fmodex_vc.lib")

 //fmod defines
 FMOD::Sound      *bkMainMenuSound, *bkLevelSound, *bkLevel2Sound, *pickUpSound, *ouchSound;
 FMOD::Channel    *channel1, *channel2, *channel3;
 FMOD::System     *systemFMOD;


void loadAudioFiles()
{

	// load music
    //FMOD_RESULT       result;

    unsigned int      version;

    FMOD::System_Create(&systemFMOD);
    systemFMOD->getVersion(&version);

    if (version < FMOD_VERSION)
    {
        printf("[ERROR]  You are using an old version of FMOD %08x. This game requires %08x\n", version, FMOD_VERSION);
    }


   systemFMOD->setOutput(FMOD_OUTPUTTYPE_ALSA);
   systemFMOD->init(32, FMOD_INIT_NORMAL, 0);

   if (musicOn==1){ systemFMOD->createSound("data/audio/main_menu.ogg", FMOD_SOFTWARE, 0, &bkMainMenuSound); }else{}
   if (musicOn==1){ systemFMOD->createSound("data/audio/level.ogg", FMOD_SOFTWARE, 0, &bkLevelSound); }else{}
   if (musicOn==1){ systemFMOD->createSound("data/audio/level2.ogg", FMOD_SOFTWARE, 0, &bkLevel2Sound); }else{}
   if (sfxOn == 1){ systemFMOD->createSound("data/audio/pickup.ogg", FMOD_SOFTWARE, 0, &pickUpSound); }else{}
   if (sfxOn == 1){ systemFMOD->createSound("data/audio/ouch.ogg", FMOD_SOFTWARE, 0, &ouchSound); }else{}

   bkMainMenuSound->setMode(FMOD_LOOP_NORMAL);
   bkLevelSound->setMode(FMOD_LOOP_NORMAL);
   pickUpSound->setMode(FMOD_LOOP_OFF);
   ouchSound->setMode(FMOD_LOOP_OFF);






}
