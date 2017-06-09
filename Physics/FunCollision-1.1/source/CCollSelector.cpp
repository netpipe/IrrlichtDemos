// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#include <irrlicht.h>
using namespace irr;

#include "CCollSelector.h"
#include "CCollCommon.h"
#include "CCollOctree.h"


namespace COLL
{

//! CCollSelector ----------------------------------------------------------------------------------------------------------------------
//! addTriangle
void CCollSelector::addTriangle(const core::triangle3df& Tri, const core::matrix4& Matrix)
{
    // transform triangle if neccessary
    CCollTriangloid trt(Tri);
    if (!Matrix.isIdentity()) trt.transform(Matrix);

    // recalc bbox and plane
    CCollTriangloid tri(trt);
    tri.recalcAll();

    // extend bbox
    if (!tris.getSize()) bbox.reset(tri.getPointA());
    else bbox.addInternalPoint(tri.getPointA());

    bbox.addInternalPoint(tri.getPointB());
    bbox.addInternalPoint(tri.getPointC());

    // set id
    tri.setID(tris.getSize());

    // add to the list
    tris.push_back(tri);
}

//! addMeshBuffer
void CCollSelector::addMeshBuffer(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
{
    if (!MeshBuffer) return;

    // copy all the triangles
    const u16* inds = MeshBuffer->getIndices();

    for (u32 i = 0; i < MeshBuffer->getIndexCount(); i += 3)
    {
        core::vector3df v0 = MeshBuffer->getPosition(inds[i]);
        core::vector3df v1 = MeshBuffer->getPosition(inds[i + 1]);
        core::vector3df v2 = MeshBuffer->getPosition(inds[i + 2]);

        addTriangle(core::triangle3df(v0, v1, v2), Matrix);
    }
}

//! addMesh
void CCollSelector::addMesh(const scene::IMesh* Mesh, const core::matrix4& Matrix)
{
    if (Mesh) for (u32 i = 0; i < Mesh->getMeshBufferCount(); ++i) addMeshBuffer(Mesh->getMeshBuffer(i), Matrix);
}

//! simplify
void CCollSelector::simplify(float Limit)
{
    core::list<CCollTriangloid>::Iterator it;

    for (it = tris.begin(); it != tris.end();)
    {
        if ((*it).degenerated(Limit)) { it = tris.erase(it); continue; }
        ++it;
    }
}

//! collect
bool CCollSelector::collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector)
{
    // only if it's near enough
    if (!isectBoxes(Collector, bbox)) return false;

    // collect only those that instersects the collector
    core::list<CCollTriangloid>::Iterator it = tris.begin();
    for (; it != tris.end(); ++it) if ((*it).isectsBBox(Collector)) List.push_back(*it);

    return List.getSize();
}

//! rayIntersect
bool CCollSelector::rayIntersect(const core::matrix4& InvMatrix, const core::matrix4& Matrix, const core::line3df& Ray, SCollContact& Contact)
{
    core::vector3df trs = Ray.start, tre = Ray.end;
    InvMatrix.transformVect(trs);
    InvMatrix.transformVect(tre);
    core::aabbox3df collector(trs, tre);
    collector.repair();
    core::vector3df trd = (tre - trs).normalize();

    core::list<CCollTriangloid> selected;
    if (!collect(selected, collector)) return false;

    core::list<CCollTriangloid>::Iterator it;
    float dcp, closest = COLL_MAX_DISTANCE; // get rid of inaccurancy bug
    core::vector3df isect;
    SCollContact temp;
    bool found = false;
    for (it = selected.begin(); it != selected.end(); ++it)
    {
        if ((*it).intersect(trs, trd, isect))
        {
            dcp = trs.getDistanceFromSQ(isect);
            if (dcp < closest && frontFacingDir(trd, isect - trs))
            {
                closest = dcp;
                temp.impact = isect;
                temp.tri = *it;
                found = true;
            }
        }
    }

    if (found)
    {
        Matrix.transformVect(Contact.impact, temp.impact);
        Contact.tri.setFrom(temp.tri);
        Contact.tri.transform(Matrix);
        Contact.tri.recalcPlane();
        Contact.normal = Contact.tri.getNorm();
    }

    return found;
}

//! drawDebugData
void CCollSelector::drawDebugData(video::IVideoDriver* Driver, const core::matrix4& Matrix)
{
    if (!Driver) return;

    core::list<CCollTriangloid>::Iterator it;
    for (it = tris.begin(); it != tris.end(); ++it)
    {
        CCollTriangloid trt(*it);
        trt.transform(Matrix);
        Driver->draw3DTriangle(trt.getTri(), video::SColor(255, 0, 255, 255));
    }
}


//! CCollOctreeSelector ----------------------------------------------------------------------------------------------------------------
//! build
void CCollOctreeSelector::build(unsigned int MinTris)
{
    // create new octree
    if (octree) delete octree;
    octree = new CCollOctree(bbox, MinTris);

    // add all the tris
    core::list<CCollTriangloid>::Iterator it = tris.begin();
    for (; it != tris.end(); ++it) octree->addTriangloid(*it);

    tris.clear();
}

//! collect
bool CCollOctreeSelector::collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector)
{
    return octree ? octree->collect(List, Collector) : false;
}

