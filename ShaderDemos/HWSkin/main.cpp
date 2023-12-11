#include <irrlicht.h>
#include <iostream>

#pragma comment(lib,"irrlicht.lib")

using namespace irr;
using namespace scene;
using namespace video;
using namespace io;
using namespace core;

void copyMat(f32* pointer, matrix4 &mat)
{
	for(int i = 0;i < 16;++i)
		*pointer++ = mat[i];
};

void copy3VEC4(f32* pointer, matrix4 &mat)
{
	for(int i = 0;i < 12;++i)
		*pointer++ = mat[i];
};

class HWSkinCB : public video::IShaderConstantSetCallBack
{
public:
	HWSkinCB(ISkinnedMesh* meshIn) : dwarfMesh(meshIn) {}

   virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
   {
		if(dwarfMesh)
		{

			if(services->getVideoDriver()->getDriverType() == EDT_DIRECT3D9)
			{
				core::matrix4 worldViewProj;
				worldViewProj = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
				worldViewProj *= services->getVideoDriver()->getTransform(video::ETS_VIEW);
				worldViewProj *= services->getVideoDriver()->getTransform(video::ETS_WORLD);

				services->setVertexShaderConstant("mWorldViewProj", &worldViewProj[0], 16);
				services->setVertexShaderConstant("mWorld", &services->getVideoDriver()->getTransform(video::ETS_WORLD)[0], 16);

				f32 lightPosArray[8 * 3];
				f32 lightColArray[8 * 4];

				f32* lightPosArrayPtr = &lightPosArray[0];
				f32* lightColArrayPtr = &lightColArray[0];

				for(int i = 0; i < 8;++i)
				{
					if(i < services->getVideoDriver()->getDynamicLightCount())
					{
						vector3df lightPos = services->getVideoDriver()->getDynamicLight(i).Position;
						SColorf lightCol = services->getVideoDriver()->getDynamicLight(i).DiffuseColor;

						lightPosArrayPtr[0] = lightPos.X;
						lightPosArrayPtr[1] = lightPos.Y;
						lightPosArrayPtr[2] = lightPos.Z;
						lightPosArrayPtr += 3;

						lightColArrayPtr[0] = lightCol.r;
						lightColArrayPtr[1] = lightCol.g;
						lightColArrayPtr[2] = lightCol.b;
						lightColArrayPtr[3] = lightCol.a;
						lightColArrayPtr += 4;
					}
					else
					{
						lightPosArrayPtr[0] = 0;
						lightPosArrayPtr[1] = 0;
						lightPosArrayPtr[2] = 0;
						lightPosArrayPtr += 3;

						lightColArrayPtr[0] = 0;
						lightColArrayPtr[1] = 0;
						lightColArrayPtr[2] = 0;
						lightColArrayPtr[3] = 0;
						lightColArrayPtr += 4;
					}

					services->setVertexShaderConstant("lightPosArray", &lightPosArray[0], 8 * 3);
					services->setVertexShaderConstant("lightColorArray", &lightColArray[0], 8 * 4);
				}
			}

			f32* JointArray = new f32[dwarfMesh->getAllJoints().size() * 16];

			int copyIncrement = 0;

			for(int i = 0;i < dwarfMesh->getAllJoints().size();++i)
			{
				core::matrix4 JointVertexPull(core::matrix4::EM4CONST_NOTHING);
				JointVertexPull.setbyproduct(
					dwarfMesh->getAllJoints()[i]->GlobalAnimatedMatrix,
					dwarfMesh->getAllJoints()[i]->GlobalInversedMatrix);

				copyMat(JointArray + copyIncrement, JointVertexPull);
				copyIncrement += 16;
			}

			services->setVertexShaderConstant("JointTransform", JointArray, dwarfMesh->getAllJoints().size() * 16);

			delete [] JointArray;
		}
   }

   ISkinnedMesh* dwarfMesh;
};

