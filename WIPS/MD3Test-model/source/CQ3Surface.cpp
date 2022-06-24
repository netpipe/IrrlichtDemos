#include "CQ3Surface.h"

namespace irr
{
namespace scene
{

CQ3Surface::CQ3Surface(const c8 * name, bones_t * box, u32 * frame, u32 numIndices, u32 numVertices)
    : Name(name), Tmp(NULL), Frame(frame), Box(box), FrameCount(0), IsCloned(false)
{
    Name.make_lower();

    Indices.set_used(numIndices);
    _IRR_DEBUG_BREAK_IF(Indices.pointer() == NULL || Indices.size() != numIndices) // access violation

    FrameCount = Box->size();

    FrameList = new core::array<video::S3DVertex2TCoords>[FrameCount];
    _IRR_DEBUG_BREAK_IF(FrameList == NULL) // access violation

    core::array<video::S3DVertex2TCoords> *current = &FrameList[0];
    core::array<video::S3DVertex2TCoords> *last = &FrameList[FrameCount - 1];
    do {
        current->set_used(numVertices);
        _IRR_DEBUG_BREAK_IF(current->pointer() == NULL || current->size() != numVertices) // access violation
    } while (current++ != last);

    Clone.set_used(numVertices);
    _IRR_DEBUG_BREAK_IF(Clone.pointer() == NULL || Clone.size() != numVertices) // access violation
}


CQ3Surface::~CQ3Surface()
{
    if (FrameList)
    {
        core::array<video::S3DVertex2TCoords> *current = &FrameList[0];
        core::array<video::S3DVertex2TCoords> *last = &FrameList[FrameCount - 1];

        do {
            current->clear();
        } while (current++ != last);

        delete[] FrameList;

        FrameList = NULL;
        FrameCount = 0;
    }
}

void CQ3Surface::operator=(const CQ3Surface& other)
{
    Name = other.Name;
    Material = other.Material;
    Frame = other.Frame;
    Box = other.Box;
    Tmp = NULL;
    IsCloned = false;

    if (other.Indices.size() > 0)
    {
        Indices.set_used(other.Indices.size());
        memcpy( Indices.pointer(), other.Indices.const_pointer(), sizeof(u16) * other.Indices.size() );
    }

    if (FrameList)
    {
        delete []FrameList;
        FrameList = NULL;
        FrameCount = 0;
    }

    if (other.FrameCount == 0)
        return;

    FrameList = new core::array<video::S3DVertex2TCoords>[other.FrameCount];
    _IRR_DEBUG_BREAK_IF(FrameList == NULL) // access violation
    if (!FrameList)
        return;

    FrameCount = other.FrameCount;

    core::array<video::S3DVertex2TCoords> *current = &FrameList[0];
    core::array<video::S3DVertex2TCoords> *last    = &FrameList[FrameCount - 1];
    core::array<video::S3DVertex2TCoords> *pother  = &other.FrameList[0];
    size_t tomove = sizeof(video::S3DVertex2TCoords) * pother->size();
    do {
        current->set_used( pother->size() );
        memcpy(current->pointer(), pother->pointer(), tomove);
        ++pother;
    } while (current++ != last);

    Clone.set_used(other.FrameList[0].size());
    _IRR_DEBUG_BREAK_IF(Clone.pointer() == NULL || Clone.size() != other.FrameList[0].size()) // access violation
}

} // end namespace scene
} // end namespace irr
