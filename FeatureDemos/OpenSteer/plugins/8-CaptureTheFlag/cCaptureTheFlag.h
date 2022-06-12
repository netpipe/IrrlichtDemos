#ifndef C_CtfBase_H
#define C_CtfBase_H

#include <irrlicht.h>
#include "../../cOpenSteerDemo.h"
#include "../../cMeshTool.h"

#define maxEnemyCount 4

using namespace OpenSteer;

typedef std::vector<SphereObstacle*> SOG;  // SphereObstacle group
typedef SOG::const_iterator SOI;           // SphereObstacle iterator
#define testOneObstacleOverlap(radius, center){              \
    float d = Vec3::distance (c, center);                    \
    float clearance = d - (r + (radius));                    \
    if (minClearance > clearance) minClearance = clearance;  \
}

class CtfBase : public SimpleVehicle, public irr::scene::ISceneNode{
  public:
    datMesh Mesh;
    datMesh Trail;
    irr::core::aabbox3df Box;
    irr::video::SMaterial Material;

    enum seekerState{running, tagged, atGoal};
    bool avoiding;
    static int obstacleCount;
    static const int maxObstacleCount;
    static SOG allObstacles;

    static void initializeObstacles();
    static void addOneObstacle();
    static void removeOneObstacle();
    float minDistanceToObstacle(const Vec3 point);
    void randomizeStartingPositionAndHeading();

    // constructor
    CtfBase(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent);
    ~CtfBase();
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

class CtfSeeker : public CtfBase{
  public:
    seekerState state;
    bool evading; // xxx store steer sub-state for anotation
    float lastRunningTime; // for auto-reset

    // constructor
    CtfSeeker(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent):CtfBase(smgr, parent){ reset(); }
    // reset state
    void reset();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);
    // is there a clear path to the goal?
    bool clearPathToGoal();

    Vec3 steeringForSeeker();
    void updateState(const float currentTime);
    void draw();
    Vec3 steerToEvadeAllDefenders();
    Vec3 XXXsteerToEvadeAllDefenders();
    void adjustObstacleAvoidanceLookAhead(const bool clearPath);

    //! SceneNode stuff
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    virtual irr::u32 getMaterialCount();
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
    virtual void setMaterial(irr::video::SMaterial newMaterial);
};

class CtfEnemy : public CtfBase{
  public:
    // constructor
    CtfEnemy(irr::scene::ISceneManager* smgr, irr::scene::ISceneNode* parent):CtfBase(smgr, parent){ reset(); }
    // reset state
    void reset();
    // per frame simulation update
    void update(const float currentTime, const float elapsedTime);

    //! SceneNode stuff
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
    virtual irr::u32 getMaterialCount();
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);
    virtual void setMaterial(irr::video::SMaterial newMaterial);
};

#endif // C_CtfBase_H
