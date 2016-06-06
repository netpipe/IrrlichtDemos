#include "manager.h"
#include <iostream>
using namespace irr;
using namespace core;
using namespace scene;

void GameManager::processEvents()
{
    ISceneCollisionManager* collision = device->getSceneManager()->getSceneCollisionManager();

    ISceneNode* temp_node = collision->getSceneNodeFromScreenCoordinatesBB( position2d<s32>(event.mouseX(), event.mouseY()), -1, true );
    card* temp_card = 0;

    u32 SELECTED_NODE_NOT_IN_TRIANGLE = 52;
    for(u32 i = 0; i < 52; i++)
    {
        if( temp_node == nodes[i]->getNode() )
        {
            if( !nodes[i]->isSelected() )
                nodes[i]->select();
            temp_card = nodes[i];
            SELECTED_NODE_NOT_IN_TRIANGLE = i;
        }
        else
        {
            if( nodes[i]->isSelected() )
                nodes[i]->select();
        }
    }

    if( active1 )
        if( !active1->isSelected() )
            active1->select();

    if( event.leftMousePressed() )  //left-click
    {
        bool MOUSE_ON_TOP_SECOND_STACK = false;
        //if( temp_node && temp_card )
        MOUSE_ON_TOP_SECOND_STACK = ( (event.mouseX()>200 && event.mouseX()<325 && event.mouseY()>650 && event.mouseY()<750) );
        bool ACTIVE1_IS_ON_FIRST_STACK = false;
        if(active1)
            ACTIVE1_IS_ON_FIRST_STACK = ( active1->getNode()->getPosition().X == 2.3f && active1->getNode()->getPosition().Y == -2.5f && SELECTED_NODE_NOT_IN_TRIANGLE>20 );

        if( temp_card && !active1 && !active2 )
        {
            active1 = temp_card;
            if( !active1->isShown() && SELECTED_NODE_NOT_IN_TRIANGLE>20 && SELECTED_NODE_NOT_IN_TRIANGLE<52 )
            active1->flip();
        }
        else if( active1 && ACTIVE1_IS_ON_FIRST_STACK && MOUSE_ON_TOP_SECOND_STACK )
        {
            for(u32 i = 0; i<52; i++)
            {
                vector3df pos = nodes[i]->getNode()->getPosition();
                if(pos.X == 0.0f && pos.Y == -2.5f)
                    nodes[i]->setPosition( pos.X, pos.Y, pos.Z+.0001f);
            }

            if( !temp_card && temp_node == restack->getNode() )
            {
                active1->setPosition(0.0f, -2.5f, 1.0f);
                active1 = 0;
            }
            else if( temp_card && temp_card!=active1 )
            {
                active2 = temp_card;
                if( (u32)active1->getFace().value + (u32)active2->getFace().value == 14 )
                {
                    active1->setPosition(0,0,-10);
                    active2->setPosition(0,0,-10);
                    active1 = active2 = 0;
                }
                else
                {
                    active1->setPosition(0.0f, -2.5f, 1.0f);
                    active1 = active2 = 0;
                }
            }
        }
        else if( active1 )
        {
            if( temp_card && temp_card!=active1 )
            {
                active2 = temp_card;
                if( (u32)active1->getFace().value + (u32)active2->getFace().value == 14 )
                {
                    active1->setPosition(0,0,-10);
                    active2->setPosition(0,0,-10);
                    active1 = active2 = 0;
                    score+= (5*difficulty);
                }
                else
                {
                    active1 = active2 = 0;
                }
            }
        }
        else if( !active1 && !temp_card && deck_empty && temp_node==oldstack->getNode() && redeal_remaining>0 )
        {
            f32 depth = 1.0f;
            for(u32 i = 21; i<52; i++)
            {
                if( nodes[i]->getNode()->getPosition().Z > 0.0f )
                {
                    nodes[i]->setPosition(2.3f, -2.5f, depth);
                    nodes[i]->flip();
                    depth+=.0001f;
                }
            }
            score-= (50*difficulty);
            redeal_remaining--;
        }
    }


    if( (nodes[1]->getNode()->getPosition().Z == nodes[2]->getNode()->getPosition().Z) && nodes[2]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[0]->isShown() )
            nodes[0]->flip();
    if( (nodes[3]->getNode()->getPosition().Z == nodes[4]->getNode()->getPosition().Z) && nodes[4]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[1]->isShown() )
            nodes[1]->flip();
    if( (nodes[4]->getNode()->getPosition().Z == nodes[5]->getNode()->getPosition().Z) && nodes[5]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[2]->isShown() )
            nodes[2]->flip();
    if( (nodes[6]->getNode()->getPosition().Z == nodes[7]->getNode()->getPosition().Z) && nodes[7]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[3]->isShown() )
            nodes[3]->flip();
    if( (nodes[7]->getNode()->getPosition().Z == nodes[8]->getNode()->getPosition().Z) && nodes[8]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[4]->isShown() )
            nodes[4]->flip();
    if( (nodes[8]->getNode()->getPosition().Z == nodes[9]->getNode()->getPosition().Z) && nodes[9]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[5]->isShown() )
            nodes[5]->flip();
    if( (nodes[10]->getNode()->getPosition().Z == nodes[11]->getNode()->getPosition().Z) && nodes[11]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[6]->isShown() )
            nodes[6]->flip();
    if( (nodes[11]->getNode()->getPosition().Z == nodes[12]->getNode()->getPosition().Z) && nodes[12]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[7]->isShown() )
            nodes[7]->flip();
    if( (nodes[12]->getNode()->getPosition().Z == nodes[13]->getNode()->getPosition().Z) && nodes[13]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[8]->isShown() )
            nodes[8]->flip();
    if( (nodes[13]->getNode()->getPosition().Z == nodes[14]->getNode()->getPosition().Z) && nodes[14]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[9]->isShown() )
            nodes[9]->flip();
    if( (nodes[15]->getNode()->getPosition().Z == nodes[16]->getNode()->getPosition().Z) && nodes[16]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[10]->isShown() )
            nodes[10]->flip();
    if( (nodes[16]->getNode()->getPosition().Z == nodes[17]->getNode()->getPosition().Z) && nodes[17]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[11]->isShown() )
            nodes[11]->flip();
    if( (nodes[17]->getNode()->getPosition().Z == nodes[18]->getNode()->getPosition().Z) && nodes[18]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[12]->isShown() )
            nodes[12]->flip();
    if( (nodes[18]->getNode()->getPosition().Z == nodes[19]->getNode()->getPosition().Z) && nodes[19]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[13]->isShown() )
            nodes[13]->flip();
    if( (nodes[19]->getNode()->getPosition().Z == nodes[20]->getNode()->getPosition().Z) && nodes[20]->getNode()->getPosition().Z != 1.0001f )
        if( !nodes[14]->isShown() )
            nodes[14]->flip();

    deck_empty = true;
    for(u32 i=21; i<52; i++)
    {
        if(nodes[i]->getNode()->getPosition().X == 2.3f)
            deck_empty = false;
    }

    event.startEventProcess();
}

