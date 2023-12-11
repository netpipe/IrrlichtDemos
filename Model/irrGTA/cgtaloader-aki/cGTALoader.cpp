#include "cGTALoader.h"

cGTALoader::cGTALoader(IrrlichtDevice* dvc){
  irrDevice = dvc;
  sceneManager = irrDevice->getSceneManager();
  CDFFMeshFileLoader* dffLoader = new CDFFMeshFileLoader(irrDevice);
  sceneManager->addExternalMeshLoader(dffLoader);
}

datMesh cGTALoader::parse3(stringc s){
  datMesh neu;
  int p;
  /*! GTA3
    inst
      id, name, x, y, z, sx, sy, sz, rx, ry, rz, rw
    end
  */
  // get ID
  p = s.findFirst(',');
  s[p] = '\0';
  stringc ID = s.c_str();
  ID.trim();
  s = &s[p + 1];
  neu.id = atoi(ID.c_str());
  // get Name
  p = s.findFirst(',');
  s[p] = '\0';
  neu.file = s.c_str();
  neu.file.trim();
  neu.file += ".dff";
  s = &s[p + 1];
  // get posX
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pX = s.c_str();
  pX.trim();
  s = &s[p + 1];
  neu.pos.X = atof(pX.c_str());
  // get posY
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pY = s.c_str();
  pY.trim();
  s = &s[p + 1];
  neu.pos.Y = atof(pY.c_str());
  // get posZ
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pZ = s.c_str();
  pZ.trim();
  s = &s[p + 1];
  neu.pos.Z = atof(pZ.c_str());
  // get sclX
  p = s.findFirst(',');
  s[p] = '\0';
  stringc sX = s.c_str();
  sX.trim();
  s = &s[p + 1];
  neu.scl.X = atof(sX.c_str());
  // get sclY
  p = s.findFirst(',');
  s[p] = '\0';
  stringc sY = s.c_str();
  sY.trim();
  s = &s[p + 1];
  neu.scl.Y = atof(sY.c_str());
  // get sclZ
  p = s.findFirst(',');
  s[p] = '\0';
  stringc sZ = s.c_str();
  sZ.trim();
  s = &s[p + 1];
  neu.scl.Z = atof(sZ.c_str());
  // get rotX
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rX = s.c_str();
  rX.trim();
  s = &s[p + 1];
  neu.rot.X = atof(rX.c_str());
  // get rotY
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rY = s.c_str();
  rY.trim();
  s = &s[p + 1];
  neu.rot.Y = atof(rY.c_str());
  // get rotZ
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rZ = s.c_str();
  rZ.trim();
  s = &s[p + 1];
  neu.rot.Z = atof(rZ.c_str());

  return neu;
}
datMesh cGTALoader::parseVC(stringc s){
  datMesh neu;
  int p;
  /*! GTAVC
    inst
      id, name, int, x, y, z, sx, sy, sz, rx, ry, rz, rw
    end
  */
  // get ID
  p = s.findFirst(',');
  s[p] = '\0';
  stringc ID = s.c_str();
  ID.trim();
  s = &s[p + 1];
  neu.id = atoi(ID.c_str());
  // get Name
  p = s.findFirst(',');
  s[p] = '\0';
  neu.file = s.c_str();
  neu.file.trim();
  neu.file += ".dff";
  s = &s[p + 1];
  // get Interior Number
  p = s.findFirst(',');
  s = &s[p + 1];
  // get posX
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pX = s.c_str();
  pX.trim();
  s = &s[p + 1];
  neu.pos.X = atof(pX.c_str());
  // get posY
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pY = s.c_str();
  pY.trim();
  s = &s[p + 1];
  neu.pos.Y = atof(pY.c_str());
  // get posZ
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pZ = s.c_str();
  pZ.trim();
  s = &s[p + 1];
  neu.pos.Z = atof(pZ.c_str());
  // get sclX
  p = s.findFirst(',');
  s[p] = '\0';
  stringc sX = s.c_str();
  sX.trim();
  s = &s[p + 1];
  neu.scl.X = atof(sX.c_str());
  // get sclY
  p = s.findFirst(',');
  s[p] = '\0';
  stringc sY = s.c_str();
  sY.trim();
  s = &s[p + 1];
  neu.scl.Y = atof(sY.c_str());
  // get sclZ
  p = s.findFirst(',');
  s[p] = '\0';
  stringc sZ = s.c_str();
  sZ.trim();
  s = &s[p + 1];
  neu.scl.Z = atof(sZ.c_str());
  // get rotX
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rX = s.c_str();
  rX.trim();
  s = &s[p + 1];
  neu.rot.X = atof(rX.c_str());
  // get rotY
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rY = s.c_str();
  rY.trim();
  s = &s[p + 1];
  neu.rot.Y = atof(rY.c_str());
  // get rotZ
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rZ = s.c_str();
  rZ.trim();
  s = &s[p + 1];
  neu.rot.Z = atof(rZ.c_str());

  return neu;
}
datMesh cGTALoader::parseSA(stringc s){
  datMesh neu;
  int p;
  /*! GTASA
    inst
      id, name, int, x, y, z, rx, ry, rz, rw, lod
    end
  */
  // get ID
  p = s.findFirst(',');
  s[p] = '\0';
  stringc ID = s.c_str();
  ID.trim();
  s = &s[p + 1];
  neu.id = atoi(ID.c_str());
  // get Name
  p = s.findFirst(',');
  s[p] = '\0';
  neu.file = s.c_str();
  neu.file.trim();
  neu.file += ".dff";
  s = &s[p + 1];
  // get Interior Number
  p = s.findFirst(',');
  s = &s[p + 1];
  // get posX
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pX = s.c_str();
  pX.trim();
  s = &s[p + 1];
  neu.pos.X = atof(pX.c_str());
  // get posY
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pY = s.c_str();
  pY.trim();
  s = &s[p + 1];
  neu.pos.Y = atof(pY.c_str());
  // get posZ
  p = s.findFirst(',');
  s[p] = '\0';
  stringc pZ = s.c_str();
  pZ.trim();
  s = &s[p + 1];
  neu.pos.Z = atof(pZ.c_str());
  // get rotX
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rX = s.c_str();
  rX.trim();
  s = &s[p + 1];
  neu.rot.X = atof(rX.c_str());
  // get rotY
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rY = s.c_str();
  rY.trim();
  s = &s[p + 1];
  neu.rot.Y = atof(rY.c_str());
  // get rotZ
  p = s.findFirst(',');
  s[p] = '\0';
  stringc rZ = s.c_str();
  rZ.trim();
  s = &s[p + 1];
  neu.rot.Z = atof(rZ.c_str());
  // get sclX (always 1,1,1)
  neu.scl = vector3df(1,1,1);

  return neu;
}

