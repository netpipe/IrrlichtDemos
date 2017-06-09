// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef CCOLLENTITY_H_INCLUDED
#define CCOLLENTITY_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "ICollEntity.h"
#include "CCollCommon.h"
#include "CCollObject.h"


namespace COLL
{

//! forward decl.
class CCollManager;
class CCollSampler;


//! CCollEntity class
class CCollEntity : virtual public ICollEntity
{
    private:
        //! vars:
        CCollManager* manager;
        CCollSampler* sampler;
        scene::ISceneNode* node;
        bool active, collided;
        core::vector3df pos, prevPos;
        core::line3df path;
        core::vector3df standResultant, normalResultant;
        core::array<SCollContact> contacts;
        core::vector3df velocity, force, impulse, gravity;
        ECollEntityState state;
        u32 sleeping;

        //! debug:
        int debugNumSelectedTris;
        int debugNumIterations;

    public:
        //! constructor
        CCollEntity(CCollManager* Manager, const core::vector3df& Pos, CCollSampler* Sampler, scene::ISceneNode* Node)
            : manager(Manager), sampler(NULL), node(Node), active(true)
        {
            setSampler(Sampler);
            spawn(Pos);
        }

        //! destructor
        virtual ~CCollEntity() { if (sampler) sampler->drop(); };


        //! ----- Set functions:
        void setActive(bool Active) { active = Active; }

        //! setAwake
        void setAwake() { sleeping = 0; }

        void setSampler(ICollSampler* Sampler);

        //! setCollided
        void setCollided() { collided = true; }

        void setNode(scene::ISceneNode* Node);
        void setPos(const core::vector3df& Pos);


        //! ----- Get functions:
        bool getActive() const { return active; }
        bool getSleeping() const { return sleeping; }
        CCollSampler* getSampler() const { return sampler; }
        core::vector3df getScale() const;
        bool getCollided() const { return collided; }
        scene::ISceneNode* getNode() const { return node; }
        const core::vector3df& getPos() const { return pos; }

        //! Retruns the path from the previous position to the current one.
        const core::line3df& getPath() const { return path; }

        core::aabbox3df getBBoxOS() const;
        core::aabbox3df getBBoxWS() const;

        //! Returns the world-space bounding box of the swept entity based on the given ray.
        core::aabbox3df getTunnel(const core::line3df& Ray) const;

        //! Returns the transformed tunnel of the swept entity for collecting tris based on the given ray and inverse matrix.
        core::aabbox3df getCollector(const core::line3df& Ray, const core::matrix4& InvMatrix) const;

        u32 getNumContacts() const { return contacts.size(); }
        SCollContact getContact(u32 Index) const { return contacts[Index]; }
        ECollEntityState getState() const { return state; }

        const core::vector3df& getNormalResultant() const { return normalResultant; }
        const core::vector3df& getStandResultant() const { return standResultant; }


        //! ----- Functions:
        void addForce(const core::vector3df& Force, bool WaitSync);
        void addImpulse(const core::vector3df& Impulse, bool WaitSync);

        void resetGravity() { gravity = core::vector3df(); }
        void resetVelocity() { velocity = core::vector3df(); }
        void resetForces() { force = core::vector3df(); }
        void resetImpulses() { impulse = core::vector3df(); }

        //! clean
        void clean();
        void teleport(const core::vector3df& Pos);
        void spawn(const core::vector3df& Pos);


        //! Gets the position of the entity from the binded node (if exists); also clears the collision contacts, updates physic vars, etc.
        void prepare();

        //! Updates the position of the binded node (if exists) after the collisions, etc.
        void update();

        //! Collects contacts
        bool collect(const core::line3df& RayOS, const core::list<CCollObject*>& SelectedObjects,
                     const core::list<CCollEntity*>& SelectedEntities, SCollContact& ClosestContact);

        //! Checks and responds if the entity collides with an object
        void collide(const core::list<CCollObject*>& SelectedObjects, const core::list<CCollEntity*>& SelectedEntities);

        //! rayIntersect
        bool rayIntersect(const core::line3df& Ray, SCollContact& Contact);


        void getDebugData(int& DebugNumSelectedTris, int& DebugNumIterations);
        void drawDebugData(video::IVideoDriver* Driver, float NormalLength);

}; // end class CCollEntity

} // end namespace COLL

#endif // CCOLLENTITY_H_INCLUDED
