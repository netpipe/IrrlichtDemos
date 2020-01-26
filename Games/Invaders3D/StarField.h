/* starfield.h */
#ifndef __STARS_H__
#define __STARS_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
using namespace std;

class StarField {
        public:
        StarField(ISceneManager* smgr,int lSPCount)
        {
              //Star Points
              this->smgr = smgr;
              
              for(unsigned short i;i < lSPCount;i++)
              {
                   StarPoint* s = new StarPoint();
                   s->vec.Z = rand()%160 - 69;
                   s->vec.Y = 20 + rand()%3;
                   s->vec.X = rand()%100 - 40;
                   s->speed = (-(rand()%100)/100.0)-0.1;    
                   s->color = SColor(255,100,130,0);
                   //Load a star model
                   int lSize = rand()%4 + 1;
     	           s->node = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(lSize,lSize));
                   s->node->setMaterialFlag(video::EMF_LIGHTING, false);
                   s->node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

                   /*
                   This whole bit of code is a placeholder, for what ever reason
                   I could not get a switch statment to work here, it would simply
                   stop irrlicht from loading a second texture...  No clue why
                   But probably my fault.
                   */
                     if(i > 14)
                          s->node->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("Media/Models/smoke.bmp")); 
                     else {
                         if(i > 6 && i < 14)
                         {
                              s->node->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("Media/Models/particlewhite.bmp"));                     
                         }
                         else{
                         if(i > 6 && i < 6)
                              s->node->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("Media/Models/portal2.bmp"));
                         else
                              s->node->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("Media/Models/particle.bmp"));                 
                         }
                     }

                   if ( s->node )
                   {
                        s->node->setPosition(s->vec);
                   }  
                   spList.push_front(*s);
              }
              //Create blob stars
              //Create debres
              //Create Rotating Planents
        }
        
        void update()
        {
              //Update movement...
              //if(iter->y > someval   
              updateStarPoint();
        }
        
        void updateStarPoint()
        {
              std::list<StarPoint>::iterator iter;
              for (iter=spList.begin(); iter != spList.end(); iter++)
              {
                  iter->vec.X += iter->speed;
                  //Update Position
                  //If > some value,  move back
                  if(iter->vec.X < -100){
                       iter->vec.Z = rand()%160 - 69;
                       iter->vec.X = 60;
                  }
                  iter->node->setPosition(iter->vec);
                  
              }             
        }

        void drop()
        {
             //Why would we want to?
        }
        
        private:
             struct StarPoint{
                    vector3df vec;
                    float speed;
                    SColor color;
                    ISceneNode* node;
             };
             std::list<StarPoint> spList;
             ISceneManager* smgr;

        
};
#endif
