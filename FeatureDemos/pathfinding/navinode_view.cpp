#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <ctime>


#include "navinode_view.h"
#include "simple_util.h"


using namespace irr;

NaviNodeView::NaviNodeView(IrrlichtDevice* device)
:	Device(device),
	Radius(20.0f),
	YOffset(10.0f),
	LineColor(0x8F00FF00),
	PathColor(0xFFFF0000),
	TextColor(0x8F0000FF),
	Font(0),
	Texture(0),
	ReservedTexture(0),
	StartTexture(0),
	EndTexture(0),
	ReticleTexture(0)
{
	Device->getFileSystem()->addFileArchive("./data/");
	Font = Device->getGUIEnvironment()->getFont("media/fonts/bh/bh.xml");
	
	
	this->setupMaterials();
	this->setupPixmapQuad();	
}


NaviNodeView::~NaviNodeView()
{
	for (u32 i = 0; i < Texts.size(); i++) { Texts[i]->remove(); }
	Texts.clear();
	IdTextMap.clear();
}


void NaviNodeView::setNodes(core::array<NaviNode*> nodes)
{
	Nodes.clear();
	Nodes = nodes;
	
	for (u32 i = 0; i < Texts.size(); i++) { Texts[i]->remove(); }
	Texts.clear();
	IdTextMap.clear();

	for (u32 n=0; n<Nodes.size(); ++n)
		addTextSceneNode(Nodes[n]);
}


NaviNode* NaviNodeView::findNode(u32 id)
{
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		if (Nodes[n]->getId() == id)
			return Nodes[n];
	}
	
	return 0;
}


bool NaviNodeView::findSelectedNode(u32& id, u32 radius)
{
	gui::ICursorControl* cursor = Device->getCursorControl();
	//if (!cursor->isVisible())
		//return false;
	
	scene::ISceneCollisionManager* cmgr = Device->getSceneManager()->getSceneCollisionManager();
	core::vector2d<s32> cursorPos = cursor->getPosition();
	//printf("cursorPos=(%i,%i)\n", cursorPos.X, cursorPos.Y);
	for (u32 n=0; n<Nodes.size(); ++n)
	{
		NaviNode* node = Nodes[n];
		core::vector2d<s32> pos = cmgr->getScreenCoordinatesFrom3DPosition(node->getPos());
		if (cursorPos.getDistanceFromSQ(pos) < (radius*radius))
		{
			id = node->getId();
			return true;
		}
	}	
	return false;
}

NaviNode* NaviNodeView::selectNode
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


void NaviNodeView::drawNodes()
{
	if (Nodes.size() == 0)
		return;
	
	for (int i = 0; i < int(Nodes.size()); i++)
		drawNode(Nodes[i]);
}

void NaviNodeView::drawNode(NaviNode* node)
{
	core::vector3df pos = node->getPos();
	std::vector<core::vector3df> pts = SimpleUtil::getPointsOnHorizontalCircle(pos, Radius, YOffset);
	if (int(pts.size()) <= 1)
		return;
	
	video::IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, core::matrix4());
	driver->setMaterial(LineMaterial);

	int n1, n2;
	for (int n = 0; n < int(pts.size()); n++)
	{
		n1 = n;
		n2 = n + 1;
		if (n2 >= int(pts.size()))
			n2 = 0;
		driver->draw3DLine(pts[n1], pts[n2], TextColor); 
	}
}

void NaviNodeView::drawReservedNodes()
{
	if (Nodes.size() == 0)
		return;
	
	for (int i = 0; i < int(Nodes.size()); i++)
	{
		if (Nodes[i]->isReserved())
		drawPixmap(Nodes[i], ReservedTexture);
	}
}


void NaviNodeView::drawStartNode(s32 id)
{
	if (id < 0)
		return;
	
	NaviNode* node = this->findNode(u32(id));
	if (!node)
		return;
	
	this->drawPixmap(node, StartTexture);
}


void NaviNodeView::drawEndNode(s32 id)
{
	if (id < 0)
		return;
	
	NaviNode* node = this->findNode(u32(id));
	if (!node)
		return;
	
	this->drawPixmap(node, EndTexture);

}

