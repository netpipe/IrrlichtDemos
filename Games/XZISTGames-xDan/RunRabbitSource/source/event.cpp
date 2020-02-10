
//#define IS_DEMO

#include "event.h"
#include <irrlicht.h>
#include "rabbit.h"
#include <stdlib.h>
#include "platforms.h"
#include "text.h"
#include "carrots.h"
#include "stringaling.h"
#include "misc.h"
#include "launcher/DeviceSettings.h"

#define SCORE_FILE "score.dat"

using namespace irr;

extern IrrlichtDevice *device;
extern scene::ISceneManager *smgr;
extern video::IVideoDriver *driver;
extern gui::IGUIEnvironment *guienv;
extern core::vector3df firstCameraPosition;
extern core::vector3df firstCameraTarget;
extern int carrotsCollected;

int state = 0;

Text *startBlinkText;

gui::IGUIStaticText *tEndCongrats;
gui::IGUIStaticText *tEndScore;
gui::IGUIStaticText *tEndCarrots;
Text *textNewHighScore;
gui::IGUIStaticText *tLastHighScore;

#ifdef IS_DEMO
enum {B_BUY, B_NOBUY};

extern bool OPEN_WEBPAGE_ON_EXIT;
#endif

class eventReceiver : public IEventReceiver
{
public:

    virtual bool OnEvent(const SEvent& e)
    {
#ifdef IS_DEMO
        if (e.EventType == EET_GUI_EVENT)
        {
            s32 id = e.GUIEvent.Caller->getID();

            switch(e.GUIEvent.EventType)
            {
                case gui::EGET_BUTTON_CLICKED:
                    if (id == B_BUY)
                    {
                        device->closeDevice();
                        OPEN_WEBPAGE_ON_EXIT = true;
                        return true;
                    }
                    if (id == B_NOBUY)
                    {
                        device->closeDevice();
                        return true;
                    }
                    break;
            }
        }
#endif
        if (e.KeyInput.Key == KEY_ESCAPE && e.KeyInput.PressedDown)
        {
#ifdef IS_DEMO
            if (state != IN_DEMOSCREEN)
            {
                if (state == IN_ENDSCREEN)
                {
                    tEndCongrats->remove();
                    tEndScore->remove();
                    tEndCarrots->remove();
                    if (textNewHighScore) removeText(textNewHighScore);
                    tLastHighScore->remove();
                }
                removeAllTexts();

                state = IN_DEMOSCREEN;

                removePlatforms();
                removeRabbit();
                removeCarrotDisplay();

                smgr->getActiveCamera()->setInputReceiverEnabled(false);
                device->getCursorControl()->setVisible(true);

                int bWidth1 = 210;
                int bWidth2 = 260;
                int bHeight = 50;
                int bGap = 10;
                int bLow = 20;
                int centreX = screenHW()-25;

                video::ITexture *tex = driver->getTexture("media/demo.png");
                core::dimension2d<s32> dim = tex->getSize();
                gui::IGUIImage *img = guienv->addImage(
                    core::rect<s32>(
                        screenHW()-dim.Width/2, screenHH()-dim.Height/2-50,
                        screenHW()+dim.Width/2, screenHH()+dim.Height/2-50
                        )
                    );
                img->setImage(tex);

                guienv->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,255,77,0));
                guienv->getSkin()->setColor(gui::EGDC_3D_FACE, video::SColor(255,255,255,100));
                guienv->getSkin()->setColor(gui::EGDC_3D_DARK_SHADOW, video::SColor(255,150,150,60));
                guienv->getSkin()->setFont(guienv->getFont("media/blinkfont.xml"));

                device->getGUIEnvironment()->addButton(
                    core::rect<s32>(
                        centreX-bGap-bWidth1, screenHH()*2-bLow-bHeight,
                        centreX-bGap, screenHH()*2-bLow
                    ),
                    0, B_BUY, L"Buy Now");

                device->getGUIEnvironment()->addButton(
                    core::rect<s32>(
                        centreX+bGap, screenHH()*2-bLow-bHeight,
                        centreX+bGap+bWidth2, screenHH()*2-bLow
                    ),
                    0, B_NOBUY, L"No, thanks");

                setMusic("media/demo.ogg");
            }
            else
            {
                device->closeDevice();
            }
#else
            device->closeDevice();
#endif
            return true;
        }
        if (
            (
            (e.EventType == EET_KEY_INPUT_EVENT && e.KeyInput.PressedDown)
            ||
            (e.EventType == EET_MOUSE_INPUT_EVENT && (
            e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN
            || e.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN
            || e.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN))
            )
            && state != IN_DEMOSCREEN // otherwise this prevents GUI button clicking, for some reason
            )
        {
            if (state == IN_GAME)
            {
                jumpRabbit();
            }
            else if (state == IN_STARTSCREEN)
            {
                removeText(startBlinkText);

                state = IN_GAME;
                printf("NEW GAME %i\n", rand());
                createRabbit();
                addCarrotDisplay();

                setMusic("media/ingame_loop.ogg");
            }
            else if (state == IN_ENDSCREEN)
            {
                printf("START SCREEN %i\n", rand());

                tEndCongrats->remove();
                tEndScore->remove();
                tEndCarrots->remove();
                if (textNewHighScore) removeText(textNewHighScore);
                tLastHighScore->remove();

                removeCarrotDisplay();
                resetCarrotsCollected();
                startScreen();
            }
            return true;
        }
        return false;
    }
} eventReceiver;

