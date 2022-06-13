#include "../../../../config.h"
#ifdef OPENSTEER3
#ifndef C_TEMPLATE_H
#define C_TEMPLATE_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "../../cMeshTool.h"

using namespace OpenSteer;

typedef OpenSteer::AbstractProximityDatabase<AbstractVehicle*> ProximityDatabase;
typedef OpenSteer::AbstractTokenForProximityDatabase<AbstractVehicle*> ProximityToken;

class Boid : public SimpleVehicle, public irr::scene::ISceneNode{
  private:
    datMesh Mesh;
    datMesh Trail;
    irr::core::aabbox3df Box;
    irr::video::SMaterial Material;


  public:
    // type for a flock: an STL vector of Boid pointers
    typedef std::vector<Boid*> groupType;
    // group of all obstacles to be avoided by each Boid
    static ObstacleGroup obstacles;
    // a pointer to this boid's interface object for the proximity database
    ProximityToken* proximityToken;
    // allocate one and share amoung instances just to save memory usage
    static AVGroup neighbors;
    static float worldRadius;
    static long minNeighbors, maxNeighbors, totalNeighbors;

    // constructor
    Boid(ProximityDatabase& pd, irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
    // reset state
    void reset();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);
    // draw this character/vehicle into the scene
    void draw();

    Vec3 steerToFlock();
    void sphericalWrapAround();

    void recordTrailVertex(const float currentTime, const Vec3& pos);
    void clearTrailHistory();
    void regenerateLocalSpace(const Vec3& newVelocity, const float elapsedTime);
    void regenerateLocalSpaceForTerrainFollowing (const Vec3& newVelocity, const float /* elapsedTime */);
    void newPD(ProximityDatabase& pd);
    void annotateAvoidObstacle(const float minDistanceToCollision);

    //! SceneNode stuff
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    virtual irr::u32 getMaterialCount();
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
    virtual void setMaterial(irr::video::SMaterial newMaterial);

};


#endif // C_TEMPLATE_H
#endif
