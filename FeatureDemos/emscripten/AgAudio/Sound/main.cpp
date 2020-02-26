#include <pthread.h>

#include "audio.h"

#define _DEBUG_MODE_
#define _PLAY_SOUND_
//#include "Sound.hpp"

class Sound
{

  public:
    unsigned int
      *musicMenu,
      *musicStage1,
      *musicStage2,
      *musicStage3,
      *musicDead,
      *musicClear,

      *sampleCellPickup;

    float
      pos[3],
      samplePos[3],
      lastPos[3],
      vel[3];

//    vector3df rot;

	pthread_t soundThread;
	agEngine::AudioDevice *adevice;

		//! Sound Init
//	agEngine::SoundData *deadS;
	agEngine::SoundData *menuM;
//	agEngine::SoundData *stage1M;
//	agEngine::SoundData *stage2M;
//	agEngine::SoundData *stage3M;
//	agEngine::SoundData *stage4M;
//	agEngine::SoundData *deadM;
//	agEngine::SoundData *clearM;
//	agEngine::SoundData *footS;

	agEngine::AudioSource *musicSource;
	// Add more channels if you want
	agEngine::AudioSource *channel1;
	agEngine::AudioSource *channel2;



    static Sound *Instance();

    void SetDefaultValues(void);
    void Close();
    int Create(void);
    void PlayBackgroundMusic(int music);
//    void PlayCellPickupSample(vector3df position);
//    void UpdateListener(vector3df position, vector3df rotation);
    void Drop(void);
    void PlayFootStepSound(bool);
    void PlayAll(void);

  protected:

    static Sound *m_sound;
    static Sound m_default_sound_buffer;
};


Sound *Sound::m_sound = NULL;
Sound Sound::m_default_sound_buffer;


int Sound::Create(void)
{
  //! Load music samples.
//  Output::Instance()->w("Loading samples.\n");

	//!Sound init
  adevice = agEngine::createDevice(true);

  menuM = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_menu.ogg"), false);
//  stage1M = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_stage01.ogg"), false);
//  stage2M = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_stage02.ogg"), false);
//  stage3M = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_stage03.ogg"), false);
//  stage4M = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_stage04.ogg"), false);
//  clearM = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_cleared.ogg"), false);
//  deadM = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_dead.ogg"), false);
//  footS = agEngine::SoundData::LoadAudio(std::string("media/Sounds/sound30m.ogg"), false);

  musicSource = adevice->createASource(NULL);
  channel1 = adevice->createASource(NULL);
  channel2 = adevice->createASource(NULL);

  musicSource->setVolume(0.3f);
  musicSource->setAttenuation(0.0f);
  musicSource->setLoop(true);
  musicSource->setPosition(0.0f, 0.0f, 0.0f);
  musicSource->setRelative(true);
//
//  channel1->setVolume(1.0f);
//  channel1->setLoop(false);
//  channel1->setRelative(true);
//
//  channel2->setVolume(1.0f);
//  channel2->setLoop(false);
//  channel2->setRelative(true);
}

void Sound::PlayBackgroundMusic(int music)
{
//  switch(music)
//  {
//    case MUSIC_MENU:
//    {
		musicSource->stop();
		musicSource->attachSound(menuM);
		musicSource->play();
//
//}}

}

int main(){


    Sound::Instance()->Create();
    Sound::Instance()->PlayBackgroundMusic(1);


 Sound::Instance()->PlayAll();

    Sound::Instance()->Drop();
}


