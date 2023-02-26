#include <irrlicht.h>
#include <string.h>

#include "CBox2DMeshLoader.h"
#include "CBox2DAnimatedImageMesh.h"

CBox2DMeshLoader::CBox2DMeshLoader(IVideoDriver *pDriver) {
  m_pDriver=pDriver;
  printf("Box2DMeshLoader created\n");
}

CBox2DMeshLoader::~CBox2DMeshLoader() {
}

//bool CBox2DMeshLoader::isALoadableFileExtension(const irr::c8 *fileName) const {
//  const char *sExtension=strrchr(fileName,'.');
//  return !strcmp(sExtension,".png");
//}

bool CBox2DMeshLoader::isALoadableFileExtension(const io::path &fileName) const {
  const char *sExtension=strrchr(fileName.c_str(),'.');
  return !strcmp(sExtension,".png");
}

IAnimatedMesh *CBox2DMeshLoader::createMesh(io::IReadFile *file) {
  IAnimatedMesh *pMesh=NULL;
  pMesh=new CBox2DAnimatedImageMesh(file->getFileName().c_str(),m_pDriver);
  return pMesh;
}

