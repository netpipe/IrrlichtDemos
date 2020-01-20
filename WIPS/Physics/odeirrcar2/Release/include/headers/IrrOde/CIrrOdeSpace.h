#ifndef _IRR_ODE_SPACE
  #define _IRR_ODE_SPACE

  #include <CIrrOdeSceneNode.h>

namespace irr {
namespace ode {

using namespace core;
using namespace scene;
using namespace io;

class CIrrOdeWorld;

const int IRR_ODE_SPACE_ID=MAKE_IRR_ID('i','o','s','p');
const wchar_t IRR_ODE_SPACE_NAME[0xFF]=L"CIrrOdeSpace";

enum _IRR_ODE_SPACE_TYPE {
  eIrrOdeSpaceSimple,
  eIrrOdeSpaceHash,
  eIrrOdeSpaceQuadTree
};

class CIrrOdeSpace : public CIrrOdeSceneNode {
  protected:
    u32 m_iSpaceId;
    _IRR_ODE_SPACE_TYPE m_iType;
    CIrrOdeSpace *m_pParentSpace;
    CIrrOdeWorld *m_pWorld;
    vector3df m_cCenter,m_cExtents;
    s32 m_iDepth;

  public:
    CIrrOdeSpace(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                 const vector3df &position=core::vector3df(0,0,0),
	  	           const vector3df &rotation = core::vector3df(0,0,0),
		             const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));
    CIrrOdeSpace(CIrrOdeSceneNode *pSource, ISceneNode *pNewParent, ISceneManager *pNewManager);
    ~CIrrOdeSpace();

    virtual void initPhysics();

    void setSpaceType(_IRR_ODE_SPACE_TYPE iType);
    _IRR_ODE_SPACE_TYPE getSpaceType();

    void setQuadTreeParams(vector3df cCenter, vector3df cExtents, s32 iDepth);

    u32 getSpaceId();

    s32 getID();

    ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();
    virtual void render();
    void OnRegisterSceneNode();
    void setType(_IRR_ODE_SPACE_TYPE iType);
    _IRR_ODE_SPACE_TYPE getType();

    CIrrOdeSpace *getParentSpace();
    vector3df &getCenter();
    vector3df &getExtents();
    s32 getDepth();

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);
    virtual void removeFromPhysics();
};

} //namespace ode
} //namespace irr

#endif
