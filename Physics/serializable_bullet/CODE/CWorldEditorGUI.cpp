#include "CWorldEditorGUI.h"

//------------------------------------------------------------------------------
//! Ctor
//! Base initialization
CWorldEditorGUI::CWorldEditorGUI() :
  iIrrDevice(NULL), iIrrVideoDriver(NULL), iIrrSceneManager(NULL),
  iIrrGUIEnvironment(NULL), iCurrentModel(NULL)
{
  iMainMenuGUIElement = NULL;
  iMainToolbarGUIElement = NULL;
  iObjsListWindowGUIElement = NULL;
  iObjParamsWindowGUIElement = NULL;

  iWorldCameraSceneNode = NULL;
  iOldCameraSceneNode = NULL;
  iOldSkin = NULL;
  iOldFont = NULL;

  iIsActive = false;
}

//------------------------------------------------------------------------------
//! Dtor
//! 
CWorldEditorGUI::~CWorldEditorGUI()
{

}

//------------------------------------------------------------------------------
//! Create
//! 
bool CWorldEditorGUI::Create(IrrlichtDevice* pIrrDevice)
{
  // set irrlicht objects
  iIrrDevice = pIrrDevice;
  iIrrVideoDriver = pIrrDevice->getVideoDriver();
  iIrrSceneManager = pIrrDevice->getSceneManager();
  iIrrGUIEnvironment = pIrrDevice->getGUIEnvironment();

  return true;
}

//------------------------------------------------------------------------------
//! Activate
//! 
void CWorldEditorGUI::Activate()
{
  Deactivate();

  // set custom skin
  if (0)
  {
    iOldSkin = iIrrGUIEnvironment->getSkin();

  }

  // set a nicer font
  gui::IGUIFont* font = 
    iIrrGUIEnvironment->getFont(gCWorldEditorGUIAssetsDir"fonthaettenschweiler.bmp");
  if (font)
  {
    iOldFont = iIrrGUIEnvironment->getSkin()->getFont();
    iIrrGUIEnvironment->getSkin()->setFont(font);
  }


  // create main menu
  iMainMenuGUIElement = CreateMainMenu(iIrrGUIEnvironment);

  // create toolbar
  iMainToolbarGUIElement = CreateMainToolbar(iIrrGUIEnvironment);

  // add list
  iObjsListWindowGUIElement = CreateObjectsListWindow(iIrrGUIEnvironment);

  // add params window
  iObjParamsWindowGUIElement = CreateObjectParamsWindow(iIrrGUIEnvironment);


  // disable alpha
  for (s32 i = 0; i < gui::EGDC_COUNT; i++)
  {
    video::SColor col = iIrrGUIEnvironment->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
    col.setAlpha(255);
    iIrrGUIEnvironment->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
  }

  //// add skybox
  //SkyBox = smgr->addSkyBoxSceneNode(
  //  iIrrVideoDriver->getTexture("irrlicht2_up.jpg"),
  //  iIrrVideoDriver->getTexture("irrlicht2_dn.jpg"),
  //  iIrrVideoDriver->getTexture("irrlicht2_lf.jpg"),
  //  iIrrVideoDriver->getTexture("irrlicht2_rt.jpg"),
  //  iIrrVideoDriver->getTexture("irrlicht2_ft.jpg"),
  //  iIrrVideoDriver->getTexture("irrlicht2_bk.jpg"));


  // add a camera scene node
  iOldCameraSceneNode = iIrrSceneManager->getActiveCamera();

  iWorldCameraSceneNode = iIrrSceneManager->addCameraSceneNodeMaya();
  iIrrSceneManager->setActiveCamera(iWorldCameraSceneNode);
  // sync camera's params
  SynchronizeCamera(iOldCameraSceneNode, iWorldCameraSceneNode);

  iIsActive = true;
}