void NaviNodeView::drawSelectedNode(s32 id)
{
	if (id < 0)
		return;
	
	NaviNode* node = this->findNode(u32(id));
	if (!node)
		return;
	
	this->drawPixmap(node, SelectedTexture, 100.0f);

}

void NaviNodeView::drawPath()
{
	if (PathNodes.size() <= 1)
		return;
	
	video::IVideoDriver* driver = Device->getVideoDriver();
	driver->setMaterial(LineMaterial);
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	
	for (u32 n=0; n<(PathNodes.size()-1); ++n)
	{
		core::vector3df p1 = PathNodes[n]->getPos();
		core::vector3df p2 = PathNodes[n+1]->getPos();
		if (p1.equals(p2))
			continue;
		p1.Y += YOffset;
		p2.Y += YOffset;
		driver->draw3DLine(p1, p2, PathColor);
	}

}

void NaviNodeView::drawPixmap(NaviNode* node, video::ITexture* tex, f32 scale)
{
	this->drawPixmap(node->getPos(), tex, scale);
}


void NaviNodeView::drawPixmap(const core::vector3df& pos, video::ITexture* tex, f32 scale)
{
	scene::ICameraSceneNode* camera = Device->getSceneManager()->getActiveCamera();
	if (!camera)
		return;

	////////////////////////////////////////////////////////////////////////////
	core::matrix4 m1;
	
	camera->updateAbsolutePosition();
	core::vector3df target = camera->getTarget() - camera->getAbsolutePosition();
	target.Y = 0.0f;

	if (!core::iszero(target.getLengthSQ()))
	{
		target.normalize();
		core::vector3df zdir(0.0f, 0.0f, 1.0f);
	
		core::quaternion q;
		q.rotationFromTo(zdir, target);
	
		core::vector3df eulerRadians;
		q.toEuler(eulerRadians);
	
		m1.setRotationRadians(eulerRadians);
	}

	////////////////////////////////////////////////////////////////////////////
	core::matrix4 m2;
	
	//core::vector3df pos = node->getPos();
	core::vector3df p = pos;
	p.Y += YOffset;

	m2.setTranslation(p);
	m2.setScale(scale);
	
	
	////////////////////////////////////////////////////////////////////////////
	video::IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, m2*m1);

	if (tex)
		PixmapMaterial.setTexture(0, tex);
	driver->setMaterial(PixmapMaterial);
	
	driver->drawIndexedTriangleList
	(
		Vertices.const_pointer(), 4,
		Indices.const_pointer(), 2
	);
	
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	
}


void NaviNodeView::drawWaypointWithRadius(f32 radius, video::SColor color)
{
	scene::ISceneCollisionManager* cmgr = Device->getSceneManager()->getSceneCollisionManager();
	scene::ICameraSceneNode* camera = Device->getSceneManager()->getActiveCamera();
	camera->updateAbsolutePosition();
	
	
	core::line3df ray;
	ray.start = camera->getAbsolutePosition();
	ray.end = ray.start + (camera->getTarget() - ray.start).normalize()*10000.0f;

	core::vector3df pos;
	core::triangle3df tri;
	const scene::ISceneNode* selectedNode = 0;
	
	selectedNode = cmgr->getSceneNodeAndCollisionPointFromRay(ray, pos, tri);
	if (!selectedNode)
		return;

	//this->drawPixmap(pos, ReticleTexture);
	
	// draw circle of radius
	std::vector<core::vector3df> pts = SimpleUtil::getPointsOnHorizontalCircle(pos, radius, YOffset, 64);
	if (int(pts.size()) <= 1)
		return;
	
	video::IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, core::matrix4());
	driver->setMaterial(LineMaterial);

	int n1, n2;
	for (int n = 0; n < int(pts.size()); n++)
	{
		n1 = n;
		n2 = n + 1;
		if (n2 >= int(pts.size()))
			n2 = 0;
		driver->draw3DLine(pts[n1], pts[n2], color);
	}
	
}


void NaviNodeView::addTextSceneNode(NaviNode* node)
{
	if (!node)
		return;
	
	u32 id = node->getId();
	scene::IBillboardTextSceneNode* text = Device->getSceneManager()->addBillboardTextSceneNode
	(
		Font,
		stringFromU32(id).c_str(),
		0,
		core::dimension2df(25.0f, 25.0f)
	);
	
	text->setColor(TextColor);
	
	core::vector3df pos = node->getPos();
	pos.Y += YOffset;
	text->setPosition(pos);
	
	Texts.push_back(text);
	IdTextMap[id] = text;
}




