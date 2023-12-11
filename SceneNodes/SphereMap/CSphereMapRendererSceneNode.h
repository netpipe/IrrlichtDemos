
#include "ISceneNode.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{

	// some debug data constants
	const E_DEBUG_SCENE_TYPE EDS_SPHEREMAP_BILLBOARD = EDS_SKELETON;
	const E_DEBUG_SCENE_TYPE EDS_SPHEREMAP_MESH      = EDS_MESH_WIRE_OVERLAY;


	class IMeshBuffer;
	class ICameraSceneNode;

	enum ESPHERE_MAP_RENDER_TARGET
	{
		ESMRT_FRONT=0,
		ESMRT_BACK,
		ESMRT_LEFT,
		ESMRT_RIGHT,
		ESMRT_UP,
		ESMRT_DOWN,
		ESMRT_OUTPUT
	};

	class CSphereMapRendererSceneNode : public ISceneNode
	{

	public:

		//! Creates a node that renders to a sphere map
		//! \param parent: parent node
		//! \param mgr: scene manager
		//! \param id: node id
		//! \param gridSize: number of segments in the width/height of each face of the hexahedral sphere
		//! it's a good idea to make this number a power of two, float innacuracies cause seams otherwise
		//! \param createRenderTargets: should the node create its own render targets? If not you can
		//! set these manually with setTexture
		//! \param wtSize: Width and height of the 6 face work textures. If these are smaller than the 
		//! output then you get free blur thanks to texture filtering
		//! \param otSize: Size of the output texture. Make this nice and large for smooth edges
		CSphereMapRendererSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id=-1, 
			u32 gridSize = 128, bool createRenderTargets=true, u32 wtSize=512, u32 otSize=512 );

		virtual ~CSphereMapRendererSceneNode();

		// updates the output render target, optionally updating the temporary ones first.
		void update(bool UpdateAll=false, bool sphereMesh=true);

		//! sets the scale of the sphere mesh
		void setMagnify(f32 zoom);

		//! returns a render target
		video::ITexture* getTexture(ESPHERE_MAP_RENDER_TARGET which);

		// node functions:
		virtual const core::aabbox3d<f32>& getBoundingBox() const;
		virtual void render();
		virtual void OnRegisterSceneNode();

	private:
		
		scene::IMeshBuffer* createMeshBuffer(u32 size, bool inside=true);
		void renderOutput(const core::matrix4& trans, bool mirror=false);
		
		core::aabbox3d<f32> Box;
		video::SMaterial Material;
		scene::IMeshBuffer* InnerBuffer, *OuterBuffer, *MeshBuffer;
		ICameraSceneNode* Camera;
		ISceneNode *SkyBox;
		bool NeedsUpdate;
		video::S3DVertex billvertices[4];
		video::SColor BackColour;
		core::array<video::ITexture*> RenderTargets;

		f32 Magnify;

	};


}

}