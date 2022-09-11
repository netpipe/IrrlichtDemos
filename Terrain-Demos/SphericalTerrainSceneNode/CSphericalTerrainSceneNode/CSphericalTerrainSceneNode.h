#ifndef __C_SPHERICAL_TERRAIN_SCENE_NODE_H_INCLUDED__
#define __C_SPHERICAL_TERRAIN_SCENE_NODE_H_INCLUDED__

#include "IMeshSceneNode.h"
#include "IMesh.h"
#include "IReadFile.h"
#include "IFileSystem.h"
#include "IImage.h"

namespace irr
{
namespace scene
{
	class CSphericalTerrainSceneNode : public IMeshSceneNode
	{
	public:

		//! constructor
		CSphericalTerrainSceneNode(const io::path& heightMapFileName, ISceneManager* mgr, f32 size = 5.0f, f32 maxHeight = 2.0f, ISceneNode* parent = 0, s32 id = -1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		CSphericalTerrainSceneNode(io::IReadFile* heightMapFile, ISceneManager* mgr, f32 size = 5.0f, f32 maxHeight = 2.0f, ISceneNode* parent = 0, s32 id = -1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CSphericalTerrainSceneNode();

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh=0,
		s32 id=-1, bool zfailmethod=true, f32 infinity=1000.0f) {};

		//! returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		virtual video::SMaterial& getMaterial(u32 i);

		//! returns amount of materials used by this scene node.
		virtual u32 getMaterialCount() const;

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_SPHERE; }

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0);

		//! The mesh cannot be changed
		virtual void setMesh(IMesh* mesh) {}

		//! Returns the current mesh
		virtual IMesh* getMesh() { return Mesh; }

		//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
		/* In this way it is possible to change the materials a mesh causing all mesh scene nodes
		referencing this mesh to change too. */
		virtual void setReadOnlyMaterials(bool readonly) {}

		//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
		virtual bool isReadOnlyMaterials() const { return false; }

		virtual void setMaxHeight(f32 maxHeight);

	private:
        IMesh* createSphericalTerrainMesh(video::IImage* heightMap, f32 radius);

		IMesh* Mesh;
		io::path HeightmapFile;
		core::aabbox3d<f32> Box;
		f32 Radius;
		f32 MaxHeight;
		u32 PolyCountX;
		u32 PolyCountY;
	};

} // end namespace scene
} // end namespace irr

#endif

