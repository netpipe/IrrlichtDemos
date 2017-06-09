// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef CCOLLSELECTOR_H_INCLUDED
#define CCOLLSELECTOR_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "ICollSelector.h"
#include "CCollCommon.h"
#include "CCollOctree.h"


namespace COLL
{

//! CCollSelector class
class CCollSelector : virtual public ICollSelector
{
    public:
        //! empty constructor
        CCollSelector() { }

        //! constructor
        CCollSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
        { addMeshBuffer(MeshBuffer, Matrix); }

        //! constructor
        CCollSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix)
        { addMesh(Mesh, Matrix); }

        //! destructor
        virtual ~CCollSelector() { }

        //! ----- Functions:
        //! getBBox functions
        const core::aabbox3df& getBBoxOS() const { return bbox; }
        const core::aabbox3df getBBoxWS(const core::matrix4& Matrix) const
        { core::aabbox3df ret(bbox.MinEdge, bbox.MaxEdge); Matrix.transformBoxEx(ret); return ret; }


        virtual void addTriangle(const core::triangle3df& Tri, const core::matrix4& Matrix);
        void addMeshBuffer(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix);
        void addMesh(const scene::IMesh* Mesh, const core::matrix4& Matrix);
        void simplify(float Limit);

        /// Collects tris that intersects with the given bounding box.
        virtual bool collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector);

        //! rayIntersect
        bool rayIntersect(const core::matrix4& InvMatrix, const core::matrix4& Matrix, const core::line3df& Ray, SCollContact& Contact);

        //! drawDebugData
        virtual void drawDebugData(video::IVideoDriver* Driver, const core::matrix4& Matrix);

        virtual void getDebugData(int& NumTris) { NumTris = tris.getSize(); }

    protected:
        //! vars:
        core::aabbox3df bbox;
        core::list<CCollTriangloid> tris;

}; // end class CCollSelector


//! CCollOctreeSelector class
class CCollOctreeSelector : virtual public ICollOctreeSelector, public CCollSelector
{
    public:
        //! empty constructor
        CCollOctreeSelector() : CCollSelector(), octree(NULL) { }

        //! constructor
        CCollOctreeSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
        : CCollSelector(MeshBuffer, Matrix), octree(NULL) { }

        //! constructor
        CCollOctreeSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix)
        : CCollSelector(Mesh, Matrix), octree(NULL) { }

        //! destructor
        ~CCollOctreeSelector() { if (octree) delete octree; octree = NULL; }

        //! ----- Functions:
        void build(unsigned int MinTris);

        //! collect
        bool collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector);

        //! drawDebugData
        void drawDebugData(video::IVideoDriver* Driver, const core::matrix4& Matrix);

        //! getDebugData
        void getDebugData(int& NumNodes, int& NumLevels, int& NumTris);

    protected:
        //! vars:
        CCollOctree* octree;

}; // end class CCollOctreeSelector


//! CCollBBoxSelector class
class CCollBBoxSelector : virtual public ICollBBoxSelector, public CCollSelector
{
    public:
        //! empty constructor
        CCollBBoxSelector() : CCollSelector(), firstTri(true) { }

        //! constructor
        CCollBBoxSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
        : CCollSelector(MeshBuffer, Matrix), firstTri(true) { }

        //! constructor
        CCollBBoxSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix)
        : CCollSelector(Mesh, Matrix), firstTri(true) { }

        //! destructor
        ~CCollBBoxSelector() { }

        //! ----- Functions:
        void addTriangle(const core::triangle3df& Tri, const core::matrix4& Matrix);

        void build();

    private:
        //! vars:
        bool firstTri;

}; // end class CCollBBoxSelector


//! CCollTerrainSelector class
class CCollTerrainSelector : virtual public ICollTerrainSelector, public CCollSelector
{
    public:
        //! empty constructor
        CCollTerrainSelector() : CCollSelector(), numCells(), cellSize() { }

        //! constructor
        CCollTerrainSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
            : CCollSelector(MeshBuffer, Matrix), numCells(), cellSize() { }

        //! constructor
        CCollTerrainSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix)
            : CCollSelector(Mesh, Matrix), numCells(), cellSize() { }

        //! constructor
        CCollTerrainSelector(const scene::ITerrainSceneNode* TerrainNode, int LODLevel, const core::matrix4& Matrix)
            : numCells(), cellSize()
        { addFromTerrainNode(TerrainNode, LODLevel, Matrix); }

        //! destructor
        ~CCollTerrainSelector() { }

        //! ----- Functions:
        void addFromTerrainNode(const scene::ITerrainSceneNode* TerrainNode, int LODLevel, const core::matrix4& Matrix);

        void build();

        //! collect
        bool collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector);

        //! drawDebugData
        void drawDebugData(video::IVideoDriver* Driver, const core::matrix4& Matrix);

        //! getDebugData
        void getDebugData(int& NumCells, int& NumTris);

    private:
        //! vars:
        core::vector2di numCells;
        core::vector2df cellSize;

        struct SCollTerrainCell
        {
            SCollTerrainCell() { numOccupied = 0; }
            CCollTriangloid tri[2];
            int numOccupied;
        };

        core::array<SCollTerrainCell> cells;

}; // end class CCollTerrainSelector


} // end namespace COLL

#endif // CCOLLSELECTOR_H_INCLUDED
