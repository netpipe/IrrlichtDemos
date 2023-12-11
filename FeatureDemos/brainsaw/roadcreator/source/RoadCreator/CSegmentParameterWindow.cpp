  #include <CSegment.h>
  #include <CMaterialManager.h>
  #include <CSegmentParameterWindow.h>
  #include <CTextureParameterWindow.h>

using namespace irr;

/**
 * The constructor
 * @param pGuienv Irrlicht GUI environment
 * @param pTex the "
 */
CSegmentParameterWindow::CSegmentParameterWindow(gui::IGUIEnvironment *pGuienv, CTextureParameterWindow *pTex)
                                                : CDialogWindow(pGuienv,
                                                                core::position2di(100,100),
                                                                core::dimension2du(400,310),false,
                                                                L"Edit Segment Parameters") {

  core::position2di pos=core::position2di(100,30),    //position of the edit fields
                    txt=core::position2di( 10,30),    //position of the labels
                    off=core::position2di(  0,25);    //offset between the lines

  core::dimension2di dim=core::dimension2di(285,20),
                     dtx=core::dimension2di( 80,20),
                     dcb=core::dimension2di(120,20);

  //Add the editfields
  m_pName       =pGuienv->addEditBox (L""  ,core::rect<s32>(pos,dim),true,m_pWindow       ); pos+=off;
  m_pWidth      =pGuienv->addEditBox (L""  ,core::rect<s32>(pos,dim),true,m_pWindow       ); pos+=off;
  m_pLength     =pGuienv->addEditBox (L""  ,core::rect<s32>(pos,dim),true,m_pWindow       ); pos+=off;
  m_pPosition   =pGuienv->addEditBox (L""  ,core::rect<s32>(pos,dim),true,m_pWindow       ); pos+=off;
  m_pDirection  =pGuienv->addEditBox (L""  ,core::rect<s32>(pos,dim),true,m_pWindow       ); pos+=off;
  m_pNormal     =pGuienv->addEditBox (L""  ,core::rect<s32>(pos,dim),true,m_pWindow       ); pos+=off;
  m_pTexture    =pGuienv->addComboBox(      core::rect<s32>(pos,dcb),     m_pWindow       );

  //initialize the editfields
  m_pEdTexture=pGuienv->addButton(core::rect<s32>(pos+core::position2di(125,0),dcb),m_pWindow,-1,L"Edit"); pos+=off;

  m_pLevelBase =pGuienv->addCheckBox(false,core::rect<s32>(pos+core::position2di( 75,0),dcb),m_pWindow);
  m_pNormalBase=pGuienv->addCheckBox(false,core::rect<s32>(pos+core::position2di(180,0),dcb),m_pWindow);
  m_pNormalWall=pGuienv->addCheckBox(false,core::rect<s32>(pos+core::position2di(265,0),dcb),m_pWindow);
  pos+=off;
  
  m_pBaseOffset=pGuienv->addSpinBox(L"0",core::rect<s32>(pos,dcb),true,m_pWindow); pos+=off;

  m_pBaseOffset->setStepSize(0.1f);
  m_pBaseOffset->setDecimalPlaces(2);
  m_pBaseOffset->setRange(1.0f,500.0f);

  m_pWallHeight =pGuienv->addSpinBox(L"5.0",core::rect<s32>(pos,dcb),true,m_pWindow);

  m_pWallHeight->setStepSize(0.1f);
  m_pWallHeight->setDecimalPlaces(1);
  m_pWallHeight->setRange(0.5f,50.0f);

  core::position2di cbPos=pos+core::position2di(130,0);
  for (u32 i=0; i<4; i++) {
    m_pWalls[i]=pGuienv->addCheckBox(false,core::rect<s32>(cbPos,core::dimension2du(25,25)),m_pWindow);
    cbPos.X+=30;
  }
  
  pos+=off;
  m_pWallWidth=pGuienv->addSpinBox(L"2.5",core::rect<s32>(pos,dcb),true,m_pWindow);
  
  cbPos=pos+core::position2di(130,0);
  for (u32 i=0; i<4; i++) {
    m_pCorner[i]=pGuienv->addCheckBox(false,core::rect<s32>(cbPos,core::dimension2di(25,25)),m_pWindow);
    cbPos.X+=30;
  }
  
  m_pWallWidth->setStepSize(0.1f);
  m_pWallWidth->setDecimalPlaces(1);
  m_pWallWidth->setRange(0.5f,50.0f);

  //add the labels
  pGuienv->addStaticText(L"Name:"           ,core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); txt+=  off;
  pGuienv->addStaticText(L"Width:"          ,core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); txt+=  off;
  pGuienv->addStaticText(L"Length:"         ,core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); txt+=  off;
  pGuienv->addStaticText(L"Position:"       ,core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); txt+=  off;
  pGuienv->addStaticText(L"Direction:"      ,core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); txt+=  off;
  pGuienv->addStaticText(L"Normal:"         ,core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); txt+=2*off;
  pGuienv->addStaticText(L"Basement:"       ,core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER);
  
  pGuienv->addStaticText(L"Level Basement: " ,core::rect<s32>(txt+core::position2di(dtx.Width    ,0),core::dimension2du(75,20)),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER);
  pGuienv->addStaticText(L"Basement Normal: ",core::rect<s32>(txt+core::position2di(dtx.Width+110,0),core::dimension2du(75,20)),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); 
  pGuienv->addStaticText(L"Wall Normal:"     ,core::rect<s32>(txt+core::position2di(dtx.Width+215,0),core::dimension2du(50,20)),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER);
  txt+=off;
  
  pGuienv->addStaticText(L"Basement Offset:",core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); txt+=  off;
  pGuienv->addStaticText(L"Segment Walls:"  ,core::rect<s32>(txt,dtx),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); txt+=  off;

  //init some more members
  m_pSegment=NULL;

  m_pTextureParams=pTex;

  //add the texture selection items
  m_pTexture->addItem(L"0: road");
  m_pTexture->addItem(L"1: bottom");
  m_pTexture->addItem(L"2: front");
  m_pTexture->addItem(L"3: left");
  m_pTexture->addItem(L"4: back");
  m_pTexture->addItem(L"5: right");
  m_pTexture->addItem(L"6: wall front outer");
  m_pTexture->addItem(L"7: wall left outer");
  m_pTexture->addItem(L"8: wall back outer");
  m_pTexture->addItem(L"9: wall right outer");
  m_pTexture->addItem(L"10: wall front inner");
  m_pTexture->addItem(L"11: wall left inner");
  m_pTexture->addItem(L"12: wall back inner");
  m_pTexture->addItem(L"13: wall right inner");
  m_pTexture->addItem(L"14: wall front top");
  m_pTexture->addItem(L"15: wall left top");
  m_pTexture->addItem(L"16: wall back top");
  m_pTexture->addItem(L"17: wall right top");
}


