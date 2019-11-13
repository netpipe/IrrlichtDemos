  #include <irrlicht.h>
  #include <CIrrOdeSurfaceParameters.h>
  #include <CGUIFileSelector.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace ode;

enum enumGuiIds {
  eBtnNew,
  eBtnLoad,
  eBtnSave,
  eBtnNewParam,
  eBtnDelParam,
  eListParams,
  eEdMu,
  eEdMu2,
  eEdBounce,
  eEdBounceVel,
  eEdSoftErp,
  eEdSoftCfm,
  eEdMotion1,
  eEdMotion2,
  eEdMotionN,
  eEdSlip1,
  eEdSlip2,
  eEdName,
  eEdLinDamp,
  eEdAngDamp,
  eVMaxLin,
  eVMaxAng,
  eAddDampLin,
  eAddDampAng
};

class myReceiver : public IEventReceiver {
  protected:
    IrrlichtDevice *m_pDevice;
    IGUIEnvironment *m_pGuienv;

    IGUIListBox *m_pParams;

    IGUIEditBox *m_pEdMu,
                *m_pEdMu2,
                *m_pEdBounce,
                *m_pEdBounceVel,
                *m_pEdSoftErp,
                *m_pEdSoftCfm,
                *m_pEdMotion1,
                *m_pEdMotion2,
                *m_pEdMotionN,
                *m_pEdSlip1,
                *m_pEdSlip2,
                *m_pEdName,
                *m_pEdLinDamp,
                *m_pEdAngDamp,
                *m_pEdVMaxLin,
                *m_pEdVMaxAng,
                *m_pEdAddDampLin,
                *m_pEdAddDampAng;

    IGUICheckBox *m_pCbMu2,
                 *m_pCbFDir1,
                 *m_pCbBounce,
                 *m_pCbSoftERP,
                 *m_pCbSoftCfm,
                 *m_pCbMotion1,
                 *m_pCbMotion2,
                 *m_pCbMotionN,
                 *m_pCbSlip1,
                 *m_pCbSlip2,
                 *m_pCbDamps;

    IGUIStaticText *m_pStBounce,
                   *m_pStLinDamp,
                   *m_pStAngDamp,
                   *m_pStVMaxLin,
                   *m_pStVMaxAng,
                   *m_pStAddDampLin,
                   *m_pStAddDampAng;
    CGUIFileSelector *m_pDialog;
    stringw m_sFileName;
    u32 m_iFileState;

    list<CIrrOdeSurfaceParameters *> m_lParams;
    CIrrOdeSurfaceParameters *m_pActive;

    void updateList() {
      m_pParams->clear();
      list<CIrrOdeSurfaceParameters *>::Iterator it;
      for (it=m_lParams.begin(); it!=m_lParams.end(); it++)
        m_pParams->addItem((*it)->getName());
    }

    void getNewName(wchar_t *sName) {
      bool found=false;
      u32 i=0;
      do {
        found=false;
        swprintf(sName,0xFF,L"ParamSet_%i",i);
        list<CIrrOdeSurfaceParameters *>::Iterator it;
        for (it=m_lParams.begin(); it!=m_lParams.end() && !found; it++)
          found=(stringw(sName)==stringw((*it)->getName()));
        i++;
      }
      while (found);
    }

