#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define _LEVELGEO 100

#define _QUITGAMEBUTTON 101

#include <iostream>
#include <vector>
#include <string.h>
#include <irrlicht.h>
#include <irrBullet.h>
#include "driverChoice.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Cparadigm
{
    public:
     static Cparadigm& Instance()
      {
        static Cparadigm singleton;
        return singleton;
      }

    int init ();
    int step ();

    void setRunning (bool run) {running = run;};
    bool getRunning () { return running;};

    char * runningPath;

    IrrlichtDevice * device;
    IVideoDriver* driver;
	ISceneManager* smgr;
	ISceneManager* mymenu;
	IGUIEnvironment* guienv;

    IEventReceiver * rec;

    // Other non-static member functions
    private:
    bool running;

    Cparadigm() {};                               // Private constructor
    Cparadigm(const Cparadigm&);                 // Prevent copy-construction
    Cparadigm& operator=(const Cparadigm&);      // Prevent assignment
 };

class Cevents
{
    public:
     static Cevents& Instance()
      {
        static Cevents singleton;
        return singleton;
      }

    int init ();
    void events ();

    void setRunning (bool run) {running = run;};
    bool getRunning () { return running;};

    void addEvent (int command) { eventlist.push_back(command); };
    void rmvEvent () {eventlist.pop_back();};
    int  getEvent () {return eventlist.back();};
    int  getEventCount () {return eventlist.size();};

    void loadchar ();
    bool loadChar;


    // Other non-static member functions
    private:

    bool running;
    vector <int> eventlist;

    Cevents() {};                            // Private constructor
    Cevents(const Cevents&);                 // Prevent copy-construction
    Cevents& operator=(const Cevents&);      // Prevent assignment
 };


class Cmap
{
    public:
     static Cmap& Instance()
      {
        static Cmap singleton;
        return singleton;
      }

    int init ();

    void setRunning (bool run) {running = run;};
    bool getRunning () { return running;};

    void loadlevel ();
    bool loadLevel;

    irrBulletWorld * world;

    // Other non-static member functions
    private:

    bool running;

    Cmap() {};                            // Private constructor
    Cmap(const Cmap&);                 // Prevent copy-construction
    Cmap& operator=(const Cmap&);      // Prevent assignment
 };

class Cchar
{
    public:
     static Cchar& Instance()
      {
        static Cchar singleton;
        return singleton;
      }

    int init ();

    void setRunning (bool run) {running = run;};
    bool getRunning () { return running;};

    void loadchar ();
    bool loadChar;

    // Other non-static member functions
    private:

    bool running;

    Cchar() {};                            // Private constructor
    Cchar(const Cchar&);                 // Prevent copy-construction
    Cchar& operator=(const Cchar&);      // Prevent assignment
 };



class Cgui
{
    public:
     static Cgui& Instance()
      {
        static Cgui singleton;
        return singleton;
      }

    int init ();

    void setRunning (bool run) {running = run;};
    bool getRunning () { return running;};

    IGUIButton * quitbutton;

    bool mainmenu;

    // Other non-static member functions
    private:

    bool running;

    Cgui() {};                            // Private constructor
    Cgui(const Cevents&);                 // Prevent copy-construction
    Cgui& operator=(const Cgui&);      // Prevent assignment
 };




class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver()  { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			//IGUIEnvironment* env = Cparadigm::Instance().guienv;

			switch(event.GUIEvent.EventType)
			{
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case _QUITGAMEBUTTON:
                        Cparadigm::Instance().device->closeDevice();
					return true;

				default:
					return false;
				}
				break;

			default:
				break;
			}
		}

		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_ESCAPE: // Main Menu
                Cevents::Instance().addEvent(2);

				return true;
			case irr::KEY_KEY_P:

				return true;
			case irr::KEY_KEY_D:

				return true;
			case irr::KEY_KEY_S:

				return true;
			default:
				break;
			}
		}

		return false;
	}

private:

};
#endif // MAIN_H_INCLUDED
