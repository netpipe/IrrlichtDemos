// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __S_MESH_H_INCLUDED__
#define __S_MESH_H_INCLUDED__

#include "IMeshBuffer.h"
#include "IMesh.h"

namespace irr
{
namespace scene
{
	//! Simple implementation of the IMesh interface.
	struct SMesh : public IMesh
	{
		//! constructor
		SMesh()
		{
			#ifdef _DEBUG
			setDebugName("SMesh");
			#endif
		}

		//! destructor
		~SMesh()
		{
			// drop buffers
			for (u32 i=0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->drop();
		};

		//! returns amount of mesh buffers.
		virtual s32 getMeshBufferCount()
		{
			return MeshBuffers.size();
		}

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(s32 nr)
		{
			return MeshBuffers[nr];
		}

		//! returns an axis aligned bounding box
		virtual const core::aabbox3d<f32>& getBoundingBox() const
		{
			return BoundingBox;
		}

		//! Returns an axis aligned bounding box of the mesh.
		//! \return A bounding box of this mesh is returned.
		virtual core::aabbox3d<f32>& getBoundingBox()
		{
			return BoundingBox;
		}

		//! recalculates the bounding box
		void recalculateBoundingBox()
		{
			if (MeshBuffers.size())
			{
				BoundingBox = MeshBuffers[0]->getBoundingBox();
				for (u32 i=1; i<MeshBuffers.size(); ++i)
					BoundingBox.addInternalBox(MeshBuffers[i]->getBoundingBox());
			}
			else
				BoundingBox.reset(0.0f, 0.0f, 0.0f);
		}

		//! adds a MeshBuffer
		void addMeshBuffer(IMeshBuffer* buf)
		{
			if (buf)
			{
				buf->grab();
				MeshBuffers.push_back(buf);
			}
		}

		//! sets a flag of all contained materials to a new value
		virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
		{
			for (u32 i=0; i<MeshBuffers.size(); ++i)
				MeshBuffers[i]->getMaterial().Flags[flag] = newvalue;
		}

		core::array<IMeshBuffer*> MeshBuffers;
		core::aabbox3d<f32> BoundingBox;
	};


} // end namespace scene
} // end namespace irr

#endif

