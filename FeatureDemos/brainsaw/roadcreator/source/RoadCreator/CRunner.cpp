  #include <CRunner.h>
  #include <CSegment.h>
  #include <CSurface.h>
  #include <CProjects.h>
  #include <IRoadPart.h>
  #include <CRoadDialog.h>
  #include <CConnection.h>
  #include <CRoadLoader.h>
  #include <CAlignDialog.h>
  #include <INotification.h>
  #include <CSurfaceWindow.h>
  #include <CMaterialManager.h>
  #include <CTextureParameterWindow.h>
  #include <CSegmentParameterWindow.h>
  #include <CConnectionParameterWindow.h>

  #include <locale.h>
  #include <irrlicht.h>

using namespace irr;

const double GRAD_PI2=3.1415926535897932384626433832795 / 180.0;

class CUnknownNodeLoader : public scene::ISceneNodeFactory {
  protected:
    IrrlichtDevice *m_pDevice;
    scene::ISceneManager *m_pSmgr;
    core::array<core::stringc> m_aNodeNames;

  public:
    CUnknownNodeLoader(IrrlichtDevice *pDevice) {
      m_pDevice=pDevice;
      m_pSmgr=pDevice->getSceneManager();
    }

    virtual scene::ISceneNode *addSceneNode (const c8 *typeName, scene::ISceneNode *parent=0) {
      static bool s_bCreating=false;

      if (!s_bCreating) {
        s_bCreating=true;
        scene::ISceneNode *pRet=m_pSmgr->addSceneNode(typeName,parent!=NULL?parent:m_pSmgr->getRootSceneNode());
        s_bCreating=false;

        if (pRet==NULL) {
          pRet=m_pSmgr->addEmptySceneNode(parent!=NULL?parent:m_pSmgr->getRootSceneNode(),-1);
          pRet->setName(typeName);

          bool bAdd=true;
          for (u32 i=0; i<m_aNodeNames.size(); i++) {
            if (!strcmp(m_aNodeNames[i].c_str(),typeName)) bAdd=false;
          }

          if (bAdd) {
            printf("**** new type: \"%s\"\n",typeName);
            m_aNodeNames.push_back(typeName);
          }
          return pRet;
        }
      }

      return NULL;
    }

    virtual scene::ISceneNode *addSceneNode (scene::ESCENE_NODE_TYPE type, scene::ISceneNode *parent=0) {
      return addSceneNode("xxxxyyyyzzzz",parent);
    }

    virtual u32 getCreatableSceneNodeTypeCount () const {
      return m_aNodeNames.size();
    }

    virtual scene::ESCENE_NODE_TYPE getCreateableSceneNodeType (u32 idx) const {
      return scene::ESNT_UNKNOWN;
    }

    virtual const c8 *getCreateableSceneNodeTypeName (scene::ESCENE_NODE_TYPE type) const {
      static c8 s_sTemp[0xFF];
      sprintf(s_sTemp,"Node_%i",type);
      return s_sTemp;
    }

    virtual const c8 *getCreateableSceneNodeTypeName (u32 idx) const {
      return m_aNodeNames[idx].c_str();
    }
};

using namespace irr;

/**
 * "Load Settings" method.
 * @see m_sSettings
 * @see CDialogWindow::load
 * @see CProjects::loadProject
 */
void CRunner::load() {
  io::IReadFile *pFile=m_pDevice->getFileSystem()->createAndOpenFile(m_sSettings);

  printf("loading settings: \"%s\"\n",m_sSettings.c_str());

  if (pFile) {
    io::IXMLReader *pReader=m_pDevice->getFileSystem()->createXMLReader(pFile);
    if (pReader) {
      u32 iState=0,iSegParamNo=0,iConParamNo=0,iSurfaceParamNo=0;
      core::stringw sWinName=L"";

      while (pReader->read()) {
        if (pReader->getNodeType()==io::EXN_ELEMENT) {
          if (core::stringw(pReader->getNodeName())==L"RoadCreatorSettings") {
            //the root element has one attribute indicating the currently selected project
            m_pProjects->setSelected(pReader->getAttributeValue(L"selected"));
          }

          //the "Project" node holds the necessary information for a project
          if (core::stringw(pReader->getNodeName())==L"Project") {
            iState=1;
          }

          //the "Window" node holds the position information of a dialog window
          if (core::stringw(pReader->getNodeName())==L"Window") {
            //One attribute "name" to identify which window the data is from
            sWinName=core::stringw(pReader->getAttributeValue(L"name"));
            if (sWinName!=L"") iState=2;
          }

          //the camera information (position, target)
          if (core::stringw(pReader->getNodeName())==L"camera") {
            iState=3;
          }

          if (core::stringw(pReader->getNodeName())==L"defaultParamSeg") {
            iState=4;
          }

          if (core::stringw(pReader->getNodeName())==L"defaultParamCon") {
            iState=5;
          }
          
          if (core::stringw(pReader->getNodeName())==L"defaultParamSurface") {
            iState=6;
          }

          if (core::stringw(pReader->getNodeName())==L"attributes") {
            //Load a project
            if (iState==1) {
              io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
              pAttr->read(pReader,true);
              m_pProjects->loadProject(pAttr);
              pAttr->drop();
            }

            //Load position information of a window
            if (iState==2) {
              core::list<CDialogWindow *>::Iterator it;

              for (it=m_lDialogs.begin(); it!=m_lDialogs.end(); it++) {
                CDialogWindow *p=*it;

                if (core::stringw(p->getSaveName())==sWinName) {
                  io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
                  pAttr->read(pReader,true);
                  p->load(pAttr);
                  pAttr->drop();
                }
              }
            }

            //Load camera information
            if (iState==3) {
              io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
              pAttr->read(pReader,true);
              m_pCam->setPosition(pAttr->getAttributeAsVector3d("position"));
              m_pCam->setTarget(pAttr->getAttributeAsVector3d("target"));
              m_fAngleV=pAttr->getAttributeAsFloat("angleV");
              m_fAngleH=pAttr->getAttributeAsFloat("angleH");
              pAttr->drop();
            }

            if (iState==4) {
              if (iSegParamNo<_SEGMENT_NUMBER_OF_BUFFERS) {
                io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
                pAttr->read(pReader,true);
                m_aDefaultSeg[iSegParamNo].load(pAttr);
                pAttr->drop();
                iSegParamNo++;
              }
            }

            if (iState==5) {
              if (iConParamNo<_CONNECTION_NUMBER_OF_BUFFERS) {
                io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
                pAttr->read(pReader,true);
                m_aDefaultCon[iConParamNo].load(pAttr);
                pAttr->drop();
                iConParamNo++;
              }
            }
            
            if (iState==6) {
              if (iSurfaceParamNo<_SURFACE_NUMBER_OF_BUFFERS) {
                io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
                pAttr->read(pReader,true);
                m_aDefaultSrf[iSurfaceParamNo].load(pAttr);
                pAttr->drop();
                iSurfaceParamNo++;
              }
            }
          }
        }
      }
      m_pProjects->updateProjectList();
      m_pProjects->updateUI();
      m_pLoader->setSurfaceDefaultParameters(m_aDefaultSrf);
    }
  }
}

