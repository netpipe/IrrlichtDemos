#include "../../../../config.h"
#ifdef OPENSTEER
#ifndef CONETURNING_H
#define CONETURNING_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "../../cMeshTool.h"

using namespace OpenSteer;

class OneTurning : public SimpleVehicle, public irr::scene::ISceneNode{
  private:
    datMesh Trail;
    irr::video::SMaterial Material;
    irr::core::aabbox3df BBox;

  public:
    datMesh Mesh;

    // constructor
    OneTurning(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
    // reset state
    void reset();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);
    // draw this character/vehicle into the scene
    void draw();

    void recordTrailVertex(const float currentTime, const Vec3& pos);
    void clearTrailHistory();

    //! SceneNode stuff
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    virtual irr::u32 getMaterialCount();
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
    virtual void setMaterial(irr::video::SMaterial newMaterial);

};


#endif // CONETURNING_H
#endif
