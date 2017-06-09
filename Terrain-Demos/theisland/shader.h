/**
 * Shader callback for the island skin
**/

#ifndef _groundShader
#define _groundShader

#include "include.h"

class CGroundShader : public irr::video::IShaderConstantSetCallBack
{
public:
	CGroundShader(irr::core::vector3df lightPos,bool UseHighLevelShaders = true)
	{
		this->lightPos = lightPos;	
		this->UseHighLevelShaders = UseHighLevelShaders;
	}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services,
			irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		// set clip matrix

		irr::core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		else
			services->setVertexShaderConstant(worldViewProj.pointer(), 4, 4);		

		irr::core::matrix4 world;
		world = driver->getTransform(irr::video::ETS_WORLD);

		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mWorld", world.pointer(), 16);
		else
			services->setVertexShaderConstant(world.pointer(), 4, 4);

		// set inverted world matrix
		// if we are using highlevel shaders (the user can select this when
		// starting the program), we must set the constants by name.

		irr::core::vector3df lightdir = irr::core::vector3df(0,1,1);
		if (UseHighLevelShaders)
			services->setVertexShaderConstant("mLightDir", reinterpret_cast<irr::f32*>(&lightdir), 3);
		else
			services->setVertexShaderConstant(reinterpret_cast<irr::f32*>(&lightPos), 8, 1);

		if (driver->getDriverType() == irr::video::EDT_OPENGL)
		{
			//set Textures for openGL Shaders 
			irr::s32 texture1 = 0; 
			services->setPixelShaderConstant("splat1",reinterpret_cast<irr::f32*> (&texture1), 1); 
			irr::s32 texture2 = 1; 
			services->setPixelShaderConstant("splat2",reinterpret_cast<irr::f32*> (&texture2), 1); 
		}
	}
private:
	irr::core::vector3df lightPos;
	bool UseHighLevelShaders;
};

#endif