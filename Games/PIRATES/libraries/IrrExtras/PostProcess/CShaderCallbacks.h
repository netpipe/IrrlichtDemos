#ifndef __C_SHADER_CALLBACKS_H_INCLUDED__
#define __C_SHADER_CALLBACKS_H_INCLUDED__
class CTexturesShaderCallback : public irr::video::IShaderConstantSetCallBack {
public:
	virtual void OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 num ) {
		irr::video::IVideoDriver* driver = services->getVideoDriver( );
		irr::core::matrix4 worldViewProj = driver->getTransform( irr::video::ETS_PROJECTION )
										* driver->getTransform( irr::video::ETS_VIEW )
										* driver->getTransform( irr::video::ETS_WORLD );
		services->setVertexShaderConstant("worldViewProj", worldViewProj.pointer( ), 16 );
		
		if( num == 0 )
			return;
		irr::s32 t1 = 0, t2 = 1, t3 = 2, t4 = 3;
		services->setPixelShaderConstant( "texture1", (irr::f32*) &t1, 1 );
		if( num == 1 )
			return;
		services->setPixelShaderConstant( "texture2", (irr::f32*) &t2, 1 );
		if( num == 2 )
			return;
		services->setPixelShaderConstant( "texture3", (irr::f32*) &t3, 1 );
		if( num == 3 )
			return;
		services->setPixelShaderConstant( "texture4", (irr::f32*) &t4, 1 );
	};
};
class CShaderCallback : public irr::video::IShaderConstantSetCallBack {
private:
	irr::f32 randNum;
	irr::f32 params[8];
	irr::f32 time;
	
public:
	bool usesrand;
	bool usestime;
	irr::u32 usesnum;
	CShaderCallback( ) {
		usesrand = false;
		usestime = false;
		usesnum = 0u;
		randNum = 0.0f;
		for( irr::u8 i = 0u; i != 8u; ++ i )
			params[ i ] = 0.0f;
		time = 0.0f;
	}
	virtual void OnSetConstants( irr::video::IMaterialRendererServices* services, irr::s32 num ) {
		if( usesrand ) {
			randNum = ((irr::f32) rand( )) / (((irr::f32) RAND_MAX) + 1.0f);
			services->setPixelShaderConstant( "randNum", &randNum, 1 );
		}
		if( usestime )		services->setPixelShaderConstant( "time", &time, 1 );
		if( usesnum > 0u )	services->setPixelShaderConstant( "v0", &params[ 0u ], 1 );
		if( usesnum > 1u )	services->setPixelShaderConstant( "v1", &params[ 1u ], 1 );
		if( usesnum > 2u )	services->setPixelShaderConstant( "v2", &params[ 2u ], 1 );
		if( usesnum > 3u )	services->setPixelShaderConstant( "v3", &params[ 3u ], 1 );
		if( usesnum > 4u )	services->setPixelShaderConstant( "v4", &params[ 4u ], 1 );
		if( usesnum > 5u )	services->setPixelShaderConstant( "v5", &params[ 5u ], 1 );
		if( usesnum > 6u )	services->setPixelShaderConstant( "v6", &params[ 6u ], 1 );
		if( usesnum > 7u )	services->setPixelShaderConstant( "v7", &params[ 7u ], 1 ); 
		
		if( num == 0 )
			return;
		irr::s32 t1 = 0, t2 = 1, t3 = 2, t4 = 3;
		services->setPixelShaderConstant( "texture1", (irr::f32*) &t1, 1 );
		if( num == 1 )
			return;
		services->setPixelShaderConstant( "texture2", (irr::f32*) &t2, 1 );
		if( num == 2 )
			return;
		services->setPixelShaderConstant( "texture3", (irr::f32*) &t3, 1 );
		if( num == 3 )
			return;
		services->setPixelShaderConstant( "texture4", (irr::f32*) &t4, 1 );
	}
	void setParams( irr::f32 p1, irr::f32 p2, irr::f32 p3, irr::f32 p4, irr::f32 p5, irr::f32 p6, irr::f32 p7, irr::f32 p8 ) {
		params[ 0u ] = p1;
		params[ 1u ] = p2;
		params[ 2u ] = p3;
		params[ 3u ] = p4;
		params[ 4u ] = p5;
		params[ 5u ] = p6;
		params[ 6u ] = p7;
		params[ 7u ] = p8;
	}
	void setParam( irr::u8 i, irr::f32 to ) {
		if( i >= 8u )
			return;
		params[ i ] = to;
	}
	irr::f32 getParam( irr::u8 i ) {
		if( i >= 8u )
			return 0.0f;
		return params[ i ];
	}
	void setTime( irr::f32 to ) { time = to; }
	irr::f32 getTime( ) { return time; }
};
#endif