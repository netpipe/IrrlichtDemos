#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <ctime>


#include "navinode_control.h"

NaviNodeControl::NaviNodeControl(IrrlichtDevice* device)
:	Selector(0),
	Device(device),
	View(0),
	Filename(""),
	StartId(-1),
	EndId(-1),
	CurrentId(UINT_MAX),
	MaxDistSQ(MAX_DIST_SQ),
	MaxDist(sqrt(fabs(MAX_DIST_SQ)))
{
	//Device->getFileSystem()->addFileArchive("...");
	View = new NaviNodeView(Device);
	
}


NaviNodeControl::~NaviNodeControl()
{
	if (View) { delete View; View = 0; }
	
	//save(Device, Nodes, Filename);
	
	// cleanup nodes:
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		delete Nodes[n];
		Nodes[n] = 0;
	}
	
	Nodes.clear();
}


void NaviNodeControl::draw()
{
	View->drawWaypointWithRadius(MaxDist, 0x8000B2EE);
	View->drawNodes();
	View->drawReservedNodes();
	View->drawStartNode(StartId);
	View->drawEndNode(EndId);
	View->drawSegments();
	
	View->drawPath();
}


void NaviNodeControl::setWaypoints(const std::vector<core::vector3df>& waypoints, bool testLineOfSight)
{
	if (waypoints.size() < 2)
		return;
	
	// cleanup:
	for (u32 n=0; n<Nodes.size(); ++n) { delete Nodes[n]; Nodes[n] = 0; }
	Nodes.clear();
	
	// create nodes:
	for (u32 n=0; n<waypoints.size(); ++n)
	{
		core::vector3df pos = waypoints[n];
		Nodes.push_back(new NaviNode(n, pos));
	}
	
	// setup neighbors of nodes:
	//f32 maxDistSQ = MAX_DIST_SQ;
	//printf("maxDistSQ=%f\n", maxDistSQ);
	if (testLineOfSight)
		NaviNode::setupNeighbors(Device, Selector, Nodes, waypoints, MaxDistSQ);
	else
		NaviNode::setupNeighbors(Nodes, waypoints, MaxDistSQ);
	
	setupSegments(Nodes);
	
	View->setNodes(Nodes);
	View->setSegments(Segments);

}


core::array<NaviNode*> NaviNodeControl::loadNodes(IrrlichtDevice* device, const core::stringc& filename)
{
	core::array<NaviNode*> nodes;
	
	if (!device)
		device = createDevice(video::EDT_NULL);
	
	if (!device)
		return nodes;
	
	io::IXMLReader* xmlReader = device->getFileSystem()->createXMLReader(filename);
	if (!xmlReader)
		return nodes;
	printf("xmlReader->getSourceFormat()=%u\n", xmlReader->getSourceFormat());
	
	const core::stringw nodeTag(L"NaviNode");
	const core::stringw idTag(L"Id");
	const core::stringw posTag(L"Pos");
	const core::stringw neighborsTag(L"Neighbors");
	
	core::stringw currentSection; //keep track of our currentsection

	////////////////////////////////////////////////////////////////////////////
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
						if (NaviNodeControl::u32FromWchars(data, id))
						{
							node->setId(id);
							printf("id=%u\n", id);
						}
					}
					else if (currentSection == neighborsTag)
					{
						core::array<u32> ids = NaviNodeControl::u32sFromWchars(data);
						for (u32 n=0; n<ids.size(); ++n)
						{
							printf("ids[%u]=%u: add as neighbor?? ", n, ids[n]);
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
		//printf("nodes[n]->removeNeighborIdsDeplicates()\n");
		//nodes[n]->removeNeighborIdsDeplicates(); // workaround:
		nodes[n]->setNeighbors(nodes);
	}
	
	for (u32 n=0; n<nodes.size(); ++n)
	{
		node->removeNeighborIdsDeplicates();
		nodes[n]->print();
	}
	return nodes;
}


