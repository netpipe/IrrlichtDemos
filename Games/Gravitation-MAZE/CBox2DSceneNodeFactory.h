#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __CBOX2DSCENENODEFACTORY
  #define __CBOX2DSCENENODEFACTORY

const int BOX2D_SCENE_NODE=MAKE_IRR_ID('b','2','s','n');
const int BOX2D_IMAGE_NODE=MAKE_IRR_ID('b','2','i','n');
const int BOX2D_IROOT_NODE=MAKE_IRR_ID('b','2','i','r');

const char BOX2D_SCENE_NODE_NAME[0xFF]="CBox2DSceneNode";
const char BOX2D_IMAGE_NODE_NAME[0xFF]="CBox2DImageSceneNode";
const char BOX2D_IROOT_NODE_NAME[0xFF]="CBox2DImageRootSceneNode";

class CBox2DSceneNodeFactory : public ISceneNodeFactory {
  private:
    array<stringc> m_aSceneNodeNames;
    ISceneManager *m_pManager;

  public:
    CBox2DSceneNodeFactory(ISceneManager *pManager);
    virtual ISceneNode *addSceneNode (ESCENE_NODE_TYPE type, ISceneNode *parent=0);
    virtual ISceneNode *addSceneNode (const c8 *typeName, ISceneNode *parent=0);
    virtual u32 getCreatableSceneNodeTypeCount() const;
    virtual ESCENE_NODE_TYPE getCreateableSceneNodeType (u32 idx) const;
    virtual const c8 *getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const;
    virtual const c8 *getCreateableSceneNodeTypeName (u32 idx) const;
    virtual ~CBox2DSceneNodeFactory ();
};

#endif
