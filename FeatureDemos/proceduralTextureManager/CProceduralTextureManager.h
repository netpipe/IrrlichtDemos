// Copyright (C) 2006 Cestmir "CZestmyr" Houska
// This file uses the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#ifndef _PROCEDURAL_TEXTURE_MANAGER_BY_CZESTMYR_5761_
#define _PROCEDURAL_TEXTURE_MANAGER_BY_CZESTMYR_5761_

#include <irrlicht.h>

namespace irr{
namespace scene{

#define _MX_PROC_TEXTURES_ 8

class ProcTexture{
public:

	ProcTexture(ITimer* Timer, video::IVideoDriver* Driver, s32 TexX, s32 TexY, u32 Step);

	virtual ~ProcTexture();

	u8 avg(u8& a, u8& b, u8& c);

	virtual void launchProcedure() = 0;

	void animate();

	friend class CProceduralTextureManager;
protected:
	ITimer* timer;
        video::IVideoDriver* driver;
        video::ITexture* texture;
        s32 texX;
        s32 texY;
	s32 texY_1;
	s32 texX_1;
        u32 prevTime;
        u32 step;
        u32 timer1;
};

class TestProcTexture : public ProcTexture{
public:

	TestProcTexture(ITimer* Timer, video::IVideoDriver* Driver, s32 TexX, s32 TexY, u32 Step);

	void launchProcedure();

};


class FireProcTexture : public ProcTexture{
public:

	FireProcTexture(ITimer* Timer, video::IVideoDriver* Driver, s32 TexX, s32 TexY, u32 Step, u8 fadecol, f32 fr, c8* coolingTextureFilename);

	void launchProcedure();

protected:
	video::ITexture* coolingTexture;
	core::dimension2d<s32> coolTexDimension;
	s32 offsetY;
	f32 faderate;
	f32 faderate_2;
	u8 fadecolor;
};

class CProceduralTextureManager : public scene::ISceneNode {
public:

	CProceduralTextureManager(ISceneNode* parent, ISceneManager* Smgr, ITimer* Timer, s32 Id = -1);

	~CProceduralTextureManager();

	const core::aabbox3d<f32>& getBoundingBox() const;

	void render();

	//void OnPreRender();
	void OnRegisterSceneNode();

	video::ITexture* getTexture(u8 num);

	bool deleteTexture(u8 num);
	
	u8 addTestTexture(s32 x = 64, s32 y = 64, u32 step = 50);

	u8 addFireTexture(c8* coolingTextureFilename, s32 x = 64, s32 y = 64, f32 fadeRate = 0.98f, u8 fadeColor = 0, u32 step = 50);

protected:
	ProcTexture** textures;
	ITimer* timer;
	core::aabbox3d<f32> bbox;
};
	
}
}

#endif
