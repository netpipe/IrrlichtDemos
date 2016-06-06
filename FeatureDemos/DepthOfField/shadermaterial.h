#ifndef SHADERMATERIAL_H
#define SHADERMATERIAL_H
#include <irrlicht.h>
using namespace irr;

class ShaderMaterial
{
    public:
        s32 material;
        video::ITexture* texture;

        ShaderMaterial(IrrlichtDevice* device, c8* vsFileName, c8* psFileName, s32 userData,
            video::IShaderConstantSetCallBack* callback=0, int width=512, int height=512);
        virtual ~ShaderMaterial();
        void init(c8* vsFileName, c8* psFileName, video::IShaderConstantSetCallBack* callback, s32 userData);
    protected:
    private:
        IrrlichtDevice* device;

};

#endif // SHADERMATERIAL_H
