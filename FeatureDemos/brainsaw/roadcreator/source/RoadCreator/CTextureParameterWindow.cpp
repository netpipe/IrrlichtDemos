  #include <CTextureParameterWindow.h>
  #include <CTextureParameters.h>
  #include <CMaterialManager.h>
  #include <INotification.h>
  #include <irrlicht.h>

/**
 * The constructor
 * @param pGuienv the Irrlicht GUI environment
 * @param pMaterials the instance of the "select texture" dialog
 * @see CMaterialManager
 */
CTextureParameterWindow::CTextureParameterWindow(gui::IGUIEnvironment *pGuienv, CMaterialManager *pMaterials)
                                                : CDialogWindow(pGuienv,
                                                                core::position2di(100,100),
                                                                core::dimension2du(310,200),false,
                                                                L"Texture Parameters") {
  m_iTextureSelectState=0;

  //dimensions of the elements
  core::dimension2di cEdDim=core::dimension2di(200,20),
                     cTxDim=core::dimension2di( 80,20),
                     cDim_2=core::dimension2di( 80,20);

  //positions of the elements
  core::position2di cEdPos=core::position2di(100,30),
                    cTxPos=core::position2di( 10,30),
                    cEdOff=core::position2di(  0,25),
                    cYOff =core::position2di(cDim_2.Width,0);

  //Add the GUI elements
  m_pTextureName   =pGuienv->addStaticText(L"Select Texture",core::rect<s32>(cEdPos,core::dimension2di(120,20)),false,true,m_pWindow); m_pTextureName->setTextAlignment(gui::EGUIA_UPPERLEFT,gui::EGUIA_CENTER);
  m_pTextureSelect =pGuienv->addButton(core::rect<s32>(cEdPos+core::position2di(125,0),core::dimension2di(80,20)),m_pWindow,-1,L"Select"); cEdPos+=cEdOff;
  m_pTextureRotate =pGuienv->addComboBox(      core::rect<s32>(cEdPos      ,cDim_2),     m_pWindow); cEdPos+=cEdOff;
  m_pTextureStretch=pGuienv->addCheckBox(false,core::rect<s32>(cEdPos      ,cDim_2),     m_pWindow); cEdPos+=cEdOff;
  m_pTextureOffsetX=pGuienv->addSpinBox (L""  ,core::rect<s32>(cEdPos      ,cDim_2),true,m_pWindow); 
  m_pTextureOffsetY=pGuienv->addSpinBox (L""  ,core::rect<s32>(cEdPos+cYOff,cDim_2),true,m_pWindow); cEdPos+=cEdOff;
  m_pTextureScaleX =pGuienv->addSpinBox (L""  ,core::rect<s32>(cEdPos      ,cDim_2),true,m_pWindow); 
  m_pTextureScaleY =pGuienv->addSpinBox (L""  ,core::rect<s32>(cEdPos+cYOff,cDim_2),true,m_pWindow); cEdPos+=cEdOff;

  //Initialize the spinboxes
  m_pTextureOffsetX->setRange(-1.0f,1.0f); m_pTextureOffsetX->setStepSize(0.01f); m_pTextureOffsetX->setDecimalPlaces(2);
  m_pTextureOffsetY->setRange(-1.0f,1.0f); m_pTextureOffsetY->setStepSize(0.01f); m_pTextureOffsetY->setDecimalPlaces(2);
  m_pTextureScaleX ->setRange( 0.1f,5.0f); m_pTextureScaleX ->setStepSize(0.01f); m_pTextureScaleX ->setDecimalPlaces(2);
  m_pTextureScaleY ->setRange( 0.1f,5.0f); m_pTextureScaleY ->setStepSize(0.01f); m_pTextureScaleY ->setDecimalPlaces(2);

  //Add the labels
  pGuienv->addStaticText(L"Texture:" ,core::rect<s32>(cTxPos,cTxDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cTxPos+=cEdOff;
  pGuienv->addStaticText(L"Rotation:",core::rect<s32>(cTxPos,cTxDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cTxPos+=cEdOff;
  pGuienv->addStaticText(L"Stretch:" ,core::rect<s32>(cTxPos,cTxDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cTxPos+=cEdOff;
  pGuienv->addStaticText(L"Offset:"  ,core::rect<s32>(cTxPos,cTxDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cTxPos+=cEdOff;
  pGuienv->addStaticText(L"Scale:"   ,core::rect<s32>(cTxPos,cTxDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cTxPos+=cEdOff;

  //Fill the texture rotate combobox
  m_pTextureRotate->addItem(  L"0");
  m_pTextureRotate->addItem( L"90");
  m_pTextureRotate->addItem(L"180");
  m_pTextureRotate->addItem(L"270");

  m_pMaterials=pMaterials;
  m_pParameters=NULL;
}

/** 
 * update the UI 
 */
void CTextureParameterWindow::updateUI() {
  if (m_pParameters==NULL) return;

  m_pTextureRotate ->setSelected(m_pParameters->getRotate());
  m_pTextureOffsetX->setValue   (m_pParameters->getOffsetX());
  m_pTextureOffsetY->setValue   (m_pParameters->getOffsetY());
  m_pTextureScaleX ->setValue   (m_pParameters->getScaleX());
  m_pTextureScaleY ->setValue   (m_pParameters->getScaleY());
  m_pTextureStretch->setChecked (m_pParameters->getStretch());
  m_pTextureName   ->setText    (core::stringw(m_pParameters->getTexture()).c_str());
}

/**
 * Add a notification listener
 * @param p notification listener to add
 */
void CTextureParameterWindow::addNotify(ITextureModified *p) {
  core::list<ITextureModified *>::Iterator it;

  //Make sure the listener is not yet in the list
  for (it=m_lNotify.begin(); it!=m_lNotify.end(); it++) {
    if (*it==p) return;
  }
  
  //add the listener
  m_lNotify.push_back(p);
}

/**
 * Remove a notification listener
 * @param p notification listener to remove
 */
void CTextureParameterWindow::delNotify(ITextureModified *p) {
  core::list<ITextureModified *>::Iterator it;
  
  //Search for a listener and remove it
  for (it=m_lNotify.begin(); it!=m_lNotify.end(); it++) {
    if (*it==p) {
      m_lNotify.erase(it);
      return;
    }
  }
}

/**
 * notify all listeners of a change 
 */
void CTextureParameterWindow::notify() {
  core::list<ITextureModified *>::Iterator it;
  
  //Call the callback method of all listeners
  for (it=m_lNotify.begin(); it!=m_lNotify.end(); it++) {
    (*it)->textureModified();
  }
}

/**
 * Irrlicht event receiver callback
 * @param event the event to handle
 */
bool CTextureParameterWindow::OnEvent(const SEvent &event) {
  bool bRet=false;

  if (m_pParameters==NULL) return false;

  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      if (event.GUIEvent.Caller==m_pTextureSelect) {
        m_iTextureSelectState=1;
        m_pMaterials->setSelectedTexture(m_pParameters->getTexture());
        m_pMaterials->setVisible(true);
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_SPINBOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pTextureOffsetX) {
        m_pParameters->setOffsetX(m_pTextureOffsetX->getValue());
        notify();
      }
      
      if (event.GUIEvent.Caller==m_pTextureOffsetY) {
        m_pParameters->setOffsetY(m_pTextureOffsetY->getValue());
        notify();
      }

      if (event.GUIEvent.Caller==m_pTextureScaleX) {
        m_pParameters->setScaleX(m_pTextureScaleX->getValue());
        printf("scale: %.2f\n",m_pTextureScaleX->getValue());
        notify();
      }

      if (event.GUIEvent.Caller==m_pTextureScaleY) {
        m_pParameters->setScaleY(m_pTextureScaleY->getValue());
        printf("scale: %.2f\n",m_pTextureScaleY->getValue());
        notify();
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_COMBO_BOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pTextureRotate) {
        m_pParameters->setRotate(m_pTextureRotate->getSelected());
        notify();
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_CHECKBOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pTextureStretch) {
        m_pParameters->setStretch(m_pTextureStretch->isChecked());
        notify();
      }
    }
  }

  if (event.EventType==EET_USER_EVENT) {
    if (event.UserEvent.UserData1==eTextureSelected && m_iTextureSelectState==1) {
      m_iTextureSelectState=0;
      m_pParameters->setTexture(m_pMaterials->getSelectedTexture());
      updateUI();
      notify();
    }
  }

  return bRet;
}
