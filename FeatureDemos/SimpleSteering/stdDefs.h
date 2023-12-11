#ifndef STD_DEFS_H
#define STD_DEFS_H

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


struct datNextWP{
  unsigned int ID;
  long Distanz; // long (integer) for speed
};
struct datWaypoint{
  array<datNextWP> next;
  vector3df pos;
  IBillboardSceneNode* bill; // only for debug mode (visualisation) used
};
struct datAStar{
  int idWaypoint;
  int idParent;
  long costF; // intersection cost (costG + costH)
  long costG; // complete way cost in path
  long costH; // distance to target
};
struct datEnemy{
  IAnimatedMeshSceneNode* node;
  ICameraSceneNode* cam;
  int wpLast;
  array<int> route;
};
struct datPlayer{
  ICameraSceneNode* cam;
  bool prgEnd;
  int selEnemy;
  bool useEnemyCam;
  bool showWaypoints;
  double speedEnemies;
  IBillboardSceneNode* bbDest;
  ITextSceneNode* txtSel;
};


#endif // STD_DEFS_H
