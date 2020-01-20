#include <irrlicht.h>

namespace irr {
namespace ode {

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef _C_IRR_ODE_SCENE_NODE_FACTORY
  #define _C_IRR_ODE_SCENE_NODE_FACTORY

class CIrrOdeSceneNodeFactory : public ISceneNodeFactory {
  private:
    array<stringc> m_cNames;
    array<ESCENE_NODE_TYPE> m_cTypes;
    ISceneManager *m_pManager;

  public:
    CIrrOdeSceneNodeFactory(ISceneManager *pManager);
    virtual ISceneNode *addSceneNode (ESCENE_NODE_TYPE type, ISceneNode *parent=0);
    virtual ISceneNode *addSceneNode (const c8 *typeName, ISceneNode *parent=0);
    virtual u32 getCreatableSceneNodeTypeCount() const;
    virtual ESCENE_NODE_TYPE getCreateableSceneNodeType (u32 idx) const;
    virtual const c8 *getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const;
    virtual const c8 *getCreateableSceneNodeTypeName(u32 idx) const;
    virtual ~CIrrOdeSceneNodeFactory ();
};

} //namespace ode
} //namespace irr

#endif

