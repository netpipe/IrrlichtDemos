// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef CCOLLOBJECT_H_INCLUDED
#define CCOLLOBJECT_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "ICollObject.h"
#include "CCollSelector.h"


namespace COLL
{

//! Forward decl.
class CCollManager;


//! CCollObject class
class CCollObject : virtual public ICollObject
{
    public:
        //! constructor
        CCollObject(CCollManager* Manager, CCollSelector* Selector, scene::ISceneNode* Node)
            : manager(Manager), selector(NULL), node(Node), active(true)
        {
            setSelector(Selector);
            reset();
        }

        //! destructor
        virtual ~CCollObject() { if (selector) selector->drop(); };


        //! ----- Get functions:
        bool getActive() const { return active; }
        CCollSelector* getSelector() const { return selector; }
        scene::ISceneNode* getNode() const { return node; }
        const core::aabbox3df getBBoxOS() const { return selector ? selector->getBBoxOS() : core::aabbox3df(); }
        const core::aabbox3df getBBoxWS() const { return selector ? selector->getBBoxWS(matrix) : core::aabbox3df(); }

        //! Retruns the applied transformation matrix.
        const core::matrix4& getMatrix() const { return matrix; }

        //! Retruns the inverse transformation matrix.
        const core::matrix4& getInvMatrix() const { return invMatrix; }


        //! ----- Set functions:
        void setActive(bool Active) { active = Active; }
        void setSelector(ICollSelector* Selector)
        {
            // detach old selector
            if (selector) selector->drop();

            // attach new one
            CCollSelector* s = NULL;
            if (Selector)
            {
                s = dynamic_cast<CCollSelector *>(Selector);
                s->grab();
            }

            // update
            selector = s;
            reset();
        }

        void setNode(scene::ISceneNode* Node) { node = Node; reset(); }


        //! ----- Functions:
        void reset()
        {
            if (!node) return;

            node->updateAbsolutePosition();
            matrix = node->getAbsoluteTransformation();
            matrix.getInverse(invMatrix);
        }

        //! collect
        bool collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector)
        { return selector ? selector->collect(List, Collector) : false; }

        //! rayIntersect
        bool rayIntersect(const core::line3df& Ray, SCollContact& Contact)
        { return selector ? selector->rayIntersect(invMatrix, matrix, Ray, Contact) : false; }

        void drawDebugData(video::IVideoDriver* Driver)
        { if (selector) selector->drawDebugData(Driver, matrix); }


    private:
        //! instances:
        CCollManager* manager;

        //! vars:
        CCollSelector* selector;
        scene::ISceneNode* node;
        core::matrix4 matrix, invMatrix;
        bool active;

}; // end class CCollObject

} // end namespace COLL

#endif // CCOLLOBJECT_H_INCLUDED
