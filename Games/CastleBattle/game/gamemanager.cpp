#include "gamemanager.h"
using namespace irr;
using namespace std;
GameManager::GameManager(
  scene::ISceneManager* smgr,
  video::IVideoDriver* driver,
  IrrlichtDevice* device,
  core::vector3df worldPosition,
  Physics* physics,
  core::stringw* names){
    this->smgr = smgr;
    this->device = device;
    this->driver = driver;
    this->physics = physics;
    this->worldPosition = worldPosition;
    this->turn = true;
    this->stop = false;
    initGamePlay(names);
    initKeyboard();
}
GameManager::~GameManager(){
    // delete this->players;
}

void GameManager::initGamePlay(core::stringw* names){
    //for now all the players will have the same HUD

    this->hud = new HUD::HUD(device,driver);
    log1("HUD initialized");
    this->players[0] = new Player(
        this->device,
        this->smgr,
        this->driver,
        this->worldPosition,
        this->physics,
        &this->keyboard,
        HUMAN,
        STRAIGHT);
    this->players[0]->name = (names) ? names[0] : L"Player 1";
    this->players[1] = new Player(
        this->device,
        this->smgr,
        this->driver,
        core::vector3df(this->worldPosition.X, 0, this->players[0]->getCannonRange().Z),
        this->physics,
        &this->keyboard,
        HUMAN,
        OPPOSITE);
        this->players[1]->name = (names) ? names[1] : L"Player 2";
        this->players[1]->setCannon();
        this->players[0]->focusCamera();
        this->hud->env->drawAll();
        this->hud->setPlayerName(this->players[0]->name);
        this->turn = true;
        this->keyboard.resetLastKey();
        this->names = names;
        log1("Default turn start: PLAYER 1");

}
void GameManager::initKeyboard(){
    this->device->setEventReceiver(&this->keyboard);
    log1("Keyboard mapped");
}
int GameManager::getWinner(){
    return turn ? 0 : 1; // return !turn is better?
}
core::stringw* GameManager::getNames(){
    return this->names;
}
bool GameManager::loop(){
    this->hud->env->drawAll();
    if(stop) return true;
    if(this->turn){
        if(!this->players[0]->loop(hud)){
            this->turn = false;
            this->players[1]->reset();
            this->players[1]->focusCamera();
            hud->setPlayerName(this->players[1]->name);
            hud->resetPower();
            log1("TURN: PLAYER2");
        }
        if(this->players[1]->checkTarget()){ stop = true; return true; }

    }
    else {
        if(!this->players[1]->loop(hud)){
            this->turn = true;
            this->players[0]->reset();
            this->players[0]->focusCamera();
            hud->setPlayerName(this->players[0]->name);
            hud->resetPower();
            log1("TURN: PLAYER1");
        }
        if(this->players[0]->checkTarget()){ stop = true; return true; }


    }
    // never goes here;
    return false;
}
