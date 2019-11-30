  #include <CAlignDialog.h>
  #include <CSegment.h>
  #include <irrlicht.h>

using namespace irr;

CAlignDialog::CAlignDialog(IrrlichtDevice *pDevice) : CDialogWindow(pDevice->getGUIEnvironment(),core::position2di(300,300),core::dimension2du(200,200),false,L"Align Segment to Scenenode") {
  m_pSceneNode=NULL;
  m_pHoverNode=NULL;

  m_bSelect=false;

  m_pDevice=pDevice;
  m_pDrv=m_pDevice->getVideoDriver();
  m_pSmgr=m_pDevice->getSceneManager();

  m_lblSegment=m_pGuiEnv->addStaticText(L"Selected Segment",core::rect<s32>(5,20,195,40),false,false,m_pWindow);
  m_pGuiEnv->addStaticText(L"Node",core::rect<s32>(5,45,55,65),false,false,m_pWindow);
  m_pSegVtx=m_pGuiEnv->addSpinBox(L"0",core::rect<s32>(55,45,105,65),true,m_pWindow);
  m_pSegVtx->setRange(0,3);
  m_pSegVtx->setDecimalPlaces(0);

  core::position2di cPos=core::position2di(5,100);
  m_lblSceneNode=m_pGuiEnv->addStaticText(L"Selected Node",core::rect<s32>(cPos,core::dimension2du(190,20)),false,false,m_pWindow);
  cPos+=core::position2di(0,25);
  m_pGuiEnv->addStaticText(L"Material",core::rect<s32>(cPos,core::dimension2du(50,20)),false,false,m_pWindow);
  m_pMaterial=m_pGuiEnv->addSpinBox(L"0",core::rect<s32>(cPos+core::position2di(50,0),core::dimension2du(50,20)),true,m_pWindow);
  m_lblMatCnt=m_pGuiEnv->addStaticText(L"0",core::rect<s32>(cPos+core::position2di(105,0),core::dimension2du(50,20)),false,false,m_pWindow);
  cPos+=core::position2di(0,25);
  m_pGuiEnv->addStaticText(L"Vertex",core::rect<s32>(cPos,core::dimension2du(50,20)),false,false,m_pWindow);
  m_pVertex=m_pGuiEnv->addSpinBox(L"0",core::rect<s32>(cPos+core::position2di(50,0),core::dimension2du(50,20)),true,m_pWindow);
  m_lblVtxCnt=m_pGuiEnv->addStaticText(L"0",core::rect<s32>(cPos+core::position2di(105,0),core::dimension2du(50,20)),false,false,m_pWindow);

  m_pAlign   =m_pGuiEnv->addButton(core::rect<s32>(125,175,195,195),m_pWindow,-1,L"Align!"   );
  m_pPickNode=m_pGuiEnv->addButton(core::rect<s32>(  5,175, 75,195),m_pWindow,-1,L"Pick Node");

  m_pMaterial->setDecimalPlaces(0);
  m_pVertex->setDecimalPlaces(0);

  m_iNodeMaterial=0;
  m_iNodeVertex=0;
  m_iSegNode=0;

  m_cBoxNode.addInternalPoint(-5,-5,-5);
  m_cBoxNode.addInternalPoint( 5, 5, 5);

  m_cBoxSeg.addInternalPoint(-5,-5,-5);
  m_cBoxSeg.addInternalPoint( 5, 5, 5);
  m_iState=0;
}

bool CAlignDialog::OnEvent(const SEvent &event) {
  bool bRet=false;

  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==gui::EGET_SPINBOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pMaterial) {
        m_iNodeMaterial=(u32)m_pMaterial->getValue();
        materialSelect();
      }

      if (event.GUIEvent.Caller==m_pVertex) {
        m_iNodeVertex=(u32)m_pVertex->getValue();
        vertexSelect();
      }

      if (event.GUIEvent.Caller==m_pSegVtx) {
        m_iSegNode=(u32)m_pSegVtx->getValue();
        segmentSelect();
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      if (event.GUIEvent.Caller==m_pAlign) {
        if (m_pSegment!=NULL && m_pSceneNode!=NULL) {
          vertexSelect();
          segmentSelect();

          core::vector3df vDiff=m_vSeg-m_vNode;
          m_pSegment->setPosition(m_pSegment->getPosition()-vDiff);
          m_pSegment->update();
        }
      }

      if (event.GUIEvent.Caller==m_pPickNode) toggleState();
    }
  }

  if (event.EventType==EET_MOUSE_INPUT_EVENT) {
  }

  return bRet;
}

