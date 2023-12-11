#ifndef _NAVI_NODE_H_
#define _NAVI_NODE_H_

#include <vector>
#include <irrlicht.h>
#include "stlastar.h"

using namespace irr;


#ifndef NAVI_NODE2_PROPERTIES
#define NAVI_NODE2_PROPERTIES
#endif

class NaviNode
{

	public:
	
	NaviNode
	(
		u32 id,
		const core::vector3df& pos
	);
	
	NaviNode();
	
	
	virtual ~NaviNode();
	
	bool isValid();
	
	
	static
	u32 doAstarSearch
	(
		const core::array<NaviNode*>& nodes,
		u32 startId,
		u32 endId,
		core::array<core::stringw>& solutions
	);
	
	static
	bool findIndexFromNodeId
	(
		const core::array<NaviNode*>& nodes,
		u32 id,
		u32& index
	);
	
	static
	void printSolutions(const core::array<core::stringw>& solutions);
	
	static
	bool pathIsClear
	(
		IrrlichtDevice* device,
		scene::ITriangleSelector* selector,
		const core::vector3df& p1,
		const core::vector3df& p2
	);

	static
	core::array<u32> stringwToU32s(const core::stringw& text);
	
	static
	core::array<u32> getNeighbors(const core::stringw& neighbors);
		
	static
	void test3();
	
	static
	void test2();
	
	static
	void test();
	
	
	static
	bool setupNeighbors
	(
		IrrlichtDevice* device,
		scene::ITriangleSelector* selector,
		core::array<NaviNode*>& nodes,
		const std::vector<core::vector3df>& points,
		f32 maxDistSQ
	);
	
	static
	bool setupNeighbors
	(
		core::array<NaviNode*>& nodes,
		const std::vector<core::vector3df>& points,
		f32 maxDistSQ
	);
	
	static
	void setNeighborIdsFromNeighbors(NaviNode* nodes);
	

	
	static
	u32 getMinNeighborCount(const std::vector<NaviNode*>& nodes);
	
	static
	bool containsNodeWithNoNeighbor(const std::vector<NaviNode*>& nodes);
	
	static
	bool allNodesHaveNeighbors(const std::vector<NaviNode*>& nodes);
	
	
	bool IsGoal(const NaviNode& goalNode);
	bool IsSameState(const NaviNode& rhs);
	bool GetSuccessors(AStarSearch<NaviNode>* astarsearch, NaviNode* parentNode);
	f32 GetCost(const NaviNode& successor);

	f32 GoalDistanceEstimate(const NaviNode& goalNode);
	
	void setNeighbors(const core::array<NaviNode*>& nodes);
	
	static
	bool u32FromWchars
	(
		const wchar_t* cs,
		u32& n
	);
	
	static
	core::array<u32> u32sFromWchars(const wchar_t* cs);
	
	
	void clearNeighbors() { Neighbors.clear(); NeighborIds.clear(); }
	
	void print();
	
	// setters + getters:
	void setId(u32 id) { Id = id; }
	void setPos(const core::vector3df& pos) { Pos = pos; }
	void setReserved(bool reserved){ Reserved = reserved; }
	//void addNeighbor(NaviNode* n) { Neighbors.push_back(n); }
	
	NaviNode* getNeighbor(u32 id);
	void addNeighbor(NaviNode* n);
	void removeNeighbor(u32 id);

	bool containsNeighborId(u32 id)
	{
		return (NeighborIds.binary_search(id) > -1);
		/***
		for (u32 n=0; n<NeighborIds.size(); ++n)
		{
			if (id == NeighborIds[n])
				return true;
		}
		
		return false;
		***/
	}
	
	void addNeighborId(u32 id)
	{
		printf("containsNeighborId(%u)=%i: ", id, containsNeighborId(id));
		if (!containsNeighborId(id))
		{
			NeighborIds.push_back(id);
			printf("neighbor(%u) ADDED\n", id);
		}
		else
		{
			printf("neighbor(%u) NOT added!\n", id);
		}
	}
	
	//workaround:
	void removeNeighborIdsDeplicates()
	{
		core::array<u32> ns;
		
		for (u32 n=0; n<NeighborIds.size(); ++n)
		{
			u32 id = NeighborIds[n];
			if (ns.binary_search(id) == -1)
				ns.push_back(id);
		}
		
		NeighborIds.clear();
		NeighborIds = ns;
		
	}
	void setNeighborIds(const core::array<u32>& neighborIds)
	{
		NeighborIds.clear();
		NeighborIds = neighborIds;
	}
	
	u32 getId() const { return Id; }
	core::vector3df getPos() const { return Pos; }
	bool isReserved() const { return Reserved; }
	u32 getNeighborCount() const { return Neighbors.size(); }
	core::array<NaviNode*> getNeighbors() const { return Neighbors; }
	bool hasNoNeighbor() const { return Neighbors.empty(); }
	
	core::array<u32> getNeighborIds();
	
	
	protected:
	
	private:
	
	u32 Id;
	core::vector3df Pos;
	bool Reserved;
	//u32 NeighborCount;
	core::array<NaviNode*> Neighbors;
	core::array<u32> NeighborIds;
	
	
	
	void setNeighborsFromIds
	(
		const core::array<NaviNode*>& nodes,
		const core::array<u32>& ids
	);

};







#endif // _NAVI_NODE_H_



