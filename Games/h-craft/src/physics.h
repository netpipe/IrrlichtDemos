// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef PHYSICS_H
#define PHYSICS_H

#include "irrlicht_manager.h"
#include <vector>
#include <map>


/*
    v=velocity: m/s
    m=mass: kg
    t=time: seconds;
    a=acceleartion: m/(s*s)
    momentum (quantity of motion energy) = m * v
    F=Force: Newton (1kg per m*s*s)
    F = m * a

    // works only for constant acceleration!
   V = V0 + acceleration * time
*/

class TiXmlElement;
class Recorder;

enum CollisionTypes
{
    CT_COLL_NONE,
    CT_COLL_WORLD,
    CT_COLL_ALL,
};

// Very rudimentary physics which is specific for the control of the hover
// The only physics geometrie for now is a ball

struct PhysicsObjectSettings
{
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    float               mMass;
    float               mMaxSpeed;
};

// Track-borders can have special handling to allow the physic
// to keep objects from falling off the track.
struct PhysicsTrackBorderInfo
{
	PhysicsTrackBorderInfo()
	: mDistanceBorder(FLT_MAX), mDistanceTrackCenter(FLT_MAX)
	{
	}

	irr::core::vector3df mNearestBorder;
	float mDistanceBorder;
	irr::core::vector3df mNearestTrackCenter;
	float mDistanceTrackCenter;
	irr::core::vector3df mTrackDir;
};

struct PhysicsObject
{
    friend class Physics;

    PhysicsObject();
    ~PhysicsObject();

    inline PhysicsObjectSettings& GetSettings()                     { return mSettings; }
    inline void SetCollisionType(CollisionTypes type_)              { mCollType = type_; }
    inline const irr::core::vector3df& GetRepulsionNormal() const   { return mRepulsionNormal; }

    inline void SetHuman(bool isHuman)                              { mIsHumanPlayer = isHuman; }
    inline bool IsHuman() const                                     { return mIsHumanPlayer; }

    inline bool HasTouchedWorldGeometry() const                     { return mHasTouchedWorldGeometryLastUpdate; }
    inline bool HasTouchedWall() const                              { return mHasTouchedWallLastUpdate; }
    inline bool HasTouchedObject() const                            { return mHasTouchedObjectLastUpdate; }

    // relative speed, scaled between 0 and 1
    inline float GetSpeedScaled()  const                            { return mSettings.mMaxSpeed ? mVelocity.getLength()/mSettings.mMaxSpeed : 0.f; }
    inline float GetSpeedAbsolute() const                           { return mVelocity.getLength(); }
    void SetVelocity(const irr::core::vector3df& vel_);

    inline const irr::core::vector3df& GetVelocity() const          { return mVelocity; }

    inline void SetForceAccu(const irr::core::vector3df& accu_)     { mForceAccu = accu_; }
    inline const irr::core::vector3df& GetForceAccu() const         { return mForceAccu; }
    inline void AddForce(const irr::core::vector3df& force_)		{ mForceAccu += force_; }

    inline void AddFriction(float friction_)						{ mFrictionAccu += friction_; }

    inline void SetTrackBorderInfo(const PhysicsTrackBorderInfo& info) { mTrackBorderInfo = info; }

    irr::core::vector3df GetCollisionCenterToModelPos() const       { return mSceneNode->getTransformedBoundingBox().getCenter() - mSceneNode->getPosition(); }
    const irr::core::vector3df& GetCurrentStepCollCenter() const    { return mCurrentStepCollCenter; }
    const irr::core::vector3df& GetLastStepCollCenter() const       { return mLastStepCollCenter; }
    float GetCushioningLastFrame() const                            { return mCushioning; }

    void ForcePositionUpdate(); // losing interpolation for a step. Do it when situations change extrem (like teleporting)


    PhysicsObjectSettings    mSettings;
    float mRadius;
    irr::scene::ISceneNode * mSceneNode;
    bool mDelayObjectCollUntilSafe;  // set to true if you don't want obj-obj collisions to happen for object which are currently colliding

    // ugly. should be private, but is currently used in a strange place and i fear changing that in masterphase
    bool mHasTouchedWorldGeometry; // internal value, can be set/unset several times within one update

private:

	bool					mIsHumanPlayer;	// damn humans get all the special treatments
    CollisionTypes      	mCollType;

    irr::core::vector3df    mCurrentStepCollCenter;     // actuall result from current physik step
    irr::core::vector3df    mLastStepCollCenter;        // result from former physik step (needed for interpolation)
    irr::core::vector3df    mModelCollCenter;           // the value we get from the model at the start of update
    irr::core::vector3df    mInterpolatedCollCenter;    // the interpolation value we found in last update

    float               	mFrictionAccu;
    irr::core::vector3df    mVelocity;          // in world coordinates
    irr::core::vector3df    mForceAccu; // in world coordinates
    PhysicsTrackBorderInfo	mTrackBorderInfo;

