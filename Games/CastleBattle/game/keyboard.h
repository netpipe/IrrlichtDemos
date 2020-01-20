#ifndef KEYBOARD_H
#define KEYBOARD_H
#include<irrlicht.h>
#include <vector>

using namespace irr;
/** Keyboard namespace with all things related to Keyboard, Key, and keyboard mapping */
namespace KEYBOARD{

 enum ACTION_KEYBOARD{ /*!< ACTION_KEYBOARD Enum. All type action possible */
                INCLINATE_UP = 1,
                INCLINATE_DOWN,
                INCLINATE_LEFT,
                INCLINATE_RIGHT,
                MOVE_LEFT,
                MOVE_RIGHT,
                MOVE_UP,
                MOVE_DOWN,
                SHOOT,
                ACTION_NULL
            };
    /** Key is a class that describe a irrlicht EKEY_CODE and what ACTION_KEYBOARD is referring. */
    class Key{
        private:
        public:
          ACTION_KEYBOARD action;
            EKEY_CODE key;
            Key(ACTION_KEYBOARD act, EKEY_CODE k);
            Key getKey();
    };
//     const Key KEYBOARD_TEMPLATE[] ={
//         Key(INCLINATE_UP, KEY_KEY_W),
//         Key(INCLINATE_DOWN, KEY_KEY_S),
//         Key(INCLINATE_RIGHT, KEY_KEY_D),
//         Key(INCLINATE_LEFT, KEY_KEY_A)
//    };
//Maybe templates could be a bettere solution, but this is plain!
//     const Key * KEYBOARD_DEFAULT = new Key[4];
    /** Default Keybaord mapping. */
     const std::vector<Key> KEYBOARD_DEFAULT {
        Key(INCLINATE_UP, KEY_KEY_W),
        Key(INCLINATE_DOWN, KEY_KEY_S),
        Key(INCLINATE_RIGHT, KEY_KEY_D),
        Key(INCLINATE_LEFT, KEY_KEY_A),
        Key(SHOOT, KEY_SPACE)
     };
    /** Keyboard class will handle keyboard input events */
    class Keyboard : public IEventReceiver{

    private:
        /** Array of keys */
        bool keyDown[KEY_KEY_CODES_COUNT];
        /** Map key with the ACTION_KEYBOARD specified in the Keyboard vector. */
        std::vector<Key> mapKeys;
        /** Last key pressed */
        Key* lastkey;
    public:
        /** Default constructor set initial state of the keys (not pressed) and map the keyboard with the actions */
        Keyboard();
        /** Map keyboard with the keys.
        * In particular given a configuration array of ::KEYBOARD::Key the ::KEYBOARD::Keyboard::IsKeyDown() method will check these key and will return the corresponding ACTION_KEYBOARD
        * Default configuration see ::KEYBOARD_DEFAULT
        * @param keys std::vector<Key>. Optional
        * @see ::Key
        */
        void mapKeyboard(const std::vector<Key> keys = KEYBOARD_DEFAULT);
        /** Virtual method called by irrlicht whenever a key is pressed. */
        virtual bool OnEvent(const SEvent& event);
        /** Get last key pressed */
        Key* getLastKey();
        /** Return a pressed ::Key.
        * This method will check only keys ::mapKeys
        * @see ::mapKeyboard
        * @see ::KEYBOARD_DEFAULT
        * @see ::Key
        */
        Key* IsKeyDown();
        /** Reset last key pressed variable */
        void resetLastKey();

};
}
#endif
