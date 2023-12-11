
#ifndef _CLODTREENODE_H_INCLUDED_
#define _CLODTREENODE_H_INCLUDED_

#include <stdlib.h>
#include "irrlicht.h"
#include <sqlite3.h>


using namespace irr;
using namespace core;
using namespace scene;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  //printf("\n");
  return 0;
}


struct LODMARKER
{
    u32                 pid;   // position id          (pid)
    u32                 uid;   // unique instance id   (uid)
    u32                 oid;   // object id            (oid)
    u32                 f;     // flags                (f)
    core::vector3df     p;     // position             (px py pz)
    core::vector3df     r;     // rotation             (rx ry rz)
    core::vector3df     s;     // scale                (sx sy sz)
    u32                 depth; // depth of this object (not in db)
};

class LODTreeSceneNode : public ISceneNode
{
private:
    int Level;                             // current depth level
    int Depth;                             // max depth
    f32 DrawDistance;                      // max view distance for each level
    u32 pid;                               // position id
    core::array<scene::ISceneNode*> arr;   // array of nodes
    core::array<scene::ISceneNode*> stack; // stack of objects
    core::aabbox3df   bbox;                // this LOD's bounding box
    LODTreeSceneNode *children[4];
    LODTreeSceneNode *root;
    ISceneManager    *smgr;
    sqlite3          *db;
    bool             DBConnected;
    video::SMaterial material;             // for debug

    void _doChildren();          // tests for existence of children nodes


public:

    LODTreeSceneNode( ISceneNode* parent, ISceneManager* mgr,
                      s32 id, vector3df pos = vector3df(0,0,0),
                      LODTreeSceneNode* root =0,
                      int iDepth = 9, int iLevel = 0,
                      f32 drawdistance = 20000);

    void setDebugDataVisible( bool visible );

    void OnPreRender();    // decides if children are visible
   // void OnPostRender();   // meh
    void render();         // render this scene node and all children

    bool addNewObject(LODMARKER *l); // adds an object to the database and tree

    //  writeObject - sets an objects position, rotation, scale, etc.
    //       all = false : does this update all nodes with this uid?
    //    offset = true  : does the update use relative distance?
    bool writeObject      (LODMARKER *l,
                           bool all = false, bool offset = true);

    // removeObject - removes uid from the database
    bool removeObject      (LODMARKER *l, bool all = false);

    bool isConnected() const;    // returns true if db is connected
    bool checkChildren();        // checks if childred are still in range

    bool refresh();        // refresh this node and all children
    bool connect();        // makes sure the database is connected
    bool destroy();        // destroys this node and all children

    const core::aabbox3d<f32>& getBoundingBox() const;
};

#endif // _CLODTREENODE_H_INCLUDED_
