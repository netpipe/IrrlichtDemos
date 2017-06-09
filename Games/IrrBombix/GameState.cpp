/*
 * GameState.h
 *
 *  Created on: Apr 11, 2011
 *      based on Irrlicht_-_from_Noob_to_Pro:_Gamestates
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "GameState.h"

 GameState::GameState(void)
 {
        name = irr::core::stringc("unknown");

        active = false;
        firstEnter = true;
 }

 GameState::GameState(irr::core::stringc newName) : name(newName)
 {
        active = false;
        firstEnter = true;
 }

 GameState::~GameState(void)
 {
 }

 irr::core::stringc GameState::getName(void)
 {
        return(name);
 }

 bool GameState::isActive(void)
 {
        return(active);
 }

 void GameState::setActive(bool isactive)
 {
        active = isactive;
 }

 void GameState::initialize(void)
 {
        active = true;
        OnEnter();
 }

 void GameState::flush(void)
 {
        active = false;
        OnLeave();
 }




#endif /* GAMESTATE_H_ */
