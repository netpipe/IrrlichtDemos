#ifndef _C_ROAD_MESH_LOADER
  #define _C_ROAD_MESH_LOADER

  #include <irrlicht.h>

using namespace irr;

class CRoadMeshLoader : public scene::IMeshLoader {
  protected:
    IrrlichtDevice *m_pDevice;

  public:
    CRoadMeshLoader(IrrlichtDevice *pDevice) { m_pDevice=pDevice; }

    virtual scene::IAnimatedMesh *createMesh(io::IReadFile *file);
    #ifdef _IRREDIT_PLUGIN
      virtual bool isALoadableFileExtension (const c8 *filename) const;
    #else
      virtual bool isALoadableFileExtension (const io::path &filename) const;
    #endif
    virtual ~CRoadMeshLoader() { }
};

#endif
