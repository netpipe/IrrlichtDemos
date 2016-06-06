#include "CShieldManager.h"

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;


CShieldManager::CShieldManager(scene::ISceneManager* psmgr, irr::ITimer *ptimer){
    //Assign all parameters
    smgr = psmgr;
    driver=psmgr->getVideoDriver();
    timer = ptimer;

    ///GLSL Shader Code
    //Vertex Shader
    const c8 *vertShader = "varying vec3 vertpos;           \
                                                            \
                    void main(void)                         \
                    {                                       \
                        vertpos = gl_Vertex.xyz;            \
                                                            \
                        gl_TexCoord[0] = gl_MultiTexCoord0; \
                        gl_Position = ftransform();         \
                    }                                       \
                    ";

    //Fragment Shader
    const c8 *fragShader = "uniform sampler2D ColorMap;                                                                     \
                    uniform sampler2D GradientTexture;                                                                      \
                    uniform float radius;                                                                                   \
                    uniform vec4 color;                                                                                     \
                    uniform vec3 current_position;                                                                          \
                                                                                                                            \
                    varying vec3 vertpos;                                                                                   \
                                                                                                                            \
                    void main(void)                                                                                         \
                    {                                                                                                       \
                        vec3 dif =  vertpos - current_position;                                                             \
                        float dist = sqrt(dif.x*dif.x + dif.y*dif.y + dif.z*dif.z);                                         \
                                                                                                                            \
                        float intensity=(1.0/dist)*radius*0.01; 			                        	                	\
                        if(intensity>0.9)                                                                                   \
                            intensity=0.9;	                                                                                \
                        if(intensity<0)                                                                                     \
                            intensity=0;	                                                                                \
                                                                                                                            \
                        vec4 diffuse = texture2D(ColorMap, vec2(gl_TexCoord[0]));                                           \
                        vec4 gradient = vec4(texture2D(GradientTexture,vec2(intensity,1)));                                 \
                                                                                                                            \
                        vec4 final_color = gradient*diffuse*intensity*color;                                                \
                                                                                                                            \
                        gl_FragColor=final_color;                                                                           \
                    }                                                                                                       \
                    ";
//float intensity=1/pow(dist,2.0)*radius*0.01;  obsolete

    //Create the shader material
    shaderMaterial = driver->getGPUProgrammingServices()->addHighLevelShaderMaterial(
            vertShader, "main", video::EVST_VS_2_0,
            fragShader, "main", video::EPST_PS_2_0,
            this, video::EMT_ONETEXTURE_BLEND);
}

CShieldManager::~CShieldManager(){
    //Erase everything
    for(u32 i=0; i<array_Shields.size();i++){
        while(!array_Shields[i].array_Impacts.empty()){
            array_Shields[i].array_Impacts.erase(0);
        }
    }
    while(!array_Shields.empty()){
        array_Shields.erase(0);
    }
}

bool CShieldManager::addShield(scene::ISceneNode *node){
    //Check if shield already exists
    for(u32 i=0; i<array_Shields.size();i++){
	    if(array_Shields[i].shieldNode == node){
	        //Shield already exists
	        return false;
	    }
    }
    //Create a new shield
    SShield tmpShield;
    tmpShield.shieldNode = node;
    array_Shields.push_back(tmpShield);
    return true;
}

bool CShieldManager::removeShield(scene::ISceneNode *node){
    //Check if shield exists
    for(u32 i=0; i<array_Shields.size();i++){
       if(array_Shields[i].shieldNode == node){
            //Erase all impacts of the shield
            while(!array_Shields[i].array_Impacts.empty()){
                array_Shields[i].array_Impacts.erase(0);
            }
           array_Shields.erase(i);
           return true;
       }
    }
    return false;
}

bool CShieldManager::addLocalImpact(const scene::ISceneNode *node, const core::vector3df position,const f32 radius, const u32 duration, const video::SColorf color)
{
    //Find the shield
    for(u32 i=0; i<array_Shields.size();i++){
	    if(array_Shields[i].shieldNode == node){
	        //Create a new impact
            SImpact tmpImpact;
            tmpImpact.pos = position;
            tmpImpact.startTime = timer->getTime();
            tmpImpact.endTime = timer->getTime()+duration;
            tmpImpact.radius = radius;
            tmpImpact.color = color;

            array_Shields[i].array_Impacts.push_back(tmpImpact);
            return true;
	    }
    }
    //Shield was not found
    return false;
}

