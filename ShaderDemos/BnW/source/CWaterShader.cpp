#include "CWaterShader.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;

CWaterShader::CWaterShader() : driver(0), smgr(0), timer(0), currentMaterial(0), matWater(-1), matWaterShader("WaterShader.hlsl")
{
}

void CWaterShader::OnSetMaterial(const irr::video::SMaterial &material)
{
  currentMaterial = &material;
}

void CWaterShader::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
{
  matrix4 mWorld = driver->getTransform(ETS_WORLD);
  services->setVertexShaderConstant("mWorld", mWorld.pointer(), 16);

  matrix4 mWorldViewProjection  = driver->getTransform(ETS_PROJECTION);
          mWorldViewProjection *= driver->getTransform(ETS_VIEW);
          mWorldViewProjection *= mWorld;

  services->setVertexShaderConstant("mWorldViewProj",  mWorldViewProjection.pointer(), 16);
  services->setPixelShaderConstant ("mWorldViewProjP", mWorldViewProjection.pointer(), 16);

  f32 camPos[4];
  smgr->getActiveCamera()->getAbsolutePosition().getAs4Values(&camPos[0]);
  services->setVertexShaderConstant("CamPos", &camPos[0], 4);

  f32 time = (f32)timer->getRealTime();
  services->setVertexShaderConstant("Time", &time, 1);

  f32 sinWave = currentMaterial->Shininess;
  services->setVertexShaderConstant("sinWave",  &sinWave, 1);
  services->setPixelShaderConstant ("sinWaveP", &sinWave, 1);

  f32 refraction = currentMaterial->Thickness;
  services->setPixelShaderConstant ("refractionP", &refraction, 1);

  f32 seaLevel = currentMaterial->MaterialTypeParam2;
  services->setPixelShaderConstant ("seaLevel", &seaLevel, 1);
}

const irr::s32 CWaterShader::createMaterial(irr::IrrlichtDevice *pDevice)
{
  if (!pDevice || matWater != -1) return matWater;

  driver = pDevice->getVideoDriver();
  smgr   = pDevice->getSceneManager();
  timer  = pDevice->getTimer();

  IGPUProgrammingServices *gpu = driver->getGPUProgrammingServices();

  if (gpu)
  {
    matWater = gpu->addHighLevelShaderMaterialFromFiles
    (
      matWaterShader, "vertexMain", EVST_VS_2_a,
      matWaterShader, "pixelMain",  EPST_PS_2_a,
      this, EMT_TRANSPARENT_ALPHA_CHANNEL
    );
  }

  return matWater;
}