    void selectSet(u32 iIdx) {
      list<CIrrOdeSurfaceParameters *>::Iterator it;
      for (it=m_lParams.begin(); it!=m_lParams.end() && iIdx>0; it++) iIdx--;
      if (it==m_lParams.end()) {
        printf("Nope!\n");
        return;
      }

      CIrrOdeSurfaceParameters *p=*it;

      m_pCbMu2    ->setChecked(p->getModeMu2    ());
      //m_pCbFDir1  ->setChecked(p->getModeFDir1());
      m_pCbBounce ->setChecked(p->getModeBounce ());
      m_pCbSoftERP->setChecked(p->getModeSoftERP());
      m_pCbSoftCfm->setChecked(p->getModeSoftCFM());
      m_pCbMotion1->setChecked(p->getModeMotion1());
      m_pCbMotion2->setChecked(p->getModeMotion2());
      m_pCbMotionN->setChecked(p->getModeMotionN());
      m_pCbSlip1  ->setChecked(p->getModeSlip1  ());
      m_pCbSlip2  ->setChecked(p->getModeSlip2  ());
      m_pCbDamps  ->setChecked(p->dampsObjects  ());

      wchar_t ws[0xFF];

      m_pEdName->setText(p->getName());

      swprintf(ws,0xFF,L"%f",p->getMu                       ()); m_pEdMu        ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getMu2                      ()); m_pEdMu2       ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getBounce                   ()); m_pEdBounce    ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getBounceVel                ()); m_pEdBounceVel ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getSoftErp                  ()); m_pEdSoftErp   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getSoftCfm                  ()); m_pEdSoftCfm   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getMotion1                  ()); m_pEdMotion1   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getMotion2                  ()); m_pEdMotion2   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getMotionN                  ()); m_pEdMotionN   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getSlip1                    ()); m_pEdSlip1     ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getSlip2                    ()); m_pEdSlip2     ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getLinearDamping            ()); m_pEdLinDamp   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getAngularDamping           ()); m_pEdAngDamp   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getMaxLinearVelocity        ()); m_pEdVMaxLin   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getMaxAngularVelocity       ()); m_pEdVMaxAng   ->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getMaxLinearVelocityDamping ()); m_pEdAddDampLin->setText(ws);
      swprintf(ws,0xFF,L"%f",p->getMaxAngularVelocityDamping()); m_pEdAddDampAng->setText(ws);
    }

    void updateGUI() {
      bool bParam=m_pActive!=NULL;

      m_pEdMu2->setVisible(m_pCbMu2->isChecked());

      m_pEdBounce    ->setVisible(m_pCbBounce ->isChecked() && bParam);
      m_pEdBounceVel ->setVisible(m_pCbBounce ->isChecked() && bParam);
      m_pStBounce    ->setVisible(m_pCbBounce ->isChecked() && bParam);
      m_pEdSoftErp   ->setVisible(m_pCbSoftERP->isChecked() && bParam);
      m_pEdSoftCfm   ->setVisible(m_pCbSoftCfm->isChecked() && bParam);
      m_pEdMotion1   ->setVisible(m_pCbMotion1->isChecked() && bParam);
      m_pEdMotion2   ->setVisible(m_pCbMotion2->isChecked() && bParam);
      m_pEdMotionN   ->setVisible(m_pCbMotionN->isChecked() && bParam);
      m_pEdSlip1     ->setVisible(m_pCbSlip1  ->isChecked() && bParam);
      m_pEdSlip2     ->setVisible(m_pCbSlip2  ->isChecked() && bParam);
      m_pEdAngDamp   ->setVisible(m_pCbDamps  ->isChecked() && bParam);
      m_pEdLinDamp   ->setVisible(m_pCbDamps  ->isChecked() && bParam);
      m_pEdVMaxAng   ->setVisible(m_pCbDamps  ->isChecked() && bParam);
      m_pEdVMaxLin   ->setVisible(m_pCbDamps  ->isChecked() && bParam);
      m_pStLinDamp   ->setVisible(m_pCbDamps  ->isChecked() && bParam);
      m_pStAngDamp   ->setVisible(m_pCbDamps  ->isChecked() && bParam);
      m_pStVMaxLin   ->setVisible(m_pCbDamps  ->isChecked() && bParam);
      m_pStVMaxAng   ->setVisible(m_pCbDamps  ->isChecked() && bParam);

      bool b=m_pEdVMaxLin->getText()!=L"" && atof(stringc(m_pEdVMaxLin->getText()).c_str())!=0.0f && bParam;

      m_pStAddDampLin->setVisible(m_pCbDamps->isChecked() && b);
      m_pEdAddDampLin->setVisible(m_pCbDamps->isChecked() && b);

      b=m_pEdVMaxAng->getText()!=L"" && atof(stringc(m_pEdVMaxAng->getText()).c_str())!=0.0f && bParam;

      m_pStAddDampAng->setVisible(m_pCbDamps->isChecked() && b);
      m_pEdAddDampAng->setVisible(m_pCbDamps->isChecked() && b);
    }

    void selectParamSet(u32 iIdx) {
      list<CIrrOdeSurfaceParameters *>::Iterator it;
      u32 i=iIdx;
      for (it=m_lParams.begin(); it!=m_lParams.end() && i>0; it++) i--;

      if (it==m_lParams.end()) {
        printf("Nope2\n");
        return;
      }

      m_pActive=*it;
      selectSet(iIdx);
      updateGUI();
    }

    void setParamName(const wchar_t *sName) {
      if (!m_pActive) return;
      for (u32 i=0; i<m_pParams->getItemCount(); i++) {
        if (stringw(m_pParams->getListItem(i))==stringw(m_pActive->getName())) {
          printf("yep!\n");
          m_pParams->setItem(i,sName,0);
        }
      }
      m_pActive->setName(sName);
    }

    void saveParam(IXMLWriter *pWriter, CIrrOdeSurfaceParameters *pParam) {
      io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes(NULL);
      pWriter->writeElement(L"parameters",false);
      pWriter->writeLineBreak();
      pParam->serializeAttributes(pAttr,NULL,0);
      pAttr->write(pWriter);
      pAttr->drop();
      pWriter->writeClosingTag(L"parameters");
      pWriter->writeLineBreak();
    }

    void saveToFile(const wchar_t *sName) {
      printf("save to file!\n");
      io::IWriteFile *pFile=m_pDevice->getFileSystem()->createAndWriteFile(sName);
      io::IXMLWriter *pWriter=m_pDevice->getFileSystem()->createXMLWriter(pFile);

      if (!pWriter) {
        printf("unable to create file \"%s\"\n",stringc(stringw(sName)).c_str());
        return;
      }

      pWriter->writeXMLHeader();
      pWriter->writeLineBreak();

      list<CIrrOdeSurfaceParameters *>::Iterator it;
      for (it=m_lParams.begin(); it!=m_lParams.end(); it++) saveParam(pWriter,*it);

      pWriter->drop();
      pFile->drop();
    }

    void loadParameter(IXMLReader *pReader) {
      while (pReader->read()) {
        switch (pReader->getNodeType()) {
          case io::EXN_ELEMENT_END:
            if (stringw("attributes")==pReader->getNodeName()) {
              return;
            }
            break;

          case io::EXN_ELEMENT:
            if (stringw("attributes")==pReader->getNodeName()) {
              io::IAttributes *pAttr=m_pDevice->getFileSystem()->createEmptyAttributes(NULL);
              pAttr->read(pReader);
              CIrrOdeSurfaceParameters *p=new CIrrOdeSurfaceParameters();
              p->setStatic(true);
              p->deserializeAttributes(pAttr,NULL,0);
              m_lParams.push_back(p);
            }
            break;

          default: break;
        }
      }
    }

    u32 loadFromFile(const wchar_t *sName) {
      u32 iRet=0;
      while (m_lParams.getSize()>0) {
        list<CIrrOdeSurfaceParameters *>::Iterator it=m_lParams.begin();
        CIrrOdeSurfaceParameters *p=*it;
        m_lParams.erase(it);
        delete p;
      }

      printf("load from file!\n");
      io::IReadFile *pRead=m_pDevice->getFileSystem()->createAndOpenFile(sName);

      if (!pRead) {
        printf("unable to open \"%s\"\n",stringc(stringw(sName)).c_str());
        return 0;
      }

      io::IXMLReader *pReader=m_pDevice->getFileSystem()->createXMLReader(pRead);

      if (!pReader) {
        printf("\"%s\" is an invalid XML file\n",stringc(stringw(sName)).c_str());
        return 0;
      }

      while (pReader->read()) {
        switch (pReader->getNodeType()) {
          case io::EXN_ELEMENT_END:
            break;

          case io::EXN_ELEMENT:
            if (stringw("parameters")==pReader->getNodeName()) {
              printf("parameter!\n");
              iRet++;
              loadParameter(pReader);
            }
            break;

          default: break;
        }
      }

      printf("%i parameter sets loaded from \"%s\"\n",iRet,stringc(stringw(sName)).c_str());
      pReader->drop();
      pRead->drop();

      updateList();
      updateGUI();

      wchar_t sTitle[0xFF];
      m_sFileName=sName;
      m_sFileName.replace(L'\\',L'/');
      swprintf(sTitle,0xFF,L"IrrOdeSurfaceEdit - %s",m_sFileName.c_str());
      m_pDevice->setWindowCaption(sTitle);

      return iRet;
    }

  public:
    myReceiver(IrrlichtDevice *pDevice) {
      m_pDevice=pDevice;
      m_pGuienv=pDevice->getGUIEnvironment();

      m_sFileName=stringw(L"");

      m_pDialog=NULL;
      m_pActive=NULL;
      m_iFileState=0;

      m_pGuienv->addImage(m_pDevice->getVideoDriver()->getTexture("../../data/s_edit_title.png"),position2di(64,448));

      position2di pos=position2di(10,10),dist=position2di(0,22);
      dimension2du dim=dimension2du(120,20);

      m_pGuienv->addButton(rect<s32>(pos,dim),NULL,eBtnNew   ,L"New"    ); pos+=dist;
      m_pGuienv->addButton(rect<s32>(pos,dim),NULL,eBtnLoad  ,L"Load"   ); pos+=dist;
      m_pGuienv->addButton(rect<s32>(pos,dim),NULL,eBtnSave  ,L"Save"   ); pos+=dist; pos+=dist;

      m_pParams=m_pGuienv->addListBox(rect<s32>(pos,dimension2du(120,150)),NULL,eListParams,true);
      pos+=position2di(0,152);

      m_pGuienv->addButton(rect<s32>(pos,dim),NULL,eBtnNewParam,L"New Parameters"   ); pos+=dist;
      m_pGuienv->addButton(rect<s32>(pos,dim),NULL,eBtnDelParam,L"Delete Parameters"); pos+=dist;

      pos=position2di(250,10);

      m_pEdName     =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdName     ); pos+=dist;
      m_pEdMu       =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdMu       ); pos+=dist;
      m_pEdMu2      =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdMu2      ); pos+=dist;
      m_pEdBounce   =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdBounce   ); pos+=dist;
      m_pEdBounceVel=m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdBounceVel); pos+=dist;
      m_pEdSoftErp  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdSoftErp  ); pos+=dist;
      m_pEdSoftCfm  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdSoftCfm  ); pos+=dist;
      m_pEdMotion1  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdMotion1  ); pos+=dist;
      m_pEdMotion2  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdMotion2  ); pos+=dist;
      m_pEdMotionN  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdMotionN  ); pos+=dist;
      m_pEdSlip1    =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdSlip1    ); pos+=dist;
      m_pEdSlip2    =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdSlip2    ); pos+=dist; pos+=dist;
      m_pEdLinDamp  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdLinDamp  ); pos+=dist;
      m_pEdAngDamp  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eEdAngDamp  ); pos+=dist;
      m_pEdVMaxLin  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eVMaxLin    );

      position2di pos2=pos+position2di(200,0);
      m_pEdAddDampLin=m_pGuienv->addEditBox(L"",rect<s32>(pos2,dim),true,NULL,eAddDampLin); pos2+=dist;

      pos+=dist;
      m_pEdVMaxAng  =m_pGuienv->addEditBox(L"",rect<s32>(pos,dim),true,NULL,eVMaxAng); pos+=dist;

      m_pEdAddDampAng=m_pGuienv->addEditBox(L"",rect<s32>(pos2,dim),true,NULL,eAddDampAng); pos2+=dist;

      dimension2du cb=dimension2du(20,20);
      pos=position2di(225,10);

      pos+=dist;  pos+=dist;
      m_pCbMu2    =m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;
      m_pCbBounce =m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist; pos+=dist;
      m_pCbSoftERP=m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;
      m_pCbSoftCfm=m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;
      m_pCbMotion1=m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;
      m_pCbMotion2=m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;
      m_pCbMotionN=m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;
      m_pCbSlip1  =m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;
      m_pCbSlip2  =m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;
      m_pCbDamps  =m_pGuienv->addCheckBox(true,rect<s32>(pos,cb)); pos+=dist;

      pos=position2di(150,10);
      dim=dimension2du(65,20);
      IGUIStaticText *t=NULL;

      t=m_pGuienv->addStaticText(L"Name:  "        ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Mu:  "          ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Mu2:  "         ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Bounce:  "      ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Bounce Vel:  "  ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      m_pStBounce=t;
      t=m_pGuienv->addStaticText(L"SoftERP:  "     ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"SoftCFM:  "     ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Motion1:  "     ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Motion2:  "     ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"MotionN:  "     ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Slip1:  "       ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Slip2:  "       ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Damps:  "       ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      t=m_pGuienv->addStaticText(L"Damp Linear: "  ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      m_pStLinDamp=t;
      t=m_pGuienv->addStaticText(L"Damp Angular:  ",rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      m_pStAngDamp=t;
      t=m_pGuienv->addStaticText(L"VMax Linear:  " ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER);
      m_pStVMaxLin=t;

      pos2=pos+position2di(230,0);
      t=m_pGuienv->addStaticText(L"Damping:  ",rect<s32>(pos2,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER);
      m_pStAddDampLin=t;
      pos+=dist;
      pos2+=dist;

      t=m_pGuienv->addStaticText(L"VMax Angular:  " ,rect<s32>(pos,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      m_pStVMaxAng=t;
      t=m_pGuienv->addStaticText(L"Damping:  " ,rect<s32>(pos2,dim)); t->setDrawBackground(true); t->setTextAlignment(EGUIA_LOWERRIGHT,EGUIA_CENTER); pos+=dist;
      m_pStAddDampAng=t;
      updateGUI();
    }

    virtual bool OnEvent(const SEvent &event) {
      bool bRet=false,b;

      if (event.EventType==EET_GUI_EVENT) {
        if (event.GUIEvent.EventType==EGET_CHECKBOX_CHANGED && m_pActive!=NULL) {
          b=((IGUICheckBox *)event.GUIEvent.Caller)->isChecked();
          if (event.GUIEvent.Caller==m_pCbMu2    ) { m_pActive->setModeMu2(b); m_pEdMu2->setVisible(b); bRet=true; }
          if (event.GUIEvent.Caller==m_pCbBounce ) {
            m_pActive->setModeBounce(b);
            m_pEdBounce   ->setVisible(m_pCbBounce->isChecked());
            m_pEdBounceVel->setVisible(m_pCbBounce->isChecked());
            m_pStBounce   ->setVisible(m_pCbBounce->isChecked());
            bRet=true;
          }
          if (event.GUIEvent.Caller==m_pCbSoftERP) { m_pActive->setModeSoftERP(b); m_pEdSoftErp->setVisible(b); bRet=true; }
          if (event.GUIEvent.Caller==m_pCbSoftCfm) { m_pActive->setModeSoftCFM(b); m_pEdSoftCfm->setVisible(b); bRet=true; }
          if (event.GUIEvent.Caller==m_pCbMotion1) { m_pActive->setModeMotion1(b); m_pEdMotion1->setVisible(b); bRet=true; }
          if (event.GUIEvent.Caller==m_pCbMotion2) { m_pActive->setModeMotion2(b); m_pEdMotion2->setVisible(b); bRet=true; }
          if (event.GUIEvent.Caller==m_pCbMotionN) { m_pActive->setModeMotionN(b); m_pEdMotionN->setVisible(b); bRet=true; }
          if (event.GUIEvent.Caller==m_pCbSlip1  ) { m_pActive->setModeSlip1  (b); m_pEdSlip1  ->setVisible(b); bRet=true; }
          if (event.GUIEvent.Caller==m_pCbSlip2  ) { m_pActive->setModeSlip2  (b); m_pEdSlip2  ->setVisible(b); bRet=true; }
          if (event.GUIEvent.Caller==m_pCbDamps  ) {
            m_pActive->setDampsObjects(b);
            m_pEdAngDamp   ->setVisible(b);
            m_pEdLinDamp   ->setVisible(b);
            m_pEdVMaxAng   ->setVisible(b);
            m_pEdVMaxLin   ->setVisible(b);
            m_pStLinDamp   ->setVisible(b);
            m_pStAngDamp   ->setVisible(b);
            m_pStVMaxLin   ->setVisible(b);
            m_pStVMaxAng   ->setVisible(b);

            bool b2=m_pEdVMaxLin->getText()!=L"" && atof(stringc(m_pEdVMaxLin->getText()).c_str())!=0.0f;

            m_pStAddDampLin->setVisible(m_pCbDamps->isChecked() && b);
            m_pEdAddDampLin->setVisible(m_pCbDamps->isChecked() && b);

            b2=m_pEdVMaxAng->getText()!=L"" && atof(stringc(m_pEdVMaxAng->getText()).c_str())!=0.0f;

            m_pStAddDampAng->setVisible(m_pCbDamps->isChecked() && b);
            m_pEdAddDampAng->setVisible(m_pCbDamps->isChecked() && b);
          }
        }

        if (event.GUIEvent.EventType==EGET_EDITBOX_CHANGED && m_pActive!=NULL) {
          f32 f=atof(stringc(stringw(event.GUIEvent.Caller->getText())).c_str());
          if (event.GUIEvent.Caller==m_pEdVMaxLin) {
            m_pStAddDampLin->setVisible(f!=0.0f);
            m_pEdAddDampLin->setVisible(f!=0.0f);
          }
          if (event.GUIEvent.Caller==m_pEdVMaxAng) {
            m_pStAddDampAng->setVisible(f!=0.0f);
            m_pEdAddDampAng->setVisible(f!=0.0f);
          }
        }

        if (event.GUIEvent.EventType==EGET_ELEMENT_FOCUS_LOST && event.GUIEvent.Caller->getType()==EGUIET_EDIT_BOX && m_pActive!=NULL) {
          f32 f=atof(stringc(stringw(event.GUIEvent.Caller->getText())).c_str()),f2=0.0f;
          switch (event.GUIEvent.Caller->getID()) {
            case eEdName     : m_pActive->setName(m_pEdName->getText()); updateList(); break;
            case eEdMu       : m_pActive->setMu            (f); break;
            case eEdMu2      : m_pActive->setMu2           (f); break;
            case eEdBounce   : m_pActive->setBounce        (f); break;
            case eEdBounceVel: m_pActive->setBounceVel     (f); break;
            case eEdSoftErp  : m_pActive->setSoftErp       (f); break;
            case eEdSoftCfm  : m_pActive->setSoftCfm       (f); break;
            case eEdMotion1  : m_pActive->setMotion1       (f); break;
            case eEdMotion2  : m_pActive->setMotion2       (f); break;
            case eEdMotionN  : m_pActive->setMotionN       (f); break;
            case eEdSlip1    : m_pActive->setSlip1         (f); break;
            case eEdSlip2    : m_pActive->setSlip2         (f); break;
            case eEdLinDamp  : m_pActive->setLinearDamping (f); break;
            case eEdAngDamp  : m_pActive->setAngularDamping(f); break;
            case eVMaxLin:
              b=m_pEdVMaxLin->getText()!=L"" && atof(stringc(m_pEdVMaxLin->getText()).c_str())!=0.0f;
              m_pStAddDampLin->setVisible(b);
              m_pEdAddDampLin->setVisible(b);
              f2=atof(stringc(stringw(m_pEdAddDampLin->getText())).c_str());
              m_pActive->setMaxLinearVelocity(f,f2);
              break;

            case eVMaxAng:
              b=m_pEdVMaxAng->getText()!=L"" && atof(stringc(m_pEdVMaxAng->getText()).c_str())!=0.0f;
              m_pStAddDampAng->setVisible(b);
              m_pEdAddDampAng->setVisible(b);
              f2=atof(stringc(stringw(m_pEdAddDampAng->getText())).c_str());
              m_pActive->setMaxAngularVelocity(f,f2);
              break;

            case eAddDampLin:
              f2=atof(stringc(stringw(m_pEdVMaxLin->getText())).c_str());
              m_pActive->setMaxLinearVelocity(f2,f);
              break;

            case eAddDampAng:
              f2=atof(stringc(stringw(m_pEdVMaxAng->getText())).c_str());
              m_pActive->setMaxAngularVelocity(f2,f);
              break;
          }
        }

        if (event.GUIEvent.EventType==EGET_BUTTON_CLICKED) {
          switch (event.GUIEvent.Caller->getID()) {
            case eBtnNew:
              while (m_lParams.getSize()>0) {
                list<CIrrOdeSurfaceParameters *>::Iterator it=m_lParams.begin();
                CIrrOdeSurfaceParameters *p=*it;
                m_lParams.erase(it);
                delete p;
              }
              m_sFileName=L"";
              m_pDevice->setWindowCaption(L"IrrOdeSurfaceEdit");
              updateList();
              updateGUI();
              break;

            case eBtnLoad:
              m_iFileState=1;
              if (m_sFileName!=stringw(L"")) {
                s32 iPos=m_sFileName.findLast(L'/');
                if (iPos>0) {
                  stringw sDir=m_sFileName.subString(0,iPos);
                  m_pDevice->getFileSystem()->changeWorkingDirectoryTo(path(sDir));
                }
              }
              m_pDialog=new CGUIFileSelector(L"File Selector",m_pGuienv,m_pGuienv->getRootGUIElement(),1,CGUIFileSelector::EFST_OPEN_DIALOG);
              break;

            case eBtnSave:
              m_iFileState=2;
              if (m_sFileName!=stringw(L"")) {
                s32 iPos=m_sFileName.findLast(L'/');
                if (iPos>0) {
                  stringw sDir=m_sFileName.subString(0,iPos);
                  m_pDevice->getFileSystem()->changeWorkingDirectoryTo(path(sDir));
                }
              }
              m_pDialog=new CGUIFileSelector(L"File Selector",m_pGuienv,m_pGuienv->getRootGUIElement(),1,CGUIFileSelector::EFST_SAVE_DIALOG);
              break;

            case eBtnNewParam: {
                wchar_t sName[0xFF];
                CIrrOdeSurfaceParameters *p;
                this->getNewName(sName);
                p=new CIrrOdeSurfaceParameters();
                p->setStatic(true);
                p->setName(sName);
                m_lParams.push_back(p);
                updateList();
                updateGUI();
              }
              break;

            case eBtnDelParam: {
                u32 idx=m_pParams->getSelected();
                list<CIrrOdeSurfaceParameters *>::Iterator it;
                for (it=m_lParams.begin(); it!=m_lParams.end() && idx>0; it++) idx--;
                if (it!=m_lParams.end()) {
                  CIrrOdeSurfaceParameters *p=*it;
                  m_lParams.erase(it);
                  delete p;
                  m_pActive=NULL;
                  updateList();
                  updateGUI();
                }
              }
              break;
          }
        }

        if (event.GUIEvent.EventType==EGET_FILE_SELECTED && m_pDialog!=NULL) {
          if (!m_pDialog) return false;
          switch (m_iFileState) {
            case 1: loadFromFile(m_pDialog->getFileName()); break;
            case 2: saveToFile(m_pDialog->getFileName()); break;
          }
          m_iFileState=0;
          m_pDialog=NULL;
          bRet=true;
        }

        if (event.GUIEvent.EventType==EGET_LISTBOX_CHANGED || event.GUIEvent.EventType==EGET_LISTBOX_SELECTED_AGAIN) {
          selectParamSet(m_pParams->getSelected());
        }
      }

      return bRet;
    }

    virtual ~myReceiver() {
    }
};

int main(int argc, char** argv) {
  IrrlichtDevice *device=createDevice(EDT_SOFTWARE, dimension2d<u32>(640,480),16,false,false,false,0);

  device->setWindowCaption(L"IrrOdeSurfaceEdit");

  IVideoDriver* driver = device->getVideoDriver();
  ISceneManager* smgr = device->getSceneManager();
  IGUIEnvironment* guienv = device->getGUIEnvironment();

  myReceiver *pReceiver=new myReceiver(device);
  device->setEventReceiver(pReceiver);

  while(device->run())
  {
    driver->beginScene(true, true, SColor(0,64,64,64));

    smgr->drawAll();
    guienv->drawAll();

    driver->endScene();
  }

  device->drop();
  return 0;
}
