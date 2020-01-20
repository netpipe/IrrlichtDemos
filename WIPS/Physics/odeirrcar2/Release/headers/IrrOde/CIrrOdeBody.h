#ifndef _C_IRR_ODE_BODY
  #define _C_IRR_ODE_BODY

  #include <CIrrOdeDampable.h>

namespace irr {
namespace ode {

const int IRR_ODE_BODY_ID=MAKE_IRR_ID('i','o','b','d'); /**< create an Irrlicht ID for this class */
const wchar_t IRR_ODE_BODY_NAME[0xFF]=L"CIrrOdeBody";   /**< store a name for this class */

class CIrrOdeWorld;       /**< foreward declaration of CIrrOdeWorld      */
class CIrrOdeJoint;       /**< foreward declaration of CIrrOdeJoint      */
class CIrrOdeSpace;       /**< foreward declaration of CIrrOdeSpace      */
class CIrrOdeGeom;        /**< foreward declaration of CIrrOdeGeom       */
class CIrrOdeGeomRay;     /**< foreward declaration of CIrrOdeGeomRay    */
class IIrrOdeStepMotor;   /**< foreware declaration of IIrrOdeStepMotor  */
class IIrrOdeEventQueue;  /**< foreward declaration of IIrrOdeEventQueue */
class IIrrOdeMotor;       /**< foreward declaration of IIrrOdeMotor      */

/**
 * @class CIrrOdeBody
 * This class encapsulates the ODE body. Bodies are dynamic objects in ODE.
 */
class CIrrOdeBody : public CIrrOdeDampable {
  protected:
    u32 m_iBodyId;                          /**< this body's ODE dBodyID */
    u32 m_iMass;                            /**< this body's ODE dMass */
    CIrrOdeWorld *m_pWorld;                 /**< this body's world */
    CIrrOdeBody *m_pParentBody;             /**< this body's parent body */
    list<CIrrOdeGeom *> m_pGeoms;           /**< this body's geoms */
    list<CIrrOdeJoint *> m_pJoints;         /**< this body's joints */
    list<CIrrOdeBody *> m_pChildBodies;     /**< this body's child bodies connected by joints */
    list<IIrrOdeStepMotor *> m_lStepMotors; /**< list of all step motors attached to the body */
    list<IIrrOdeMotor *> m_lMotors;         /**< list of all motors and servos attached to the body */
    bool m_bEnabled,                        /**< is this body active or not? */
         m_bParamMaster,                    /**< is this body the parameter master, i.e. does it set the body parameters of other bodies with the same ODE classname? */
         m_bCollision,                      /**< did this body collide in the last simulation step? */
         m_bDampingChanged,                 /**< was one of the damping parameters changed since the last simulation step? */
         m_bFastMoving,                     /**< is this a fast moving body? */
				 m_bUpdateGraphics,                 /**< is the graphical representation of this ODE body updated? */
         m_bFiniteRotationMode;             /**< the finite rotation mode flag */
    s32 m_iGravityMode,                     /**< the gravity mode */
        m_iCollisionMaterial;               /**< index of the material if the body collided */
    CIrrOdeGeom *m_pTouching;               /**< the geom the body is touching */
    CIrrOdeGeomRay *m_pRay;                 /**< the ray for a fast moving object */

    IIrrOdeEventQueue *m_pQueue;            /**< the event queue used to post all kinds of events */

    vector3df m_vLinear,              /**< linear velocity of the body. Is updated when calling "getLinearVelocity" */
              m_vAngular,             /**< angular velocity of the body. Is updated when calling "getAngularVelocity" */
              m_vFiniteRotationAxis,  /**< the finite rotation mode axis */
              m_vCollisionPoint;      /**< point of the collision if the body collided */

