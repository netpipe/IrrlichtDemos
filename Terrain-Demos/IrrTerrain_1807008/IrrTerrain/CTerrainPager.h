#ifndef _C_TERRAIN_PAGER_
#define _C_TERRAIN_PAGER_

#include <irrlicht.h>
#include "CTerrain.h"
#include <vector>	// TODO : use irr collection

using namespace irr;

//--------------------------------------------------------------------------------------
// Terrain Pager parameters
//--------------------------------------------------------------------------------------
struct STerrainParameters
{
	c8 ** MapListFileName;
	c8 ** ColorMapListFileName;
	CTerrain::E_TERRAIN_QUALITY Quality;
	f32 MapSize;
	core::vector3df Position;
	core::vector3df Scale;	
	f32 LodTextureScale;
	core::dimension2di Dimension;	
	f32 DistanceLoad;
	f32 DistanceUnload;
	f32 DistanceMaxRender;
	bool Fog;
	bool Debug;
};


//--------------------------------------------------------------------------------------
// buffer class to store terrain informations
//--------------------------------------------------------------------------------------
class CBufferZone : public scene::ISceneNode
{
public:

	// create buffer zone
	CBufferZone(f32 size, core::vector3df pos, core::vector3df scale,  scene::ISceneNode* parent,scene::ISceneManager* smgr,s32 id) : scene::ISceneNode(parent, smgr, id)
	{
		SceneManager = smgr;		
		setPosition(pos);
		setScale(scale);	
		core::vector3df min = pos * scale;
		core::vector3df max = (pos + core::vector3df(size,size*0.2f,size)) * scale;	
		bbox = core::aabbox3df(min, max);
		setVisible(true);
		setLoaded(false);
		setAutomaticCulling(scene::EAC_OFF);
	}	
	

	// render bounding box of buffer zone
	virtual void render() 
	{
 		if (DebugDataVisible & scene::EDS_BBOX)
		{							
			video::SMaterial m;
			m.Lighting = false;			
			SceneManager->getVideoDriver()->setMaterial(m);
			video::SColor c (255,255,0,0);			
			SceneManager->getVideoDriver()->draw3DBox(this->getBoundingBox(), c);
		}		
	}

	void OnRegisterSceneNode()
	{
		if (IsVisible && loaded)
			SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const { return bbox; }
	bool isLoaded() { return loaded; }
	void setLoaded(bool state) { loaded = state; }
	CTerrain * getObjectInside() { return object; }
	void setObjectInside(CTerrain * terrain) { object = terrain; }

private:
	core::aabbox3df bbox;
	bool loaded;	
	CTerrain * object;
};


//--------------------------------------------------------------------------------------
// Terrain Pager Class
//--------------------------------------------------------------------------------------
class CTerrainPager : public scene::ISceneNode
{
    public:
		CTerrainPager(STerrainParameters param, scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id);
        ~CTerrainPager();
		virtual void OnRegisterSceneNode();
		virtual void render();
		virtual const core::aabbox3d<f32>& getBoundingBox() const;
		void loadTerrain(u16 index);
		void computePosition(u16);

	private:
		core::aabbox3d<f32> Box;
		std::vector<CBufferZone*> zoneList;
		std::vector<c8*> heightMapList;		
		f32 posX, posZ;
		u16 x;
		STerrainParameters Param;
};

#endif