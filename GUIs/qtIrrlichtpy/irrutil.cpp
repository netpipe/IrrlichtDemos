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


    //std::map< std::string, EKEY_CODE > keydictionary;
//keydictionary["KEY_LBUTTON"] = KEY_LBUTTON;
//keydictionary["KEY_RBUTTON"] = KEY_RBUTTON;
//keydictionary["KEY_CANCEL"] = KEY_CANCEL;
//keydictionary["KEY_MBUTTON"] = KEY_MBUTTON;
//keydictionary["KEY_XBUTTON1"] = KEY_XBUTTON1;
//keydictionary["KEY_XBUTTON2"] = KEY_XBUTTON2;
//keydictionary["KEY_BACK"] = KEY_BACK;
//keydictionary["KEY_TAB"] = KEY_TAB;
//keydictionary["KEY_CLEAR"] = KEY_CLEAR;
//keydictionary["KEY_RETURN"] = KEY_RETURN;
//keydictionary["KEY_SHIFT"] = KEY_SHIFT;
//keydictionary["KEY_CONTROL"] = KEY_CONTROL;
//keydictionary["KEY_MENU"] = KEY_MENU;
//keydictionary["KEY_PAUSE"] = KEY_PAUSE;
//keydictionary["KEY_CAPITAL"] = KEY_CAPITAL;
//keydictionary["KEY_KANA"] = KEY_KANA;
//keydictionary["KEY_HANGUEL"] = KEY_HANGUEL;
//keydictionary["KEY_HANGUL"] = KEY_HANGUL;
//keydictionary["KEY_JUNJA"] = KEY_JUNJA;
//keydictionary["KEY_FINAL"] = KEY_FINAL;
//keydictionary["KEY_HANJA"] = KEY_HANJA;
//keydictionary["KEY_KANJI"] = KEY_KANJI;
//keydictionary["KEY_ESCAPE"] = KEY_ESCAPE;
//keydictionary["KEY_CONVERT"] = KEY_CONVERT;
//keydictionary["KEY_NONCONVERT"] = KEY_KEY_E;
//keydictionary["KEY_ACCEPT"] = KEY_NONCONVERT;
//keydictionary["KEY_MODECHANGE"] = KEY_MODECHANGE;
//keydictionary["KEY_SPACE"] = KEY_SPACE;
//keydictionary["KEY_PRIOR"] = KEY_PRIOR;
//keydictionary["KEY_NEXT"] = KEY_NEXT;
//keydictionary["KEY_END"] = KEY_END;
//keydictionary["KEY_HOME"] = KEY_HOME;
//keydictionary["KEY_LEFT"] = KEY_LEFT;
//keydictionary["KEY_UP"] = KEY_UP;
//keydictionary["KEY_RIGHT"] = KEY_RIGHT;
//keydictionary["KEY_DOWN"] = KEY_DOWN;
//keydictionary["KEY_SELECT"] = KEY_SELECT;
//keydictionary["KEY_PRINT"] = KEY_PRINT;
//keydictionary["KEY_EXECUT"] = KEY_EXECUT;
//keydictionary["KEY_SNAPSHOT"] = KEY_SNAPSHOT;
//keydictionary["KEY_INSERT"] = KEY_INSERT;
//keydictionary["KEY_DELETE"] = KEY_DELETE;
//keydictionary["KEY_HELP"] = KEY_HELP;
//keydictionary["KEY_KEY_0"] = KEY_KEY_0;

