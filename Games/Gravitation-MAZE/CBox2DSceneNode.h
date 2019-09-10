#include <irrlicht.h>
#include "CBox2DManager.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __CBOX2DSCENENODE
  #define __CBOX2DSCENENODE

class CBox2DSceneNode : public ISceneNode {
  protected:
    IMeshBuffer *m_pBuffer,*m_pBox2DBuffer;
    SMaterial    m_cMaterial;
    IVideoDriver *m_pDrv;
    ISceneManager *m_pManager;
    aabbox3d<f32> m_cBoundingBox;
    vector3df m_cPosition;
    bool m_bDrawBox2D,m_bDrawBoundingBox;

    char c_sTypeName[0xFF];

  public:
    static const char m_cTypeName[0xFF];

    CBox2DSceneNode(ISceneNode *pParent, ISceneManager *pManager, u32 id);
    ~CBox2DSceneNode();
    virtual void render();
    const core::aabbox3d<f32> &getBoundingBox() const;
    void setMeshBuffer(IMeshBuffer *pBuffer);
    void setBox2DBuffer(IMeshBuffer *pBuffer);
    unsigned int getVertexCount();
    virtual void OnRegisterSceneNode();
    virtual const s32 getMaterialCount();
    virtual video::SMaterial& getMaterial(s32 i);
    void 	setMaterialTexture(u32 iTextureLayer, ITexture *pTexture);
    void setMaterialFlag(E_MATERIAL_FLAG flag, bool bNewValue);
    void setDrawBox2D(bool b);
    void setDrawBoundingBox(bool b);
    ESCENE_NODE_TYPE getType() const;
    const char *getTypeName() const;
    virtual void setPosition(const vector3df &newPos);
    virtual void setScale(const vector3df &scale);
    virtual void setRotation(const vector3df &rotation);
    IMeshBuffer *getBox2DBuffer() { return m_pBox2DBuffer; }
};

#endif
