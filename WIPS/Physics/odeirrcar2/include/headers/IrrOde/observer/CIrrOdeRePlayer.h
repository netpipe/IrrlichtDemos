#ifndef _C_IRR_ODE_REPLAYER
  #define _C_IRR_ODE_REPLAYER

  #include <irrlicht.h>
  #include <CSerializer.h>

namespace irr {
namespace ode {

class IIrrOdeEvent;

class CIrrOdeRePlayer {
  protected:
    irr::IrrlichtDevice *m_pDevice;
    irr::core::stringc m_sReplay,
                       m_sSceneFile;
    irr::scene::ISceneManager *m_pSmgr;
    irr::ITimer *m_pTimer;

    irr::core::list<irr::ode::IIrrOdeEvent *> m_lEvents;
    irr::ode::CSerializer m_cSerializer;					  /**< the serializer for the messages */

    irr::u32 m_iLastStep,m_iTimeBuffer;

    irr::core::list<irr::ode::IIrrOdeEvent *>::Iterator m_iIt;

    bool m_bIsPaused,
         m_bValidReplay,
         m_bSceneLoaded,
         m_bReplayFinished;

    void clearEventList();

  public:
    CIrrOdeRePlayer(irr::IrrlichtDevice *pDevice, const irr::c8 *sReplay);
    ~CIrrOdeRePlayer();

    void update();

    void setIsPaused(bool b) { m_bIsPaused=b; }
    bool isPaused() { return m_bIsPaused; }

    bool isFinished() { return m_bReplayFinished; }
};

}
}

#endif
