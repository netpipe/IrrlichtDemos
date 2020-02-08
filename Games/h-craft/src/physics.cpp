// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "physics.h"
#include "tinyxml/tinyxml.h"
#include "main.h"
#include "recorder.h"
#include "globals.h"
#include "helper_irr.h"
#include "logging.h"
#include <math.h>
#include <float.h>

using namespace irr;

// --------------------------------------------------------
void PhysicsObjectSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    settings_->QueryFloatAttribute("mass", &mMass);
    settings_->QueryFloatAttribute("max_speed", &mMaxSpeed);
}

void PhysicsObjectSettings::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    settings_->SetDoubleAttribute("mass", mMass );
    settings_->SetDoubleAttribute("max_speed", mMaxSpeed );
}


// --------------------------------------------------------
PhysicsObject::PhysicsObject()
: mSceneNode(NULL)
, mDelayObjectCollUntilSafe(false)
, mIsHumanPlayer(false)
{
    mHasTouchedWorldGeometry = false;
    mCushioning = 0.f;
    mCushionErosion = 0.f;
    mHasTouchedWorldGeometryLastUpdate = false;
    mHasTouchedWall = false;
    mHasTouchedWallLastUpdate = false;
    mHasTouchedObject = false;
    mSettings.mMass = 10.1f;
    mSettings.mMaxSpeed = 2000.f;
    mRadius = 1.f;
    mFrictionAccu = 0.f;
    mCollType = CT_COLL_ALL;

    mCurrentStepCollCenter.set(0,0,0);
    mLastStepCollCenter.set(0,0,0);
    mModelCollCenter.set(0,0,0);
    mInterpolatedCollCenter.set(0,0,0);
}

PhysicsObject::~PhysicsObject()
{
}


void PhysicsObject::SetVelocity(const core::vector3df& vel_)
{
    mVelocity = vel_;
    mLastStepCollCenter = mCurrentStepCollCenter - mVelocity * APP.GetPhysics()->GetSettings().mTimeStep;
}

void PhysicsObject::ForcePositionUpdate()
{
    mModelCollCenter = mSceneNode->getTransformedBoundingBox().getCenter();
    mInterpolatedCollCenter = mModelCollCenter;
    mCurrentStepCollCenter = mModelCollCenter;
    mLastStepCollCenter = mModelCollCenter;
}

// --------------------------------------------------------
PhysicsSettings::PhysicsSettings()
: mTimeStep(0.16)
, mRollFriction(0.f)
, mAirConst(0.f)
, mGravity(0.f)
, mWallFriction(0.f)
, mWallBoundary(0.f)
, mCollPush(0.f)
, mCushionFactor(0.5f)
, mCushionSlowFall(0.3f)
{
}

void PhysicsSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    double val = 0.f;
    if ( settings_->Attribute("timestep", &val) )
        mTimeStep = (float)val;
    if ( settings_->Attribute("rollfriction", &val) )
        mRollFriction = (float)val;
    if ( settings_->Attribute("airconst", &val) )
        mAirConst = (float)val;
    if ( settings_->Attribute("gravity", &val) )
        mGravity = (float)val;
    if ( settings_->Attribute("wallfriction", &val) )
        mWallFriction = (float)val;
    if ( settings_->Attribute("wallboundary", &val) )
        mWallBoundary = (float)val;
    if ( settings_->Attribute("collpush", &val) )
        mCollPush = (float)val;
	if ( settings_->Attribute("cushion", &val) )
        mCushionFactor = (float)val;
    if ( settings_->Attribute("slowfall", &val) )
		mCushionSlowFall = (float)val;
}

void PhysicsSettings::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    double val = 0.f;
    val = mTimeStep;
    settings_->SetDoubleAttribute("timestep", val );

    val = mRollFriction;
    settings_->SetDoubleAttribute("rollfriction", val );

    val = mAirConst;
    settings_->SetDoubleAttribute("airconst", val );

    val = mGravity;
    settings_->SetDoubleAttribute("gravity", val );

    val = mWallFriction;
    settings_->SetDoubleAttribute("wallfriction", val );

    val = mWallBoundary;
    settings_->SetDoubleAttribute("wallboundary", val );

    val = mCollPush;
    settings_->SetDoubleAttribute("collpush", val );

    val = mCushionFactor;
    settings_->SetDoubleAttribute("cushion", val );

    val = mCushionSlowFall;
    settings_->SetDoubleAttribute("slowfall", val );
}


