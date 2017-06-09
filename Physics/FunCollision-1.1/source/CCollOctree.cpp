// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#include <irrlicht.h>
using namespace irr;

#include "CCollOctree.h"


namespace COLL
{

//! constructor
CCollOctree::CCollOctree(const core::aabbox3df& BBox, unsigned int MinTris)
{
    bbox = BBox;
    center = bbox.getCenter();
    half = bbox.getExtent() / 2;
    minTris = MinTris;

    for (int i = 0; i < 8; ++i) branches[i] = NULL;
}

//! destructor
CCollOctree::~CCollOctree()
{
    for (int i = 0; i < 8; ++i)
    {
        if (branches[i]) delete branches[i];
        branches[i] = NULL;
    }
}

//! addTriangloid
bool CCollOctree::addTriangloid(const CCollTriangloid& Tri)
{
    if (!Tri.insideBBox(bbox)) return false;

    // start adding tris to branches only after the minimum amount reached
    if (tris.getSize() >= minTris) if (passToBranches(Tri)) return true;

    // otherwise, add it to the list
    tris.push_back(Tri);
    return true;
}

//! collect
bool CCollOctree::collect(core::list<CCollTriangloid>& List, const core::aabbox3df& Collector)
{
    // add every tri if the branch is inside the collector bbox
    if (bbox.isFullInside(Collector))
    {
        addAllToList(List);
        return true;
    }
    else // add only those that intersects with the collector
    if (isectBoxes(bbox, Collector))
    {
        core::list<CCollTriangloid>::Iterator it = tris.begin();
        for (; it != tris.end(); ++it) if ((*it).isectsBBox(Collector)) List.push_back(*it);

        for (int i = 0; i < 8; ++i) if (branches[i]) branches[i]->collect(List, Collector);

        return List.getSize();
    }

    return false;
}

//! passToBranches
bool CCollOctree::passToBranches(const CCollTriangloid& Tri)
{
    core::vector3df mn, mx, tmn = Tri.getBBox().MinEdge, tmx = Tri.getBBox().MaxEdge;

    mn.X = tmn.X < center.X ? 0 : 1;
    mn.Y = tmn.Y < center.Y ? 0 : 1;
    mn.Z = tmn.Z < center.Z ? 0 : 1;
    mx.X = tmx.X < center.X ? 0 : 1;
    mx.Y = tmx.Y < center.Y ? 0 : 1;
    mx.Z = tmx.Z < center.Z ? 0 : 1;

    // doesn't fit into any of the branches
    if (mn.X != mx.X || mn.Y != mx.Y || mn.Z != mx.Z) return false;

    // fits exactly into this branch
    int branchID = int(mn.X + (mn.Y * 2) + (mn.Z * 4));

    // create the branch, if not exists
    if (!branches[branchID])
    {
        core::vector3df mnhalf = mn * half;
        branches[branchID] = new CCollOctree(core::aabbox3df(bbox.MinEdge + mnhalf, center + mnhalf), minTris);
    }

    if (branches[branchID]->addTriangloid(Tri)) return true;

    return false;
}

//! addAllToList
void CCollOctree::addAllToList(core::list<CCollTriangloid>& List)
{
    core::list<CCollTriangloid>::Iterator it = tris.begin();
    for (; it != tris.end(); ++it) List.push_back(*it);

    for (int i = 0; i < 8; ++i) if (branches[i]) branches[i]->addAllToList(List);
}

//! getDebugData
void CCollOctree::getDebugData(int& NumNodes, int& NumLevels, int& NumTris)
{
    NumNodes = 0;
    getNumNodes(NumNodes);

    NumLevels = 0;
    getNumLevels(NumLevels, 0);

    NumTris = 0;
    getNumTris(NumTris);
}

//! getNumNodes
void CCollOctree::getNumNodes(int &NumNodes)
{
    ++NumNodes;

    for (int i = 0; i < 8; ++i) if (branches[i]) branches[i]->getNumNodes(NumNodes);
}

//! getNumLevels
void CCollOctree::getNumLevels(int &NumLevels, int CurrentLevel)
{
    NumLevels = CurrentLevel > NumLevels ? CurrentLevel : NumLevels;

    for (int i = 0; i < 8; ++i) if (branches[i]) branches[i]->getNumLevels(NumLevels, CurrentLevel + 1);
}

//! getNumTris
void CCollOctree::getNumTris(int &NumTris)
{
    NumTris += tris.getSize();

    for (int i = 0; i < 8; ++i) if (branches[i]) branches[i]->getNumTris(NumTris);
}

} // end namespace COLL
