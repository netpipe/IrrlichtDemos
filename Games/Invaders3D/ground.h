//Ground.h
#ifndef __GROUND_H__
#define __GROUND_H__
#include <list>
using namespace std;
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
//using namespace std;
const unsigned int MaxX = 5;
const unsigned int MaxY = 35;

class Ground {
public:
    ITriangleSelector* selector;
    Ground();
    Ground(ISceneManager* smgr,IVideoDriver* driver,unsigned tile);
    void Drop();
    void LoadLevel(unsigned short lvlArray[MaxX][MaxY]);
    void SetLevel(unsigned short lvlArray[MaxX][MaxY]);
  //  void Flip(unsigned short index,unsigned short slice,unsigned short lvlArray[MaxX][MaxY]);
    void Flip(unsigned short index,unsigned short lvlArray[MaxX][MaxY]);
    float GetFriction();
    void ZeroLevel(unsigned short lvlArray[MaxX][MaxY]);
    void SetShaders(bool set);
private: 
   IAnimatedMesh* grnd;
   ISceneManager* smgr;
   IVideoDriver* driver;
   float LevelFriction;
   bool shaders;
   unsigned tileset;
   //IAnimatedMeshSceneNode* lvl[MaxX][MaxY];
   ISceneNode* lvl[MaxX][MaxY];
};       
#endif
