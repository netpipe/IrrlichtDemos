#include "mainloop.h"

#define delayCount 100

int mainLoop()
{
    log << std::string("Entering main loop");

    while(1)
    {
        #ifdef WIN32
            timeBeginPeriod(1);
            Sleep(delayCount);
            timeEndPeriod(1);
        #else
            usleep(1000 * delayCount);
        #endif
    }
    return 0;
}

void handleConnection()
{

}

void handleAI()
{

}
