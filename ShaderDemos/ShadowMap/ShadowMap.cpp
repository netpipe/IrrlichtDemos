#include "ShadowMap.h"

// Constructor.
CShadowMap::CShadowMap(const dimension2du& size)
{
	// Shadow texture.
	shadowTex = pVideo->addRenderTargetTexture(size,"rt",ECF_G16R16F);

	// Load depth shader.
	depth = pGPU->addShaderMaterialFromFiles("shader/depth.vs","shader/depth.fs",
											 this,EMT_SOLID,0);

	// Load shadowmap shader.
	shadow = pGPU->addShaderMaterialFromFiles("shader/shadow.vs","shader/shadow.fs",
											  this,EMT_ONETEXTURE_BLEND,1);
	// Materials.
	depthMat.MaterialType = (E_MATERIAL_TYPE)depth;
	depthMat.setFlag(EMF_BILINEAR_FILTER,false);

	shadowMat.MaterialType = (E_MATERIAL_TYPE)shadow;
	shadowMat.TextureLayer[0].Texture = shadowTex;
//	shadowMat.TextureLayer[0].TextureWrap = ETC_CLAMP_TO_BORDER;
	shadowMat.MaterialTypeParam = pack_textureBlendFunc(EBF_ZERO,EBF_DST_COLOR);

	fov = (30.0f * DEGTORAD);
}

// Destructor.
CShadowMap::~CShadowMap()
{
	pVideo->removeTexture(shadowTex);
}

// Updated the shadows.
void CShadowMap::update()
{
	farPlane = 1000.0f;
	pos = globalLight->getLightData().Position;
	tar = globalLight->getLightData().Direction;

	// Update view and project matrix.
	projMat.buildProjectionMatrixPerspectiveFovLH(fov,1.0f,1.0f,farPlane);
	viewMat.buildCameraLookAtMatrixLH(pos,tar,(pos-tar).dotProduct(vector3df(1.0f,0.0f,1.0f)) == 0.0f ?
	vector3df(0.0f,0.0f,1.0f) : vector3df(0.0f,1.0f,0.0f));

	pVideo->setTransform(ETS_PROJECTION,projMat);
	pVideo->setTransform(ETS_VIEW,viewMat);

	// Render shadow texture.
	pVideo->setRenderTarget(shadowTex,true,true,SColor(0xffffffff));

	// Depth pass.
	for(u32 i=0; i<meshArray.size(); ++i)
	{
		pVideo->setTransform(ETS_WORLD,meshArray[i]->getAbsoluteTransformation());
		pVideo->setMaterial(depthMat);

		for(u32 j=0; j<meshArray[i]->getMaterialCount(); ++j)
			pVideo->drawMeshBuffer(meshArray[i]->getMesh()->getMeshBuffer(j));
	}

	pVideo->setRenderTarget(0,true,true,SColor(0,128,128,128));

	// Updated all !!!!
	pScene->drawAll();

	// Shadow pass.
	for(u32 i=0; i<meshArray.size(); ++i)
	{
		pVideo->setTransform(ETS_WORLD,meshArray[i]->getAbsoluteTransformation());
		pVideo->setMaterial(shadowMat);

		for(u32 j=0; j<meshArray[i]->getMaterialCount(); ++j)
			pVideo->drawMeshBuffer(meshArray[i]->getMesh()->getMeshBuffer(j));
	}
}

void CShadowMap::OnSetConstants(IMaterialRendererServices* services,s32 userData)
{
	if(userData == 0)
		services->setVertexShaderConstant(reinterpret_cast<const f32*>(&farPlane),5,1);

	if(userData == 1)
	{
		matrix4 worldViewProj = projMat;
		worldViewProj *= viewMat;
		worldViewProj *= pVideo->getTransform(ETS_WORLD);
		services->setVertexShaderConstant(worldViewProj.pointer(),5,4);

		services->setVertexShaderConstant(reinterpret_cast<f32*>(&farPlane),9,1);
	}
}
