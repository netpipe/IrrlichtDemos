#ifndef C_LowSpeedTurn_H
#define C_LowSpeedTurn_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "../../cMeshTool.h"

using namespace OpenSteer;

class LowSpeedTurn : public SimpleVehicle, public irr::scene::ISceneNode{
  private:
    datMesh Mesh;
    datMesh Trail;
    irr::core::aabbox3df Box;
    irr::video::SMaterial Material;

  public:
    static float startX;
    static float startSpeed;

    // constructor
    LowSpeedTurn(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
    // reset state
    void reset();
    static void resetStarts();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);
    // constant steering force
    Vec3 steering();
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


#endif // C_LowSpeedTurn_H