//keydictionary["KEY_KEY_1"] = KEY_KEY_1;
//keydictionary["KEY_KEY_2"] = KEY_KEY_2;
//keydictionary["KEY_KEY_3"] = KEY_KEY_3;
//keydictionary["KEY_KEY_4"] = KEY_KEY_4;
//keydictionary["KEY_KEY_5"] = KEY_KEY_5;
//keydictionary["KEY_KEY_6"] = KEY_KEY_6;
//keydictionary["KEY_KEY_7"] = KEY_KEY_7;
//keydictionary["KEY_KEY_8"] = KEY_KEY_8;
//keydictionary["KEY_KEY_9"] = KEY_KEY_9;
////keydictionary["KEY_KEY_10"] = KEY_KEY_10;
//keydictionary["KEY_KEY_A"] = KEY_KEY_A;
//keydictionary["KEY_KEY_B"] = KEY_KEY_B;
//keydictionary["KEY_KEY_C"] = KEY_KEY_C;
//keydictionary["KEY_KEY_D"] = KEY_KEY_D;
//keydictionary["KEY_KEY_E"] = KEY_KEY_E;
//keydictionary["KEY_KEY_F"] = KEY_KEY_F;
//keydictionary["KEY_KEY_G"] = KEY_KEY_G;
//keydictionary["KEY_KEY_H"] = KEY_KEY_H;
//keydictionary["KEY_KEY_I"] = KEY_KEY_I;
//keydictionary["KEY_KEY_J"] = KEY_KEY_J;
//keydictionary["KEY_KEY_K"] = KEY_KEY_K;
//keydictionary["KEY_KEY_L"] = KEY_KEY_L;
//keydictionary["KEY_KEY_M"] = KEY_KEY_M;
//keydictionary["KEY_KEY_N"] = KEY_KEY_N;
//keydictionary["KEY_KEY_O"] = KEY_KEY_O;
//keydictionary["KEY_KEY_P"] = KEY_KEY_P;
//keydictionary["KEY_KEY_Q"] = KEY_KEY_Q;
//keydictionary["KEY_KEY_R"] = KEY_KEY_R;
//keydictionary["KEY_KEY_S"] = KEY_KEY_S;
//keydictionary["KEY_KEY_T"] = KEY_KEY_T;
//keydictionary["KEY_KEY_U"] = KEY_KEY_U;
//keydictionary["KEY_KEY_V"] = KEY_KEY_V;
//keydictionary["KEY_KEY_W"] = KEY_KEY_W;
//keydictionary["KEY_KEY_X"] = KEY_KEY_X;
//keydictionary["KEY_KEY_Y"] = KEY_KEY_Y;
//keydictionary["KEY_KEY_Z"] = KEY_KEY_Z;


//keydictionary["KEY_LWIN"] = KEY_LWIN;
//keydictionary["KEY_RWIN"] = KEY_RWIN;
//keydictionary["KEY_APPS"] = KEY_APPS;
//keydictionary["KEY_SLEEP"] = KEY_SLEEP;
//keydictionary["KEY_NUMPAD0"] = KEY_NUMPAD0;
//keydictionary["KEY_NUMPAD1"] = KEY_NUMPAD1;
//keydictionary["KEY_NUMPAD2"] = KEY_NUMPAD2;
//keydictionary["KEY_NUMPAD3"] = KEY_NUMPAD3;
//keydictionary["KEY_NUMPAD4"] = KEY_NUMPAD4;
//keydictionary["KEY_NUMPAD5"] = KEY_NUMPAD5;
//keydictionary["KEY_NUMPAD6"] = KEY_NUMPAD6;
//keydictionary["KEY_NUMPAD7"] = KEY_NUMPAD7;
//keydictionary["KEY_NUMPAD3"] = KEY_NUMPAD8;
//keydictionary["KEY_NUMPAD4"] = KEY_NUMPAD9;
//keydictionary["KEY_LWIN"] = KEY_LWIN;
//keydictionary["KEY_RWIN"] = KEY_RWIN;
//keydictionary["KEY_APPS"] = KEY_APPS;
//keydictionary["KEY_SLEEP"] = KEY_SLEEP;
//keydictionary["KEY_MULTIPLY"] = KEY_MULTIPLY;
////keydictionary["KEY_ADKEY_KEY_ED"] = KEY_ADKEY_KEY_ED;
//keydictionary["KEY_SEPARATOR"] = KEY_SEPARATOR;
//keydictionary["KEY_SUBTRACT"] = KEY_SUBTRACT;
//keydictionary["KEY_DECIMAL"] = KEY_DECIMAL;
//keydictionary["KEY_DIVIDE"] = KEY_DIVIDE;
//keydictionary["KEY_F1"] = KEY_F1;
//keydictionary["KEY_F2"] = KEY_F2;
//keydictionary["KEY_F3"] = KEY_F3;
//keydictionary["KEY_F4"] = KEY_F4;
//keydictionary["KEY_F5"] = KEY_F5;
//keydictionary["KEY_F6"] = KEY_F6;
//keydictionary["KEY_F7"] = KEY_F7;
//keydictionary["KEY_F8"] = KEY_F8;
//keydictionary["KEY_F9"] = KEY_F9;
//keydictionary["KEY_F10"] = KEY_F10;
//keydictionary["KEY_F11"] = KEY_F11;
//keydictionary["KEY_F12"] = KEY_F12;
////keydictionary["KEY_F13"] = KEY_F13;
////keydictionary["KEY_F14"] = KEY_F14;
////keydictionary["KEY_F15"] = KEY_F15;
////keydictionary["KEY_F16"] = KEY_F16;
////keydictionary["KEY_F17"] = KEY_F17;
////keydictionary["KEY_F18"] = KEY_F18;
////keydictionary["KEY_F19"] = KEY_F19;
////keydictionary["KEY_F20"] = KEY_F20;
////keydictionary["KEY_F21"] = KEY_F21;
////keydictionary["KEY_F22"] = KEY_F22;
////keydictionary["KEY_F23"] = KEY_F23;
////keydictionary["KEY_F24"] = KEY_F24;