void CAlignDialog::vertexSelect() {
  if (!m_pSceneNode) return;

  m_vNode=m_pSceneNode->getAbsolutePosition();
  core::vector3df vSize=core::vector3df(5,5,5),vVert;

  switch (m_pSceneNode->getType()) {
    case scene::ESNT_ANIMATED_MESH:
      vVert=m_pSceneNode->getScale()*((scene::IAnimatedMeshSceneNode *)(m_pSceneNode))->getMesh()->getMeshBuffer(m_iNodeMaterial)->getPosition(m_iNodeVertex);
      break;

    case scene::ESNT_MESH:
      vVert=m_pSceneNode->getScale()*((scene::IMeshSceneNode *)m_pSceneNode)->getMesh()->getMeshBuffer(m_iNodeMaterial)->getPosition(m_iNodeVertex);
      break;

    default: break;
  }

  vVert=m_pSceneNode->getRotation().rotationToDirection(vVert);
  m_vNode+=vVert;

  m_cBoxNode.MinEdge=m_vNode-vSize;
  m_cBoxNode.MaxEdge=m_vNode+vSize;
}

void CAlignDialog::materialSelect() {
  if (!m_pSceneNode) return;

  wchar_t s[0xFF];
  u32 iCnt=0;

  switch (m_pSceneNode->getType()) {
    case scene::ESNT_ANIMATED_MESH:
      iCnt=((scene::IAnimatedMeshSceneNode *)(m_pSceneNode))->getMesh()->getMeshBuffer(m_iNodeMaterial)->getVertexCount();
      break;

    case scene::ESNT_MESH:
      iCnt=((scene::IMeshSceneNode *)m_pSceneNode)->getMesh()->getMeshBuffer(m_iNodeMaterial)->getVertexCount();
      break;

    default: break;
  }

  swprintf(s,0xFF,L"%i",iCnt);
  m_lblVtxCnt->setText(s);
  m_pVertex->setRange(0,iCnt-1);
}

void CAlignDialog::setSelectedNode(scene::ISceneNode *p) {
  if (m_pSceneNode) m_pSceneNode->setMaterialFlag(video::EMF_LIGHTING,false);
  m_pSceneNode=p;

  if (m_pSceneNode) {
    m_pSceneNode->setMaterialFlag(video::EMF_LIGHTING,true);
    wchar_t s[0xFF];
    swprintf(s,0xFF,L"Selected Node: \"%s\" (%i)",core::stringw(m_pSceneNode->getName()).c_str(),m_pSceneNode->getID());
    m_lblSceneNode->setText(s);
    m_iNodeMaterial=0;
    m_iNodeVertex=0;
    m_pMaterial->setValue(0);
    m_pVertex->setValue(0);

    m_pMaterial->setRange(0,m_pSceneNode->getMaterialCount()-1);
    swprintf(s,0xFF,L"%i",m_pSceneNode->getMaterialCount());
    m_lblMatCnt->setText(s);

    materialSelect();
    m_pVertex->setValue(0.0f);
  }
}

void CAlignDialog::draw(bool bLeftBtn, s32 iMouseX, s32 iMouseY) {
  video::SMaterial cMat;
  cMat.Lighting=false;
  m_pDrv->setMaterial(cMat);
  m_pDrv->setTransform(video::ETS_WORLD,core::CMatrix4<f32>());
  m_pDrv->draw3DBox(m_cBoxNode,video::SColor(0xFF,0xFF,0x88,0x88));
  m_pDrv->draw3DBox(m_cBoxSeg ,video::SColor(0x88,0xFF,0xFF,0x88));

  if (m_iState==1) {
    scene::ISceneNode *pSelect=m_pSmgr->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(core::position2di(iMouseX,iMouseY));

    if (pSelect!=NULL) {
      if (m_pHoverNode!=NULL) m_pHoverNode->setMaterialFlag(video::EMF_LIGHTING,false);
      m_pHoverNode=pSelect;
      m_pHoverNode->setMaterialFlag(video::EMF_LIGHTING,true);
    }

    if (!bLeftBtn && m_bSelect && m_pHoverNode) {
      setSelectedNode(m_pHoverNode);
      toggleState();
    }

    m_bSelect=bLeftBtn;
  }
}

void CAlignDialog::segmentSelect() {
  if (m_pSegment==NULL) return;

  m_vSeg=m_pSegment->getPoint(m_iSegNode);
  core::vector3df vSize=core::vector3df(5,5,5);

  m_cBoxSeg.MinEdge=m_vSeg-vSize;
  m_cBoxSeg.MaxEdge=m_vSeg+vSize;
}

void CAlignDialog::setSelectedSegment(CSegment *p) {
  m_pSegment=p;

  if (m_pSegment) {
    wchar_t s[0xFF];
    swprintf(s,0xFF,L"Selected Segment: \"%s\"",core::stringw(m_pSegment->getName()).c_str());
    m_lblSegment->setText(s);
    m_iSegNode=0;
    m_pSegVtx->setValue(0);
    segmentSelect();
  }
}

void CAlignDialog::setVisible(bool b) {
  CDialogWindow::setVisible(b);
  if (m_pSceneNode) m_pSceneNode->setMaterialFlag(video::EMF_LIGHTING,false);
}