ISceneNode* cGTALoader::loadMap(char* iplFile, gtaVersion v, bool asOctTree){
  array<datMesh> lstMeshes;
  char inp[1024];
  bool inINST = false;
  if(FILE* fTMP = fopen(iplFile, "r")){
    while(!feof(fTMP)){
      if(fgets(inp, 1024, fTMP) != NULL){
        stringc test = inp;
        test.trim();
        if(!inINST){
          if(test.equals_ignore_case("inst")) inINST = true;
        }else{
          if(test.equals_ignore_case("end")) break;
          datMesh neu;
          switch(v){
            case gta3:
              neu = parse3(test);
              break;
            case gtaVC:
              neu = parseVC(test);
              break;
            case gtaSA:
              neu = parseSA(test);
              break;
          }
          lstMeshes.push_back(neu);
        }
      }
    }
    fclose(fTMP);
  }
  ISceneNode* root = sceneManager->addEmptySceneNode();
  for(u32 t = 0; t < lstMeshes.size(); ++t)
    loadMesh(lstMeshes[t], root, asOctTree);

  return root;
}

bool cGTALoader::loadMesh(datMesh mesh, ISceneNode* root, bool asOctTree){
  IAnimatedMesh* m = sceneManager->getMesh(mesh.file.c_str());
  if(!m) return false;
  ISceneNode* node;
  if(asOctTree){
    node = sceneManager->addOctTreeSceneNode(m, root, mesh.id);
  }else{
    node = sceneManager->addAnimatedMeshSceneNode(m, root, mesh.id);
  }
  if(!node) return false;
  node->setPosition(mesh.pos);
  node->setRotation(mesh.rot);
  node->setScale(mesh.scl);

  return true;
}
