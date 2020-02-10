
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <ode/ode.h>
#include <irrlicht.h>

#include "info.h"
#include "level.h"

Info info;

/*
 * Main()
 */
int main()
{    
    // get various handles from irrlicht
    IrrlichtDevice *device = info.set();
    
    // don't want same level each time...or do we?
    ITimer *timer = info.timer;
    srand(timer->getRealTime());
    
    Level *lev = new Level;
    lev->Start(&info, 1);
	
    // main loop
    u32 time_start = timer->getRealTime();
    float delta_t;
    
    while(device->run())
    {
        if (device->isWindowActive())
        {
            // timing... IF time passed >= Frames Per Second Fraction THEN display the next frame
            if ( (timer->getRealTime() - time_start >= 1000.0 / 60.0) && device->isWindowActive())
            {
                // calculate change in time, used for physics and animation
                delta_t = (float)(timer->getRealTime() - time_start) / 1000.0f;
                
                lev->Refresh(delta_t);
                
                time_start = timer->getRealTime();
            }
        }
    }
    
    lev->Finish();
    
    // close ODE
    dCloseODE();
    
    return 0;
}


