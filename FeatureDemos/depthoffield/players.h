#ifndef PLAYERS_H
#define PLAYERS_H
#include "savegames.h"

SAppContext Data;
EventReceiver EventRec(Data);
unsigned int players=0;

class player {
public:
   player (irr::core::vector3df pos, irr::IrrlichtDevice* dev, bool md2, char* meshfile);
   void setJoystick (joystick* j) {joys=j;}
   void* getInput ();
   int getInputType () {return input;}
   joystick* getJoystick () {return joys;}
   irr::scene::IAnimatedMesh* getmesh () {return playermesh;}
   irr::scene::IAnimatedMeshSceneNode* getnode () {return playernode;}
   ~player() {
      playernode->remove();
      players--;
   }
   void setInput (int a);
private:
   irr::scene::IAnimatedMesh* playermesh;
   irr::scene::IAnimatedMeshSceneNode* playernode;
   float life;
   float stamina;
   int kills;
   int input;
   joystick* joys;
};

player::player(irr::core::vector3df pos, irr::IrrlichtDevice* dev, bool md2, char* meshfile) {
   playermesh = device->getSceneManager()->getMesh(meshfile);
   life = 1000;
   stamina = 100;
   if (playermesh)
      playernode = device->getSceneManager()->addAnimatedMeshSceneNode(playermesh);
   if (playernode) {
      playernode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
      if (md2) {
         playernode->setMD2Animation(irr::scene::EMAT_STAND);
         playernode->setMaterialTexture( 0, dev->getVideoDriver()->getTexture("./media/sydney.bmp") );
      }
      playernode->setPosition(pos);
   }
}

void player::setInput (int a) {
   switch (a) {
      case 0 :
         input=0;
         break;
      case 1 :
         input=1;
         break;
      case 2 :
         input=2;
         break;
      default:
         break;
   }
}

void* player::getInput () {
   if (input==2)
      return &EventRec;/*
   else if (input)
   else if (!input)*/
}

bool init_player(player* made_player, joystick* joy=NULL, player* play1=NULL) {
   if (players==1) {
      if ((!play1)&&joy) {
         made_player->setInput(joy->getType());
         made_player->setJoystick(joy);
      }
      else if (joy) {
         if (play1->getInputType()!=joy->getType()) {
            made_player->setInput(joy->getType());
            made_player->setJoystick(joy);
         }
         else {
#ifdef DEBUG
            std::cout << "Error 18: No input devices available for the second player!" << std::endl;
#endif
            return 0;
         }
      }
      else if (play1->getInputType()!=2) made_player->setInput(2);
      else {
#ifdef DEBUG
         std::cout << "Error 18: No input devices available for the second player!" << std::endl;
#endif
         return 0;
      }
      players++;
      return 1;
   }
   else if (players==0) {
      if (!joy) made_player->setInput(2);
      else if (joy) {
         made_player->setInput(joy->getType());
         made_player->setJoystick(joy);
      }
      else {
#ifdef DEBUG
         cout << "Error 19: Encountered a problem while assigning an input device to the first player!" << endl;
#endif
         return 0;
      }
      players++;
      return 1;
   }
   else
      return 0;
}
#endif
