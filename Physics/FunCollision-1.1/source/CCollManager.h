// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef CCOLLMANAGER_H_INCLUDED
#define CCOLLMANAGER_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "ICollManager.h"
#include "CCollSelector.h"
#include "CCollSampler.h"
#include "CCollObject.h"
#include "CCollEntity.h"


namespace COLL
{

//! CCollManager class
/// Manages the whole collsion system.
class CCollManager : virtual public ICollManager
{
    public:
        //! constructor
        CCollManager(IrrlichtDevice* Device, float GravityStrength, float BaseFriction)
        {
            device = Device;
            restartTimeing();
            setBaseFriction(BaseFriction);
            setGravityNorm(core::vector3df(0, -1, 0));
            setGravityStrength(GravityStrength);
        }

        //! destructor
        virtual ~CCollManager() { removeAll(); }


        //! ----- Get functions:
        IrrlichtDevice* getDevice() const { return device; }

        //! Returns the elapsed time in miliseconds.
        u32 getElapsedTime() const { return elapsedTime; }

        //! getTimeSpent
        u32 getTimeSpent() const { return timeSpent; }

        //! getNumTimePeriods
        u32 getNumTimePeriods() const { return numTimePeriods; }

        //! getTimePeriodEnded
        bool getTimePeriodEnded() const { return (numTimePeriods == 0) ? false : true; }

        //! getMinLengthSQ
        float getMinLengthSQ() const { return minLengthSQ; }

        float getBaseFriction() const { return baseFriction; }
        const core::vector3df& getGravityNorm() const { return gravityNorm; }
        float getGravityStrength() const { return gravityStrength; }
        core::vector3df getGravityVect() const { return gravityNorm * gravityStrength; }


        //! ----- Set functions:
        void setDevice(IrrlichtDevice* Device) { device = Device; }

        void setBaseFriction(float BaseFriction) { baseFriction = BaseFriction; }
        void setGravityNorm(const core::vector3df& GravityNorm) { gravityNorm = GravityNorm; }
        void setGravityStrength(float GravityStrength) { gravityStrength = GravityStrength; }

        void setGravityVect(const core::vector3df& GravityVect)
        {
            setGravityNorm(core::vector3df(GravityVect).normalize());
            setGravityStrength(GravityVect.getLength());
        }


        //! ----- Functions:
        void restartTimeing()
        {
            elapsedTime = timeSpent = numTimePeriods = 0;
            minLengthSQ = COLL_MIN_LENGTH_SQ;
        }

        void checkTimeing() { if (device && !device->isWindowActive()) restartTimeing(); }


        ICollSelector* addSelector();
        ICollSelector* addSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix);
        ICollSelector* addSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix);

        ICollBBoxSelector* addBBoxSelector();
        ICollBBoxSelector* addBBoxSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix);
        ICollBBoxSelector* addBBoxSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix);

        ICollOctreeSelector* addOctreeSelector();
        ICollOctreeSelector* addOctreeSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix);
        ICollOctreeSelector* addOctreeSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix);

        ICollTerrainSelector* addTerrainSelector();
        ICollTerrainSelector* addTerrainSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix);
        ICollTerrainSelector* addTerrainSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix);
        ICollTerrainSelector* addTerrainSelector(const scene::ITerrainSceneNode* TerrainNode, int LODLevel, const core::matrix4& Matrix);

        ICollSphereSampler* addSphereSampler(const core::vector3df& Scale, const core::vector3df& Origin,
                                             float StandAngle, float SlideAngle, float InverseMass, float BounceFactor);

        ICollEllipsoidSampler* addEllipsoidSampler(const core::vector3df& Scale, const core::vector3df& Origin,
                                                   float StandAngle, float SlideAngle, float InverseMass, float BounceFactor);

        ICollObject* addObject(ICollSelector* Selector, scene::ISceneNode* Node);

        ICollEntity* addEntity(const core::vector3df& Pos, ICollSampler* Sampler, scene::ISceneNode* Node);


        void removeSelector(ICollSelector *Selector);
        void removeSampler(ICollSampler *Sampler);
        void removeObject(ICollObject *Object);
        void removeEntity(ICollEntity *Entity);
        void removeSelectors();
        void removeSamplers();
        void removeObjects();
        void removeEntities();
        void removeAll();


        void update(u32 ElapsedTime);


        bool rayIntersect(const core::vector3df& RayStart, const core::vector3df& RayDir, SCollContact& Contact,
                          float RayLength, int RaySegments, ICollEntity* EmitterEntity, ICollObject* EmitterObject);

        //! Checks if the given segment intersects any of the given objects or entities and finds the closest result.
        bool segmentIntersect(const core::line3df& Segment, SCollContact& Contact,
                              const core::list<CCollObject*>& SelectedObjects, const core::list<CCollEntity*>& SelectedEntities);

        void getDebugData(u32& DebugSimulationTime) { DebugSimulationTime = debugSimulationTime; }

    private:
        //! instances:
        IrrlichtDevice* device;

        //! lists:
        core::list<CCollSelector*> selectors;
        core::list<CCollSampler*> samplers;
        core::list<CCollObject*> objects;
        core::list<CCollEntity*> entities;

        //! timeing:
        u32 elapsedTime;
        u32 timeSpent;
        u32 numTimePeriods;

        //! physics:
        float minLengthSQ;
        float baseFriction;
        core::vector3df gravityNorm;
        float gravityStrength;

        //! debug:
        u32 debugSimulationTime;

}; // end class CCollManager

} // end namespace COLL

#endif // CCOLLMANAGER_H_INCLUDED