// --------------------------------------------------------
Physics::Physics()
{
    mCollisionTrianglesSize = 0;
    mNextObjectId = 0;
    mDebuggingEnabled = false;
    mTimeRest = 0.f;
    mTicksSinceGameStart = 0;
    mInterpolatedTick = 0.f;
    mSettings.mTimeStep = 0.1f;
    mSettings.mRollFriction = 10.f;
    mSettings.mAirConst = 0.01;
    mSettings.mGravity = 100.f;
    mSettings.mWallFriction = 100.f;
    mSettings.mWallBoundary = 0.7f;
    mSettings.mCollPush = 500.f;

    PROFILE_ADD_GROUP(PGROUP_PHYSICS, "physics");
    PROFILE_ADD(300, PGROUP_PHYSICS, "ph update");
    PROFILE_ADD(301, PGROUP_PHYSICS, "ph trackinters");
    PROFILE_ADD(302, PGROUP_PHYSICS, "ph coll");
    PROFILE_ADD(303, PGROUP_PHYSICS, "ph collTri1");
    PROFILE_ADD(304, PGROUP_PHYSICS, "ph collTri2");
    PROFILE_ADD(305, PGROUP_PHYSICS, "ph collTri3");
    PROFILE_ADD(306, PGROUP_PHYSICS, "ph update2");
    //PROFILE_ADD(307, PGROUP_PHYSICS, "ph update3");
}

Physics::~Physics()
{
    ClearAll();
    TickFunctorMap::iterator itFunctors = mTickFunctors.begin();
    for ( ; itFunctors != mTickFunctors.end(); ++itFunctors )
    {
        delete itFunctors->second;
    }
}

void Physics::ClearAll()
{
    mTimeRest = 0.f;
    ClearCollisionMeshes();
    ClearPhysicsObjects();
}

void Physics::ClearCollisionMeshes()
{
    mTimeRest = 0.f;
    for ( size_t i=0; i < mCollisionSelectors.size(); ++i )
    {
		mCollisionSelectors[i]->drop();
    }
    mCollisionSelectors.clear();
}

void Physics::ClearPhysicsObjects()
{
    mTimeRest = 0.f;

    typedef PhysicsObjectMap::iterator ItMap;
    for ( ItMap itObjects1 = mPhysicsObjects.begin(); itObjects1 != mPhysicsObjects.end(); ++itObjects1 )
    {
        PhysicsObject * obj = itObjects1->second;
        delete obj;
    }
    mPhysicsObjects.clear();
}

int Physics::AddPhysicsObject(scene::ISceneNode * node_, float radius_)
{
    if ( !node_)
        return -1;

    int id = mNextObjectId;
    ++mNextObjectId;
    PhysicsObject * obj = new PhysicsObject();
    obj->mSceneNode = node_;
    obj->mRadius = radius_;
    mPhysicsObjects[id] = obj;

    // printf("added physics object with id %d radius %f", id, obj->mRadius);

    return id;
}

PhysicsObject* Physics::GetPhysicsObject(int id_) const
{
    if ( id_ < 0 )
        return NULL;
    PhysicsObjectMap::const_iterator it = mPhysicsObjects.find(id_);
    if  (it != mPhysicsObjects.end() )
        return it->second;
    return NULL;
}

void Physics::AddCollisionMesh(scene::ITriangleSelector* selector_)
{
    if ( selector_)
    {
    	LOG.LogLn(LP_DEBUG, "collision mesh added");
        selector_->grab();
        mCollisionSelectors.push_back(selector_);
    }
}

bool Physics::RemoveCollisionMesh(scene::ITriangleSelector* selector_)
{
    for ( CollisionSelectorVector::iterator it  = mCollisionSelectors.begin(); it != mCollisionSelectors.end(); ++it )
    {
        if ( *it == selector_ )
        {
            (*it)->drop();
            mCollisionSelectors.erase(it);
            return true;
        }
    }
    return false;
}

scene::ITriangleSelector* Physics::GetTriangleSelector(size_t index_) const
{
	return mCollisionSelectors[index_];
}

