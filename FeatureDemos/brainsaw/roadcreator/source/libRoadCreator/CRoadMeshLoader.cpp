  #include <CRoadMeshLoader.h>
  #include <CRoadLoader.h>

scene::IAnimatedMesh *CRoadMeshLoader::createMesh(io::IReadFile *file) {
  core::stringc sName=file->getFileName();

  CRoadLoader *pLoader=new CRoadLoader(m_pDevice);
  pLoader->loadRoad(sName);

  return pLoader->createMesh();
}

#ifdef _IRREDIT_PLUGIN
  bool CRoadMeshLoader::isALoadableFileExtension (const c8 *filename) const {
    const c8 *s=strrchr(filename,'.');
    return s!=NULL && strcmp(s,".road")==0;
  }
#else
  bool CRoadMeshLoader::isALoadableFileExtension (const io::path &filename) const {
    return core::hasFileExtension(filename,"Road","road");
  }
#endif
