#ifndef _NAVINODE_CONTROL_H_
#define _NAVINODE_CONTROL_H_

/***
Acknowledgement
===============
Some codes are based on PathCreator by kazymjir:
http://irrlicht.sourceforge.net/forum/viewtopic.php?t=38645

CityDriving.irr scene file is ripped from CityDriving.zip of sio2:
http://irrlicht.sourceforge.net/forum/viewtopic.php?t=45781


astar algoithm is written by Justin Heyes-Jones:
http://code.google.com/p/a-star-algorithm-implementation/
Most updated version of stlastar.h and fsa.h can be obtained from the above.

Thanks.

***/

#include <irrlicht.h>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>


#include "navinode.h"
#include "segment.h"
#include "navinode_view.h"

using namespace irr;

#ifndef NAVINODE_CONTROL_PROPERTIES
#define NAVINODE_CONTROL_PROPERTIES
#define MAX_DIST_SQ 200000.0f
#endif



class NaviNodeControl
{
	public:
	NaviNodeControl(IrrlichtDevice* device);
	virtual ~NaviNodeControl();
	
	void draw();
	
	//CAUSTION: slope angle not taken into account!!
	void setWaypoints(const std::vector<core::vector3df>& waypoints, bool testLineOfSight = true);
	
	static
	core::array<NaviNode*> loadNodes(IrrlichtDevice* device, const core::stringc& filename);
	
	

	static
	void addYOffset(core::array<NaviNode*>& nodes, f32 yOffset);
	
	static
	void save(IrrlichtDevice* device, const core::array<NaviNode*>& nodes, const core::stringc& filename);
	
	static
	core::array<u32> getUnusedIds(const core::array<NaviNode*>& nodes);
	
	static
	u32 getMaxId(const core::array<NaviNode*>& nodes);
	
	static
	bool u32FromWchars(const wchar_t* cs, u32& n);
	
	static
	core::array<u32> u32sFromWchars(const wchar_t* cs);

	static
	core::stringw stringFromF32(f32 val);
	
	static
	core::stringw stringFromU32(u32 val);

	static
	core::stringw stringFromU32s(const core::array<u32>& nums);
	
	static
	s32 getNumberOfMiddleNodes(NaviNode* n0, NaviNode* n1, f32 maxDistSQ);
	
	static
	void resetAnimatorCollisionResponse(scene::ISceneNode* node); // for teleporting the camera with AnimatorCollisionResponse
	
	
	
	
	bool findSelectedNode(u32& id);

	NaviNode* findNode
	(
		const core::vector3df& cursor,
		bool& found,
		f32 radius = 10.0f
	);
	
	void save(const core::stringc& filename);
	
	bool nodesAreTooClose
	(
		NaviNode* n1,
		NaviNode* n2,
		f32 minDistance = 25.0f
	);
	
	u32 getNodeCount() const { return Nodes.size(); }
	core::array<NaviNode*> getNodes() const { return Nodes; }
	NaviNode* getNode(u32 id) const;

	void addNode
	(
		IrrlichtDevice* device,
		bool testLineOfSight = true,
		f32 maxSlopeDegs = 45.0f
	);

	void addNode(const core::vector3df& pos, bool testLineOfSight = true);
	void removeNode(u32 id);
	void removeSegment(u32 id1, u32 id2);
	
	void setFilename(const core::stringc& filename);
	
	void setStartNode(s32 id) { StartId = id; }
	void setEndNode(s32 id) { EndId = id; }
	void setNodeReserved(s32 id, bool reserved);
	void unsetAllNodesReserved();
	
	void setMaxDistSQ(f32 maxDistSQ);
	f32 getMaxDistSQ() const { return MaxDistSQ; }
	NaviNodeView* getView() const { return View; }
	
	scene::ITriangleSelector* Selector;
	
private:
	
	IrrlichtDevice* Device;
	
	core::array<NaviNode*> Nodes;
	core::array<Segment> Segments;
	
	NaviNodeView* View;
	
	core::stringc Filename;
	s32 StartId;
	s32 EndId;
	u32 CurrentId;
	f32 MaxDistSQ;
	f32 MaxDist;
	
	void sleep(float ms);
	void setupSegments(const core::array<NaviNode*>& nodes);
	bool segmentIsFound(const core::array<Segment>& ss, const Segment& s);
};

#endif // _NAVINODE_CONTROL_H_