int main()
{
	printf("Dx or Ogl?\n1.Dx\n2.Ogl\n");

	char i = 0;

	std::cin >> i;

	IrrlichtDevice* device = 0;

	if(i == '1')
		device = createDevice(EDT_DIRECT3D9);
	else
		device = createDevice(EDT_OPENGL);

	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	IAnimatedMeshSceneNode* dwarf = smgr->addAnimatedMeshSceneNode(smgr->getMesh("dwarf.x"));
	dwarf->setMaterialFlag(EMF_LIGHTING, true);

	ISkinnedMesh* dwarfMesh = (ISkinnedMesh*)dwarf->getMesh();

	for(int i = 0;i < dwarfMesh->getMeshBuffers().size();++i)
		for(int g = 0;g < dwarfMesh->getMeshBuffers()[i]->getVertexCount();++g)
			dwarfMesh->getMeshBuffers()[i]->getVertex(g)->Color = SColor(0,0,0,0);

	for(int z = 0;z < dwarfMesh->getAllJoints().size();++z)
	{
		for(int j = 0;j < dwarfMesh->getAllJoints()[z]->Weights.size();++j)
		{
			int buffId = dwarfMesh->getAllJoints()[z]->Weights[j].buffer_id;

			int vertexId = dwarfMesh->getAllJoints()[z]->Weights[j].vertex_id;
			SColor* vColor = &dwarfMesh->getMeshBuffers()[buffId]->getVertex(vertexId)->Color;

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

	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	int HWSkinMat = EMT_SOLID;
	if(driver->getDriverType() == EDT_OPENGL)
		HWSkinMat = gpu->addHighLevelShaderMaterialFromFiles("HWSkinV.glsl","main",EVST_VS_3_0,"","",EPST_PS_2_0, new HWSkinCB(dwarfMesh));
	else
		HWSkinMat = gpu->addHighLevelShaderMaterialFromFiles("HWSkin.hlsl","vertexMain",EVST_VS_2_0,"","", EPST_PS_2_0, new HWSkinCB(dwarfMesh));

	dwarf->setMaterialType((E_MATERIAL_TYPE)HWSkinMat);

	ICameraSceneNode* cammy = smgr->addCameraSceneNodeFPS();
	cammy->setPosition(vector3df(0,30,-100));

	dwarfMesh->setHardwareSkinning(true);

	ILightSceneNode* light = smgr->addLightSceneNode(0,vector3df(0,0,0));
	light->addAnimator(smgr->createFlyCircleAnimator(vector3df(0,50,0),100,0.001));
	light->getLightData().DiffuseColor = SColorf(0.5,0.5,0,0);

	light = smgr->addLightSceneNode(0,vector3df(0,0,0));
	light->addAnimator(smgr->createFlyCircleAnimator(vector3df(0,40,0),100,-0.001));
	light->getLightData().DiffuseColor = SColorf(0.0,1.0,0,0);

	light = smgr->addLightSceneNode(0,vector3df(0,0,0));
	light->addAnimator(smgr->createFlyCircleAnimator(vector3df(0,60,0),80,0.001));
	light->getLightData().DiffuseColor = SColorf(0.0,0.5,1,0);

	light = smgr->addLightSceneNode(0,vector3df(0,0,0));
	light->addAnimator(smgr->createFlyCircleAnimator(vector3df(0,20,0),60,-0.002));
	light->getLightData().DiffuseColor = SColorf(0.8,0.2,0,0);

	printf("Joint Num: %d\n",dwarfMesh->getAllJoints().size());
u32 lastFPS = 0;
	while(device->run())
	{
		driver->beginScene(true,true,SColor(255,0,0,200));
		smgr->drawAll();
		driver->endScene();
			if(driver->getFPS() != lastFPS)
			{
				lastFPS = driver->getFPS();
				stringw windowCaption = "CST FPS: ";
				windowCaption += lastFPS;

				device->setWindowCaption(windowCaption.c_str());
			}
	}

	return 0;
};
