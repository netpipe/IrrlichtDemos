#ifndef __C_SPHERICAL_TERRAIN_H_INCLUDED__
#define __C_SPHERICAL_TERRAIN_H_INCLUDED__

#include "IMeshSceneNode.h"
#include "IMesh.h"
#include "IReadFile.h"
#include "IFileSystem.h"
#include "IImage.h"
#include "SMeshBuffer.h"

namespace irr
{
namespace scene
{
	class CSphericalTerrain : public scene::ISceneNode
	{
	public:

		//! constructor
		CSphericalTerrain(const io::path& heightMapFileName, ISceneManager* mgr, f32 size = 5.0f, f32 maxHeight = 2.0f, ISceneNode* parent = 0, s32 id = -1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		CSphericalTerrain(io::IReadFile* heightMapFile, ISceneManager* mgr, f32 size = 5.0f, f32 maxHeight = 2.0f, ISceneNode* parent = 0, s32 id = -1,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CSphericalTerrain();

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

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

		//! This is basically a height multiplier.
		virtual void setMaxHeight(f32 maxHeight);
		
		virtual bool removeChild(ISceneNode* child);
		
		f32 getHeight(u32 x, u32 y) {
			const core::dimension2d<u32> hMapSize = heightMap->getDimension();
			if (x > hMapSize.Width-1 || y > hMapSize.Height-1) {
				return 0.0f;
			}
			
			return heightMap->getPixel(x, y).getAverage() * MaxHeight;
		}

		//! This can be used to place stuff on the terrain
		core::vector3df getSurfacePosition(u32 x, u32 y, f32 height = 1.0f) {
			f32 terrain_height = getHeight(x, y);
			f32 alpha = (f32)heightMap->getPixel(x, y).getAlpha()/255.0f;
			
			f64 AngleX = 2 * core::PI / PolyCountX;
			f64 AngleY = core::PI / PolyCountY;
			f64 ay = (y+1)*AngleY;
			f64 sinay = sin(ay);
			f64 axz = x*AngleX;
			
            core::vector3df pos(static_cast<f32>(Radius * cos(axz) * sinay),
						static_cast<f32>(Radius * cos(ay)),
						static_cast<f32>(Radius * sin(axz) * sinay));
			pos.setLength((Radius + terrain_height) * alpha);
			
			pos *= height;
			pos += getPosition();
			pos *= getScale();
			
			return pos;
		}
		
		//! This can be used to align stuff on the terrain
		core::vector3df getSurfaceNormal(u32 x, u32 y) {
			u32 index = x + y * (PolyCountX+2);
			return Mesh->getMeshBuffer(0)->getNormal(index);
		}

	private:
        IMesh* createSphericalTerrainMesh(video::IImage* heightMap, f32 radius, f32 MaxHeight);

		IMesh* Mesh;
		io::path HeightmapFile;
		video::IImage* heightMap;
		core::aabbox3d<f32> Box;
		f32 Radius;
		f32 MaxHeight;
		u32 PolyCountX;
		u32 PolyCountY;
	};

} // end namespace scene
} // end namespace irr

#endif

