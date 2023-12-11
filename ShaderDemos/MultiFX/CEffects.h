/*-----------------------------------------

Programmed by Fabre Cédric

(Copland/simson) 02/10/2006

Thanks to the Irrlicht team

for this good 3D engine !

This code has not been tested under Linux.

If you find any errors or corrections,

in advance thanks you for any feedbacks.

-------------------------------------------

Contact : cf46@coder3d.com

-----------------------------------------*/

#ifndef _CEFFECTS_
#define _CEFFECTS_

#include  <irrlicht.h>


using namespace irr;



//Classe QuadSceneNode pour dessiner un Quad perso

class CQuadSceneNode : public scene::ISceneNode

{

public:

   CQuadSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
   virtual void OnRegisterSceneNode();

   virtual void render();

   virtual const core::aabbox3d<f32>& getBoundingBox() const;

   virtual u32 getMaterialCount() const;
   virtual video::SMaterial& getMaterial(u32 i);

private :

   core::aabbox3d<f32> Box;

   video::S3DVertex VertexQuad[4];
   u16 Indices[6];

   video::SMaterial Material;
   void Update();

};


class CEffects
{
public:

    void CreateGlow(scene::ICameraSceneNode* CamGlow,scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 GlowSize);

    void RenderGlow(scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 gA,s32 gR,s32 gG,s32 gB);

    void DeleteGlow();

    void CreateDream(scene::ICameraSceneNode* CamDream,scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 DreamSize);
    void RenderDream(scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 gA,s32 gR,s32 gG,s32 gB);
    void DeleteDream();

    void CreateVision(scene::ICameraSceneNode* CamVision,scene::ISceneManager* smgr,video::IVideoDriver* driver,irr::c8* NoiseFile,s32 VisionSize);
    void RenderVision(scene::ISceneManager* smgr,video::IVideoDriver* driver,s32 gA,s32 gR,s32 gG,s32 gB);
    void DeleteVision();

    void RenderNothing();

private:
    CQuadSceneNode* QuadGlow[4];
    video::ITexture* RTexGlow;

    CQuadSceneNode* QuadDream[7];
    video::ITexture* RTexDream;

    CQuadSceneNode* QuadVision[2];
    video::ITexture* RTexVision;
};

#endif