//------------------------------------------------------------------------------
//! Deactivate
//! 
void CWorldEditorGUI::Deactivate()
{
  if (iMainMenuGUIElement != NULL) { iMainMenuGUIElement->remove(); iMainMenuGUIElement = NULL; }
  if (iMainToolbarGUIElement != NULL) { iMainToolbarGUIElement->remove(); iMainToolbarGUIElement = NULL; }
  if (iObjsListWindowGUIElement != NULL) { iObjsListWindowGUIElement->remove(); iObjsListWindowGUIElement = NULL; }
  if (iObjParamsWindowGUIElement != NULL) { iObjParamsWindowGUIElement->remove(); iObjParamsWindowGUIElement = NULL; }

  if (iOldFont != NULL) { iIrrGUIEnvironment->getSkin()->setFont(iOldFont); iOldFont = NULL; }
  if (iOldSkin != NULL) { iIrrGUIEnvironment->setSkin(iOldSkin); iOldSkin = NULL; }

  SynchronizeCamera(iWorldCameraSceneNode, iOldCameraSceneNode);
  if (iOldCameraSceneNode != NULL) { iIrrSceneManager->setActiveCamera(iOldCameraSceneNode); iOldCameraSceneNode = NULL; }
  if (iWorldCameraSceneNode != NULL) { iWorldCameraSceneNode->remove(); iWorldCameraSceneNode = NULL; }

  iIsActive = false;
}

//------------------------------------------------------------------------------
//! CreateMainMenu
//! Creates a main menu object 
void CWorldEditorGUI::SynchronizeCamera(scene::ICameraSceneNode* from, 
                                        scene::ICameraSceneNode* to)
{
  if (from != NULL && to != NULL)
  {
    to->setPosition(from->getPosition());
    to->setRotation(from->getRotation());
    to->setProjectionMatrix(from->getProjectionMatrix());
    to->setTarget(from->getTarget());
  }
}

//------------------------------------------------------------------------------
//! CreateMainMenu
//! Creates a main menu object 
gui::IGUIContextMenu* CWorldEditorGUI::CreateMainMenu(gui::IGUIEnvironment* GUIEnv)
{
  gui::IGUIContextMenu* menu = NULL;

  // create menu
  menu = GUIEnv->addMenu();
  menu->addItem(L"File", -1, true, true);
  menu->addItem(L"View", -1, true, true);
  menu->addItem(L"Camera", -1, true, true);
  menu->addItem(L"Help", -1, true, true);

  gui::IGUIContextMenu* submenu;
  submenu = menu->getSubMenu(0);
  submenu->addItem(L"Open iCurrentModel File & Texture...", 100);
  submenu->addItem(L"Set iCurrentModel Archive...", 101);
  submenu->addSeparator();
  submenu->addItem(L"Quit", 200);

  submenu = menu->getSubMenu(1);
  submenu->addItem(L"toggle sky box visibility", 300);
  submenu->addItem(L"toggle model debug information", -1, true, true);
  submenu->addItem(L"model material", -1, true, true );

  submenu = menu->getSubMenu(1)->getSubMenu(1);
  submenu->addItem(L"Off", 400);
  submenu->addItem(L"Bounding Box", 410);
  submenu->addItem(L"Normals", 420);
  submenu->addItem(L"Skeleton", 430);
  submenu->addItem(L"Wire overlay", 440);
  submenu->addItem(L"Half-Transparent", 450);
  submenu->addItem(L"Buffers bounding boxes", 460);
  submenu->addItem(L"All", 499);

  submenu = menu->getSubMenu(1)->getSubMenu(2);
  submenu->addItem(L"Solid", 610);
  submenu->addItem(L"Transparent", 620);
  submenu->addItem(L"Reflection", 630);

  submenu = menu->getSubMenu(2);
  submenu->addItem(L"Maya Style", 1000);
  submenu->addItem(L"First Person", 1100);

  submenu = menu->getSubMenu(3);
  submenu->addItem(L"About", 500);

  return menu;
}