    bool                	mHasTouchedWorldGeometryLastUpdate;
    bool                	mHasTouchedWallLastUpdate;  // it touched geometry which ain't used as floor
    bool                	mHasTouchedObjectLastUpdate;  // collisions with other physics object
    bool                	mHasTouchedWall;    // internal value, can be set/unset several times within one update
    bool                	mHasTouchedObject;  // internal value. collisions with other physics object

//    irr::core::triangle3df  mNearestTriangle;   // nearest triangle from last collision
    irr::core::vector3df    mRepulsionNormal;   // repulsion normal for sliding plane from last collision

	float mCushioning;	  // how much did cushions affect the hover this frame
	float mCushionErosion; // factor between 0 and 1 how much cushioning get's reduced when it's used too much for a while.
};

struct PhysicsSettings
{
	PhysicsSettings();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    float mTimeStep;        // in seconds
    float mRollFriction;
    float mAirConst;
    float mGravity;
    float mWallFriction;
    float mWallBoundary;
    float mCollPush;
    float mCushionFactor;
    float mCushionSlowFall;
};

class IPhysicsTickFunctor
{
public:
    virtual ~IPhysicsTickFunctor() {}
    virtual void OnTick(unsigned int ticksSinceGameStart_) = 0;
};

template <class T>
class PhysicsTickFunctor : public IPhysicsTickFunctor
{
public:
    PhysicsTickFunctor(T* obj_, void (T::* fpt_)(unsigned int ticksSinceGameStart_))
    {
        mObj = obj_;
        mFunctionPtr = fpt_;
    }
    virtual ~PhysicsTickFunctor() {}
    virtual void OnTick(unsigned int ticksSinceGameStart_)
    {
        return (*mObj.*mFunctionPtr)(ticksSinceGameStart_);
    }

private:
    void (T::*  mFunctionPtr)(unsigned int ticksSinceGameStart_);
    T*          mObj;
};

#define PHYSICS_MAX_COLLISION_TRIANGLES 4000

class Physics
{
public:
    Physics();
    ~Physics();

    void ClearAll();
    void ClearCollisionMeshes();
    void ClearPhysicsObjects();

    void AddCollisionMesh(irr::scene::ITriangleSelector* selector_); // world meshes
    bool RemoveCollisionMesh(irr::scene::ITriangleSelector* selector_);

    int AddPhysicsObject(irr::scene::ISceneNode * node_, float radius_); // return physicsId
    PhysicsObject* GetPhysicsObject(int id_) const;

    // timeTick_: time in seconds for this game tick (since last Update)
    // enableObjObjCollision: when true hovers collide against each other, when false they don't
    void Update(irr::f32 timeTick_, bool enableObjObjCollision);

    void EnableDebugging(bool enable_)  { mDebuggingEnabled = enable_; }
    bool IsDebuggingEnabled() const     { return mDebuggingEnabled; }

    PhysicsSettings& GetSettings()       { return mSettings; }

    size_t GetNumTriangleSelectors() const  { return mCollisionSelectors.size(); }
    irr::scene::ITriangleSelector* GetTriangleSelector(size_t index_) const;

    bool GetTrackIntersection(irr::core::line3d<float> &line_, const irr::core::vector3df &searchPos_, irr::core::vector3df & result_);
    bool FindBorder(const irr::core::vector3df& start_, const irr::core::vector3df& dir_, irr::core::vector3df&  target_, float radius_);
    bool CalcSphereAboveTrack(const irr::core::vector3df &pos_, irr::core::vector3df &target_, irr::core::vector3df & repulsion_, float radius_);
    bool HasCollision( const irr::core::line3d<irr::f32>& ray_);

    void ResetTicksSinceGameStart()             { mTicksSinceGameStart = 0; mInterpolatedTick = 0.f; }
    unsigned int GetTicksSinceGameStart() const { return mTicksSinceGameStart; }
    float GetInterpolatedTick() const           { return mInterpolatedTick; }

    // functors which are called once each time the physics does make a "tick"
    void AddTickFunctor(int id_, IPhysicsTickFunctor * functor_ );
    void RemoveTickFunctor(int id_);

protected:
    bool HandleCollision(irr::core::vector3df &center_, float radius_, irr::core::triangle3df &nearestTriangle_, irr::core::vector3df &repulsionNormal_);
    void HandleCushions(PhysicsObject * obj, irr::core::vector3df& step);
    void HandleObjObjCollision();
    void FindCollisionTriangles( const irr::core::aabbox3d<irr::f32> &box_);

private:
    int mNextObjectId;
    bool mDebuggingEnabled;
    float mTimeRest;
    unsigned int mTicksSinceGameStart;
    float mInterpolatedTick;

    irr::core::vector3df mWallNormal;

    typedef std::map<int, PhysicsObject*> PhysicsObjectMap ;
    PhysicsObjectMap mPhysicsObjects;

    typedef std::vector<irr::scene::ITriangleSelector*> CollisionSelectorVector;
    std::vector<irr::scene::ITriangleSelector*> mCollisionSelectors;

    PhysicsSettings  mSettings;

    typedef std::map<int, IPhysicsTickFunctor*> TickFunctorMap;
    TickFunctorMap   mTickFunctors;

    irr::core::triangle3df mCollisionTriangles[PHYSICS_MAX_COLLISION_TRIANGLES];  // triangles used in collision checks
    int mCollisionTrianglesSize;    // nr of collision triangles which are used

};

#endif // PHYSICS_H