/**
 * Save the settings
 * @see m_sSettings
 * @see CDialogWindow::save
 * @see CProjects::saveProject
 */
void CRunner::save() {
  io::IWriteFile *pFile=m_pDevice->getFileSystem()->createAndWriteFile(m_sSettings,false);
  if (pFile) {
    io::IXMLWriter *pWriter=m_pDevice->getFileSystem()->createXMLWriter(pFile);
    if (pWriter) {
      //Firat part: save the project list
      pWriter->writeXMLHeader();
      pWriter->writeElement(L"RoadCreatorSettings",false,L"selected",core::stringw(m_pProjects->getSelected()).c_str());
      pWriter->writeLineBreak();

      for (u32 i=0; i<m_pProjects->getProjectCount(); i++) {
        pWriter->writeElement(L"Project",false);
        pWriter->writeLineBreak();
        io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
        m_pProjects->saveProject(pAttr,i);
        pAttr->write(pWriter);
        pAttr->drop();
        pWriter->writeClosingTag(L"Project");
        pWriter->writeLineBreak();
      }

      //Second step: save the positions of all dialogs
      core::list<CDialogWindow *>::Iterator it;
      for (it=m_lDialogs.begin(); it!=m_lDialogs.end(); it++) {
        CDialogWindow *p=*it;
        pWriter->writeElement(L"Window",false,L"name",core::stringw(p->getSaveName()).c_str());
        pWriter->writeLineBreak();
        io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
        p->save(pAttr);
        pAttr->write(pWriter);
        pAttr->drop();
        pWriter->writeClosingTag(L"Window");
        pWriter->writeLineBreak();
      }

      //Third step: save the camera information
      pWriter->writeElement(L"camera",false);
      pWriter->writeLineBreak();
      io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
      pAttr->addVector3d("position",m_pCam->getPosition());
      pAttr->addVector3d("target",m_pCam->getTarget());
      pAttr->addFloat("angleV",m_fAngleV);
      pAttr->addFloat("angleH",m_fAngleH);
      pAttr->write(pWriter);
      pAttr->drop();
      pWriter->writeClosingTag(L"camera");
      pWriter->writeLineBreak();

      for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
        pWriter->writeElement(L"defaultParamSeg",false);
        pWriter->writeLineBreak();

        io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
        m_aDefaultSeg[i].save(pAttr);
        pAttr->write(pWriter);
        pAttr->drop();

        pWriter->writeClosingTag(L"defaultParamSeg");
        pWriter->writeLineBreak();
      }

      for (u32 i=0; i<_CONNECTION_NUMBER_OF_BUFFERS; i++) {
        pWriter->writeElement(L"defaultParamCon",false);
        pWriter->writeLineBreak();

        io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
        m_aDefaultCon[i].save(pAttr);
        pAttr->write(pWriter);
        pAttr->drop();

        pWriter->writeClosingTag(L"defaultParamCon");
        pWriter->writeLineBreak();
      }
      
      for (u32 i=0; i<_SURFACE_NUMBER_OF_BUFFERS; i++) {
        pWriter->writeElement(L"defaultParamSurface");
        pWriter->writeLineBreak();
        
        io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
        m_aDefaultSrf[i].save(pAttr);
        pAttr->write(pWriter);
        pAttr->drop();
        
        pWriter->writeClosingTag(L"defaultParamSurface");
        pWriter->writeLineBreak();
      }

      pWriter->writeClosingTag(L"RoadCreatorSettings");
      pWriter->writeLineBreak();
    }

    pWriter->drop();
    pFile->drop();
  }
}

/**
 * This method is used to make sure a segment name is unique within a road definition
 * @param s the input name
 * @return unique version of the name (maybe with an appended "_<number>" string
 */
core::stringc &CRunner::unifySegmentName(core::stringc &s) {
  core::stringc sUnique=s;
  bool bPassed=false;
  u32 iCnt=0;

  do {
    core::list<CSegment *> lSegments=m_pLoader->getSegments();
    core::list<CSegment *>::Iterator it;
    bPassed=true;
    for (it=lSegments.begin(); it!=lSegments.end() && bPassed; it++) {
      CSegment *p=*it;
      if (p->getName()==sUnique) {
        bPassed=false;
        sUnique=s; sUnique+="_"; sUnique+=iCnt;
        iCnt++;
      }
    }
  }
  while (!bPassed);

  s=sUnique;
  return s;
}

/** Update the list of segments */
void CRunner::updateSegmentList() {
  core::list<CSegment *> lSegments=m_pLoader->getSegments();
  core::list<CSegment *>::Iterator it;
  m_pLstSeg->clear();
  for (it=lSegments.begin(); it!=lSegments.end(); it++)
    m_pLstSeg->addItem(core::stringw((*it)->getName()).c_str());
}

