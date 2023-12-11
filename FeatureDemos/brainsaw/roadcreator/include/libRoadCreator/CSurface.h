#ifndef _C_SURFACE
  #define _C_SURFACE

  #include <irrlicht.h>

using namespace irr;

#define _SURFACE_NUMBER_OF_BUFFERS 2

class CTextureParameters;

class CSurface {
  protected:
    bool m_bVisible;

    core::vector3df m_cMinPos,
                    m_cMaxPos;
    core::vector2df m_cTexRepeat;
    f32 m_fFenceHeight;

    scene::IMeshBuffer *m_pBuffers[_SURFACE_NUMBER_OF_BUFFERS];  /**<! Meshbuffer for ground and fence */

    CTextureParameters *m_pParams[_SURFACE_NUMBER_OF_BUFFERS]; /**<! Texture Parameters */

    IrrlichtDevice *m_pDevice;
    video::IVideoDriver *m_pDrv;
    io::IFileSystem *m_pFs;

  public:
    CSurface(IrrlichtDevice *pDevice, CTextureParameters *pInitParam);
    virtual ~CSurface();

    virtual void recalcMeshBuffer();

    virtual u32 getTextureCount();
    virtual CTextureParameters *getTextureParameters(u32 i);
    virtual scene::IMeshBuffer *getMeshBuffer(u32 i);
    virtual void render();
    virtual void save(io::IAttributes *out);
    virtual void load(io::IAttributes *in );

    void setCorners(const core::vector3df &cMinPos, const core::vector3df &cMaxPos);

    void setFenceHeight(f32 f) {
      m_fFenceHeight=f;
    }

    const core::vector3df &getMinPos() { return m_cMinPos; }
    const core::vector3df &getMaxPos() { return m_cMaxPos; }

    f32 getFenceHeight() { return m_fFenceHeight; }

    bool isVisible() { return m_bVisible; }
    void setVisible(bool b) { m_bVisible=b; }

    void setTextureRepeat(core::vector2df &v) { m_cTexRepeat=v; }
    const core::vector2df &getTextureRepeat() { return m_cTexRepeat; }
};

#endif

