// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CRTTSkyBoxSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"
#include "S3DVertex.h"
 

namespace irr
{
namespace scene
{

//! constructor
CRTTSkyBoxSceneNode::CRTTSkyBoxSceneNode(core::dimension2d<u32> size, ISceneNode* parent, ISceneManager* mgr, s32 id)
: ISceneNode(parent, mgr, id)
{
 
   this->setAutomaticCulling(scene::E_CULLING_TYPE::EAC_OFF);
   // create indices

   Indices[0] = 0;
   Indices[1] = 1;
   Indices[2] = 2;
   Indices[3] = 0;
   Indices[4] = 2;
   Indices[5] = 3;

   // create material
   
   video::SMaterial mat;
   mat.Lighting = false;
   mat.ZBuffer = false;
   mat.ZWriteEnable = false;
   //mat.BilinearFilter = true;
   mat.setFlag(video::EMF_BILINEAR_FILTER, true);
   /* Hey, I am no artist, but look at that
      cool ASCII art I made! ;)  (Niko)

       -111         111
          /6--------/5        y
         /  |      / |        ^  z
        /   |   11-1 |        | /
  -11-1 3---------2  |        |/
        |   7- - -| -4 1-11    *---->x
        | -1-11   |  /       3-------|2
        |/        | /         |    //|       
        0---------1/          |  //  |   
     -1-1-1     1-1-1         |//    |   
                            0--------1
   */

   f32 onepixel = 1.0f / (size.Width * 1.5f);

    // create the render targets   
    for (s32 n=0; n<6; ++n)
      rt[n] = mgr->getVideoDriver()->addRenderTargetTexture(size);
    // create camera (remember last camera)
    scene::ICameraSceneNode *currentcam = mgr->getActiveCamera();
    skyCam = mgr->addCameraSceneNode();
    skyCam->setFarValue(100000.0f);   
    skyCam->setAspectRatio(1.0f);
    skyCam->setFOV(1.573f);       // dunno why, but it works here
    mgr->setActiveCamera(currentcam);

   f32 l = 10.0f;
   f32 t = 1.0f - onepixel;
   f32 o = 0.0f + onepixel;

   Material[0] = mat;
   Material[0].setTexture(0,rt[4]);
   Vertices[0] = video::S3DVertex(-l,-l,-l, 0,0,1, video::SColor(255,255,255,255), t, t);
   Vertices[1] = video::S3DVertex( l,-l,-l, 0,0,1, video::SColor(255,255,255,255), o, t);
   Vertices[2] = video::S3DVertex( l, l,-l, 0,0,1, video::SColor(255,255,255,255), o, o);
   Vertices[3] = video::S3DVertex(-l, l,-l, 0,0,1, video::SColor(255,255,255,255), t, o);

   // create left side

   Material[1] = mat;
   Material[1].setTexture(0,rt[2]);
   Vertices[4] = video::S3DVertex( l,-l,-l, -1,0,0, video::SColor(255,255,255,255), t, t);
   Vertices[5] = video::S3DVertex( l,-l, l, -1,0,0, video::SColor(255,255,255,255), o, t);
   Vertices[6] = video::S3DVertex( l, l, l, -1,0,0, video::SColor(255,255,255,255), o, o);
   Vertices[7] = video::S3DVertex( l, l,-l, -1,0,0, video::SColor(255,255,255,255), t, o);

   // create back side

   Material[2] = mat;
   Material[2].setTexture(0,rt[5]);
   Vertices[8]  = video::S3DVertex( l,-l, l, 0,0,-1, video::SColor(255,255,255,255), t, t);
   Vertices[9]  = video::S3DVertex(-l,-l, l, 0,0,-1, video::SColor(255,255,255,255), o, t);
   Vertices[10] = video::S3DVertex(-l, l, l, 0,0,-1, video::SColor(255,255,255,255), o, o);
   Vertices[11] = video::S3DVertex( l, l, l, 0,0,-1, video::SColor(255,255,255,255), t, o);

   // create right side

   Material[3] = mat;
   Material[3].setTexture(0,rt[3]);
   Vertices[12] = video::S3DVertex(-l,-l, l, 1,0,0, video::SColor(255,255,255,255), t, t);
   Vertices[13] = video::S3DVertex(-l,-l,-l, 1,0,0, video::SColor(255,255,255,255), o, t);
   Vertices[14] = video::S3DVertex(-l, l,-l, 1,0,0, video::SColor(255,255,255,255), o, o);
   Vertices[15] = video::S3DVertex(-l, l, l, 1,0,0, video::SColor(255,255,255,255), t, o);

   // create top side

   Material[4] = mat;
   Material[4].setTexture(0,rt[0]);
   Vertices[16] = video::S3DVertex( l, l, l, 0,-1,0, video::SColor(255,255,255,255), t, o);
   Vertices[17] = video::S3DVertex(-l, l, l, 0,-1,0, video::SColor(255,255,255,255), t, t);
   Vertices[18] = video::S3DVertex(-l, l,-l, 0,-1,0, video::SColor(255,255,255,255), o, t);
   Vertices[19] = video::S3DVertex( l, l,-l, 0,-1,0, video::SColor(255,255,255,255), o, o);

   // create bottom side

   Material[5] = mat;
   Material[5].setTexture(0,rt[1]);
   Vertices[20] = video::S3DVertex(-l,-l, l, 0,1,0, video::SColor(255,255,255,255), o, t);
   Vertices[21] = video::S3DVertex( l,-l, l, 0,1,0, video::SColor(255,255,255,255), o, o);
   Vertices[22] = video::S3DVertex( l,-l,-l, 0,1,0, video::SColor(255,255,255,255), t, o);
   Vertices[23] = video::S3DVertex(-l,-l,-l, 0,1,0, video::SColor(255,255,255,255), t, t);
   
}



//! destructor
CRTTSkyBoxSceneNode::~CRTTSkyBoxSceneNode()
{
    // drop render targets
    for (s32 n=0; n<6; ++n)
      rt[n]->drop();
     
    // drop sky camera
    skyCam->remove();
}

void CRTTSkyBoxSceneNode::renderToSkyBox(core::vector3df pos, video::SColor col, bool self)
{
     
     ICameraSceneNode *lastcam = SceneManager->getActiveCamera();
     SceneManager->setActiveCamera(skyCam);
     bool vis; // is visible
     if (!self)
     {
         vis = isVisible();
         setVisible(false);         
     }
     skyCam->setPosition(pos);
     
     // render up
     SceneManager->getVideoDriver()->setRenderTarget(rt[0], true, true, col);
     skyCam->setTarget(pos + core::vector3df(0,10,0));
     skyCam->updateAbsolutePosition();
     SceneManager->drawAll();

     // render down
     SceneManager->getVideoDriver()->setRenderTarget(rt[1], true, true, col);
     skyCam->setTarget(pos + core::vector3df(0,-10,0));
     skyCam->updateAbsolutePosition();
     SceneManager->drawAll();

     // render left
     SceneManager->getVideoDriver()->setRenderTarget(rt[2], true, true, col);
     skyCam->setTarget(pos + core::vector3df(10,0,0));
     skyCam->updateAbsolutePosition();
     SceneManager->drawAll();

     // render right
     SceneManager->getVideoDriver()->setRenderTarget(rt[3], true, true, col);
     skyCam->setTarget(pos + core::vector3df(-10,0,0));
     skyCam->updateAbsolutePosition();
     SceneManager->drawAll();

     // render front
     SceneManager->getVideoDriver()->setRenderTarget(rt[4], true, true, col);
     skyCam->setTarget(pos + core::vector3df(0,0,-10));
     skyCam->updateAbsolutePosition();
     SceneManager->drawAll();

     // render back
     SceneManager->getVideoDriver()->setRenderTarget(rt[5], true, true, col);
     skyCam->setTarget(pos + core::vector3df(0,0,10));
     skyCam->updateAbsolutePosition();
     SceneManager->drawAll();

     if (!self) setVisible(vis);
     SceneManager->getVideoDriver()->setRenderTarget(0);
     
     SceneManager->setActiveCamera(lastcam);
}


//! renders the node.
void CRTTSkyBoxSceneNode::render()
{
   video::IVideoDriver* driver = SceneManager->getVideoDriver();
   scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

   if (!camera || !driver)
      return;

   core::matrix4 mat;
   mat.setTranslation(camera->getAbsolutePosition());

   driver->setTransform(video::ETS_WORLD, mat);
   
   for (s32 i=0; i<6; ++i)
   {
      driver->setMaterial(Material[i]);
      driver->drawIndexedTriangleList(&Vertices[i*4], 4, Indices, 2);
   }
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CRTTSkyBoxSceneNode::getBoundingBox() const
{
   return Box;
}


void CRTTSkyBoxSceneNode::OnRegisterSceneNode()
{
   if (IsVisible)
      SceneManager->registerNodeForRendering(this, ESNRP_SKY_BOX);

   ISceneNode::OnRegisterSceneNode();
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CRTTSkyBoxSceneNode::getMaterial(s32 i)
{
   return Material[i];
}


//! returns amount of materials used by this scene node.
s32 CRTTSkyBoxSceneNode::getMaterialCount()
{
   return 6;
}

} // end namespace scene
} // end namespace irr 
