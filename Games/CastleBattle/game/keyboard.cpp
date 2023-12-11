#include "keyboard.h"
#include <iostream>
using namespace KEYBOARD;
Key::Key(ACTION_KEYBOARD action, EKEY_CODE k){
    this->action = action;
    this->key = k;
};

Keyboard::Keyboard(){
    for(u32 i = 0; i < KEY_KEY_CODES_COUNT; i++){
        this->keyDown[i] = false;
    }
    this->lastkey = new Key(ACTION_NULL,irr::KEY_EREOF);

    this->mapKeyboard();
}
bool Keyboard::OnEvent(const SEvent& event) {
    if(event.EventType == EET_KEY_INPUT_EVENT)
            this->keyDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            this->IsKeyDown();

    return false;
}
Key* Keyboard::IsKeyDown(){
   for(std::vector<Key>::const_iterator it = this->mapKeys.begin(); it != this->mapKeys.end(); ++it){
//               std::cout<<it->k<<std::endl;

          if(this->keyDown[it->key]) {
            Key* lastkey = new Key(it->action,it->key);
                if(this->lastkey->action == ACTION_NULL || this->lastkey->action != lastkey->action) this->lastkey = lastkey;
            return lastkey;
            }
    }
    return 0; // null


//    if(this->keyDown[keyCode]) return keyCode;
//    else (EKEY_CODE)0x0;

};
void Keyboard::mapKeyboard(const std::vector<Key> keys){

//        if(keys == 0) this->mapKeys.push_back(Key(INCLINATE_UP, KEY_KEY_W));
//            std::cout<<" "<<keys[0].action<<" "<<keys[1].action<<" "<<keys[2].action<<" "<<keys[3].action<<std::endl;
            this->mapKeys.clear();
            this->mapKeys = keys;
//            std::cout<<this->mapKeys.size()<<std::endl;
};

Key* Keyboard::getLastKey(){
 return this->lastkey;
}
void Keyboard::resetLastKey(){
 this->lastkey = new Key(ACTION_NULL,irr::KEY_EREOF);
}
