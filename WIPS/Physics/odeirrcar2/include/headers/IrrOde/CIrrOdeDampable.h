#ifndef _C_IRR_ODE_DAMPABLE
  #define _C_IRR_ODE_DAMPABLE

  #include <CIrrOdeSceneNode.h>
  #include <limits>

  #define _DEFAULT_DAMPING_LINEAR 0.0f
  #define _DEFAULT_DAMPING_ANGULAR 0.0f
  #define _DEFAULT_DAMPING_LINEAR_THRESHOLD 0.01f
  #define _DEFAULT_DAMPING_ANGULAR_THRESHOLD 0.01f

  #define _DEFAULT_AUTO_DISABLE_FLAG 0
  #define _DEFAULT_AUTO_DISABLE_LINEAR_THRESHOLD 0.01f
  #define _DEFAULT_AUTO_DISABLE_ANGULAR_THRESHOLD 0.01f
  #define _DEFAULT_AUTO_DISABLE_STEPS 10
  #define _DEFAULT_AUTO_DISABLE_TIME 0.0f

  #define _DEFAULT_MAX_ANGULAR_SPEED 0.0f
  
namespace irr {
namespace ode {

/**
 * @class CIrrOdeDampable
 * @brief an interface for classes that support damping
 * @see CIrrOdeWorld
 * @see CIrrOdeBody
 * This is just a little interface that provides access to the damping and enable / disable functions of ODE. There are
 * only two subclasses: CIrrOdeWorld and CIrrOdeBody
 */
class CIrrOdeDampable : public CIrrOdeSceneNode {
  protected:
    f32 m_fDampingLinear,m_fDampingAngular,m_fDampingLinearThreshold,m_fDampingAngularThreshold,
        m_fAutoDisableLinearThreshold,m_fAutoDisableAngularThreshold,m_fAutoDisableTime,
        m_fMaxAngularSpeed;
    s32 m_iAutoDisableSteps,m_iAutoDisableFlag;
		stringw m_sParamName;
		bool m_bUseDefined;

  public:
    CIrrOdeDampable(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                    const vector3df &position=core::vector3df(0,0,0),
		                const vector3df &rotation = core::vector3df(0,0,0),
		                const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeDampable();

    virtual void setLinearDamping(f32 fScale)=0;               /*!< set linear damping */
    virtual f32 getLinearDamping()=0;                          /*!< get linear damping */
    virtual void setAngularDamping(f32 fScale)=0;              /*!< set angular damping */
    virtual f32 getAngularDamping()=0;                         /*!< get angular damping */
    virtual void setLinearDampingThreshold(f32 fThreshold)=0;  /*!< set linear damping threshold */
    virtual f32 getLinearDampingThreshold()=0;                 /*!< get linear damping threshold */
    virtual void setAngularDampingThreshold(f32 fThreshold)=0; /*!< set angular damping threshold */
    virtual f32 getAngularDampingThreshold()=0;                /*!< get angular damping threshold */
    virtual void setMaxAngularSpeed(f32 fMaxSpeed)=0;          /*!< set maximum angular speed */
    virtual f32 getMaxAngularSpeed()=0;                        /*!< get maximum angular speed */

    virtual void setAutoDisableFlag(int iFlag)=0;                   /*!< set the auto disable flag */
    virtual int getAutoDisableFlag()=0;                             /*!< get the auto disable flag */
    virtual void setAutoDisableLinearThreshold(f32 fThreshold)=0;   /*!< set the auto disable linear threshold */
    virtual f32 getAutoDisableLinearThreshold()=0;                  /*!< get the auto disable linear threshold */
    virtual void setAutoDisableAngularThreshold(f32 fThreshold)=0;  /*!< set the auto disable angular threshold */
    virtual f32 getAutoDisableAngularThreshold()=0;                 /*!< get the auto disable angular threshold */
    virtual void setAutoDisableSteps(s32 iSteps)=0;                 /*!< set the auto disable steps parameter */
    virtual s32 getAutoDisableSteps()=0;                            /*!< get the auto disable steps parameter */
    virtual void setAutoDisableTime(f32 fTime)=0;                   /*!< set the auto disable time parameter */
    virtual f32 getAutoDisableTime()=0;                             /*!< get the auto disable time parameter */

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

		const wchar_t *getParamName() { return m_sParamName.c_str(); }
		const bool doesUseDefined() { return m_bUseDefined; }
};

} //namespace ode
} //namespace irr

#endif
