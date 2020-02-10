
#ifndef __TargetManager_h
#define __TargetManager_h

#include <vector>
#include "Target.h"
#include <irrlicht.h>
//#include <irrKlang.h>

using namespace irr;

class TargetManager
{
public:
    TargetManager(IrrlichtDevice *);
    ~TargetManager();
    void update(double,double);
    bool shootTargets(core::line3df);
    int getScoreIncrements();
    int takeUnhitCount();
    int takeShellsGained();
    int takeFullnessIncreases();
    std::vector <Splatter> getNewSplatters();

private:
    IrrlichtDevice *device;
    scene::ISceneManager *smgr;
//    audio::ISoundEngine *sengine;

    std::vector <Target *> targets;
    f32 level;
    int mode;

    // time stepper (possibly only temp method of controlling Target creation - may desire waves in the future)
    bool timeStepped(int, double, double, double);
    std::vector <double> lastTargetTimes;
};

#endif
