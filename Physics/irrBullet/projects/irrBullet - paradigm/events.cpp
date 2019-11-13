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

int Cevents::init()
{
        myevents->setRunning(true);

        return 0;
};

void Cevents::events()
{
    if (myevents->getRunning())
    {
        if (myevents->getEventCount()!=0)
        {
               // Load a map
               if (myevents->getEvent()==0)
               {
                    mymap->loadlevel();
               }

                //Create the character
                else if (myevents->getEvent()==1)
                {
                    mychar->loadchar();
                }

                //Main Menu
                else if (myevents->getEvent()==2)
                {
                    if (mygui->mainmenu)
                    {
                        mygui->mainmenu = false;
                        mygui->quitbutton->setVisible(false);
                    }

                    else
                    {
                         mygui->mainmenu = true;
                         mygui->quitbutton->setVisible(true);
                    }


                }
        myevents->rmvEvent();
        }


    }
};
