
/*
* For comments regarding functions please see the header file.
*/

#include "guichan/irrlicht/irrlichtinput.hpp"

#include "guichan/exception.hpp"

namespace gcn
{
    IrrlichtInput::IrrlichtInput(irr::IrrlichtDevice* device)
    {
        mIrrlichtDevice = device;
    }

    bool IrrlichtInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    KeyInput IrrlichtInput::dequeueKeyInput()
    {
        KeyInput keyInput;

        if (mKeyInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    bool IrrlichtInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    MouseInput IrrlichtInput::dequeueMouseInput()
    {
        MouseInput mouseInput;

        if (mMouseInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void IrrlichtInput::pushInput(const irr::SEvent& event)
    {
        KeyInput keyInput;
        MouseInput mouseInput;

        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            mouseInput.setX(event.MouseInput.X);
            mouseInput.setY(event.MouseInput.Y);

            irr::u32 ticks = mIrrlichtDevice->getTimer()->getRealTime();

            switch(event.MouseInput.Event)
            {
                case irr::EMIE_LMOUSE_PRESSED_DOWN:
                    mouseInput.setButton(MouseInput::LEFT);
                    mouseInput.setType(MouseInput::PRESSED);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_RMOUSE_PRESSED_DOWN:
                    mouseInput.setButton(MouseInput::RIGHT);
                    mouseInput.setType(MouseInput::PRESSED);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_MMOUSE_PRESSED_DOWN:
                    mouseInput.setButton(MouseInput::MIDDLE);
                    mouseInput.setType(MouseInput::PRESSED);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_LMOUSE_LEFT_UP:
                    mouseInput.setButton(MouseInput::LEFT);
                    mouseInput.setType(MouseInput::RELEASED);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_RMOUSE_LEFT_UP:
                    mouseInput.setButton(MouseInput::RIGHT);
                    mouseInput.setType(MouseInput::RELEASED);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_MMOUSE_LEFT_UP:
                    mouseInput.setButton(MouseInput::MIDDLE);
                    mouseInput.setType(MouseInput::RELEASED);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_MOUSE_MOVED:
                    mouseInput.setButton(MouseInput::EMPTY);
                    mouseInput.setType(MouseInput::MOVED);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                case irr::EMIE_MOUSE_WHEEL:
                    mouseInput.setButton(MouseInput::MIDDLE);
                    mouseInput.setType(event.MouseInput.Wheel > 0 ? MouseInput::WHEEL_MOVED_UP : MouseInput::WHEEL_MOVED_DOWN);
                    mouseInput.setTimeStamp(ticks);
                    mMouseInputQueue.push(mouseInput);
                    break;

                default:
                    break;
            }
        }

        else if(event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            int value = convertIrrlichtEventToGuichanKeyValue(event);

            if (value == -1)
            {
                value = (int)event.KeyInput.Char;
            }

            keyInput.setKey(Key(value));
            keyInput.setType(event.KeyInput.PressedDown ? KeyInput::PRESSED : KeyInput::RELEASED);
            keyInput.setShiftPressed(event.KeyInput.Shift);
            keyInput.setControlPressed(event.KeyInput.Control);
            // keyInput.setAltPressed();
            keyInput.setNumericPad(event.KeyInput.Key >= irr::KEY_NUMPAD0 && event.KeyInput.Key <= irr::KEY_DIVIDE); 
            
            mKeyInputQueue.push(keyInput);
        }
    }

    int IrrlichtInput::convertIrrlichtEventToGuichanKeyValue(const irr::SEvent& event)
    {
        using namespace irr;
        int value = -1;

        switch (event.KeyInput.Key)
        {
            case KEY_LBUTTON:
                break;
            case KEY_RBUTTON:
                break;
            case KEY_CANCEL:
                break;
            case KEY_MBUTTON:
                break;
            case KEY_XBUTTON1:
                break;
            case KEY_XBUTTON2:
                break;
            case KEY_BACK:
                value = Key::BACKSPACE;
                break;
            case KEY_TAB:
                value = Key::TAB;
                break;
            case KEY_CLEAR:
                break;
            case KEY_RETURN:
                value = Key::ENTER;
                break;
            case KEY_SHIFT:
                break;
            case KEY_CONTROL:
                break;
            case KEY_MENU:
                // value = Key::LeftAlt; // left?
                break;
            case KEY_PAUSE:
                value = Key::PAUSE;
                break;
            case KEY_CAPITAL:
                value = Key::CAPS_LOCK;
                break;
            case KEY_KANA:
                break;
            //case KEY_HANGUEL:
            //    break;
            //case KEY_HANGUL:
            //    break;
            case KEY_JUNJA:
                break;
            case KEY_FINAL:
                break;
            //case KEY_HANJA:
            //    break;
            case KEY_KANJI:
                break;
            case KEY_ESCAPE:
                value = Key::ESCAPE;
                break;
            case KEY_CONVERT:
                break;
            case KEY_NONCONVERT:
                break;
            case KEY_ACCEPT:
                break;
            case KEY_MODECHANGE:
                // value = Key::ALT_GR;
                break;
            case KEY_SPACE:
                value = Key::SPACE;
                break;
            case KEY_PRIOR:
                // value = Key::PageUp;
                break;
            case KEY_NEXT:
                // value = Key::PageDown;
            case KEY_END:
                value = Key::END;
                break;
            case KEY_HOME:
                value = Key::HOME;
                break;
            case KEY_LEFT:
                value = Key::LEFT;
                break;
            case KEY_UP:
                value = Key::UP;
                break;
            case KEY_RIGHT:
                value = Key::RIGHT;
                break;
            case KEY_DOWN:
                value = Key::DOWN;
                break;
            case KEY_SELECT:
                break;
            case KEY_PRINT:
                // value = Key::PrintScreen;
                break;
            case KEY_EXECUT:
                break;
            case KEY_SNAPSHOT:
                // value = Key::PrintScreen;
                break;
            case KEY_INSERT:
                value = Key::INSERT;
                break;
            case KEY_DELETE:
                value = Key::DELETE;
                break;
            case KEY_HELP:
                break;

            case KEY_LWIN:
                break;
            case KEY_RWIN:
                break;
            case KEY_APPS:
                break;
            case KEY_SLEEP:
                break;

            case KEY_F1:
                value = Key::F1;
                break;
            case KEY_F2:
                value = Key::F2;
                break;
            case KEY_F3:
                value = Key::F3;
                break;
            case KEY_F4:
                value = Key::F4;
                break;
            case KEY_F5:
                value = Key::F5;
                break;
            case KEY_F6:
                value = Key::F6;
                break;
            case KEY_F7:
                value = Key::F7;
                break;
            case KEY_F8:
                value = Key::F8;
                break;
            case KEY_F9:
                value = Key::F9;
                break;
            case KEY_F10:
                value = Key::F10;
                break;
            case KEY_F11:
                value = Key::F11;
                break;
            case KEY_F12:
                value = Key::F12;
                break;
            case KEY_F13:
                value = Key::F13;
                break;
            case KEY_F14:
                value = Key::F14;
                break;
            case KEY_F15:
                value = Key::F15;
                break;
            case KEY_NUMLOCK:
                value = Key::NUM_LOCK;
                break;
            case KEY_SCROLL:
                value = Key::SCROLL_LOCK;
                break;
            default:
                break;
        }

        return value;
    }
}
