#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef _TKGRASSSCENENODESHADERCB_
#define _TKGRASSSCENENODESHADERCB_

class TKGrassShaderCallBack : public video::IShaderConstantSetCallBack
{
private:
    int layer;
public:
    TKGrassShaderCallBack()
    {
    }

    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
    {
            layer=0;
            services->setPixelShaderConstant("GrassTexture",(float*)&layer,1);
    }
};

#endif
