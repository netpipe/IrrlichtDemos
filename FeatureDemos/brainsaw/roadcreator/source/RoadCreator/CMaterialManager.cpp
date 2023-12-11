  #include <CMaterialManager.h>
  #include <INotification.h>

using namespace irr;

CMaterialManager::CMaterialManager(IrrlichtDevice *pDevice) : CDialogWindow(pDevice->getGUIEnvironment(),core::position2di(100,100),core::dimension2du(500,320),false,L"Materials") {
  m_pDevice=pDevice;
  m_pDrv=pDevice->getVideoDriver();

  m_pGuienv=m_pDevice->getGUIEnvironment();
  m_pTextures   =m_pGuienv->addListBox(core::rect<s32>( 10, 30,210,286),m_pWindow,-1,true);
  m_pTexture    =m_pGuienv->addImage  (core::rect<s32>(220, 30,476,286),m_pWindow,-1,L"Texture");
  m_pSelect     =m_pGuienv->addButton (core::rect<s32>( 10,290,210,310),m_pWindow,-1,L"Select");

  m_sSelectedFile="";
  m_sTextureFolder="";
}

void CMaterialManager::updateList() {
  //clear the list of textures and ...
  m_pTextures->clear();
  
  //... refill it again
  for (u32 i=0; i<m_aMaterials.size(); i++) {
    m_pTextures->addItem(core::stringw(m_aMaterials[i]).c_str());
  }
}

u32 CMaterialManager::getMaterialCount() {
  return m_aMaterials.size();
}

const core::stringc &CMaterialManager::getMaterial(u32 iIdx) {
  static core::stringc s_sSelected=iIdx<m_aMaterials.size()?m_sTextureFolder+"/"+m_aMaterials[iIdx]:m_sTextureFolder+"/"+m_aMaterials[0];
  return s_sSelected;
}

void CMaterialManager::setVisible(bool b) {
  if (b) {
    //if the window is shown we need to select no texture at all or ...
    m_pTextures->setSelected(-1);
    for (u32 i=0; i<m_aMaterials.size(); i++) {
      //... the appropriate one if a selection is made.
      if (m_sSelectedFile.find(m_aMaterials[i].c_str())!=-1) {
        m_pTextures->setSelected(i);
      }
    }
  }

  m_pWindow->setVisible(b);
}

bool CMaterialManager::OnEvent(const SEvent &event) {
  bool bRet=false;

  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      //"Use texture" button clicked: post a user event so that the main routine knows what happened
      if (event.GUIEvent.Caller==m_pSelect) {
        SEvent theEvent;
        theEvent.EventType=EET_USER_EVENT;
        theEvent.UserEvent.UserData1=eTextureSelected;
        m_pDevice->postEventFromUser(theEvent);
        m_pWindow->setVisible(false);
      }
    }
    
    //If the selected item of the listbox has changed we update the "selected" string
    if (event.GUIEvent.EventType==gui::EGET_LISTBOX_CHANGED) {
      m_sSelectedFile=m_sTextureFolder+"/"+m_pTextures->getListItem(m_pTextures->getSelected());
      updateTexture();
    }
  }

  return bRet;
}

const core::stringc &CMaterialManager::getSelectedTexture() {
  return m_sSelectedFile;
}

void CMaterialManager::setTextureFolder(const core::stringc sFolder) {
  m_sTextureFolder=sFolder;
  io::IFileSystem *fs=m_pDevice->getFileSystem();

  core::stringc sOld=fs->getWorkingDirectory();
  fs->changeWorkingDirectoryTo(sFolder);
  io::IFileList *pList=fs->createFileList();
  
  //Search for all files in the given folder and add them to the list
  m_aMaterials.clear();
  for (u32 i=0; i<pList->getFileCount(); i++) {
    if (!pList->isDirectory(i)) {
      m_aMaterials.push_back(pList->getFileName(i).c_str());
    }
  }
  fs->changeWorkingDirectoryTo(sOld);
  updateList();
}

void CMaterialManager::setSelectedTexture(const core::stringc sTexture) {
  m_sSelectedFile=sTexture;
  updateTexture();
}

void CMaterialManager::updateTexture() {
  m_pTexture->setImage(m_pDevice->getVideoDriver()->getTexture(m_sSelectedFile));
  m_pTexture->setScaleImage(true);
}

const core::stringc &CMaterialManager::getSaveName() {
  static core::stringc s="MaterialManager";
  return s;
}