  public:
    CIrrOdeBody(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                const vector3df &position=core::vector3df(0,0,0),
		            const vector3df &rotation = core::vector3df(0,0,0),
		            const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeBody();

    /**
     * get the body's ODE ID
     * @return the body's dBodyID
     */
    u32 getBodyId();

    virtual void addChild(ISceneNode *pChild);

    virtual void setLinearDamping(f32 fScale);                /**< set the linear damping of the body */
    virtual f32 getLinearDamping();                           /**< get the linear damping of the body */
    virtual void setAngularDamping(f32 fScale);               /**< set the angular damping of the body */
    virtual f32 getAngularDamping();                          /**< get the angular damping of the body */
    virtual void setLinearDampingThreshold(f32 fThreshold);   /**< set the linear damping threshold of the body */
    virtual f32 getLinearDampingThreshold();                  /**< get the linear damping threshold of the body */
    virtual void setAngularDampingThreshold(f32 fThreshold);  /**< set the angular damping threshold of the body */
    virtual f32 getAngularDampingThreshold();                 /**< get the angular damping threshold of the body */
    virtual void setMaxAngularSpeed(f32 fMaxSpeed);           /**< set the maximum angular speed of the body */
    virtual f32 getMaxAngularSpeed();                         /**< get the maximum angular speed of the body */

    virtual void setAutoDisableFlag(int iFlag);                  /**< enable / disable the auto disable feature */
    virtual int getAutoDisableFlag();                            /**< is the auto disable feature enabled? */
    virtual void setAutoDisableLinearThreshold(f32 fThreshold);  /**< set auto disable linear threshold */
    virtual f32 getAutoDisableLinearThreshold();                 /**< get auto disable linear threshold */
    virtual void setAutoDisableAngularThreshold(f32 fThreshold); /**< set auto disable angular threshold */
    virtual f32 getAutoDisableAngularThreshold();                /**< get auto disable angular threshold */
    virtual void setAutoDisableSteps(int iSteps);                /**< set auto disable steps */
    virtual int getAutoDisableSteps();                           /**< get auto disable steps */
    virtual void setAutoDisableTime(f32 fTime);                  /**< set auto disable time */
    virtual f32 getAutoDisableTime();                            /**< get auto disable time */

    virtual void initPhysics();  /**< init the body's physics */

    void setFiniteRotationMode(bool bFlag);        /**< set finite rotation mode */
    bool getFiniteRotationMode();                  /**< get finite rotation mode */
    void setFiniteRotationAxis(vector3df &pAxis);  /**< set finite rotation axis */
    vector3df &getFiniteRotationAxis();            /**< get finite rotation axis */
    CIrrOdeWorld *getWorld();                      /**< get the assiciated CIrrOdeWorld object */
    void setGravityMode(int iMode);                /**< set the gravity mode */
    int getGravityMode();                          /**< get the gravity mode */
    CIrrOdeGeom *getFirstGeom();                   /**< get the body's first geom */
    CIrrOdeGeom *getNextGeom(CIrrOdeGeom *pPrev);  /**< get the body's next geom */
    void addGeom(CIrrOdeGeom *pGeom);              /**< add a geom to the body */
    void addJoint(CIrrOdeJoint *pJoint);           /**< add a joint to the body */

    vector3df getPointVel(vector3df vPos);         /**< get the velocity at a specific position */
    vector3df getRelPointVel(vector3df vPos);      /**< get the relative velocity at a specific position */
    vector3df getRelPointPos(vector3df vPos);      /**< get the relative position of a specific position */

    void removeGeom(CIrrOdeGeom *pGeom);  /**< remove a geom from the body */

    virtual void render();  /**< render the body (for debugging) */
    virtual void OnRegisterSceneNode();

    s32 getID();  /**< get the body's ID */

    ESCENE_NODE_TYPE getType() const;    /**< get the body's object type */
    const char *getTypeName() const;     /**< get the body's object type name */
    bool isEnabled();                    /**< is the body enabled? */
    void frameUpdate();                  /**< called each frame to check for activation changes */

    void addForce(vector3df force);                          /**< add a force to the body */
    void addForceAtPosition(vector3df pos, vector3df force); /**< add a force at a specific position to the body */
    void addTorque(vector3df torque);                        /**< add a torque to the body */
    void setTorque(vector3df torque);                        /**< set the body's torque */
    void setLinearVelocity(vector3df newVelocity);           /**< set the body's linear velocity */
    void setAngularVelocity(vector3df newVelocity);          /**< set the body's angular velocity */
    vector3df &getLinearVelocity();                          /**< get the body's linear velocity */
    vector3df &getAngularVelocity();                         /**< get the body's angular velocity */

    virtual void setPosition(const vector3df &newPos);  /**< set the body's position */
    virtual void setRotation(const vector3df &newRot);  /**< set the body's rotation */

    CIrrOdeBody *getParentBody();  /**< get the parent body */

    virtual const wchar_t *getTypeName();  /**< get the object type name */

    void addChildBody(CIrrOdeBody *pBody);  /**< add a child body */

    list<CIrrOdeJoint *> getJoints();  /**< get a list of the body's joints */

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

    void setParamMaster(bool b);  /**< set this body to be the parameter master for all bodies of the same OdeClassName */
    bool isParamMaster();         /**< is this body the parameter master? */

    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true); /**< copy the parameters to another IrrOdeSceneNode */
    virtual void removeFromPhysics(); /**< remove the body from physics */
    void doRemoveFromPhysics();

