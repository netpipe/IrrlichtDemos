#include "Sound.h"
#include <SFML/Audio.hpp>
Sound::Sound(char * fileName)
{
    Load(fileName);
    mSound.setBuffer(this->buffer);
    eSound.setBuffer(this->buffer);
}

void Sound::Load(char* fileName){
    this->buffer.loadFromFile(fileName);
}

void Sound::start()
{
    mSound.play();
}

void Sound::stop()
{
    mSound.stop();
}

void Sound::setVolume(float vol)
{
    mSound.setVolume(vol);
}

void Sound::playFrom(float sec)
{
    mSound.setPlayingOffset(sf::seconds(sec));
    if(eSound.getStatus() == eSound.Playing){
        this->stopEcho();
        this->echo(this->eDel,this->eVol);
    }
}

void Sound::echo(float delay, float vol)
{
    if (eSound.getStatus() != eSound.Playing)
    {
        eSound.setPlayingOffset(mSound.getPlayingOffset());
        eSound.setVolume(vol);
        sf::sleep(sf::seconds(delay));
        eSound.play();
        this->eDel = delay;
        this->eVol = vol;
    }
}

void Sound::stopEcho()
{
    eSound.stop();
}

float Sound::getVolume()
{
    return mSound.getVolume();
}

float Sound::getCurrentPlay()
{
    return mSound.getPlayingOffset().asSeconds();
}

int Sound::getStatus()
{
    if(mSound.getStatus() == mSound.Playing)
    {
        return 1;
    }
    else
        return 0;
}

int Sound::getEchoStatus()
{
    if(eSound.getStatus() == eSound.Playing)
    {
        return 1;
    }
    else
        return 0;
}
Sound::~Sound()
{
    //dtor
}
