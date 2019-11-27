#ifndef Q_EVENT_H
#define Q_EVENT_H

    #include <irrlicht.h>

    class QEventReceiver : public irr::IEventReceiver
    {
        public:
            virtual bool OnEvent(const irr::SEvent& event)
            {
                if(event.EventType == irr::EET_KEY_INPUT_EVENT)
                {
                    if(event.KeyInput.PressedDown == true)
                    {
                        if(keyState[event.KeyInput.Key] != DOWN)
                            keyState[event.KeyInput.Key] = PRESSED;
                        else
                            keyState[event.KeyInput.Key] = DOWN;
                    }
                    else
                    {
                        if(keyState[event.KeyInput.Key] != UP)
                            keyState[event.KeyInput.Key] = RELEASED;
                    }
                    //return true;
                }

                if ((event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_SPACE &&
                    event.KeyInput.PressedDown == false) ||
                    (event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
                    )
                {
                    scene::ICameraSceneNode * camera = device->getSceneManager()->getActiveCamera ();
                    if ( camera && camera->isInputReceiverEnabled () && player->ammo > 0)
                    {
                        weaponFire();
                        player->ammo--;
                    }
                }

                return false;
            }

            virtual bool keyPressed(irr::EKEY_CODE keyCode)
            {
                if(keyState[keyCode] == PRESSED)
                    return true;
                else
                    return false;
            }

            virtual bool keyDown(irr::EKEY_CODE keyCode)
            {
                if(keyState[keyCode] == DOWN || keyState[keyCode] == PRESSED)
                    return true;
                else
                    return false;
            }

            virtual bool keyUp(irr::EKEY_CODE keyCode)
            {
                if(keyState[keyCode] == UP || keyState[keyCode] == RELEASED)
                    return true;
                else
                    return false;
            }

            virtual bool keyReleased(irr::EKEY_CODE keyCode)
            {
                if(keyState[keyCode] == RELEASED)
                    return true;
                else
                    return false;
            }

            virtual bool clearKeyState()
            {
                for(irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
                {
                    if(keyState[i] == RELEASED)
                        keyState[i] = UP;
                    else if(keyState[i] == PRESSED)
                        keyState[i] = DOWN;
                }
            }

            QEventReceiver()
            {
                for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
                    keyState[i] = UP;
            }

        private:
            enum keyStatesEnum{ UP, DOWN, PRESSED, RELEASED };

            keyStatesEnum keyState[irr::KEY_KEY_CODES_COUNT];
    };

#endif