//! drawDebugData
void CCollOctreeSelector::drawDebugData(video::IVideoDriver* Driver, const core::matrix4& Matrix)
{
    if (!Driver || !octree) return;

    core::list<CCollTriangloid> temp;

    octree->collect(temp, bbox);

    core::list<CCollTriangloid>::Iterator it;
    for (it = temp.begin(); it != temp.end(); ++it)
    {
        CCollTriangloid trt(*it);
        trt.transform(Matrix);
        Driver->draw3DTriangle(trt.getTri(), video::SColor(255, 0, 255, 255));
    }
}

//! getDebugData
void CCollOctreeSelector::getDebugData(int& NumNodes, int& NumLevels, int& NumTris)
{
    NumNodes = NumLevels = NumTris = 0;
    if (octree) octree->getDebugData(NumNodes, NumLevels, NumTris);
}


//! CCollBBoxSelector ----------------------------------------------------------------------------------------------------------------
// TODO: kesobb nem kellenek triangle-k, csak contactok!
//! addTriangle
void CCollBBoxSelector::addTriangle(const core::triangle3df& Tri, const core::matrix4& Matrix)
{
    // transform triangle if neccessary
    CCollTriangloid trt(Tri);
    if (!Matrix.isIdentity()) trt.transform(Matrix);

    // extend bbox and do nothing else
    if (firstTri)
    {
        bbox.reset(trt.getPointA());
        firstTri = false;
    }
    else bbox.addInternalPoint(trt.getPointA());

    bbox.addInternalPoint(trt.getPointB());
    bbox.addInternalPoint(trt.getPointC());
}