    void addMass(u32 iMassId);        /**< add a geom's mass to the body */
    void setEnabled(bool bEnabled);   /**< enable or disable the body */

    void setCollision(bool b);
    bool getCollision();

    void setIsTouching(CIrrOdeGeom *pTouch);
    CIrrOdeGeom *getTouched();

    void setNodePosition(vector3df newPos);           /**< update the node's position without affecting ODE */
    void setNodeRotation(vector3df newRot);           /**< update the node's rotation without affecting ODE */
    void setNodeLinearVelocity(vector3df newVel);     /**< update the node's linear velocity wihout affecing ODE */
    void setNodeAngularVelocity(vector3df newVel);    /**< update the node's angular velocity without affecting ODE */

    vector3df getNodeLinearVelocity();    /**< get the node's "linear velocity" attribute wihtout querying ODE */
    vector3df getNodeAngularVelocity();   /**< get the node's "angular velocity" attribute wihtout querying ODE */

    void setNodeLinearDamping(f32 fScale);  /**< set the node's "linear damping" attribute without affecting ODE */
    void setNodeAngularDamping(f32 fScale); /**< set the node's "angular damping" attribute without affecting ODE */

    f32 getNodeLinearDamping();
    f32 getNodeAngularDamping();

    void setCollisionPoint(vector3df vPoint);   /**< set the point of the collision */
    vector3df getCollisionPoint();              /**< get the point of the collision */

    void setCollisionMaterial(s32 iIdx);    /**< set the index of the collision material */
    s32 getCollisionMaterial();             /**< get the index of the collision material */

    /**
     * This method shows whether or not the damping has changed since the last step.
     * Useful for the (upcoming) IrrOdeNet and IrrOdeRecorder
     * @return has the damping changed since the last step
     */
    bool dampingChanged();

    /**
     * Use this method to mark a body as fast moving, e.g. bullets. If a body is marked as "fast moving"
     * a ray will be cast after each step from the old position to the new one so that a detection is done
     * to find out whether or not the body has passed another body in the last step
     * @param b fast moving flag
     */

    void setIsFastMoving(bool b);
    /**
     * query the fast moving flag
     * @return the fast moving flag
     */
    bool isFastMoving();

    /**
     * This is a callback method that is used for the "fast moving" ray. In this method the ray's position
     * and target are updated
     */
    void bodyMoved(vector3df newPos);

    /**
     * add a step motor
     * @param p the step motor to add
     */
    void addStepMotor(IIrrOdeStepMotor *p);

    /**
     * add a motor or servo
     * @param p the motor/servo to add
     */
    void addMotor(IIrrOdeMotor *p);

    /**
     * remove a step motor
     * @param p the step motor to remove
     */
    void removeStepMotor(IIrrOdeStepMotor *p);

    void setOdeRotation(vector3df vRot);

    vector3df getPosRelPoint(vector3df vPos);
    vector3df getVectorFromWorld(vector3df vPos);

    void getOdeRotation(vector3df &cRot);

    /**
     * Is the graphical representation of this body updated when the physics body moves? Can be false
     * for "dummy" bodies that are used if more than joint joints connect two bodies (e.g. hinge and slider
     * joints to attach a wheel to a car
     */
		bool updateGraphics() { return m_bUpdateGraphics; }

		/**
		 * Set this value to "false" if you don't want to update the graphical representation of an ODE body.
		 * Parameter can't be changed after "initPhysics" was called.
		 */
		void setUpdateGraphics(bool b) { if (!m_bPhysicsInitialized) m_bUpdateGraphics=b; }

		/**
		 * Get a list of all child bodies attached to this one
		 * @return list of all child bodies attached to this one
		 */
    list<CIrrOdeBody *> &getChildBodies();

    CIrrOdeGeom *getGeomFromName(const c8 *sName);
    CIrrOdeJoint *getJointFromName(const c8 *sName);
    CIrrOdeBody *getChildBodyFromName(const c8 *sName);
    IIrrOdeStepMotor *getStepMotorFromName(const c8 *sName);
    IIrrOdeMotor *getMotorFromName(const c8 *sName);
};

} //namespace ode
} //namespace irr

#endif
