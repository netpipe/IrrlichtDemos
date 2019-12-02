#ifndef CGTALOADER_H
#define CGTALOADER_H

#include <irrlicht.h>
#include "CDFFMeshFileLoader.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

enum gtaVersion{
  gta3 = 0,
  gtaVC,
  gtaSA
};

struct datMesh{
  s32 id;
  stringc file;
  vector3df pos;
  vector3df scl;
  vector3df rot;
};

class cGTALoader{
  private:
    ISceneManager* sceneManager;
    IrrlichtDevice* irrDevice;

    bool loadMesh(datMesh mesh, ISceneNode* root, bool asOctTree);
    datMesh parse3(stringc s);
    datMesh parseVC(stringc s);
    datMesh parseSA(stringc s);

  public:
    cGTALoader(IrrlichtDevice* dvc);
    ISceneNode* loadMap(char* iplFile, gtaVersion v, bool asOctTree = true);
};

#endif // CGTALOADER_H
