#ifndef _IRR_ODE_MANAGER
  #define _IRR_ODE_MANAGER

  #include <irrlicht.h>

namespace irr {
namespace ode {

class CIrrOdeSurfaceParameters;
class CIrrOdeGeom;
class CIrrOdeWorld;
class IIrrOdeEventListener;
class CIrrOdeSceneNode;
class IIrrOdeDevice;
class IIrrOdeEvent;

/**
 * @class CIrrOdeManager
 * This class is the central manager of IrrODE.
 */
class CIrrOdeManager {
  protected:
    u32 m_iLastStep,                                      /**< the time in ms left over from the last step */
        m_iFrameNo;                                       /**< counter of rendered frames */

    core::array<irr::ode::CIrrOdeWorld *> m_pWorlds;            /**< the worlds */
    core::list<irr::ode::IIrrOdeEventListener *> m_pListeners;  /**< the event listeners */
    core::list<irr::ode::CIrrOdeSceneNode *> m_pSceneNodes;     /**< the scene nodes */
    core::list<irr::ode::CIrrOdeGeom *>m_pGeoms;                /**< the geoms */
    ITimer *m_pTimer;                                     /**< the Irrlicht timer */

		core::list<irr::ode::CIrrOdeSurfaceParameters *> m_lParamList;

		#ifndef _USE_ODE_NULL_DEVICE
		  IIrrOdeDevice *m_pOdeDevice;
		#endif

    CIrrOdeManager();  /**< protected contructor for singleton */
    ~CIrrOdeManager();

  public:
    static CIrrOdeManager *getSharedInstance();   /*!< get a pointer to the singleton instance of this class */

    void setTimer(ITimer *pTimer);            /*!< set the timer used for stepping the world */
    void addWorld(CIrrOdeWorld *pWorld);      /*!< add a world to the manager */
    void removeWorld(CIrrOdeWorld *pWorld);   /*!< remove a world from the manager */

    void addOdeSceneNode(CIrrOdeSceneNode *pNode);    /*!< add an ode scenenode to the manager */
    void removeOdeSceneNode(CIrrOdeSceneNode *pNode); /*!< remove an ode scenenode from the manager */

    u32 getWorldCount();    /*!< get the number of worlds in the scene manager */

    CIrrOdeWorld *getWorld(u32 iPos);   /*!< get a world */

    void initODE();   /*!< init ODE */
    void closeODE();  /*!< close ODE */
    void clearODE();  /*!< remove all objects from the simulation */

    void step();      /*!< step the world(s) */
    void quickStep(); /*!< quickstep the world(s) */

    void addEventListener(IIrrOdeEventListener *pListener);			/*!< add an event listener */
		void removeEventListener(IIrrOdeEventListener *pListener);	/*!< remove an event listener */
    void postEvent(IIrrOdeEvent *pEvent);												/*!< post an event to the listeners */
    IIrrOdeDevice *getOdeDevice();															/*!< get a pointer to the ODE device */
    void setOdeDevice(IIrrOdeDevice *pDevice);                  /*!< set a new ODE device */
    void initPhysics();																					/*!< init all dynamic objects */
    void addGeom(CIrrOdeGeom *pGeom);														/*!< add a geom */
    core::list<CIrrOdeSceneNode *> &getIrrOdeNodes();									/*!< get a list of all registered IrrOde scene nodes */

		void addSurfaceParameter(CIrrOdeSurfaceParameters *pParam);
		CIrrOdeSurfaceParameters *getSurfaceParameter(core::stringw sName);

		u32 getFrameNo() { return m_iFrameNo; }
};

} //namespace ode
} //namespace irr

#endif
