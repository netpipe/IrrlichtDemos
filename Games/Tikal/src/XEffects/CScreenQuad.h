#ifndef H_XEFFECTS_SQ
#define H_XEFFECTS_SQ

// Based on "CBaseFilter" by ItIsFree.
// Original thread: http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=9857

#include <irrlicht.h>

class CScreenQuad
{
public:
	CScreenQuad()
	{
		Material.Wireframe = false;
		Material.Lighting = false;
		Material.ZWriteEnable = false;

		Vertices[0] = irr::video::S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,1.0f);
		Vertices[1] = irr::video::S3DVertex(-1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,0.0f);
		Vertices[2] = irr::video::S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,0.0f);
		Vertices[3] = irr::video::S3DVertex( 1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,1.0f);
		Vertices[4] = irr::video::S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,1.0f);
		Vertices[5] = irr::video::S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,0.0f);
	}

	virtual void render(irr::video::IVideoDriver* driver)
	{
		irr::u16 indices[6] = {0, 1, 2, 3, 4, 5};

		driver->setMaterial(Material);
		driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
		driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
	}

	virtual irr::video::SMaterial& getMaterial()
	{
		return Material;
	}   

	irr::video::ITexture* rt[2];

private:
	irr::video::S3DVertex Vertices[6];
	irr::video::SMaterial Material;
};

#endif