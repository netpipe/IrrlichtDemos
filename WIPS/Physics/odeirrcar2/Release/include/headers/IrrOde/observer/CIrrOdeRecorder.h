#ifndef _C_IRR_ODE_RECORDER
  #define _C_IRR_ODE_RECORDER

  #include <irrlicht.h>

  #include <event/IIrrOdeEventListener.h>
  #include <event/IIrrOdeEvent.h>

namespace irr { namespace ode {

/**
 * @class CIrrOdeRecorder
 * @author Christian Keimel / bulletbyte.de
 * This class is a recorder that records all changes within a world and saves them to a
 * binary file. To use this recording function you need to to install the CIrrOdeWorldObserver
 * before starting the simulation.
 * @see CIrrOdeWorldObserver::install
 */
class CIrrOdeRecorder : public IIrrOdeEventListener {
  protected:
    IrrlichtDevice *m_pDevice;
    bool m_bRecording;
    irr::core::list<IIrrOdeEvent *> m_lEvents;
    stringc m_sAppName;

  public:
    /**
     * The constructor
     * @param pDevice the Irrlicht device to use
     * @param sAppName the application's name (needed for later identification of the reording application)
     */
    CIrrOdeRecorder(IrrlichtDevice *pDevice, const c8 *sAppName);
    ~CIrrOdeRecorder();

    /**
     * Start the recording
     */
    void startRecording();

    /**
     * Stop the recording
     */
    void stopRecording();

    /**
     * Clear the recording
     */
    void clearRecording();

    /**
     * Save the recording to a file
     * @param sFileName the file to save to
     * @return "true" if saving succeeded, "false" otherwise
     */
    bool saveRecording(const c8 *sFileName);

    /**
     * Is a recording running?
     */
    bool isRecording() { return m_bRecording; }

    /**
     * IrrOde callback method
     */
    virtual bool onEvent(IIrrOdeEvent *pEvent);

    /**
     * IrrOde callback method
     */
    virtual bool handlesEvent(IIrrOdeEvent *pEvent);
};

} } //namespaces

#endif
