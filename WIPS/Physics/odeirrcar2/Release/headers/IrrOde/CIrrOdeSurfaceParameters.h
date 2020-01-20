#ifndef _C_IRR_ODE_SURFACE_PARAMETERS
  #define _C_IRR_ODE_SURFACE_PARAMETERS

  #include <irrlicht.h>

namespace irr {
namespace ode {

using namespace scene;
using namespace core;
using namespace io;

const int IRR_ODE_SURFACE_PARAMETERS_ID=MAKE_IRR_ID('i','o','s','p');
const c8 IRR_ODE_SURFACE_PARAMETER_NAME[0xFF]="CIrrOdeSurfaceParameter";

enum {
  eContactMu2		= 0x001,
  eContactFDir1		= 0x002,
  eContactBounce	= 0x004,
  eContactSoftERP	= 0x008,
  eContactSoftCFM	= 0x010,
  eContactMotion1	= 0x020,
  eContactMotion2	= 0x040,
  eContactMotionN	= 0x080,
  eContactSlip1		= 0x100,
  eContactSlip2		= 0x200,

  eContactApprox0	= 0x0000,
  eContactApprox1_1	= 0x1000,
  eContactApprox1_2	= 0x2000,
  eContactApprox1	= 0x3000
};

/**
 * @class CIrrOdeSurfaceParameters
 * This class encapsulates the ODE contact parameter structure.
 */
class CIrrOdeSurfaceParameters {
  protected:
		stringw m_sName;

		bool m_bUseOther;

    s32 mode;
    f32 mu,mu2;
    f32 bounce,bounce_vel;
    f32 soft_erp,soft_cfm;
    f32 motion1, motion2, motionN;
    f32 slip1, slip2,
        m_fLinearDamp,            /**< used if m_bDamps is true. Linear damping applied to dynamic object */
        m_fAngularDamp,           /**< used if m_bDamps is true. Angular damping applied to dynamic object */
        m_fMaxLinearVel,          /**< used if m_bDamps is true. Limit dynamic object to this linear velocity */
        m_fMaxAngularVel,         /**< used if m_bDamps is true. Limit dynamic object to this angular velocity */
        m_fMaxLVelDamp,           /**< used if m_bDamps is true. Damping if linear velocity exceeds m_fMaxLinearVel */
        m_fMaxAVelDamp;           /**< used if m_bDamps is true. Damping if angular velocity exceeds m_fMaxAnuglarVel */


    bool m_bDamps,                /**< does the (static) geom damp danymic geoms? */
         m_bStatic;               /**< is this a material of a static geom? */
  public:
    CIrrOdeSurfaceParameters();
    virtual ~CIrrOdeSurfaceParameters();

		const wchar_t *getName();
		void setName(const wchar_t *sName);

		bool doesUseDefined();
		void setUseDefined(bool b);

    void setModeMu2(bool bIn);        /**!< set the "mu2" contact flag */
    void setModeFDir1(bool bIn);      /**!< set the "FDir1" contact flag */
    void setModeBounce(bool bIn);     /**!< set the "bounce" contact flag */
    void setModeSoftERP(bool bIn);    /**!< set the "SoftERP" contact flag */
    void setModeSoftCFM(bool bIn);    /**!< set the "SoftCFM" contact flag */
    void setModeMotion1(bool bIn);    /**!< set the "Motion1" contact flag */
    void setModeMotion2(bool bIn);    /**!< set the "Motion2" contact flag */
    void setModeMotionN(bool bIn);    /**!< set the "MotionN" contact flag */
    void setModeSlip1(bool bIn);      /**!< set the "slip1" contact flag */
    void setModeSlip2(bool bIn);      /**!< set the "slip2" contact flag */
    void setModeApprox1_1(bool bIn);  /**!< set the "Approx1_1" contact flag */
    void setModeApprox1_2(bool bIn);  /**!< set the "Aprrox1_2" contact flag */
    void setModeApprox1(bool bIn);    /**!< set the "Approx1" contact flag */

    bool getModeMu2();
    bool getModeFDir1();
    bool getModeBounce();
    bool getModeSoftERP();
    bool getModeSoftCFM();
    bool getModeMotion1();
    bool getModeMotion2();
    bool getModeMotionN();
    bool getModeSlip1();
    bool getModeSlip2();
    bool getModeApprox1_1();
    bool getModeApprox1_2();
    bool getModeApprox1();

    void copy(CIrrOdeSurfaceParameters *pParam);

