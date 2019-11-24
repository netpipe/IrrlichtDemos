#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>
#include <map>
#include <string>
#include <vector>

#define MOD_NONE 0x0004

class KeyCatcher
{
    public:
        virtual ~KeyCatcher() {}

        virtual void keyPressed(const SDLKey &keysym, const short &ch) = 0;
};

class Input
{
    public:
        Input();
        ~Input();

        void get();
        void flush();

        inline void setKeyCatcher(KeyCatcher *kc)
        {
            keyCatcher = kc;
            capturedKeys = true;
        }
        inline void clearKeyCatcher()
        {
            keyCatcher = NULL;
            capturedKeys = false;
        }

        inline bool getKeyPress(int no)
        {
            bool temp = keys[no];
            keys[no] = false;
            return temp;
        }

        inline bool getKeyDown(int no)
        {
            return keys[no];
        }

        inline bool anyKeysPressed() const { return (keysp > 0); }

        inline int getMouseX() const { return mouse[0]; }
        inline int getMouseY() const { return mouse[1]; }

        inline bool getMouseClick(int no)
        {
            bool temp = mouseButton[no];
            mouseButton[no] = false;
            return temp;
        }

        inline bool getMouseDown(int no)
        {
            return mouseButton[no];
        }

        inline bool getMouseScroll(int no)
        {
            bool temp = mouseButton[no];
            mouseButton[no + 2] = false;
            return temp;
        }

        inline bool getMouseScrollNoPop(int no)
        {
            return mouseButton[no + 2];
        }

        inline bool anyMousePressed() const { return (mousep > 0); }
    private:
        unsigned int keysp;
        unsigned int mousep;
        std::map<int, bool> keys;
        std::map<int, bool> mouseButton;
        int mouse[2];

        bool capturedKeys;
        KeyCatcher *keyCatcher;

        SDL_Event event;
};
#endif

