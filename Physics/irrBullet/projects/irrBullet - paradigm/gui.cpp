#include "main.h"
class Cparadigm;
class Cevents;
class Cmap;
class Cchar;
class Cgui;

static Cparadigm *instance = &Cparadigm::Instance();
static Cevents *myevents = &Cevents::Instance();
static Cmap *mymap = &Cmap::Instance();
static Cchar *mychar = &Cchar::Instance();
static Cgui *mygui = &Cgui::Instance();


int Cgui::init()
{
        mygui->setRunning(true);
        mygui->quitbutton = instance->guienv->addButton(rect<s32>(10,240,110,240 + 32), 0, _QUITGAMEBUTTON,
                L"Quit", L"Exits Program");
        mygui->quitbutton->setVisible(false);
        return 0;
};



