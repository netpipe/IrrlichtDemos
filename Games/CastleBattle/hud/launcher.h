#ifndef LAUNCHER_H
#define LAUNCHER_H
#include <irrlicht.h>
#include <iostream>
#include "Application.h"
using namespace irr;
enum{ /*!< Enum of the GUI ID object on the launcher screen */
  GUI_ID_PLAYER1_NAME=0, /*!< PLAYER 1 GUI ID OBJECT (EDITBOX) */
  GUI_ID_PLAYER2_NAME, /*!< PLAYER  2 GUI ID OBJECT (EDITBOX) */
  GUI_ID_START_BUTTON /*!< START BUTTON GUI ID OBJECT */
};
/** Launcher screen GUI elements Event receiver */
class LauncherEventReceiver : public IEventReceiver{
 private:
  public:
    /** Number of elements to listen events. 3 object on GUI but i only need the start button */
    bool stateElement[3];
    /** virtual OnEvent method call whenever there are events.
    * @see ::KEYBOARD::Keyboard::OnEvent()
    */
    virtual bool OnEvent(const SEvent& event);
    ~LauncherEventReceiver();
};
/** Launcher start screen */
class Launcher{
  private:
    gui::IGUIEnvironment* gui;
    IrrlichtDevice* device;
    video::IVideoDriver* video;
    /** Start button */
    gui::IGUIButton* start;
    /** Player names */
  public:
    /** EditBox players names  */
    gui::IGUIEditBox* players[2];
    /** GUI event listener
    * @see ::LauncherEventReceiver
    * @see ::KEYBOARD::Keyboard
    */
    LauncherEventReceiver* listener;
    /** Clear all gui objects */
    void clear();
    /** Setup the main start screen
    * @param env IGUIEnvironment Irrlicht GUI Enviroment
    * @param device IrrlichtDevice Irrlicht device
    * @see HUD::HUD
    */
    Launcher(gui::IGUIEnvironment* env, IrrlichtDevice* device);
    /** Show winner screen */
    void endScreen(core::stringw* names, int winner);
    ~Launcher();
    /** Empty default constructor. Use the other*/
    Launcher();

};
#endif
