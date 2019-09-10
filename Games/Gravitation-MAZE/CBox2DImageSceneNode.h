#include <irrlicht.h>
#include "CBox2DSceneNode.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __CBOX2DIMAGEMESHSCENENODE
  #define __CBOX2DIMAGEMESHSCENENODE

class CBox2DImageSceneNode : public CBox2DSceneNode {
  private:
    IMeshBuffer *m_pHorizontalBuffer;
    SMaterial m_cHorizontalMaterial;

  public:
    CBox2DImageSceneNode(ISceneNode *pParent, ISceneManager *pManager, u32 id);
    ~CBox2DImageSceneNode();
    void setHorizontalMeshBuffer(IMeshBuffer *pBuffer);
    virtual void render();
    virtual const s32 getMaterialCount();
    virtual video::SMaterial& getMaterial(s32 i);
    void setMaterialTexture(u32 iTextureLayer, ITexture *pTexture);
    void setMaterialFlag(E_MATERIAL_FLAG flag, bool bNewValue);
    ESCENE_NODE_TYPE getType() const;
    const char *getTypeName() const;
};

#endif