//------------------------------------------------------------------------------
//! CreateMainMenu
//! Creates a main toolbar object 
gui::IGUIToolBar* CWorldEditorGUI::CreateMainToolbar(gui::IGUIEnvironment* GUIEnv)
{
  gui::IGUIToolBar* toolbar = NULL;

  toolbar = GUIEnv->addToolBar();

  video::ITexture* image = GUIEnv->getVideoDriver()->getTexture(gCWorldEditorGUIAssetsDir"open.png");
  toolbar->addButton(1102, 0, L"Open a model",image, 0, false, true);

  image = GUIEnv->getVideoDriver()->getTexture(gCWorldEditorGUIAssetsDir"tools.png");
  toolbar->addButton(1104, 0, L"Open Toolset",image, 0, false, true);

  image = GUIEnv->getVideoDriver()->getTexture(gCWorldEditorGUIAssetsDir"zip.png");
  toolbar->addButton(1105, 0, L"Set iCurrentModel Archive",image, 0, false, true);

  image = GUIEnv->getVideoDriver()->getTexture(gCWorldEditorGUIAssetsDir"help.png");
  toolbar->addButton(1103, 0, L"Open Help", image, 0, false, true);

  // create a combobox with some senseless texts
  gui::IGUIComboBox* box = GUIEnv->addComboBox(
    core::rect<s32>(250,4,350,23), 
    toolbar, 
    108
  );
  box->addItem(L"No filtering");
  box->addItem(L"Bilinear");
  box->addItem(L"Trilinear");
  box->addItem(L"Anisotropic");
  box->addItem(L"Isotropic");

  // create fps text
  gui::IGUIStaticText* fpstext = GUIEnv->addStaticText(
    L"", 
    core::rect<s32>(400,4,570,23), 
    true, 
    false, 
    toolbar
  );


  return toolbar;
}

//------------------------------------------------------------------------------
//! CreateObjectsListToolBox
//! Creates a toolbox window to represent object parameters 
gui::IGUIWindow* CWorldEditorGUI::CreateObjectsListWindow(gui::IGUIEnvironment* GUIEnv)
{
  // create the toolbox window
  gui::IGUIWindow* wnd = GUIEnv->addWindow(
    core::rect<s32>(900,25,1100,480),
    false, 
    L"Scene nodes", 
    0, 
    gCWorldEditorObjsListWindowID);

  // create tab control and tabs
  gui::IGUITabControl* tab = GUIEnv->addTabControl(
    core::rect<s32>(2,20,800-602,480-7), wnd, true, true);

  gui::IGUITab* t1 = tab->addTab(L"Nodes List");

  // add list box
  gui::IGUIListBox* listBox = GUIEnv->addListBox(core::rect<s32>(10,10,160,370), t1, -1, true);

  scene::ISceneNode* rootScene = iIrrSceneManager->getRootSceneNode();

  core::list<scene::ISceneNode*>::Iterator it = rootScene->getChildren().begin();
  for (; it != rootScene->getChildren().end(); it++)
  {
    core::stringw elementInfo = L"";
    elementInfo += (*it)->getID();
    elementInfo += ": ";
    elementInfo += (*it)->getName();

    listBox->addItem(elementInfo.c_str());
  }



  //// add some edit boxes and a button to tab one
  //GUIEnv->addEditBox(L"1.0", core::rect<s32>(40,50,130,70), true, t1, 901);
  //GUIEnv->addEditBox(L"1.0", core::rect<s32>(40,80,130,100), true, t1, 902);
  //GUIEnv->addEditBox(L"1.0", core::rect<s32>(40,110,130,130), true, t1, 903);

  //GUIEnv->addButton(core::rect<s32>(10,150,100,190), t1, 1101, L"set");

  //// add senseless checkbox
  //GUIEnv->addCheckBox(true, core::rect<s32>(10,220,200,240), t1, -1, L"Senseless Checkbox");

  //// add undocumentated transparent control
  //GUIEnv->addStaticText(L"Transparent Control:", core::rect<s32>(10,240,150,260), true, false, t1);
  //gui::IGUIScrollBar* scrollbar = GUIEnv->addScrollBar(true, core::rect<s32>(10,260,150,275), t1, 104);
  //scrollbar->setMax(255);
  //scrollbar->setPos(255);

  return wnd;
}

//------------------------------------------------------------------------------
//! RefreshObjectsListWindow
//! Creates a toolbox window to represent object parameters 
void CWorldEditorGUI::RefreshObjectsListWindow()
{

}

