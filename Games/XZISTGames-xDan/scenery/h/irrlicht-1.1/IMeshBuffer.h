// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_MESH_BUFFER_H_INCLUDED__
#define __I_MESH_BUFFER_H_INCLUDED__

#include "IUnknown.h"
#include "SMaterial.h"
#include "irrArray.h"
#include "aabbox3d.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{
	//! Struct for holding a mesh with a single material
	/** SMeshBuffer is a simple implementation of a MeshBuffer. */
	class IMeshBuffer : public virtual IUnknown
	{
	public:

		//! destructor
		virtual ~IMeshBuffer() {}; 

        //! returns the material of this meshbuffer
        virtual video::SMaterial& getMaterial() = 0;

		//! returns the material of this meshbuffer
        virtual const video::SMaterial& getMaterial() const = 0;

		//! returns which type of vertex data is stored.
		virtual video::E_VERTEX_TYPE getVertexType() const = 0;

		//! returns pointer to vertex data. The data is a array of vertices. Which vertex
		//! type is used can be determinated with getVertexType().
		virtual const void* getVertices() const = 0; 

		//! returns pointer to vertex data. The data is a array of vertices. Which vertex
		//! type is used can be determinated with getVertexType().
		virtual void* getVertices() = 0; 

		//! returns amount of vertices
		virtual s32 getVertexCount() const = 0;

		//! returns pointer to Indices
		virtual const u16* getIndices() const = 0;

		//! returns pointer to Indices
		virtual u16* getIndices() = 0;

		//! returns amount of indices
		virtual s32 getIndexCount() const = 0;

		//! returns an axis aligned bounding box
		virtual const core::aabbox3d<f32>& getBoundingBox() const = 0;

		//! returns an axis aligned bounding box
		virtual core::aabbox3d<f32>& getBoundingBox() = 0;
	};

} // end namespace scene
} // end namespace irr

#endif

