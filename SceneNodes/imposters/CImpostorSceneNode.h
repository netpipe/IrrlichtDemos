#include "ISceneNode.h"
#include "ITexture.h"
#include "CMeshBuffer.h"
#include "IAttributes.h"

namespace irr
{

class ITimer;

namespace gui
{
	class IGUIFont;
}
namespace scene
{

class CImpostorSceneNode : public ISceneNode
{
public:
	CImpostorSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id=-1, ITimer* timer=0);
	~CImpostorSceneNode();


	/*
	 * user functions
	 */

	//! Fills an IAttributes class with statistics information
	virtual void getStats( io::IAttributes* statistics);

	//! Cleans up memory
	//virtual void doGarbageCollection() { } // todo
	
	//! 
	//virtual void setLocked(bool locked) {} // todo

	//! Sets a new font for drawing debug text EDS_MESH_WIRE_OVERLAY debug info
	//! \param newFont: Font to draw. If 0 then text will not be drawn
	virtual void setDebugFont(gui::IGUIFont* newFont);


	/*
	 * node functions
	 */

	// render the actual object
	virtual void render();

	// get the bounding box
	const irr::core::aabbox3df& getBoundingBox() const;

	// redirect renders here, 
	virtual void OnRegisterSceneNode();
	// render to texture here,
	virtual void OnAnimate(u32 timeMs);

	// deal with children
	virtual void addChild(ISceneNode* child);
	virtual bool removeChild(ISceneNode* child);
	
	void doRenderQueue();

//private:

	//! returns next power of two within available range
	s32 getTextureSizeFromSurfaceSize(s32 size) const;
/*
	//! gets texture coordinates for a node
	void getTextureCoords(int bufid, int slotid)
	{
		Buffers[bufid].Slots
	}
*/


	struct SNodeLink
	{
		SNodeLink() 
		: Node(0), IsActive(false), Symmetrical(false), Animated(false),
		  BufferID(-1), SlotID(-1), RotVec(100.0f,100.0f,100.0f), IsQueued(false), Time(0) {}

		// the scene node
		ISceneNode*	       Node;

		// about the impostor
		bool               IsActive;
		s32                BufferID;
		s32                SlotID;
		
		core::rect<s32>    ScreenPos; // screen rectangle at time of render
		core::rect<s32>    NewPos; // screen rectangle right now
		bool               IsQueued; // is this in the render queue?

		core::vector3df    RotVec; // view vector it was rendered from
		core::vector3df    NewVec; // current view vector

		core::vector3df    BilPos1; // billboard position and normal vectors
		core::vector3df    BilPos2;
		core::vector3df    BilPos3;
		core::vector3df    BilPos4;
		core::vector3df    BilNorm;
		video::SColor      Color;

		u32                Time; //time it was rendered
		f32                Score; // Score value, between 0 and 1

		bool               Symmetrical; // todo: angle changes don't cause updates
		bool               Animated;  // todo:
		//u32                UpdateMS; // todo: number of milisecs before update (if animated)

		bool operator == (const SNodeLink& other) const { return Node == other.Node; }
	};

	struct SBufferLink
	{
		video::ITexture* Texture;
		SMeshBuffer* MeshBuffer;
		core::array<ISceneNode*> Slots;
		u32 FreeSlots;
		s32 SlotSize;
	};

	struct SNodeSorter
	{
		u32 NodeID;
		f32 Distance;
		core::vector3df Position;
		// farthest is first
		bool operator < (const SNodeSorter& other) const { return other.Distance < Distance; }
	};

	struct SBufferSorter
	{
		u32 BufferID;
		u32 SlotSize;
		u32 ItemCount;
		bool operator < (const SBufferSorter& other) const 
		{ 
			// sort first by slot size
			if (SlotSize < other.SlotSize)
				return true;
			else // then by number of items
			if (SlotSize == other.SlotSize && ItemCount < other.ItemCount)
				return true;

			return false;
		}
	};

	// for queued renders
	struct SRenderQueueItem
	{
		//u32 NodeID;
		SNodeLink* Node;

		bool operator < (const SRenderQueueItem& other) const
		{
			// calculate areas
			//f32 score1 = f32(Node->NewPos.getArea());
			//f32 score2 = f32(other.Node->NewPos.getArea());

			//core::max_<u32>(Node->Time, other.Node->Time) - core::min_<u32>(Node->Time, other.Node->Time);
			//Node->UpdateMS

			return Node->Score < other.Node->Score;
		}
		bool operator == (const SRenderQueueItem& other) const
		{
			return Node == other.Node;
		}
	};

	void renderNode(SNodeLink& Imp);
	void setTarget(SNodeLink& Imp);
	void releaseSlot(SNodeLink& Imp);
	void getSlot(SNodeLink& Imp, s32 size);
	void reallocateBuffers();
	void flattenBox(const core::aabbox3df& box);
	void flushRenderQueue() { } // todo
	void updatePosAndVector(SNodeLink& Imp);

	ISceneManager *LocalManager;
	ITimer* Timer;
	core::aabbox3df Box;
	f32 YDirection;

	core::array<SNodeLink> Impostors;
	core::array<ISceneNode*> IgnoredNodes;
	core::array<SBufferLink> Buffers;
	core::array<SRenderQueueItem> RenderQueue;

	video::ITexture* WorkTexture;
	video::SMaterial Material;
	gui::IGUIFont* DebugFont;


	// some arrays that we don't want to recreate each frame

	// sorts the nodes by distance
	core::array<SNodeSorter> NodeSorter;
	// sorts textures by size and count
	core::array<SBufferSorter> BufferSorter;
	// just counts positions in a buffer
	core::array<u32> Positions;

	// statistics
	s32 RenderCount, CacheHits, CacheMisses;

};

} // scene
} // irr