//! build
void CCollBBoxSelector::build()
{
    /*
                -------- mx
              /          /|
             /          / |
             ----------   |
            |\         |  |
            |   \      |  |
            |      \   | /
            |         \|/
            mn --------
    */
    tris.clear();
    bbox.repair();
    firstTri = true;

    // add to the list
    core::vector3df mn(bbox.MinEdge), mx(bbox.MaxEdge);
    tris.push_back(CCollTriangloid(core::vector3df(mn.X, mx.Y, mn.Z), core::vector3df(mx.X, mn.Y, mn.Z), core::vector3df(mn.X, mn.Y, mn.Z), 0));
    tris.push_back(CCollTriangloid(core::vector3df(mn.X, mx.Y, mn.Z), core::vector3df(mx.X, mx.Y, mn.Z), core::vector3df(mx.X, mn.Y, mn.Z), 1));

    tris.push_back(CCollTriangloid(core::vector3df(mx.X, mx.Y, mn.Z), core::vector3df(mx.X, mn.Y, mx.Z), core::vector3df(mx.X, mn.Y, mn.Z), 2));
    tris.push_back(CCollTriangloid(core::vector3df(mx.X, mx.Y, mn.Z), core::vector3df(mx.X, mx.Y, mx.Z), core::vector3df(mx.X, mn.Y, mx.Z), 3));

    tris.push_back(CCollTriangloid(core::vector3df(mx.X, mx.Y, mx.Z), core::vector3df(mn.X, mn.Y, mx.Z), core::vector3df(mx.X, mn.Y, mx.Z), 4));
    tris.push_back(CCollTriangloid(core::vector3df(mx.X, mx.Y, mx.Z), core::vector3df(mn.X, mx.Y, mx.Z), core::vector3df(mn.X, mn.Y, mx.Z), 5));

    tris.push_back(CCollTriangloid(core::vector3df(mn.X, mx.Y, mx.Z), core::vector3df(mn.X, mn.Y, mn.Z), core::vector3df(mn.X, mn.Y, mx.Z), 6));
    tris.push_back(CCollTriangloid(core::vector3df(mn.X, mx.Y, mx.Z), core::vector3df(mn.X, mx.Y, mn.Z), core::vector3df(mn.X, mn.Y, mn.Z), 7));

    tris.push_back(CCollTriangloid(core::vector3df(mn.X, mx.Y, mx.Z), core::vector3df(mx.X, mx.Y, mn.Z), core::vector3df(mn.X, mx.Y, mn.Z), 8));
    tris.push_back(CCollTriangloid(core::vector3df(mn.X, mx.Y, mx.Z), core::vector3df(mx.X, mx.Y, mx.Z), core::vector3df(mx.X, mx.Y, mn.Z), 9));

    tris.push_back(CCollTriangloid(core::vector3df(mn.X, mn.Y, mn.Z), core::vector3df(mx.X, mn.Y, mx.Z), core::vector3df(mn.X, mn.Y, mx.Z), 10));
    tris.push_back(CCollTriangloid(core::vector3df(mn.X, mn.Y, mn.Z), core::vector3df(mx.X, mn.Y, mn.Z), core::vector3df(mx.X, mn.Y, mx.Z), 11));

    core::list<CCollTriangloid>::Iterator it = tris.begin();
    for (; it != tris.end(); ++it) (*it).recalcAll();
}


//! CCollTerrainSelector ----------------------------------------------------------------------------------------------------------------
//! addFromTerrainNode
void CCollTerrainSelector::addFromTerrainNode(const scene::ITerrainSceneNode* TerrainNode, int LODLevel, const core::matrix4& Matrix)
{
    if (!TerrainNode) return;

	scene::CDynamicMeshBuffer* mb = new scene::CDynamicMeshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
	TerrainNode->getMeshBufferForLOD(*mb, LODLevel);

	if (!mb) return;

    addMeshBuffer(mb, Matrix);
    mb->drop();
}

//! build
void CCollTerrainSelector::build()
{
    // set up parameters
    cells.clear();
    bbox.repair();

    u32 numTris = tris.getSize();
    if (numTris < 2) return;

    // compute cell sizes
    core::list<CCollTriangloid>::Iterator it = tris.begin();

    core::vector2df pa((*it).getPointA().X, (*it).getPointA().Z);
    core::vector2df pb((*it).getPointB().X, (*it).getPointB().Z);
    core::vector2df pc((*it).getPointC().X, (*it).getPointC().Z);

    if      (pa.X == pb.X) cellSize.Y = core::abs_(pa.Y - pb.Y);
    else if (pb.X == pc.X) cellSize.Y = core::abs_(pb.Y - pc.Y);
    else if (pc.X == pa.X) cellSize.Y = core::abs_(pc.Y - pa.Y);

    if      (pa.Y == pb.Y) cellSize.X = core::abs_(pa.X - pb.X);
    else if (pb.Y == pc.Y) cellSize.X = core::abs_(pb.X - pc.X);
    else if (pc.Y == pa.Y) cellSize.X = core::abs_(pc.X - pa.X);

    // how many cells should be stored?
    core::vector3df ext = bbox.getExtent();
    numCells.X = int(ext.X / cellSize.X);
    numCells.Y = int(ext.Z / cellSize.Y);
    u32 maxCells = numCells.X * numCells.Y;

    core::vector2df shift = (cellSize / 2) + core::vector2df(bbox.MinEdge.X, bbox.MinEdge.Z);

    // build cells
    cells.reallocate(maxCells);
    for (u32 i = 0; i < maxCells; ++i) cells[i].numOccupied = 0;

    u32 index, numOccupiedCells = 0;
    for (it = tris.begin(); it != tris.end(); ++it)
    {
        // get the longest edge and use it's center as the reference for the array index
        core::vector2df pa((*it).getPointA().X, (*it).getPointA().Z);
        core::vector2df pb((*it).getPointB().X, (*it).getPointB().Z);
        core::vector2df pc((*it).getPointC().X, (*it).getPointC().Z);

        float lab = pa.getDistanceFrom(pb);
        float lbc = pb.getDistanceFrom(pc);
        float lca = pc.getDistanceFrom(pa);

        core::vector2df ref;
        if (lab > lbc)
        {
            if (lab > lca)  ref = (pa + pb) / 2;
            else            ref = (pc + pa) / 2;
        }
        else
        {
            if (lbc > lca)  ref = (pb + pc) / 2;
            else            ref = (pc + pa) / 2;
        }
        ref = (ref - shift) / cellSize;

        index = int(ref.X) + (int(ref.Y) * numCells.X);

        if (index >= maxCells) continue; // bad index

        int n = cells[index].numOccupied;
        if (n >= 2) continue; // too much occupation for a cell

        cells[index].tri[n].setFrom(*it);

        if (n == 0) ++numOccupiedCells;

        ++cells[index].numOccupied;
    }

    // clean up
    cells.set_used(numOccupiedCells);
    tris.clear();
}

