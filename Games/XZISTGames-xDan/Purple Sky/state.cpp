
#include "state.h"
#include "player.h"
#include "platforms.h"
#include <irrKlang.h>
#include "stringaling.h"

using namespace audio;

extern IrrlichtDevice *device;
extern ISoundEngine *sengine;

int state;
int score_nutCount;
s32 mouseDX = 0, mouseDY = 0;

gui::IGUIImage *scoreImg = NULL; // 2d nut image, indicating score
gui::IGUIStaticText *scoreText = NULL;

gui::IGUIStaticText *staticText(core::stringw str, int x, int y, char *fontFile)
{
    gui::IGUIFont *font = device->getGUIEnvironment()->getFont(fontFile);
    
    if (!font)
    {
        printf("Error, could not load font: %s\n", fontFile);
        return NULL;
    }
    
    core::dimension2d<s32> dim = font->getDimension(str.c_str());
    core::rect<s32> rec;
    
    rec = core::rect<s32>(x,y-dim.Height/2, x+dim.Width,y+dim.Height/2);
    
    gui::IGUIStaticText *text = device->getGUIEnvironment()->addStaticText(str.c_str(), rec, false,false);
    text->setOverrideFont(font);
    text->setOverrideColor(video::SColor(255,220,100,255));
    
    return text;
}

gui::IGUIStaticText *tempText = NULL;
void setTempText(char *text)
{
    if (tempText) tempText->remove();
    tempText = staticText(text, 60,120, "media/scorefont.xml");
}
void removeTempText()
{
    if (tempText) tempText->remove();
    tempText = NULL;
}

ISound *sMusic = NULL;
void stopMusic()
{
    if (sMusic)
    {
        sMusic->stop();
        sMusic->drop();
        sMusic = NULL;
    }
}
void setMusic(char *fileName)
{
    stopMusic();
    //if (fileName) sMusic = sengine->play2D(fileName, true, false, true, ESM_NO_STREAMING);
}

class pausedEventReceiver : public IEventReceiver
{
public:
    virtual bool OnEvent(SEvent e)
    {
        if (e.EventType == EET_KEY_INPUT_EVENT)
        {
            if (e.KeyInput.PressedDown)
            {
                switch (e.KeyInput.Key)
                {
                case KEY_ESCAPE:
                    endGame();
                    startScreen();
                    return true;
                }
            }
        }
        else if (e.EventType == EET_MOUSE_INPUT_EVENT)
        {
            if (e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
            {
                continueGame();
                return true;
            }
        }
        return false;
    }
} pausedEventReceiver;

class ingameEventReceiver : public IEventReceiver
{
public:

    virtual bool OnEvent(SEvent e)
    {
        if (e.EventType == EET_KEY_INPUT_EVENT)
        {
            if (e.KeyInput.PressedDown)
            {
                switch (e.KeyInput.Key)
                {
                case KEY_ESCAPE:
                    pauseGame();
                    return true;
                case KEY_SPACE:
                    //playerPlatformJump();
                    return true;
                case KEY_LEFT:
                    turnCamera(-1);
                    return true;
                case KEY_RIGHT:
                    turnCamera(1);
                    return true;
                case KEY_KEY_A:
                    turnCamera(-1);
                    return true;
                case KEY_KEY_D:
                    turnCamera(1);
                    return true;
                case KEY_KEY_1:
                    cameraType = CAMERA_FOLLOW;
                    return true;
                case KEY_KEY_2:
                    cameraType = CAMERA_PIVOT;
                    return true;
                }
            }
            else
            {
                switch (e.KeyInput.Key)
                {
                case KEY_LEFT:
                    turnCamera(0);
                    return true;
                case KEY_RIGHT:
                    turnCamera(0);
                    return true;
                case KEY_KEY_A:
                    turnCamera(0);
                    return true;
                case KEY_KEY_D:
                    turnCamera(0);
                    return true;
                }
            }
        }
        else if (e.EventType == EET_MOUSE_INPUT_EVENT)
        {
            mouseDX = e.MouseInput.X-100;
            mouseDY = e.MouseInput.Y-100;
            
            if (e.MouseInput.Event == EMIE_MOUSE_WHEEL)
            {
                //mouseWheel = e.MouseInput.Wheel;
            }
            else if (e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN
                || e.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN
                || e.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
            {
                playerPlatformJump();
            }
            return true;
        }
        return false;
    }
} ingameEventReceiver;

class startEventReceiver : public IEventReceiver
{
public:
    virtual bool OnEvent(SEvent e)
    {
        if (e.EventType == EET_KEY_INPUT_EVENT)
        {
            if (e.KeyInput.PressedDown)
            {
                switch (e.KeyInput.Key)
                {
                case KEY_ESCAPE:
                    device->closeDevice();
                    return true;
                }
            }
        }
        else if (e.EventType == EET_MOUSE_INPUT_EVENT)
        {
            if (e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
            {
                startGame();
                return true;
            }
        }
        return false;
    }
} startEventReceiver;

class gameoverEventReceiver : public IEventReceiver
{
public:
    virtual bool OnEvent(SEvent e)
    {
        if (e.EventType == EET_KEY_INPUT_EVENT)
        {
            if (e.KeyInput.PressedDown)
            {
                switch (e.KeyInput.Key)
                {
                case KEY_ESCAPE:
                    endGame();
                    startScreen();
                    return true;
                }
            }
        }
        else if (e.EventType == EET_MOUSE_INPUT_EVENT)
        {
            if (e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
            {
                endGame();
                startScreen();
                return true;
            }
        }
        return false;
    }
} gameoverEventReceiver;

void pauseGame()
{
    setTempText("(GAME PAUSED, click to resume)");
    state = GAME_PAUSED;
    device->setEventReceiver(&pausedEventReceiver);
}

void continueGame()
{
    removeTempText();
    
    state = IN_GAME;
    device->setEventReceiver(&ingameEventReceiver);
}

void setScoreDisplayCount(int c)
{
    if (scoreText) scoreText->remove();
    scoreText = staticText(("x "+stringify(c)).c_str(), 120,65, "media/scorefont.xml");
}

void startGame()
{
    removeTempText();
    
    // score
    score_nutCount = 0;
    video::ITexture *tex = device->getVideoDriver()->getTexture("media/scorenut.png");
    core::dimension2d<s32> dim = tex->getSize();
    scoreImg = device->getGUIEnvironment()->addImage(
        tex,
        core::position2d<s32>(30,30),
        true
        );
    setScoreDisplayCount(10000);
    
    createPlatforms();
    createPlayer();
    device->setEventReceiver(&ingameEventReceiver);
    setMusic("media/Armageddon Loops_120BPM_50LOOP_155_01.ogg");
    state = IN_GAME;
}

void endGame()
{
    removePlatforms();
    removePlayer();
    
    if (scoreImg) scoreImg->remove();
    if (scoreText) scoreText->remove();
    scoreImg = NULL;
    scoreText = NULL;
}

void startScreen()
{
    device->setEventReceiver(&startEventReceiver);
    state = IN_STARTSCREEN;
    setMusic(NULL);
    
    setTempText("(start screen, click mouse to continue)");
}

void gameOver()
{
    setTempText("(GAME OVER, click mouse to continue)");
    
    state = GAME_OVER;
    device->setEventReceiver(&gameoverEventReceiver);
    setMusic("media/Armageddon Loops_120BPM_50LOOP_155_02.ogg");
}
