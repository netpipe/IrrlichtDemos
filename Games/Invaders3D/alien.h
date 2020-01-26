/* Alien.h
*/

#ifndef __ALIEN_H__
#define __ALIEN_H__

#include "bullet.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const unsigned int AlienSize = 7;
class Alien {
      public:
      Alien(ISceneManager* smgr,int posX,int posY,std::list<Bullet>* buList,float lMoveSpeed)
      {
            mActive = true;
            this->buList = buList;
            this->smgr = smgr;
            this->posX = posX;
            this->posY = posY;
            mPos = getStartPosition();
            mPos.Z -= 45;
            /*
            Load Animation
            */
            scene::ISceneNodeAnimator* anim = 0;
	        core::array<video::ITexture*> textures;
		    video::ITexture* t = smgr->getVideoDriver()->getTexture("Media/Models/enemy.png");
		    textures.push_back(t);
		    t = smgr->getVideoDriver()->getTexture("Media/Models/enemy2.png");
		    textures.push_back(t);
	        anim = smgr->createTextureAnimator(textures, 400);

  	        mBill = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(AlienSize, AlienSize),mPos);
            mBill->setMaterialFlag(video::EMF_LIGHTING, false);
            mBill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
            mBill->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("Media/Models/enemy.png"));
            mBill->addAnimator(anim);

            mDir = false; //go right.
            mMoveSpeed = lMoveSpeed;
           	anim->drop();
      }
      ~Alien()
      {
      }
      
      bool killsPlayer()
      {
           if(mPos.X < -35)
             return true;
           return false;
      }
      //Returns true, if the alien reaches the end of the screen
      //So next frame all the aliens will go the other way if we want
      bool update(){
           if(!mActive)
               return false;
           bool lEndFlag = false;
           if(mDir){//go Left
               mPos.Z += mMoveSpeed;
               if(mPos.Z > 55)
                 lEndFlag = true;  
           }else{ //go Right
               mPos.Z -= mMoveSpeed;
               if(mPos.Z < -55)
                 lEndFlag = true;
           }
           mBill->setPosition(mPos);
           
           //Should we fire?
           if(rand()%5000==1)
           {
              Bullet* b = new Bullet(smgr,vector3df(mPos.X+5,mPos.Y,mPos.Z),-0.5);
              buList->push_front(*b);
           }
              
           return lEndFlag;
      }
      
      void changeDirection()
      {
           if(!mActive)
              return;
           mDir = !mDir;
           //Also, drop the invaders down a line
           mPos.X = mPos.X - (AlienSize + 2);
           mMoveSpeed += 0.03;
           posY -= 1;
      }
      
      void drop()
      {
           mActive = false;
           mBill->remove();
      }
      
      vector3df getStartPosition()
      {
            vector3df vec;
            vec.X = (AlienSize + 2) * posY + 10;
            vec.Y = 30;
            vec.Z = (AlienSize + 2) * posX;
            return vec;
      }
      
      private:
          std::list<Bullet>* buList;
          bool mDir;
          bool mActive;
          float mMoveSpeed;
          scene::ISceneNode* mBill;
          ISceneManager* smgr;
          int posX, posY;
      public:
          vector3df mPos;
};

#endif