bool NaviNodeView::u32FromWchars(const wchar_t* cs, u32& n)
{
	std::wstringstream ss;
	ss << cs;
	ss >> n;
	
	return (!ss.fail());
}


core::array<u32> NaviNodeView::u32sFromWchars(const wchar_t* cs)
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


core::stringw NaviNodeView::stringFromF32(f32 val)
{
	std::wstringstream ss;
	ss.setf(std::ios::fixed);
	ss.precision(1);
	
	ss << val;
	return core::stringw(std::wstring(ss.str()).c_str());
}

core::stringw NaviNodeView::stringFromU32(u32 val)
{
	std::wstringstream ss;
	ss << val;
	return core::stringw(std::wstring(ss.str()).c_str());
}

core::stringw NaviNodeView::stringFromU32s(const core::array<u32>& nums)
{
	std::wstringstream ss;
	for (u32 n=0; n<nums.size(); ++n)
	{
		ss << nums[n] << " ";
	}
	
	return core::stringw(std::wstring(ss.str()).c_str());
}



void NaviNodeView::setupPixmapQuad()
{
	Vertices.set_used(4);

	//S3DVertex => pos, normal, color, texcoords
	video::SColor color(128, 255, 255, 255);
	Vertices[0] = video::S3DVertex(-1.f, 0.f, -1.f,  0.f, 1.f, 0.f,  color,  0.f, 1.f);
	Vertices[1] = video::S3DVertex(-1.f, 0.f,  1.f,  0.f, 1.f, 0.f,  color,  0.f, 0.f);
	Vertices[2] = video::S3DVertex( 1.f, 0.f,  1.f,  0.f, 1.f, 0.f,  color,  1.f, 0.f);
	Vertices[3] = video::S3DVertex( 1.f, 0.f, -1.f,  0.f, 1.f, 0.f,  color,  1.f, 1.f);
	/***
	Vertices[0] = video::S3DVertex(-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, color, 0.f, 1.f);
	Vertices[1] = video::S3DVertex(-0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, color, 0.f, 0.f);
	Vertices[2] = video::S3DVertex( 0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, color, 1.f, 0.f);
	Vertices[3] = video::S3DVertex( 0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, color, 1.f, 1.f);
	***/
	
	
	Indices.set_used(6);
	
	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;
	Indices[3] = 2;
	Indices[4] = 3;
	Indices[5] = 0;
	
}




void NaviNodeView::drawSegments()
{
	video::IVideoDriver* driver = Device->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, core::matrix4());
	driver->setMaterial(LineMaterial);

	for (u32 n=0; n<Segments.size(); ++n)
	{
		Segment seg = Segments[n];
		
		u32 id1 = seg.getNode1();
		u32 id2 = seg.getNode2();
		
		NaviNode* n1 = this->findNode(id1);
		NaviNode* n2 = this->findNode(id2);
		if ((!n1) || (!n2))
			continue;
		core::vector3df p1 = n1->getPos();
		core::vector3df p2 = n2->getPos();
		p1.Y += YOffset;
		p2.Y += YOffset;
		driver->draw3DLine(p1, p2, LineColor);
	}
	
	
}

	
void NaviNodeView::setupMaterials()
{
	LineMaterial.Lighting = false;
	LineMaterial.Thickness = 2.0f;
	LineMaterial.BackfaceCulling = false;
	LineMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	
	
	TextMaterial.Lighting = false;
	TextMaterial.BackfaceCulling = false;
	TextMaterial.MaterialType = video::EMT_SOLID;
	
	
	PixmapMaterial.Lighting = false;
	PixmapMaterial.BackfaceCulling = false;
	PixmapMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	
	ReservedTexture = Device->getVideoDriver()->getTexture("reserved.png");
	StartTexture = Device->getVideoDriver()->getTexture("start.png");
	EndTexture = Device->getVideoDriver()->getTexture("end.png");
	SelectedTexture = Device->getVideoDriver()->getTexture("selected.png");
	ReticleTexture = Device->getVideoDriver()->getTexture("reticle.png");
}


//void NaviNodeView::

