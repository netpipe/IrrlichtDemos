#include "main.h"
#include "Grid.h"

#ifndef __CBLOOM__
#define __CBLOOM__

class CBloom : public IShaderConstantSetCallBack
{
public:

	// Constructor / Destructor.
	CBloom();
	~CBloom();

	void render(CGrid* grid);
	virtual void OnSetConstants(IMaterialRendererServices* services,s32 userData);

	f32 blurValue;
	f32 brightnessValue;
	f32 multiValue;

	ITexture* tex0;
	ITexture* tex1;
	ITexture* tex2;
	ITexture* tex3;

private:

	S3DVertex Vertices[6];

	SMaterial material0;
	SMaterial material1;
	SMaterial material2;
	SMaterial material3;

	int filter;
	int vblur;
	int hblur;
	int combine;

};

#endif