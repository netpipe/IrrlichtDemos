#ifndef _EDITABLETERRAIN_H_INCLUDED_
#define _EDITABLETERRAIN_H_INCLUDED_

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui; 

class EditableTerrain
{
protected:
	float round( float f ){ return (f > 0.0) ? (float)floor(f + 0.5) : (float)ceil(f - 0.5); }
	ITerrainSceneNode* terrainNode;
	/*============================================================================== 
	 * Raise or lower an individual vertex (Don't use for gridraising)
	 *==============================================================================*/ 
	void RaiseTerrainVertex(s32 index, f32 step);

public:
	EditableTerrain(ISceneManager* smgr, irr::io::path HeightMapFile);
	
	/*============================================================================== 
	 * Save the terrain as a hightmap file 
	 *==============================================================================*/ 
	void Save (IVideoDriver* driver);

	/*============================================================================== 
	 * Draw a grid showing which vertices will be raised/lowered
     *==============================================================================*/ 
	void DrawSelectionGrid(f32 xStart, f32 zStart, int size);

	/*==============================================================================
	 * Edit the selected grid
	 *==============================================================================*/
	void RaiseVertexGrid(s32 xMid, s32 zMid, s32 terrainDimension, s32 size, s32 step);

	void SmoothGrid(s32 xMid, s32 zMid, s32 terrainDimension, s32 size);

	/*==============================================================================
	 * Get the terrain node (For direct access for materials etc.)
	 *==============================================================================*/
	ITerrainSceneNode* GetTerrainNode();
};

#endif //_EDITABLETERRAIN_H_INCLUDED_