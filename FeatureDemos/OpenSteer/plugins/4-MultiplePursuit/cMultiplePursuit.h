#ifndef C_MpBase_H
#define C_MpBase_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "../../cMeshTool.h"

using namespace OpenSteer;

// This PlugIn uses two vehicle types: MpWanderer and MpPursuer.  They have
// a common base class, MpBase, which is a specialization of SimpleVehicle.
class MpBase : public SimpleVehicle, public irr::scene::ISceneNode{
  private:
    datMesh Trail;
    irr::core::aabbox3df Box;
    irr::video::SMaterial Material;

  public:
    datMesh Mesh;
    irr::video::SColor bColor;

    // constructor
    MpBase(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
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

//! The wanderer
class MpWanderer : public MpBase{
  public:
    // constructor
    MpWanderer(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
    // reset state
    void reset();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);
};
//! The pursuer
class MpPursuer : public MpBase{
  private:
    MpWanderer* wanderer;

  public:
    // constructor
    MpPursuer(MpWanderer* w, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
    // reset state
    void reset();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);
    void randomizeStartingPositionAndHeading();

};

#endif // C_MpBase_H