//------------------------------------------------------------------------------
//! CreateObjectParamsToolBox
//! Creates a toolbox window to represent object parameters 
gui::IGUIWindow* CWorldEditorGUI::CreateObjectParamsWindow(gui::IGUIEnvironment* GUIEnv)
{
  // create the toolbox window
  gui::IGUIWindow* wnd = GUIEnv->addWindow(core::rect<s32>(600,25,800,480),
    false, L"Toolset", 0, 5000);

  // create tab control and tabs
  gui::IGUITabControl* tab = GUIEnv->addTabControl(
    core::rect<s32>(2,20,800-602,480-7), wnd, true, true);

  gui::IGUITab* t1 = tab->addTab(L"Scale");

  // add some edit boxes and a button to tab one
  GUIEnv->addEditBox(L"1.0", core::rect<s32>(40,50,130,70), true, t1, 901);
  GUIEnv->addEditBox(L"1.0", core::rect<s32>(40,80,130,100), true, t1, 902);
  GUIEnv->addEditBox(L"1.0", core::rect<s32>(40,110,130,130), true, t1, 903);

  GUIEnv->addButton(core::rect<s32>(10,150,100,190), t1, 1101, L"set");

  // add senseless checkbox
  GUIEnv->addCheckBox(true, core::rect<s32>(10,220,200,240), t1, -1, L"Senseless Checkbox");

  // add undocumentated transparent control
  GUIEnv->addStaticText(L"Transparent Control:", core::rect<s32>(10,240,150,260), true, false, t1);
  gui::IGUIScrollBar* scrollbar = GUIEnv->addScrollBar(true, core::rect<s32>(10,260,150,275), t1, 104);
  scrollbar->setMax(255);
  scrollbar->setPos(255);

  return wnd;
}

