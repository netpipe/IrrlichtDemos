// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef ICOLLSELECTOR_H_INCLUDED
#define ICOLLSELECTOR_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


namespace COLL
{

//! ICollSelector class
/// Interface for smaller objects like a table, a tree, etc.
/// Holds and selects triangloids for the collision detection.
class ICollSelector : virtual public IReferenceCounted
{
    public:
        //! ----- Functions:
        //! Adds a triangle (transformed with the specified matrix).
        virtual void addTriangle(const core::triangle3df& Tri, const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        //! Adds a meshbuffer (transformed with the specified matrix).
        virtual void addMeshBuffer(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        //! Adds a mesh (transformed with the specified matrix).
        virtual void addMesh(const scene::IMesh* Mesh, const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        //! Simplifies the geometry, if it's possible.
        /// This function should be called after adding data and before calling "build()".
        virtual void simplify(float Limit = core::ROUNDING_ERROR_f32) = 0;

        //! getDebugData
        virtual void getDebugData(int& NumTris) = 0;

}; // end class ICollSelector


//! ICollOctreeSelector class
/// Interface for larger objects like a level, etc.
/// Holds and selects triangloids for the collision detection by selecting from an octree.
class ICollOctreeSelector : virtual public ICollSelector
{
    public:
        //! ----- Functions:
        //! Builds the tree structure. This function should be called after adding data.
        /// \param MinTris: minimum number of triangles a branch should contain.
        virtual void build(unsigned int MinTris = 128) = 0;

}; // end class ICollOctreeSelector


//! ICollBBoxSelector class
/// For smaller objects or box-like objects: a wardrobe, etc.
/// Holds and selects only a couple of triangloids for the collision detection.
class ICollBBoxSelector : virtual public ICollSelector
{
    public:
        //! ----- Functions:
        //! Builds the bounding box selector. This function should be called after adding data.
        virtual void build() = 0;

}; // end class ICollBBoxSelector


//! ICollTerrainSelector class
/// Holds and selects triangloids for the collision detection by selecting from a 2D grid structure.
class ICollTerrainSelector : virtual public ICollSelector
{
    public:
        //! ----- Functions:
        //! Adds terrain data from an Irrlicht terrain scene node.
        /// \param TerrainNode: the terrain scene node to get data from
        /// \param LODLevel: the level of detail you need (0 is the highest)
        virtual void addFromTerrainNode(const scene::ITerrainSceneNode* TerrainNode, int LODLevel = 0,
                                        const core::matrix4& Matrix = core::IdentityMatrix) = 0;

        //! Builds the terrain selector. This function should be called after adding data.
        virtual void build() = 0;

}; // end class ICollTerrainSelector


} // end namespace COLL


#endif // ICOLLSELECTOR_H_INCLUDED
