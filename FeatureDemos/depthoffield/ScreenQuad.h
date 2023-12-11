#include "RealisticWater.h"

#ifndef SCREENQUAD_H
#define SCREENQUAD_H

float FarLink;
class DepthShaderCB : public irr::video::IShaderConstantSetCallBack
{
public:

   virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
      irr::video::IVideoDriver* driver = services->getVideoDriver();

      irr::core::matrix4 worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
      worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
      worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

      services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
      services->setVertexShaderConstant("MaxD", &FarLink, 1);
   }
};

class ScreenQuad: public irr::scene::ISceneNode, irr::video::IShaderConstantSetCallBack
{
public:
   ScreenQuad(irr::scene::ICameraSceneNode* camera1=NULL, irr::scene::ICameraSceneNode* camera2=NULL);
   ~ScreenQuad();
   virtual void OnRegisterSceneNode() {
      SceneManager->registerNodeForRendering(this);
   }
   virtual void renderSplit2();
   virtual void renderSplit();
   virtual void renderB();
   virtual void render();
   virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);
   virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const {return Box;}
   void addNodeToDepthPass(irr::scene::ISceneNode *node);
   void removeNodeFromDepthPass(irr::scene::ISceneNode *node);
   irr::video::ITexture* RT2;
   irr::video::ITexture* DM2;
   irr::video::ITexture* DM;
   irr::video::ITexture* RT;
private:
   irr::scene::ICameraSceneNode* cam1;
   irr::scene::ICameraSceneNode* cam2;
   irr::video::S3DVertex Vertices[4];
   bool splitscreen;
   irr::u16 Indices[6];
   irr::video::SMaterial Material;
   irr::core::aabbox3d<irr::f32> Box;
   irr::s32 Depth;
   DepthShaderCB* depthMC;
   irr::video::IVideoDriver* VideoDriver;
   irr::core::array<irr::s32> BufferMaterialList;
   irr::core::array<irr::scene::ISceneNode*> DepthPassArray;
   irr::core::matrix4 Matrix;
   irr::scene::ISceneManager* smgr;
};

void ScreenQuad::addNodeToDepthPass(irr::scene::ISceneNode *node)
{
	if(DepthPassArray.binary_search(node) == -1)
		DepthPassArray.push_back(node);
}

void ScreenQuad::removeNodeFromDepthPass(irr::scene::ISceneNode *node)
{
	irr::s32 i = DepthPassArray.binary_search(node);
	
	if(i != -1) 
		DepthPassArray.erase(i);
}
ScreenQuad::~ScreenQuad() {
   VideoDriver->removeTexture(RT);
   VideoDriver->removeTexture(DM);
   VideoDriver->removeTexture(DM2);
   VideoDriver->removeTexture(RT2);
   BufferMaterialList.clear();
   DepthPassArray.clear();
   delete depthMC;
}

