#include "navinode.h"
#include <queue>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace irr;

NaviNode::NaviNode
(
	u32 id,
	const core::vector3df& pos
)
:	Id(id),
	Pos(pos),
	Reserved(false)
{
	
}


NaviNode::NaviNode()
:	Id(UINT_MAX),
	Pos(FLT_MAX),
	Reserved(false)
{
	
}


NaviNode::~NaviNode()
{
	
}


bool NaviNode::isValid()
{
	return (this->getId() != UINT_MAX);
}


u32 NaviNode::doAstarSearch
(
	const core::array<NaviNode*>& nodes,
	u32 startId,
	u32 endId,
	core::array<core::stringw>& solutions
)
{
	u32 start = 0;
	u32 end = 0;
	
	if ((!findIndexFromNodeId(nodes, startId, start)) || (!findIndexFromNodeId(nodes, endId, end)))
		return 0;
	
	//if ((start >= nodes.size()) || (end >= nodes.size()))
		//return 0;
	
	u32 solutionCount = 0;
	// Create an instance of the search class...
	AStarSearch<NaviNode> astarsearch;

	unsigned int SearchCount = 0;
	const unsigned int NumSearches = 1;
	
	srand(time(NULL));
	while (SearchCount < NumSearches)
	{
		// Create a start state
		NaviNode nodeStart = *(nodes[start]);
		
		// Define the goal state
		NaviNode nodeEnd = *(nodes[end]);
		
		// Set Start and goal states
		astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);

		unsigned int SearchState;
		unsigned int SearchSteps = 0;

		do
		{
			SearchState = astarsearch.SearchStep();

			SearchSteps++;

	#if DEBUG_LISTS

			cout << "Steps:" << SearchSteps << "\n";

			int len = 0;

			cout << "Open:\n";
			NaviNode *p = astarsearch.GetOpenListStart();
			while (p)
			{
				len++;
	#if !DEBUG_LIST_LENGTHS_ONLY			
				((NaviNode *)p)->print();
	#endif
				p = astarsearch.GetOpenListNext();
				
			}

			cout << "Open list has " << len << " nodes\n";

			len = 0;

			cout << "Closed:\n";
			p = astarsearch.GetClosedListStart();
			while (p)
			{
				len++;
	#if !DEBUG_LIST_LENGTHS_ONLY			
				p->print();
	#endif			
				p = astarsearch.GetClosedListNext();
			}

			cout << "Closed list has " << len << " nodes\n";
	#endif

		}
		while (SearchState == AStarSearch<NaviNode>::SEARCH_STATE_SEARCHING);

		if (SearchState == AStarSearch<NaviNode>::SEARCH_STATE_SUCCEEDED)
		{
			core::stringw text;
			cout << "Search found goal state\n";
			
			//<-----------------------------------------------------------------	
			NaviNode *node = astarsearch.GetSolutionStart();
			
	#if DISPLAY_SOLUTION
			cout << "Displaying solution\n";
	#endif
			int steps = 0;

			node->print();
			text += node->getId();
			text += L" ";
				
			for (;;)
			{
				node = astarsearch.GetSolutionNext();

				if (!node)
				{
					break;
				}

				node->print();
				text += node->getId();
				text += L" ";
					
				steps ++;
				
			};

			cout << "Solution steps " << steps << endl;

			// Once you're done with the solution you can free the nodes up
			astarsearch.FreeSolutionNodes();
			solutions.push_back(text);
			++solutionCount;
		}
		else if (SearchState == AStarSearch<NaviNode>::SEARCH_STATE_FAILED) 
		{
			cout << "Search terminated. Did not find goal state\n";
		
		}

		// Display the number of loops the search went through
		cout << "SearchSteps : " << SearchSteps << "\n";

		SearchCount ++;

		astarsearch.EnsureMemoryFreed();
	}
	
	return solutionCount;
}

bool NaviNode::findIndexFromNodeId
(
	const core::array<NaviNode*>& nodes,
	u32 id,
	u32& index
)
{
	for (u32 n=0; n<nodes.size(); ++n)
	{
		if (id == nodes[n]->getId())
		{
			index = n;
			return true;
		}
	}
	return false;
}

void NaviNode::printSolutions(const core::array<core::stringw>& solutions)
{
	for (u32 m=0; m<solutions.size(); ++m)
	{
		core::stringw text = solutions[m];
		core::array<u32> ns = stringwToU32s(text);
		
		printf("solution (%i):", m);
		for (u32 n=0; n<ns.size(); ++n)
		{
			printf("%i ", ns[n]);
		}
		printf("\n");
	
	}
}

