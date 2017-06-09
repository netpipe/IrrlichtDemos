#ifndef CGAMETIMER_H_INCLUDED
#define CGAMETIMER_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


//! CGameTimer class
/// timeing, thread delay
class CGameTimer
{
    public:
        //! constructor
        CGameTimer(IrrlichtDevice* Device)
        {
            device = Device;

            restarted = true;
            currentTime = lastTime = elapsedTime = 0;
            deltaTime = 0.0f;
        }

        //! restartTimer
        void restartTimer() { restarted = true; }

        //! checkTimer
        /// checks if it's needed to restart the timer
        void checkTimer() { if (device && !device->isWindowActive()) restartTimer(); }

        //! updateTimer
        void updateTimer()
        {
            if (!device) return;

            if (restarted)
            {
                restarted = false;
                lastTime = device->getTimer()->getTime();
                elapsedTime = 0;
                deltaTime = 0.0f;
            }

            currentTime = device->getTimer()->getTime();
            elapsedTime = currentTime - lastTime;
            deltaTime = elapsedTime / 1000.0f;
            lastTime = currentTime;
        }

        //! threadDelay
        /// allows Windows to do other things too
        void threadDelay() { device->sleep(1, false); }

        //! get functions
        inline const u32 getCurrentTime() const { return currentTime; }
        inline const u32 getElapsedTime() const { return elapsedTime; }
        inline const float getDeltaTime() const { return deltaTime; }

    private:
        // instances
        IrrlichtDevice* device;

        // vars:
        bool restarted;
        u32 currentTime;
        u32 lastTime;
        u32 elapsedTime;
        float deltaTime;

}; // end class CGameTimer

#endif // CGAMETIMER_H_INCLUDED