bool Physics::GetTrackIntersection(core::line3d<float> &line_, const core::vector3df &searchPos_, core::vector3df & result_)
{
    PROFILE_START(301);
    core::aabbox3d<f32> box;
    box.reset(line_.start);
    box.addInternalPoint(line_.end);

    FindCollisionTriangles( box );

    double nearestDist = FLT_MAX;
    core::vector3df nearestPoint;

    // find nearest collision
    bool found = false;
    for ( int i=0; i < mCollisionTrianglesSize; i++ )
    {
//        if ( !useTriangles[i] )
//            continue;

        core::vector3df pointOnPlane;
        if ( mCollisionTriangles[i].getIntersectionWithLimitedLine (line_, pointOnPlane) )
        {
            double dist = searchPos_.getDistanceFromSQ(pointOnPlane);
            if ( dist < nearestDist )
            {
                nearestDist = dist;
                result_ = pointOnPlane;
                found = true;
            }
        }
    }

     if ( mDebuggingEnabled )
    {
        video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();
        APP.GetIrrlichtManager()->SetDriverDrawMode();
        if ( found )
        {
            driver->draw3DLine (line_.start, line_.end, video::SColor(255, 0, 0, 255));
            driver->draw3DLine (line_.start+core::vector3df(5,0,5), line_.end+core::vector3df(5,0,5), video::SColor(255, 0, 0, 255));
        }
        else
            driver->draw3DLine (line_.start, line_.end, video::SColor(255, 255, 255, 255));
    }

    PROFILE_STOP(301);

    return found;
}

void Physics::FindCollisionTriangles( const core::aabbox3d<f32> &box_)
{
    // get all triangles in this area
//    static bool useTriangles[PHYSICS_MAX_COLLISION_TRIANGLES];

    mCollisionTrianglesSize = 0;
    for ( size_t i=0; i<mCollisionSelectors.size(); ++i )
    {
        scene::ITriangleSelector* selector = mCollisionSelectors[i];
        int trianglesReceived = 0;

        selector->getTriangles( &mCollisionTriangles[mCollisionTrianglesSize], PHYSICS_MAX_COLLISION_TRIANGLES-mCollisionTrianglesSize, trianglesReceived, box_, /*transform*/ 0 );
        mCollisionTrianglesSize += trianglesReceived;
    }

    // FIXED: with my own octree-selector it's faster now
//    // octree returns a lot triangles outside which we can already ignore here
//    PROFILE_START(304);
//    core::aabbox3d<f32> boxTriangle;
//    // find nearest collision
//    for ( int i=0; i < mCollisionTrianglesSize; ++i )
//    {
//        boxTriangle.reset(mCollisionTriangles[i].pointA);
//        boxTriangle.addInternalPoint(mCollisionTriangles[i].pointB);
//        boxTriangle.addInternalPoint(mCollisionTriangles[i].pointC);
//        useTriangles[i] = boxTriangle.intersectsWithBox(box);
//    }
//    PROFILE_STOP(304);

}

bool Physics::HandleCollision(core::vector3df &center_, float radius_, core::triangle3df &nearestTriangle_, core::vector3df &repulsionNormal_)
{
    PROFILE_START(302);
    // for debug output
    video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();

    bool hasCollision = false;

    PROFILE_START(303);
    const float BOX_SPACING = 2*radius_; // we get the collision triangles just once - therefore make it bigger than the radius
    core::aabbox3d<f32> box(center_.X-BOX_SPACING, center_.Y-BOX_SPACING, center_.Z-BOX_SPACING, center_.X+BOX_SPACING, center_.Y+BOX_SPACING, center_.Z+BOX_SPACING);
    if ( mDebuggingEnabled )
    {
        APP.GetIrrlichtManager()->SetDriverDrawMode();
        driver->draw3DBox (box, video::SColor(255, 255, 255, 255));
    }

    FindCollisionTriangles( box );
    PROFILE_STOP(303);

    bool findMoreCollisions = true;
    int triesCountdown = 20;
    while ( findMoreCollisions && triesCountdown >= 0 )
    {
        --triesCountdown;
        findMoreCollisions = false;

        double nearestDist = radius_*radius_;
        core::vector3df nearestPoint;
        core::vector3df repulsion;
        core::triangle3df triangleNearest;

        // find nearest collision
        for ( int i=0; i < mCollisionTrianglesSize; i++ )
        {
//            if ( !useTriangles[i] )
//                continue;

            // debug
            if ( mDebuggingEnabled )
            {
                driver->draw3DTriangle(mCollisionTriangles[i], video::SColor(0, 255, 0,127));
            }

            core::vector3df pointOnPlane;
            if ( mCollisionTriangles[i].getIntersectionOfPlaneWithLine (center_, mCollisionTriangles[i].getPlane().Normal, pointOnPlane) )
            {
                core::vector3df pointOnTriangle;
                if ( mCollisionTriangles[i].isPointInsideFast(pointOnPlane) )
                    pointOnTriangle = pointOnPlane;
                else
                    pointOnTriangle = mCollisionTriangles[i].closestPointOnTriangle(pointOnPlane);
                double dist = center_.getDistanceFromSQ(pointOnTriangle);
                if ( dist < nearestDist )
                {
                    findMoreCollisions = true;
                    nearestDist = dist;
                    triangleNearest = mCollisionTriangles[i];
                    repulsion =  (center_ - pointOnTriangle);
                    nearestPoint = pointOnTriangle;
                }
            }
        }
        if ( findMoreCollisions )
        {
            nearestTriangle_ = triangleNearest;
            repulsionNormal_ = repulsion;
            repulsionNormal_.normalize();
            if ( !hasCollision )
            {
                hasCollision = true;
                mWallNormal = repulsionNormal_;
            }
            else if ( fabs(repulsionNormal_.Y) < fabs(mWallNormal.Y) )
            {
                mWallNormal = repulsionNormal_;
            }
//          core::plane3df collisionPlane(nearestPoint, lineToTriangle);
            repulsion.setLength(0.001f+radius_-sqrt(nearestDist));

            center_ += repulsion;
//          printf("radius: %.2f nearestDist:%.2f sqrt(nearestDist):%.2f\n", radius_, nearestDist, sqrt(nearestDist));

            // debug
            if ( mDebuggingEnabled )
            {
                // nearest collision triangle
                triangleNearest.pointA.Y += 10.f;
                triangleNearest.pointB.Y += 10.f;
                triangleNearest.pointC.Y += 10.f;
                driver->draw3DTriangle(triangleNearest, video::SColor(255, 0, 0,127));

                // push back vector
                driver->draw3DLine (nearestPoint, center_, video::SColor(0, 0, 255, 255));

                // nearest point (as cross)
                driver->draw3DLine (core::vector3df(nearestPoint.X-50, nearestPoint.Y, nearestPoint.Z), core::vector3df(nearestPoint.X+50, nearestPoint.Y, nearestPoint.Z), video::SColor(0, 255, 255, 255));
                driver->draw3DLine (core::vector3df(nearestPoint.X, nearestPoint.Y, nearestPoint.Z-50), core::vector3df(nearestPoint.X, nearestPoint.Y, nearestPoint.Z+50), video::SColor(0, 255, 255, 255));
            }
        }
    }

    PROFILE_STOP(302);
    return hasCollision;
}