//------------------------------------------------------------------------------
//! SetVisible
//! 
bool CWorldEditorGUI::OnEvent(SEvent event)
{
  if (!iIsActive)
  {
    return false;
  }

  // Escape swaps Camera Input
  if (event.EventType == EET_KEY_INPUT_EVENT &&
      event.KeyInput.Key == irr::KEY_ESCAPE &&
      event.KeyInput.PressedDown == false)
  {
    if (iIrrDevice)
    {
      scene::ICameraSceneNode* camera = iIrrSceneManager->getActiveCamera();
      if (camera == iWorldCameraSceneNode)
      {
        camera->setInputReceiverEnabled(!camera->isInputReceiverEnabled());
      }
      return true;
    }
  }

  if (event.EventType == EET_GUI_EVENT)
  {
    s32 id = event.GUIEvent.Caller->getID();
    gui::IGUIEnvironment* env = iIrrGUIEnvironment;

    switch(event.GUIEvent.EventType)
    {
    case gui::EGET_MENU_ITEM_SELECTED:
      {
        // a menu item was clicked

        gui::IGUIContextMenu* menu = (gui::IGUIContextMenu*)event.GUIEvent.Caller;
        s32 id = menu->getItemCommandId(menu->getSelectedItem());

        switch(id)
        {
        case 100: // File -> Open iCurrentModel
          env->addFileOpenDialog(L"Please select a model file to open");
          break;
        case 101: // File -> Set iCurrentModel Archive
          env->addFileOpenDialog(L"Please select your game archive/directory");
          break;
        case 200: // File -> Quit
          iIrrDevice->closeDevice();
          break;
        //case 300: // View -> Skybox
        //  SkyBox->setVisible(!SkyBox->isVisible());
        //  break;
        //case 400: // View -> Debug Information
        //  if (iCurrentModel)
        //    iCurrentModel->setDebugDataVisible(scene::EDS_OFF);
        //  break;
        //case 410: // View -> Debug Information
        //  if (iCurrentModel)
        //    iCurrentModel->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(iCurrentModel->isDebugDataVisible()^scene::EDS_BBOX));
        //  break;
        //case 420: // View -> Debug Information
        //  if (iCurrentModel)
        //    iCurrentModel->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(iCurrentModel->isDebugDataVisible()^scene::EDS_NORMALS));
        //  break;
        //case 430: // View -> Debug Information
        //  if (iCurrentModel)
        //    iCurrentModel->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(iCurrentModel->isDebugDataVisible()^scene::EDS_SKELETON));
        //  break;
        //case 440: // View -> Debug Information
        //  if (iCurrentModel)
        //    iCurrentModel->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(iCurrentModel->isDebugDataVisible()^scene::EDS_MESH_WIRE_OVERLAY));
        //  break;
        //case 450: // View -> Debug Information
        //  if (iCurrentModel)
        //    iCurrentModel->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(iCurrentModel->isDebugDataVisible()^scene::EDS_HALF_TRANSPARENCY));
        //  break;
        //case 460: // View -> Debug Information
        //  if (iCurrentModel)
        //    iCurrentModel->setDebugDataVisible((scene::E_DEBUG_SCENE_TYPE)(iCurrentModel->isDebugDataVisible()^scene::EDS_BBOX_BUFFERS));
        //  break;
        //case 499: // View -> Debug Information
        //  if (iCurrentModel)
        //    iCurrentModel->setDebugDataVisible(scene::EDS_FULL);
        //  break;
        case 500: // Help->About
          //showAboutText();
          break;
        //case 610: // View -> Material -> Solid
        //  if (iCurrentModel)
        //    iCurrentModel->setMaterialType(video::EMT_SOLID);
        //  break;
        //case 620: // View -> Material -> Transparent
        //  if (iCurrentModel)
        //    iCurrentModel->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
        //  break;
        //case 630: // View -> Material -> Reflection
        //  if (iCurrentModel)
        //    iCurrentModel->setMaterialType(video::EMT_SPHERE_MAP);
        //  break;

        case 1000:
          SetActiveCamera(iWorldCameraSceneNode);
          break;
        case 1100:
          SetActiveCamera(iWorldCameraSceneNode);
          break;

        }
        break;
      }

    case gui::EGET_FILE_SELECTED:
      {
        // load the model file, selected in the file open dialog
        gui::IGUIFileOpenDialog* dialog =
          (gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;
        //loadModel(core::stringc(dialog->getFilename()).c_str());
      }
      break;

    case gui::EGET_SCROLL_BAR_CHANGED:

      // control skin transparency
      if (id == 104)
      {
        s32 pos = ((gui::IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
        for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
        {
          video::SColor col = env->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
          col.setAlpha(pos);
          env->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
        }
      }
      break;

    case gui::EGET_COMBO_BOX_CHANGED:

      // control anti-aliasing/filtering
      if (id == 108)
      {
        s32 pos = ((gui::IGUIComboBox*)event.GUIEvent.Caller)->getSelected();
        switch (pos)
        {
        case 0:
          if (iCurrentModel)
          {
            iCurrentModel->setMaterialFlag(video::EMF_BILINEAR_FILTER, false);
            iCurrentModel->setMaterialFlag(video::EMF_TRILINEAR_FILTER, false);
            iCurrentModel->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, false);
          }
          break;
        case 1:
          if (iCurrentModel)
          {
            iCurrentModel->setMaterialFlag(video::EMF_BILINEAR_FILTER, true);
            iCurrentModel->setMaterialFlag(video::EMF_TRILINEAR_FILTER, false);
          }
          break;
        case 2:
          if (iCurrentModel)
          {
            iCurrentModel->setMaterialFlag(video::EMF_BILINEAR_FILTER, false);
            iCurrentModel->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
          }
          break;
        case 3:
          if (iCurrentModel)
          {
            iCurrentModel->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
          }
          break;
        case 4:
          if (iCurrentModel)
          {
            iCurrentModel->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, false);
          }
          break;
        }
      }
      break;

    case gui::EGET_BUTTON_CLICKED:

      switch(id)
      {
      case 1101:
        {
          // set scale
          gui::IGUIElement* root = env->getRootGUIElement();
          core::vector3df scale;
          core::stringc s;

          s = root->getElementFromId(901, true)->getText();
          scale.X = (f32)atof(s.c_str());
          s = root->getElementFromId(902, true)->getText();
          scale.Y = (f32)atof(s.c_str());
          s = root->getElementFromId(903, true)->getText();
          scale.Z = (f32)atof(s.c_str());

          if (iCurrentModel)
            iCurrentModel->setScale(scale);
        }
        break;
      case 1102:
        env->addFileOpenDialog(L"Please select a model file to open");
        break;
      case 1103:
        //showAboutText();
        break;
      case 1104:
        //createToolBox();
        break;
      case 1105:
        env->addFileOpenDialog(L"Please select your game archive/directory");
        break;
      }

      break;
    }
  }

  return false;
}

//------------------------------------------------------------------------------
//! SetVisible
//! 
void CWorldEditorGUI::SetActive(bool pActive)
{
  if (pActive)
  {
    Activate();
  }
  else
  {
    Deactivate();
  }
}

//------------------------------------------------------------------------------
//! setActiveCamera
//! toggles between various cameras
void CWorldEditorGUI::SetActiveCamera(scene::ICameraSceneNode* newActive)
{
  if (iIrrSceneManager == NULL)
    return;

  iOldCameraSceneNode = iIrrSceneManager->getActiveCamera();

  if (newActive != NULL)
    newActive->setInputReceiverEnabled(true);

  iIrrSceneManager->setActiveCamera(newActive);
}
