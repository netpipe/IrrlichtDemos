#include "CMultiTexturingManager.h"

CMultiTexturingManager::CMultiTexturingManager(scene::ISceneManager* psmgr){
    //Assign all parameters
    smgr = psmgr;
    driver=psmgr->getVideoDriver();

    ///GLSL Shader Code
    //Vertex Shader
    const c8 *vertShader = "void main()\
                            {\
                                gl_TexCoord[0] = gl_MultiTexCoord0;\
                                gl_TexCoord[1] = gl_MultiTexCoord1;\
                                gl_Position = ftransform();\
                            }\
                            ";

    //Fragment Shader
    const c8 *fragShader = "uniform sampler2D splatMap;\
                            uniform sampler2D layer_red;\
                            uniform sampler2D layer_green;\
                            uniform sampler2D layer_blue;\
                            \
                            void main(){\
                                vec4 SplatCol=texture2D(splatMap,gl_TexCoord[0].xy);\
                                vec4 RedCol=texture2D(layer_red,gl_TexCoord[1].xy);\
                                vec4 GreenCol=texture2D(layer_green,gl_TexCoord[1].xy);\
                                vec4 BlueCol=texture2D(layer_blue,gl_TexCoord[1].xy);\
                                gl_FragColor=(vec4(SplatCol.r*RedCol+SplatCol.g*GreenCol+SplatCol.b*BlueCol))*vec4(1,1,1,SplatCol.a);\
                            }\
                            ";

    //Create the shader material
    shaderMaterial = driver->getGPUProgrammingServices()->addHighLevelShaderMaterial(
            vertShader, "main", video::EVST_VS_1_1,
            fragShader, "main", video::EPST_PS_1_1,
            this, video::EMT_TRANSPARENT_ALPHA_CHANNEL);
}

CMultiTexturingManager::~CMultiTexturingManager(){
    //Erase everything
    for(u32 i=0; i<array_Nodes.size();i++){
        while(!array_Nodes[i].array_Passes.empty()){
            array_Nodes[i].array_Passes.erase(0);
        }
    }
    while(!array_Nodes.empty()){
        array_Nodes.erase(0);
    }
}

bool CMultiTexturingManager::addNode(scene::ISceneNode *node){
    for(u32 i=0; i<array_Nodes.size();i++){
	    if(array_Nodes[i].Node == node){
	        return false;
	    }
    }
    SMultiTextureNode tmpNode;
    tmpNode.Node = node;
    array_Nodes.push_back(tmpNode);
    return true;
}

bool CMultiTexturingManager::removeNode(scene::ISceneNode *node){
    for(u32 i=0; i<array_Nodes.size();i++){
	    if(array_Nodes[i].Node == node){
	        array_Nodes.erase(i);
	        return true;
	    }
    }
    return false;
}

CMultiTexturingManager::STexturePass *CMultiTexturingManager::addPass(scene::ISceneNode *node){
    for(u32 i=0; i<array_Nodes.size();i++){
        if(array_Nodes[i].Node == node){
            STexturePass *pass = new STexturePass();
            pass->red_texture = node->getMaterial(0).getTexture(0);
            pass->green_texture = node->getMaterial(0).getTexture(1);
            pass->blue_texture = node->getMaterial(0).getTexture(2);
            pass->splat_texture = node->getMaterial(0).getTexture(3);

            array_Nodes[i].array_Passes.push_back(pass);
            return pass;
        }
    }
    return 0;
}

CMultiTexturingManager::STexturePass *CMultiTexturingManager::addPass(scene::ISceneNode *node, video::ITexture *splat, video::ITexture *red, video::ITexture *green, video::ITexture *blue){
    for(u32 i=0; i<array_Nodes.size();i++){
        if(array_Nodes[i].Node == node){
            STexturePass *pass = new STexturePass();
            pass->red_texture = red;
            pass->green_texture = green;
            pass->blue_texture = blue;
            pass->splat_texture = splat;

            array_Nodes[i].array_Passes.push_back(pass);
            return pass;
        }
    }
    return 0;
}

CMultiTexturingManager::STexturePass *CMultiTexturingManager::addPass(scene::ISceneNode *node, STexturePass *pass){
    for(u32 i=0; i<array_Nodes.size();i++){
        if(array_Nodes[i].Node == node){
            STexturePass *Pass = pass;

            array_Nodes[i].array_Passes.push_back(pass);
            return pass;
        }
    }
    return 0;
}

