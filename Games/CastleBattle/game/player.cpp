#include <iostream>
#include <cmath>
#include "Logger.h"
#include "player.h"
using namespace irr;
using namespace KEYBOARD;
using namespace std;
/*drawline rotate irrlicht
* Init : ..
*/
Player::Player(IrrlichtDevice* device, scene::ISceneManager* smgr, video::IVideoDriver* driver, core::vector3df position, Physics* physics, Keyboard* keyboard, PLAYER_TYPE type, PLAYER_POSITION side){
    this->type = type;
    this->smgr = smgr;
    this->driver = driver;
    this->physics = physics;
    this->keyboard = keyboard;
    this->device = device;
    this->camera = 0;
    this->side = side;
    this->castle = new Castle(this->smgr,this->physics, device, this->driver, position);
    position = this->castle->calculateAbsoluteCenter() + core::vector3df(0,0,this->castle->getSideSize('l').X);
    this->cannon = new Cannon(device, smgr,driver,position,physics);
    if(type == HUMAN) { log1("Player type HUMAN"); }
    else { log1("Player type AI"); }
}
Player::~Player(){
    // delete this->castle;
    // delete this->cannon;
}
scene::IMeshSceneNode* Player::getNode() {
    return this->cannon->getCannon();
}
bool Player::checkTarget(){
    return this->castle->checkTarget();
}
bool Player::loop(HUD::HUD* hud){
    Key* key = this->keyboard->IsKeyDown();
    ACTION_KEYBOARD action = key == 0 ? ACTION_NULL : key->action;
    if(!this->cannon->moveCannon(action)) return false;
    switch(action){
        case SHOOT:
                hud->animatePower();
        break;
        default:
            if(this->keyboard->getLastKey()->action == SHOOT){
                this->cannon->shoot(hud->getPower());
                this->keyboard->resetLastKey();
                #if (DEBUG_OUTPUT_MASK & 2)
                   return false;
                #endif
            }
        break;
    };
    return true;
}
core::vector3df Player::getCannonRange(){
return this->cannon->getRange();
}
void Player::focusCamera(){
  #if (DEBUG_OUTPUT_MASK & 2)
      return;
  #endif
    if(this->type == HUMAN){
        core::vector3df offset;
        switch(this->side){
            case STRAIGHT:
                 offset =  core::vector3df(
                0,
                this->cannon->getBoundingBox().MaxEdge.Y+0.6f,
                -2.f);
            break;
            case OPPOSITE:
                offset =  core::vector3df(
                0,
                this->cannon->getBoundingBox().MaxEdge.Y+0.6f,
                -2.f);
        };
        core::vector3df rotation = this->cannon->getCannon()->getRotation();
        this->cannon->setCamera(offset,rotation,smgr,this->cannon->getCannon());
    }
}
void Player::setCannon(){
    core::vector3df position = core::vector3df(0,0,0);
    switch(this->side){

        case OPPOSITE:
            position = this->castle->calculateAbsoluteCenter() - core::vector3df(0,0,this->castle->getSideSize('l').X);
            this->cannon->initCannon(position, core::vector3df(0,180,0));
            break;

        default: break;
    }
}
void Player::reset(){
    // core::vector3df position = this->cannon->getCannon()->getAbsolutePosition();
    core::vector3df rotation = this->cannon->rotation;
    log1("Reset player");
    this->cannon->setRotation(rotation);

}
