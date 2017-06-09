/*
original code by Copland (original source) : http://forum.irrlicht.fr/viewtopic.php?id=336
modified version of _CTerrain by fredakilla@gmail.com

Modified version :
- change map loader for support 2^N+1 heightmap size
- add internally VBO support
- add VBO destuction (for paging)
- add colormap destruction (for paging)
- improve render method and add render debug data
- change bounding box calculation and remove it by default (not necessary for paging)
- add clamping on color textures (don't work with opengl - irrlicht bug ?)
- add possibility to change material type or flag externally
- remove normals recalculation (improve loading time for pager)
- remove splatting shader data and functions => changed by setShader to accept generic shader
- generals and minors code organisations
*/

#ifndef _CTerrain
#define _CTerrain

#include <irrlicht.h>

using namespace irr;


//----------------------------------------------------------------------------------------
// Tiled Terrain Class
//----------------------------------------------------------------------------------------
class CTerrain : public scene::ISceneNode
{
public:

	 enum E_TERRAIN_QUALITY
	{
		ETQ_HIGH = 1,
		ETQ_MEDIUM = 2,
		ETQ_LOW = 4,
		ETQ_EXTRALOW = 8
	};

	CTerrain(c8* HeightmapFile,E_TERRAIN_QUALITY Quality,f32 ScaleTexture,
	          ISceneNode* parent,
	          scene::ISceneManager* smgr,s32 id);//:ISceneNode(parent,smgr,id);
	virtual ~CTerrain();
	void setColorTexture(c8* FileName);
	void setDetailTexture(c8* FileName);
	void setRenderDistance(f32 Distance);
	scene::IMesh* getMesh();
	f32 getHeight(f32 x,f32 z);
	virtual void setMaterialType(const video::E_MATERIAL_TYPE Mat);
	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual video::SMaterial& getMaterial(u32 i);
	virtual void setPosition(const core::vector3df &Pos);
	virtual void setScale(const core::vector3df &Scale);
	u32 getMaterialCount();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual void recalculateBoundingBox();
	virtual void setMaterialFlag(const video::E_MATERIAL_FLAG Mat, bool Flag);
	void setTexture(u32 NumTex,c8* FileName);
	void setShader(c8 * shaderFileName, scene::ISceneManager* smgr);


private:

	core::aabbox3d<f32> BoundingBox;
	video::SMaterial Material;
	scene::SMeshBufferLightMap** CTTileBuffer;
	void calculateNormals ( scene::SMeshBufferLightMap* pMeshBuffer,s32 Size);

	f32 RenderDistance;
	scene::SMesh* TerrainMesh;
	u16 NbsTiles;
};



//----------------------------------------------------------------------------------------
// shader callback
//----------------------------------------------------------------------------------------
class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:
    scene::ISceneManager* smgr;

    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
    {
         video::IVideoDriver* driver = services->getVideoDriver();

		// World * View * Projection transformation
		core::matrix4 worldViewProj;
        worldViewProj  = driver->getTransform(video::ETS_PROJECTION);
        worldViewProj *= driver->getTransform(video::ETS_VIEW);
        worldViewProj *= driver->getTransform(video::ETS_WORLD);
        services->setVertexShaderConstant("mWorldViewProj",worldViewProj.pointer(),16);

    }
};

#endif
