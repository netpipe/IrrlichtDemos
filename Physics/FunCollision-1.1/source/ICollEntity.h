// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef ICOLLENTITY_H_INCLUDED
#define ICOLLENTITY_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


namespace COLL
{

//! ICollEntity class
/// Interface for moveing characters. Entities can collide with objects and with each other.
class ICollEntity : virtual public IReferenceCounted
{
    public:
        //! ----- Set functions:
        //! Activates or deactivates the entity.
        /// That is: makes it able to collide with others and others to collide into this entity.
        virtual void setActive(bool Active) = 0;

        //! Detaches the previous and sets a new sampler for the entity.
        /// This function also makes it possible to share samplers among many entities.
        virtual void setSampler(ICollSampler* Sampler) = 0;

        //! Attaches a scene node to the entity, so that it's position will be automatically
        /// changed every frame. To detach a scene node, simply call "setNode(NULL)"
        virtual void setNode(scene::ISceneNode* Node) = 0;

        //! Sets a new position for the entity. This change is tracked by the collision system.
        /// This means that walls, etc. will impede the movement. If this is not what you want,
        /// use "spawn()" or "teleport()" instead.
        virtual void setPos(const core::vector3df& Pos) = 0;


        //! ----- Get functions:
        //! Returns true if the entity is active.
        virtual bool getActive() const = 0;

        //! Returns true if the entity is sleeping.
        virtual bool getSleeping() const = 0;

        //! Returns the ointer to the actual sampler; NULL if there's no sampler.
        virtual ICollSampler* getSampler() const = 0;

        //! Returns the actual scale factor (= half-sizes) of the entity.
        virtual core::vector3df getScale() const = 0;

        //! Returns true if the entity was collided into something.
        virtual bool getCollided() const = 0;

        //! Retruns the pointer to the scene node attached to the entity.
        virtual scene::ISceneNode* getNode() const = 0;

        //! Returns the current position of the entity.
        virtual const core::vector3df& getPos() const = 0;

        //! Returns the object-space bounding box of the entity.
        virtual core::aabbox3df getBBoxOS() const = 0;

        //! Retruns the world-space bounding box of the entity.
        virtual core::aabbox3df getBBoxWS() const = 0;


        //! Returns the number of contacts (= collision results) the entity had the last frame.
        virtual u32 getNumContacts() const = 0;

        //! Returns the contact selected by the given index.
        virtual SCollContact getContact(u32 Index) const = 0;

        //! Returns the current state of the entity; if it's falling or slideing or standing.
        virtual ECollEntityState getState() const = 0;

        //! Returns the accumulated and normalised normal of all the contacts in "sphere-space".
        virtual const core::vector3df& getNormalResultant() const  = 0;

        //! Retruns the accumulated and normalised normal of all the contacts that are useful for standing in "sphere-space".
        virtual const core::vector3df& getStandResultant() const = 0;


        //! ----- Functions:
        //! Adds a continuous force to the entity.
        /// \param Force: the vector to add
        /// \param WaitSync: normally this value should be true. Only if you don't
        ///                  want to wait for the time period to end, set it to false.
        virtual void addForce(const core::vector3df& Force, bool WaitSync = true) = 0;

        //! Adds an impulse (= a sudden force) to the entity.
        /// \param Impulse: the vector to add
        /// \param WaitSync: normally this value should be true. Only if you don't
        ///                  want to wait for the time period to end, set it to false.
        virtual void addImpulse(const core::vector3df& Impulse, bool WaitSync = true) = 0;

        //! Resets the entity's gravity vector. Useful for jumping.
        virtual void resetGravity() = 0;

        //! Resets the entity's velocity vector.
        virtual void resetVelocity() = 0;

        //! Sets all the forces that would affect the entity to zero.
        virtual void resetForces() = 0;

        //! Sets all the impulses that would affect the entity to zero.
        virtual void resetImpulses() = 0;

        //! Teleports the entity from it's current postion to another one.
        /// The movement is not tracked by the system, so it's possible to teleport through walls, etc.
        /// The velocity, forces, etc. are kept.
        /// \param Pos: the new position to set
        virtual void teleport(const core::vector3df& Pos) = 0;

        //! Spawns the entity at the given postion. Practically resets it.
        /// The movement is not tracked by the system, so it's possible to teleport through walls, etc.
        /// \param Pos: the new position to set
        virtual void spawn(const core::vector3df& Pos) = 0;


        //! Retruns the debug data.
        /// \param DebugSelectedTris: number of triangles the entity has met will be saved into this variable
        /// \param DebugNumIterations: number of iterations it took to calculate the new position
        virtual void getDebugData(int& DebugNumSelectedTris, int& DebugNumIterations) = 0;

        //! Draws the debug data.
        /// \param Driver: pointer to the Irrlicht VideoDriver
        /// \param NormalLength: length of the normals
        virtual void drawDebugData(video::IVideoDriver* Driver, float NormalLength = 10.0f) = 0;

}; // end class ICollEntity

} // end namespace COLL

#endif // ICOLLENTITY_H_INCLUDED
