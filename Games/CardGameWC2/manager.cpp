#include "cardgame.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace gui;

GameManager::GameManager()
{
    device = createDevice(EDT_OPENGL,dimension2d<s32>(550,800),true, false);
    event.init();

    if(device)
    device->setEventReceiver(&event);
    srand( (u32)time( NULL ) );

    array<card*> tempNodes;
    if(device)
    for(int i=1; i<5; i++)
    {
        for(int j=1; j<14; j++)
        {
            Face newFace;
            newFace.suit = (E_CARD_SUIT)i;
            newFace.value = (E_CARD_NUMBER)j;

            tempNodes.push_back( new card(this, newFace) );
        }
    }

    //Shuffle the cards now...
    bool tempDeal[52];
    for(int x=0;x<52;x++)
        tempDeal[x] = 0;

    u32 i=0;
    while(i<52)
    {
        u32 j = (rand()%52);

        if(tempDeal[j])
            continue;

        nodes.push_back(tempNodes[j]);
        i++;
        tempDeal[j] = true;
    }

    active1 = 0;
    active2 = 0;

    device->getSceneManager()->setAmbientLight( SColor(255,255,255,255) );
    ICameraSceneNode* cam = device->getSceneManager()->addCameraSceneNode( 0, vector3df(0,0,-5), vector3df(0,.3,1), 1 );
    cam->setIsDebugObject(true);

    score = 0;
    difficulty = 0;

    chooseLevel();
    deal();

    Face newFace;
    newFace.suit = ECS_NULL;
    newFace.value = ECN_NULL;

    restack = new card(this, newFace);
    restack->setPosition(0.0f,-3.0f,1.5f);
    restack->flip();

    oldstack = new card(this, newFace);
    oldstack->setPosition(2.5f,-3.0f, 1.5f);
    oldstack->flip();
}

IrrlichtDevice * GameManager::Device()
{
    return device;
}

bool GameManager::run()
{
    if(device)
        return device->run();
    else
        return 0;
}

void GameManager::drawAll()
{
    stringw score_string = "";

    if(device)
    {
        score_string = L"Traingle Solitaire! SCORE: ";
        score_string += score;

        if( difficulty>0 )
        {
            if( difficulty == 2 )
                score_string += "     Difficulty: EASY";
            else
                score_string += "     Difficulty: HARD";
        }

        device->setWindowCaption( score_string.c_str() );
        device->getVideoDriver()->beginScene(true, true, SColor(255, 10, 185, 10) );
        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();
        device->getVideoDriver()->endScene();

        event.endEventProcess();
    }
}

void GameManager::endGame()
{
    if(device)
    {
        device->closeDevice();
        device->run();
    }
}

void GameManager::chooseLevel()
{
    Face newFace;
    newFace.suit = (E_CARD_SUIT)0;
    newFace.value = (E_CARD_NUMBER)1;
    card* easy = new card(this, newFace);
    easy->setPosition(-1.0f, 0.0f, 0.0f);
    easy->flip();

    newFace.value = (E_CARD_NUMBER)2;
    card* hard = new card(this, newFace);
    hard->setPosition(1.0f, 0.0f, 0.0f);
    hard->flip();

    ISceneCollisionManager* collision = device->getSceneManager()->getSceneCollisionManager();

    bool selection = false;
    while( run() && !selection )
    {
        drawAll();

        ISceneNode* temp_node = collision->getSceneNodeFromScreenCoordinatesBB( position2d<s32>(event.mouseX(), event.mouseY()), -1, true );
        if( temp_node == easy->getNode() )
        {
            if( !easy->isSelected() )
                easy->select();
        }
        else if( temp_node == hard->getNode() )
        {
            if( !hard->isSelected() )
                hard->select();
        }
        else if( !temp_node )
        {
            if( easy->isSelected() )
                easy->select();
            if( hard->isSelected() )
                hard->select();
        }

        if( event.leftMousePressed() )
        {
            if( easy->isSelected() )
            {
                difficulty = 2;
                redeal_remaining = 3;
                selection = true;
            }
            else if( hard->isSelected() )
            {
                difficulty = 5;
                redeal_remaining = 1;
                selection = true;
            }
        }

        event.startEventProcess();
    }

    easy->setPosition(0,0,-10);
    hard->setPosition(0,0,-10);
}
