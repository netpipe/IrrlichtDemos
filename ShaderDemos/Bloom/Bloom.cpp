#include "Bloom.h"

/* CONSTRUCTOR */
CBloom::CBloom()
{
	// Create a quad in front of the camera
	Vertices[0] = S3DVertex(-100.0,-100.0,0.0,1,1,0,SColor(0,255,255,255),0.0,1.0);
	Vertices[1] = S3DVertex(-100.0, 100.0,0.0,1,1,0,SColor(0,255,255,255),0.0,0.0);
	Vertices[2] = S3DVertex( 100.0, 100.0,0.0,1,1,0,SColor(0,255,255,255),1.0,0.0);
	Vertices[3] = S3DVertex( 100.0,-100.0,0.0,1,1,0,SColor(0,255,255,255),1.0,1.0);
	Vertices[4] = S3DVertex(-100.0,-100.0,0.0,1,1,0,SColor(0,255,255,255),0.0,1.0);
	Vertices[5] = S3DVertex( 100.0, 100.0,0.0,1,1,0,SColor(0,255,255,255),1.0,0.0);

	// Textures for rendering.
	tex0 = pVideo->addRenderTargetTexture(dimension2d<u32>(1024,1024));
	tex1 = pVideo->addRenderTargetTexture(dimension2d<u32>(512,512));
	tex2 = pVideo->addRenderTargetTexture(dimension2d<u32>(256,256));
	tex3 = pVideo->addRenderTargetTexture(dimension2d<u32>(256,256));

	// Load the shaders.
	filter = pGPU->addShaderMaterialFromFiles("shader/quad.vs",
											  "shader/filter.fs",
											  this,EMT_SOLID,0);

	vblur = pGPU->addShaderMaterialFromFiles("shader/quad.vs",
											 "shader/vblur.fs",
											 this,EMT_SOLID,1);

	hblur = pGPU->addShaderMaterialFromFiles("shader/quad.vs",
											 "shader/hblur.fs",
											 this,EMT_SOLID,1);

	combine = pGPU->addShaderMaterialFromFiles("shader/quad.vs",
											   "shader/combine.fs",
											   this,EMT_SOLID,2);

	// Create materials.
	material0.setTexture(0,tex0);
	material0.setFlag(EMF_TEXTURE_WRAP,true);
//	material0.TextureLayer[0].TextureWrap = ETC_CLAMP_TO_EDGE;
	material0.MaterialType = (E_MATERIAL_TYPE)filter;

	material1.setTexture(0,tex1);
	material1.setFlag(EMF_TEXTURE_WRAP,true);
//	material1.TextureLayer[0].TextureWrap = ETC_CLAMP_TO_EDGE;
	material1.MaterialType = (E_MATERIAL_TYPE)vblur;

	material2.setTexture(0,tex2);
	material2.setFlag(EMF_TEXTURE_WRAP,true);
//	material2.TextureLayer[0].TextureWrap = ETC_CLAMP_TO_EDGE;
	material2.MaterialType = (E_MATERIAL_TYPE)hblur;

	material3.setTexture(1,tex3);
	material3.setTexture(0,tex0);
	material3.setFlag(EMF_TEXTURE_WRAP,true);
	material3.TextureLayer[0].BilinearFilter = false;
	material3.TextureLayer[0].TrilinearFilter = true;
//	material3.TextureLayer[0].TextureWrap = ETC_CLAMP_TO_EDGE;
//	material3.TextureLayer[1].TextureWrap = ETC_CLAMP_TO_EDGE;
	material3.MaterialType = (E_MATERIAL_TYPE)combine;

	blurValue = 0.005f;
	brightnessValue = 0.25f;
	multiValue = 0.45f;
}

/* DESTRUCTOR */
CBloom::~CBloom()
{
	// Remove the texture used.
	pVideo->removeTexture(tex0);
	pVideo->removeTexture(tex1);
	pVideo->removeTexture(tex2);
	pVideo->removeTexture(tex3);
}

/* UPDATE */
void CBloom::render(CGrid* grid)
{
	u16 indices[] = {0,1,2,3,4,5};

	pVideo->setRenderTarget(tex0,true,true,SColor(0,128,128,128));

	grid->render();
	pScene->drawAll();

	// Brightness filter.
	pVideo->setRenderTarget(tex1);
	pVideo->setMaterial(material0);
	pVideo->drawIndexedTriangleList(&Vertices[0],6,&indices[0],2);

	// Vertical blurring.
	pVideo->setRenderTarget(tex2);
	pVideo->setMaterial(material1);
	pVideo->drawIndexedTriangleList(&Vertices[0],6,&indices[0],2);

	// Horizontal blurring.
	pVideo->setRenderTarget(tex3);
	pVideo->setMaterial(material2);
	pVideo->drawIndexedTriangleList(&Vertices[0],6,&indices[0],2);

	pVideo->setRenderTarget(0);

	// Combiner (combines all the textures).
	pVideo->setMaterial(material3);
	pVideo->drawIndexedTriangleList(&Vertices[0],6,&indices[0],2);
}

void CBloom::OnSetConstants(IMaterialRendererServices* services,s32 userData)
{
	if(userData == 0)
		services->setPixelShaderConstant(reinterpret_cast<const f32*>(&brightnessValue),0,1);

	if(userData == 1)
		services->setPixelShaderConstant(reinterpret_cast<const f32*>(&blurValue),0,1);

	if(userData == 2)
		services->setPixelShaderConstant(reinterpret_cast<const f32*>(&multiValue),0,1);
}
