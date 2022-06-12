
#ifdef OPENSTEER
#ifndef C_Pedestrian_H
#define C_Pedestrian_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "../../cMeshTool.h"

using namespace OpenSteer;

typedef AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
typedef AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;
// How many pedestrians to create when the plugin starts first?
int const gPedestrianStartCount = 100;

class Pedestrian : public SimpleVehicle, public irr::scene::ISceneNode{
  private:
    datMesh Mesh;
    datMesh Trail;
    irr::core::aabbox3df Box;
    irr::video::SMaterial Material;

  public:
    // type for a group of Pedestrians
    typedef std::vector<Pedestrian*> groupType;
    // a pointer to this boid's interface object for the proximity database
    ProximityToken* proximityToken;
    // allocate one and share amoung instances just to save memory usage
    // (change to per-instance allocation to be more MP-safe)
    static AVGroup neighbors;
    // path to be followed by this pedestrian
    PolylineSegmentedPathwaySingleRadius* path;
    // direction for path following (upstream or downstream)
    int pathDirection;

    // constructor
    Pedestrian(ProximityDatabase& pd, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
    virtual ~Pedestrian();

    // reset state
    void reset();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);
    // draw this character/vehicle into the scene
    void draw();

    void recordTrailVertex(const float currentTime, const Vec3& pos);
    void clearTrailHistory();

    Vec3 determineCombinedSteering(const float elapsedTime);

    void newPD(ProximityDatabase& pd);

    //! SceneNode stuff
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    virtual irr::u32 getMaterialCount();
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
    virtual void setMaterial(irr::video::SMaterial newMaterial);

};

#endif // C_Pedestrian_H
#endif
