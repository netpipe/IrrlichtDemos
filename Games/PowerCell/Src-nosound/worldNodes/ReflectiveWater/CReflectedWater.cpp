// Reflected Water.
// sio2 'at' users.sourceforge.net
// Based on .NET code by DeusXL (Irrlicht forum)
//

#include "CReflectedWater.h"
#include "CReflectedWater_ShaderCode.h"

CReflectedWater::CReflectedWater(const irr::c8 *name, IrrlichtDevice *device, scene::ISceneManager* smgr,
                                 s32 id, f32 tileSize, s32 tileCount, core::dimension2di RenderTargetSize)
{
    m_device   = device;
    m_smgr      = smgr;
    m_name      = name;

    // Init constants
    m_AddedColor      = video::SColor(255, 1, 1, 30);
    m_MultiColor      = video::SColor(255, 190, 190, 210);
    m_WaveHeight      = 3.0f;
    m_WaveLength      = 50.0f;
    m_WaveSpeed         = 10.0f;
    m_WaveDisplacement   = 7.0f;
    m_WaveRepetition   = 5.0f;
    m_RefractionFactor   = 0.8f;

  //  scene::IAnimatedMesh *wmesh =  smgr->addHillPlaneMesh(m_name, core::dimension2df(tileSize, tileSize), core::dimension2di(tileCount, tileCount));

 	 scene::IAnimatedMesh *wmesh = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(20,20),
		core::dimension2d<u32>(40,40), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));


    if (!wmesh) return;
    m_waternode = smgr->addMeshSceneNode(wmesh->getMesh(0));
    if (!m_waternode) return;

    s32 dmat = irr::video::EMT_SOLID;

    video::IVideoDriver* _driver = smgr->getVideoDriver();
    video::E_DRIVER_TYPE edt = _driver->getDriverType();
    video::IGPUProgrammingServices *gpu = _driver->getGPUProgrammingServices();
    if (edt == video::EDT_OPENGL)
    {
        dmat = gpu->addHighLevelShaderMaterial(
                   WATER_VERTEX_GLSL, "main", video::EVST_VS_1_1,
                   WATER_FRAGMENT_GLSL, "main", video::EPST_PS_1_1,
                   this, video::EMT_TRANSPARENT_ALPHA_CHANNEL, 0);
    }
    else
    {
        dmat = gpu->addHighLevelShaderMaterial(
                   WATER_HLSL, "vertexMain", video::EVST_VS_2_0,
                   WATER_HLSL, "pixelMain", video::EPST_PS_2_0,
                   this, video::EMT_TRANSPARENT_ALPHA_CHANNEL, 2);
    }

    m_waternode->setMaterialType((video::E_MATERIAL_TYPE)dmat);
    m_waternode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
 //   m_waternode->setMaterialFlag(video::EMF_LIGHTING, false);
    m_waternode->setMaterialFlag(video::EMF_FOG_ENABLE, false);

    m_rt = _driver->createRenderTargetTexture(RenderTargetSize);
    m_waternode->setMaterialTexture(0, m_rt);

    scene::ICameraSceneNode *camera = smgr->getActiveCamera();
    m_watercam = smgr->addCameraSceneNode();
    smgr->setActiveCamera(camera);
}

// --------------------------------------------------------------------------

CReflectedWater::~CReflectedWater()
{
    m_rt->drop();
}

// --------------------------------------------------------------------------

void CReflectedWater::updateRendertarget(scene::ISceneManager* smgr)
{
    if (!(m_rt && m_waternode && smgr))
        return;

    video::IVideoDriver* driver = smgr->getVideoDriver();
    if (!driver) return;

    scene::ICameraSceneNode *camera = smgr->getActiveCamera();
    m_watercam->setFarValue(camera->getFarValue());
    core::vector3df Position = m_waternode->getPosition();
    core::vector3df campos = camera->getPosition();
    if (campos.Y >= Position.Y)
    {
        m_watercam->setPosition(core::vector3df(campos.X, 2 * Position.Y - campos.Y, campos.Z));
        core::vector3df target = camera->getTarget() - campos;
        target.normalize();
        target.Y *= -1;
        m_watercam->setTarget(m_watercam->getPosition() + target * 20000);
        m_watercam->setUpVector(camera->getUpVector());
    }
    else
    {
        m_watercam->setPosition(camera->getPosition());

        core::vector3df target = camera->getTarget() - camera->getPosition();
        target.normalize();
        target *= 200000;
        m_watercam->setTarget(m_watercam->getPosition() + target);
        m_watercam->setUpVector(camera->getUpVector());
    }

    driver->setRenderTarget(m_rt, true, true, video::SColor(0,100,100,100));
    smgr->setActiveCamera(m_watercam);
    m_waternode->setVisible(false);
    smgr->drawAll();
    driver->setRenderTarget(0);
    m_waternode->setVisible(true);
    smgr->setActiveCamera(camera);
}

// --------------------------------------------------------------------------

void CReflectedWater::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if (!services) return;
    video::IVideoDriver* driver = services->getVideoDriver();
    if (!driver || !m_device) return;

    if (userData == 2 || userData == 3) //All DirectX Shaders
    {
        core::matrix4 worldViewProj;
        worldViewProj = driver->getTransform(video::ETS_PROJECTION);
        worldViewProj *= driver->getTransform(video::ETS_VIEW);
        worldViewProj *= driver->getTransform(video::ETS_WORLD);
        services->setVertexShaderConstant("mWorldViewProj", &worldViewProj.M[0], 16);
    }
    /*   if (userData == 1 || userData == 3) //Clamp Shaders
       {
          if (userData == 1) //OpenGL Clamp Shader
          {
             services->setPixelShaderConstant("DiffuseMap", 0f);
             services->setPixelShaderConstant("DetailMap", 1f);
          }
          services->setPixelShaderConstant("WaterPositionY", WaterNode.Position.Y);
          return;
       }*/

    //Water Shaders
    float time = (float)m_device->getTimer()->getRealTime();
    services->setVertexShaderConstant("Time", &time, 1);
    services->setVertexShaderConstant("WaveHeight", &m_WaveHeight, 1);
    services->setVertexShaderConstant("WaveLength", &m_WaveLength, 1);
    services->setVertexShaderConstant("WaveSpeed", &m_WaveSpeed, 1);

    services->setPixelShaderConstant("AddedColor", reinterpret_cast<f32*>(&m_AddedColor), 4);
    services->setPixelShaderConstant("MultiColor", reinterpret_cast<f32*>(&m_MultiColor), 4);
    services->setPixelShaderConstant("WaveDisplacement", &m_WaveDisplacement, 1);
    services->setPixelShaderConstant("WaveRepetition", &m_WaveRepetition, 1);
    services->setPixelShaderConstant("RefractionFactor", &m_RefractionFactor, 1);
    float fUnderWater = (m_smgr->getActiveCamera()->getPosition().Y < m_waternode->getPosition().Y) ? 1.0f : 0.0f;
    services->setPixelShaderConstant("UnderWater", &fUnderWater, 1);
}
