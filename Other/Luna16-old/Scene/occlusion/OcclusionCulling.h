#ifndef BXPCULLING
#define BXPCULLING

/* Occlusion culling by Burt McCauley

  Todo :
    *  loadBXPfile
    *  create BXP file easily
    *  add light info into bxp, as well as fog, entity list
    *  with LOD should be optimal
   
*/

//#include <iostream>
#include <fstream>
using namespace std;

#include "irrlicht.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

/* BXP FILE ------------------------------------------------------------------*/
struct SBXPheader
{
 u16 nbleaves; // world is composed by 25 blocs
 u32 nbmdl; // models list contains 4 models
 u16 MDLlistSize; //models list represents 365 bytes
 //leafActuel
 //fogstart
 //fogend
 //ambient
};

typedef struct SBXPleaf
{
 vector3df point1;
 vector3df point2;
 u16 nbmdls;
};

typedef struct SBXPmdl
{
 u16 id; //index of the model in models list to get filename
 vector3df pos;
 u16 angle;
 u16 upvector;
};

/* READ FOR UPDATES AS REFERENCE ---------------------------------------------*/
typedef struct Smodel
{
 u16 id; //index of the model in models list
 IAnimatedMeshSceneNode* node; // a pointer to manipulate it
 vector3df pos;
 u16 angle;
 u16 upvector;
};

typedef struct Sleaf
{
 vector3df point1;
 vector3df point2;
 core::array<Smodel*> mdl; //array of models in this leaf
};


class OcclusionCulling
{
   ISceneManager* smgr;
   core::array<char*> listMDL; //list of model filenames
   core::array<Sleaf> world; // the world is an array of leaves
   u16 currentLeaf;
   void print_stats();
   void loadLeaf(s16 n);
   void unloadLeaf(s16 n);
   Smodel* loadModel(u16 idmdl, vector3df pos, u16 angle, u16 upvector);
   Smodel* createSmodel(u16 id, vector3df pos, u16 angle, u16 upvector);
public:
   OcclusionCulling(ISceneManager* smgra, char* bxpfilename);
   void update(vector3df pos);
   ~OcclusionCulling();
   void loadBXPFile(char* file);
   void writeBXPfile(char* file);
};

#endif
