// FunCollisionDemo -- Demo application for "FunCollision" -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or main.cpp

#ifndef CGAMEINPUT_H_INCLUDED
#define CGAMEINPUT_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


namespace GAME
{

//! CGameInput class
class CGameInput : public IEventReceiver
{
    public:
        //! constructor
        CGameInput(IrrlichtDevice* Device);


        //! OnEvent
        virtual bool OnEvent(const SEvent& event);

        //! update
        /// allowes relative states
        virtual void update();


        //! set functions
        void setLockMouse(bool Lock) { lockMouse = Lock; }


        //! get functions: keys
        bool getKeyDown(EKEY_CODE keyCode) const { return keys[keyCode]; }
        bool getKeyReleased(EKEY_CODE keyCode) const { return !keys[keyCode] && keysPrev[keyCode]; }
        bool getKeyPressed(EKEY_CODE keyCode) const { return keys[keyCode] && !keysPrev[keyCode]; }

        bool getShiftDown() const { return shift; }
        bool getShiftReleased() const { return !shift && shiftPrev; }
        bool getShiftPressed() const { return shift && !shiftPrev; }

        bool getControlDown() const { return control; }
        bool getControlReleased() const { return !control && controlPrev; }
        bool getControlPressed() const { return control && !controlPrev; }


        //! get functions: mouse
        bool getLMBDown() const { return LMB; }
        bool getLMBReleased() const { return !LMB && LMBPrev; }
        bool getLMBPressed() const { return LMB && !LMBPrev; }

        bool getMMBDown() const { return MMB; }
        bool getMMBReleased() const { return !MMB && MMBPrev; }
        bool getMMBPressed() const { return MMB && !MMBPrev; }

        bool getRMBDown() const { return RMB; }
        bool getRMBReleased() const { return !RMB && RMBPrev; }
        bool getRMBPressed() const { return RMB && !RMBPrev; }

        f32 getWheel() const { return wheel; }

        void getAbsMousePos(s32& X, s32& Y) const { X = mouseX; Y = mouseY; }
        void getRelMousePos(s32& X, s32& Y) const { X = relMouseX; Y = relMouseY; }

    private:
        // instances
        IrrlichtDevice* device;

        // keys
        bool keys[KEY_KEY_CODES_COUNT], keysPrev[KEY_KEY_CODES_COUNT];
        bool shift, shiftPrev, control, controlPrev;

        // mouse
        bool lockMouse, locked;
        core::position2di center;
        s32 mouseX, mouseY, relMouseX, relMouseY;
        bool LMB, MMB, RMB, LMBPrev, MMBPrev, RMBPrev;
        f32 wheel;

}; // end class CGameInput

} // end namespace GAME

#endif // CGAMEINPUT_H_INCLUDED