bool Physics::FindBorder(const core::vector3df& start_, const core::vector3df& dir_, core::vector3df&  target_, float radius_)
{
    const float MAX_WIDTH = 600.f;
    const float STEPSIZE = 10.f;
    core::vector3df step(dir_);
    core::vector3df repulsion;
    core::vector3df dirNormalXZ(dir_);
    dirNormalXZ.Y = 0.f;
    dirNormalXZ.normalize();
    step.setLength(STEPSIZE);

    target_ = start_;
    CalcSphereAboveTrack(start_, target_, repulsion, radius_);

    core::vector3df next(target_+step);
    const float SEARCH_ABOVE_TRACK = 150.f;
    const float SEARCH_BELOW_TRACK = 500.f;
    core::vector3df intersection;
    core::line3d<float> line(next+core::vector3df(0, SEARCH_ABOVE_TRACK, 0)
                            , next+core::vector3df(0, -SEARCH_BELOW_TRACK, 0) );

    while ( GetTrackIntersection(line, next, intersection) )
    {
        core::vector3df targetPos;
        if ( !CalcSphereAboveTrack(intersection, targetPos, repulsion, radius_) )
        {
            return true;
        }
        if ( fabs(target_.Y - targetPos.Y) > radius_* 0.5f )
        {
            return true;
        }
        if ( (targetPos-target_).getLength() < STEPSIZE * 0.5f )
        {
            return true;
        }
        if ( (targetPos-start_).getLength() > MAX_WIDTH )
        {
            return false; // not a border!
        }
//        core::vector3df newDirXZ(targetPos-start_);
//        newDirXZ.Y = 0.f;
//        newDirXZ.normalize();
//        if ( fabs( newDirXZ.dotProduct(dirNormalXZ) ) < 0.6f )
//        {
//            return true;
//        }

        target_ = intersection;
        target_.Y = targetPos.Y;
        next = target_+ step;
        line = core::line3d<float>(next+core::vector3df(0, SEARCH_ABOVE_TRACK, 0)
                            , next+core::vector3df(0, -SEARCH_BELOW_TRACK, 0) );
        //fprintf(stderr, "next: %.2f %.2f %.2f\n", next.X, next.Y, next.Z );
    }

    return true;
}

bool Physics::HasCollision( const core::line3d<f32>& ray_)
{
    for ( size_t i=0; i < GetNumTriangleSelectors(); ++i )
    {
        core::vector3df intersection;
        core::triangle3df tri;
#if IRR_SVN <= 3231
        const irr::scene::ISceneNode* collidingNode = 0;
#else
		irr::scene::ISceneNode* collidingNode = 0;
#endif

        if (APP.GetIrrlichtManager()->GetSceneManager()->getSceneCollisionManager()->getCollisionPoint(
            ray_, GetTriangleSelector(i), intersection, tri, collidingNode))
        {
            return true;
        }
    }

    return false;
}

