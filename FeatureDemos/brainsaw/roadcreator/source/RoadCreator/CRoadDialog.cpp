  #include <CRoadDialog.h>
  #include <INotification.h>

using namespace irr;

/**
 * The constructor
 * @param pDevice the Irrlicht device
 */
CRoadDialog::CRoadDialog(IrrlichtDevice *pDevice)
                         : CDialogWindow(pDevice->getGUIEnvironment(),
                                         core::position2di(100,100),
                                         core::dimension2du(220,315),true,L"") {
  m_pDevice=pDevice;

  gui::IGUIEnvironment *pGuienv=m_pDevice->getGUIEnvironment();

  m_pList=pGuienv->addListBox (core::rect<s32>( 10, 30,210,250),m_pWindow,-1,true);
  m_pEdName=pGuienv->addEditBox(L"",core::rect<s32>(10,255,210,275),true,m_pWindow);
  m_pOk    =pGuienv->addButton(core::rect<s32>( 10,280,105,300),m_pWindow,-1,L"OK");
  m_pCancel=pGuienv->addButton(core::rect<s32>(110,280,210,300),m_pWindow,-1,L"Cancel");
  
  setVisible(false);
}

/**
 * Show the dialog
 * @param bLoad make it a "Load" or "Save as" dialog
 * @param sHeadline headline of the dialog
 */
void CRoadDialog::showDialog(bool bLoad, const core::stringc sHeadline) {
  m_pWindow->setText(core::stringw(sHeadline).c_str());
  m_bLoad=bLoad;
  m_pEdName->setEnabled(!m_bLoad);
  m_pEdName->setText(L"");
  m_pOk->setEnabled(false);
  setVisible(true);
}

/**
 * Callback method for the Irrlicht event receiver
 * @param event the event to handle
 */
bool CRoadDialog::OnEvent(const SEvent &event) {
  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      //"OK" button clicked
      if (event.GUIEvent.Caller==m_pOk) {
        SEvent theEvent;
        theEvent.EventType=EET_USER_EVENT;
        theEvent.UserEvent.UserData1=m_bLoad?eFileDialogLoad:eFileDialogSave;
        m_pDevice->postEventFromUser(theEvent);
        m_pWindow->setVisible(false);
      }
      
      //"Cancel" button clicked
      if (event.GUIEvent.Caller==m_pCancel) {
        m_sSelectedFile="";
        m_pWindow->setVisible(false);
      }
    }
    
    if (event.GUIEvent.EventType==gui::EGET_LISTBOX_CHANGED) {
      //a new file was selected
      if (event.GUIEvent.Caller==m_pList && m_pList->getSelected()!=-1) {
        m_sSelectedFile=m_pList->getListItem(m_pList->getSelected());
        m_pEdName->setText(m_pList->getListItem(m_pList->getSelected()));
        m_pOk->setEnabled(true);
      }
    }
    
    if (event.GUIEvent.EventType==gui::EGET_EDITBOX_CHANGED) {
      //the name of the file changed (mainly for "SaveAs")
      if (event.GUIEvent.Caller==m_pEdName) {
        m_pOk->setEnabled(core::stringw(m_pEdName->getText())!=L"");
        m_sSelectedFile=m_pEdName->getText();
      }
    }
  }
  return false;
}

/**
 * Set the folder to search for road files
 * @param sFolder the new folder
 */
void CRoadDialog::setFolder(const core::stringc &sFolder) {
  io::IFileSystem *fs=m_pDevice->getFileSystem();

  core::stringc sOld=fs->getWorkingDirectory();
  fs->changeWorkingDirectoryTo(sFolder);
  io::IFileList *pList=fs->createFileList();
  
  //clear and refill the filelist
  m_pList->clear();
  for (u32 i=0; i<pList->getFileCount(); i++) {
    if (!pList->isDirectory(i)) {
      m_pList->addItem(core::stringw(pList->getFileName(i).c_str()).c_str());
    }
  }
  fs->changeWorkingDirectoryTo(sOld);
}
