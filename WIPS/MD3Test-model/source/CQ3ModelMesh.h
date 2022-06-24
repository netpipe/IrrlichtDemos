#ifndef __C_Q3MODELMESH_H_INCLUDED__
#define __C_Q3MODELMESH_H_INCLUDED__

#include <IReadFile.h>
#include <IVideoDriver.h>
#include <S3DVertex.h>
#include <irrArray.h>
#include <irrString.h>
#include "IQ3ModelMesh.h"
#include "CQ3Surface.h"

namespace irr
{
namespace scene
{


    struct tag_t
    {
         core::vector3df Origin;
         core::vector3df Rotation[3];
         core::matrix4 Transformation;
    };

    struct tags_t {
        core::stringc Name;
        core::array<tag_t> Data;
    };

    typedef core::array<CQ3Surface*> surfaces_t;

	class CQ3ModelMesh : public IQ3ModelMesh
	{
	public:

		//! constructor
		CQ3ModelMesh();

		//! destructor
		virtual ~CQ3ModelMesh();


     // For IAnimatedMesh.
		//! returns the amount of frames in milliseconds. If the amount is 1, it is a static (=non animated) mesh.
		virtual u32 getFrameCount() const {	return FrameCount; }

		//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
		virtual IMesh* getMesh(s32 frame, s32 detailLevel=255, s32 startFrameLoop=-1, s32 endFrameLoop=-1) {
            if (frame >= 0 && (u32)frame < FrameCount)
               Frame = frame;
            else if (frame < 0)
               Frame = 0;
            else if ((u32)frame >= FrameCount)
               Frame = FrameCount - 1;
            return this;
        }

		//! Returns the type of the animated mesh.
		virtual E_ANIMATED_MESH_TYPE getMeshType() const { return EAMT_UNKNOWN; }


    // For IMesh.
		//! returns an axis aligned bounding box
		virtual const core::aabbox3d<f32>& getBoundingBox() const { return BoxList[Frame]; }

		//! Returns pointer to a mesh buffer which fits a material.
		virtual IMeshBuffer* getMeshBuffer(const video::SMaterial& material) const {
            IMeshBuffer* s;
            s = NULL;
            u32 i;
            for (i=0; i < Surfaces.size() && Surfaces[i]->getMaterial() != material; i++);
                 if (Surfaces[i]->getMaterial() == material)
                     s = (IMeshBuffer*)Surfaces[i];
            return s;
		}

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(u32 nr) const {
		    return (IMeshBuffer*) ( nr<=Surfaces.size()-1 ? Surfaces[nr] : NULL );
		}

		//! returns amount of mesh buffers.
		virtual u32 getMeshBufferCount() const { return Surfaces.size(); }

		//! set user axis aligned bounding box
        virtual void setBoundingBox(const core::aabbox3d<f32> &box) { BoxList[Frame] = box; }

		//! sets a flag of all contained materials to a new value
		virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue) {
		    u32 i;
		    for (i = 0; i < Surfaces.size(); i++)
		        Surfaces[i]->getMaterial().setFlag(flag, newvalue);
        }


     // IQ3ModelMesh.
        virtual s32 getTagIndex(const c8* name) const
        {
            u32 i;
            for (i = 0; i < TagCount; i++) {
                if (Tags[i].Name == name)
                    return (s32)i;
            }
            return -1; // No tag by that name
        }

        virtual u32 getTagCount() const { return TagCount; }


        //! Return a pointer to a transformation matrix of a tag.
        //! Useful for attaching objects to an MD3 model
        virtual core::matrix4* getMatrixOfJoint (u32 tagNumber, s32 frame = -1)
        {
            if (tagNumber < TagCount) {
                if (frame >= 0 && (u32)frame < FrameCount)
                    return &Tags[tagNumber].Data[frame].Transformation;
                else
                    return &Tags[tagNumber].Data[Frame].Transformation;
            }
            return 0;
        }

        //virtual IMeshBuffer* getSurface(const c8 * surfaceName)
        virtual IQ3Surface* getSurface(const c8 * surfaceName)
        {
            u32 i;
            for (i=0; i < Surfaces.size(); i++)
               if (Surfaces[i]->getName() == surfaceName)
                   return Surfaces[i];
            return NULL;
        }

        virtual IQ3Surface* getSurface(u32 nr)
        {
            return ( (IQ3Surface*) (nr < Surfaces.size() ? Surfaces[nr] : NULL) );
        }

        virtual u32 getSurfaceCount() const { return Surfaces.allocated_size(); }

     // CQ3ModelMesh.
        virtual void clear();

        virtual void setMeshName(const c8 * meshName) { MeshName = meshName; }

        virtual const c8 * getMeshName() const { return MeshName.c_str(); }

        virtual const c8 * getSurfaceName(u32 s) const { return Surfaces[s]->getName().c_str(); }

		virtual void setFrameCount(u32 frameCount);

		virtual void addBone(const bone_t & bone) { BoxList.push_back(bone); }

		virtual void setTagCount(u32 tagCount);

		virtual void addTag(const c8 * name, const tag_t & tag);

        virtual void setSurfacesCount(u32 surfacesCount);

        virtual CQ3Surface* newSurface(const c8 * name, u32 numIndices, u32 numVertices);


	private:
        friend class CQ3ModelMeshFileLoader;

		core::stringc MeshName;

		u32 FrameCount;
        u32 Frame;

		bones_t BoxList;

		u32 TagCount, TagIndex;
		tags_t* Tags; // Tags x Frames

        surfaces_t Surfaces;
	};

} // end namespace scene
} // end namespace irr

#endif // __C_Q3MODELMESH_H_INCLUDED__

