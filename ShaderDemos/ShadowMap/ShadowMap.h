#include "main.h"

#ifndef __CSHADOWMAP__
#define __CSHADOWMAP__

class CShadowMap : public IShaderConstantSetCallBack
{
public:

	// Constructor/destructor.
	CShadowMap(const dimension2du& size);
	~CShadowMap();

	// Updated the shadows.
	void update();

	virtual void OnSetConstants(IMaterialRendererServices* services,s32 userData);

	// Shadow texture.
	ITexture* shadowTex;

	// Light/mesh array.
	ILightSceneNode* globalLight;
	array<IAnimatedMeshSceneNode*> meshArray;

	f32 fov;
	f32 farPlane;

private:
	
	// Shaders.
	s32 depth;
	s32 shadow;

	// Material.
	SMaterial depthMat;
	SMaterial shadowMat;

	matrix4 viewMat;
	matrix4 projMat;
	vector3df pos;
	vector3df tar;

};

#endif