bool Physics::CalcSphereAboveTrack(const core::vector3df &pos_, core::vector3df &target_, core::vector3df & repulsion_, float radius_)
{
    core::line3d<f32> line;
    line.start = pos_ + core::vector3df(0, 50, 0);  // slightly above
    line.end = pos_ + core::vector3df(0, -5000, 0); // way below

    float nearestDist = FLT_MAX;
    core::triangle3df nearestTriangle;
    bool hasTarget = false;

    // find nearest geometry around pos_
    for ( size_t i=0; i < GetNumTriangleSelectors(); ++i )
    {
        core::vector3df intersection;
        core::triangle3df tri;
#if IRR_SVN <= 3231
        const irr::scene::ISceneNode* collidingNode = 0;
#else
		irr::scene::ISceneNode* collidingNode = 0;
#endif

        if (APP.GetIrrlichtManager()->GetSceneManager()->getSceneCollisionManager()->getCollisionPoint(
            line, GetTriangleSelector(i), intersection, tri, collidingNode))
        {
            float dist = (intersection-line.start).getLength();
            if ( dist < nearestDist )
            {
                nearestDist = dist;
                target_ = intersection;
                nearestTriangle = tri;
                hasTarget = true;
            }
        }
    }

    if ( hasTarget )
    {
        repulsion_ = core::vector3df(0, 1, 0);
        core::triangle3df tri;
        target_.Y += radius_;
        HandleCollision(target_, radius_, tri, repulsion_);
    }

    return hasTarget;
}

void Physics::HandleObjObjCollision()
{
    typedef PhysicsObjectMap::iterator ItMap;

    // reset touched flag
    for ( ItMap itObjects1 = mPhysicsObjects.begin(); itObjects1 != mPhysicsObjects.end(); ++itObjects1 )
    {
        PhysicsObject * obj = itObjects1->second;
        obj->mHasTouchedObject = false;
    }

    // just add some force vector to push them in different directions
    for ( ItMap itObjects1 = mPhysicsObjects.begin(); itObjects1 != mPhysicsObjects.end(); ++itObjects1 )
    {
        PhysicsObject * obj1 = itObjects1->second;
        if ( obj1->mCollType != CT_COLL_ALL )
            continue;

        ItMap itObjects2 = itObjects1;
        ++itObjects2;
        for ( ; itObjects2 != mPhysicsObjects.end(); ++itObjects2 )
        {
            PhysicsObject * obj2 = itObjects2->second;
            if ( obj2->mCollType != CT_COLL_ALL )
                continue;

            core::vector3df collCenter1(obj1->mCurrentStepCollCenter);
            core::vector3df collCenter2(obj2->mCurrentStepCollCenter);

            core::vector3df distVec (collCenter1 - collCenter2);
            float dist = distVec.getLength();
            float distColl = (obj1->mRadius+obj2->mRadius) - dist;
            if ( distColl > 0.f )
            {
                if (    !obj1->mDelayObjectCollUntilSafe
                    &&  !obj2->mDelayObjectCollUntilSafe)
                {
                    core::vector3df push1(distVec);
                    core::vector3df push2(distVec*-1);

					// playability hack added in 1.3 - give humans more power
					float humansAreSooStrong1 = 1.f;
					float humansAreSooStrong2 = 1.f;
					if ( obj1->IsHuman() && !obj2->IsHuman() )
					{
						humansAreSooStrong1 = 0.75f;
						humansAreSooStrong2 = 1.25f;
					}
					else if ( !obj1->IsHuman() && obj2->IsHuman() )
					{
						humansAreSooStrong1 = 1.25f;
						humansAreSooStrong2 = 0.75f;
					}

                    push1.setLength( distColl * obj2->mSettings.mMass * mSettings.mCollPush * humansAreSooStrong1);
                    push2.setLength( distColl * obj1->mSettings.mMass * mSettings.mCollPush * humansAreSooStrong2);

                    obj1->mForceAccu += push1;
                    obj2->mForceAccu += push2;
                }

                obj1->mHasTouchedObject = true;
                obj1->mHasTouchedObjectLastUpdate = true;

                obj2->mHasTouchedObject = true;
                obj2->mHasTouchedObjectLastUpdate = true;
            }
        }
    }

    // check mDelayObjectCollUntilSafe settings
    for ( ItMap itObjects1 = mPhysicsObjects.begin(); itObjects1 != mPhysicsObjects.end(); ++itObjects1 )
    {
        PhysicsObject * obj = itObjects1->second;
        if(     obj->mDelayObjectCollUntilSafe
            && !obj->mHasTouchedObject
         )
        {
            obj->mDelayObjectCollUntilSafe = false;
        }
    }
}