void NaviNodeControl::save(IrrlichtDevice* device, const core::array<NaviNode*>& nodes, const core::stringc& filename)
{
	if (nodes.size() == 0)
		return;
	
	if (!device)
		device = createDevice(video::EDT_NULL);
	if (!device)
		return;
	
	io::IXMLWriter* xmlWriter = device->getFileSystem()->createXMLWriter(filename);
	if (!xmlWriter)
		return;
	
	xmlWriter->writeXMLHeader();

NaviNode* node = 0;
for (u32 n=0; n<nodes.size(); ++n)
{
	node = nodes[n];
	
	core::stringw id = stringFromU32(node->getId());
	core::stringw x = stringFromF32(node->getPos().X);
	core::stringw y = stringFromF32(node->getPos().Y);
	core::stringw z = stringFromF32(node->getPos().Z);
	node->removeNeighborIdsDeplicates();
	core::stringw neighbors = stringFromU32s(node->getNeighborIds());
	
	printf("NaviNodeControl::save(): neighbors=");
	core::array<u32> ids = node->getNeighborIds();
	for (u32 n=0; n<ids.size(); ++n)
		printf("%u ", ids[n]);
	printf("\n");
	
	
	////////////////////////////////////////////////////////////////////////////
	// node
	////////////////////////////////////////////////////////////////////////////
	xmlWriter->writeElement(L"NaviNode");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"Id");
	xmlWriter->writeText(id.c_str());
	xmlWriter->writeClosingTag(L"Id");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement
	(
		L"Pos", true,
		L"X", x.c_str(),
		L"Y", y.c_str(),
		L"Z", z.c_str()
	);
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeElement(L"Neighbors");
	xmlWriter->writeText(neighbors.c_str());
	xmlWriter->writeClosingTag(L"Neighbors");
	xmlWriter->writeLineBreak();
	
	xmlWriter->writeClosingTag(L"NaviNode");
	xmlWriter->writeLineBreak();
	

	////////////////////////////////////////////////////////////////////////////
}
	xmlWriter->drop();
	
	

}

core::array<u32> NaviNodeControl::getUnusedIds(const core::array<NaviNode*>& nodes)
{
	core::array<u32> unused;
	
	u32 max = getMaxId(nodes);
	printf("max id=%u\n", max);
	for (u32 n=0; n<max; ++n)
		unused.push_back(n);
	
	u32 id = UINT_MAX;
	s32 index = -1;
	for (u32 i = 0; i < nodes.size(); i++)
	{
		id = nodes[i]->getId();
		index = unused.binary_search(id);
		
		if (index > -1)
			unused.erase(index);
	}
	
	return unused;
}


u32 NaviNodeControl::getMaxId(const core::array<NaviNode*>& nodes)
{
	u32 maxId = 0;
	for (u32 n=0; n<nodes.size(); ++n)
	{
		u32 id = nodes[n]->getId();
		if (id > maxId)
			maxId = id;
	}
	return maxId;
}

bool NaviNodeControl::findSelectedNode(u32& id)
{
	gui::ICursorControl* cursor = Device->getCursorControl();
	//if (!cursor->isVisible())
		//return false;
	
	scene::ISceneCollisionManager* cmgr = Device->getSceneManager()->getSceneCollisionManager();
	core::vector2d<s32> cursorPos = cursor->getPosition();
	printf("cursorPos=(%i,%i)\n", cursorPos.X, cursorPos.Y);
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		NaviNode* node = Nodes[n];
		core::vector2d<s32> pos = cmgr->getScreenCoordinatesFrom3DPosition(node->getPos());
		if (cursorPos.getDistanceFromSQ(pos) < 100)
		{
			id = node->getId();
			return true;
		}
	}	
	return false;
}

