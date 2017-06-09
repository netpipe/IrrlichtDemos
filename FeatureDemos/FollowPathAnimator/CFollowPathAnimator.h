#ifndef _C_FOLLOW_PATH_ANIMATOR
  #define _C_FOLLOW_PATH_ANIMATOR

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
 * @class CFollowPathAnimator
 * This class is a simple animator for camera scene nodes only. You can specify a path in IrrEdit using any type
 * of scene nodes (I prefer empty scene nodes) and have the camera follow the path you defined. There must be one
 * scene node as parent of the path and the children of this node define the waypoints.
 */
class CFollowPathAnimator : public ISceneNodeAnimator {
  private:
    list<vector3df>::Iterator m_cIter;  /**< the iterator of the waypoints */
    list<vector3df> m_cPathPoints;      /**< the positions of the waypoints */
    vector3df m_cCamPos,                /**< the current camera position */
              m_cCamTgt,                /**< the next waypoint the camera goes to */
              m_cCamDir,                /**< the direction the camera is looking at */
              m_cCamNDr,                /**< the target direction of the camera when focusing the next waypoint */
              m_cCamTrn;                /**< the turn vector of the camera when focusing the next waypoint */

    bool m_bAnimRunning,    /**< is the animation still running? */
         m_bStopNextHit,    /**< is the animation going to be stopped once the next waypoint is reached? */
         m_bLoopAnim;       /**< is the anim looped? */

    f32 m_fAnimSpeed,       /**< the animation speed */
        m_fCamTargetSpeed;  /**< the speed of focusing the next waypoint */
    u32 m_iLastTime,        /**< the last animator time */
        m_iState,           /**< state of the animation */
        m_iNextState,
        m_iDelayTime;

  public:
    /**
     * The constructor
     * @param pDevice the irrlicht device
     * @param sPathRootName the name of the node that is parent of the list of waypoints
     * @param fAnimSpeed the animation speed
     * @param fCamTargetSpeed the speed of focusing the next waypoint
     */
    CFollowPathAnimator(IrrlichtDevice *pDevice, c8 *sPathRootName, f32 fAnimSpeed, f32 fCamTargetSpeed);
    virtual void animateNode(ISceneNode *pNode, u32 timeMs);
    virtual ISceneNodeAnimator *createClone(ISceneNode *node, ISceneManager *pNewMngr);
    virtual ESCENE_NODE_ANIMATOR_TYPE getType() const;
    virtual bool isEventReceiverEnabled() const;
    virtual ~CFollowPathAnimator();

    /**
     * If you want to get notified when the animation ends derive your own class from this class and override
     * the implementation of this method
     */
    virtual void animatedEnded();

    /**
     * Call this method before you start the animation to set the camera to it's initial position and target
     */
    void setCamParameter(ICameraSceneNode *pCam);
};

#endif
