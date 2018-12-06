#include "HardwareSkinCallback.h"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


HWSkinCB::HWSkinCB()
{
	delay=0; //(17 ms = appr 1/60th of a second = 60 FPS)	
}

HWSkinCB::~HWSkinCB()
{
	this->drop();
}

HWSkinCB* HWSkinCB::getInstance()
{
    static HWSkinCB* instance = 0;
    if (!instance) instance = new HWSkinCB();
    return instance;
}

void HWSkinCB::setupNode(IrrlichtDevice *device, IAnimatedMeshSceneNode* node, u32 refresh)
{
	if (refresh)
		delay=refresh;

	bool result = node->getMaterial(0).getFlag(video::EMF_LIGHTING); 
	if (result==false)
		lightsenabled = false;
	else 
		lightsenabled = true;

	this->device = device;
	this->mesh = (ISkinnedMesh*)node->getMesh();
	this->mesh->setHardwareMappingHint(EHM_DYNAMIC);
		
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();
	
	for(u32 i = 0;i < this->mesh->getMeshBuffers().size();++i)
	{
		for(u32 g = 0;g < this->mesh->getMeshBuffers()[i]->getVertexCount();++g)
		{
			this->mesh->getMeshBuffers()[i]->getVertex(g)->Color = SColor(0,0,0,0);
		}
	}
	
	for(u32 z = 0;z < this->mesh->getAllJoints().size();++z)
	{
		for(u32 j = 0;j < this->mesh->getAllJoints()[z]->Weights.size();++j)
		{
			int buffId = this->mesh->getAllJoints()[z]->Weights[j].buffer_id;

			int vertexId = this->mesh->getAllJoints()[z]->Weights[j].vertex_id;
			SColor* vColor = &this->mesh->getMeshBuffers()[buffId]->getVertex(vertexId)->Color;
			
			if(vColor->getRed() == 0)
				vColor->setRed(z + 1);
			else if(vColor->getGreen() == 0)
				vColor->setGreen(z + 1);
			else if(vColor->getBlue() == 0)
				vColor->setBlue(z + 1);
			else if(vColor->getAlpha() == 0)
				vColor->setAlpha(z + 1);
		}
	}

	printf("Joint Num: %d, current limit is 55 joints.\n",mesh->getAllJoints().size());

	// Applying the shader
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	int HWSkinMat = EMT_SOLID;
	
	if(driver->getDriverType() == EDT_OPENGL && !lightsenabled)
		HWSkinMat = gpu->addHighLevelShaderMaterialFromFiles
		("HWSkinV.glsl","main",EVST_VS_2_0,"","",EPST_PS_2_0,this);

	if(driver->getDriverType() == EDT_OPENGL && lightsenabled)
		HWSkinMat = gpu->addHighLevelShaderMaterialFromFiles
		("HWSkinV_lights.glsl","main",EVST_VS_2_0,"","",EPST_PS_2_0,this);

	if(driver->getDriverType() != EDT_OPENGL)
		HWSkinMat = gpu->addHighLevelShaderMaterialFromFiles
		("HWSkin.hlsl","vertexMain",EVST_VS_2_0,"","",EPST_PS_2_0,this);

	node->setMaterialType((E_MATERIAL_TYPE)HWSkinMat);
	mesh->setHardwareSkinning(true);
}

void HWSkinCB::copyMat(f32* pointer, matrix4 &mat)
{
	for(int i = 0;i < 16;++i)
		*pointer++ = mat[i];
}

void HWSkinCB::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
	if(services->getVideoDriver()->getDriverType() == EDT_DIRECT3D9)
	{
		core::matrix4 worldViewProj;
		worldViewProj = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
		worldViewProj *= services->getVideoDriver()->getTransform(video::ETS_VIEW);
		worldViewProj *= services->getVideoDriver()->getTransform(video::ETS_WORLD);
				
		core::matrix4 world;
		world = services->getVideoDriver()->getTransform(video::ETS_WORLD)[0];
		services->setVertexShaderConstant("mWorldViewProj", &worldViewProj[0], 16);
		services->setVertexShaderConstant("mWorld", &world[0], 16);
		
		if (lightsenabled)		
		{
			f32* lightPosArray = new f32[8 * 3];
			f32* lightPosArrayPtr = lightPosArray;
			f32* lightColArray = new f32[8 * 4];
			f32* lightColArrayPtr = lightColArray;

			for(u32 i = 0; i < 8;++i)
			{
				if(i < services->getVideoDriver()->getDynamicLightCount())
				{
					vector3df lightPos = services->getVideoDriver()->getDynamicLight(i).Position;
					SColorf lightCol = services->getVideoDriver()->getDynamicLight(i).DiffuseColor;
					lightPosArray[0] = lightPos.X;
					lightPosArray[1] = lightPos.Y;
					lightPosArray[2] = lightPos.Z;
					lightPosArray += 3;
				
					lightColArray[0] = lightCol.r;
					lightColArray[1] = lightCol.g;
					lightColArray[2] = lightCol.b;
					lightColArray[3] = lightCol.a;
					lightColArray += 4;
				}
				else
				{
					lightPosArray[0] = 0;
					lightPosArray[1] = 0;
					lightPosArray[2] = 0;
					lightPosArray += 3;
				
					lightColArray[0] = 0;
					lightColArray[1] = 0;
					lightColArray[2] = 0;
					lightColArray[3] = 0;
					lightColArray += 4;
				}

				services->setVertexShaderConstant("lightPosArray", &lightPosArrayPtr[0], 8 * 3);
				services->setVertexShaderConstant("lightColorArray", &lightColArrayPtr[0], 8 * 4);
			}
			delete (lightPosArrayPtr);
			delete (lightColArrayPtr);
		}
	}
	u32 timer = device->getTimer()->getRealTime();
	if (timer-oldtimer>=delay)
	{	
		oldtimer = timer;
		if (mesh)
		{
			f32* JointArray = new f32[mesh->getAllJoints().size() * 16];
			int copyIncrement = 0;

			for(u32 i = 0;i < mesh->getAllJoints().size();++i)
			{
				core::matrix4 JointVertexPull(core::matrix4::EM4CONST_NOTHING);
				JointVertexPull.setbyproduct(
				mesh->getAllJoints()[i]->GlobalAnimatedMatrix, 
				mesh->getAllJoints()[i]->GlobalInversedMatrix);
			
				copyMat(JointArray + copyIncrement, JointVertexPull);
				copyIncrement += 16;				
			}
			
			
			/*
			// Special method for ATI.
			if(use_ati == 'y')
			{
				printf("Using ATI method!\n");
				copyIncrement = 0;
				for(int i = 0;i < dwarfMesh->getAllJoints().size();++i)
				{
					char tmp[64] = {};
					sprintf(tmp,"JointTransform[%d]",i);
					services->setVertexShaderConstant(tmp, JointArray + copyIncrement, 16);
					copyIncrement += 16;
				}
			}
			else
			*/
			bool success = services->setVertexShaderConstant("JointTransform", JointArray, mesh->getAllJoints().size() * 16);
			delete(JointArray);
		}
	}
}