NaviNode* NaviNodeControl::findNode
(
	const core::vector3df& cursor,
	bool& found,
	f32 radius
)
{
	found = false;
	f32 dsq = radius * radius;
	
	for (u32 i = 0; i < Nodes.size(); i++)
	{
		NaviNode* node = Nodes[i];
		core::vector3df pos = node->getPos();
		
		if (cursor.getDistanceFromSQ(pos) <= dsq)
		{
			printf("selected node=%i\n", i);
			found = true;
			return node;
		}
	}
	return 0;
}




void NaviNodeControl::addYOffset(core::array<NaviNode*>& nodes, f32 yOffset)
{
	for (u32 i = 0; i < nodes.size(); i++)
	{
		core::vector3df pos = nodes[i]->getPos();
		pos.Y += yOffset;
		
		nodes[i]->setPos(pos);
	}
}


void NaviNodeControl::save(const core::stringc& filename)
{
	Filename = filename;
	this->save(Device, Nodes, Filename);
	
}


bool NaviNodeControl::u32FromWchars(const wchar_t* cs, u32& n)
{
	std::wstringstream ss;
	ss << cs;
	ss >> n;
	
	return (!ss.fail());
}


core::array<u32> NaviNodeControl::u32sFromWchars(const wchar_t* cs)
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


core::stringw NaviNodeControl::stringFromF32(f32 val)
{
	std::wstringstream ss;
	ss.setf(std::ios::fixed);
	ss.precision(1);
	
	ss << val;
	return core::stringw(std::wstring(ss.str()).c_str());
}

core::stringw NaviNodeControl::stringFromU32(u32 val)
{
	std::wstringstream ss;
	ss << val;
	return core::stringw(std::wstring(ss.str()).c_str());
}

core::stringw NaviNodeControl::stringFromU32s(const core::array<u32>& nums)
{
	std::wstringstream ss;
	for (u32 n=0; n<nums.size(); ++n)
	{
		ss << nums[n] << " ";
	}
	
	return core::stringw(std::wstring(ss.str()).c_str());
}


s32 NaviNodeControl::getNumberOfMiddleNodes(NaviNode* n0, NaviNode* n1, f32 maxDistSQ)
{
	if (maxDistSQ <= 0.0f)
		return -1;
	
	f32 maxDist = sqrt(maxDistSQ);
	core::vector3df p0 = n0->getPos();
	core::vector3df p1 = n1->getPos();
	return s32(ceil
	(
		p0.getDistanceFrom(p1) / maxDist
	));
}


void NaviNodeControl::resetAnimatorCollisionResponse(scene::ISceneNode* node)
{
	node->updateAbsolutePosition();
	
	const core::list<scene::ISceneNodeAnimator*>& anims = node->getAnimators();
	//const core::list<scene::ISceneNodeAnimator*> anims = node->getAnimators();
	core::list<scene::ISceneNodeAnimator*>::ConstIterator iter = anims.begin();
	while (iter != anims.end())
	{
		if ((*iter)->getType() == scene::ESNAT_COLLISION_RESPONSE)
		{
			scene::ISceneNodeAnimatorCollisionResponse* anim = static_cast<scene::ISceneNodeAnimatorCollisionResponse*>(*iter);
			
			core::vector3df pos = node->getAbsolutePosition();
			pos += anim->getEllipsoidTranslation().Y * 2.0f; //FIXME
			node->setPosition(pos);
			
			anim->setTargetNode(node);
			return;
		}
		++iter;
	}
	
}




/*
 This function is needed to make some delay between addPath and delPath,
 becasue in standard case (without sleep()), user may call one of this functions on key press event,
 so if user have pressed key too long (like 10ms too long), addPath function may be called many times in one second,
 so that will add many same, or very close (about 0.0001) entries. sleep() is for prevent this.
 Also, I made my own sleep() function to make code portable between operating systems.
*/
void NaviNodeControl::sleep(float ms)
{
	ms /= 100;
	clock_t a = clock () + ms * CLOCKS_PER_SEC ;
	while (clock() < a) {}
}


