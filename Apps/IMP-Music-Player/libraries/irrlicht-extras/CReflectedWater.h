// 
// Reflected Water. 
// sio2 'at' users.sourceforge.net 
// Based on .NET code by DeusXL (Irrlicht forum) 
// 

#ifndef _CREFLECTEDWATER_H 
#define _CREFLECTEDWATER_H 

#include <irrlicht.h> 

using namespace irr; 

class CReflectedWater: public video::IShaderConstantSetCallBack { 

public: 
   //! constructor 
   CReflectedWater(const irr::c8 *name, IrrlichtDevice *device, scene::ISceneManager* smgr, s32 id, f32 tileSize, s32 tileCount, core::dimension2di RenderTargetSize); 

   //! destructor 
   ~CReflectedWater(); 

   void updateRendertarget(scene::ISceneManager* smgr); 

   //! Returns type of the scene node 

   virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData); 

   const irr::c8 *m_name; 

   video::SColorf m_AddedColor; 
   video::SColorf m_MultiColor; 
   f32            m_WaveHeight; 
   f32            m_WaveLength; 
   f32            m_WaveSpeed; 
   f32            m_WaveDisplacement; 
   f32            m_WaveRepetition; 
   f32            m_RefractionFactor; 

   IrrlichtDevice          *m_device; 
   scene::ISceneManager    *m_smgr; 
   scene::ISceneNode       *m_waternode; 
   scene::ICameraSceneNode *m_watercam; 

   video::ITexture         *m_rt; 
}; 

#endif 
