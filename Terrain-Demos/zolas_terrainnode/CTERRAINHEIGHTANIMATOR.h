#ifndef CTERRAINHEIGHTANIMATOR_H_INCLUDED 
#define CTERRAINHEIGHTANIMATOR_H_INCLUDED 

#include <irrlicht.h> 
#include "CLMTerrainSceneNode.h" 

namespace irr 
{ 
   namespace scene 
   { 
      // animator that clamps the animated node to the terrain height 
      class CTerrainHeightAnimator : public ISceneNodeAnimator 
      { 
      protected: 

         // the terrian node 
         CLMTerrainSceneNode* d_terrain; 
         // minimal height over ground 
         f32 d_h; 
         // fit the height of the node or let it fly 
         bool d_fit; 
          
         // to speed things up a little  some cached memory, 
         // so anim node doesn't have to allocate everytime 
         matrix4 Trans; 
         matrix4 invTrans; 
         vector3df pos; 
      public: 

         /*! 
         \brief constructor 
         \param terrain, the terrain we want to follow 
         \param h, min height over ground 
         \param fit, if true the clamp height to terrain height otherwise allow to fly 
         */ 
         CTerrainHeightAnimator(CLMTerrainSceneNode* terrain, f32 h, bool fit) 
            : d_terrain(terrain),d_h(h),d_fit(fit) 
         { } 

         // destructor 
         virtual ~CTerrainHeightAnimator(){} 

         //! Animates a scene node. 
         //! \param node: Node to animate. 
         //! \param timeMs: Current time in milli seconds. 
         virtual void animateNode(ISceneNode* node, u32 timeMs) 
         { 
            pos.set(node->getPosition()); 

            Trans=d_terrain->getAbsoluteTransformation(); 
             
            Trans.getInverse(invTrans); 
            invTrans.transformVect(pos); 

            float h = d_terrain->getHeight(pos.X,pos.Z)+d_h; 
            pos.Y = d_fit ? h : (pos.Y > h ? pos.Y : h); 

            Trans.transformVect(pos); 
            node->setPosition(pos); 
         } 
      }; 
   } 
} 


#endif 
