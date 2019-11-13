#ifndef _MY_EVENT_LISTENERS
  #define _MY_EVENT_LISTENERS

  #include <irrlicht.h>
  #include <IrrOde.h>

  #include "myEventReceiver.h"

class myBodyEventListener : public IIrrOdeEventListener {
  private:
    IVideoDriver *m_pDriver;
    IGUIStaticText *m_pActiveBodies;
    ITexture *m_pBoxTextures[3],*m_pSphereTextures[3],*m_pCylTexture[3],*m_pTriTexture[3];
    s32 m_iActive,m_iTotal;
    f32 m_fMinX,m_fMinZ,m_fMaxX,m_fMaxZ;

    void updateBodyText() {
      wchar_t buffer[0xFF];
      swprintf(buffer,255,L"active bodies: %i / %i\n",m_iActive,m_iTotal);
      m_pActiveBodies->setText(buffer);
    }

  public:
    myBodyEventListener(IVideoDriver *pDriver, IGUIStaticText *pActiveBodies, f32 fMinX, f32 fMinZ, f32 fMaxX, f32 fMaxZ);
    bool onEvent(IIrrOdeEvent *pEvent);
    bool handlesEvent(IIrrOdeEvent *pEvent);
};

const wchar_t cInfoText[255]=L"Body #%i:\n\"%s\"\ndistance=%.2f\npos = (%.0f, %.0f, %.0f)\n rot = (%.0f, %.0f, %.0f)\nlin = %.2f = (%.2f, %.2f, %.2f)\nang = %.2f = (%.2f, %.2f, %.2f)\n%s";

class myRayListener : public IIrrOdeEventListener {
  private:
    myEventReceiver *m_pInput;
    ICameraSceneNode *m_pCam;
    IGUIStaticText *m_pInfo;
    IGUIStaticText *m_pJoints;
    bool m_bRayHit;
    int m_iRayCnt;

  public:
    myRayListener(myEventReceiver *pInput, ICameraSceneNode *pCam, IGUIStaticText *pInfo, IGUIStaticText *pJoints);
    virtual bool onEvent(IIrrOdeEvent *pEvent);
    virtual bool handlesEvent(IIrrOdeEvent *pEvent);

    bool rayHit();
};

#endif