bool NaviNodeControl::nodesAreTooClose
(
	NaviNode* n1,
	NaviNode* n2,
	f32 minDistance
)
{
	core::vector3df p1 = n1->getPos();
	core::vector3df p2 = n2->getPos();
	return (p1.getDistanceFromSQ(p2) <= (minDistance * minDistance));
}


NaviNode* NaviNodeControl::getNode(u32 id) const
{
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		if (Nodes[n]->getId() == id)
			return Nodes[n];
	}
	
	return 0;
}

void NaviNodeControl::addNode
(
	IrrlichtDevice* device,
	bool testLineOfSight,
	f32 maxSlopeDegs
)
{
	scene::ISceneCollisionManager* cmgr = device->getSceneManager()->getSceneCollisionManager();
	scene::ICameraSceneNode* camera = device->getSceneManager()->getActiveCamera();
	camera->updateAbsolutePosition();
	
	
	core::line3df ray;
	ray.start = camera->getAbsolutePosition();
	ray.end = ray.start + (camera->getTarget() - ray.start).normalize()*100000.0f;

	core::vector3df intersection;
	core::triangle3df tri;
	const scene::ISceneNode* selectedNode = 0;
	selectedNode = cmgr->getSceneNodeAndCollisionPointFromRay(ray, intersection, tri);
	if (!selectedNode)
		return;
	core::vector3df normal = tri.getNormal().normalize();
	core::vector3df yaxis(0.0f, 1.0f, 0.0f);
	
	if (acos(fabs(normal.dotProduct(yaxis))) * core::RADTODEG > maxSlopeDegs)
		return;
	
	this->addNode(intersection, testLineOfSight);
}



void NaviNodeControl::addNode(const core::vector3df& pos, bool testLineOfSight)
{
	u32 id = 0;
	core::array<u32> unused = this->getUnusedIds(Nodes);
	if (unused.empty())
		id = getMaxId(Nodes) + 1;
	else
		id = unused[0];
	
	NaviNode* node = new NaviNode(id, pos);

	
	
	// setup neighbors of nodes:
	////////////////////////////////////////////////////////////////////////////
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		NaviNode* nn = Nodes[n];
		u32 i = nn->getId();
		core::vector3df p = nn->getPos();
		
		printf("NaviNodeControl::addNode(): comparing nodes=> (%u) and (%u)\n", id, i);
		
		if (pos.getDistanceFromSQ(p) > MaxDistSQ)
			continue;
		if	((testLineOfSight) && (!NaviNode::pathIsClear(Device, Selector, pos, p)))
			continue;
		node->addNeighbor(nn);
		node->addNeighborId(i);
			
		nn->addNeighbor(node);
		nn->addNeighborId(id);
	}

	////////////////////////////////////////////////////////////////////////////
	Nodes.push_back(node);
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		Nodes[n]->print();
	}
	
	setupSegments(Nodes);
	
	View->setNodes(Nodes);
	View->setSegments(Segments);
}


//TODO: not tested!
void NaviNodeControl::removeNode(u32 id)
{
	NaviNode* node = getNode(id);
	if (!node)
		return;
	
	core::array<u32> neighborIds = node->getNeighborIds();
	for (u32 n=0; n<neighborIds.size(); ++n)
	{
		u32 neighborId = neighborIds[n];
		NaviNode* neighbor = getNode(neighborId);
		if (!neighbor)
			continue;
		neighbor->removeNeighbor(id);
	}
	
	////////////////////////////////////////////////////////////////////////////
	s32 index = -1;
	////////////////////////////////////////////////////////////////////////////
	
	
	// remove text scene node:
	//TODO: do this in View!
	/***
	scene::IBillboardTextSceneNode* text = 0;
	
	std::map<u32, scene::IBillboardTextSceneNode*>::iterator iter = IdTextMap.find(id);
	if (iter != IdTextMap.end())
	{
		text = iter->second;
		IdTextMap.erase(iter);
	}
	
	index = Texts.binary_search(text);
	if (index > -1)
		Texts.erase(index);
	
	if (text) { text->remove(); }
	***/
	

	// remove entry from Nodes:
	index = Nodes.binary_search(node);
	if (index > -1)
		Nodes.erase(index);
	for (u32 n=0; n<Nodes.size(); ++n) { Nodes[n]->print(); };
		
	// remove segment(s)
	// lazy:
	this->setupSegments(Nodes);

	
	// remove node:
	delete node;
	node = 0;

	View->setNodes(Nodes);
	View->setSegments(Segments);

}


