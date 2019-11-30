  #include <CSegment.h>
  #include <CConnection.h>
  #include <CMaterialManager.h>
  #include <CTextureParameterWindow.h>
  #include <CConnectionParameterWindow.h>

using namespace irr;

CConnectionParameterWindow::CConnectionParameterWindow(gui::IGUIEnvironment *pGuienv, CTextureParameterWindow *pTex) :
                            CDialogWindow(pGuienv,core::position2di(100,100),core::dimension2du(400,445),false,L"Edit Connection Parameters") {

  //Create some position and dimension variables to easily create the GUI
  core::position2di edtPos=core::position2di(100,30),   //Position of the edit fields
                    lblPos=core::position2di( 10,30),   //Position of the labels
                    offset=core::position2di(  0,25);   //Offset between two lines

  core::dimension2di edtDim=core::dimension2di(135,20), //Dimension of the edit fields
                     spnDim=core::dimension2di( 60,20), //Dimension of the spinboxes
                     btnDim=core::dimension2di(120,20), //Dimension of the buttons
                     lblDim=core::dimension2di( 80,20); //Dimension of the labels

  //Create the GUI (all elements that will change / are editable)
  m_pUseAsFirst =pGuienv->addButton  (core::rect<s32>(edtPos,btnDim),m_pWindow,-1,L"Use as first segment"         ); edtPos+=offset;
  m_pBorder1    =pGuienv->addSpinBox (L"0",core::rect<s32>(edtPos,spnDim),true,m_pWindow                          ); edtPos+=offset;
  m_pUseAsSecond=pGuienv->addButton  (core::rect<s32>(edtPos,btnDim),m_pWindow,-1,L"Use as second segment"        ); edtPos+=offset;
  m_pBorder2    =pGuienv->addSpinBox (L"0",core::rect<s32>(edtPos,spnDim),true,m_pWindow                          ); edtPos+=offset;
  m_pSteps      =pGuienv->addSpinBox (L"0",core::rect<s32>(edtPos,spnDim),true,m_pWindow                          ); edtPos+=offset;
  m_pType       =pGuienv->addComboBox(core::rect<s32>(edtPos,btnDim),m_pWindow,-1                                 ); edtPos+=offset;
  m_pFlip       =pGuienv->addCheckBox(false,core::rect<s32>(edtPos,edtDim),m_pWindow                              ); edtPos+=offset;
  m_pFlipVerts  =pGuienv->addCheckBox(false,core::rect<s32>(edtPos,edtDim),m_pWindow                              ); edtPos+=offset;
  m_pTextures   =pGuienv->addComboBox(core::rect<s32>(edtPos,btnDim),m_pWindow                                    );
  m_pEdTexture  =pGuienv->addButton  (core::rect<s32>(edtPos+core::position2di(125,0),btnDim),m_pWindow,-1,L"Edit"); edtPos+=offset;
  m_pOffset     =pGuienv->addSpinBox (L"0",core::rect<s32>(edtPos,spnDim),true,m_pWindow                          ); edtPos+=offset;
  m_pWallHeight =pGuienv->addSpinBox (L"10.0",core::rect<s32>(edtPos,spnDim),true,m_pWindow                       );
  m_pWallWidth  =pGuienv->addSpinBox (L"2.0",core::rect<s32>(edtPos+core::position2di(70,0),spnDim),true,m_pWindow);

  m_pWallHeight->setRange(0.5f,50.0f);
  m_pWallHeight->setDecimalPlaces(1);
  m_pWallHeight->setStepSize(0.1f);

  m_pWallWidth->setRange(0.5f,10.0f);
  m_pWallWidth->setDecimalPlaces(1);
  m_pWallWidth->setStepSize(0.1f);

  core::position2di cbPos=edtPos+core::position2di(140,0);
  for (u32 i=0; i<2; i++) {
    m_pWalls[i]=m_pGuiEnv->addCheckBox(false,core::rect<s32>(cbPos,edtDim),m_pWindow);
    cbPos.X+=30;
  }

  edtPos+=offset;

  //Create the editfield for the help points
  for (u32 i=0; i<4; i++) {
    m_pPoint[i]=pGuienv->addEditBox(L"",core::rect<s32>(edtPos,edtDim),true,m_pWindow);
    m_pPointOff[i]=pGuienv->addSpinBox(L"1",core::rect<s32>(edtPos+core::position2di(200,0),core::dimension2du(50,20)),true,m_pWindow);
    m_pPointOff[i]->setDecimalPlaces(2);
    m_pPointOff[i]->setStepSize(0.01f);
    m_pPointOff[i]->setRange(0.0f,2.0f);
    m_pGuiEnv->addStaticText(L"Offset",core::rect<s32>(edtPos+core::position2di(145,0),core::dimension2du(55,20)),false,false,m_pWindow);
    edtPos+=offset;
  }

  //Init the spinboxes
  m_pBorder1->setRange(0.0f,3.0f); m_pBorder1->setDecimalPlaces(0);
  m_pBorder2->setRange(0.0f,3.0f); m_pBorder2->setDecimalPlaces(0);

  m_pSteps->setRange(0.0f,100.0f); m_pSteps->setDecimalPlaces(0);

  m_pOffset->setRange(0.5f,300.0f); m_pOffset->setDecimalPlaces(2); m_pOffset->setStepSize(0.1f);

  //Init the comboboxes
  m_pType->addItem(L"Bezier1");
  m_pType->addItem(L"Bezier2");
  m_pType->addItem(L"Bezier3");

  m_pTextures->addItem(L"0: road");
  m_pTextures->addItem(L"1: bottom");
  m_pTextures->addItem(L"2: right");
  m_pTextures->addItem(L"3: left");
  m_pTextures->addItem(L"4: wall right outer");
  m_pTextures->addItem(L"5: wall left outer");
  m_pTextures->addItem(L"6: wall right inner");
  m_pTextures->addItem(L"7: wall left inner");
  m_pTextures->addItem(L"8: wall right top");
  m_pTextures->addItem(L"9: wall left top");

  //add the static labels to the GUI
  pGuienv->addStaticText(L"First Segment:"      ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Bordernumber:"       ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Second Segment:"     ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Bordernumber:"       ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Steps:"              ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Type:"               ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Flip Connection:"    ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Flip Vertices:"      ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Texture:"            ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Base Offset:"        ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;
  pGuienv->addStaticText(L"Connection Walls:"   ,core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); lblPos+=offset;

  for (u32 i=0; i<4; i++) {
    core::stringw s=L"Bezier Point #";
    s+=i;

    pGuienv->addStaticText(s.c_str(),core::rect<s32>(lblPos,lblDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER);
    lblPos+=offset;
  }

  m_pSegOne=pGuienv->addStaticText(L"",core::rect<s32>(230, 30,490, 50),false,false,m_pWindow); m_pSegOne->setTextAlignment(gui::EGUIA_UPPERLEFT,gui::EGUIA_CENTER);
  m_pSegTwo=pGuienv->addStaticText(L"",core::rect<s32>(230, 70,490,110),false,false,m_pWindow); m_pSegTwo->setTextAlignment(gui::EGUIA_UPPERLEFT,gui::EGUIA_CENTER);

  //Init the other members
  m_pSelectedCon=NULL;
  m_pSelectedSeg=NULL;

  m_pTextureParams=pTex;

  updateUI();
}

/**
 * Update the UI (copy changes from the actual connection object)
 */
void CConnectionParameterWindow::updateUI() {
  if (m_pSelectedCon==NULL) return;

  //Update the "Bezier Type" Combobox
  m_pType->setSelected(m_pSelectedCon->getConnectionType());

  //Enabled/Disable the help points buttons depending on the bezier type
  switch (m_pType->getSelected()) {
    case 0:
      m_pPoint[0]->setEnabled(false); m_pPointOff[0]->setEnabled(false);
      m_pPoint[1]->setEnabled(false); m_pPointOff[1]->setEnabled(false);
      m_pPoint[2]->setEnabled(false); m_pPointOff[2]->setEnabled(false);
      m_pPoint[3]->setEnabled(false); m_pPointOff[3]->setEnabled(false);
      break;

    case 1:
      m_pPoint[0]->setEnabled(true ); m_pPointOff[0]->setEnabled(true );
      m_pPoint[1]->setEnabled(false); m_pPointOff[1]->setEnabled(false);
      m_pPoint[2]->setEnabled(true ); m_pPointOff[2]->setEnabled(true );
      m_pPoint[3]->setEnabled(false); m_pPointOff[3]->setEnabled(false);
      break;

    case 2:
      m_pPoint[0]->setEnabled(true); m_pPointOff[0]->setEnabled(true);
      m_pPoint[1]->setEnabled(true); m_pPointOff[1]->setEnabled(true);
      m_pPoint[2]->setEnabled(true); m_pPointOff[2]->setEnabled(true);
      m_pPoint[3]->setEnabled(true); m_pPointOff[3]->setEnabled(true);
      break;
  }

  //Update the textfield of the two connected segments
  if (m_pSelectedCon->getSegment1()!=NULL) m_pSegOne->setText(core::stringw(m_pSelectedCon->getSegment1()->getName()).c_str());
  if (m_pSelectedCon->getSegment2()!=NULL) m_pSegTwo->setText(core::stringw(m_pSelectedCon->getSegment2()->getName()).c_str());

  //Update the bezier point editfields
  for (u32 i=0; i<4; i++) {
    char s[0xFF];
    core::vector3df v=m_pSelectedCon->getHelpPoint(i);
    sprintf(s,"%f, %f, %f",v.X,v.Y,v.Z);
    m_pPoint[i]->setText(core::stringw(s).c_str());

    m_pPointOff[i]->setValue(m_pSelectedCon->getHpOffset(i));
  }

  //Update the spinboxes
  m_pBorder1->setValue(m_pSelectedCon->getSegment1Border());
  m_pBorder2->setValue(m_pSelectedCon->getSegment2Border());
  m_pSteps  ->setValue(m_pSelectedCon->getNumberOfSteps ());
  m_pOffset ->setValue(m_pSelectedCon->getOffset        ());

  //Update the checkboxes
  m_pFlip      ->setChecked(m_pSelectedCon->getFlipConnection());
  m_pFlipVerts ->setChecked(m_pSelectedCon->getFlipVertices  ());

  m_pWallHeight->setValue(m_pSelectedCon->getWallHeight());
  m_pWallWidth ->setValue(m_pSelectedCon->getWallWidth ());

  for (u32 i=0; i<2; i++)
    m_pWalls[i]->setChecked(m_pSelectedCon->getWallFlag(i));
}

/**
 * Update the data (copy changes to the actual connection object). Please
 * note that in this case there are special updates done when an event from
 * a GUI element is processed, this method just updates the help points.
 */
void CConnectionParameterWindow::updateData() {
  for (u32 i=0; i<4; i++) {
    core::array<core::stringc> aParts;
    core::stringc s=core::stringc(m_pPoint[i]->getText());
    s.split(aParts,",");
    core::vector3df v=core::vector3df(0,0,0);
    for (u32 j=0; j<aParts.size() && j<3; j++)
      switch (j) {
        case 0: v.X=atof(aParts[0].c_str()); break;
        case 1: v.Y=atof(aParts[1].c_str()); break;
        case 2: v.Z=atof(aParts[2].c_str()); break;
      }
    m_pSelectedCon->setHelpPoint(i,v);
  }
}

void CConnectionParameterWindow::setSelectedConnection(CConnection *p) {
  if (m_pSelectedCon!=NULL) m_pSelectedCon->setSelected(false);
  printf("set selected connection!\n");
  m_pSelectedCon=p;
  if (p!=NULL) {
    p->setSelectedPoint(-1);
    updateUI();
    m_pSelectedCon->setSelected(true);
  }
}

void CConnectionParameterWindow::setSelectedSegment(CSegment *p) {
  m_pSelectedSeg=p;
  updateUI();
}

/**
 * Irrlicht event callback
 * @param event the event from Irrlicht
 */
bool CConnectionParameterWindow::OnEvent(const SEvent &event) {
  if (m_pSelectedCon==NULL) return false;

  //just handle GUI events
  if (event.EventType==EET_GUI_EVENT) {
    //If the focus was lost...
    if (event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST) {
      gui::IGUIElement *p=event.GUIEvent.Caller;

      //...and the caller is one of the help point edit fields...
      if (p==m_pType || p==m_pFlip || p==m_pPoint[0] || p==m_pPoint[1] || p==m_pPoint[2] || p==m_pPoint[3]) {
        //...we update the date and the UI
        m_pSelectedCon->setSelectedPoint(-1);
        updateData();
        updateUI();
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUSED) {
      gui::IGUIElement *p=event.GUIEvent.Caller;

      //If one of the help points edit fields is focued we tell the
      //selected connection about this for highlighting of the point
      if (p==m_pPoint[0]) m_pSelectedCon->setSelectedPoint(1);
      if (p==m_pPoint[1]) m_pSelectedCon->setSelectedPoint(2);
      if (p==m_pPoint[2]) m_pSelectedCon->setSelectedPoint(3);
      if (p==m_pPoint[3]) m_pSelectedCon->setSelectedPoint(0);
    }

    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      //The "Use as first segment" button was clicked
      if (event.GUIEvent.Caller==m_pUseAsFirst && m_pSelectedSeg!=NULL) {
        if (m_pSelectedCon->getSegment2()!=m_pSelectedSeg) {
          m_pSelectedCon->setSegment1(m_pSelectedSeg);
          updateUI();
        }
      }

      //The "Use as second segment" button was clicked
      if (event.GUIEvent.Caller==m_pUseAsSecond && m_pSelectedSeg!=NULL) {
        if (m_pSelectedCon->getSegment1()!=m_pSelectedSeg) {
          m_pSelectedCon->setSegment2(m_pSelectedSeg);
          updateUI();
        }
      }

      //The "Edit Texture" button was clicked
      if (event.GUIEvent.Caller==m_pEdTexture) {
        m_pTextureParams->setParameters(m_pSelectedCon->getTextureParameters(m_pTextures->getSelected()));
        m_pTextureParams->setVisible(true);
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_SPINBOX_CHANGED) {
      //The "Segment 1 Border" Spinbox was changed so the selected connection needs an update
      if (m_pSelectedCon->getSegment1()!=NULL && event.GUIEvent.Caller==m_pBorder1)
        m_pSelectedCon->setSegment1Border((u32)m_pBorder1->getValue());

      //The "Segment 2 Border" Spinbox was changed so the selected connection needs an update
      if (m_pSelectedCon->getSegment2()!=NULL && event.GUIEvent.Caller==m_pBorder2)
        m_pSelectedCon->setSegment2Border((u32)m_pBorder2->getValue());

      //The "Number of Steps" Spinbox was changed so the selected connection needs an update
      if (event.GUIEvent.Caller==m_pSteps)
        m_pSelectedCon->setNumberOfSteps((u32)m_pSteps->getValue());

      //The "Base Offset" Spinbox was changed so the selected connection needs an update
      if (event.GUIEvent.Caller==m_pOffset)
        m_pSelectedCon->setOffset(m_pOffset->getValue());

      for (u32 i=0; i<4; i++) {
        if (event.GUIEvent.Caller==m_pPointOff[i]) {
          m_pSelectedCon->setHpOffset(i,m_pPointOff[i]->getValue());
          updateUI();
        }
      }

      if (event.GUIEvent.Caller==m_pWallHeight) {
        m_pSelectedCon->setWallHeight(m_pWallHeight->getValue());
        updateUI();
      }

      if (event.GUIEvent.Caller==m_pWallWidth) {
        m_pSelectedCon->setWallWidth(m_pWallWidth->getValue());
        updateUI();
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_COMBO_BOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pType) {
        //The "Bezier Type" combobox has changed
        switch (m_pType->getSelected()) {
          case 0: m_pSelectedCon->setType(CConnection::eBezier1); break;
          case 1: m_pSelectedCon->setType(CConnection::eBezier2); break;
          case 2: m_pSelectedCon->setType(CConnection::eBezier3); break;
        }
        updateUI();
      }

      //The "Texture" combobox has changed
      if (event.GUIEvent.Caller==m_pTextures) {
        m_pTextureParams->setVisible(false);
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_CHECKBOX_CHANGED) {
      //The "Flip Connection" checkbox has changed
      if (event.GUIEvent.Caller==m_pFlip) {
        m_pSelectedCon->setFlipConnection(m_pFlip->isChecked());
      }

      //The "Flip Vertices" checkbox has changed
      if (event.GUIEvent.Caller==m_pFlipVerts) {
        m_pSelectedCon->setFlipVertices(m_pFlipVerts->isChecked());
      }

      for (u32 i=0; i<2; i++)
        if (event.GUIEvent.Caller==m_pWalls[i])
          m_pSelectedCon->setWallFlag(i,m_pWalls[i]->isChecked());

      updateUI();
    }
  }
  return false;
}

void CConnectionParameterWindow::textureModified() {
  if (m_pSelectedCon) m_pSelectedCon->update();
}

const core::stringc &CConnectionParameterWindow::getSaveName() {
  static core::stringc s="ConnectionParameterWindow";
  return s;
}
