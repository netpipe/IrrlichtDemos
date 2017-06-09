#include "shader.h"
#include <irrlicht.h>

void MyShaderCallBack::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    // set inverted world matrix
    // if we are using highlevel shaders (the user can select this when
    // starting the program), we must set the constants by name.

    float fUVScale = 1.0f;
    vector3df vGravity = vector3df(0.0, -100.0f, 0.0f);

    services->setVertexShaderConstant("fFurLength", &fFurLength, 1);
    services->setVertexShaderConstant("fMaxLength", &fMaxLength, 1);
    services->setVertexShaderConstant("fUVScale", &fUVScale, 1);
    services->setVertexShaderConstant("fLayer", &fLayer, 1);
    services->setVertexShaderConstant("vGravity", reinterpret_cast<f32*>(&vGravity), 3);

    //vLightDir
	vector3df vLightDir = vector3df(0.8f, 0.8f, 0.8f);
    services->setPixelShaderConstant("vLightDir", reinterpret_cast<f32*>(&vLightDir), 3);

    //vAmbient
    SColorf vAmbient = smgr->getAmbientLight().toSColor();
    services->setPixelShaderConstant("vAmbient", reinterpret_cast<f32*>(&vAmbient), 4);

    //Set Textures
    int texVar = 1;
	services->setPixelShaderConstant("FurTexture", (int*)(&texVar), 1);
	texVar = 0;
	services->setPixelShaderConstant("ColourTexture", (int*)(&texVar), 1);

    //mWorld
    matrix4 mWorld = driver->getTransform(video::ETS_WORLD);
    services->setVertexShaderConstant("mWorld", mWorld.pointer(), 16);

    //mWorldViewProj
    matrix4 mWorldViewProj;
    mWorldViewProj = driver->getTransform(video::ETS_PROJECTION);
    mWorldViewProj *= driver->getTransform(video::ETS_VIEW);
    mWorldViewProj *= driver->getTransform(video::ETS_WORLD);
    services->setVertexShaderConstant("mWorldViewProj", mWorldViewProj.pointer(), 16);

}

void InitShaders()
{
    ShaderCB = new MyShaderCallBack();
    blShader=true;

    if (!driver->queryFeature(video::EVDF_PIXEL_SHADER_1_3) &&
		!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1))
	{
		device->getLogger()->log("WARNING: Pixel shaders disabled "\
			"because of missing driver/hardware support.");
		blShader=false;
	}

	if (!driver->queryFeature(video::EVDF_VERTEX_SHADER_1_1) &&
		!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1))
	{
		device->getLogger()->log("WARNING: Vertex shaders disabled "\
			"because of missing driver/hardware support.");
        blShader=false;
	}

    EMT_FUR = 0;
    if(blShader)
    {
        EMT_FUR = gpu->addHighLevelShaderMaterialFromFiles(
	        "./res/fur.vert", "main", video::EVST_VS_1_1,
	        "./res/fur.frag", "main", video::EPST_PS_1_3,
	        ShaderCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF
        );
//        EMT_FUR = gpu->addHighLevelShaderMaterialFromFiles(
//	        "./res/FurShader.glsl", "fragMain", video::EVST_VS_1_1,
//	        "./res/FurShader.glsl", "vertMain", video::EPST_PS_1_3,
//	        ShaderCB, video::EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR//       );
//       );
//  EMT_SOLID = 0, EMT_SOLID_2_LAYER, EMT_LIGHTMAP, EMT_LIGHTMAP_ADD,
//  EMT_LIGHTMAP_M2, EMT_LIGHTMAP_M4, EMT_LIGHTMAP_LIGHTING, EMT_LIGHTMAP_LIGHTING_M2,
//  EMT_LIGHTMAP_LIGHTING_M4, EMT_DETAIL_MAP, EMT_SPHERE_MAP, EMT_REFLECTION_2_LAYER,
//  EMT_TRANSPARENT_ADD_COLOR, EMT_TRANSPARENT_ALPHA_CHANNEL, EMT_TRANSPARENT_ALPHA_CHANNEL_REF, EMT_TRANSPARENT_VERTEX_ALPHA,
//  EMT_TRANSPARENT_REFLECTION_2_LAYER, EMT_NORMAL_MAP_SOLID, EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR, EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA,
//  EMT_PARALLAX_MAP_SOLID, EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR, EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA, EMT_ONETEXTURE_BLEND,
//  EMT_FORCE_32BIT = 0x7fffffff


    }
    else EMT_FUR = EMT_SOLID;
   // EMT_FUR = EMT_SOLID;
}

CFurSceneNode::~CFurSceneNode()
{

}

void CFurSceneNode::OnRegisterSceneNode()
{
//    if(blShader){
//        //printf("shader\n");
	    SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_SOLID);
        SceneManager->registerNodeForRendering(this, irr::scene::ESNRP_SHADOW);
//    }
//    else{
//        SceneManager->registerNodeForRendering(nodeSelf);
//    }
}

void CFurSceneNode::render()
{
    f32 CurrentLength = 0.0f;
    f32 CurrentLayer = 0.0f;
    ShaderCB->fMaxLength = fFurLength + ( fFurLength * (1 / iLayers));
//    if(smgr->getSceneNodeRenderPass() == ESNRP_SOLID)
//    {
//        ShaderCB->fLayer = 0.0f;
//        ShaderCB->fFurLength = 0.0f;
//        nodeSelf->render();
//    }
//    else
//    {
        for(float i= 1.0f; i < iLayers; i++)
        {
            //calc value's
            CurrentLayer = i / iLayers;
            CurrentLength = fFurLength * CurrentLayer;

            //set value's
            ShaderCB->fLayer = CurrentLayer;
            ShaderCB->fFurLength = CurrentLength;

            //render child
            nodeSelf->render();
        }
//    }
}

void CFurSceneNode::setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue)
{
    nodeSelf->setMaterialFlag(flag,newvalue);
    ISceneNode::setMaterialFlag(flag,newvalue);
}


SMaterial& CFurSceneNode::getMaterial(u32 i)
{
    return nodeSelf->getMaterial(i);
}

u32 CFurSceneNode::getMaterialCount() const
{
    return nodeSelf->getMaterialCount();
}

const aabbox3d<f32>& CFurSceneNode::getBoundingBox() const
{
    return nodeSelf->getBoundingBox();
}

void CFurSceneNode::setMaterialTexture(u32 textureLayer, video::ITexture* texture)
{
    nodeSelf->setMaterialTexture(textureLayer,texture);
}

void CFurSceneNode::setAnimationSpeed(u32 speed)
{
    nodeSelf->setAnimationSpeed(speed);
}

void CFurSceneNode::setFrameLoop(u32 first, u32 last)
{
    nodeSelf->setFrameLoop(first, last);
}

f32 CFurSceneNode::getFrameNr()
{
    return nodeSelf->getFrameNr();
}

void CFurSceneNode::setLayers(u32 Layer)
{
    if(Layer)
        iLayers = Layer;
}

void CFurSceneNode::setFurLength(f32 Length)
{
    fFurLength = Length;
}

//Custom Functions
void CFurSceneNode::Init()
{
    //create childnode
    nodeSelf = device->getSceneManager()->addAnimatedMeshSceneNode(meshSelf, this);
    getMaterial(0).MaterialTypeParam = pack_textureBlendFunc(EBF_SRC_ALPHA, EBF_ONE_MINUS_SRC_ALPHA, EMFN_MODULATE_2X );
    fFurLength = 0.0f;
    iLayers = 1;
}

IAnimatedMeshSceneNode* CFurSceneNode::getNode(){return nodeSelf;}

