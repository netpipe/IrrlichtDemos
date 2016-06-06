//!
//! CQUADTREESCENENODE
//! Irrlicht scenenode for quadtree frustrum culling
//!
//!
//! By Paulo Rogério de Oliveira ( paulo_cmv@hotmail.com < MSN - EMAIL > )
//! ----------------------------
//! You can use it for any purpose, as you want, only dont remove this notice.
//! If you make any changes, improvements, or you have comments, sugestions,
//! you can report it back by mail or msn. This way i can try to improve the code.

#ifndef _CQUADTREESCENENODE_H_INCLUDED_
#define _CQUADTREESCENENODE_H_INCLUDED_

#include "stdlib.h"
#include "irrlicht.h"
using namespace irr;
using namespace scene;

class CQuadTreeSceneNode : public ISceneNode {
private:
    int Level;
    int Depth;
    bool started;
    core::array<scene::ISceneNode*> arr;
    core::aabbox3df bbox;
    CQuadTreeSceneNode* childs[4];
    video::SMaterial material;
    ISceneManager* smgr;
    int allAdded;
    void prepareNodes();
public:

    CQuadTreeSceneNode( ISceneNode* parent, ISceneManager* mgr, s32 id, int idepth = 3, int cLevel = 0 );

    void setDebugDataVisible( bool visible );

    void startAdding();

    void addNode( ISceneNode* node );

    void endAdding();

    void OnPreRender();

    void render();

    const core::aabbox3d<f32>& getBoundingBox() const;
};

#endif
