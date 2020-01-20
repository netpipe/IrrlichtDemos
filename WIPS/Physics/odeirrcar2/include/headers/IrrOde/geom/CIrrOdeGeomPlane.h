#ifndef _IRR_ODE_GEOM_PLANE
  #define _IRR_ODE_GEOM_PLANE

  #include <geom/CIrrOdeGeom.h>

namespace irr {
namespace ode {

const int IRR_ODE_GEOM_PLANE_ID=MAKE_IRR_ID('i','o','g','p');
const wchar_t IRR_ODE_GEOM_PLANE_NAME[0xFF]=L"CIrrOdeGeomPlane";

class CIrrOdeGeomPlane : public CIrrOdeGeom {
  protected:
    f32 m_fA,m_fB,m_fC,m_fD;

  public:
    CIrrOdeGeomPlane(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                     const vector3df &position=core::vector3df(0,0,0),
		                 const vector3df &rotation = core::vector3df(0,0,0),
		                 const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    void setParams(f32 fA, f32 fB, f32 fC, f32 fD);
    virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual s32 getID() const;
    virtual ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

    virtual void setMassTotal(f32 fMass);

  	virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
  	virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
};

} //namespace ode
} //namespace irr

#endif