    virtual s32 getMode() { return mode; }
    virtual f32 getMu() { return mu; }
    virtual f32 getMu2() { return mu2; }
    virtual f32 getBounce() { return bounce; }
    virtual f32 getBounceVel() { return bounce_vel; }
    virtual f32 getSoftErp() { return soft_erp; }
    virtual f32 getSoftCfm() { return soft_cfm; }
    virtual f32 getMotion1() { return motion1; }
    virtual f32 getMotion2() { return motion2; }
    virtual f32 getMotionN() { return motionN; }
    virtual f32 getSlip1() { return slip1; }
    virtual f32 getSlip2() { return slip2; }

    virtual void setMode(s32 i) { mode=i; }
    virtual void setMu(f32 f) { mu=f; }
    virtual void setMu2(f32 f) { mu2=f; }
    virtual void setBounce(f32 f) { bounce=f; }
    virtual void setBounceVel(f32 f) { bounce_vel=f; }
    virtual void setSoftErp(f32 f) { soft_erp=f; }
    virtual void setSoftCfm(f32 f) { soft_cfm=f; }
    virtual void setMotion1(f32 f) { motion1=f; }
    virtual void setMotion2(f32 f) { motion2=f; }
    virtual void setMotionN(f32 f) { motionN=f; }
    virtual void setSlip1(f32 f) { slip1=f; }
    virtual void setSlip2(f32 f) { slip2=f; }

    virtual void serializeAttributes(irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options, u32 iIdx) const;
    virtual void deserializeAttributes(irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options, u32 iIdx);

   	void initPhysics();

   	/**
   	 * set this surface to be of a static or dynamic mesh
   	 * @param b "true" if surface is of static mesh
   	 */
   	void setStatic(bool b) { m_bStatic=b; }

   	/**
   	 * set whether or not this surface damps colliding object
   	 * @param b the new value
   	 */
   	void setDampsObjects(bool b) { m_bDamps=b; }

   	/**
   	 * set the linear damping applied to colliding objects
   	 * @param f the linear damping applied to colliding objects
   	 */
   	void setLinearDamping(f32 f) { m_fLinearDamp=f; }

   	/**
   	 * set the angular damping applied to colliding objects
   	 * @param f the angular damping applied to colliding objects
   	 */
   	void setAngularDamping(f32 f) { m_fAngularDamp=f; }

   	/**
   	 * set maximum linear velocity of colliding objects
   	 * @param f the maximum linear velocity of colliding objects
   	 * @param d the damping applied to faster objects
   	 */
   	void setMaxLinearVelocity(f32 f, f32 d) { m_fMaxLinearVel=f; m_fMaxLVelDamp=d; }

   	/**
   	 * set the maximum angular velocity of colliding objects
   	 * @param f the maxaimum angular velocity of colliding objects
   	 * @param d the damping applied to faster objects
   	 */
   	void setMaxAngularVelocity(f32 f, f32 d) { m_fMaxAngularVel=f; m_fMaxAVelDamp=d; }

   	/**
   	 * Is this a surface of a static mesh?
   	 * @return is this the surface of a static mesh
   	 */
   	bool isStatic() {
   	  return m_bStatic;
    }

    /**
     * Does this surface (of a static mesh) damp colliding dynamic objects?
     * @return true or false
     */
   	bool dampsObjects() {
   	  return m_bDamps;
    }

    /**
     * get the linear damping that is applied to colliding objects
     * @return the linear damping (0.0=no damping, 1.0=full damping)
     */
   	f32 getLinearDamping() {
   	  return m_fLinearDamp;
    }

    /**
     * get the angular damping that is applied to colliding objects
     * @return the angular damping (0.0=no damping, 1.0=full damping)
     */
   	f32 getAngularDamping() { return m_fAngularDamp; }

   	/**
   	 * get the maximum linear velocity that colliding objects are limited to
   	 * @return the maximum linear velocity
   	 */
   	f32 getMaxLinearVelocity() {
   	  return m_fMaxLinearVel;
    }

    /**
     * get the maximum angular velocity that colliding objects are limited to
     * @return the maximum angular velocity
     */
   	f32 getMaxAngularVelocity() {
   	  return m_fMaxAngularVel;
    }

    /**
     * get the linear damping that is applied to colliding objects that have a faster linear velocity than m_fMaxLinearVel
     * @return the damping
     * @see m_fMaxLinearVel
     */
   	f32 getMaxLinearVelocityDamping() {
   	  return m_fMaxLVelDamp;
   	}

   	/**
   	 * get the angular damping that is applied to colliding objects that have a faster angular velocity than m_fMaxAngularVel
   	 * @return the damping
   	 * @see m_fMaxAngularVel
   	 */
   	f32 getMaxAngularVelocityDamping() {
   	  return m_fMaxAVelDamp;
   	}
};

} //namespace ode
} //namespace irr

#endif