bool CShieldManager::addGlobalImpact(const scene::ISceneNode *node, core::vector3df &position,const f32 radius, const u32 duration, const video::SColorf color)
{
    //Find the shield
    for(u32 i=0; i<array_Shields.size();i++){
	    if(array_Shields[i].shieldNode == node){
	        //Transform the position to local space
            core::matrix4 trans = node->getAbsoluteTransformation();
            trans.makeInverse();
            trans.transformVect(position);

	        //Create a new impact
            SImpact tmpImpact;
            tmpImpact.pos = position;
            tmpImpact.startTime = timer->getTime();
            tmpImpact.endTime = timer->getTime()+duration;
            tmpImpact.radius = radius;
            tmpImpact.color = color;

            array_Shields[i].array_Impacts.push_back(tmpImpact);
            return true;
	    }
    }
    //Shield was not found
    return false;
}

core::vector3df CShieldManager::addImpact(const scene::ISceneNode *node, const core::line3df ray, const f32 radius, const u32 duration, const video::SColorf color){
    //Check if node pointer is given, otherwise the collision point routine will crash
    if(!node)
        return ray.end;

    //Check for intersection
    core::vector3df colPoint;
    core::triangle3df colTris;
    scene::ISceneNode* outNode;
    if(smgr->getSceneCollisionManager()->getCollisionPoint(ray,
            node->getTriangleSelector(), colPoint,
            colTris,outNode))
    {
        //Intersection found
        //Transform position to local space
        core::vector3df tmpPoint = core::vector3df(colPoint);
        core::matrix4 trans = node->getAbsoluteTransformation();
        trans.makeInverse();
        trans.transformVect(colPoint);
        //Add impact
        addLocalImpact(node,colPoint,radius,duration,color);
        return tmpPoint;
    }
    //No intersection
    return ray.end;
}

void CShieldManager::drawAll(){

	for(u32 i=0; i<array_Shields.size();i++)
	{
        //Check for dead impacts and delete them
        for(u32 j=0; j<array_Shields[i].array_Impacts.size();){
            if(timer->getTime()>array_Shields[i].array_Impacts[j].endTime){ //If impact is dead
                array_Shields[i].array_Impacts.erase(j);
            }else{
                ++j;
            }
	    }
        //I learned this meshbuffer trick from Viz_Fuerte's "Simple but useful projects"
        //Draw shield only when node is visible
      //  if(!smgr->isCulled(array_Shields[i].shieldNode))
      //  {
            //Reset the transformation
            driver->setTransform(video::ETS_WORLD,array_Shields[i].shieldNode->getAbsoluteTransformation());

            //Draw all impacts
            for(u32 j=0; j<array_Shields[i].array_Impacts.size();j++){
                tmp_position = array_Shields[i].array_Impacts[j].pos;

                //Calculate the impact's radius dependant on time
                u32 dif1 = array_Shields[i].array_Impacts[j].endTime-array_Shields[i].array_Impacts[j].startTime;
                u32 dif2 = timer->getTime()-array_Shields[i].array_Impacts[j].startTime;
                //Range from start (0.0) to end (1.0)
                f32 dif = (dif2/(dif1*1.0));
                //Calculate the radius with a simple parable (you can of course change this function. It uses the range (dif) as x parameter and returns the current radius
                tmp_radius = array_Shields[i].array_Impacts[j].radius*(0.25 - pow((dif-0.5),2));
                tmp_color = array_Shields[i].array_Impacts[j].color;

                //Draw the impact on all materials of the shield node
				for(u32 k=0; k<array_Shields[i].shieldNode->getMaterialCount(); ++k)
				{
					video::SMaterial material = array_Shields[i].shieldNode->getMaterial(k);
					material.MaterialType = (video::E_MATERIAL_TYPE)shaderMaterial;
					material.MaterialTypeParam = pack_textureBlendFunc(video::EBF_ONE,video::EBF_ONE_MINUS_SRC_COLOR);
					//Prevent transparency issues
					material.setFlag(video::EMF_ZWRITE_ENABLE,false);
 //material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

 material.Lighting = true;
					driver->setMaterial(material);
					switch(array_Shields[i].shieldNode->getType()){
                    case scene::ESNT_ANIMATED_MESH:
                        driver->drawMeshBuffer(((scene::IAnimatedMeshSceneNode*)array_Shields[i].shieldNode)->getMesh()->getMeshBuffer(k));
                        break;
                    default:
                        driver->drawMeshBuffer(((scene::IMeshSceneNode*)array_Shields[i].shieldNode)->getMesh()->getMeshBuffer(k));
                        break;
					};
				}
			}
        }
//	}
}

//This is called for each impact render
void CShieldManager::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    services->setPixelShaderConstant("current_position", (float*)(&tmp_position ), 3);
    services->setPixelShaderConstant("radius", (float*)(&tmp_radius ), 1);
    services->setPixelShaderConstant("color", (float*)(&tmp_color ), 4);

    //Set the texture layers
    int var0=1;
    services->setPixelShaderConstant("ColorMap", (float*)(&var0), 1);
    int var1=2;
    services->setPixelShaderConstant("GradientTexture", (float*)(&var1), 1);
}