/**
 * Update the UI (Segment object --> UI)
 */
void CSegmentParameterWindow::updateUI() {
  //No segment selected ==> nothing to update
  if (m_pSegment==NULL) return;

  c8 s[0xFF];

  core::stringc sName=m_pSegment->getName();
  m_pName->setText(core::stringw(sName.c_str()).c_str());

  sprintf(s,"%f",m_pSegment->getWidth ()); m_pWidth ->setText(core::stringw(s).c_str());
  sprintf(s,"%f",m_pSegment->getLength()); m_pLength->setText(core::stringw(s).c_str());

  core::vector3df v=m_pSegment->getPosition();
  sprintf(s,"%f, %f, %f",v.X,v.Y,v.Z); m_pPosition->setText(core::stringw(s).c_str());

  v=m_pSegment->getDirection();
  sprintf(s,"%f, %f, %f",v.X,v.Y,v.Z); m_pDirection->setText(core::stringw(s).c_str());

  v=m_pSegment->getNormal();
  sprintf(s,"%f, %f, %f",v.X,v.Y,v.Z); m_pNormal->setText(core::stringw(s).c_str());

  m_pLevelBase ->setChecked(m_pSegment->getLevelBase ());
  m_pNormalBase->setChecked(m_pSegment->getNormalBase());
  m_pNormalWall->setChecked(m_pSegment->getNormalWall());
  
  m_pBaseOffset->setValue(m_pSegment->getBaseOffset());

  m_pWallHeight->setValue(m_pSegment->getWallHeight());
  m_pWallWidth->setValue(m_pSegment->getWallWidth());
  for (u32 i=0; i<4; i++) {
    m_pWalls [i]->setChecked(m_pSegment->getWallFlag  (i));
    m_pCorner[i]->setChecked(m_pSegment->getCornerFlag(i));
  }
}

/**
 * Update the segment object (UI --> Segment object)
 */
