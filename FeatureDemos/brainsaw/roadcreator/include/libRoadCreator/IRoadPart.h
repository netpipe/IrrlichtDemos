#ifndef _I_ROAD_PART
  #define _I_ROAD_PART

  #include <irrlicht.h>

using namespace irr;

class CTextureParameters;

class IRoadPart {
  protected:
    s32 m_iMeshBufferToDraw;

  public:
    IRoadPart(video::IVideoDriver *pDrv) {
      m_iMeshBufferToDraw=-1;
    }

    virtual void recalcMeshBuffer()=0;

    virtual u32 getTextureCount()=0;
    virtual CTextureParameters *getTextureParameters(u32 i)=0;
    virtual scene::IMeshBuffer *getMeshBuffer(u32 i)=0;
    virtual void render()=0;
    virtual void save(io::IAttributes *out)=0;
    virtual void load(io::IAttributes *in )=0;
    virtual s32 getNumberOfMeshBuffers()=0;

    virtual void renderMeshBuffer(s32 iNum) {
      m_iMeshBufferToDraw=iNum;
    }
};

#endif