bool NaviNode::pathIsClear
(
	IrrlichtDevice* device,
	scene::ITriangleSelector* selector,
	const core::vector3df& p1,
	const core::vector3df& p2
)
{
	if ((!device) || (!selector))
		return true;
	
	scene::ISceneCollisionManager* cmgr = device->getSceneManager()->getSceneCollisionManager();
	
	core::line3df ray;
	ray.start = p1;
	ray.end = p2;

	core::vector3df pos;
	core::triangle3df tri;
	const scene::ISceneNode* selectedNode = 0;
	selectedNode = cmgr->getSceneNodeAndCollisionPointFromRay(ray, pos, tri);
	
	return (!selectedNode);

}


core::array<u32> NaviNode::stringwToU32s(const core::stringw& text)
{
	core::array<u32> ns;
	
	std::wstringstream ss;
	ss << text.c_str();
	
	u32 n = 0;
	while (!ss.fail())
	{
		ss >> n;
		
		if (!ss.fail())
			ns.push_back(n);
	}
	
	return ns;
}


core::array<u32> NaviNode::getNeighbors(const core::stringw& neighbors)
{
	return stringwToU32s(neighbors);
}


void NaviNode::test3()
{
	core::stringw neighbors = L"  2  7  6   9 117  ";
	
	core::array<u32> ns = getNeighbors(neighbors);
	for (u32 n=0; n<ns.size(); ++n)
	{
		printf("ns[n]=%u\n", ns[n]);
	}
}


// nodes from xml file:
void NaviNode::test2()
{
	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	char* str = new char[100];
	wchar_t array[] = L" 2 8 -1 235";
	wcstombs(str, array, 100);
	std::cout << "str=" << str << std::endl;
	delete[] str;
	
	IrrlichtDevice* device = createDevice(video::EDT_NULL);
	if (!device)
		return;
		
	io::IXMLReader* xmlReader = device->getFileSystem()->createXMLReader("test.xml");
	if (!xmlReader)
		return;
	printf("xmlReader->getSourceFormat()=%u\n", xmlReader->getSourceFormat());
	
	const core::stringw nodeTag(L"NaviNode");
	const core::stringw idTag(L"Id");
	const core::stringw posTag(L"Pos");
	const core::stringw neighborsTag(L"Neighbors");
	
	core::stringw currentSection; //keep track of our currentsection

	////////////////////////////////////////////////////////////////////////////
	core::array<NaviNode*> nodes;
	NaviNode* node = 0;
	
	while (xmlReader->read())
	{
		switch (xmlReader->getNodeType())
		{
			case io::EXN_ELEMENT:
			{
				if (nodeTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					node = new NaviNode();
					nodes.push_back(node);
					
					currentSection = nodeTag;
				}
				else if (idTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					currentSection = idTag;
				}
				else if (posTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					f32 x, y, z;
					x = xmlReader->getAttributeValueAsFloat(L"X");
					y = xmlReader->getAttributeValueAsFloat(L"Y");
					z = xmlReader->getAttributeValueAsFloat(L"Z");
					printf("pos=(%f,%f,%f)\n", x, y, z);
					
					node->setPos(core::vector3df(x, y, z));
					
					currentSection = posTag;
				}
				else if (neighborsTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					currentSection = neighborsTag;
				}
				
				//	stringw key = xml->getAttributeValueSafe(L"");
				/***
				if
				(
					(currentSection.empty())
					&& (...Tag.equals_ignore_case(xmlReader->getNodeName()))
				)
				{
					currentSection = videoTag;
				}
				else if
				(
					(currentSection.equals_ignore_case(...))
					&& (...Tag.equals_ignore_case(xmlReader->getNodeName()))
				)
				{
				
				}
				***/
			}
			break;
			
			case io::EXN_TEXT:
			{
				const wchar_t* data = xmlReader->getNodeData();
				if (data)
				{
					if (currentSection == idTag)
					{
						u32 id;
						if (u32FromWchars(data, id))
						{
							node->setId(id);
							printf("id=%u\n", id);
						}
					}
					else if (currentSection == neighborsTag)
					{
						core::array<u32> ids = u32sFromWchars(data);
						for (u32 n=0; n<ids.size(); ++n)
						{
							//printf("neighbor=%u\n", ids[n]);
							node->addNeighborId(ids[n]);
						}
						
					}
				}
			}
			break;
			
			case io::EXN_ELEMENT_END:
				if (nodeTag.equals_ignore_case(xmlReader->getNodeName()))
				{
					currentSection = L"";
				}
			break;
			
			default:
			break;
		}
	}
	
	xmlReader->drop();
	
	
	// show results:
	for (u32 n=0; n<nodes.size(); ++n)
	{
		nodes[n]->setNeighbors(nodes);
		nodes[n]->print();
	}
	
	// cleanup nodes:
	for (u32 n=0; n<nodes.size(); ++n)
	{
		delete nodes[n];
		nodes[n] = 0;
	}
}


