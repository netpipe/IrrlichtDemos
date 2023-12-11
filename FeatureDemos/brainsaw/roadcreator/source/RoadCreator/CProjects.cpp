  #include <CProjects.h>
  #include <INotification.h>
  #include <irrlicht.h>

using namespace irr;

/**
 * The constructor
 * @param pDevice the Irrlicht device
 */
CProjects::CProjects(IrrlichtDevice *pDevice) : CDialogWindow(pDevice->getGUIEnvironment(),core::position2di(100,100),core::dimension2du(410,250),true,L"Select Project") {
  m_pDevice=pDevice;

  core::position2di edtPos=core::position2di(200, 55),
                    lblPos=core::position2di(200, 40),
                    offset=core::position2di(  0, 40),
                    btnpos=core::position2di(200,210),
                    btnoff=core::position2di( 70,  0);

  core::dimension2di edtDim=core::dimension2di(200,20),
                     btnDim=core::dimension2di( 60,20);

  m_pProjectList=m_pGuiEnv->addListBox(core::rect<s32>(10,30,180,240),m_pWindow,-1,true);

  m_pProjectName=m_pGuiEnv->addEditBox(L"",core::rect<s32>(edtPos,edtDim),true,m_pWindow); edtPos+=offset;
  m_pProjectRoot=m_pGuiEnv->addEditBox(L"",core::rect<s32>(edtPos,edtDim),true,m_pWindow); edtPos+=offset;
  m_pTexturePath=m_pGuiEnv->addEditBox(L"",core::rect<s32>(edtPos,edtDim),true,m_pWindow); edtPos+=offset;
  m_pSavePath   =m_pGuiEnv->addEditBox(L"",core::rect<s32>(edtPos,edtDim),true,m_pWindow); edtPos+=offset;

  m_pGuiEnv->addStaticText(L"Project Name"             ,core::rect<s32>(lblPos,edtDim),false,false,m_pWindow); lblPos+=offset;
  m_pGuiEnv->addStaticText(L"Project Root Folder"      ,core::rect<s32>(lblPos,edtDim),false,false,m_pWindow); lblPos+=offset;
  m_pGuiEnv->addStaticText(L"Texture Folder (Relative)",core::rect<s32>(lblPos,edtDim),false,false,m_pWindow); lblPos+=offset;
  m_pGuiEnv->addStaticText(L"Save Folder (Relative)"   ,core::rect<s32>(lblPos,edtDim),false,false,m_pWindow); lblPos+=offset;

  m_pSelect=m_pGuiEnv->addButton(core::rect<s32>(btnpos,btnDim),m_pWindow,-1,L"Select"); btnpos+=btnoff;
  m_pNew   =m_pGuiEnv->addButton(core::rect<s32>(btnpos,btnDim),m_pWindow,-1,L"New"   ); btnpos+=btnoff;
  m_pDel   =m_pGuiEnv->addButton(core::rect<s32>(btnpos,btnDim),m_pWindow,-1,L"Delete"); btnpos+=btnoff;
  
  updateElements();
}

/**
 * update enabled flag of GUI elements depending on the current state 
 */
void CProjects::updateElements() {
  bool b=m_pProjectList->getSelected()!=-1;
  m_pProjectName->setEnabled(b);
  m_pProjectRoot->setEnabled(b);
  m_pTexturePath->setEnabled(b);
  m_pSavePath   ->setEnabled(b);
  m_pDel        ->setEnabled(b);
  m_pSelect     ->setEnabled(b);
}


/**
 * Update the list of projects list
 */
void CProjects::updateProjectList() {
  m_pProjectList->clear();
  for (u32 i=0; i<m_aProjects.size(); i++) {
    u32 iIdx=m_pProjectList->addItem(core::stringw(m_aProjects[i]->m_sName).c_str());
    if (m_aProjects[i]==m_pSelected) {
      m_pProjectList->setSelected(iIdx);
      updateUI();
      updateElements();
    }
  }
}

/**
 * Update the complete UI
 */
void CProjects::updateUI() {
  if (m_pSelected==NULL) return;

  m_pProjectName->setText(core::stringw(m_pSelected->m_sName       ).c_str());
  m_pProjectRoot->setText(core::stringw(m_pSelected->m_sProjectRoot).c_str());
  m_pTexturePath->setText(core::stringw(m_pSelected->m_sTexturePath).c_str());
  m_pSavePath   ->setText(core::stringw(m_pSelected->m_sSavePath   ).c_str());
}

/**
 * Irrlicht event receiver callback
 * @param event the event to handle
 */
bool CProjects::OnEvent(const SEvent &event) {
  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==gui::EGET_LISTBOX_CHANGED) {
      //Selected project changed
      if (event.GUIEvent.Caller==m_pProjectList && m_pProjectList->getSelected()!=-1) {
        for (u32 i=0; i<m_aProjects.size(); i++) {
          if (m_aProjects[i]->m_sName==core::stringc(m_pProjectList->getListItem(m_pProjectList->getSelected()))) {
            m_pSelected=m_aProjects[i];
            m_sSelected=m_pSelected->m_sName;
          }
        }
        updateElements();
        updateUI();
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      //Button "new project" clicked
      if (event.GUIEvent.Caller==m_pNew) {
        CProject *p=new CProject();
        p->m_sName="New Project";
        m_aProjects.push_back(p);
        m_pSelected=p;
        updateProjectList();
      }
      
      //Button "select project" clicked
      if (event.GUIEvent.Caller==m_pSelect && m_pSelected!=NULL) {
        SEvent theEvent;
        theEvent.EventType=EET_USER_EVENT;
        theEvent.UserEvent.UserData1=eProjectSelected;
        m_pDevice->postEventFromUser(theEvent);
        m_pWindow->setVisible(false);
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST) {
      //We update the data of the selected project if one of the editing elements lost the focus
      if (event.GUIEvent.Caller==m_pProjectName || event.GUIEvent.Caller==m_pProjectRoot ||
          event.GUIEvent.Caller==m_pTexturePath || event.GUIEvent.Caller==m_pSavePath) {

        if (m_pSelected) {
          m_pSelected->m_sName       =core::stringc(m_pProjectName->getText());
          m_pSelected->m_sProjectRoot=core::stringc(m_pProjectRoot->getText());
          m_pSelected->m_sTexturePath=core::stringc(m_pTexturePath->getText());
          m_pSelected->m_sSavePath   =core::stringc(m_pSavePath   ->getText());

          updateProjectList();
        }
      }
    }
  }
  return false;
}

/**
 * Save a project to an attributes object
 * @param pOut the attributes object
 * @param iProject index of the project to save
 */
void CProjects::saveProject(io::IAttributes *pOut, irr::u32 iProject) {
  if (iProject>=m_aProjects.size()) return;
  m_aProjects[iProject]->save(pOut);
}

/**
 * Load a project from an attributes object
 * @param pIn the attributes object
 */
void CProjects::loadProject(io::IAttributes *pIn) {
  //Create a new project, ...
  CProject *p=new CProject();
  //... add it to the list and ...
  m_aProjects.push_back(p);
  //... load the data
  p->load(pIn);
  if (m_sSelected==p->m_sName) m_pSelected=p;
}
