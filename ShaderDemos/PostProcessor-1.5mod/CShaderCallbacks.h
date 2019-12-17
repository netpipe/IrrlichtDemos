#ifndef __C_SHADER_CALLBACKS_H_INCLUDED__
#define __C_SHADER_CALLBACKS_H_INCLUDED__

class CTexturesShaderCallback : public IShaderConstantSetCallBack {
public:
	virtual void OnSetConstants( IMaterialRendererServices *services, s32 num ) {
		if( num == 0 )
			return;
		int t1 = 0, t2 = 1, t3 = 2, t4 = 3;
		services->setPixelShaderConstant( "texture1", (float*) &t1, 1 );
		if( num == 1 )
			return;
		services->setPixelShaderConstant( "texture2", (float*) &t2, 1 );
		if( num == 2 )
			return;
		services->setPixelShaderConstant( "texture3", (float*) &t3, 1 );
		if( num == 3 )
			return;
		services->setPixelShaderConstant( "texture4", (float*) &t4, 1 );
	};
};

class CShaderCallback : public IShaderConstantSetCallBack {
	f32* shaderparameters;
	f32 randNum;
	
public:
	virtual void OnSetConstants( IMaterialRendererServices *services, s32 num ) {
		randNum = ((f32) rand( )) / ((f32) RAND_MAX);
		services->setPixelShaderConstant( "randNum", &randNum, 1 );
		services->setPixelShaderConstant( "vecValue0", &shaderparameters[0], 1 );
		services->setPixelShaderConstant( "vecValue1", &shaderparameters[1], 1 );
		services->setPixelShaderConstant( "vecValue2", &shaderparameters[2], 1 );
		services->setPixelShaderConstant( "vecValue3", &shaderparameters[3], 1 );
		services->setPixelShaderConstant( "vecValue4", &shaderparameters[4], 1 );
		services->setPixelShaderConstant( "vecValue5", &shaderparameters[5], 1 );
		services->setPixelShaderConstant( "vecValue6", &shaderparameters[6], 1 );
		services->setPixelShaderConstant( "vecValue7", &shaderparameters[7], 1 ); 
		
		if( num == 0 )
			return;
		int t1 = 0, t2 = 1, t3 = 2, t4 = 3;
		services->setPixelShaderConstant( "texture1", (float*) &t1, 1 );
		if( num == 1 )
			return;
		services->setPixelShaderConstant( "texture2", (float*) &t2, 1 );
		if( num == 2 )
			return;
		services->setPixelShaderConstant( "texture3", (float*) &t3, 1 );
		if( num == 3 )
			return;
		services->setPixelShaderConstant( "texture4", (float*) &t4, 1 );
	}
	void setShaderParameters( f32 *paras ) {
		shaderparameters = paras;
	}
	f32 getParam( u8 i ) {
		if( i >= 8u )
			return 0.0f;
		return shaderparameters[ i ];
	}
};

#endif