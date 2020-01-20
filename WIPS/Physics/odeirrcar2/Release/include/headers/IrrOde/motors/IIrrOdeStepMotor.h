#ifndef _I_IRR_ODE_STEP_MOTOR
  #define _I_IRR_ODE_STEP_MOTOR

  #include <CIrrOdeSceneNode.h>
  #include <CIrrOdeManager.h>
  #include <CIrrOdeBody.h>
  #include <CIrrOdeWorld.h>

namespace irr {
  namespace ode {

/**
 * @class IIrrOdeStepMotor
 * This class is an addition to ODE. Classes implementing this interface can add forces or torques
 * to IrrOde bodies after each simulation step, e.g. rocket engines.
 */
class IIrrOdeStepMotor : public CIrrOdeSceneNode {
  protected:
    CIrrOdeBody *m_pBody;   /*!< the body this motor will affect */
    f32 m_fPower,           /*!< the percentage of the max power this motor adds (between 0 and 1) */
        m_fMaxPower;        /*!< the max power of this motor. The added force or torque is "m_fPercent*m_fMaxPower*m_vVector" */
    bool m_bIsActive;       /*!< is this motor active? */

  public:
    IIrrOdeStepMotor(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                     const vector3df &position=core::vector3df(0,0,0),
		                 const vector3df &rotation = core::vector3df(0,0,0),
		                 const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f)) : CIrrOdeSceneNode(parent,mgr,id,position,rotation,scale) {
      m_pBody=(CIrrOdeBody *)getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_BODY_ID);
      if (m_pBody!=NULL) m_pBody->addStepMotor(this);
      CIrrOdeWorld *pWorld=reinterpret_cast<CIrrOdeWorld *>(getAncestorOfType((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID));
      if (pWorld!=NULL) CIrrOdeManager::getSharedInstance()->addStepMotor(this);
      m_fMaxPower=0.0f;
      m_fPower=0.0f;
      m_bIsActive=false;
    }

    virtual ~IIrrOdeStepMotor() {
      CIrrOdeManager::getSharedInstance()->removeStepMotor(this);
    }

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const {
      CIrrOdeSceneNode::serializeAttributes(out,options);

      out->addBool("active",m_bIsActive);
      out->addFloat("max_power",m_fMaxPower);
      out->addFloat("power",m_fPower);
    }

    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) {
      CIrrOdeSceneNode::deserializeAttributes(in,options);

      m_bIsActive=in->getAttributeAsBool("active");
      m_fMaxPower=in->getAttributeAsFloat("max_power");
      m_fPower=in->getAttributeAsFloat("power");
    }

    virtual void step()=0;

    virtual void initPhysics() {
      if (m_bPhysicsInitialized) return;
      CIrrOdeSceneNode::initPhysics();

      ISceneNode *pParent=getParent();
      if (pParent->getType()==IRR_ODE_BODY_ID)
        m_pBody=reinterpret_cast<CIrrOdeBody *>(pParent);
      else
        m_pBody=NULL;
    }

    void setBody(CIrrOdeBody *pBody) {
      if (m_pBody!=NULL) m_pBody->removeStepMotor(this);
      m_pBody=pBody;
    }

    virtual void setPower(f32 f) { m_fPower=f; }
    virtual f32 getPower() { return m_fPower; }
    virtual void setMaxPower(f32 f) { m_fMaxPower=f; }
    virtual f32 getMaxPower() { return m_fMaxPower; }
    virtual void setIsActive(bool b) { m_bIsActive=b; }
    virtual bool isActive() { return m_bIsActive; }

    virtual void removeFromPhysics() {
      m_pBody=NULL;
      CIrrOdeManager::getSharedInstance()->removeStepMotor(this);
      CIrrOdeSceneNode::removeFromPhysics();
    }
};

  } //namespace ode
} //namespace irr
#endif
