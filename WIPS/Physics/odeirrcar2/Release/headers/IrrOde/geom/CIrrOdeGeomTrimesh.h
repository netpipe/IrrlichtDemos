#ifndef _C_IRR_ODE_TRIMESH
  #define _C_IRR_ODE_TRIMESH

  #include <geom/CIrrOdeGeom.h>

namespace irr {
namespace ode {

const int IRR_ODE_GEOM_TRIMESH_ID=MAKE_IRR_ID('i','o','g','t');
const wchar_t IRR_ODE_GEOM_TRIMESH_NAME[0xFF]=L"CIrrOdeGeomTrimesh";

class CIrrOdeGeomTrimesh : public CIrrOdeGeom {
  protected:
    array<u32> m_aDataIds;
    array<u32> m_aGeomIds;

  public:
    CIrrOdeGeomTrimesh(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                       const vector3df &position=core::vector3df(0,0,0),
		                   const vector3df &rotation = core::vector3df(0,0,0),
		                   const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeGeomTrimesh();

    virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual s32 getID() const;
    virtual ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

    /**
     * Get the geom's ODE dGeomID
     * @param iNum the index of the geom id to return
     * @return the geom's ODE dGeomID
     */
    virtual u32 getGeomId(u32 iNum=0);

    s32 getIndexOfGeom(u32 iGeom);

    virtual void setMassTotal(f32 fMass);

  	virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
  	virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    virtual void setParent(ISceneNode *pParent);
    virtual CIrrOdeSurfaceParameters *getSurfaceParameters(u32 iIdx);
    virtual void removeFromPhysics();
};

} //namespace ode
} //namespace irr

#endif
