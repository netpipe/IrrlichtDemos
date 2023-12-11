#ifndef CWATER_H_HG_INCLUDE
#define CWATER_H_HG_INCLUDE

#include <irrlicht.h>
#include "CWaterMesh.h"

class CWater : public irr::video::IShaderConstantSetCallBack
{
public:
	CWater(	irr::IrrlichtDevice* irrlichtDevice, 
			const irr::core::dimension2d<irr::f32>& tileSize,
			const irr::core::dimension2d<irr::u32>& tileCount);
	
	~CWater();

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

	void update();

private:
	irr::IrrlichtDevice* device;
	irr::scene::ISceneManager* smgr;
	irr::video::IVideoDriver* driver;
	
	CWaterMesh mesh;
	irr::scene::ISceneNode* parentNode;
	
	irr::s32 waterMat;
};

#endif