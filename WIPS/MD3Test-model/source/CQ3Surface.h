#ifndef __C_Q3SURFACE_H_INCLUDED__
#define __C_Q3SURFACE_H_INCLUDED__

#include <IVideoDriver.h>
#include <S3DVertex.h>
#include <irrArray.h>
#include <irrString.h>
#include "IQ3ModelMesh.h"

namespace irr
{
namespace scene
{
    typedef core::aabbox3d<f32> bone_t;

    typedef core::array<bone_t> bones_t;

    class CQ3Surface : public IQ3Surface
    {
    public:
        CQ3Surface() : FrameList(NULL), Tmp(NULL), Frame(NULL), Box(NULL), FrameCount(0), IsCloned(false) { }

        CQ3Surface(const c8 *name, bones_t *box, u32 *frame, u32 numIndices, u32 numVertices);

        CQ3Surface(const CQ3Surface &other) { *this = other; }

        CQ3Surface(const IQ3Surface &other) { *this = (CQ3Surface)other; }

        //! destructor
        ~CQ3Surface();

        //!Assignment operator.
        virtual void operator=(const CQ3Surface& other);

        virtual void operator=(const IQ3Surface& other) { *this = (CQ3Surface) other; }

        //! returns name of surface
		virtual const core::stringc& getName() const { return Name; }

		//! returns IMeshBuffer for a frame.
		virtual IMeshBuffer* getMeshBuffer(u32 frame) const
		{
		    *Frame = frame;
		    return (IMeshBuffer*)this;
		}

        //! returns amount of frames
		virtual u32 getFrameCount() const { return FrameCount; }

		//! returns the material of this meshbuffer
		virtual const video::SMaterial& getMaterial() const { return Material; }

		//! returns the material of this meshbuffer
		virtual video::SMaterial& getMaterial() { return Material; }

        //! Create a surface copy for a specific frame.
		virtual IQ3Surface* clone()
		{
		    memcpy(Clone.pointer(), FrameList[*Frame].pointer(), sizeof(video::S3DVertex2TCoords) * FrameList[*Frame].size());
            Tmp = &Clone;
		    return this;
        }

		//! Restore asurface original for a specific frame.
		virtual IQ3Surface* original()
        {
		    Tmp = &FrameList[*Frame];
		    return this;
        }

		//! returns pointer to vertices
		virtual const void* getVertices() const { return Tmp->const_pointer(); }

		virtual void* getVertices() { return Tmp->pointer(); }

        //! returns pointer to vertices for a frame.
		virtual const void* getVertices(u32 frame) const { return FrameList[frame].const_pointer(); }

		virtual void* getVertices(u32 frame) { return FrameList[frame].pointer(); }

		//! returns amount of vertices
		virtual u32 getVertexCount() const { return Tmp->size(); }

		virtual u32 getVertexCount(u32 frame) const { return FrameList[frame].size(); }

		//! returns pointer to Indices
		virtual const u16* getIndices() const { return Indices.const_pointer(); }

		//! returns pointer to Indices
		virtual u16* getIndices() { return Indices.pointer(); }

		//! returns amount of indices
		virtual u32 getIndexCount() const { return Indices.size(); }

		//! returns an axis aligned bounding box
		virtual const core::aabbox3d<f32>& getBoundingBox() const { return (*Box)[*Frame]; }

		virtual const core::aabbox3d<f32>& getBoundingBox(u32 frame) const { return (*Box)[frame]; }

		//! set user axis aligned bounding box
		virtual void setBoundingBox( const core::aabbox3df& box) { (*Box)[*Frame] = box;  }

		virtual void setBoundingBox( u32 frame, const core::aabbox3df& box) { (*Box)[frame] = box;  }

		//! returns which type of vertex data is stored.
		virtual video::E_VERTEX_TYPE getVertexType() const { return video::S3DVertex2TCoords().getType(); }

		//! recalculates the bounding box. should be called if the mesh changed.
		virtual void recalculateBoundingBox() { }

		//! append the vertices and indices to the current buffer
		virtual void append(const void* const vertices, u32 numVertices, const u16* const indices, u32 numIndices) {}

		//! append the meshbuffer to the current buffer
		virtual void append(const IMeshBuffer* const other) {}

    private:
        core::stringc Name;
        core::array<u16> Indices;
        core::array<video::S3DVertex2TCoords> *FrameList, *Tmp, Clone;
        video::SMaterial Material;
        u32 * Frame;
        bones_t * Box;
        u32 FrameCount;
        bool IsCloned;
    };
} // end namespace scene
} // end namespace irr

#endif // __C_SURFACEDATA_H_INCLUDED__