/** Update the list of connections */
void CRunner::updateConnectionList() {
  m_pLstCon->clear();

  core::list<CConnection *> lConnections=m_pLoader->getConnections();
  core::list<CConnection *>::Iterator it;

  u32 i=0;
  for (it=lConnections.begin(); it!=lConnections.end(); it++) {
    core::stringw sName="connection_";
    sName+=i;

    m_pLstCon->addItem(sName.c_str());
    i++;
  }
}

/**
 * This method loads a road from a file
 * @param sName the file to load
 * @see CSegment::load
 * @see CConnection::load
 * @see CTextureParameters::load
 */
void CRunner::loadRoad(const core::stringc sName) {
  m_pLoader->loadRoad(sName);
  updateSegmentList();
  updateConnectionList();
  m_pShrink->setChecked(m_pLoader->didShrinkNode());
}

/**
 * Remove a road from memory
 */
void CRunner::destroyRoad() {
  m_pLoader->destroyRoad();

  updateSegmentList();
  updateConnectionList();

  //reset the selected objects
  m_pSelectedCon=NULL;
  m_pSelectedSeg=NULL;

  //hide all dialogs
  m_pWinCon ->setVisible(false);
  m_pWinSeg ->setVisible(false);
  m_pWinTex ->setVisible(false);
  m_pSurface->setVisible(false);

  m_pArrowBase->setVisible(false);
}

void CRunner::updateSingleBufferBox(IRoadPart *p) {
  m_pSingleBuf->clear();
  if (!p) return;

  wchar_t s[0xFF];
  swprintf(s,0xFF,L"Show all buffers");
  m_pSingleBuf->addItem(s);

  for (s32 i=0; i<p->getNumberOfMeshBuffers(); i++) {
    swprintf(s,0xFF,L"Show buffer %i",i);
    m_pSingleBuf->addItem(s);
  }
}

/**
 * The constructor
 * @param eDriver the driver type
 * @param cDim size of the window
 * @param bFullscreen fullscreen or not
 * @param bStencilBuffer use stencil buffer shadows?
 * @param bVSync use synchronisation with the monitor
 */