void Physics::HandleCushions(PhysicsObject * obj, irr::core::vector3df& step)
{
	// Trying to prevent player hovers from falling off the track so much by adding some cushions at the outside of the tracks.
	// This was't planned originally, but introduced for the 1.3 (android) version.
	// We don't have correct border walls, so we use some lines from the ai which are roughly in the right place.
	// Those lines are not good enough to use directly for the collisions so we just use the information
	// about those lines in obj->mTrackBorderInfo and try to figure out a solution which works "good enough".
	// Unfortunately it's not completely deterministic and not even completely frame-rate independent (because
	// the border-lines are not updated each calculation step).
	// Also some problems with getting stuck to the border which looks funny.
	// Still... I think it's better to play than before.

	// to prevent h-craft's getting stuck on the outside the force keeping it on the border decreases after a while
	const float BASE_EROSION = -0.1f;
	const float INC_EROSION = mSettings.mTimeStep * 0.5f;
	const float DEC_EROSION = mSettings.mTimeStep * 1.f;

	if ( mSettings.mCushionFactor > 0.f )
	{
		const float radius = obj->mRadius * 1.25f;	// try&error - without scaling radius it was too often at the exact border where hovers started to fall off.
		if ( obj->mTrackBorderInfo.mDistanceBorder < radius )
		{
			// Find the line that shouldn't be crossed.
			// (this part can be removed if we ever improve the border-lines so they can be used directly)
			core::vector3df towardCenter(obj->mTrackBorderInfo.mNearestTrackCenter-obj->mTrackBorderInfo.mNearestBorder);
			core::vector2df towardCenter2d(towardCenter.X, towardCenter.Z);
			core::vector2df perpToCenter(-towardCenter2d.Y, towardCenter2d.X);
			core::vector2df border2d(obj->mTrackBorderInfo.mNearestBorder.X, obj->mTrackBorderInfo.mNearestBorder.Z);
			core::line2df borderLine( border2d, border2d + perpToCenter );
			towardCenter2d.normalize();
			borderLine += towardCenter2d*radius;	// subtract radius, so we can work with object-center

			core::vector2df target2d(obj->mCurrentStepCollCenter.X, obj->mCurrentStepCollCenter.Z);
			core::vector2df start2d(obj->mLastStepCollCenter.X, obj->mLastStepCollCenter.Z);

			// find the closest point on border for current new position
			core::vector2df onBorder(borderLine.getClosestPoint(target2d, false));

			// are we past the border already?
			if ( ExtIrr::IsSameDir((target2d-onBorder), (start2d-onBorder)) )
			{
				// we don't want to move backwards, so in this case the border-point has to be moved
				core::vector2df startOnBorder(borderLine.getClosestPoint(start2d, false));
				onBorder += (start2d-startOnBorder);
			}

			float factor = mSettings.mCushionFactor;
			if ( obj->mCushionErosion > 0 )
				factor *= (1.f-obj->mCushionErosion);

			// Do we move toward the outside? And are we going to cushion things?
			if ( !ExtIrr::IsSameDir(step, towardCenter) && factor > 0.f )
			{
				// We prevent (or reduce when mCushionFactor < 1) the movement opposite to the towardCenter vector
				core::vector2df newTarget(start2d + ((target2d-start2d)*(1.f-factor) + (onBorder-start2d)*factor) );

				core::vector2df diffVec(newTarget.X-obj->mCurrentStepCollCenter.X, newTarget.Y-obj->mCurrentStepCollCenter.Z);
				obj->mCurrentStepCollCenter.X = newTarget.X;
				obj->mCurrentStepCollCenter.Z = newTarget.Y;

				if ( mSettings.mCushionSlowFall <= 0.f )
				{
					float oldLen = ExtIrr::GetLengthXZ(step);
					step = obj->mCurrentStepCollCenter - obj->mLastStepCollCenter;
					float newLen = ExtIrr::GetLengthXZ(step);
					if ( newLen < oldLen && obj->mLastStepCollCenter.Y > obj->mCurrentStepCollCenter.Y )
					{
						// Prevent getting stuck in geometry if we are moved back on a slope going downwards to the outside.
						// Still can be stuck a little because of bumps, but better than not doing it.
						obj->mCurrentStepCollCenter.Y += (obj->mCurrentStepCollCenter.Y-obj->mLastStepCollCenter.Y)*(1.f-newLen/oldLen);
						step = obj->mCurrentStepCollCenter - obj->mLastStepCollCenter;
					}
				}

				obj->mCushioning += diffVec.getLength();

				obj->mCushionErosion += INC_EROSION;
				if ( obj->mCushionErosion > mSettings.mCushionFactor )
					obj->mCushionErosion = mSettings.mCushionFactor;
			}

			if ( mSettings.mCushionSlowFall >= 0.f && obj->mLastStepCollCenter.Y > obj->mCurrentStepCollCenter.Y )
			{
				obj->mCurrentStepCollCenter.Y += (obj->mLastStepCollCenter.Y-obj->mCurrentStepCollCenter.Y)*mSettings.mCushionSlowFall;
				step = obj->mCurrentStepCollCenter - obj->mLastStepCollCenter;
			}

			// update the values - it's not exact but our borders are not exact anyway so that's hopefully good enough
			obj->mTrackBorderInfo.mDistanceBorder = (onBorder - target2d).getLength();
		}
		else
		{
			obj->mCushionErosion -= DEC_EROSION;
			if ( obj->mCushionErosion < BASE_EROSION )
				obj->mCushionErosion = BASE_EROSION;
		}
	}
}

