
#include "Shader.h"

// Shader material definitions

s32 SHADER_MATERIAL_BASE = video::EMT_SOLID;
s32 SHADER_MATERIAL_STANDARD = video::EMT_SOLID;
s32 SHADER_MATERIAL_BLOOM = video::EMT_SOLID;
s32 SHADER_MATERIAL_ANOTHER_EXAMPLE = video::EMT_SOLID;

Shader::Shader(IrrlichtDevice *device)
{
    driver = device->getVideoDriver();
    currentMaterial = NULL;
    
    // Initialise shaders
    
    pass = 0;
    
	shadersAvailable =
        driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1)
		&& driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1);
	
	//shadersAvailable = false;
	
	if (shadersAvailable)
	{
        video::IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();
        
        SHADER_MATERIAL_BASE = gpu->addHighLevelShaderMaterialFromFiles(
			"media/shaders/standard.vert", "main", video::EVST_VS_1_1,
			"media/shaders/standard.frag", "main", video::EPST_PS_1_1,
			this, video::EMT_SOLID, 0);
		
        SHADER_MATERIAL_STANDARD = driver->addMaterialRenderer(this, "SHADER_MATERIAL_STANDARD");
        
        // Although baseMaterialRenderer is always assumed, we can have another
        // shader here as it doesn't rely on a material renderer.
        // What we couldn't do is have a shader with this base...
        
        // ACTUALLY I DON'T THINK ABOVE PARAGRAPH IS CORRECT! NEED TO THINK THIS CODE OUT SOME MORE.
        // (although it *seems* to work, it may be that bloom is indeed using the standard base,
        // which would not be desirable!)
        
        SHADER_MATERIAL_BLOOM = gpu->addHighLevelShaderMaterialFromFiles(
			"media/shaders/bloom.vert", "main", video::EVST_VS_1_1,
			"media/shaders/bloom.frag", "main", video::EPST_PS_1_1,
			this, video::EMT_TRANSPARENT_ADD_COLOR , 0);
		
		SHADER_MATERIAL_ANOTHER_EXAMPLE = driver->addMaterialRenderer(this, "SHADER MATERIAL EXAMPLE");
	}
	else
	{
        printf("Warning: Shaders disabled because of missing driver/hardware support.\n");
    }
    
    baseMaterialRenderer = driver->getMaterialRenderer(SHADER_MATERIAL_BASE);
}

bool Shader::areAvailable()
{
    return shadersAvailable;
}

void Shader::setPass(int p)
{
    pass = p;
}

void Shader::OnSetConstants(video::IMaterialRendererServices *services, s32 userData)
{
    if (!currentMaterial)
    {
        printf("Shader::OnSetConstants: Error: no currentMaterial.\n");
        return;
    }
    
    // Now send different parameters to shader based on state of currentMaterial.
    
    if (currentMaterial->MaterialType == SHADER_MATERIAL_STANDARD)
    {
        // Use Texture?
        f32 useTexture0 = currentMaterial->TextureLayer[0].Texture ? 1.0 : 0.0;
        services->setPixelShaderConstant("UseTexture0", &useTexture0, 1);
        
        // Set Texture 0
        int tex0 = 0;
        services->setPixelShaderConstant("texture0", (f32 *)(&tex0), 1);
        
        // Colour saturation... Increase for nice warm and sunny look
        float sat = 1.6;
        services->setVertexShaderConstant("Saturation", &sat, 1);
    
        // Hemisphere Lighting
    
        f32 sunPos[3] = {0.0,100.0,0.0};
        f32 skyCol[3] = {0xee/256.0, 0xee/256.0, 0x88/256.0};
        f32 groundCol[3] = {0x55/256.0, 0x55/256.0, 0x77/256.0};
        services->setVertexShaderConstant("SunPos", sunPos, 3);
        services->setVertexShaderConstant("SkyCol", skyCol, 3);
        services->setVertexShaderConstant("GroundCol", groundCol, 3);
    }
    else if (currentMaterial->MaterialType == SHADER_MATERIAL_BLOOM)
    {
        // Set Texture 0
        int tex0 = 0;
        services->setPixelShaderConstant("texture0", (f32 *)(&tex0), 1);
        
        // Blurs horizontally on 1st pass, vertically on 2nd.
        services->setPixelShaderConstant("pass", (f32 *)(&pass), 1);
    }
    else if (currentMaterial->MaterialType == SHADER_MATERIAL_ANOTHER_EXAMPLE)
    {
        printf("Shader: Example material used!! :O\n");
    }
    else
    {
        printf("Shader::OnSetConstants: Error: Undefined material!\n");
    }
}

void Shader::OnSetMaterial(const video::SMaterial &material, const video::SMaterial &lastMaterial,
    bool resetAllRenderstates, video::IMaterialRendererServices *services)
{
    // save current material for use by shader
    currentMaterial = &material;
    baseMaterialRenderer->OnSetMaterial(material, lastMaterial, resetAllRenderstates, services);
}

bool Shader::OnRender(video::IMaterialRendererServices *services, video::E_VERTEX_TYPE vtxtype)
{
    return baseMaterialRenderer->OnRender(services, vtxtype);
}

void Shader::OnUnsetMaterial()
{
    baseMaterialRenderer->OnUnsetMaterial();
}

bool Shader::isTransparent() const
{
    return baseMaterialRenderer->isTransparent();
}

s32 Shader::getRenderCapability() const
{
    return baseMaterialRenderer->getRenderCapability();
}