// nodes to xml file:
void NaviNode::test()
{
	IrrlichtDevice* device = createDevice(video::EDT_NULL);
	if (!device)
		return;
	
	io::IXMLWriter* xmlWriter = device->getFileSystem()->createXMLWriter("test.xml");
	if (!xmlWriter)
		return;
	
	xmlWriter->writeXMLHeader();
	
	////////////////////////////////////////////////////////////////////////////
	// node 77
	////////////////////////////////////////////////////////////////////////////
	xmlWriter->writeElement(L"NaviNode");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"Id");
	xmlWriter->writeText(L"77");
	xmlWriter->writeClosingTag(L"Id");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement
	(
		L"Pos", true,
		L"X", L"10.0",
		L"Y", L"23.0",
		L"Z", L"0.0"
	);
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"Neighbors");
	xmlWriter->writeText(L"78 ");
	xmlWriter->writeClosingTag(L"Neighbors");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeClosingTag(L"NaviNode");
	xmlWriter->writeLineBreak();
	
	////////////////////////////////////////////////////////////////////////////
	// node 78
	////////////////////////////////////////////////////////////////////////////
	xmlWriter->writeElement(L"NaviNode");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"Id");
	xmlWriter->writeText(L"78");
	xmlWriter->writeClosingTag(L"Id");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement
	(
		L"Pos", true,
		L"X", L"-190.0",
		L"Y", L"13.0",
		L"Z", L"240.0"
	);
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"Neighbors");
	xmlWriter->writeText(L"79  77   ");
	xmlWriter->writeClosingTag(L"Neighbors");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeClosingTag(L"NaviNode");
	xmlWriter->writeLineBreak();
	////////////////////////////////////////////////////////////////////////////
	// node 79
	////////////////////////////////////////////////////////////////////////////
	xmlWriter->writeElement(L"NaviNode");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"Id");
	xmlWriter->writeText(L"79");
	xmlWriter->writeClosingTag(L"Id");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement
	(
		L"Pos", true,
		L"X", L"190.0",
		L"Y", L"13.0",
		L"Z", L"40.0"
	);
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"Neighbors");
	xmlWriter->writeText(L" 78 ");
	xmlWriter->writeClosingTag(L"Neighbors");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeClosingTag(L"NaviNode");
	xmlWriter->writeLineBreak();

	////////////////////////////////////////////////////////////////////////////
	
	xmlWriter->drop();

}


bool NaviNode::setupNeighbors
(
	IrrlichtDevice* device,
	scene::ITriangleSelector* selector,
	core::array<NaviNode*>& nodes,
	const std::vector<core::vector3df>& points,
	f32 maxDistSQ
)
{
	/***
	if (!device)
	{
		printf("!device\n");
		return false;
	}
	
	if (!selector)
	{
		printf("!selector\n");
		return false;
	}
	***/
	
	// error checking:
	if (nodes.size() != points.size())
		return false;
	
	// remove old neighbors first:
	for (u32 n=0; n<nodes.size(); ++n)
	{
		//FIXME:
		//nodes[n]->getNeighbors().clear();
		nodes[n]->clearNeighbors();
	}
	// for sequential action on each node:
	std::deque<NaviNode*> ns;
	for (u32 n=0; n<points.size(); ++n)
		ns.push_back(nodes[n]);
////////////////////////////////////////////////////////////////////////////////
u32 id = UINT_MAX;
u32 neighborId = UINT_MAX;

u32 pointNum = 0;
u32 neighborPointNum = 1;

NaviNode* currentNode = 0;
std::deque<NaviNode*>::iterator iter;

while (!ns.empty())
{
	currentNode = ns.front();
	ns.pop_front();
	
	id = currentNode->getId();
	for (iter=ns.begin(); iter!=ns.end(); ++iter)
	{
		if (neighborPointNum >= points.size())
			break;
		
		NaviNode* n = *iter;
		neighborId = n->getId();
		
		core::vector3df p1 = points[pointNum];
		core::vector3df p2 = points[neighborPointNum];
		if
		(
			(p1.getDistanceFromSQ(p2) <= maxDistSQ)
			//FIXME:
			&& (pathIsClear(device, selector, p1, p2))
		)
		{
			currentNode->addNeighbor(n);
			n->addNeighbor(currentNode);
		}
		++neighborPointNum;
	}

	++pointNum;
	neighborPointNum = pointNum + 1;
	
}
////////////////////////////////////////////////////////////////////////////////
	for (u32 n=0; n<nodes.size(); ++n)
	{
		NaviNode* node = nodes[n];
		setNeighborIdsFromNeighbors(node); //<----------
	}
	
	return true;
}