CRunner::CRunner(video::E_DRIVER_TYPE eDriver,core::dimension2d<u32> cDim, u32 iBits, bool  bFullscrenn, bool bStencilBuffer, bool bVSync) {
  //Create the Irrlicht device
  m_pDevice=createDevice(eDriver,cDim,iBits,bFullscrenn,bStencilBuffer,bVSync,this);
  m_pDevice->setWindowCaption(L"BulletByte RoadCreator");
  m_pFileOpen=NULL;

  if (m_pDevice) {
    //make it resizeable
    m_pDevice->setResizable(true);

    //get the working directory
    m_sBaseDir=m_pDevice->getFileSystem()->getWorkingDirectory();
    m_sSettings=m_sBaseDir+"/settings.xml";

    m_bInitOk=true;
    m_bLeftBtn=false;
    m_pSelected=NULL;

    m_pDriver=m_pDevice->getVideoDriver();
    m_pSmgr=m_pDevice->getSceneManager();
    m_pGuienv=m_pDevice->getGUIEnvironment();

    //add a camera and the arrow nodes
    m_pCam=m_pSmgr->addCameraSceneNode();

    m_pArrowBase=m_pSmgr->addEmptySceneNode();
    m_pArrowBase->setVisible(false);

    m_pLoader=new CRoadLoader(m_pDevice);
    m_pNode=NULL;

    scene::IAnimatedMesh *pArrowMesh=m_pSmgr->addArrowMesh("data/room.3ds",
                                                           video::SColor(0xFF,0xFF,0,0),
                                                           video::SColor(0xFF,0xFF,0,0),
                                                           4,8,24,18,0.5,1.5);

    scene::IAnimatedMeshSceneNode *pArrow=m_pSmgr->addAnimatedMeshSceneNode(pArrowMesh,m_pArrowBase,1001);
    pArrow->setMaterialFlag(video::EMF_LIGHTING,false);

    pArrowMesh=m_pSmgr->addArrowMesh("data/room.3ds",
                                      video::SColor(0xFF,0,0xFF,0),
                                      video::SColor(0xFF,0,0xFF,0),
                                      4,8,24,18,0.5,1.5);


    pArrow=m_pSmgr->addAnimatedMeshSceneNode(pArrowMesh,m_pArrowBase,1002);
    pArrow->setMaterialFlag(video::EMF_LIGHTING,false);
    pArrow->setRotation(core::vector3df(90.0f,0.0f,0.0f));

    pArrowMesh=m_pSmgr->addArrowMesh("data/room.3ds",
                                     video::SColor(0xFF,0,0xFF,0),
                                     video::SColor(0xFF,0,0xFF,0),
                                     4,8,24,18,0.5,1.5);


    pArrow=m_pSmgr->addAnimatedMeshSceneNode(pArrowMesh,m_pArrowBase,1003);
    pArrow->setMaterialFlag(video::EMF_LIGHTING,false);
    pArrow->setRotation(core::vector3df(0.0f,0.0f,90.0f));

    m_vCamLookAt=core::vector3df(0.0f,0.0f,1.0f);

    //Add the GUI elements
    m_pGuiRoot=m_pGuienv->addTab(core::rect<s32>(0,0,1023,767));

    core::position2di cPos=core::position2di(10,10),cOffset=core::position2di(0,25),cCbPos;
    core::dimension2di cDim=core::dimension2di(120,20);

    //Add the button menu (left size of the window)
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnProjects     ,L"Projects"        ); cPos+=  cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnResetCamera  ,L"Reset Camera"    ); cPos+=2*cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnNew          ,L"New"             ); cPos+=  cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnLoad         ,L"Load"            ); cPos+=  cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnSave         ,L"Save"            ); cPos+=  cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnSaveAs       ,L"Save As"         ); cPos+=2*cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnAddSegment   ,L"Add Segment"     ); cPos+=  cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnDelSegment   ,L"Del Segment"     ); cPos+=  cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnCloneSegment ,L"Clone Segment"   ); cPos+=2*cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnAddConnection,L"Add Connection"  ); cPos+=  cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnDelConnection,L"Del Connection"  ); cPos+=2*cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnCreateNode   ,L"Create Node"     ); cPos+=  cOffset; cCbPos=cPos; cPos+=4*cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnSurface      ,L"Surface"         ); cPos+=2*cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnLoadScene    ,L"Load Scene"      ); cPos+=  cOffset;
    m_pGuienv->addButton(core::rect<s32>(cPos,cDim),m_pGuiRoot,eBtnAlignSegment ,L"Align Segment"   );

    m_pShrink=m_pGuienv->addCheckBox(false,core::rect<s32>(cCbPos,cDim),m_pGuiRoot,-1,L"Shrink Node"  ); cCbPos+=cOffset;
    m_pOffset=m_pGuienv->addEditBox(L"",core::rect<s32>(cCbPos,cDim),true,m_pGuiRoot); cCbPos+=cOffset;
    m_pOffset->setVisible(false);

    m_pSingle=m_pGuienv->addCheckBox(false,core::rect<s32>(cCbPos,cDim),m_pGuiRoot,-1,L"Show Single Part"); cCbPos+=cOffset;

    m_pSingleBuf=m_pGuienv->addComboBox(core::rect<s32>(cCbPos,cDim),m_pGuiRoot); cCbPos+=cOffset;
    m_pSingleBuf->setVisible(false);

    //Add the tabs with the segment and connection list (right side of the window)
    m_pSegTab=m_pGuienv->addTab(core::rect<s32>(828, 15,1008,376),m_pGuiRoot);
    m_pJntTab=m_pGuienv->addTab(core::rect<s32>(828,390,1008,752),m_pGuiRoot);

    m_pSegTab->setDrawBackground(true); m_pSegTab->setBackgroundColor(video::SColor(128,192,192,192));
    m_pJntTab->setDrawBackground(true); m_pJntTab->setBackgroundColor(video::SColor(128,192,192,192));

    m_pGuienv->addStaticText(L"Segments"   ,core::rect<s32>(0,0,180,15),true,true,m_pSegTab)->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);
    m_pGuienv->addStaticText(L"Connections",core::rect<s32>(0,0,180,15),true,true,m_pJntTab)->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);

    m_pLstSeg=m_pGuienv->addListBox(core::rect<s32>(0,18,180,361),m_pSegTab,-1,true);
    m_pLstCon=m_pGuienv->addListBox(core::rect<s32>(0,18,180,361),m_pJntTab,-1,true);

    //Create and initialize the dialogs
    m_pMaterials=new CMaterialManager(m_pDevice); m_pMaterials->setVisible(false);

    m_pWinTex =new CTextureParameterWindow   (m_pGuienv,m_pMaterials);
    m_pWinSeg =new CSegmentParameterWindow   (m_pGuienv,m_pWinTex);
    m_pWinCon =new CConnectionParameterWindow(m_pGuienv,m_pWinTex);
    m_pSurface=new CSurfaceWindow            (m_pGuienv,m_pWinTex);

    m_pWinTex->addNotify(m_pWinCon);
    m_pWinTex->addNotify(m_pWinSeg);

    m_pWinSeg ->setVisible(false); this->addEventReceiver(m_pWinSeg );
    m_pWinCon ->setVisible(false); this->addEventReceiver(m_pWinCon );
    m_pWinTex ->setVisible(false); this->addEventReceiver(m_pWinTex );
    m_pSurface->setVisible(false); this->addEventReceiver(m_pSurface);

    m_pProjects=new CProjects(m_pDevice);

    m_pRoad=new CRoadDialog(m_pDevice);
    m_pRoad->setVisible(false);

    //Add all dialogs to a list
    m_lDialogs.push_back(m_pWinCon);
    m_lDialogs.push_back(m_pWinSeg);
    m_lDialogs.push_back(m_pWinTex);
    m_lDialogs.push_back(m_pRoad);
    m_lDialogs.push_back(m_pMaterials);
    m_lDialogs.push_back(m_pProjects);
    m_lDialogs.push_back(m_pSurface);

    //Load the positions of the dialogs (and the projects (and the camera position))
    load();

    //Add the necessary event receivers
    this->addEventReceiver(m_pProjects );
    this->addEventReceiver(m_pMaterials);
    this->addEventReceiver(m_pRoad);

    //init the remaining members
    m_iMouseX=-100;
    m_iMouseY=-100;


    m_pSelectedSeg=NULL;
    m_pSelectedCon=NULL;

    m_pTheSurface=m_pLoader->getSurface();
    m_pSurface->setSurface(m_pTheSurface);

    m_bHover=false;
    m_iState=0;

    m_fAngleH=0.0f;
    m_fAngleV=0.0f;

    m_pDlgAlign=new CAlignDialog(m_pDevice);
    m_pDlgAlign->setVisible(false);
    addEventReceiver(m_pDlgAlign);
    m_bSegment=true;
  }
  else m_bInitOk=false;

  CUnknownNodeLoader *pUnknown=new CUnknownNodeLoader(m_pDevice);
  m_pSmgr->registerSceneNodeFactory(pUnknown);
}

/**
 * The destructor
 */
CRunner::~CRunner() {
  delete m_pMaterials;
  delete m_pWinTex;
  delete m_pWinSeg;
  delete m_pWinCon;
  delete m_pProjects;
  delete m_pRoad;
  delete m_pSurface;
}

/**
 * Update the tabs on the right side after the window was resized
 * @param cNewSize the new size of the window
 */
