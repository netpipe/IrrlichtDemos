#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __CBOX2DANIMATEDIMAGEMESH
  #define __CBOX2DANIMATEDIMAGEMESH

#define _TILE_SIZE_X 32
#define _TILE_SIZE_Y 32
#define _TILE_Z -30
#define _TILE_STEP 5
#define _TILE_DEPTH 60

class CBox2DAnimatedImageMesh : public SAnimatedMesh {
  private:
    int m_iTextureCount,m_iMeshCnt;

    irr::core::array<irr::core::stringc> m_aTextureNames;
    irr::core::array<u16> m_a2ndMeshIdx;
    irr::core::stringc m_sImageName;

    array<IMeshBuffer *> m_aMeshBuffers;

    bool addImageMeshBuffer(IImage *pSrc, int iStartX, int iStartY);

  public:
    CBox2DAnimatedImageMesh(const c8 *pImageName, IVideoDriver *pDriver);
};

#endif
