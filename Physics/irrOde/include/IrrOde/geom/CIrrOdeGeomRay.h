#ifndef _IRR_ODE_RAY
  #define _IRR_ODE_RAY

  #include <irrlicht.h>
  #include <geom/CIrrOdeGeom.h>

namespace irr {
namespace ode {

const int IRR_ODE_GEOM_RAY_ID=MAKE_IRR_ID('i','o','g','r');
const wchar_t IRR_ODE_GEOM_RAY_NAME[0xFF]=L"CIrrOdeGeomRay";

class CIrrOdeBody;

class CIrrOdeGeomRay : public CIrrOdeGeom {
  protected:
    f32 m_fLength;          /**< length of the ray */
    vector3df m_pPos,       /**< position of the ray */
              m_pDir;       /**< direction of the ray */
    bool m_bBackfaceCull;   /**< do backface culling for ray hits */
    CIrrOdeBody *m_pBody;

  public:
    CIrrOdeGeomRay(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                   const vector3df &position=core::vector3df(0,0,0),
		               const vector3df &rotation = core::vector3df(0,0,0),
		               const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeGeomRay();

    void set(vector3df pos, vector3df dir, f32 fLength);

    virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual ESCENE_NODE_TYPE getType() const;
    virtual s32 getID() const;
    virtual const wchar_t *getTypeName();

    void bodyHit(CIrrOdeBody *pBody);
    void setAttachedBody(CIrrOdeBody *pBody);
    CIrrOdeBody *getAttachedBody();

    virtual void setMassTotal(f32 fMass);

  	virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
  	virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    void setBackfaceCull(bool b);
    bool getBackfaceCull();
};

} //namespace ode
} //namespace irr

#endif
