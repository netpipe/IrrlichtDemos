#ifndef _C_IRR_ODE_GEOM
  #define _C_IRR_ODE_GEOM

  #include <CIrrOdeSceneNode.h>
  #include <CIrrOdeSurfaceParameters.h>

namespace irr {
namespace ode {

const double GRAD_PI =180.0 / 3.1415926535897932384626433832795;
const double GRAD_PI2=3.1415926535897932384626433832795 / 180.0;

const int IRR_ODE_GEOM_ID=MAKE_IRR_ID('i','o','g','m');
const wchar_t IRR_ODE_GEOM_NAME[0xFF]=L"CIrrOdeGeom";

class CIrrOdeBody;
class CIrrOdeWorld;
class CIrrOdeSpace;

/**
 * @class CIrrOdeGeom
 * This is the base class for all ODE geoms (like sphere, box and trimesh).
 */
class CIrrOdeGeom : public CIrrOdeSceneNode {
  protected:
    u32 m_iGeomId;                /**< the ODE geom id */
    u32 m_iMass,                  /**< the ODE mass    */
			  m_iCollisionGroup;				/**< the collision group. Two geoms with the same group!=0 won't collide */
    f32 m_fMass;                  /**< the mass */
    bool m_bCollide,              /**< does this geom collide with other geoms? */
         m_bSurfaceMaster;
    CIrrOdeBody *m_pBody;         /**< the IrrODE body the geom is attached to */
    CIrrOdeWorld *m_pWorld;       /**< the IrrOde world the geom is in */
    CIrrOdeSpace *m_pSpace;       /**< the IrrOde space the geom is in */
    vector3df m_cMassTranslate;   /**< the translation of the geom's mass */
    vector3df m_cCenterOfGravity; /**< the geom's center of gravity    */
    vector3df m_cInertia1;
    vector3df m_cInertia2;
    vector3df m_cOffsetPos;
    vector3df m_cOffsetRot;

    array<CIrrOdeSurfaceParameters *> m_aParams;

  public:
    /**
     * The constructor (standard irrlicht scenenode)
     * @param parent the parent node
     * @param mgr the ISceneManager
     * @param id the ID of the node
     * @param position the position of the node
     * @param rotation the rotation of the node
     * @param scale the scale of the node
     */
    CIrrOdeGeom(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                const vector3df &position=core::vector3df(0,0,0),
		            const vector3df &rotation = core::vector3df(0,0,0),
		            const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    /** the destructor */
    virtual ~CIrrOdeGeom();

    /**
     * get the body the geom is attached to
     * @return the IrrOdeBody the geom is attached to, NULL if the geom is static (i.e. not attached to a body)
     */
    CIrrOdeBody *getBody();

    /**
     * Attach the geom to a body
     * @param pBody the body the geom will be attached to
     */
    void setBody(CIrrOdeBody *pBody);

    /**
     * Set the total mass of the geom
     * @param fMass the total mass of the geom
     */
    virtual void setMassTotal(f32 fMass)=0;

    /**
     * Get the geom's ODE dGeomID. The parameter is only necessary for static trimeshes which can contain more
     * than one ODE trimesh.
     * @param iNum the index of the geom id to return
     * @return the geom's ODE dGeomID
     */
    virtual u32 getGeomId(u32 iNum=0);

    /**
     * Get the Geom's type name
     * @return the geom's type name
     */
    virtual const wchar_t *getTypeName()=0;

    /**
     * get the number of surface parameters
     * @return the number of surface parameters
     */
    u32 getSurfaceParametersCount() const;

    /**
     * get the geom's surface parameters
     * @param iIdx only used for CIrrOdeGeomTrimesh, because it is the only geom that can have more than one
     */
    virtual CIrrOdeSurfaceParameters *getSurfaceParameters(u32 iIdx);

    /**
     * set the geom's surface parameters
     * @param pParams the params
     * @param iIdx use params for this material. Only used for CIrrOdeGeomTrimesh
     */
    void setSurfaceParameters(CIrrOdeSurfaceParameters *pParams, u32 iIdx);

    CIrrOdeWorld *getWorld();

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    /**
     * set the mass parameters
     * @param fMass the new absolute mass
     * @param c
     * @param i1
     * @param i2
     */
    void setMassParameters(f32 fMass, vector3df c, vector3df i1, vector3df i2);

		/**
		 * the "setPosition" method is overridden to make sure the IrrOdeGeom nodes always have a relative position of (0,0,0)
		 * @param newpos the new position. Will be discarded and set to (0,0,0)
		 */
		virtual void setPosition(const vector3df &newpos);

    /**
     * Translate the center of gravity to a new position
     * @param pos the new position
     */
    void setMassTranslation(vector3df pos);

    virtual void initPhysics();

    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);
    virtual void removeFromPhysics();

    void setOffsetPosition(vector3df pPos);
    void setOffsetQuaternion(vector3df pRot);

    /**
     * Set whether or not this geom collides with other geoms
     * @param b the new value
     */
    void setCollide(bool b);

    /**
     * Does this geom collide with other geoms?
     * @return true if this geom collides with other geoms
     */
    bool doesCollide();

    /**
     * Set the m_iCollisionGroup attribute. A value of zero means the geom will collide with any other geoms,
     * if the value is not equal zero it won't collide with other geoms of the same group id
     * @see m_iCollisionGroup
     */
		void setCollisionGroup(u32 i) { m_iCollisionGroup=i; }

		/**
		 * Get the m_iCollisionGroup attribute
		 * @see m_iCollisionGroup
		 */
		u32 getCollisionGroup() { return m_iCollisionGroup; }
};

} //namespace ode
} //namespace irr

#endif
