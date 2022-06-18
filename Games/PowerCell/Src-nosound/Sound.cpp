#include "Sound.hpp"

#include <pthread.h>

using namespace agEngine;


Sound *Sound::m_sound = NULL;
Sound Sound::m_default_sound_buffer;

Sound *Sound::Instance()
{
  if(!m_sound)
    m_sound = &m_default_sound_buffer;

	return m_sound;
}

void Sound::SetDefaultValues()
{
	// Nothing to set
}

// Isn't this redundant?
// Yes is is -dee
void Sound::Close()
{
	Drop();
}

int Sound::Create(void)
{
  //! Load music samples.
  Output::Instance()->w("Loading samples.\n");

	//!Sound init
//  adevice = agEngine::createDevice(true);
//
//  menuM = SoundData::LoadAudio(std::string("media/Sounds/cell_menu.ogg"), false);
//  stage1M = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_stage01.ogg"), false);
//  stage2M = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_stage02.ogg"), false);
//  stage3M = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_stage03.ogg"), false);
//  stage4M = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_stage04.ogg"), false);
//  clearM = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_cleared.ogg"), false);
//  deadM = agEngine::SoundData::LoadAudio(std::string("media/Sounds/cell_dead.ogg"), false);
//  footS = agEngine::SoundData::LoadAudio(std::string("media/Sounds/sound30m.ogg"), false);
//
//  musicSource = adevice->createASource(NULL);
//  channel1 = adevice->createASource(NULL);
//  channel2 = adevice->createASource(NULL);
//
//  musicSource->setVolume(0.3f);
//  musicSource->setAttenuation(0.0f);
//  musicSource->setLoop(true);
//  musicSource->setPosition(0.0f, 0.0f, 0.0f);
//  musicSource->setRelative(true);
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
  switch(music)
  {
    case MUSIC_MENU:
    {
		musicSource->stop();
		musicSource->attachSound(menuM);
		musicSource->play();
    } break;
    case MUSIC_STAGE1:
    {
		musicSource->stop();
		musicSource->attachSound(stage1M);
		musicSource->play();
    } break;
    case MUSIC_STAGE2:
    {
		musicSource->stop();
		musicSource->attachSound(stage2M);
		musicSource->play();
    } break;
    case MUSIC_STAGE3:
    {
		musicSource->stop();
		musicSource->attachSound(stage3M);
		musicSource->play();
    } break;
        case MUSIC_STAGE4:
    {
		musicSource->stop();
		musicSource->attachSound(stage4M);
		musicSource->play();
    } break;
    case MUSIC_CLEAR:
    {
		musicSource->stop();
		musicSource->attachSound(clearM);
		musicSource->play();
    } break;
    case MUSIC_DEAD:
    {
		musicSource->stop();
		musicSource->attachSound(deadM);
		musicSource->play();
    } break;
  }
}

void Sound::PlayCellPickupSample(vector3df position)
{
  samplePos[0] = position.X;
  samplePos[1] = position.Y;
  samplePos[2] = position.Z;

  // Fix Me! (put whatever the pickup sound is @_@
  agEngine::SoundData *pickupSound = footS;

  // Check all the available channels for one that's free
  //if (channel1->isPlaying())
  //{
	  channel1->stop();
	  channel1->attachSound(pickupSound);
	  channel1->play();
 // }
  //else if (channel2->isPlaying())
  //{
	//  channel2->stop();
	//  channel2->attachSound(pickupSound);
	//  channel2->play();
  //}
}

void Sound::UpdateListener(vector3df position, vector3df rotation)
{
  pos[0] = position.X;
  pos[1] = position.Y;
  pos[2] = position.Z;

  vel[0] = (pos[0] - lastPos[0]) * 1000;
  vel[1] = (pos[1] - lastPos[1]) * 1000;
  vel[2] = (pos[2] - lastPos[2]) * 1000;

  lastPos[0] = pos[0];
  lastPos[1] = pos[1];
  lastPos[2] = pos[2];

  adevice->setListenerPosition(pos[0], pos[1], pos[2]);
//  Output::Instance()->w("\n%f %f %f\n%f %f %f\n\n", pos[0], pos[1], pos[2], vel[0], vel[1], vel[2]);
}

void Sound::Drop(void)
{
	deadS->drop();
	menuM->drop();
	stage1M->drop();
	stage2M->drop();
	stage3M->drop();
	stage4M->drop();
	deadM->drop();
	clearM->drop();

	// Make sure to drop the sources AFTER the sound data.
	musicSource->drop();
	channel1->drop();
	channel2->drop();

	adevice->drop();
}

void Sound::PlayFootStepSound(bool play){
    if ( play ){
	// Fix Me! (put whatever the foot sound is =/
	agEngine::SoundData *footSound = footS;

	  // Check all the available channels for one that's free
	if (!channel1->isPlaying())
	{
		channel1->stop();
		channel1->attachSound(footSound);
		channel1->setPosition(samplePos[0], samplePos[1], samplePos[2]);
		channel1->play();
	}
	else if (!channel2->isPlaying())
	{
		channel2->stop();
		channel2->attachSound(footSound);
		channel2->setPosition(samplePos[0], samplePos[1], samplePos[2]);
		channel2->play();
	}
    }
}

void Sound::PlayAll(void)
{
	adevice->playAll();
}
