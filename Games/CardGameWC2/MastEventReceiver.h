#ifndef MAST_EVENT_RECEIVER_H_INCLUDED
#define MAST_EVENT_RECEIVER_H_INCLUDED

/// ==================================================================================================
/// MastEventReceiver code is © (Copyright) Robert E. Demarest, AKA Mastiff or Mastiff Odit
/// This file may be used in any non-commercial or commercial project as long as the following conditions are met:
/// You may not claim this code as being your own.
/// You may not use this code for any harmful, malicious or otherwise damaging programs.
///
/// This is version 1.2a of the class.
/// This class is designed for use with the Irrlicht Engine, it was written for version 1.3 of the engine.
/// ==================================================================================================

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To use this Class just add  #include "MastEventReceiver.cpp"  to the end of your includes list. (or add the class in-line into your program)
// Then create an instance of it like so: MastEventReceiver eventReceiver;
// Then call the initialization fucntion like so: eventReceiver.init();
// Then inside your Main Game Loop place "eventReceiver.endEventProcess();" in the beginning of your game loop, before anything -
// that would require input, then put "eventReceiver.startEventProcess();" at the very end of your Main Game Loop.
// yeah I know it's confusing, but it makes alot more sense in the internals of the class.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

// Change this to the path where your Irrlicht Header Files are.
#include "cardgame.h"
#include <irrlicht.h>
/// ==============================
/// MastEventReceiver
/// ==============================

class MastEventReceiver : public irr::IEventReceiver
{
protected:
    // Enumeration for UP, DOWN, PRESSED and RELEASED key states. Also used for mouse button states.
    enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};

    // Enumeration for Event Handling State.
    enum processStateENUM {STARTED, ENDED};

    // Mouse button states.
    keyStatesENUM mouseButtonState[2]; //Left(0), Middle(1) and Right(2) Buttons.

    // Keyboard key states.
    keyStatesENUM keyState[irr::KEY_KEY_CODES_COUNT];

    // Mouse X/Y coordinates and Wheel data.
    struct Mouse
    {
        irr::s32 X;
        irr::s32 Y;
        irr::f32 wheel; //wheel is how far the wheel has moved
    };

    Mouse mouse;

    processStateENUM processState; // STARTED = handling events, ENDED = not handling events

    virtual bool OnEvent(const irr::SEvent &event);

    //////////////////////
    // Public functions
    //////////////////////
public:

    irr::f32 mouseWheel();

    irr::s32 mouseX();

    irr::s32 mouseY();

    bool leftMouseReleased();

    bool leftMouseUp();

    bool leftMousePressed();

    bool leftMouseDown();

    bool middleMouseReleased();

    bool middleMouseUp();

    bool middleMousePressed();

    bool middleMouseDown();

    bool rightMouseReleased();

    bool rightMouseUp();

    bool rightMousePressed();

    bool rightMouseDown();

    bool keyPressed(char keycode);

    bool keyDown(char keycode);

    bool keyUp(char keycode);

    bool keyReleased(char keycode);


    // This is used so that the Key States will not be changed during execution of your Main game loop.
    // Place this at the very START of your Main Loop
    void endEventProcess();

    // This is used so that the Key States will not be changed during execution of your Main game loop.
    // Place this function at the END of your Main Loop.
    void startEventProcess();

    void init();
};
/// ==========================================
/// END OF MastEventReceiver
/// ==========================================

#endif