bool CMultiTexturingManager::removePass(scene::ISceneNode *node, u32 layer){
    for(u32 i=0; i<array_Nodes.size();i++){
        if(array_Nodes[i].Node == node){
            for(u32 j = 0; j<array_Nodes[i].array_Passes.size();j++){
                if(j==layer)
                    array_Nodes[i].array_Passes.erase(j);
                    return true;
            }
        }
    }
    return false;
}

bool CMultiTexturingManager::removePass(scene::ISceneNode *node, STexturePass *pass){
    for(u32 i=0; i<array_Nodes.size();i++){
        if(array_Nodes[i].Node == node){
            for(u32 j = 0; j<array_Nodes[i].array_Passes.size();j++){
                if(array_Nodes[i].array_Passes[j]==pass)
                    array_Nodes[i].array_Passes.erase(j);
                    return true;
            }
        }
    }
    return false;
}


void CMultiTexturingManager::drawAll(){

	for(u32 i=0; i<array_Nodes.size();i++)
	{
        //I learned this meshbuffer trick from Viz_Fuerte's "Simple but useful projects"
        if(!smgr->isCulled(array_Nodes[i].Node))
        {
            array_Nodes[i].Node->setVisible(true);
            array_Nodes[i].Node->OnRegisterSceneNode();
            array_Nodes[i].Node->updateAbsolutePosition();
            array_Nodes[i].Node->setVisible(false);
            //Reset the transformation
            if(array_Nodes[i].Node->getType()==scene::ESNT_TERRAIN)
                driver->setTransform(video::ETS_WORLD,core::IdentityMatrix);
            else
                driver->setTransform(video::ETS_WORLD,array_Nodes[i].Node->getAbsoluteTransformation());

            for(u32 j=0; j<array_Nodes[i].array_Passes.size();j++){
                array_Nodes[i].Node->setMaterialTexture(0,array_Nodes[i].array_Passes[j]->splat_texture);
                array_Nodes[i].Node->setMaterialTexture(1,array_Nodes[i].array_Passes[j]->red_texture);
                array_Nodes[i].Node->setMaterialTexture(2,array_Nodes[i].array_Passes[j]->green_texture);
                array_Nodes[i].Node->setMaterialTexture(3,array_Nodes[i].array_Passes[j]->blue_texture);

                if(array_Nodes[i].Node->getType()==scene::ESNT_TERRAIN){
                    video::SMaterial material = array_Nodes[i].Node->getMaterial(0);
                    material.MaterialType = (video::E_MATERIAL_TYPE)shaderMaterial;
                    material.MaterialTypeParam =pack_textureBlendFunc(video::EBF_DST_COLOR,video::EBF_ONE);

                    driver->setMaterial(material);
                    driver->drawMeshBuffer(((scene::ITerrainSceneNode*)array_Nodes[i].Node)->getRenderBuffer());
                }else{
                    for(u32 k=0; k<array_Nodes[i].Node->getMaterialCount(); ++k)
                    {
                        video::SMaterial material = array_Nodes[i].Node->getMaterial(k);
                        material.MaterialType = (video::E_MATERIAL_TYPE)shaderMaterial;
                        material.MaterialTypeParam =pack_textureBlendFunc(video::EBF_DST_COLOR,video::EBF_ONE);

                        driver->setMaterial(material);
                        switch(array_Nodes[i].Node->getType()){
                        case scene::ESNT_ANIMATED_MESH:
                            driver->drawMeshBuffer(((scene::IAnimatedMeshSceneNode*)array_Nodes[i].Node)->getMesh()->getMeshBuffer(k));
                            break;
                        default:
                            driver->drawMeshBuffer(((scene::IMeshSceneNode*)array_Nodes[i].Node)->getMesh()->getMeshBuffer(k));
                            break;
                        };
                    }
                }
			}
        }
	}
}

void CMultiTexturingManager::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    int layer0 = 0;
    int layer1 = 1;
    int layer2 = 2;
    int layer3 = 3;
    services->setPixelShaderConstant("splatMap",(float*)&layer0,1);
    services->setPixelShaderConstant("layer_red",(float*)&layer1,1);
    services->setPixelShaderConstant("layer_green",(float*)&layer2,1);
    services->setPixelShaderConstant("layer_blue",(float*)&layer3,1);
}

