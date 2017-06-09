// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef ICOLLMANAGER_H_INCLUDED
#define ICOLLMANAGER_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "CCollCommon.h"


namespace COLL
{

//! forward decl.
class ICollSelector;
class ICollOctreeSelector;
class ICollBBoxSelector;
class ICollTerrainSelector;
class ICollSampler;
class ICollSphereSampler;
class ICollEllipsoidSampler;
class ICollObject;
class ICollEntity;


//! ICollManager class
/// Interface for the collision manager class.
class ICollManager : virtual public IReferenceCounted
{
    public:
        //! ----- Get functions:
        //! Retruns a pointer to the Irrlicht device if it was set.
        virtual IrrlichtDevice* getDevice() const = 0;

        //! Returns the base friction.
        virtual float getBaseFriction() const = 0;

        //! Returns the normal (direction) of the gravity.
        virtual const core::vector3df& getGravityNorm() const = 0;

        //! Returns the strength of the gravity.
        virtual float getGravityStrength() const = 0;

        //! Returns the full vector of the gravity.
        virtual core::vector3df getGravityVect() const = 0;


        //! ----- Set functions:
        //! Sets the Irrlicht device.
        virtual void setDevice(IrrlichtDevice* Device) = 0;

        //! Sets the base friction. Default value is 0.92f.
        /// \param BaseFriction: ranges from 0.0f to 1.0f, but normally it should be closely around 0.95f
        virtual void setBaseFriction(float BaseFriction) = 0;

        //! Sets the gravity normal (direction). Default value is (0, -1, 0).
        /// \param GravityNorm: it should be normalized.
        virtual void setGravityNorm(const core::vector3df& GravityNorm) = 0;

        //! Sets the gravity strength. Default value is 0.0f.
        /// \param GravityStrength: ranges from 0.0f (no gravity) to 1.0f (extreme). Normal gravity is 0.01f.
        virtual void setGravityStrength(float GravityStrength) = 0;

        //! Sets the gravity vector (both normal and strength).
        virtual void setGravityVect(const core::vector3df& GravityVect) = 0;


        //! ----- Functions:
        //! Restarts the timeing related variables.
        /// Useful for example when the window was first de- and then re-activated, or when a game level starts.
        virtual void restartTimeing() = 0;

        //! Checks if the window was deactivated and restarts the timeing related variables.
        virtual void checkTimeing() = 0;


        //! Adds a simple mesh triangle selector to the list.
        /// The returned pointer should not be dropped or deleted, it's only for manipulation!
        /// Use removeSelector() command if you want to detach and remove this selector.
        /// \param MeshBuffer: a meshbuffer to put into the selector
        /// \param Mesh: a mesh to put into the selector
        /// \param Matrix: transformation matrix to apply on the geometry
        /// \return the pointer to the selector
        virtual ICollSelector* addSelector() = 0;