//! collect
bool CCollTerrainSelector::collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector)
{
    // if there are occupied cells
    if (!cells.size()) return false;

    // only if it's near enough
    if (!isectBoxes(Collector, bbox)) return false;

    // repair region extent
    core::vector2di rs, re;
    core::vector2df shift = -(cellSize / 2) + core::vector2df(bbox.MinEdge.X, bbox.MinEdge.Z);
    rs.X = int((Collector.MinEdge.X - shift.X) / cellSize.X);
    rs.Y = int((Collector.MinEdge.Z - shift.Y) / cellSize.Y);
    re.X = int((Collector.MaxEdge.X - shift.X) / cellSize.X);
    re.Y = int((Collector.MaxEdge.Z - shift.Y) / cellSize.Y);
    // swap if needed
    if (re.X < rs.X) { int sw = re.X; re.X = rs.X; rs.X = sw; }
    if (re.Y < rs.Y) { int sw = re.Y; re.Y = rs.Y; rs.Y = sw; }
    // increase area
    rs -= core::vector2di(2);
    re += core::vector2di(1);
    // check array limits
    rs.X = (rs.X < 0) ? 0 : rs.X;
    rs.Y = (rs.Y < 0) ? 0 : rs.Y;
    re.X = (re.X > numCells.X) ? numCells.X : re.X;
    re.Y = (re.Y > numCells.Y) ? numCells.Y : re.Y;

    // collect only those that are occupied and instersects the collector
    u32 index;
    for (int j = rs.Y; j < re.Y; ++j)
    {
        for (int i = rs.X; i < re.X; ++i)
        {
            index = i + (j * numCells.X);

            for (int n = 0; n < cells[index].numOccupied; ++n)
            {
                if (cells[index].tri[n].isectsBBox(Collector)) List.push_back(cells[index].tri[n]);
            }
        }
    }

    return List.getSize();
}

//! drawDebugData
void CCollTerrainSelector::drawDebugData(video::IVideoDriver* Driver, const core::matrix4& Matrix)
{
    if (!Driver || !cells.size()) return;

    for (u32 i = 0; i < cells.size(); ++i)
    {
        for (int j = 0; j < cells[i].numOccupied; ++j)
        {
            CCollTriangloid trt(cells[i].tri[j]);
            trt.transform(Matrix);
            Driver->draw3DTriangle(trt.getTri(), video::SColor(255, 0, 255, 255));
        }
    }
}

//! getDebugData
void CCollTerrainSelector::getDebugData(int& NumCells, int& NumTris)
{
    NumCells = cells.size();
    NumTris = 0;
    for (u32 i = 0; i < cells.size(); ++i) NumTris += cells[i].numOccupied;
}

} // end namespace COLL
