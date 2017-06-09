/*
 * gamestat.h
 *
 *  Created on: Apr 11, 2011
 *      based on Irrlicht_-_from_Noob_to_Pro:_Gamestates
 */

#ifndef GAMESTAT_H_
#define GAMESTAT_H_

#include "irrlicht.h"

 class GameState: public irr::IEventReceiver
 {
 protected:
        irr::core::stringc         name;
        bool           active;
        bool           firstEnter;
        irr::scene::ISceneManager* smgr;

 public:
        GameState(void);
        GameState(irr::core::stringc newName);
        virtual ~GameState(void);

        //get Name of GameState
        irr::core::stringc getName(void);

        //is GameState currently running?
        bool  isActive(void);

        //set to true if currently running otherwise false
        void  setActive(bool);

        //initialize the GameState, setup gui or something
        virtual void  initialize(void);

        //clean up the GameState
        virtual void  flush(void);

        //this is called everytime the GameState is entered
        //So load the map when the game starts or something like this
        virtual void  OnEnter(void) = 0;

        //this is called everytime when another GameState is entered
        //so first leave this (e.g. destroy the map)
        virtual void  OnLeave(void) = 0;

        //here we get the user input
        //this method is derived by IEventReceiver
        virtual bool  OnEvent(const irr::SEvent &event) = 0;

        //render all the stuff (gui, scene...)
        virtual void  render(void) = 0;

        virtual void  updateThisState(void) = 0;
 };


#endif /* GAMESTAT_H_ */
