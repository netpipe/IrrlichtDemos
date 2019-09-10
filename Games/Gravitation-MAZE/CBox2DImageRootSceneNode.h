#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __CBOX2DIMAGEROOTSCENENODE
  #define __CBOX2DIMAGEROOTSCENENODE

class CBox2DImageRootSceneNode : public CBox2DImageSceneNode {
  private:
    IAnimatedMesh *m_pMesh;
    array <CBox2DImageSceneNode *>m_pNodes;

  public:
    CBox2DImageRootSceneNode(ISceneNode *pParent, ISceneManager *pManager, u32 id);

    void setMesh(IAnimatedMesh *pMesh);
    void setDrawBox2D(bool b);
    void setDrawBoundingBox(bool b);
    ESCENE_NODE_TYPE getType() const;
    const char *getTypeName() const;
    void loadImageMesh(const c8 *sImageName);
    void clear();
};

#endif
