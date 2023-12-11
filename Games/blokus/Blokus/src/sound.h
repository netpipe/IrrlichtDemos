#ifndef __SOUND_H_
#define __SOUND_H_

#ifdef WIN32
	#include <conio.h>//1.0ïKê{
	#pragma comment(lib, "irrKlang.lib")
#else
	#include "conio.h"//1.0ïKê{
#endif

//#include <irrKlang.h>

//using namespace irr;//0.9
//using namespace audio;//0.9
//using namespace irrklang;//1.0
//
//class CSound
//{
//	public:
//		CSound();
//		~CSound();
//
//		void play(char* filename, bool bLoop);
//		void stop(bool bStop);
//		void setVolume(float vol);
//
//	private:
//		ISoundEngine* sound;
//};

#endif //__CSOUND_H_