//TODO: rewrite
bool NaviNode::setupNeighbors
(
	core::array<NaviNode*>& nodes,
	const std::vector<core::vector3df>& points,
	f32 maxDistSQ
)
{

	// error checking:
	if (nodes.size() != points.size())
		return false;
	
	// remove old neighbors first:
	for (u32 n=0; n<nodes.size(); ++n)
	{
		//FIXME:
		//nodes[n]->getNeighbors().clear();
		nodes[n]->clearNeighbors();
	}
	// for sequential action on each node:
	std::deque<NaviNode*> ns;
	for (u32 n=0; n<points.size(); ++n)
		ns.push_back(nodes[n]);
////////////////////////////////////////////////////////////////////////////////
u32 num = 0;
u32 neighborNum = 1;
NaviNode* currentNode = 0;
std::deque<NaviNode*>::iterator iter;
while (!ns.empty())
{
	neighborNum = num + 1;
	currentNode = ns.front();
	ns.pop_front();
	
	if (currentNode->getId() != num)
	{
		++num;
		continue;
	}
	
	for (iter=ns.begin(); iter!=ns.end(); ++iter)
	{
		NaviNode* n = *iter;
		if (n->getId() != neighborNum)
		{
			++neighborNum;
			continue;
		}
		
		core::vector3df p1 = points[num];
		core::vector3df p2 = points[neighborNum];
		if (p1.getDistanceFromSQ(p2) <= maxDistSQ)
		{
			currentNode->addNeighbor(n);
			n->addNeighbor(currentNode);
		}
		++neighborNum;
	}

	++num;
}
////////////////////////////////////////////////////////////////////////////////
	for (u32 n=0; n<nodes.size(); ++n)
	{
		NaviNode* node = nodes[n];
		setNeighborIdsFromNeighbors(node); //<----------
	}
	
	return true;
}

void NaviNode::setNeighborIdsFromNeighbors(NaviNode* node)
{
	core::array<u32> neighborIds;
	
	core::array<NaviNode*> neighbors = node->getNeighbors();
	for (u32 n=0; n<neighbors.size(); ++n)
	{
		neighborIds.push_back(neighbors[n]->getId());
	}
	
	node->setNeighborIds(neighborIds);
}


u32 NaviNode::getMinNeighborCount(const std::vector<NaviNode*>& nodes)
{
	u32 minCount = UINT_MAX;
	
	for (u32 n=0; n<nodes.size(); ++n)
	{
		u32 count = nodes[n]->getNeighborCount();
		if (count < minCount)
			minCount = count;
	}
	return minCount;
}

bool NaviNode::containsNodeWithNoNeighbor(const std::vector<NaviNode*>& nodes)
{
	for (u32 n=0; n<nodes.size(); ++n)
	{
		if (nodes[n]->hasNoNeighbor())
			return true;
	}

	return false;
	
}

bool NaviNode::allNodesHaveNeighbors(const std::vector<NaviNode*>& nodes)
{
	for (u32 n=0; n<nodes.size(); ++n)
	{
		if (nodes[n]->hasNoNeighbor())
			return false;
	}

	return true;

}


bool NaviNode::IsGoal(const NaviNode& goalNode)
{
	return (this->getId() ==  goalNode.getId());
}

bool NaviNode::IsSameState(const NaviNode& rhs)
{
	return (this->getPos().equals(rhs.getPos(), 0.001f));
}


bool NaviNode::GetSuccessors(AStarSearch<NaviNode>* astarsearch, NaviNode* parentNode)
{
	//FIXME:
	//segfault!
	//if (parentNode->getNeighborCount() == 0)
	//	return false;
	
	u32 parentId = UINT_MAX;
	if (parentNode)
		parentId = parentNode->getId(); 
	
	core::array<NaviNode*> neighbors = this->getNeighbors();
	for (u32 n=0; n<neighbors.size(); ++n)
	{
		NaviNode neighbor = *(neighbors[n]);
		if (neighbor.getId() == parentId)
			continue;
		//FIXME
		
		if (neighbor.isReserved() == true)
		{
			//printf("Node(%i) is reserved!\n", neighbor.getId());
			//printf("neighbor.isReserved() => %i\n", neighbor.isReserved());
			continue;
		}
		
		astarsearch->AddSuccessor(neighbor);
	}
	
	return true;
	
}