//TODO:
void NaviNodeControl::removeSegment(u32 id1, u32 id2)
{
	NaviNode* n1 = getNode(id1);
	if (!n1)
		return;

	NaviNode* n2 = getNode(id2);
	if (!n2)
		return;
	
	//TODO:
	/***
	s32 index = -1;
	
	core::array<NaviNode*> nbs1 = n1->getNeighbors();
	if ((index = nbs1.binary_search(id2)) > -1)
	{
		nbs1.erase(index);
		n1->setNeighbors(nbs1);
	}
	
	index = -1;
	core::array<NaviNode*> nbs2 = n2->getNeighbors();
	if ((index = nbs2.binary_search(id1)) > -1)
	{
		nbs2.erase(index);
		n2->setNeighbors(nbs2);
	}
	
	setNeighborIds(const core::array<u32>& neighborIds)
	
	***/
}


void NaviNodeControl::setFilename(const core::stringc& filename)
{
	// cleanup nodes:
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		delete Nodes[n];
		Nodes[n] = 0;
	}
	Nodes.clear();
	
		
	Nodes = loadNodes(Device, filename);
	
	setupSegments(Nodes); // for drawing
	
	Filename = filename;
	
	View->setNodes(Nodes);
	View->setSegments(Segments);
}


void NaviNodeControl::setNodeReserved(s32 id, bool reserved)
{
	if (id < 0)
		return;
	
	NaviNode* node = this->getNode(u32(id));
	if (!node)
		return;
	
	node->setReserved(reserved);
}


void NaviNodeControl::unsetAllNodesReserved()
{
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		Nodes[n]->setReserved(false);
	}
}


void NaviNodeControl::setMaxDistSQ(f32 maxDistSQ)
{
	MaxDistSQ = maxDistSQ;
	MaxDist = sqrt(fabs(maxDistSQ));
	
	/***
	std::vector<core::vector3df> waypoints;
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		waypoints.push_back(Nodes[n]->getPos());
	}
	
	NaviNode::setupNeighbors(Device, Selector, Nodes, waypoints, MaxDistSQ);
	for (u32 n=0; n<Nodes.size(); ++n)
		Nodes[n]->print();
	setupSegments(Nodes);
	
	View->setNodes(Nodes);
	View->setSegments(Segments);
	***/
}

void NaviNodeControl::setupSegments(const core::array<NaviNode*>& nodes)
{
	if (nodes.size() < 2)
		return;
	
	// clean up old segments:
	Segments.clear();
	
	for (u32 n=0; n<nodes.size(); ++n)
	{
		NaviNode* node = nodes[n];
		u32 id = node->getId();
		
		core::array<u32> neighbors = node->getNeighborIds();
		for (u32 n=0; n<neighbors.size(); ++n)
		{
			//printf("neighbors[n]=%u\n", neighbors[n]);
			Segment segment(id, neighbors[n]);
			//segment.print();
			if (!segmentIsFound(Segments, segment))
				Segments.push_back(segment);
		}
	}
	
	////////////////////////////////////////////////////////////////////////////
	//printf("Segments.size()=%i\n", Segments.size());
	//for (u32 n=0; n<Segments.size(); ++n)
		//Segments[n].print();
	
}


bool NaviNodeControl::segmentIsFound(const core::array<Segment>& ss, const Segment& s)
{
	for (u32 n=0; n<ss.size(); ++n)
	{
		if (s == ss[n])
			return true;
	}
	
	return false;
}


