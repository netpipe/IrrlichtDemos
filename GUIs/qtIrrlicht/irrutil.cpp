#ifdef IRRLICHT
#include "irrutil.h"
#include "irrlicht.h"

#include <QKeyEvent>

using namespace irr;

irr::SKeyMap* getCameraKeyMap(){
    static irr::SKeyMap keyMap[10];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;
    keyMap[8].Action = EKA_JUMP_UP;
    keyMap[8].KeyCode = KEY_SPACE;
    keyMap[9].Action = EKA_CROUCH;
    keyMap[9].KeyCode = KEY_CONTROL;
    return keyMap;
}

SIrrlichtKey convertToIrrlichtKey(int key){
    SIrrlichtKey irrKey;
    irrKey.code = (irr::EKEY_CODE)(0);
    irrKey.ch = (wchar_t)(0);

    // Letters A..Z and numbers 0..9 are mapped directly
    if ( (key >= Qt::Key_A && key <= Qt::Key_Z) || (key >= Qt::Key_0 && key <= Qt::Key_9) )
    {
        irrKey.code = (irr::EKEY_CODE)( key );
        irrKey.ch = (wchar_t)( key );
    }
    else
    {
        // map keys individually
        switch( key )
        {
        case Qt::Key_Up:
            irrKey.code = irr::KEY_UP;
            break;

        case Qt::Key_Down:
            irrKey.code = irr::KEY_DOWN;
            break;

        case Qt::Key_Left:
            irrKey.code = irr::KEY_LEFT;
            break;

        case Qt::Key_Right:
            irrKey.code = irr::KEY_RIGHT;
            break;
        case Qt::Key_Space:
            irrKey.code = irr::KEY_SPACE;
            break;
        default:
            break;
        }
    }
    return irrKey;
}
#endif
