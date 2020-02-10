
#ifndef __Target_h
#define __Target_h

#include <irrlicht.h>
using namespace irr;

#include "Effects.h"
//#include <irrKlang.h>
#include <vector>

// for blood splatters etc...
struct Splatter
{
    core::vector3df pos;
    video::ITexture *texture;
};

class Target : public Effects
{
public:
    Target(IrrlichtDevice *);
    ~Target();
    bool update(double);
    bool shoot(core::line3df);
    int takeScore();
    bool takeUnhitFlag();
    int takeShellsGiven();
    int takeFullnessIncrease();
    std::vector <Splatter> getNewSplatters();

protected:
    void emptySceneNode();
    void removeSoon();
//    audio::ISoundEngine *sengine;
    core::vector3df vel;
    core::vector3df acc;
    core::vector3df rvel;
    core::vector3df bbExtent;
    double age;     // in seconds, incremented in update
    bool inactive;  // may have been shot, or destroyed itself...
    bool removable; // completely over and removable by manager
    int score;
    bool failedToHit;   // set when player fails to hit target (and target disappears)
                        // should only be set ONCE!!
    int shellsGiven;
    int fullnessIncrease;
    std::vector <Splatter> splatters;

private:
    double removeSoonAge;
    virtual void secondUpdate(double);
    void hit(core::line3df);
    /*
     * Main score function - all other incrementing of score variable is for BONUS points!!
     */
    virtual void hitScore() = 0;
    virtual void hitEffects(core::line3df) = 0;
};

#endif
