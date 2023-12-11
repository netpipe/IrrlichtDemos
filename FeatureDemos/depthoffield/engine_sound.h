#ifndef ENGINE_MUSIC_H
#define ENGINE_MUSIC_H
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <iostream>
//#include <irrKlang.h>
#include "irrlicht.h"
#include <time.h>
#include <unistd.h>
#include <cstdlib>
//#include <conio.h>
#include <fstream>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <sys/timeb.h>
#include <ctime>
#pragma comment(lib, "irrKlang.lib")

irrklang::ISoundEngine* enginesound = irrklang::createIrrKlangDevice();
const irrklang::ik_c8* track[46] = { "./Game Data/music/are you dead yet_.mp3", "./Game Data/music/atwa.mp3", "./Game Data/music/banned from heaven.mp3", "./Game Data/music/bastards of bodom.mp3", "./Game Data/music/bedrazor.mp3", "./Game Data/music/blooddrunk.mp3", "./Game Data/music/breaking the habit.mp3", "./Game Data/music/burn the eye.mp3", "./Game Data/music/byob.mp3", "./Game Data/music/chinese democracy.mp3", "./Game Data/music/cigaro.mp3", "./Game Data/music/crawling.mp3", "./Game Data/music/deer dance.mp3", "./Game Data/music/downfall.mp3", "./Game Data/music/everytime i die.mp3", "./Game Data/music/faint.mp3", "./Game Data/music/flight to the dream.mp3", "./Game Data/music/from the inside.mp3", "./Game Data/music/fuck the system.mp3", "./Game Data/music/heavy metal is my law.mp3", "./Game Data/music/hellbounds.mp3", "./Game Data/music/in the end.mp3", "./Game Data/music/in your face.mp3", "./Game Data/music/living dead beat.mp3", "./Game Data/music/nil.mp3", "./Game Data/music/nem.mp3", "./Game Data/music/numb.mp3", "./Game Data/music/one day you will cry.mp3", "./Game Data/music/one step closer.mp3", "./Game Data/music/open your eyes.mp3", "./Game Data/music/papercut.mp3", "./Game Data/music/punchmeibleed.mp3", "./Game Data/music/questions.mp3", "./Game Data/music/raining blood.mp3", "./Game Data/music/requirem.mp3", "./Game Data/music/roadkill.mp3", "./Game Data/music/runaway.mp3", "./Game Data/music/shut me up.mp3", "./Game Data/music/smile pretty for the devil.mp3", "./Game Data/music/somwhere i belong.mp3", "./Game Data/music/storm.mp3", "./Game Data/music/the unforgiven.mp3", "./Game Data/music/tie my rope.mp3", "./Game Data/music/trashed lost & strungout.mp3", "./Game Data/music/hate me.mp3", "./Game Data/music/silent night bodom night.mp3" };
unsigned int track_count = 4;
bool changing=false;
class joystick;
struct resolution {
   irr::s32 x;
   irr::s32 y;
};
resolution res = {1280, 960};
int savemenu();
class MusicEndedReceiver : public irrklang::ISoundStopEventReceiver {
   public:
   virtual void OnSoundStopped (irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE ESEC_SOUND_FINISHED_PLAYING, void* userData)
   {
      if ((changing==false)&&(sound)) {
         if (track_count<45)
            track_count++;
         else if (track_count==45)
            track_count=0;
         std::ifstream inp;
         const char* myFileName = track[track_count];
         inp.open(myFileName, std::ifstream::in);
         inp.close();
         if(inp.fail())
         {
            inp.clear(std::ios::failbit);
            unsigned int track_c = track_count;
            if (track_count==0)
               track_c=45;
            else if (track_count<46)
               track_c--;
            sound = enginesound->play2D(track[track_c], false, false, true);
         }
         else
            sound = enginesound->play2D(track[track_count], false, false, true);
         sound->setSoundStopEventReceiver(this);
         savemenu();
      }
   }
};

void change_track(irrklang::ISound* sound, MusicEndedReceiver* musstop, unsigned int tracknumber, int & track_c) {
   if (tracknumber<46) {
      if (((sound)&&(track[track_c]))&&((track[tracknumber])&&(musstop))) {
         changing=true;
         enginesound->removeSoundSource(track[track_c]);
         std::ifstream inp;
         std::ofstream out;
         const char* myFileName = track[tracknumber];
         inp.open(myFileName, std::ifstream::in);
         inp.close();
         if (inp.fail()) {
            inp.clear(std::ios::failbit);
            sound = enginesound->play2D(track[track_c], false, false, true);
         }
         else {
            sound = enginesound->play2D(track[tracknumber], false, false, true);
            track_c = tracknumber;
         }
         sound->setSoundStopEventReceiver(musstop);
         changing=false;
         savemenu();
      }
#ifdef DEBUG
      else
         std::cout << "Error 5: irrklang::ISound, track array or MusicEndedReceiver are NULL" << std::endl;
#endif
   }
#ifdef DEBUG
   else
      std::cout << "Error 6: Track number!" << std::endl;
#endif
}

float Master_Volume=1.00;

void change_volume(float vol) {
   enginesound->setSoundVolume(vol);
   Master_Volume = vol;
   savemenu();
}

float Music_Volume=1.00;
#endif
