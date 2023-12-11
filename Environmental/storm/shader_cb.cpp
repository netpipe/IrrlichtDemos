#include "shader_cb.h"


using namespace irr;

void ShaderCallBack::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
	//video::IVideoDriver* driver = services->getVideoDriver();

	//services->setPixelShaderConstant("tex",(float*)&layer,1);
	int tex = 0;
	//services->setPixelShaderConstant("tex", (float*)(&tex), 1); 
	services->setPixelShaderConstant("tex", &tex, 1); 
	
	services->setPixelShaderConstant("time", &_time, 1);
	
	//float alpha = 0.15f;
	services->setPixelShaderConstant("alpha", &_alpha, 1);
	/***
	core::matrix4 worldViewProj = driver->getTransform(video::ETS_PROJECTION);
	worldViewProj *= driver->getTransform(video::ETS_VIEW);
	worldViewProj *= driver->getTransform(video::ETS_WORLD);

	services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
	services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

	f32 FarValue = 2000.0f;
	services->setPixelShaderConstant("MaxD", &FarValue, 1);


	f32 time=device->getTimer()->getTime()/1000.0f;
	services->setVertexShaderConstant("waterTime",&time,1);

	atributo=0;
	services->setPixelShaderConstant("NormalMap",(float*)&atributo,1);

	int ids[] = {0, 1, 2}; // sampler2d IDs
	services->setPixelShaderConstant("", (float*)&ids[0], 1);
	services->setPixelShaderConstant("", (float*)&ids[1], 1);
	services->setPixelShaderConstant("", (float*)&ids[2], 1);

	***/
}



