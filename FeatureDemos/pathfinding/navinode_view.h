#ifndef _NAVINODE_VIEW_H_
#define _NAVINODE_VIEW_H_


#include <irrlicht.h>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>

#include "navinode.h"
#include "segment.h"

using namespace irr;

#ifndef NAVINODE_VIEW_PROPERTIES
#define NAVINODE_VIEW_PROPERTIES
//#define MAX_DIST_SQ 400000.0f
#endif



class NaviNodeView
{
	public:
	NaviNodeView(IrrlichtDevice* device);
	virtual ~NaviNodeView();
	void setNodes(core::array<NaviNode*> nodes);
	
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
	
	NaviNode* findNode(u32 id);
	
	bool findSelectedNode(u32& id, u32 radius);

	NaviNode* selectNode
	(
		const core::vector3df& cursor,
		bool& found,
		f32 radius = 10.0f
	);
	
	void drawNodes();
	void drawNode(NaviNode* node);
	
	void drawReservedNodes();
	void drawStartNode(s32 id);
	void drawEndNode(s32 id);
	void drawSelectedNode(s32 id);
	
	void drawPath();
	
	void drawPixmap(NaviNode* node, video::ITexture* tex, f32 scale = 50.0f);
	void drawPixmap(const core::vector3df& pos, video::ITexture* tex, f32 scale = 50.0f);
	
	void drawSegments();
	//void drawWaypointWithRadius(f32 radius, video::SColor color = 0xFFEECC00); //yellow
	void drawWaypointWithRadius(f32 radius, video::SColor color = 0xFF0000FF);
	void addTextSceneNode(NaviNode* node);
	
	void setPathNodes(const core::array<NaviNode*>& nodes) { PathNodes.clear(); PathNodes = nodes; }
	void setSegments(const core::array<Segment>& segments) { Segments.clear(); Segments = segments; }
	void setRadius(f32 radius) { Radius = radius; }
	void setYOffset(f32 offset) { YOffset = offset; }
	void setLineColor(video::SColor color) { LineColor = color; }
	void setTextColor(video::SColor color) { TextColor = color; }
	void setColor(video::SColor color) { LineColor = color; TextColor = color; }
	
	
	core::array<NaviNode*> getNodes() const { return Nodes; }
	core::array<Segment> getSegments() const { return Segments; }
	f32 getRadius() const { return Radius; }
	f32 getYOffset() const { return YOffset; }
	video::SColor getLineColor() const { return LineColor; }
	video::SColor getTextColor() const { return TextColor; }
private:
	
	IrrlichtDevice* Device;
	
	core::array<NaviNode*> Nodes;
	core::array<Segment> Segments;
	core::array<NaviNode*> PathNodes;
	
	f32 Radius;
	f32 YOffset;

	video::SColor LineColor;
	video::SColor PathColor;
	video::SColor TextColor;
	
	gui::IGUIFont* Font;
	core::array<scene::IBillboardTextSceneNode*> Texts;
	std::map<u32, scene::IBillboardTextSceneNode*> IdTextMap;
	
	
	
	
	// for drawing reserved node:
	core::array<video::S3DVertex> Vertices;
	core::array<u16> Indices;
	
	video::ITexture* Texture;
	video::ITexture* ReservedTexture;
	video::ITexture* StartTexture;
	video::ITexture* EndTexture;
	video::ITexture* SelectedTexture;
	video::ITexture* ReticleTexture;
	
	video::SMaterial LineMaterial;
	video::SMaterial TextMaterial;
	video::SMaterial PixmapMaterial;
	
	void setupPixmapQuad();
	void setupMaterials();
};

#endif // _NAVINODE_VIEW_H_
