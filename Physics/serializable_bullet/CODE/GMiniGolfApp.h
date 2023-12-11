// A game application wrapper class

#ifndef __G_MINI_GOLF_APP_H_INCLUDED__
#define __G_MINI_GOLF_APP_H_INCLUDED__

#include "CGameApp.h"

namespace irr
{
namespace scene
{
  class CBulletAnimatorManager;
  class CBulletObjectAnimator;
  class CBulletWorldAnimator;
};
};
class CWorldEditorGUI;

//!	CGameApp
/** 
*/
class GMiniGolfApp : public CGameApp
{
public:

  //! Ctor
  GMiniGolfApp();
  //! Dtor
  ~GMiniGolfApp();


private:

  //! Base initialization
  bool OnCreate();
  //! Update
  void OnUpdate(u32 timeMs);
  //! Render
  void OnRender();
  //! called if an event happened. returns true if event was processed
  bool OnEvent(SEvent event);

  bool AddSampleScene(scene::ISceneNode* node);

  bool SaveWorld();
  bool LoadWorld();


  scene::ISceneNode*  iWorldNode1;     
  scene::ISceneNode*  iWorldNode2;     

  scene::ISceneNode*       iSphereSceneNode;
  scene::CBulletObjectAnimator* iSphereBulletAnimator;

  core::position2d<f32>   iCenterCursor;
  scene::ICameraSceneNode* iCameraSceneNode;


  scene::CBulletAnimatorManager*   iBulletPhysicsFactory;
  CWorldEditorGUI*         iWorldEditorGUI;
};

#endif //__G_MINI_GOLF_APP_H_INCLUDED__
