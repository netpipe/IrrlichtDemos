#ifndef _MY_EVENT_RECEIVER
  #define _MY_EVENT_RECEIVER

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace ode;

class myEventReceiver : public IEventReceiver {
  private:
    bool m_bNewSphere,m_bNewBox,m_bNewCylinder,m_bNewStack,m_bNewTrimesh,m_bNewCapsule,m_bNewBridge,
         m_bNewFixedJoint,m_bNewBallJoint,m_bNewHingeJoint,m_bNewHinge2Joint,
         m_bNewSliderJoint,m_bNewPistonJoint,
         m_bLeftBtn,m_bLeftBtnLast,
         m_bRghtBtn,m_bRghtBtnLast,
         m_bMidlBtn,m_bMidlBtnLast,
         m_bShift,m_bCtrl,m_bXKey,
         m_bPause,m_bIncSpeed,m_bDecSpeed,m_bHelp,m_bDrawTerrainGrid;
    s32  m_iWheel;

  public:
    myEventReceiver();
    virtual ~myEventReceiver();

    virtual bool OnEvent(const SEvent &event);

    bool newSphere  ();
    bool newBox     ();
    bool newCylinder();
    bool newCapsule ();
    bool newStack   ();
    bool newTrimesh ();
    bool newBridge  ();

    bool newFixedJoint ();
    bool newBallJoint  ();
    bool newHingeJoint ();
    bool newHinge2Joint();
    bool newSliderJoint();
    bool newPistonJoint();

    bool leftBtn();
    bool rghtBtn();
    bool midlBtn();

    bool ctrlDown ();
    bool shiftDown();
    bool xKeyDown ();

    bool pause   ();
    bool help    ();
    bool incSpeed();
    bool decSpeed();

    bool drawTerrainGrid();

    s32 getWheelDelta();
};

#endif
