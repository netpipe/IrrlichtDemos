/*
 * Copyright (c) 2007, elvman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY elvman ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <string>
#include <irrlicht.h>
#include <ctime>
#include "intro.h"
#ifndef RW_H
#define RW_H

class RealisticWaterSceneNode: public irr::scene::ISceneNode, irr::video::IShaderConstantSetCallBack
{
public:
   RealisticWaterSceneNode(irr::f32 width, irr::f32 height, irr::video::ITexture* bumpTexture, irr::ITimer* timer, irr::core::dimension2di renderTargetSize=irr::core::dimension2di(512,512),irr::scene::ISceneNode* parent = 0, irr::s32 id = -1);
   virtual ~RealisticWaterSceneNode();
   // frame
   virtual void OnRegisterSceneNode();

   virtual void OnAnimate(irr::u32 timeMs, int camorder);
   // renders terrain
   virtual void render();
   virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const {return WaterMesh->getBoundingBox();}
   virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);
   virtual void setWindForce(const irr::f32 windForce) {WindForce=windForce;}
   virtual void setWindDirection(const irr::core::vector2df& windDirection);
   virtual void setWaveHeight(const irr::f32 waveHeight) {WaveHeight=waveHeight;}
   virtual void setWaterColor(const irr::video::SColorf& waterColor) {WaterColor=waterColor;}
   virtual void setColorBlendFactor(const irr::f32 colorBlendFactor) {ColorBlendFactor=colorBlendFactor;}
   virtual void setReflectivity(const irr::f32 reflectiv) {reflectivity=reflectiv;}
   void writeReflection();
   virtual irr::f32 getWindForce() {return WindForce;}
   virtual irr::core::vector2df getWindDirection() {return WindDirection;};
   virtual irr::f32 getWaveHeight() {return WaveHeight;}
   virtual irr::video::SColorf getWaterColor() {return WaterColor;}
   virtual irr::f32 getColorBlendFactor() {return ColorBlendFactor;}
   virtual irr::f32 getReflectivity() {return reflectivity;}
   virtual irr::core::dimension2df getSize() {return Size;}

private:

   irr::scene::ICameraSceneNode* Camera;
   irr::scene::ICameraSceneNode* CurrentCamera;
   irr::scene::IMesh* WaterMesh;
   irr::core::vector3df positionOA;
   irr::core::vector3df targetOA;

   irr::video::IVideoDriver* VideoDriver;
   irr::ITimer* Timer;
   irr::core::dimension2d<irr::f32> Size;
   irr::s32 ShaderMaterial;

   irr::video::ITexture* RefractionMap;
   irr::video::ITexture* ReflectionMap;
   irr::f32 WindForce;   irr::core::vector2df WindDirection;   irr::f32 WaveHeight;
   irr::video::SColorf WaterColor;
   irr::f32 ColorBlendFactor;
   irr::f32 reflectivity;
   irr::video::SMaterial Material;
   bool CamAbove;
};

RealisticWaterSceneNode::RealisticWaterSceneNode(irr::f32 width, irr::f32 height, irr::video::ITexture* bumpTexture, irr::ITimer* timer, irr::core::dimension2di renderTargetSize, irr::scene::ISceneNode* parent, irr::s32 id) : irr::scene::ISceneNode( parent, device->getSceneManager(), id) {
   Timer = timer;
   Size = irr::core::dimension2d<irr::f32>(width,height);
   RefractionMap=NULL;
   ReflectionMap=NULL;
   reflectivity=0.25;
   WindForce = 20.0f;
   WindDirection = irr::core::vector2df(0,1);
   WaveHeight =0.3f;
   WaterColor= irr::video::SColorf(0.1f, 0.1f, 0.6f, 1.0f);
   ColorBlendFactor=0.2f;
   Camera=NULL;
   VideoDriver = device->getSceneManager()->getVideoDriver();

   CurrentCamera= device->getSceneManager()->getActiveCamera(); //get current camera
   Camera=device->getSceneManager()->addCameraSceneNode(); //create new camera
   device->getSceneManager()->setActiveCamera(CurrentCamera); //set back the previous camera	

   WaterMesh = device->getSceneManager()->addHillPlaneMesh("realisticwater",Size,irr::core::dimension2d<irr::u32>(1,1));
   WaterMesh->setHardwareMappingHint(irr::scene::EHM_STATIC);
   irr::video::IGPUProgrammingServices* GPUProgrammingServices = VideoDriver->getGPUProgrammingServices();

   std::string WaterPixelShader="shaders/Water_ps.glsl";
   std::string WaterVertexShader="shaders/Water_vs.glsl";
   ShaderMaterial=GPUProgrammingServices->addHighLevelShaderMaterialFromFiles( WaterVertexShader.c_str(),"main",irr::video::EVST_VS_1_1, WaterPixelShader.c_str(), "main",irr::video::EPST_PS_2_0, this,irr::video::EMT_LIGHTMAP);

   Material.MaterialType = (irr::video::E_MATERIAL_TYPE)ShaderMaterial;
   Material.setTexture(0,bumpTexture);

   RefractionMap=VideoDriver->addRenderTargetTexture(renderTargetSize);
   ReflectionMap=VideoDriver->addRenderTargetTexture(renderTargetSize);

   Material.setTexture(1,RefractionMap);
   Material.setTexture(2,ReflectionMap);
}

RealisticWaterSceneNode::~RealisticWaterSceneNode()
{
        Camera->remove();
}

// frame
void RealisticWaterSceneNode::OnRegisterSceneNode()
{
	SceneManager->registerNodeForRendering(this);
	ISceneNode::OnRegisterSceneNode();
}
void RealisticWaterSceneNode::writeReflection() {
	if(!VideoDriver->writeImageToFile(VideoDriver->createImageFromData(irr::video::ECF_A8R8G8B8,ReflectionMap->getSize(),ReflectionMap->lock(),false,false), "./shot.jpg"))
		std::cout<<"gay!\n";
}

void RealisticWaterSceneNode::OnAnimate(irr::u32 timeMs, int camorder)
{
   if (IsVisible) {
      irr::scene::ICameraSceneNode* CurrentCamera=SceneManager->getActiveCamera(); //get current camera
      CamAbove = CurrentCamera->getAbsolutePosition().Y>=getAbsolutePosition().Y;
      if (CamAbove) {
         VideoDriver->setRenderTarget(RefractionMap, true,true, irr::video::SColor(255,0,0,0)); //render to refraction
         device->getSceneManager()->drawAll();
      
         VideoDriver->setRenderTarget(ReflectionMap, true,true, irr::video::SColor(255,0,0,0)); //render to reflection
      // Added by Christian Clavet to update the camera FOV (Zooming)
         Camera->setFarValue(CurrentCamera->getFarValue());
         Camera->setFOV(CurrentCamera->getFOV());
         positionOA=CurrentCamera->getAbsolutePosition();
         positionOA.Y=-positionOA.Y+2*RelativeTranslation.Y; //position of the water
         Camera->setPosition(positionOA);
         targetOA=CurrentCamera->getTarget();
         targetOA.Y=-targetOA.Y+2*RelativeTranslation.Y;
         Camera->setTarget(targetOA);
         SceneManager->setActiveCamera(Camera); //set the reflection camera         SceneManager->drawAll(); //draw the scene
         SceneManager->setActiveCamera(CurrentCamera);
         VideoDriver->setRenderTarget(0,false,true);

         OnRegisterSceneNode();
      }
   }
   ISceneNode::OnAnimate(timeMs);
}

void RealisticWaterSceneNode::render() {
   if (CamAbove) {
      VideoDriver->setMaterial(Material);

   //On sauvegarde les matrices
      irr::core::matrix4 MatrixWorld = VideoDriver->getTransform(irr::video::ETS_WORLD);

   //On applique les matrices de monde et de vue
      VideoDriver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
   //On dessine nos Quad ici
      VideoDriver->drawMeshBuffer(WaterMesh->getMeshBuffer(0));

   //On restaure les matrices
      VideoDriver->setTransform(irr::video::ETS_WORLD,MatrixWorld);
   }
}

void RealisticWaterSceneNode::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData){
   irr::core::matrix4 view = VideoDriver->getTransform(irr::video::ETS_VIEW);			
   services->setVertexShaderConstant("View", view.pointer(), 16);
//vertex shader constants
   irr::core::matrix4 worldViewProj = VideoDriver->getTransform(irr::video::ETS_PROJECTION);			
   worldViewProj *= VideoDriver->getTransform(irr::video::ETS_VIEW);
   worldViewProj *= VideoDriver->getTransform(irr::video::ETS_WORLD);
   services->setVertexShaderConstant("WorldViewProj", worldViewProj.pointer(), 16);

   irr::core::matrix4 worldReflectionViewProj=VideoDriver->getTransform(irr::video::ETS_PROJECTION);
   worldReflectionViewProj *= Camera->getViewMatrix();
   worldReflectionViewProj *= VideoDriver->getTransform(irr::video::ETS_WORLD);
   services->setVertexShaderConstant("WorldReflectionViewProj", worldReflectionViewProj.pointer(), 16);

   irr::f32 WaveLength=0.1f;
   services->setVertexShaderConstant("WaveLength", &WaveLength, 1);
   irr::f32 time=Timer->getTime()/100000.0f;
   services->setVertexShaderConstant("Time", &time, 1);
   services->setVertexShaderConstant("WindForce", &WindForce, 1);
   services->setVertexShaderConstant("WindDirection", &WindDirection.X, 2);
   //pixel shader constants
   irr::core::vector3df CameraPosition = SceneManager->getActiveCamera()->getPosition();
   services->setPixelShaderConstant("CameraPosition", &CameraPosition.X, 3);
   services->setPixelShaderConstant("WaveHeight", &WaveHeight, 1);
   services->setPixelShaderConstant("reflectivity", &reflectivity, 1);
   services->setPixelShaderConstant("WaterColor", &WaterColor.r, 4);
   services->setPixelShaderConstant("ColorBlendFactor", &ColorBlendFactor, 1);
   int d[] = {  0,1,2  }; // sampler2d IDs
   services->setPixelShaderConstant("WaterBump", (float*)&d[0], 1);
   services->setPixelShaderConstant("RefractionMap", (float*)&d[1], 1);
   services->setPixelShaderConstant("ReflectionMap", (float*)&d[2], 1);
}

void RealisticWaterSceneNode::setWindDirection(const irr::core::vector2df& windDirection)
{
	WindDirection=windDirection;
	WindDirection.normalize();
}
#endif RW_H
