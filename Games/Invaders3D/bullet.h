/*
Bullets.h
*/

#ifndef __BULLET_H__
#define __BULLET_H__

class Bullet{
      public:
          Bullet(ISceneManager* smgr,vector3df pos,float speed){
               this->smgr = smgr;
               mSpeed = speed;
               mPos = pos;
               mYS = 7;
               mXS = 2;
         	   mBill = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(mYS, mXS),mPos);
               mBill->setMaterialFlag(video::EMF_LIGHTING, false);
               mBill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
               mBill->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("Media/Models/lazer.png"));
          }
           
          bool update(){
              bool lOutOfRange = false;
              mPos.X += mSpeed;
              mBill->setPosition(mPos);
              if(mSpeed > 0)
                 if(mPos.X > 100)
                   lOutOfRange = true;
              else
                 if(mPos.X < -100)
                   lOutOfRange = false;
              return lOutOfRange;
          }
          //Returns true if alien
          //false if human
          bool getTarget()
          {
               if(mSpeed > 0)
                   return true;
               return false;
          }           

          bool collidesWith(float x,float y,int pad = 0)
          {
               //Left / right
               if(x + pad > mPos.Z - mXS  && x - pad < mPos.Z + mXS)
               {
                    //Up / down
                    if(y + pad > mPos.X && y - pad < mPos.X)
                    {
                         return true;          
                    }
               }
               return false;
          }
           
          void drop()
          {
               mBill->grab();
               mBill->drop();
               mBill->remove();
          }
          vector3df getPosition(){
                    return mPos;
          }
          
      private:
          float mSpeed;
          int mXS,mYS;
          vector3df mPos;
          scene::ISceneNode* mBill;
          ISceneManager* smgr;
};

#endif