f32 NaviNode::GetCost(const NaviNode& successor)
{
	//if (successor.isReserved())
		//return FLT_MAX;
	
	core::vector3df p1 = this->getPos();
	core::vector3df p2 = successor.getPos();
	
	return p1.getDistanceFrom(p2);
}


f32 NaviNode::GoalDistanceEstimate(const NaviNode& goalNode)
{
	//if (this->isReserved())
		//return FLT_MAX;
	
	core::vector3df p1 = this->getPos();
	core::vector3df p2 = goalNode.getPos();
	
	return p1.getDistanceFromSQ(p2);
}

void NaviNode::setNeighbors(const core::array<NaviNode*>& nodes)
{
	this->setNeighborsFromIds(nodes, this->getNeighborIds());
}



bool NaviNode::u32FromWchars
(
	const wchar_t* cs,
	u32& n
)
{
	std::wstringstream ss;
	ss << cs;
	ss >> n;
	
	return (!ss.fail());
}


core::array<u32> NaviNode::u32sFromWchars(const wchar_t* cs)
{
	core::array<u32> ns;
	
	std::wstringstream ss;
	ss << cs;
	
	u32 n = 0;
	while (!ss.fail())
	{
		ss >> n;
		
		if (!ss.fail())
			ns.push_back(n);
	}
	
	return ns;
}

void NaviNode::print()
{
	printf("Node=%u, ", getId());
	
	core::vector3df pos = getPos();
	printf("Pos=(%.1f,%.1f,%.1f), ", pos.X, pos.Y, pos.Z);
	
	if (Reserved)
		printf("IsReserved=TRUE, ");
	//else
		//printf("IsReserved=FALSE, ");
	
	if (!Neighbors.empty())
	{
		printf("Neighbors=");
		for (u32 n=0; n<Neighbors.size(); ++n)
		{
			printf("%u ", Neighbors[n]->getId());
		}
		//printf(", ");
	}
	
	/***
	if (!NeighborIds.empty())
	{
		printf("NeighborIds=");
		for (u32 n=0; n<NeighborIds.size(); ++n)
		{
			printf("%u ", NeighborIds[n]);
		}
	}
	***/
	printf("\n");
}

NaviNode* NaviNode::getNeighbor(u32 id)
{
	for (u32 n=0; n<Neighbors.size(); ++n)
	{
		if ((Neighbors[n]->getId() == id) && (this->getId() != id))
			return Neighbors[n];
	}
	
	return 0;
}


void NaviNode::addNeighbor(NaviNode* n)
{
	u32 neighborId = n->getId();
	if (this->getId() == neighborId)
		return;
	
	Neighbors.push_back(n);
	//NeighborIds.push_back(neighborId); //gotcha!!
}


void NaviNode::removeNeighbor(u32 id)
{
	if (this->getId() == id)
		return;
	
	NaviNode* neighbor = this->getNeighbor(id);
	if (!neighbor)
		return;
	
	s32 index = -1;
	index = Neighbors.binary_search(neighbor);
	if (index > -1)
		Neighbors.erase(index);
	
	index = NeighborIds.binary_search(id);
	if (index > -1)
		NeighborIds.erase(index);
	
	
}


core::array<u32> NaviNode::getNeighborIds()
{
	return NeighborIds;
}


void NaviNode::setNeighborsFromIds
(
	const core::array<NaviNode*>& nodes,
	const core::array<u32>& ids
)
{
	/***
	printf("setNeighborsFromIds(): ");
	for (u32 m=0; m<ids.size(); ++m)
		printf("%u ", ids[m]);
	printf("\n");
	***/
	u32 id = UINT_MAX;
	NaviNode* node = 0;
	for (u32 m=0; m<ids.size(); ++m)
	{
		id = ids[m];
		if (id == this->getId())
			continue;
		
		for (u32 n=0; n<nodes.size(); ++n)
		{
			node = nodes[n];
			if (node->getId() == id)
			{
				this->addNeighbor(node);
				break;
			}
		}
	}
}


/***
void NaviNode::()
{
	
}
***/



