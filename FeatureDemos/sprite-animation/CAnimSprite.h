#ifndef CANIMSPRITE_H_INCLUDED
#define CANIMSPRITE_H_INCLUDED

#include "irrlicht.h"
using namespace irr;

class CAnimSprite : public scene::ISceneNode
{
   
     public:
         CAnimSprite(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, ITimer* tim);
         virtual void Load(char* filename,s32 frmWidth,s32 frmHeight,bool useClrKey=false);
         virtual void Load(char* filename,s32 Ax,s32 Ay,s32 Aw,s32 Ah,s32 frmWidth,s32 frmHeight,bool useClrKey=false);
         virtual void PlayForward() {forward = true;}
         virtual void PlayBackward() {forward = false;}
         virtual void setSpeed(s32 spd)  {speed = spd;}
         virtual void OnRegisterSceneNode();         
         virtual void setFrame(s32 n);
         virtual void OnAnimate();
         virtual void setStartEndFrame( s32 st, s32 ed);         
         virtual s32 GetMaxFrames() { return TotalFrm; }
         virtual void render();
         virtual const core::aabbox3d<f32>& getBoundingBox() const {return Box;}
         virtual u32 getMaterialCount(){return 1;}
         virtual video::SMaterial& getMaterial(u32 i){return Material;}
         
         
     private:
          core::aabbox3d<f32> Box;
          video::S3DVertex    Vertices[4];
          u16                 Indices[12];
          video::SMaterial    Material;
          video::ITexture*    Texture;
          f32                 fWidth,fHeight;
          s32                 crntFrm,TotalFrm;
          s32                 stepww,stephh;
          bool                forward;
          s32                 speed;
          u32                 oldtick;
          s32                 startFrame,endFrame;
          f32                 xCoord,yCoord;
          core::matrix4       Ortho;
          ITimer*             timer;
};


#endif // CANIMSPRITE_H_INCLUDED
