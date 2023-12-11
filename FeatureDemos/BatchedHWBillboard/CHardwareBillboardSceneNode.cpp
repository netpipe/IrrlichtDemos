
#include "CHardwareBillboardSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "IGPUProgrammingServices.h"

char vertexGLSL[] = "void main()"\
"{"\
"	gl_Position    = gl_ProjectionMatrix*(gl_ModelViewMatrix*gl_Vertex+vec4(gl_Normal,0.0) );"\
"	gl_FrontColor  = gl_Color;"\
"	gl_TexCoord[0] = gl_MultiTexCoord0;"\
"}";

char pixelGLSL[] = "uniform sampler2D tex;"\
"void main()"\
"{"\
"	gl_FragColor    = texture2D(tex,gl_TexCoord[0].xy)*gl_Color;"\
"}";

namespace irr
{
namespace scene
{

CHardwareBillboardSceneNode::CHardwareBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr,
        IBillboardSceneNode** nodes, u32 count, core::array<IEmptyBillboardSceneNode*> *impostorsOut,  s32 id,
		const core::vector3df& position)
		: CBatchedBillboardSceneNode(parent,mgr,nodes,count,impostorsOut,id,position)
{
}

CHardwareBillboardSceneNode::~CHardwareBillboardSceneNode()
{
    for (u32 i=0; i<impostors.size(); i++)
        impostors[i]->drop();
}

IEmptyBillboardSceneNode* CHardwareBillboardSceneNode::addBillboard(IBillboardSceneNode* node)
{
    IEmptyBillboardSceneNode* result = CBatchedBillboardSceneNode::addBillboard(node);
    if (!result)
        return result;
    u32 i = impostors.size()-1;
    core::vector3df pos = impostorTranslations[i];
    core::vector3df scale = impostorScales[i];
    vertices[i*4].Pos = pos;
    vertices[1+i*4].Pos = pos;
    vertices[2+i*4].Pos = pos;
    vertices[3+i*4].Pos = pos;/*
    vertices[i*4].Normal = core::vector3df(1.f,1.f,0.f);
    vertices[1+i*4].Normal = core::vector3df(1.f,-1.f,0.f);
    vertices[2+i*4].Normal = - core::vector3df(1.f,1.f,0.f);
    vertices[3+i*4].Normal = core::vector3df(-1.f,1.f,0.f);*/
    vertices[i*4].Normal = core::vector3df(scale.X*impostorSizes[i].Width,scale.Y*impostorSizes[i].Height,0.f);
    vertices[1+i*4].Normal = core::vector3df(scale.X*impostorSizes[i].Width,-scale.Y*impostorSizes[i].Height,0.f);
    vertices[2+i*4].Normal = - core::vector3df(scale.X*impostorSizes[i].Width,scale.Y*impostorSizes[i].Height,0.f);
    vertices[3+i*4].Normal = core::vector3df(-scale.X*impostorSizes[i].Width,scale.Y*impostorSizes[i].Height,0.f);
    if (impostors.size()!=1)
        return result;
    material = billMaterial[0];
    material.MaterialType = (video::E_MATERIAL_TYPE)SceneManager->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(vertexGLSL,"main",video::EVST_VS_1_1,pixelGLSL,"main",video::EPST_PS_1_1,this,billMaterial[0].MaterialType);
    material.setFlag(video::EMF_BACK_FACE_CULLING,false);
    material.setFlag(video::EMF_ZWRITE_ENABLE,false);
}

void CHardwareBillboardSceneNode::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
}

void CHardwareBillboardSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!driver)
		return;

	// draw
	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(getBoundingBox(), video::SColor(0,208,195,152));
	}

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

    driver->setMaterial(material);
    driver->drawIndexedTriangleList(vertices.pointer(),vertices.size(),indices.pointer(),vertices.size()/2);
}

}
}