int getHighScore()
{
    int score;
    FILE *f = fopen(SCORE_FILE, "rb");
    if (f)
    {
        fread(&score, sizeof(int), 1, f);// == sizeof(int))
        //{
            fclose(f);
            return score;
        //}
        //printf("Warning: score file corrupted?\n");
        //fclose(f);
    }
    return 0;
}

void saveHighScore(int score)
{
    FILE *f = fopen(SCORE_FILE, "wb");
    if (f)
    {
        fwrite(&score, sizeof(int), 1, f);
        fclose(f);
    }
    else
    {
        printf("Warning: error saving high score.\n");
    }
}

extern f32 startHeight;

void startScreen()
{
    state = IN_STARTSCREEN;
    device->setEventReceiver(&eventReceiver);

    resetPlatforms();
    startBlinkText = blinkMessage(L"< press to play >", screenHH(), 0);

    smgr->getActiveCamera()->setPosition(firstCameraPosition+core::vector3df(0,startHeight,0));
    smgr->getActiveCamera()->setTarget(firstCameraTarget+core::vector3df(0,startHeight,0));

    setMusic("media/main_loop.ogg");
}

void endScreen()
{
    // remove any messages like "administering speed"
    removeAllTimedTexts();
    removeCarrotDisplay();

    state = IN_ENDSCREEN;

    int oldScore = getHighScore();

    int y = screenHH();
    int gap = 100;

    if (carrotsCollected > oldScore)
    {
        y -= 200;
    }
    else
    {
        y -= 150;
    }

    tEndCongrats = staticText(L"CONGRATULATIONS", screenHW(),y, 4, "media/bigfont.xml");
    y += gap;
    tEndScore = staticText(stringify(carrotsCollected).c_str(), screenHW(),y, 4, "media/blinkfont.xml");
    y += gap;
    tEndCarrots = staticText(L"Carrots", screenHW(),y, 4, "media/blinkfont.xml");
    y += gap;

    if (carrotsCollected > oldScore)
    {
        textNewHighScore = blinkMessage(L"New High Score!!!", y, 0);
        y += gap;
        saveHighScore(carrotsCollected);
    }
    else
    {
        textNewHighScore = NULL;
    }

    core::stringw str = L"Last High Score: ";
    str += stringify(oldScore).c_str();
    tLastHighScore = staticText(str.c_str(), screenHW(),y, 4, "media/blinkfont.xml");

    setMusic("media/end_loop.ogg");
}