//keydictionary["KEY_NUMLOCK"] = KEY_NUMLOCK;
//keydictionary["KEY_SCROLL"] = KEY_SCROLL;
//keydictionary["KEY_LSHIFT"] = KEY_LSHIFT;
//keydictionary["KEY_RSHIFT"] = KEY_RSHIFT;
//keydictionary["KEY_LCONTROL"] = KEY_LCONTROL;
//keydictionary["KEY_RCONTROL"] = KEY_RCONTROL;
//keydictionary["KEY_LMENU"] = KEY_LMENU;
//keydictionary["KEY_RMENU"] = KEY_RMENU;
//keydictionary["KEY_OEM_1"] = KEY_OEM_1;
//keydictionary["KEY_PLUS"] = KEY_PLUS;
//keydictionary["KEY_COMMA"] = KEY_COMMA;
//keydictionary["KEY_MINUS"] = KEY_MINUS;
//keydictionary["KEY_PERIOD"] = KEY_PERIOD;
//keydictionary["KEY_OEM_2"] = KEY_OEM_2;
//keydictionary["KEY_OEM_3"] = KEY_OEM_3;
//keydictionary["KEY_OEM_4"] = KEY_OEM_4;
//keydictionary["KEY_OEM_5"] = KEY_OEM_5;
//keydictionary["KEY_OEM_6"] = KEY_OEM_6;
//keydictionary["KEY_OEM_7"] = KEY_OEM_7;
//keydictionary["KEY_OEM_8"] = KEY_OEM_8;
//keydictionary["KEY_OEM_AX"] = KEY_OEM_AX;
//keydictionary["KEY_OEM_102"] = KEY_OEM_102;
//keydictionary["KEY_ATTN"] = KEY_ATTN;
//keydictionary["KEY_CRSEL"] = KEY_CRSEL;
//keydictionary["KEY_EXSEL"] = KEY_EXSEL;
//keydictionary["KEY_EREOF"] = KEY_EREOF;
//keydictionary["KEY_PLAY"] = KEY_PLAY;
//keydictionary["KEY_ZOOM"] = KEY_ZOOM;
//keydictionary["KEY_PA1"] = KEY_PA1;
//keydictionary["KEY_OEM_CLEAR"] = KEY_OEM_CLEAR;
//keydictionary["KEY_KEY_CODES_COUNT"] = KEY_KEY_CODES_COUNT;