void CSegmentParameterWindow::updateSegment() {
  //No segment selected ==> nothing to update
  if (m_pSegment==NULL) return;

  m_pSegment->setWidth (atof(core::stringc(m_pWidth ->getText()).c_str()));
  m_pSegment->setLength(atof(core::stringc(m_pLength->getText()).c_str()));

  core::array<core::stringc> aParts;
  core::stringc s=core::stringc(m_pPosition->getText());
  s.split(aParts,",");
  core::vector3df v=core::vector3df(0,0,0);
  for (u32 i=0; i<aParts.size() && i<3; i++)
    switch (i) {
      case 0: v.X=atof(aParts[i].c_str()); break;
      case 1: v.Y=atof(aParts[i].c_str()); break;
      case 2: v.Z=atof(aParts[i].c_str()); break;
    }

  m_pSegment->setPosition(v);

  s=core::stringc(m_pDirection->getText());
  aParts.clear();
  s.split(aParts,",");
  v=core::vector3df(0,0,0);
  for (u32 i=0; i<aParts.size() && i<3; i++)
    switch (i) {
      case 0: v.X=atof(aParts[i].c_str()); break;
      case 1: v.Y=atof(aParts[i].c_str()); break;
      case 2: v.Z=atof(aParts[i].c_str()); break;
    }
  if (v.getLength()!=0) m_pSegment->setDirection(v);


  s=core::stringc(m_pNormal->getText());
  aParts.clear();
  s.split(aParts,",");
  v=core::vector3df(0,0,0);
  for (u32 i=0; i<aParts.size() && i<3; i++)
    switch (i) {
      case 0: v.X=atof(aParts[i].c_str()); break;
      case 1: v.Y=atof(aParts[i].c_str()); break;
      case 2: v.Z=atof(aParts[i].c_str()); break;
    }
  if (v.getLength()!=0) m_pSegment->setNormal(v);

  m_pSegment->setLevelBase (m_pLevelBase ->isChecked());
  m_pSegment->setNormalBase(m_pNormalBase->isChecked());
  m_pSegment->setNormalWall(m_pNormalWall->isChecked());
  
  m_pSegment->setBaseOffset(m_pBaseOffset->getValue());

  for (u32 i=0; i<4; i++) {
    m_pSegment->setWallFlag  (i,m_pWalls [i]->isChecked());
    m_pSegment->setCornerFlag(i,m_pCorner[i]->isChecked());
  }
  m_pSegment->setWallHeight(m_pWallHeight->getValue());
  m_pSegment->setWallWidth(m_pWallWidth->getValue());

  updateUI();
}

/**
 * The callback of the Irrlicht event receiver
 * @param event the event to handle
 */
bool CSegmentParameterWindow::OnEvent(const SEvent &event) {
  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==gui::EGET_ELEMENT_FOCUS_LOST) {
      //if one of the editfields has lost the focus we should update the segment
      gui::IGUIElement *p=event.GUIEvent.Caller;

      if (p==m_pName   || p==m_pWidth     || p==m_pLength    || p==m_pPosition   || p==m_pDirection  || p==m_pNormalWall ||
          p==m_pNormal || p==m_pLevelBase || p==m_pBaseOffset|| p==m_pWallHeight || p==m_pNormalBase || p==m_pWallWidth)
        updateSegment();
    }

    //The selected texture has changed
    if (event.GUIEvent.EventType==gui::EGET_COMBO_BOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pTexture) {
        m_pTextureParams->setVisible(false);
      }
    }

    //The "edit texture" button was clicked
    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      if (event.GUIEvent.Caller==m_pEdTexture) {
        m_pTextureParams->setParameters(m_pSegment->getTextureParameters(m_pTexture->getSelected()));
        m_pTextureParams->setVisible(true);
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_CHECKBOX_CHANGED) {
      for (u32 i=0; i<4; i++) {
        if (event.GUIEvent.Caller==m_pWalls[i]) {
          m_pSegment->setWallFlag(i,m_pWalls[i]->isChecked());
          updateUI();
          break;
        }
        
        if (event.GUIEvent.Caller==m_pCorner[i]) {
          m_pSegment->setCornerFlag(i,m_pCorner[i]->isChecked());
          updateUI();
          break;
        }
      }
      
      if (event.GUIEvent.Caller==m_pLevelBase) {
        updateSegment();
        updateUI();
      }
      
      if (event.GUIEvent.Caller==m_pNormalBase) {
        updateSegment();
        updateUI();
      }
      
      if (event.GUIEvent.Caller==m_pNormalWall) {
        updateSegment();
        updateUI();
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_SPINBOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pWallHeight) {
        m_pSegment->setWallHeight(m_pWallHeight->getValue());
        updateUI();
      }
      
      if (event.GUIEvent.Caller==m_pWallWidth) {
        m_pSegment->setWallWidth(m_pWallWidth->getValue());
        updateUI();
      }
      
      if (event.GUIEvent.Caller==m_pBaseOffset) {
        m_pSegment->setBaseOffset(m_pBaseOffset->getValue());
        updateUI();
      }
    }
  }

  return false;
};