void CRunner::updateTabs(core::dimension2du cNewSize) {
  m_pGuiRoot->setRelativePosition(core::rect<s32>(0,0,cNewSize.Width,cNewSize.Height));

  s32 x1=cNewSize.Width-196,y1=15,
      x2=cNewSize.Width-16,y2=cNewSize.Height/2-8;

  m_pSegTab->setRelativePosition(core::rect<s32>(x1,y1,x2,y2));
  m_pLstSeg->setRelativePosition(core::rect<s32>(0,18,180,cNewSize.Height/2-25));

  x1=cNewSize.Width-196; y1=cNewSize.Height/2+8;
  x2=cNewSize.Width-16; y2=cNewSize.Height-15;

  m_pJntTab->setRelativePosition(core::rect<s32>(x1,y1,x2,y2));
  m_pLstCon->setRelativePosition(core::rect<s32>(0,18,180,cNewSize.Height/2-25));
}

/**
 * The main method. This is called on startup and
 * the program ends when this method is finished.
 */
void CRunner::run() {
  //Initialization failed --> return
  if (!m_bInitOk) return;

  scene::ISceneNode *pLastSelected=NULL;
  core::dimension2du cSize=m_pDriver->getCurrentRenderTargetSize();

  while(m_pDevice->run()) {
    m_pDriver->beginScene(true,true,video::SColor(0,200,200,200));

    m_pSmgr->drawAll();

    if (m_pDlgAlign->isVisible()) {
      m_pDlgAlign->draw(m_bLeftBtn,m_iMouseX,m_iMouseY);
    }

    if (m_iState==0) {
      if (m_pSingle->isChecked()) {
        if (m_bSegment && m_pSelectedSeg!=NULL) m_pSelectedSeg->render();
      }
      else {
        //Draw all segments
        core::list<CSegment *> lSegments=m_pLoader->getSegments();
        core::list<CSegment *>::Iterator sit;
        for (sit=lSegments.begin(); sit!=lSegments.end(); sit++) (*sit)->render();
      }

      if (m_pSingle->isChecked()) {
        if (!m_bSegment && m_pSelectedCon!=NULL) m_pSelectedCon->render();
      }
      else {
        //Draw all connections
        core::list<CConnection *> lConnections=m_pLoader->getConnections();
        core::list<CConnection *>::Iterator cit;
        for (cit=lConnections.begin(); cit!=lConnections.end(); cit++) (*cit)->render();
      }

      m_pTheSurface->render();

      scene::ISceneNode *pSelect=m_pSmgr->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(core::position2di(m_iMouseX,m_iMouseY));      //Check selection of an arrow

      if (!m_bLeftBtn) {
        if (pSelect==NULL)
          m_pSelected=NULL;
        else
          if (m_pArrowBase->isVisible()) {
            if (pSelect->getID()<1001 || pSelect->getID()>1003) m_pSelected=NULL; else m_pSelected=pSelect;

            if (m_pSelected!=NULL) {
              if (pLastSelected) pLastSelected->setMaterialFlag(video::EMF_LIGHTING,false);
              m_pSelected->setMaterialFlag(video::EMF_LIGHTING,true);
              pLastSelected=m_pSelected;
            }
          }
      }
    }

    m_pGuienv->drawAll();
    m_pDriver->endScene();
    m_pDevice->sleep(1);

    //Check for window resize
    core::dimension2du cNewSize=m_pDriver->getCurrentRenderTargetSize();
    if (cNewSize!=cSize) {
      cSize=cNewSize;
      updateTabs(cSize);
    }
  }

  save();
  m_pDevice->drop();
}

u32 countNodes(scene::ISceneNode *pNode) {
  u32 iRet=1;
  core::list<scene::ISceneNode *> lKids=pNode->getChildren();
  core::list<scene::ISceneNode *>::Iterator it;

  for (it=lKids.begin(); it!=lKids.end(); it++) {
    iRet+=countNodes(*it);
  }

  return iRet;
}

/**
 * The Irrlicht event receiver callback
 * @param event the event to handle
 */