ScreenQuad::ScreenQuad(irr::scene::ICameraSceneNode* camera1, irr::scene::ICameraSceneNode* camera2) : irr::scene::ISceneNode(0, device->getSceneManager(), 0) {
   smgr = device->getSceneManager();
   splitscreen = (camera1)&&(camera2);
   this->setAutomaticCulling(irr::scene::EAC_OFF);
   depthMC = new DepthShaderCB();
   Depth = device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles("./shaders/ShadowPass1V.glsl", "vertexMain", irr::video::EVST_VS_2_0,"./shaders/ShadowPass1P.glsl", "pixelMain", irr::video::EPST_PS_2_0, depthMC, irr::video::EMT_SOLID);

   if (splitscreen) {
      cam1=camera1;
      cam2=camera2;
      irr::s32 ShaderMaterial=device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles( "./shaders/simpleVS.glsl","main",irr::video::EVST_VS_1_1, "./shaders/DepthofFieldPS_splitscreen.glsl", "main",irr::video::EPST_PS_2_0, this, irr::video::EMT_LIGHTMAP);
      RT2 = device->getVideoDriver()->addRenderTargetTexture(irr::core::dimension2di(res.x,res.y/2));
      RT = device->getVideoDriver()->addRenderTargetTexture(irr::core::dimension2di(res.x,res.y/2));
      DM = device->getVideoDriver()->addRenderTargetTexture(irr::core::dimension2di(res.x,res.y/2));
      DM2 = device->getVideoDriver()->addRenderTargetTexture(irr::core::dimension2di(res.x,res.y/2));
      Material.MaterialType=(irr::video::E_MATERIAL_TYPE)ShaderMaterial;
      Material.setTexture(0,RT);
      Material.setTexture(1,DM);
      Material.setTexture(2,RT2);
      Material.setTexture(3,DM2);
      FarLink = (cam1->getFarValue()+cam2->getFarValue())/2;
   }
   else {
      irr::s32 ShaderMaterial=device->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles( "./shaders/simpleVS.glsl","main",irr::video::EVST_VS_1_1, "./shaders/DepthofFieldPS.glsl", "main",irr::video::EPST_PS_2_0, this, irr::video::EMT_LIGHTMAP);
      RT = device->getVideoDriver()->addRenderTargetTexture(irr::core::dimension2di(res.x,res.y));
      DM = device->getVideoDriver()->addRenderTargetTexture(irr::core::dimension2di(res.x,res.y));
      Material.MaterialType=(irr::video::E_MATERIAL_TYPE)ShaderMaterial;
      Material.setTexture(0,RT);
      Material.setTexture(1,DM);
      FarLink = smgr->getActiveCamera()->getFarValue();
   }
   irr::video::SColor white(255, 255, 255, 255);

   Vertices[0] = irr::video::S3DVertex(-1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
   Vertices[1] = irr::video::S3DVertex(-1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
   Vertices[2] = irr::video::S3DVertex( 1.f,  1.f, 0.f, 0.f, 0.f, 1.f, white, 1.f, 0.f);
   Vertices[3] = irr::video::S3DVertex( 1.f, -1.f, 0.f, 0.f, 0.f, 1.f, white, 1.f, 1.f);

   Indices[0] = 1;
   Indices[1] = 2;
   Indices[2] = 3;
   Indices[3] = 0;
   Indices[4] = 1;
   Indices[5] = 3;
   Material.Wireframe = false;
   Material.Lighting = false;
   Material.ZWriteEnable = false;
   Material.BackfaceCulling = false;
   VideoDriver = device->getVideoDriver();
}

void ScreenQuad::renderB() {
   short count;
   VideoDriver->setRenderTarget(DM, true, true, irr::video::SColor(0xffffffff));
   for(irr::u32 i = 0;i < DepthPassArray.size();++i) {
      BufferMaterialList.set_used(0);
      count = DepthPassArray[i]->getMaterialCount();
      for(irr::u32 g = 0;g < count;++g)
         BufferMaterialList.push_back(DepthPassArray[i]->getMaterial(g).MaterialType);
      DepthPassArray[i]->setMaterialType((irr::video::E_MATERIAL_TYPE)Depth);
      DepthPassArray[i]->OnRegisterSceneNode();
      DepthPassArray[i]->render();
      for(irr::u32 g = 0;g < count;++g)
         DepthPassArray[i]->getMaterial(g).MaterialType = (irr::video::E_MATERIAL_TYPE)BufferMaterialList[g];
   }
   VideoDriver->setRenderTarget(RT, true, true, irr::video::SColor(0xffffffff));
}

void ScreenQuad::renderSplit2() {
   short count;
   VideoDriver->setRenderTarget(DM, true, true, irr::video::SColor(0xffffffff));
   smgr->setActiveCamera(cam1);
   for(irr::u32 i = 0;i < DepthPassArray.size();++i) {
      BufferMaterialList.set_used(0);
      count = DepthPassArray[i]->getMaterialCount();
      for(irr::u32 g = 0;g < count;++g)
         BufferMaterialList.push_back(DepthPassArray[i]->getMaterial(g).MaterialType);
      DepthPassArray[i]->setMaterialType((irr::video::E_MATERIAL_TYPE)Depth);
      DepthPassArray[i]->OnRegisterSceneNode();
      DepthPassArray[i]->render();
      for(irr::u32 g = 0;g < count;++g)
         DepthPassArray[i]->getMaterial(g).MaterialType = (irr::video::E_MATERIAL_TYPE)BufferMaterialList[g];
   }
   VideoDriver->setRenderTarget(RT, true, true, irr::video::SColor(0xffffffff));
}

void ScreenQuad::renderSplit() {
   short count;
   VideoDriver->setRenderTarget(DM2, true, true, irr::video::SColor(0xffffffff));
   smgr->setActiveCamera(cam2);
   for(irr::u32 i = 0;i < DepthPassArray.size();++i) {
      BufferMaterialList.set_used(0);
      count = DepthPassArray[i]->getMaterialCount();
      for(irr::u32 g = 0;g < count;++g)
         BufferMaterialList.push_back(DepthPassArray[i]->getMaterial(g).MaterialType);
      DepthPassArray[i]->setMaterialType((irr::video::E_MATERIAL_TYPE)Depth);
      DepthPassArray[i]->OnRegisterSceneNode();
      DepthPassArray[i]->render();
      for(irr::u32 g = 0;g < count;++g)
         DepthPassArray[i]->getMaterial(g).MaterialType = (irr::video::E_MATERIAL_TYPE)BufferMaterialList[g];
   }
   VideoDriver->setRenderTarget(RT2, true, true, irr::video::SColor(0xffffffff));
}

void ScreenQuad::render() {
   VideoDriver->setRenderTarget(0, false, false, irr::video::SColor(0xffffffff));

   // clear the projection matrix
   irr::core::matrix4 oldProjMat = VideoDriver->getTransform(irr::video::ETS_PROJECTION);
   VideoDriver->setTransform(irr::video::ETS_PROJECTION, irr::core::matrix4());

   // clear the view matrix
   irr::core::matrix4 oldViewMat = VideoDriver->getTransform(irr::video::ETS_VIEW);
   VideoDriver->setTransform(irr::video::ETS_VIEW, irr::core::matrix4());

   VideoDriver->setTransform(irr::video::ETS_WORLD, Matrix);
   VideoDriver->setMaterial(Material);

   VideoDriver->drawIndexedTriangleList(Vertices, 4, Indices, 2);
   // restore view matrix
   VideoDriver->setTransform(irr::video::ETS_VIEW, oldViewMat);

   // restore projection matrix
   VideoDriver->setTransform(irr::video::ETS_PROJECTION, oldProjMat);

}

void ScreenQuad::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
   irr::core::vector2df resolution(res.x,res.y);
   int d[] = { 0,1,2,3 }; // sampler2d IDs
   services->setPixelShaderConstant("RTexture", (float*)&d[0], 1);
   services->setPixelShaderConstant("depthTex", (float*)&d[3], 1);
   if (splitscreen) {
      services->setPixelShaderConstant("RTexture2", (float*)&d[2], 1);
      services->setPixelShaderConstant("depthTex2", (float*)&d[1], 1);
   }
   services->setPixelShaderConstant("res", &resolution.X, 2);
}
#endif SCREENQUAD_H
