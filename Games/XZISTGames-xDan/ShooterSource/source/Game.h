
#ifndef __Game_h
#define __Game_h

#include "TargetManager.h"
#include <irrlicht.h>
//#include <irrKlang.h>
#include "AttributeBarManager.h"
#include <vector>

#define SCORE_FILE "woo.yay"

using namespace irr;

class Game : public IEventReceiver
{
public:
    Game(IrrlichtDevice *);
    ~Game();
    void initNew();
    bool run();
    bool OnEvent(const SEvent&);

private:
    TargetManager *tm;

    IrrlichtDevice *device;
    scene::ISceneManager *smgr;
    video::IVideoDriver *driver;
    gui::IGUIEnvironment *guienv;
//    audio::ISoundEngine *sengine;

    scene::IAnimatedMeshSceneNode *wallsSceneNode;
//    audio::ISound *sMusic;

    bool finished; // Game Over?
    u32 finishTime;

    // scoring
    gui::IGUIStaticText *scoreText;
    int score;

    // game "status"
    abm::AttributeBarManager *abm;
    int percentFull; // how full the "status" bar is

    // shell display
    const static int maxShells = 8;
    int shellCount;
    std::vector <gui::IGUIImage *> shellImages;

    // muzzle flash
    gui::IGUIImage *gunFlash;

    // end
    std::vector <gui::IGUIElement *> endTexts;

    // start
    bool started;
    gui::IGUIImage *startImg;

    // methods
    void showFirstScreen();
    void hideFirstScreen();
    void restart();
    void start();
    void createHUD();
    void removeHUD();
    void createGunFlash();
    void removeGunFlash();
    void shoot();
    void bloodSplat(core::vector3df, video::ITexture *);

    void stopMusic();
    void setMusic(char *);
    int loadScore();
    void saveScore();

    void initRTTs();
    video::ITexture *wallRTT1, *wallRTT2;
    video::ITexture *groundRTT1, *groundRTT2;
};

#endif