void Physics::Update(f32 timeTick_, bool enableObjObjCollision_)
{
    PROFILE_START(300);
    typedef PhysicsObjectMap::iterator ItMap;

    for ( ItMap itObjects = mPhysicsObjects.begin(); itObjects != mPhysicsObjects.end(); ++itObjects )
    {
        PhysicsObject * obj = itObjects->second;
        obj->mHasTouchedWorldGeometryLastUpdate = false;
        obj->mHasTouchedWallLastUpdate = false;
        obj->mHasTouchedObjectLastUpdate = false;
        obj->mCushioning = 0.f;

        obj->mSceneNode->updateAbsolutePosition();
        obj->mModelCollCenter = obj->mSceneNode->getTransformedBoundingBox().getCenter();

        // We handle objects moved outside the physics, so that it seems like they had always been in that other position in the past.
        // Rewriting history, to make it fit to current needs, a very political algorithm ;-)
        core::vector3df movedOutsidePhysics( obj->mModelCollCenter - obj->mInterpolatedCollCenter );
        obj->mCurrentStepCollCenter += movedOutsidePhysics;
        obj->mLastStepCollCenter += movedOutsidePhysics;
    }
	PROFILE_STOP(300);

    mTimeRest += timeTick_;
    // clamp to max. value to avoid long calculations in cases gone awry
    if ( mTimeRest > 2.f )
        mTimeRest = 2.f;

	PROFILE_START(306);
    int countSteps = 0;
    while ( mTimeRest > mSettings.mTimeStep )
    {
        ++mTicksSinceGameStart;

        for ( ItMap itObjects = mPhysicsObjects.begin(); itObjects != mPhysicsObjects.end(); ++itObjects )
        {
            PhysicsObject * obj = itObjects->second;
            obj->mLastStepCollCenter = obj->mCurrentStepCollCenter;
        }

        ++countSteps;
        mTimeRest -= mSettings.mTimeStep;

        if ( enableObjObjCollision_ )
        {
            HandleObjObjCollision();
        }

        for ( ItMap itObjects = mPhysicsObjects.begin(); itObjects != mPhysicsObjects.end(); ++itObjects )
        {
            PhysicsObject * obj = itObjects->second;

            float mass = obj->mSettings.mMass;
            float speed = obj->mVelocity.getLength();
            core::vector3df forceSum = obj->mForceAccu;
            //printf("forceSum1: %.1f %.1f %.1f\n", forceSum.X, forceSum.Y, forceSum.Z);

            // depending on radius is correct, but harder to use
//            float airResistanceFactor = obj->mRadius / 1000.f;
//            airResistanceFactor = airResistanceFactor*airResistanceFactor*mSettings.mAirConst*speed*speed;
            float airResistanceFactor = mSettings.mAirConst*speed*speed;

            core::vector3df frictionForce(obj->mVelocity);
            if ( frictionForce.getLength() != 0.f )
                frictionForce.normalize();
            core::vector3df airResistance(frictionForce);
            frictionForce *= -(mSettings.mRollFriction + obj->mFrictionAccu);	// mFrictionAccu is about braking
            airResistance *= -airResistanceFactor;
            //printf("force %.3f %.3f %.2f\n", obj->mForceAccu.X, obj->mForceAccu.Y, obj->mForceAccu.Z);

            core::vector3df frictionSum;
            if ( obj->mHasTouchedWorldGeometry )    // one step behind, but i don't think it's much of a difference
            {
                if ( obj->mHasTouchedWall )
                {
                    frictionForce *= mSettings.mWallFriction;
                }
                frictionSum += frictionForce;
                //printf("frictionForce: %.1f %.1f %.1f\n", frictionForce.X, frictionForce.Y, frictionForce.Z);
            }
            //printf("forceSum2: %.1f %.1f %.1f\n", forceSum.X, forceSum.Y, forceSum.Z);

            float maxFriction = obj->mVelocity.getLength() * 2 * mass / mSettings.mTimeStep;
            if ( frictionSum.getLength() > maxFriction)
                frictionSum.setLength(maxFriction);

            forceSum += airResistance;

            forceSum += frictionSum;

            core::vector3df acceleration = forceSum / mass;
            acceleration.Y -= mSettings.mGravity;

            core::vector3df step(obj->mVelocity * mSettings.mTimeStep + acceleration * 0.5f * mSettings.mTimeStep * mSettings.mTimeStep);

            if ( step.getLength() > obj->mSettings.mMaxSpeed * mSettings.mTimeStep )
                step.setLength(obj->mSettings.mMaxSpeed * mSettings.mTimeStep);
            //printf("step: %.2f %.2f %2.f", step.X, step.Y, step.Z);

            obj->mCurrentStepCollCenter += step;
            core::triangle3df collTriangle;
            core::vector3df repulsion;
            obj->mHasTouchedWorldGeometry = HandleCollision(obj->mCurrentStepCollCenter, obj->mRadius, collTriangle, repulsion);
            if ( obj->mHasTouchedWorldGeometry )
            {
                obj->mHasTouchedWorldGeometryLastUpdate = true;
                obj->mHasTouchedWall = fabs(mWallNormal.Y) < mSettings.mWallBoundary ? true : false;

                if( obj->mHasTouchedWall )
                {
                    // check if we're above a floor
                    core::line3d<f32> ray;
                    ray.start = obj->mCurrentStepCollCenter;
                    ray.end = obj->mCurrentStepCollCenter + core::vector3df(0, -1000, 0); // below
                    if ( !HasCollision( ray ) )
                    {
                        obj->mHasTouchedWall = false;
                    }
                }

                if ( obj->mHasTouchedWall )
                {
                    obj->mHasTouchedWallLastUpdate = true;
                }
            }

//            obj->mNearestTriangle = collTriangle;
            obj->mRepulsionNormal = repulsion;
            core::vector3df realStep(obj->mCurrentStepCollCenter - obj->mLastStepCollCenter);
            if ( realStep.getLengthSQ() > obj->mRadius * obj->mRadius )
            {
				LOG.LogLn(LP_WARN, "WARNING: object moves too fast for physics stepsize");
            }

			// Hack to help players staying on the track
            HandleCushions(obj, realStep);

            obj->mVelocity = realStep / mSettings.mTimeStep;
        }

        TickFunctorMap::iterator itFunctors = mTickFunctors.begin();
        for ( ; itFunctors != mTickFunctors.end(); ++itFunctors )
        {
            itFunctors->second->OnTick(mTicksSinceGameStart);
        }
    }
    PROFILE_STOP(306);

	//PROFILE_START(307);	// it's cheap and I need little profile output.
    for ( ItMap itObjects = mPhysicsObjects.begin(); itObjects != mPhysicsObjects.end(); ++itObjects )
    {
        PhysicsObject * obj = itObjects->second;

        // reset forces
        obj->mForceAccu.set(0,0,0);
        obj->mFrictionAccu = 0.f;

        // interpolate
        const float alpha = mTimeRest / mSettings.mTimeStep;
        obj->mInterpolatedCollCenter = obj->mCurrentStepCollCenter*alpha + obj->mLastStepCollCenter*(1.0f-alpha);
        mInterpolatedTick = mTicksSinceGameStart > 0 ? (mTicksSinceGameStart-1) + alpha : 0;

        // move model
        core::vector3df moved( obj->mInterpolatedCollCenter - obj->mModelCollCenter );
        obj->mSceneNode->setPosition(obj->mSceneNode->getPosition() + moved);
    }
    //PROFILE_STOP(307);
}

void Physics::AddTickFunctor(int id_, IPhysicsTickFunctor * functor_ )
{
    mTickFunctors[id_] = functor_;
}

void Physics::RemoveTickFunctor(int id_)
{
    TickFunctorMap::iterator it = mTickFunctors.find(id_);
    if ( it != mTickFunctors.end() )
    {
        delete it->second;
        mTickFunctors.erase(it);
    }
}
