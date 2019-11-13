  #include <CSettings.h>

/**
 * The constructor
 * @param scene to be loaded and displayed in the background. Can be "NULL" if no scene should be shown
 * @param the XML file to read and store the settings
 * @param title of the window
 * @param background color of the window
 */
CSettings::CSettings(c8 *sScene, c8 *sSettingsFile, wchar_t *sTitle, SColor cBackground) {
  //Create a software device
  m_pDevice=createDevice(EDT_SOFTWARE, dimension2d<u32>(230,200), 16, false, false, false, 0);
  m_pDevice->setWindowCaption(sTitle);

  strcpy(m_sSettingsFile,sSettingsFile);

  //read the settings from the settings file
  IXMLReaderUTF8 *pXml=m_pDevice->getFileSystem()->createXMLReaderUTF8(m_sSettingsFile);

  m_iResolution=0;
  m_iDriver    =EDT_NULL;
  m_bFullscreen=true;
  m_bShadows   =false;

  while (pXml->read()) {
    //This is special: the video drivers to choose from need to be in the XML file
    if (!strcmp(pXml->getNodeName(),"videodriver")) {
      _DRV *drv=new _DRV();
      wcscpy(drv->sName,stringw(pXml->getAttributeValue("name")).c_str());
      drv->iDriver=(E_DRIVER_TYPE)atoi(pXml->getAttributeValue("id"  ));
      m_aDrvs.push_back(drv);
    }

    if (!strcmp(pXml->getNodeName(),"resolution")) m_iResolution=atoi(pXml->getAttributeValue("value"));
    if (!strcmp(pXml->getNodeName(),"fullscreen")) m_bFullscreen=atoi(pXml->getAttributeValue("value"));
    if (!strcmp(pXml->getNodeName(),"shadows"   )) m_bShadows   =atoi(pXml->getAttributeValue("value"));
    if (!strcmp(pXml->getNodeName(),"driver"    )) m_iDriver    =(E_DRIVER_TYPE)atoi(pXml->getAttributeValue("value"));
  }

  pXml->drop();

  //init the Irrlicht classes
  m_pGuiEnv=m_pDevice->getGUIEnvironment();
  m_pSceneManager=m_pDevice->getSceneManager();
  m_pDriver=m_pDevice->getVideoDriver();
  m_cBackground=cBackground;

  m_bSettingsChanged=false;
  m_iClose=0;

  //add the static textfields
  m_pGuiEnv->addStaticText(sTitle,rect<s32>(5,5,125,18),true,true,0,-1,true);
  m_pGuiEnv->addStaticText(L"graphics driver",rect<s32>(5, 80,75, 92),false,true,0,-1,true);
  m_pGuiEnv->addStaticText(L"resolution"     ,rect<s32>(5,105,75,117),false,true,0,-1,true);
  m_pGuiEnv->addStaticText(L"fullscreen"     ,rect<s32>(5,130,75,142),false,true,0,-1,true);
  m_pGuiEnv->addStaticText(L"shadows"        ,rect<s32>(5,155,75,167),false,true,0,-1,true);

  //add the "driver" and "resolution" comboboxes
  m_pDrivers   =m_pGuiEnv->addComboBox(rect<s32>(80, 79,180, 93),0,3);
  m_pResolution=m_pGuiEnv->addComboBox(rect<s32>(80,104,180,118),0,4);

  //add the "fullscreen" and "shadows" checkboxes
  m_pFullscreen=m_pGuiEnv->addCheckBox(false,rect<s32>(80,128,94,142),0,5);
  m_pShadows   =m_pGuiEnv->addCheckBox(false,rect<s32>(80,153,94,167),0,6);

  //add the "OK" and "Cancel" buttons
  m_pOk    =m_pGuiEnv->addButton(rect<s32>( 80,180,120,195),0,1,L"Start");
  m_pCancel=m_pGuiEnv->addButton(rect<s32>(125,180,175,195),0,2,L"Close");

  //If no driver information was found in the settings file we initialize the driver list with all available drivers
  if (m_aDrvs.size()==0) {
    _DRV *drv;
    drv=new _DRV(); drv->iDriver=EDT_SOFTWARE     ; wcscpy(drv->sName,L"Software"     ); m_aDrvs.push_back(drv);
    drv=new _DRV(); drv->iDriver=EDT_BURNINGSVIDEO; wcscpy(drv->sName,L"BurningsVideo"); m_aDrvs.push_back(drv);
//    drv=new _DRV(); drv->iDriver=EDT_DIRECT3D8    ; wcscpy(drv->sName,L"Direct3D8"    ); m_aDrvs.push_back(drv);
    drv=new _DRV(); drv->iDriver=EDT_DIRECT3D9    ; wcscpy(drv->sName,L"Direct3D9"    ); m_aDrvs.push_back(drv);
    drv=new _DRV(); drv->iDriver=EDT_OPENGL       ; wcscpy(drv->sName,L"OpenGL"       ); m_aDrvs.push_back(drv);
  }

  //If driver information were found we just add the drivers from the XML file
  for (u32 i=0; i<m_aDrvs.size(); i++) m_pDrivers->addItem(m_aDrvs[i]->sName);

  //we add all video modes with a bit depth of at least 16 to the "resolution" combobox
  m_pVModes=m_pDevice->getVideoModeList();
  for (s32 i=0; i<m_pVModes->getVideoModeCount(); i++)
    if (m_pVModes->getVideoModeDepth(i)>=16) {
      dimension2du res=m_pVModes->getVideoModeResolution(i);
      _VRES *vRes=new _VRES();
      vRes->iWidth =res.Width ;
      vRes->iHeight=res.Height;
      vRes->iBpp   =m_pVModes->getVideoModeDepth(i);
      m_aVModes.push_back(vRes);
      printf("--> %i\n",m_aVModes.size());
    }

  //now we fill the "resolution" combobox with the drivers we added to the array
  for (u32 i=0; i<m_aVModes.size(); i++) {
    wchar_t s[0xFF];
    _VRES *res=m_aVModes[i];
    swprintf(s,0xFE,L"%ix%i %i bpp",res->iWidth,res->iHeight,res->iBpp);
    m_pResolution->addItem(s);
  }

  //we load the scene if a scene is wanted
  if (sScene) {
    m_pSceneManager->loadScene(sScene);
    m_pSceneManager->addCameraSceneNode(NULL,vector3df(0.0f,0.0f,-20.0f),vector3df(0.0f,0.0f,0.0f));
  }

  //Now add an event receiver
  m_pDevice->setEventReceiver(this);

  //we init the GUI items with the values read from the settings XML file
  for (u32 i=0; i<m_aDrvs.size(); i++) if (m_aDrvs[i]->iDriver==m_iDriver) m_pDrivers->setSelected(i);

  m_pResolution->setSelected(m_iResolution);

  m_pFullscreen->setChecked(m_bFullscreen);
  m_pShadows   ->setChecked(m_bShadows   );

  m_pDevice->getCursorControl()->setVisible(true);
}

