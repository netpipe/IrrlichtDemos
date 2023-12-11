// Based on "CBaseFilter" by ItIsFree.
// Original thread: http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=9857

#include <irrlicht.h>

class ScreenQuadCB : public irr::video::IShaderConstantSetCallBack
{
public:
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		if(services->getVideoDriver()->getDriverType() == irr::video::EDT_OPENGL)
		{
			irr::u32 TexVar = 0;
			services->setPixelShaderConstant("ColorMapSampler", (irr::f32*)(&TexVar), 1); 

			TexVar = 1;
			services->setPixelShaderConstant("ScreenMapSampler", (irr::f32*)(&TexVar), 1); 

			TexVar = 2;
			services->setPixelShaderConstant("DepthMapSampler", (irr::f32*)(&TexVar), 1); 

			TexVar = 3;
			services->setPixelShaderConstant("UserMapSampler", (irr::f32*)(&TexVar), 1);
		}
	}
};

class CBaseFilter : public irr::scene::ISceneNode
{

	irr::core::aabbox3d<irr::f32> Box;
	irr::video::S3DVertex Vertices[6];

public:
	irr::video::SMaterial Material;
	irr::video::ITexture* rt[2];

public:

	CBaseFilter(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id)
		: irr::scene::ISceneNode(parent, mgr, id)
	{
		Material.Wireframe = false;
		Material.Lighting = false;

		Vertices[0] = irr::video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, irr::video::SColor(255,0,255,255), 0.0f, 1.0f);
		Vertices[1] = irr::video::S3DVertex(-1.0f,  1.0f, 0.0f,1,1,0, irr::video::SColor(255,0,255,255), 0.0f, 0.0f);
		Vertices[2] = irr::video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, irr::video::SColor(255,0,255,255), 1.0f, 0.0f);
		Vertices[3] = irr::video::S3DVertex( 1.0f, -1.0f, 0.0f,1,1,0, irr::video::SColor(255,0,255,255), 1.0f, 1.0f);
		Vertices[4] = irr::video::S3DVertex(-1.0f, -1.0f, 0.0f,1,1,0, irr::video::SColor(255,0,255,255), 0.0f, 1.0f);
		Vertices[5] = irr::video::S3DVertex( 1.0f,  1.0f, 0.0f,1,1,0, irr::video::SColor(255,0,255,255), 1.0f, 0.0f);

		Box.reset(Vertices[0].Pos);
		for (irr::s32 i=1; i<4; ++i)
			Box.addInternalPoint(Vertices[i].Pos);

		irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
	}


	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			irr::scene::ISceneNode::OnRegisterSceneNode();
	}


	virtual void render()
	{
		irr::u16 indices[] = {0,1,2,3,4,5};
		irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
		driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
	}


	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual irr::u32 getMaterialCount()
	{
		return 1;
	}

	virtual irr::video::SMaterial& getMaterial(irr::s32 i)
	{
		return Material;
	}   
}; 