bool CRunner::OnEvent(const SEvent &event) {
  bool bRet=false;

  //foreward the event to all registered event listeners
  core::list<IEventReceiver *>::Iterator it;
  for (it=m_lReceivers.begin(); it!=m_lReceivers.end() && !bRet; it++) bRet=(*it)->OnEvent(event);

  //we only care about GUI events
  if (event.EventType==EET_GUI_EVENT) {
    if (event.GUIEvent.EventType==gui::EGET_ELEMENT_HOVERED) m_bHover=event.GUIEvent.Caller->getType()!=gui::EGUIET_TAB;
    if (event.GUIEvent.EventType==gui::EGET_ELEMENT_LEFT   ) m_bHover=false;

    if (event.GUIEvent.EventType==gui::EGET_ELEMENT_CLOSED) {
      event.GUIEvent.Caller->setVisible(false);
      bRet=true;
    }

    if (event.GUIEvent.EventType==gui::EGET_CHECKBOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pShrink) m_pLoader->shrinkNode(m_pShrink->isChecked());
      if (event.GUIEvent.Caller==m_pSingle) {
        if (m_pSelectedSeg==NULL && m_pSelectedCon==NULL) {
          bRet=true;
          m_pSingle->setChecked(false);
        }
        else {
          if (m_pSelectedSeg) m_pSelectedSeg->renderMeshBuffer(-1);
          if (m_pSelectedCon) m_pSelectedCon->renderMeshBuffer(-1);

          if ( m_bSegment && m_pSelectedSeg!=NULL) updateSingleBufferBox(m_pSelectedSeg);
          if (!m_bSegment && m_pSelectedCon!=NULL) updateSingleBufferBox(m_pSelectedCon);
          m_pSingleBuf->setVisible(m_pSingle->isChecked());
        }
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_COMBO_BOX_CHANGED) {
      if (event.GUIEvent.Caller==m_pSingleBuf) {
        s32 iIdx=m_pSingleBuf->getSelected();
        iIdx--;
        if ( m_bSegment && m_pSelectedSeg) m_pSelectedSeg->renderMeshBuffer(iIdx);
        if (!m_bSegment && m_pSelectedCon) m_pSelectedCon->renderMeshBuffer(iIdx);
      }
    }

    //We mainly handle button events this time
    if (event.GUIEvent.EventType==gui::EGET_BUTTON_CLICKED) {
      switch (event.GUIEvent.Caller->getID()) {
        case eBtnResetCamera:
          if (m_pSelectedSeg) {
            core::vector3df vPos=m_pSelectedSeg->getPosition()+10.0f*m_pSelectedSeg->getWidth()+core::vector3df(0.0f,10.0f,0.0f);
            m_pSmgr->getActiveCamera()->setPosition(vPos);
            m_pSmgr->getActiveCamera()->setTarget(m_pSelectedSeg->getPosition());
          }
          else {
            m_pSmgr->getActiveCamera()->setPosition(core::vector3df(0.0f,0.0f,0.0f));
            m_pSmgr->getActiveCamera()->setTarget(core::vector3df(0.0f,0.0f,1.0f));
          }
          m_fAngleH=225.0f;
          m_fAngleV=-33.0f;
          break;

        //"New" button clicked
        case eBtnNew:
          destroyRoad();
          m_pSmgr->addToDeletionQueue(m_pNode);
          m_iState=0;
          break;

        //"Load" button clicked
        case eBtnLoad: {
            core::stringc s=m_pProjects->getSelectedSavePath();
            printf("load path: \"%s\"\n",s.c_str());
            m_pRoad->setFolder(s);
            m_pRoad->showDialog(true,core::stringc("Load Road"));
          }
          break;

        //"Save" button clicked
        case eBtnSave:
          m_pLoader->saveRoad();
          break;

        //"Save as" button clicked
        case eBtnSaveAs:
          m_pRoad->setFolder(m_pProjects->getSelectedSavePath());
          m_pRoad->showDialog(false,core::stringc("Save Road As"));
          break;

        //"Add segment" button clicked
        case eBtnAddSegment: {
            core::stringc sName=core::stringc("Segment");
            CSegment *p=new CSegment(unifySegmentName(sName),m_pCam->getPosition(),m_pDevice,m_aDefaultSeg);
            p->setDirection(m_pCam->getTarget()-m_pCam->getPosition());
            m_pLoader->getSegments().push_back(p);
            updateSegmentList();
          }
          break;

        //"Delete segment" button clicked
        case eBtnDelSegment:
          if (m_pLoader->deleteSegment(m_pSelectedSeg)) {
            m_pSelectedSeg=NULL;
            m_pWinSeg->setVisible(false);
            updateSegmentList();
            m_pArrowBase->setVisible(false);
            return true;
          }
          break;

        case eBtnCloneSegment:
          if (m_pSelectedSeg!=NULL) {
            CSegment *p=m_pSelectedSeg->clone();
            core::stringc sName="Segment";
            p->setName(unifySegmentName(sName));
            
            for (u32 i=0; i<_SEGMENT_NUMBER_OF_BUFFERS; i++) {
              CTextureParameters *t=m_pSelectedSeg->getTextureParameters(i),*tNew=p->getTextureParameters(i);
              io::IAttributes *pTexAttr=m_pDevice->getFileSystem()->createEmptyAttributes();
              t->save(pTexAttr);
              tNew->load(pTexAttr);
              pTexAttr->drop();
            }
            
            m_pLoader->getSegments().push_back(p);
            updateSegmentList();
          }
          break;
          
        //"Add connection" button clicked
        case eBtnAddConnection: {
            CConnection *p=new CConnection(m_pDevice,m_aDefaultCon);
            m_pLoader->getConnections().push_back(p);
            updateConnectionList();
          }
          break;
        
        //"Delete connection" button clicked
        case eBtnDelConnection:
          if (m_pLoader->deleteConnection(m_pSelectedCon)) {
            m_pSelectedCon=NULL;
            m_pWinCon->setSelectedConnection(NULL);
            m_pWinCon->setVisible(false);
            m_pWinTex->setVisible(false);
            m_pSurface->setVisible(false);
            updateConnectionList();
            return true;
          }
          break;

        case eBtnCreateNode:
          switch (m_iState) {
            case 0: {
                scene::IMesh *pMesh=m_pLoader->createMesh();
                m_pNode=m_pSmgr->addMeshSceneNode(pMesh,m_pSmgr->getRootSceneNode());
                m_pNode->setIsDebugObject(true);
                m_pNode->setAutomaticCulling(scene::EAC_OFF);
                pMesh->drop();
                m_iState=1;
                event.GUIEvent.Caller->setText(L"Remove Node");

                if (m_pShrink->isChecked()) {
                  wchar_t s[0xFF];
                  core::vector3df vOff=m_pLoader->getOffset();
                  swprintf(s,0xFF,L"%.2f, %.2f, %.2f",vOff.X,vOff.Y,vOff.Z);
                  m_pOffset->setText(s);
                  m_pOffset->setVisible(true);
                }
              }
              break;

            case 1: {
                m_pSmgr->addToDeletionQueue(m_pNode);
                m_pNode=NULL;
                m_iState=0;
                event.GUIEvent.Caller->setText(L"Create Node");
                m_pOffset->setVisible(false);
                core::list<CSegment *> lSegments=m_pLoader->getSegments();
                core::list<CSegment *>::Iterator sit;
                for (sit=lSegments.begin(); sit!=lSegments.end(); sit++) (*sit)->update();

                core::list<CConnection *> lConnections=m_pLoader->getConnections();
                core::list<CConnection *>::Iterator cit;
                for (cit=lConnections.begin(); cit!=lConnections.end(); cit++) (*cit)->update();
              }
              break;
          }
          break;

        case eBtnSurface:
          m_pWinCon->setVisible(false);
          m_pWinTex->setVisible(false);
          m_pSurface->setVisible(true);
          break;

        case eBtnLoadScene: {
            char s[0xFF],*cp;
            strcpy(s,m_pProjects->getScene());
            cp=strrchr(s,'\\');
            if (!cp) cp=strrchr(s,'/');
            if (cp) { *cp='\0'; cp++; }
            printf("File Open: \"%s\" (%s)\n",s,cp);
            m_pDevice->getFileSystem()->changeWorkingDirectoryTo(s);
            m_pFileOpen=m_pGuienv->addFileOpenDialog(L"Load Scene",true);
            m_pFileOpen->setText(core::stringw(m_pProjects->getScene()).c_str());
          }
          break;

        case eBtnAlignSegment:
          m_pDlgAlign->setVisible(true);
          m_pWinSeg->setVisible(false);
          break;
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_LISTBOX_CHANGED) {
      //Selected segment has changed
      if (event.GUIEvent.Caller==m_pLstSeg) {
        m_pSingle->setChecked(false);
        m_pSingleBuf->setVisible(false);
        if (m_pLstSeg->getSelected()!=-1) {
          core::list<CSegment *> lSegments=m_pLoader->getSegments();
          core::list<CSegment *>::Iterator it;
          core::stringc sName=core::stringc(m_pLstSeg->getListItem(m_pLstSeg->getSelected()));
          for (it=lSegments.begin(); it!=lSegments.end(); it++) {
            CSegment *p=*it;
            if (p->getName()==sName) {
              m_bSegment=true;
              if (m_pSelectedSeg) m_pSelectedSeg->renderMeshBuffer(-1);
              m_pSelectedSeg=p;
              m_pArrowBase->setPosition(m_pSelectedSeg->getPosition());
              m_pArrowBase->setVisible(true);
              m_pWinSeg->setSelectedSegment(p);
              m_pWinCon->setVisible(false);
              m_pWinTex->setVisible(false);
              m_pMaterials->setVisible(false);
              m_pSurface->setVisible(false);
              m_pWinCon->setSelectedSegment(p);

              if (m_pDlgAlign->isVisible())
                m_pDlgAlign->setSelectedSegment(m_pSelectedSeg);
              else
                m_pWinSeg->setVisible(p!=NULL);
            }
          }
        }
        else m_pArrowBase->setVisible(false);
      }

      //Selected connection has changed
      if (event.GUIEvent.Caller==m_pLstCon) {
        m_pSingle->setChecked(false);
        m_pSingleBuf->setVisible(false);
        if (m_pSelectedCon) m_pSelectedCon->renderMeshBuffer(-1);
        m_pSelectedCon=NULL;
        s32 i=(s32)m_pLstCon->getSelected();
        core::list<CConnection *> lConnections=m_pLoader->getConnections();
        core::list<CConnection *>::Iterator it;
        for (it=lConnections.begin(); it!=lConnections.end() && i>=0; it++) {
          i--;
          if (i<0) {
            m_pWinCon->setSelectedConnection(*it);
            m_pWinCon->setVisible(true);
            m_pWinSeg->setVisible(false);
            m_pWinTex->setVisible(false);
            m_pMaterials->setVisible(false);
            m_pSurface->setVisible(false);
            m_pSelectedCon=*it;
            m_bSegment=false;
          }
        }
      }
    }

    if (event.GUIEvent.EventType==gui::EGET_FILE_SELECTED && m_pFileOpen!=NULL) {
      printf("Selected file: \"%s\"\nSelected folder: \"%s\"\n",core::stringc(m_pFileOpen->getFileName()).c_str(),core::stringc(m_pFileOpen->getDirectoryName()).c_str());
      m_pDevice->getFileSystem()->changeWorkingDirectoryTo(m_pProjects->getSelectedProjectRoot());
      m_pSmgr->loadScene(io::path(m_pFileOpen->getFileName()));
      m_pProjects->setScene(core::stringc(m_pFileOpen->getFileName()).c_str());
      m_pFileOpen->remove();
      m_pFileOpen=NULL;
      setlocale(LC_ALL,"C");
    }

    if (event.GUIEvent.EventType==gui::EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
      m_pDevice->getFileSystem()->changeWorkingDirectoryTo(m_pProjects->getSelectedProjectRoot());
      m_pFileOpen->remove();
      m_pFileOpen=NULL;
      setlocale(LC_ALL,"C");
    }

    if (m_pArrowBase->isVisible() && m_pSelectedSeg!=NULL) m_pArrowBase->setPosition(m_pSelectedSeg->getPosition());
  }

  //Handle mouse input events to update the camera (or the position of a segment when the arrow is dragged)
  if (event.EventType==EET_MOUSE_INPUT_EVENT) {
    if (m_iMouseX!=-100 && m_iMouseY!=-100) {
      if (event.MouseInput.isLeftPressed()) {
        bool bShift=event.MouseInput.Shift;
        m_bLeftBtn=true;
        if (m_pSelected!=NULL && (m_pSelected->getID()==1001 || m_pSelected->getID()==1002 || m_pSelected->getID()==1003)) {
          core::vector3df vOffsetX=0.5f*core::vector3df(m_iMouseX-event.MouseInput.X,0.0f,0.0f),
                          vOffsetY=0.5f*core::vector3df(0.0f,m_iMouseY-event.MouseInput.Y,0.0f),
                          vOffsetZ=0.5f*core::vector3df(0.0f,0.0f,m_iMouseX-event.MouseInput.X);

          switch (m_pSelected->getID()) {
            case 1001:
              if (bShift) {
                if (m_pSelectedSeg!=NULL) {
                  core::vector3df vOffset=m_pSelectedSeg->getNormal()*(m_iMouseY-event.MouseInput.Y);
                  m_pSelectedSeg->setPosition(m_pSelectedSeg->getPosition()+vOffset);
                  m_pArrowBase->setPosition(m_pArrowBase->getPosition()+vOffset);
                }
              }
              else {
                if (m_pSelectedSeg!=NULL) m_pSelectedSeg->setPosition(m_pSelectedSeg->getPosition()+vOffsetY);
                m_pArrowBase->setPosition(m_pArrowBase->getPosition()+vOffsetY);
              }
              
              break;

            case 1002:
              if (bShift) {
                if (m_pSelectedSeg!=NULL) {
                  core::vector3df vOffset=m_pSelectedSeg->getNormal().crossProduct(m_pSelectedSeg->getDirection())*(m_iMouseX-event.MouseInput.X);
                  m_pSelectedSeg->setPosition(m_pSelectedSeg->getPosition()+vOffset);
                  m_pArrowBase->setPosition(m_pArrowBase->getPosition()+vOffset);
                }
              }
              else {
                if (m_pSelectedSeg!=NULL) m_pSelectedSeg->setPosition(m_pSelectedSeg->getPosition()-vOffsetZ);
                m_pArrowBase->setPosition(m_pArrowBase->getPosition()-vOffsetZ);
              }
              
              break;

            case 1003:
              if (bShift) {
                if (m_pSelectedSeg!=NULL) {
                  core::vector3df vOffset=m_pSelectedSeg->getDirection()*(m_iMouseX-event.MouseInput.X);
                  m_pSelectedSeg->setPosition(m_pSelectedSeg->getPosition()+vOffset);
                  m_pArrowBase->setPosition(m_pArrowBase->getPosition()+vOffset);
                }
              }
              else {
                if (m_pSelectedSeg!=NULL) m_pSelectedSeg->setPosition(m_pSelectedSeg->getPosition()-vOffsetX);
                m_pArrowBase->setPosition(m_pArrowBase->getPosition()-vOffsetX);
              }
              
              break;
          }
          m_pWinSeg->updateUI();
        }
        else {
          if (!m_bHover) {
            m_fAngleH-=0.5f*((f32)m_iMouseX-event.MouseInput.X);
            m_fAngleV+=0.5f*((f32)m_iMouseY-event.MouseInput.Y);

            while (m_fAngleV> 80.0f) m_fAngleV= 80.0f;
            while (m_fAngleV<-80.0f) m_fAngleV=-80.0f;

            f32 fCamX=(irr::f32)(1.0f*sin(m_fAngleH*GRAD_PI2)*cos(m_fAngleV*GRAD_PI2)),
            fCamY=(irr::f32)(1.0f*sin(m_fAngleV*GRAD_PI2)),
            fCamZ=(irr::f32)(1.0f*cos(m_fAngleH*GRAD_PI2)*cos(m_fAngleV*GRAD_PI2));

            m_vCamLookAt=core::vector3df(fCamX,fCamY,fCamZ);
            m_pCam->setTarget(m_pCam->getPosition()+m_vCamLookAt);
          }
        }
      }
      else m_bLeftBtn=false;

      if (event.MouseInput.isRightPressed()) {
        core::vector3df vStrafe=m_vCamLookAt.crossProduct(core::vector3df(0,1,0)),
        v=m_pCam->getPosition()+((f32)(m_iMouseY-event.MouseInput.Y))*m_vCamLookAt+((f32)(m_iMouseX-event.MouseInput.X))*vStrafe;
        m_pCam->setPosition(v);
        m_pCam->setTarget(v+m_vCamLookAt);
      }
    }

    m_iMouseX=event.MouseInput.X;
    m_iMouseY=event.MouseInput.Y;
  }

  //Handle Key events
  if (event.EventType==EET_KEY_INPUT_EVENT) {
    if (!event.KeyInput.PressedDown && event.KeyInput.Key==KEY_F2) {
      if (m_pSelectedSeg!=NULL && m_pSelectedCon!=NULL) {
        if (m_pWinCon->isVisible()) {
          m_pWinCon->setVisible(false);
          m_pWinSeg->setVisible(true);
        }
        else {
          m_pWinCon->setVisible(true);
          m_pWinSeg->setVisible(false);
        }
      }
    }
  }

  //Handle User events
  if (event.EventType==EET_USER_EVENT) {
    switch (event.UserEvent.UserData1) {
      //"Load Road" dialog closed
      case eFileDialogLoad: {
          destroyRoad();
          core::stringc sRoad=m_pProjects->getSelectedSavePath()+"/"+m_pRoad->getSelectedFile();
          printf("load road: \"%s\"\n",sRoad.c_str());
          loadRoad(sRoad);
          m_pSurface->setSurface(m_pTheSurface);
        }
        break;

      //"Select Project" dialog closed
      case eProjectSelected:
        m_pDevice->getFileSystem()->changeWorkingDirectoryTo(m_pProjects->getSelectedProjectRoot());
        m_pMaterials->setTextureFolder(m_pProjects->getSelectedTexturePath());
        printf("new working directory: \"%s\"\n",m_pDevice->getFileSystem()->getWorkingDirectory().c_str());
        break;

      //"Save Road" dialog closed
      case eFileDialogSave: {
          core::stringc sRoad=m_pProjects->getSelectedSavePath()+"/"+m_pRoad->getSelectedFile();
          printf("save road: \"%s\"\n",sRoad.c_str());
          m_pLoader->setCurrentRoadName(sRoad);
          m_pLoader->saveRoad();
        }
        break;
    }
  }

  return bRet;
}

/**
 * Add an additional event receiver
 * @param p event receiver to add
 */
void CRunner::addEventReceiver(IEventReceiver *p) {
  core::list<IEventReceiver *>::Iterator it;
  for (it=m_lReceivers.begin(); it!=m_lReceivers.end(); it++) if (*it==p) return;
  m_lReceivers.push_back(p);
}

/**
 * Remove an event receiver
 * @param p event receiver to remove
 */
void CRunner::removeEventReceiver(IEventReceiver *p) {
  core::list<IEventReceiver *>::Iterator it;
  for (it=m_lReceivers.begin(); it!=m_lReceivers.end(); it++)
    if (*it==p) {
      m_lReceivers.erase(it);
      return;
    }
}
