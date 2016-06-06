#ifndef _C_MANAGED_SHADOW_FACTORY
  #define _C_MANAGED_SHADOW_FACTORY

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CManagedShadowFactory : public ISceneNodeFactory {
  protected:
    ISceneManager *m_pManager;

  public:
    CManagedShadowFactory(ISceneManager *pManager);
    virtual ~CManagedShadowFactory();
    virtual ISceneNode *addSceneNode (ESCENE_NODE_TYPE type, ISceneNode *parent=0);
    virtual ISceneNode *addSceneNode (const c8 *typeName, ISceneNode *parent=0);
    virtual u32 getCreatableSceneNodeTypeCount() const;
    virtual ESCENE_NODE_TYPE getCreateableSceneNodeType (u32 idx) const;
    virtual const c8 *getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const;
    virtual const c8 *getCreateableSceneNodeTypeName(u32 idx) const;
};

#endif
