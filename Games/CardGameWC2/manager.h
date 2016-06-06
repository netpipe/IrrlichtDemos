#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include "cardgame.h"
#include "card.h"
#include <irrlicht.h>

class card;

class GameManager
{
    MastEventReceiver event;
    irr::IrrlichtDevice* device;

    irr::core::array<card*> nodes;

    card* active1;
    card* active2;
    card* restack;
    card* oldstack;

    bool deck_empty;
    irr::u32 redeal_remaining;
    irr::s32 score;
    irr::u32 difficulty;

    void deal();
    void chooseLevel();

public:
    GameManager();
    irr::IrrlichtDevice* Device();

    bool run();
    void drawAll();
    void processEvents();

    void endGame();
};

#endif
