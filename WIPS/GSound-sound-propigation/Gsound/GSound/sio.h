#ifndef SOUND_H
#define SOUND_H
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

class Sound
{
    public:
        Sound(char *);
        void Load(char*);
        void setVolume(float);
        void playFrom(float);
        void start();
        void stop();
        void echo(float delay = 0.1f, float vol = 20);
        void stopEcho();

        float getVolume();
        float getCurrentPlay();
        int getStatus();
        int getEchoStatus();

        virtual ~Sound();
    protected:
    private:
        sf::SoundBuffer buffer;
        sf::Sound mSound;
        sf::Sound eSound;
        float eVol;
        float eDel;

};

#endif // SOUND_H
