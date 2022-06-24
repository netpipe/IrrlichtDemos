#ifndef __I_Q3MODELMESH_H_INCLUDED__
#define __I_Q3MODELMESH_H_INCLUDED__

#include <IAnimatedMesh.h>
#include <IMeshBuffer.h>
#include <S3DVertex.h>
#include <irrString.h>

namespace irr
{
namespace scene
{
    enum EMD3_MODEL_TYPE
    {
        MD3_UNKNOWN = 0,
        MD3_HEAD = 1,
        MD3_UPPER,
        MD3_LOWER,
        MD3_WEAPON
    };

    enum EMD3_TAG_TYPE
    {
        TAG_UNKNOWN = 0,
        TAG_HEAD = 1,
        TAG_TORSO,
        TAG_WEAPON
    };

    class IQ3Surface : public IMeshBuffer
    {
    public:
        IQ3Surface() {}

       ~IQ3Surface() {}

        //! returns name of surface
		virtual const core::stringc& getName() const = 0;

		//! returns IMeshBuffer for a frame.
		virtual IMeshBuffer* getMeshBuffer(u32 frame) const = 0;

		//! returns amount of frames
		virtual u32 getFrameCount() const = 0;

		//! returns pointer to vertices, for a frame.
        virtual const void* getVertices(u32 frame) const = 0;

        //! returns pointer to vertices, for a frame.
        virtual void* getVertices(u32 frame) = 0;

        //! returns amount of vertices
		virtual u32 getVertexCount(u32 frame) const = 0;

		//! returns an axis aligned bounding box, for a frame.
		virtual const core::aabbox3d<f32>& getBoundingBox(u32 frame) const = 0;

		//! set user axis aligned bounding box, for a frame.
		virtual void setBoundingBox( u32 frame, const core::aabbox3df& box) = 0;

		//! Create a surface copy for a specific frame.
		virtual IQ3Surface* clone() = 0;

		//! Restore asurface original for a specific frame.
		virtual IQ3Surface* original() = 0;

    };

    class IQ3ModelMesh : public IAnimatedMesh
    {
    public:
        IQ3ModelMesh(): Type(MD3_UNKNOWN) {}

        virtual ~IQ3ModelMesh() { }

        virtual EMD3_MODEL_TYPE getModelType() const { return Type; }

        virtual s32 getTagIndex(EMD3_TAG_TYPE type) const {
            switch (type) {
                case TAG_HEAD: return getTagIndex("tag_head");
                case TAG_TORSO: return getTagIndex("tag_torso");
                case TAG_WEAPON: return getTagIndex("tag_weapon");
                default: return -1;
            }
        }

        virtual s32 getTagIndex(const c8* name) const = 0;

        virtual u32 getTagCount() const = 0;

        virtual core::matrix4* getMatrixOfJoint (u32 tagIndex, s32 frame = -1) = 0;

        virtual IQ3Surface* getSurface(const c8 * surfaceName) = 0;

        virtual IQ3Surface* getSurface(u32 nr) = 0;

        virtual u32 getSurfaceCount() const = 0;

    protected:
        virtual void setModelType(EMD3_MODEL_TYPE type) { Type = type; }

    private:
        EMD3_MODEL_TYPE Type;
    };

} // end namespace scene
} // end namespace irr

#endif // __I_Q3MODELMESH_H_INCLUDED__

