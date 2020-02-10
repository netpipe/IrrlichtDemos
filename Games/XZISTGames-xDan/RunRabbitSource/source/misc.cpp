
#include <stdlib.h>
#include "misc.h"
//#include <irrKlang.h>

//using namespace audio;

//extern ISoundEngine *sengine;

//ISound *sMusic = NULL;

void stopMusic()
{
   // if (sMusic)
   // {
      //  sMusic->stop();
    //    sMusic->drop();
    //}
}

void setMusic(char *fileName)
{
    //stopMusic();
    //sMusic = sengine->play2D(fileName, true, false, true, ESM_NO_STREAMING);
}

int randSign()
{
    int i = rand()%2;
    if (i == 0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

f32 frandU() // unsigned 0.0 to 1.0
{
    return ((f32)(rand()%1000))/1000.0;
}

f32 frandS() // signed -1.0 to 1.0
{
    return ((f32)(rand()%2000-1000))/1000.0;
}

