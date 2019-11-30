  #include <CTextureParameterWindow.h>
  #include <CSurfaceWindow.h>
  #include <CSurface.h>
  
using namespace irr;

CSurfaceWindow::CSurfaceWindow(gui::IGUIEnvironment *pGuienv, CTextureParameterWindow *pTex) : CDialogWindow(pGuienv,core::position2di(100,100),core::dimension2du(330,260),false,L"Surface Parameters") {
  core::position2di  cEdPos =core::position2di(120,30),
                     cE2Pos =core::position2di(220,30),
                     cOffset=core::position2di(  0,25),
                     cLblPos=core::position2di( 10,30);
  core::dimension2du cEdDim=core::dimension2du(100,20);


  m_pOffset =m_pGuiEnv->addEditBox(L"",core::rect<s32>(cEdPos,cEdDim),true,m_pWindow); cEdPos+=cOffset; cE2Pos+=cOffset;
  m_pStart_X=m_pGuiEnv->addEditBox(L"",core::rect<s32>(cEdPos,cEdDim),true,m_pWindow); 
  m_pStart_Y=m_pGuiEnv->addEditBox(L"",core::rect<s32>(cE2Pos,cEdDim),true,m_pWindow); cEdPos+=cOffset; cE2Pos+=cOffset;
  m_pEnd_X  =m_pGuiEnv->addEditBox(L"",core::rect<s32>(cEdPos,cEdDim),true,m_pWindow); 
  m_pEnd_Y  =m_pGuiEnv->addEditBox(L"",core::rect<s32>(cE2Pos,cEdDim),true,m_pWindow); cEdPos+=cOffset; cE2Pos+=cOffset;
  m_pFences =m_pGuiEnv->addEditBox(L"",core::rect<s32>(cEdPos,cEdDim),true,m_pWindow); cEdPos+=cOffset; cE2Pos+=cOffset;
  m_pTexR_X =m_pGuiEnv->addEditBox(L"",core::rect<s32>(cEdPos,cEdDim),true,m_pWindow);
  m_pTexR_Y =m_pGuiEnv->addEditBox(L"",core::rect<s32>(cE2Pos,cEdDim),true,m_pWindow); cEdPos+=cOffset; cE2Pos+=cOffset;

  m_pTextureSelect=m_pGuiEnv->addComboBox(core::rect<s32>(cEdPos,cEdDim),m_pWindow);
  
  m_pTexture=m_pGuiEnv->addButton(core::rect<s32>(cE2Pos,cEdDim),m_pWindow,-1,L"Edit"          ); cEdPos+=2*cOffset; cE2Pos+=2*cOffset;
  m_pCreate =m_pGuiEnv->addButton(core::rect<s32>(cEdPos,cEdDim),m_pWindow,-1,L"Create Surface"); cEdPos+=  cOffset; cE2Pos+=  cOffset;
  m_pRemove =m_pGuiEnv->addButton(core::rect<s32>(cEdPos,cEdDim),m_pWindow,-1,L"Remove Surface"); cEdPos+=  cOffset; cE2Pos+=  cOffset;
  
  m_pGuiEnv->addStaticText(L"Y of Surface:"    ,core::rect<s32>(cLblPos,cEdDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cLblPos+=cOffset;
  m_pGuiEnv->addStaticText(L"Min Coordinate:"  ,core::rect<s32>(cLblPos,cEdDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cLblPos+=cOffset;
  m_pGuiEnv->addStaticText(L"Max Coordinate:"  ,core::rect<s32>(cLblPos,cEdDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cLblPos+=cOffset;
  m_pGuiEnv->addStaticText(L"Height of Fences:",core::rect<s32>(cLblPos,cEdDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cLblPos+=cOffset;
  m_pGuiEnv->addStaticText(L"Texture Repeat:"  ,core::rect<s32>(cLblPos,cEdDim),false,false,m_pWindow)->setTextAlignment(gui::EGUIA_LOWERRIGHT,gui::EGUIA_CENTER); cLblPos+=cOffset;
  
  m_pTextureSelect->addItem(L"0: ground");
  m_pTextureSelect->addItem(L"1: fence");
  
  pTex->addNotify(this);
  m_pTextureParams=pTex;
  m_pSurface=NULL;
}


void CSurfaceWindow::updateSurface() {
  core::vector3df v1,v2;
  
  v1.X=atof(core::stringc(m_pStart_X->getText()).c_str());
  v1.Y=atof(core::stringc(m_pOffset ->getText()).c_str());
  v1.Z=atof(core::stringc(m_pStart_Y->getText()).c_str());

  v2.X=atof(core::stringc(m_pEnd_X ->getText()).c_str());
  v2.Y=atof(core::stringc(m_pOffset->getText()).c_str());
  v2.Z=atof(core::stringc(m_pEnd_Y ->getText()).c_str());
  
  core::vector2df v;
  
  v.X=atof(core::stringc(m_pTexR_X->getText()).c_str());
  v.Y=atof(core::stringc(m_pTexR_Y->getText()).c_str());
  
  m_pSurface->setCorners(v1,v2);
  m_pSurface->setFenceHeight(atof(core::stringc(m_pFences->getText()).c_str()));
  m_pSurface->recalcMeshBuffer();
}

bool CSurfaceWindow::OnEvent(const SEvent &event) {
  if (m_pSurface==NULL) return false;
  
  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==gui::EGET_EDITBOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pOffset ) updateSurface();
      if (event.GUIEvent.Caller==m_pStart_X) updateSurface();
      if (event.GUIEvent.Caller==m_pStart_Y) updateSurface();
      if (event.GUIEvent.Caller==m_pEnd_X  ) updateSurface();
      if (event.GUIEvent.Caller==m_pEnd_Y  ) updateSurface();
      if (event.GUIEvent.Caller==m_pFences ) updateSurface();
      if (event.GUIEvent.Caller==m_pTexR_X ) updateSurface();
      if (event.GUIEvent.Caller==m_pTexR_Y ) updateSurface();
    }
    
    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      if (event.GUIEvent.Caller==m_pTexture) {
        m_pTextureParams->setParameters(m_pSurface->getTextureParameters(m_pTextureSelect->getSelected()));
        m_pTextureParams->setVisible(true);
      }
      
      if (event.GUIEvent.Caller==m_pCreate) m_pSurface->setVisible(true );
      if (event.GUIEvent.Caller==m_pRemove) m_pSurface->setVisible(false);
    }
    
    if (event.GUIEvent.EventType==gui::EGET_COMBO_BOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pTextureSelect) {
        bool b=m_pSurface->getMeshBuffer(m_pTextureSelect->getSelected())!=NULL;
        m_pTexture->setEnabled(b);
        m_pTextureParams->setVisible(false);
      }
    }
  }
  
  return false;
}

void CSurfaceWindow::setSurface(CSurface *p) { 
  m_pSurface=p; 
  
  p->recalcMeshBuffer();
  
  core::vector3df v1=p->getMinPos(),
                  v2=p->getMaxPos();
  
  m_pStart_X->setText(core::stringw(v1.X).c_str());
  m_pStart_Y->setText(core::stringw(v1.Z).c_str());
  m_pEnd_X  ->setText(core::stringw(v2.X).c_str());
  m_pEnd_Y  ->setText(core::stringw(v2.Z).c_str());
  m_pOffset ->setText(core::stringw(v1.Y).c_str());
  
  m_pFences->setText(core::stringw(p->getFenceHeight()).c_str());
  
  core::vector2df v=p->getTextureRepeat();
  
  m_pTexR_X->setText(core::stringw(v.X).c_str());
  m_pTexR_Y->setText(core::stringw(v.Y).c_str());
}
