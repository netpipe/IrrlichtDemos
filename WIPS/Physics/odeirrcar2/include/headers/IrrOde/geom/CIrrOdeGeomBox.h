#ifndef _C_IRR_ODE_BOX
  #define _C_IRR_ODE_BOX

  #include <geom/CIrrOdeGeom.h>

namespace irr {
namespace ode {

const int IRR_ODE_GEOM_BOX_ID=MAKE_IRR_ID('i','o','g','b');
const wchar_t IRR_ODE_GEOM_BOX_NAME[0xFF]=L"CIrrOdeGeomBox";

class CIrrOdeGeom;

/**
 * @class CIrrOdeGeomBox
 * This is the wrapper class for the ODE box geom
 */
class CIrrOdeGeomBox : public CIrrOdeGeom {
  protected:
    f32 m_fWidth ,  /**< width of the box */
        m_fHeight,  /**< height of the box */
        m_fDepth;   /**< depth of the box */

	bool m_bUseAllMeshBuffers;		/**< use all mesh buffers for collision box? */
	array<bool> m_aUseMeshBuffer;	/**< which mesh buffers to use if not all */

  public:
    /**
     * Standard Irrlicht scenenode constructor
     */
    CIrrOdeGeomBox(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                   const vector3df &position=core::vector3df(0,0,0),
		               const vector3df &rotation = core::vector3df(0,0,0),
		               const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    /**
     * Destructor
     */
    virtual ~CIrrOdeGeomBox();

    virtual void render();
    virtual void OnRegisterSceneNode();
    virtual void initPhysics();

    virtual s32 getID() const;
    virtual ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

    /**
     * set the total mass of the box
     * @param fMass mass of the box
     */
    virtual void setMassTotal(f32 fMass);

    void setWidth(f32 fWidth);
    void setHeight(f32 fHeight);
    void setDepht(f32 fDepth);

  	virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
  	virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);
};

} //namespace ode
} //namespace irr

#endif
