
#include <stdlib.h>
#include <string.h>
#include <irrlicht.h>
#include <debug.h>
#include <level.h>
#include <irrInfo.h>
#include <vector>

// class containing irrlicht pointers. and other stuff.
irrInfo info;

// current level
Level *lev = NULL;

// menu button IDs
enum {MAIN_NEW_GAME, MAIN_EXIT, MAIN_CONTINUE};

// converts a fraction of screen to an integer based on screen resolution.
rect<s32> rectScreen(f32 x1, f32 x2, f32 y1, f32 y2)
{
    return rect<s32>(
        (s32)(x1*info.Options.Screen.WindowSize.Width),
        (s32)(y1*info.Options.Screen.WindowSize.Height),
        (s32)(x2*info.Options.Screen.WindowSize.Width),
        (s32)(y2*info.Options.Screen.WindowSize.Height)
        );
}

/*
 * MainMenu
 * Creates buttons etc, sets event reciever
 * has methods for showing/hiding itself
 */
class MainMenu
{
public:
    MainMenu()
    {
        visible = false;
    }
    void Show(IEventReceiver *eventReciever)
    {
        Hide();
        // set event reciever
        info.device->setEventReceiver(eventReciever);
        // set mouse visible
        info.cursor->setVisible(true);
        // add menu buttons
        if (info.gamePaused)
        {
            buttons.push_back(info.guienv->addButton(rectScreen(0.42,0.58,0.42,0.46), 0, MAIN_CONTINUE, L"Continue..."));
        }
        buttons.push_back(info.guienv->addButton(rectScreen(0.42,0.58,0.48,0.52), 0, MAIN_NEW_GAME, L"New Game"));
        buttons.push_back(info.guienv->addButton(rectScreen(0.42,0.58,0.54,0.58), 0, MAIN_EXIT, L"Quit"));
        visible = true;
    }
    void Hide()
    {
        // remove all buttons
        for (i = 0; i < buttons.size(); i ++)
        {
            buttons[i]->remove();
        }
        buttons.clear();
        visible = false;
    }
    bool Visible()
    {
        return visible;
    }
    ~MainMenu()
    {
        Hide();
    }
private:
    int i;
    bool visible;
    std::vector <IGUIButton *> buttons;
} MainMenu;


/*
 * Main menu event reciever
 */
class MenuEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
        if (event.EventType == EET_KEY_INPUT_EVENT)
        {
            switch (event.KeyInput.Key)
            {
                case KEY_ESCAPE:
                    /*
                    if (!event.KeyInput.PressedDown && !EscapeLetGo)
                    {
                        EscapeLetGo = true;
                        return true;
                    }
                    */
                    if (info.gamePaused && event.KeyInput.PressedDown)// && EscapeLetGo)
                    {
                        lev->unPause();
                        MainMenu.Hide();
                    }
                    return true;
            }
        }
        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment *env = info.guienv;
            switch(event.GUIEvent.EventType)
            {
                case EGET_BUTTON_CLICKED:
                    // start a new game, destroying old level if present
                    if (id == MAIN_NEW_GAME)
                    {
                        // destroy old level
                        if (lev)
                        {
                            lev->Finish();
                            delete lev;
                            lev = NULL;
                        }
                        // hide main menu
                        MainMenu.Hide();
                        // start test level
                        lev = new Level;
                        lev->Start(&info);
                        return true;
                    }
                    // unpause the game and continue
                    if (id == MAIN_CONTINUE)
                    {
                        lev->unPause();
                        MainMenu.Hide();
                        return true;
                    }
                    // exit game
                    if (id == MAIN_EXIT)
                    {
                        info.device->closeDevice();
                        return true;
                    }
                    break;
            }
        }

        return false;
    }
} MenuEventReceiver;


/*
 * Main()
 */
int main()
{
    debugClear();

    // get various handles from irrlicht
    IrrlichtDevice *device = info.set();
//    if (!info.driver)
//    {
//        say("Error: video driver not found.");
//        return 0;
//    }

    // don't want same level each time...or do we?
    ITimer *timer = info.timer;
    srand(timer->getRealTime());

    // start main menu
    //MainMenu.Show(&MenuEventReceiver);
    lev = new Level;
    lev->Start(&info);

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

                if (info.GlobalGameState.endLevelCheck())
                {
                    lev->Finish();
                    delete lev;
                    lev = NULL;
                    MainMenu.Show(&MenuEventReceiver);
                }

                if (lev && !info.gamePaused)
                {
                    // refresh the test level
                    lev->refresh(delta_t);
                }
                else
                {
                    if (!MainMenu.Visible())
                    {
                        MainMenu.Show(&MenuEventReceiver);
                    }
                    info.driver->beginScene(true, true, SColor(0,200,200,200));
                    info.smgr->drawAll();
                    info.guienv->drawAll();
                    info.driver->endScene();
                }

                time_start = timer->getRealTime();
            }
        }
        else
        {
            info.gamePaused = true;
        }
        device->sleep(1);
    }

    // close any levels
    if (lev) lev->Finish();

    // close ODE
    dCloseODE();

    say("Exit success.");

    return 0;
}


