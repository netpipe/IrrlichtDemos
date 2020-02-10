
#ifndef __Shader_h
#define __Shader_h

#include <irrlicht.h>

#define SHADER_BLUR_HORIZONTAL  0
#define SHADER_BLUR_VERTICAL    1

using namespace irr;

// Declare new materials externally for use by scene nodes
extern s32 SHADER_MATERIAL_BASE;
extern s32 SHADER_MATERIAL_STANDARD;
extern s32 SHADER_MATERIAL_BLOOM;
extern s32 SHADER_MATERIAL_ANOTHER_EXAMPLE;

class Shader : public video::IMaterialRenderer, public video::IShaderConstantSetCallBack
{
public:
    Shader(IrrlichtDevice *);
    
    // if shaders were successfully created
    bool areAvailable();
    
    // some shaders called in two passes
    // Note this requires that a single shader contains functionality
    // for both passes (and the desired pass is passed as a uniform)
    // There is no support for multi passes with different shaders.
    void setPass(int);
    
    void OnSetConstants(video::IMaterialRendererServices *, s32);
    
    void OnSetMaterial(const video::SMaterial &, const video::SMaterial &, bool, video::IMaterialRendererServices *);
    bool OnRender(video::IMaterialRendererServices *, video::E_VERTEX_TYPE);
    void OnUnsetMaterial();
    bool isTransparent() const;
    s32 getRenderCapability() const;
    
private:
    video::IVideoDriver *driver;
    video::IMaterialRenderer *baseMaterialRenderer;
    const video::SMaterial *currentMaterial;
    bool shadersAvailable;
    int pass;
};

#endif
