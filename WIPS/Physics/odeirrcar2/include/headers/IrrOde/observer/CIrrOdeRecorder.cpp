  #include <observer/CIrrOdeRecorder.h>
  #include <observer/CIrrOdeWorldObserver.h>
  #include <event/CIrrOdeEventIdentify.h>
  #include <event/CIrrOdeEventNodeCloned.h>

namespace irr {
namespace ode {

CIrrOdeRecorder::CIrrOdeRecorder(IrrlichtDevice *pDevice, const c8 *sAppName) {
  m_bRecording=false;
  m_pDevice=pDevice;
  m_sAppName=sAppName;
}

CIrrOdeRecorder::~CIrrOdeRecorder() {
  clearRecording();
}

void CIrrOdeRecorder::startRecording() {
  CIrrOdeEventIdentify *p=new CIrrOdeEventIdentify(m_sAppName.c_str());
  m_lEvents.push_back(p);
  m_bRecording=true;
  CIrrOdeWorldObserver::getSharedInstance()->addEventListener(this);
}

void CIrrOdeRecorder::stopRecording() {
  CIrrOdeWorldObserver::getSharedInstance()->removeEventListener(this);
  m_bRecording=false;
}

bool CIrrOdeRecorder::saveRecording(const c8 *sFileName) {
  printf("saving replay to file \"%s\" (%i events) ",sFileName,m_lEvents.getSize());
  IWriteFile *f=m_pDevice->getFileSystem()->createAndWriteFile(sFileName);

  if (m_lEvents.getSize()==0) { printf("no recording to save!\n"); return false; }
  if (!f) { printf("error while saving\n"); return false; }

  list<IIrrOdeEvent *>::Iterator it;
  for (it=m_lEvents.begin(); it!=m_lEvents.end(); it++) {
    IIrrOdeEvent *p=*it;
    CSerializer *pSerialize=p->serialize();
    if (pSerialize!=NULL) {
      u32 iSize=pSerialize->getBufferSize();
      f->write(&iSize,sizeof(u32));
      f->write(pSerialize->getBuffer(),iSize);
    }
  }

  f->drop();
  clearRecording();
  printf("... Ready.\n");
  return true;
}

void CIrrOdeRecorder::clearRecording() {
  while (m_lEvents.getSize()>0) {
    list<IIrrOdeEvent *>::Iterator it=m_lEvents.begin();
    IIrrOdeEvent *p=*it;
    m_lEvents.erase(it);
    delete p;
  }
}

bool CIrrOdeRecorder::onEvent(IIrrOdeEvent *pEvent) {
  if (!m_bRecording) return false;
  m_lEvents.push_back(pEvent->clone());
  return true;
}

bool CIrrOdeRecorder::handlesEvent(IIrrOdeEvent *pEvent) {
  return true;
}

}
}