/**
 * The destructor saves the settings file if the settings were changed
 */
CSettings::~CSettings() {
  //Settings changed and "OK" button clicked?
  if (m_bSettingsChanged && m_iClose==1) {
    wchar_t res[0xFF],drv[0xFF];

    //Write settings to the settings file
    IXMLWriter *pXml=m_pDevice->getFileSystem()->createXMLWriter(m_sSettingsFile);

    swprintf(res,0xFE,L"%i",m_iResolution);
    swprintf(drv,0xFE,L"%i",m_iDriver);

    array<stringw> aResN; aResN.push_back(stringw(L"value"));
    array<stringw> aResV; aResV.push_back(stringw(res));
    array<stringw> aDrvN; aDrvN.push_back(stringw(L"value"));
    array<stringw> aDrvV; aDrvV.push_back(stringw(drv));
    array<stringw> aFlsN; aFlsN.push_back(stringw(L"value"));
    array<stringw> aFlsV; aFlsV.push_back(stringw(m_bFullscreen?L"1":L"0"));
    array<stringw> aShdN; aShdN.push_back(stringw(L"value"));
    array<stringw> aShdV; aShdV.push_back(stringw(m_bShadows?L"1":L"0"));

    pXml->writeXMLHeader();
    pXml->writeElement(L"settings"); pXml->writeLineBreak();
    for (u32 i=0; i<m_aDrvs.size(); i++) {
      wchar_t s[0xFF];
      swprintf(s,0xFE,L"%i",m_aDrvs[i]->iDriver);
      pXml->writeElement(L"videodriver",true,L"name",m_aDrvs[i]->sName,L"id",s);
      pXml->writeLineBreak();
    }
    pXml->writeElement(L"resolution",true,aResN,aResV); pXml->writeLineBreak();
    pXml->writeElement(L"fullscreen",true,aFlsN,aFlsV); pXml->writeLineBreak();
    pXml->writeElement(L"shadows"   ,true,aShdN,aShdV); pXml->writeLineBreak();
    pXml->writeElement(L"driver"    ,true,aDrvN,aDrvV); pXml->writeLineBreak();

    pXml->writeClosingTag(L"settings");
    pXml->drop();
  }

  for (u32 i=0; i<m_aVModes.size(); i++) delete m_aVModes[i];
}

/**
 * Run the dialog
 */
u32 CSettings::run() {
  while (m_pDevice->run() && !m_iClose) {
    m_pDriver->beginScene(true,true,m_cBackground);

    m_pSceneManager->drawAll();
    m_pGuiEnv->drawAll();

    m_pDriver->endScene();
  }

  return m_iClose?m_iClose:2;
}

/**
 * Event receiver for the Irrlicht event pipeline
 */
bool CSettings::OnEvent(const SEvent &event) {
  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==EGET_BUTTON_CLICKED) {
      m_iClose=event.GUIEvent.Caller->getID();
    }

    if (event.GUIEvent.EventType==EGET_COMBO_BOX_CHANGED) {
      m_bSettingsChanged=true;
      switch (event.GUIEvent.Caller->getID()) {
        case 3: m_iDriver=m_aDrvs[m_pDrivers->getSelected()]->iDriver; break;
        case 4: m_iResolution=m_pResolution->getSelected(); break;
      }
    }

    if (event.GUIEvent.EventType==EGET_CHECKBOX_CHANGED) {
      m_bSettingsChanged=true;
      switch (event.GUIEvent.Caller->getID()) {
        case 5: m_bFullscreen=m_pFullscreen->isChecked(); break;
        case 6: m_bShadows   =m_pShadows   ->isChecked(); break;
      }
    }
  }

  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (!event.KeyInput.PressedDown) {
      switch (event.KeyInput.Key) {
        case KEY_RETURN:
          m_iClose=1;
          break;

        case KEY_ESCAPE:
          m_iClose=2;
          break;

        default:
          break;
      }
    }
  }

  return false;
}

/**
 * This function drops the software device created to display the window and creates a new device
 * with the settings of the user
 */
IrrlichtDevice *CSettings::createDeviceFromSettings() {
  m_pDevice->closeDevice();
  m_pDevice->run();
  m_pDevice->drop();
  _VRES *res=m_aVModes[m_iResolution];
  printf("==> %i\n",m_iResolution);
  m_pDevice=createDevice(m_iDriver,dimension2du(res->iWidth,res->iHeight),res->iBpp,m_bFullscreen,m_bShadows,false,0);
  return m_pDevice;
}
