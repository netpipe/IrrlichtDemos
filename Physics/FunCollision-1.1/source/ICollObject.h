// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef ICOLLOBJECT_H_INCLUDED
#define ICOLLOBJECT_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


namespace COLL
{

//! forward decl.
class ICollSelector;


//! ICollObject class
/// Interface for static objects e.g. a house, a tree, a level, etc.
/// Objects cannot collide with anything, entities can collide with objects.
class ICollObject : virtual public IReferenceCounted
{
    public:
        //! ----- Get functions:
        //! Returns true if the object is active.
        virtual bool getActive() const = 0;

        //! Returns a pointer to the attached selector.
        virtual ICollSelector* getSelector() const = 0;

        //! Returns a pointer to the attached scene node.
        virtual scene::ISceneNode* getNode() const = 0;

        //! Returns the object-space bounding box.
        virtual const core::aabbox3df getBBoxOS() const = 0;

        //! Returns the world-space bounding box.
        virtual const core::aabbox3df getBBoxWS() const = 0;


        //! ----- Set functions:
        //! Activates or deactivates the object. That is: makes entities able or unable to collide with it.
        virtual void setActive(bool Active) = 0;

        //! Sets a new selector for the object.
        /// This function also makes it possible to share selectors among many objects.
        virtual void setSelector(ICollSelector* Selector) = 0;

        //! Binds a new scenenode to the object.
        virtual void setNode(scene::ISceneNode* Node) = 0;


        //! ----- Functions:
        //! Resets the binded node: re-reads the matrices from the it
        /// Note that this change is not tracked by the system!
        virtual void reset() = 0;

        //! Draws the debug data.
        /// \param Driver: pointer to the Irrlicht VideoDriver
        virtual void drawDebugData(video::IVideoDriver* Driver) = 0;

}; // end class ICollObject

} // end namespace COLL


#endif // ICOLLOBJECT_H_INCLUDED
