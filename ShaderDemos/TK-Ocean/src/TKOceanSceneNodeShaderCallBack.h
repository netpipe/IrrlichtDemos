//====================================================//
// TKOceanSceneNodeShaderCallBack.h                   //
// Project Tikal                                      //
// Ocean Demo v1.0, december 2009                     //
// by Andres J. Porfirio ... andresjesse@yahoo.com.br //
//====================================================//


#ifndef _TKOceanSceneNodeSHADERCB_
#define _TKOceanSceneNodeSHADERCB_

#include <irrlicht.h>
#include <iostream>

using namespace std;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class TKOceanSceneNodeShaderCallBack : public video::IShaderConstantSetCallBack
{
private:
     IrrlichtDevice *device;
     float atributo;
public:
        TKOceanSceneNodeShaderCallBack(IrrlichtDevice *idevice)
        {
            device=idevice;
        }

        virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
        {
                video::IVideoDriver* driver = services->getVideoDriver();


                float time2 = (float)device->getTimer()->getRealTime();
                services->setVertexShaderConstant("Time", &time2, 1);


                int layer=0;
                services->setPixelShaderConstant("A1",(int*)&layer,1);
                layer=1;
                services->setPixelShaderConstant("t0",(int*)&layer,1);
                layer=2;
                services->setPixelShaderConstant("t1",(int*)&layer,1);
                layer=3;
                services->setPixelShaderConstant("t2",(int*)&layer,1);
                layer=4;
                services->setPixelShaderConstant("t3",(int*)&layer,1);
                layer=5;
                services->setPixelShaderConstant("t4",(int*)&layer,1);
                layer=7;
                services->setPixelShaderConstant("ShadowMap",(int*)&layer,1);

                layer=2;
                services->setPixelShaderConstant("txDir",(int*)&layer,1);



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
                services->setPixelShaderConstant("NormalMap",(int*)&atributo,1);

        }
};

#endif
