// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef CCOLLOCTREE_H_INCLUDED
#define CCOLLOCTREE_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


#include "CCollCommon.h"

namespace COLL
{

//! CCollOctree class
/// Hierarchy tree for faster selection of the triangles.
class CCollOctree
{
    public:
        //! constructor
        CCollOctree(const core::aabbox3df& BBox, unsigned int MinTris = 128);

        //! destructor
        ~CCollOctree();

        /// Adds a triangloid to this branch or it's child branches.
        bool addTriangloid(const CCollTriangloid& Tri);

        //! collect (for inner use)
        /// Collects tris that intersects the given bounding box.
        bool collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector);

        //! Returnes the debug data.
        /// \param NumNodes: the number of nodes created
        /// \param NumLevels: the number of levels created
        /// \param NumTris: the number of triangles added
        void getDebugData(int& NumNodes, int& NumLevels, int& NumTris);

    private:
        //! passToBranches (for inner use)
        bool passToBranches(const CCollTriangloid& Leaf);

        //! addAllToList (for inner use)
        void addAllToList(core::list<CCollTriangloid>& List);

        //! getNumNodes (for inner use)
        void getNumNodes(int &NumNodes);

        //! getNumLevels (for inner use)
        void getNumLevels(int &NumLevels, int CurrentLevel);

        //! getNumTris (for inner use)
        void getNumTris(int &NumTris);

        //! vars
        unsigned int minTris;
        core::aabbox3df bbox;
        core::vector3df center, half;
        CCollOctree* branches[8];
        core::list<CCollTriangloid> tris;

}; // end class CCollOctree

} // end namespace COLL

#endif // CCOLLOCTREE_H_INCLUDED
