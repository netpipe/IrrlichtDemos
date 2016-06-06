// GUI

#ifndef __C_WORLD_EDITOR_GUI_H_INCLUDED__
#define __C_WORLD_EDITOR_GUI_H_INCLUDED__

//! Irrlicht
#include "irrlicht.h"
using namespace irr;


#define gCWorldEditorGUIAssetsDir "assets\\"

const s32 gCWorldEditorRootID = 1000;
const s32 gCWorldEditorMainMenuID = 2000;
const s32 gCWorldEditorMainToolbarID = 3000;
const s32 gCWorldEditorObjsListWindowID = 4000;
const s32 gCWorldEditorObjParamWindowID = 5000;

//!	CWorldEditorGUI
/** 
*/
class CWorldEditorGUI : public IEventReceiver
{
public:

  //! Ctor
  CWorldEditorGUI();
  //! Dtor
  ~CWorldEditorGUI();

  bool  Create(IrrlichtDevice* pIrrDevice);

  bool  IsActive()                { return iIsActive; }
  void  SetActive(bool pActive);

private:

  void  Activate();
  void  Deactivate();

  //! Toggles between various cameras
  void  SetActiveCamera(scene::ICameraSceneNode* newActive);
  void  SynchronizeCamera(scene::ICameraSceneNode* from, scene::ICameraSceneNode* to);

  //! Helper GUI functions
  gui::IGUIContextMenu* CreateMainMenu(gui::IGUIEnvironment* GUIEnv);
  gui::IGUIToolBar*     CreateMainToolbar(gui::IGUIEnvironment* GUIEnv);
  gui::IGUIWindow*      CreateObjectsListWindow(gui::IGUIEnvironment* GUIEnv);
  gui::IGUIWindow*      CreateObjectParamsWindow(gui::IGUIEnvironment* GUIEnv);

  void RefreshObjectsListWindow();


  bool OnEvent(SEvent event);


  //! Irrlicht related objects
  IrrlichtDevice*          iIrrDevice;
  video::IVideoDriver*     iIrrVideoDriver;
  scene::ISceneManager*    iIrrSceneManager;
  gui::IGUIEnvironment*    iIrrGUIEnvironment;

  //! Activity flag
  bool                     iIsActive;

  //! Editor's camera node
  scene::ICameraSceneNode* iWorldCameraSceneNode;
  scene::ICameraSceneNode* iOldCameraSceneNode;

  gui::IGUIFont*           iOldFont;
  gui::IGUISkin*           iOldSkin;

  //! GUI elements
  gui::IGUIContextMenu*    iMainMenuGUIElement;
  gui::IGUIToolBar*        iMainToolbarGUIElement;
  gui::IGUIWindow*         iObjsListWindowGUIElement;
  gui::IGUIWindow*         iObjParamsWindowGUIElement;


  //! Currently selected model
  scene::IAnimatedMeshSceneNode* iCurrentModel;



};

#endif //__C_WORLD_EDITOR_GUI_H_INCLUDED__