void GameManager::deal()
{
    nodes[0]->setPosition(0,4,1.0001f);
    nodes[1]->setPosition(-.5,3,1.0001f);
    nodes[2]->setPosition(.5,3,1.0001f);
    nodes[3]->setPosition(-1,2,1.0001f);
    nodes[4]->setPosition(0,2,1.0001f);
    nodes[5]->setPosition(1,2,1.0001f);
    nodes[6]->setPosition(-1.5,1,1.0001f);
    nodes[7]->setPosition(-.5,1,1.0001f);
    nodes[8]->setPosition(.5,1,1.0001f);
    nodes[9]->setPosition(1.5,1,1.0001f);
    nodes[10]->setPosition(-2,0,1.0001f);
    nodes[11]->setPosition(-1,0,1.0001f);
    nodes[12]->setPosition(0,0,1.0001f);
    nodes[13]->setPosition(1,0,1.0001f);
    nodes[14]->setPosition(2,0,1.0001f);
    nodes[15]->setPosition(-2.5,-1,1.0001f);
    nodes[16]->setPosition(-1.5,-1,1.0001f);
    nodes[17]->setPosition(-.5,-1,1.0001f);
    nodes[18]->setPosition(.5,-1,1.0001f);
    nodes[19]->setPosition(1.5,-1,1.0001f);
    nodes[20]->setPosition(2.5,-1,1.0001f);

    f32 depth = 1.0f;

    for(u32 i = 21; i<52; i++)
    {
        nodes[i]->setPosition(2.3f, -2.5f, depth);
        depth+=.0001f;
    }

    for(u32 i = 15; i<21; i++)
        if( !nodes[i]->isShown() )
            nodes[i]->flip();
}
