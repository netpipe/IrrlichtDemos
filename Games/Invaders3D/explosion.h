/*
Bullets.h
*/

#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include <stdio.h>

class Explosion{
      public:
          Explosion(IrrlichtDevice *device,vector3df pos)
          {
               this->device = device;
               smgr = device->getSceneManager();
               mPos = pos;

               scene::ISceneNodeAnimator* anim = 0;
	           core::array<video::ITexture*> textures;
	           
	           //Adapted from the irrlicht 'demo' sample
           	   for (unsigned short i=1; i<15; ++i)
           	   {
                   char tmp[64];
                   sprintf(tmp,"Media/Models/Explosion%d.png", i);
		           video::ITexture* t = smgr->getVideoDriver()->getTexture(tmp);                  
                   textures.push_back(t);
               } 
	           anim = smgr->createTextureAnimator(textures, 80,false);
               mAnimLength = 80 * 14;
         	   mBill = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(13, 13),mPos);
               mBill->setMaterialFlag(video::EMF_LIGHTING, false);
               mBill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
               mBill->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("Media/Models/Explosion1.png"));
               mBill->addAnimator(anim);
               anim->drop();
               t = device->getTimer();
               mAnimLength = t->getTime() + mAnimLength;
          }
           
          bool update()
          {
               //If Animation has stopped,  return true.
               u32 lTime = t->getTime();
               if(mAnimLength < lTime)
                  return true;
               return false;
          }

           
          void drop(){
               mBill->grab();
               mBill->drop();
               mBill->remove();
          }
          
      private:
          vector3df mPos;
          scene::ISceneNode* mBill;
          ISceneManager* smgr;
          IrrlichtDevice *device;
          u32 mAnimLength;
          ITimer* t;
};

#endif