        virtual ICollSelector* addSelector(const scene::IMeshBuffer* MeshBuffer,
                                           const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        virtual ICollSelector* addSelector(const scene::IMesh* Mesh,
                                           const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        //! Adds a bounding-box selector to the list.
        /// The returned pointer should not be dropped or deleted, it's only for manipulation!
        /// Use removeSelector() command if you want to detach and remove this selector.
        /// \param MeshBuffer: a meshbuffer to put into the selector
        /// \param Mesh: a mesh to put into the selector
        /// \param Matrix: transformation matrix to apply on the geometry
        /// \return the pointer to the selector
        virtual ICollBBoxSelector* addBBoxSelector() = 0;

        virtual ICollBBoxSelector* addBBoxSelector(const scene::IMeshBuffer* MeshBuffer,
                                                   const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        virtual ICollBBoxSelector* addBBoxSelector(const scene::IMesh* Mesh,
                                                   const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        //! Adds an octree mesh triangle selector to the list.
        /// The returned pointer should not be dropped or deleted, it's only for manipulation!
        /// Use removeSelector() command if you want to detach and remove this selector.
        /// \param MeshBuffer: a meshbuffer to put into the selector
        /// \param Mesh: a mesh to put into the selector
        /// \param Matrix: transformation matrix to apply on the geometry
        /// \return the pointer to the selector
        virtual ICollOctreeSelector* addOctreeSelector() = 0;

        virtual ICollOctreeSelector* addOctreeSelector(const scene::IMeshBuffer* MeshBuffer,
                                                       const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        virtual ICollOctreeSelector* addOctreeSelector(const scene::IMesh* Mesh,
                                                       const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        //! Adds a terrain triangle selector to the list.
        /// The returned pointer should not be dropped or deleted, it's only for manipulation!
        /// Use removeSelector() command if you want to detach and remove this selector.
        /// \param MeshBuffer: a meshbuffer to put into the selector
        /// \param Mesh: a mesh to put into the selector
        /// \param TerrainNode: the terrain scene node to get the meshbuffer from
        /// \param LODLevel: the desired LOD level to get the meshbuffer from (0 is the highest level)
        /// \return the pointer to the selector
        virtual ICollTerrainSelector* addTerrainSelector() = 0;

        virtual ICollTerrainSelector* addTerrainSelector(const scene::IMeshBuffer* MeshBuffer,
                                                         const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        virtual ICollTerrainSelector* addTerrainSelector(const scene::IMesh* Mesh,
                                                         const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        virtual ICollTerrainSelector* addTerrainSelector(const scene::ITerrainSceneNode* TerrainNode, int LODLevel = 0,
                                                         const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        //! Adds a sphere shape sampler to the list.
        /// The returned pointer should not be dropped or deleted, it's only for manipulation!
        /// Use removeSampler() command if you want to detach and remove this sampler.
        /// \param Scale: the scale of the shape
        /// \param Origin: the relative translation
        /// \param StandAngle: ranges from 0 to 90 degrees, determines the standing range.
        ///        It should be smaller than SlideValue. Default is 45.0f.
        /// \param SlideAngle: ranges from 0 to 90 degrees, determines the sliding on slopes.
        ///        It should be larger than StandValue. Default is 60.0f.
        /// \param InverseMass: ranges from 0.0f (highest) to 1.0f (lowest), determines the mass.
        ///        0.0f is inmovable, 1.0f is like feather. Default is 0.5f.
        /// \param BounceFactor: ranges from 0.0f to 1.0f, determines the bouncing.
        ///        0.0f is for no bouncing, 1.0f is for maximum bouncing. Default is 0.0f.
        /// \return the pointer to the sampler
        virtual ICollSphereSampler* addSphereSampler(const core::vector3df& Scale = core::vector3df(1),
                                                     const core::vector3df& Origin = core::vector3df(),
                                                     float StandAngle = 45.0f, float SlideAngle = 60.0f,
                                                     float InverseMass = 0.5f, float BounceFactor = 0.0f) = 0;

        //! Adds an axis-aligned ellipsoid shape sampler to the list.
        /// The returned pointer should not be dropped or deleted, it's only for manipulation!
        /// Use removeSampler() command if you want to detach and remove this sampler.
        /// \param Scale: the scale of the shape
        /// \param Origin: the relative translation
        /// \param StandAngle: ranges from 0 to 90 degrees, determines the standing range.
        ///        It should be smaller than SlideValue. Default is 45.0f.
        /// \param SlideAngle: ranges from 0 to 90 degrees, determines the sliding on slopes.
        ///        It should be larger than StandValue. Default is 60.0f.
        /// \param InverseMass: ranges from 0.0f (highest) to 1.0f (lowest), determines the mass.
        ///        0.0f is inmovable, 1.0f is like feather. Default is 0.5f.
        /// \param BounceFactor: ranges from 0.0f to 1.0f, determines the bouncing.
        ///        0.0f is for no bouncing, 1.0f is for maximum bouncing. Default is 0.0f.
        /// \return the pointer to the sampler
        virtual ICollEllipsoidSampler* addEllipsoidSampler(const core::vector3df& Scale = core::vector3df(1),
                                                           const core::vector3df& Origin = core::vector3df(),
                                                           float StandAngle = 45.0f, float SlideAngle = 60.0f,
                                                           float InverseMass = 0.5f, float BounceFactor = 0.0f) = 0;

        //! Adds a static collision object to the list.
        /// The returned pointer should not be dropped or deleted, it's only for manipulation!
        /// Use removeObject() command if you want to remove this object.
        /// \param Selector: the triangle selector to attach to the object
        /// \param Node: the scenenode to attach to the object
        /// \return the pointer to the object
        virtual ICollObject* addObject(ICollSelector* Selector, scene::ISceneNode* Node = NULL) = 0;

        //! Adds a dynamic collision entity to the list.
        /// The returned pointer should not be dropped or deleted, it's only for manipulation!
        /// Use removeEntity() command if you want to remove this entity.
        /// \param Sampler: the shape sampler to attach to the entity
        /// \param Node: the scenenode to attach to the entity
        /// \return the pointer to the entity
        virtual ICollEntity* addEntity(const core::vector3df& Pos, ICollSampler* Sampler, scene::ISceneNode* Node = NULL) = 0;


        //! Detaches from all the objects that uses it, removes from the list and drops the selector.
        virtual void removeSelector(ICollSelector *Selector) = 0;

        //! Detaches from all the entities that uses it, removes from the list and drops the sampler.
        virtual void removeSampler(ICollSampler *Sampler) = 0;

        //! Removes from the list and drops the object.
        virtual void removeObject(ICollObject *Object) = 0;

        //! Removes from the list and drops the entity.
        virtual void removeEntity(ICollEntity *Entity) = 0;

        //! Detaches all the selectors, clears the list and drops them.
        virtual void removeSelectors() = 0;

        //! Detaches all the samplers, clears the list and drops them.
        virtual void removeSamplers() = 0;

        //! Clears the list and drops the objects.
        virtual void removeObjects() = 0;

        //! Clears the list and drops the entities.
        virtual void removeEntities() = 0;

        //! Clears all the lists and drops all the items.
        virtual void removeAll() = 0;


        /// Updates the collision simulation. You should call it in your main loop every frame.
        /// \param ElapsedTime: the time passed from the last call in milliseconds
        virtual void update(u32 ElapsedTime) = 0;


        /// Checks if the given ray intersects any of the objects or entities and finds the closest result.
        /// \param RayStart: the start-point of the ray
        /// \param RayDir: the direction vector of the ray
        /// \param Contact: if any intersection was found, this variable will contain all the information about it
        /// \param RayLength: how far we should be looking for?
        /// \param RaySegments: how many smaller segments the ray should be divided to?
        /// \param EmitterEntity: pointer to the entity that is excluded from investigation
        /// \param EmitterObject: pointer to the object that is excluded from investigation
        /// \return True, if an intersection was found
        virtual bool rayIntersect(const core::vector3df& RayStart, const core::vector3df& RayDir, SCollContact& Contact,
                                  float RayLength = 3000.0f, int RaySegments = 20.0f,
                                  ICollEntity* EmitterEntity = NULL, ICollObject* EmitterObject = NULL) = 0;

        //! getDebugData
        /// \param DebugSimulationTime: the amount of milliseconds the simulation has taken will be saved into this variable
        virtual void getDebugData(u32& DebugSimulationTime) = 0;

}; // end class ICollManager

} // end namespace COLL


#endif // ICOLLMANAGER_H_INCLUDED
