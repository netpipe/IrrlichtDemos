// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "irrlicht_manager.h"
#include "event_receiver_base.h"
#include "config.h"
#include "physics.h"
#include "CLMOMeshFileLoader.h"
#include "mesh_texture_loader.h"
#include "helper_file.h"
#include "helper_str.h"
#include "helper_irr.h"
#include "logging.h"
#include "globals.h"
#include "ogles2_materials.h"
#include "grid_triangle_selector.h"
#include <iostream>
#include <sstream>
#include <SMeshBuffer.h>
#include <cassert>
#include <algorithm>
#ifdef _IRR_ANDROID_PLATFORM_
#include <android_native_app_glue.h>
#endif


using namespace irr;
using namespace gui;

IrrlichtManager::IrrlichtManager()
:   mIrrlichtDevice(NULL)
    , mVideoDriver(NULL)
    , mSceneManager(NULL)
    , mEventReceiver(NULL)
    , mCameraMaya(NULL)
    , mCameraFPS(NULL)
    , mCameraGame(NULL)
    , mCameraEditor(NULL)
    , mCameraGui(NULL)
    , mStaticMeshTextureLoader(NULL)
    , mDynamicMeshTextureLoader(NULL)
{
    mDefaultEditorMaterial.DiffuseColor = video::SColor(130, 200, 200, 200);
    mDefaultEditorMaterial.AmbientColor = video::SColor(255, 255, 255, 255);
    mDefaultEditorMaterial.MaterialType = video::EMT_SOLID;
    mDefaultEditorMaterial.Lighting = false;
//    mDefaultEditorMaterial.ZWriteEnable = true;
    mDefaultEditorMaterial.Wireframe = true;
    mDefaultEditorMaterial.BackfaceCulling = false;
    mDefaultEditorMaterial.setFlag(video::EMF_BILINEAR_FILTER, false);
}

IrrlichtManager::~IrrlichtManager()
{
}

irr::IrrlichtDevice* IrrlichtManager::CreateIrrlichtDevicePC(const Config& config)
{
    // create device and exit if creation failed
    core::dimension2d<s32> dimension;
    dimension.Width = config.GetResolutionX();
    dimension.Height = config.GetResolutionY();
    int bitdepth = config.GetBitDepth();

    IrrlichtDevice *dummyDevice = 0;
    dummyDevice = createDevice(video::EDT_NULL,core::dimension2d<u32>(100,100),false,false);
    if ( dummyDevice )
    {
        LOG.Debug("got a dummy device\n");

        IOSOperator * osOperator = dummyDevice->getOSOperator();
        if ( osOperator )
        {
			const core::stringc& osVersion( osOperator->getOperatingSystemVersion() );
			if ( !osVersion.empty() )
            {
                LOG.Log(LP_INFO, osVersion.c_str() );
                LOG.LogLn(LP_INFO, NULL);
            }
        }

        video::IVideoModeList *videoModes = dummyDevice->getVideoModeList();
        core::dimension2d<u32> desktopRes = videoModes->getDesktopResolution();
        s32 desktopDepth = videoModes->getDesktopDepth();

        dummyDevice->closeDevice();
		dummyDevice->clearSystemMessages();
        dummyDevice->drop();
        dummyDevice = 0;

        LOG.LogLn(LP_INFO, "Desktop width:", (int)desktopRes.Width);
        LOG.LogLn(LP_INFO, "Desktop height:", (int)desktopRes.Height);
        LOG.LogLn(LP_INFO, "Desktop depth:", desktopDepth);

        // use the resolution of the desktop
        if ( config.GetUseScreenResolution()
            && desktopRes.Width && desktopRes.Height && desktopDepth )
        {
            LOG.Info("Use desktop settings\n");
            dimension = desktopRes;
            bitdepth = desktopDepth;
        }
    }

    mEventReceiver = new EventReceiverBase(config);

	SIrrlichtCreationParameters creationParameters;
	creationParameters.AntiAlias = config.GetAntiAlias();
    creationParameters.Bits = bitdepth;
    creationParameters.DriverType = config.GetDriverType();
#ifdef HC1_OGLES
	creationParameters.DriverType = video::EDT_OGLES2;
#endif
    creationParameters.EventReceiver = mEventReceiver;
    creationParameters.Fullscreen = config.GetFullscreen();
    creationParameters.HighPrecisionFPU  = false;
    creationParameters.Stencilbuffer = false;
    creationParameters.Vsync = config.GetVSync() > 0 ? true : false;
    creationParameters.WindowId = 0;
    creationParameters.WindowSize = dimension;
#ifdef HC1_OGLES
    creationParameters.OGLES2ShaderPath = io::path(config.GetPathShader().c_str());
#endif

#ifndef _DEBUG
	creationParameters.LoggingLevel = ELL_WARNING;
#endif

    LOG.Info(L"createDevice\n");
    LOG.LogLn(LP_INFO, "Resolution width:", (int)dimension.Width);
	LOG.LogLn(LP_INFO, "Resolution height:", (int)dimension.Height);
	LOG.LogLn(LP_INFO, "Resolution depth:", bitdepth);

	mIrrlichtDevice = createDeviceEx(creationParameters);
	if (!mIrrlichtDevice)
	{
	    LOG.Info(L"createDevice - fallback\n");
	    // fallback
		mIrrlichtDevice = createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(640, 480), 16, false, false, false, mEventReceiver);
        if (!mIrrlichtDevice)
            return 0;
	}
	LOG.Info(L"createDevice ok\n");

	InitVideoModes(config, mIrrlichtDevice);

	mIrrlichtDevice->setResizable(false);
	mIrrlichtDevice->setWindowCaption(L"Hovercraft - Loading...");

	mIrrlichtDevice = mIrrlichtDevice;

#ifdef HC1_OGLES
	if ( mIrrlichtDevice && creationParameters.DriverType == video::EDT_OGLES2)
	{
		AddAllES2Shaders(config, mIrrlichtDevice->getVideoDriver(), mIrrlichtDevice->getSceneManager());
	}
#endif

	return mIrrlichtDevice;
}

irr::IrrlichtDevice* IrrlichtManager::CreateIrrlichtDeviceAndroid(const Config& config, android_app * systemData)
{
	mEventReceiver = new EventReceiverBase(config);

#ifdef _IRR_ANDROID_PLATFORM_
	SIrrlichtCreationParameters param;
//	param.DriverType = video::EDT_OGLES1;				// android:glEsVersion in AndroidManifest.xml should be at least "0x00010000"
	param.DriverType = video::EDT_OGLES2;				// android:glEsVersion in AndroidManifest.xml should be at least "0x00020000"
	param.WindowSize = core::dimension2d<u32>(0,0);	// using 0,0 it will automatically set it to the maximal size
	param.PrivateData = systemData;
	param.AntiAlias  = true; 	// config.GetAntiAlias();			// made no difference at all on my tests so far, either Irrlicht or my Android devices ignore that.
	param.Vsync = true; // config.GetVSync() > 0 ? true : false;	// made no difference at all on my tests so far, either Irrlicht or my Android devices ignore that.
	param.Bits = 32;
	param.ZBufferBits = 16;
	param.EventReceiver = mEventReceiver;
	param.OGLES2ShaderPath = io::path(config.GetPathShader().c_str());

#ifndef _DEBUG
	param.LoggingLevel = ELL_NONE;
#endif

	HC1_PROFILE( s32 profCreate = getProfiler().add(L"CreateDevice", L"STARTUP"); )
	HC1_PROFILE(getProfiler().start(profCreate);)
	mIrrlichtDevice = createDeviceEx(param);
	HC1_PROFILE(getProfiler().stop(profCreate);)

	if ( mIrrlichtDevice && param.DriverType == video::EDT_OGLES2)
	{
		HC1_PROFILE(CProfileScope ps1(L"AddShaders", L"STARTUP");)
		AddAllES2Shaders(config, mIrrlichtDevice->getVideoDriver(), mIrrlichtDevice->getSceneManager());
	}

	return mIrrlichtDevice;
#else
	return 0;
#endif
}

void IrrlichtManager::LoadES2Shader(video::IGPUProgrammingServices* gpu, const irr::c8* vertexShaderName, const irr::c8* fragmentShaderName, irr::video::IShaderConstantSetCallBack * cb, irr::video::E_MATERIAL_TYPE baseMaterial)
{
#ifdef HC1_OGLES
	irr::io::IFileSystem * fs = mIrrlichtDevice->getFileSystem();
	if ( !fs )
		return;

	// Load shaders into memory first - this allows adding code to it at runtime if necessary
	irr::core::array<irr::c8> vertexFileData;
	if (!ExtIrr::ReadFile(fs, vertexShaderName, vertexFileData, true) )
	{
		LOG.LogLn(LP_WARN, "LoadES2Shader - opening vertex shader failed:", vertexShaderName);
		return;
	}
	irr::core::array<irr::c8> fragmentFileData;
	if (!ExtIrr::ReadFile(fs, fragmentShaderName, fragmentFileData, true) )
	{
		LOG.LogLn(LP_WARN, "LoadES2Shader - opening fragment shader failed:", fragmentShaderName);
		return;
	}

	core::stringc vertexProgram, fragmentProgram;
	// Here we could insert further code (for example defines) into the shader.
	vertexProgram.append(vertexFileData.const_pointer());
	fragmentProgram.append(fragmentFileData.const_pointer());

	s32 material = gpu->addHighLevelShaderMaterial(
		vertexProgram.c_str(),"main", video::EVST_VS_1_1,
		fragmentProgram.c_str(), "main",video::EPST_PS_1_1,
		cb, baseMaterial, 0, video::EGSL_DEFAULT);

	if ( material >= 0 )
	{
		mES2ShaderMaterials[baseMaterial] = material;
		mES2Shaders[material] = cb;
	}
	else
	{
		LOG.Error("error: Creating shader failed\n");
	}
#endif // HC1_OGLES
}

void IrrlichtManager::AddAllES2Shaders(const Config& config, irr::video::IVideoDriver * driver, irr::scene::ISceneManager* smgr)
{
#ifdef HC1_OGLES
	if ( !driver || !smgr )
		return;

	ClearES2Shaders();

	video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
	if ( !gpu )
	{
		LOG.Error("error: no gpu programming services\n");
		return;
	}

	hc1::COGLES2MaterialSolidCB* solidCb = new hc1::COGLES2MaterialSolidCB();
	LoadES2Shader(gpu, config.MakeFilenameShader("ES2Solid.vsh").c_str(),
	              config.MakeFilenameShader("ES2Solid.fsh").c_str(),
	              solidCb, video::EMT_SOLID);

	hc1::COGLES2MaterialLightmapCB* lightmapCb = new hc1::COGLES2MaterialLightmapCB();
	LoadES2Shader(gpu, config.MakeFilenameShader("ES2Solid2.vsh").c_str(),
			      config.MakeFilenameShader("ES2LightmapModulate.fsh").c_str(),
			      lightmapCb, video::EMT_LIGHTMAP_LIGHTING_M2);

	hc1::COGLES2MaterialTransparentAlphaChannelCB* transparentAlphaChannelCb = new hc1::COGLES2MaterialTransparentAlphaChannelCB();
	LoadES2Shader(gpu, config.MakeFilenameShader("ES2SolidTransparent.vsh").c_str(),
			      config.MakeFilenameShader("ES2TransparentAlphaChannel.fsh").c_str(),
			      transparentAlphaChannelCb, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

	hc1::COGLES2MaterialTransparentAddColorCB* transparentAddColorCb = new hc1::COGLES2MaterialTransparentAddColorCB();
	LoadES2Shader(gpu, config.MakeFilenameShader("ES2SolidTransparent.vsh").c_str(),
			config.MakeFilenameShader("ES2TransparentAddColor.fsh").c_str(),
			transparentAddColorCb, video::EMT_TRANSPARENT_ADD_COLOR);

	hc1::COGLES2MaterialTransparentVertexAlphaCB* transparentVertexAlphaCb = new hc1::COGLES2MaterialTransparentVertexAlphaCB();
	LoadES2Shader(gpu, config.MakeFilenameShader("ES2SolidTransparent.vsh").c_str(),
			      config.MakeFilenameShader("ES2TransparentVertexAlpha.fsh").c_str(),
			      transparentVertexAlphaCb, video::EMT_TRANSPARENT_VERTEX_ALPHA);
#endif // HC1_OGLES
}

void IrrlichtManager::ClearES2Shaders()
{
#ifdef HC1_OGLES
	mES2ShaderMaterials.clear();

	for ( ES2ShaderIter it = mES2Shaders.begin(); it != mES2Shaders.end(); ++it )
	{
		it->second->drop();
	}
	mES2Shaders.clear();
#endif // HC1_OGLES
}

bool IrrlichtManager::Init(const Config& config)
{
	if ( !mIrrlichtDevice )
		return false;

	mVideoDriver = mIrrlichtDevice->getVideoDriver();
	if ( !mVideoDriver )
        return false;

    mVideoDriver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT, config.GetUseTex16Bit());
    mVideoDriver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, config.GetUseTex32Bit());
	mVideoDriver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY, config.GetUseTexQuality());
	mVideoDriver->setTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_SPEED, config.GetUseTexSpeed());
	mVideoDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, config.GetUseMipMaps());

//	IGUIEnvironment* env = mIrrlichtDevice->getGUIEnvironment();
    mSceneManager = mIrrlichtDevice->getSceneManager();
    mSceneManager->setShadowColor(video::SColor(220,0,0,0));

    // add .lmo file loader (own format which adds lightmaps to maya .obj)
	irr::scene::CLMOMeshFileLoader* lmoMeshFileLoader = new scene::CLMOMeshFileLoader(mSceneManager, mIrrlichtDevice->getFileSystem(), mIrrlichtDevice->getTimer());
	mSceneManager->addExternalMeshLoader(lmoMeshFileLoader);

    mCameraMaya = mSceneManager->addCameraSceneNodeMaya();
    mCameraMaya->setNearValue( config.GetNearClipping() );
    mCameraMaya->setFarValue( config.GetFarClipping() );
    mCameraFPS = mSceneManager->addCameraSceneNodeFPS(0,100.0f,1200.0f);
    mCameraFPS->setNearValue( config.GetNearClipping() );
    mCameraFPS->setFarValue( config.GetFarClipping() );
    mCameraGame = mSceneManager->addCameraSceneNode();
    mCameraGame->setNearValue( config.GetNearClipping() );
    mCameraGame->setFarValue( config.GetFarClipping() );
    mCameraEditor = mSceneManager->addCameraSceneNode();
    mCameraEditor->setNearValue( config.GetNearClipping() );
    mCameraEditor->setFarValue( config.GetFarClipping() );
    mCameraGui = mSceneManager->addCameraSceneNode();
    mCameraGui->setFarValue( config.GetFarClipping() );

#ifdef _IRR_ANDROID_PLATFORM_
	// The Android assets file-system does not know which sub-directories it has (blame google).
	// So we have to add all sub-directories in assets manually. Otherwise we could still open the files,
	// but existFile checks will fail (which are for example needed by getFont).
	HC1_PROFILE(s32 p1=getProfiler().add(L"asset-addDir", L"STARTUP");)
	for ( u32 i=0; i < mIrrlichtDevice->getFileSystem()->getFileArchiveCount(); ++i )
	{
		io::IFileArchive* archive = mIrrlichtDevice->getFileSystem()->getFileArchive(i);
		if ( archive->getType() == io::EFAT_ANDROID_ASSET )
		{
			HC1_PROFILE(CProfileScope ps1(p1);)
			archive->addDirectoryToFileList( config.GetPathMedia().c_str() );
			archive->addDirectoryToFileList( config.GetPathTextures().c_str() );
			archive->addDirectoryToFileList( config.GetPathSounds().c_str() );
			archive->addDirectoryToFileList( config.GetPathMusicMenu().c_str() );
			archive->addDirectoryToFileList( config.GetPathMusicGame().c_str() );
			archive->addDirectoryToFileList( config.GetPathLevels().c_str() );
			archive->addDirectoryToFileList( config.GetPathUI().c_str() );
			archive->addDirectoryToFileList( config.GetPathModels().c_str() );
			break;
		}
	}
#endif

	scene::IMeshTextureLoader * objTextureLoader = NULL;
	scene::IMeshTextureLoader * xTextureLoader = NULL;
	scene::IMeshLoader * xMeshLoader = NULL;
	for ( u32 i=0; i<mSceneManager->getMeshLoaderCount(); ++i )
	{
		scene::IMeshLoader * loader = mSceneManager->getMeshLoader(i);
		scene::IMeshTextureLoader * textureLoader = loader ? loader->getMeshTextureLoader() : NULL;
		if ( textureLoader )
		{
			if ( loader->isALoadableFileExtension(".obj") )
			{
				objTextureLoader = textureLoader;
			}
			if ( loader->isALoadableFileExtension(".x") )
			{
				xMeshLoader = loader;
				xTextureLoader = textureLoader;
			}
			textureLoader->setTexturePath( io::path(config.GetPathTextures().c_str()) );
		}
	}
	// We use a thin wrapper around the file texture loaders
	if ( xTextureLoader && xMeshLoader )
	{
		// .x is actually hardly worth it - saving maybe 1MB when using lowest texture resolution.
		mDynamicMeshTextureLoader = new MeshTextureLoader(xTextureLoader, mVideoDriver);
		mDynamicMeshTextureLoader->downscaleTexture(config.GetTextureDownscaleDivider());
		xMeshLoader->setMeshTextureLoader(mDynamicMeshTextureLoader);
	}
	if ( objTextureLoader && lmoMeshFileLoader )
	{
		mStaticMeshTextureLoader = new MeshTextureLoader(objTextureLoader, mVideoDriver);
		mStaticMeshTextureLoader->downscaleTexture(config.GetTextureDownscaleDivider());
		if ( lmoMeshFileLoader )
			lmoMeshFileLoader->setMeshTextureLoader(mStaticMeshTextureLoader);
	}
	else
	{
		LOG.DebugLn("Found no texture loader to use for our lmo loader", "");
	}

	lmoMeshFileLoader->drop();

    return true;
}

void IrrlichtManager::ShutDownEventReceiver()
{
	if ( mIrrlichtDevice )
	{
		mIrrlichtDevice->clearSystemMessages();
		mIrrlichtDevice->setEventReceiver(NULL);
		delete mEventReceiver;
		mEventReceiver = NULL;
	}
}

void IrrlichtManager::InitVideoModes(const Config& config, irr::IrrlichtDevice* device)
{
	mVideoModes.clear();
	if ( !device )
		return;

    // get the available video modes
    video::IVideoModeList *videoModes = device->getVideoModeList();
    for ( s32 m = 0; m < videoModes->getVideoModeCount(); ++m)
    {
        VideoMode mode;
        mode.mResolution = videoModes->getVideoModeResolution(m);
        mode.mDepth = videoModes->getVideoModeDepth(m);

        if ( mode.mResolution.Width > 600
        &&  GetVideoModeIndex(mode.mResolution.Width, mode.mResolution.Height ) < 0 )
        {
            mVideoModes.push_back(mode);
        }
    }
    if ( !mVideoModes.size() )
    {
    	LOG.Info(L"Found no video modes so just adding some default modes\n");
        // some default modes
        VideoMode mode;
        mode.mDepth = 32;
        mode.mResolution = core::dimension2d<s32>(640,480);
        mVideoModes.push_back(mode);
        mode.mResolution = core::dimension2d<s32>(800,600);
        mVideoModes.push_back(mode);
        mode.mResolution = core::dimension2d<s32>(1024,768);
        mVideoModes.push_back(mode);
        mode.mResolution = core::dimension2d<s32>(1152,864);
        mVideoModes.push_back(mode);
        mode.mResolution = core::dimension2d<s32>(1280,1024);
        mVideoModes.push_back(mode);
    }
    // Check if resolution from config exists and add it otherwise
    // We do that so users can set "strange" resolutions by config it they wish to do so
    if ( GetVideoModeIndex( config.GetResolutionX(), config.GetResolutionY()) < 0 )
    {
    	LOG.Info(L"We don't have config resolution yet, so we add that\n");
        VideoMode mode;
        mode.mDepth = 32;
        mode.mResolution = core::dimension2d<s32>(config.GetResolutionX(),config.GetResolutionY());
        mVideoModes.push_back(mode);
    }

	LOG.LogLn(LP_DEBUG, "Number of video modes: ", (int)mVideoModes.size());
	for ( unsigned int m= 0; m < mVideoModes.size(); ++m)
	{
		LOG.LogLn(LP_DEBUG, "width: ", (int)mVideoModes[m].mResolution.Width);
		LOG.LogLn(LP_DEBUG, "height: ", (int)mVideoModes[m].mResolution.Height);
		LOG.LogLn(LP_DEBUG, "depth: ", mVideoModes[m].mDepth );
	}
}

int IrrlichtManager::GetVideoModeIndex( unsigned int width_, unsigned int height_) const
{
    for ( unsigned int m= 0; m < mVideoModes.size(); ++m)
    {
        if (    mVideoModes[m].mResolution.Width == width_
            &&  mVideoModes[m].mResolution.Height == height_ )
        {
            return (int)m;
        }
    }
    return -1;
}

ITimer* IrrlichtManager::GetIrrlichtTimer()
{
    if ( mIrrlichtDevice )
        return mIrrlichtDevice->getTimer();
    return NULL;
}

scene::ICameraSceneNode* IrrlichtManager::GetActiveCamera() const
{
    if ( !mSceneManager )
        return NULL;
    return mSceneManager->getActiveCamera();
}

scene::IAnimatedMesh *IrrlichtManager::GetMeshInCache(const irr::io::path &path_)
{
    if (path_.size() == 0)
        return NULL;

    scene::IMeshCache * cache = mSceneManager->getMeshCache();
    assert(cache);

    for ( u32 i=0; i < cache->getMeshCount(); ++i )
    {
		if ( path_.equals_ignore_case( cache->getMeshName(i).getPath() ) )
            return cache->getMeshByIndex(i);
    }
    return 0;
}

bool IrrlichtManager::IsMeshInCache(const irr::io::path &path_)
{
    return GetMeshInCache(path_) ? true : false;
}

void IrrlichtManager::SetMaterialTypeByTextureName(irr::scene::IAnimatedMesh* animatedMesh_, video::E_MATERIAL_TYPE type_, const char *label_)
{
    if ( !animatedMesh_ || !label_ || !strlen(label_))
        return;

	// Note that materials are frame-independent in animated meshes (at least in all formats we use so far).
	// That's why we can just get the meshes for the first frame and they will have the same materials as the rest
	// of the frames.

	scene::IMesh * mesh = animatedMesh_->getMesh(0);

    for ( u32 i=0; i < mesh->getMeshBufferCount() ; ++i )
    {
    	scene::IMeshBuffer * meshBuffer = mesh->getMeshBuffer(i);
        video::SMaterial & material = meshBuffer->getMaterial();

        if ( material.TextureLayer[0].Texture )
        {
			video::ITexture * texture = material.TextureLayer[0].Texture;	// variable for easier debugging
            std::string textureName(texture->getName().getPath().c_str());
            ExtFile::ErasePathFromFilename(textureName); // otherwise behaviour would depend on the pathname.
            ExtStr::ToLower(textureName);
            if ( strstr(textureName.c_str(), label_) )
            {
                //LOG.DebugLn("Setting new material for", material.TextureLayer[0].Texture->getName().getPath().c_str() );
                material.MaterialType = type_;
            }
        }
    }
}

void IrrlichtManager::SetEmissiveByTextureName(scene::ISceneNode* node_, int red_, int green_, int blue_, const char *label_)
{
	// TODO: That looks pretty hacky. And way too slow for realtime - guess I coded that when time run out.
	// It's used for the break-lights of the hovers.

    if ( !node_ || !label_ || !strlen(label_))
        return;

    for ( u32 i=0; i < node_->getMaterialCount(); ++i )
    {
        video::SMaterial & material = node_->getMaterial(i);
        if ( material.TextureLayer[0].Texture )
        {
            std::string textureName(material.TextureLayer[0].Texture->getName().getPath().c_str());
            ExtFile::ErasePathFromFilename(textureName); // otherwise behaviour can depend on the pathname. This did actually happen...
            if ( strstr(textureName.c_str(), label_) )
            {
                material.EmissiveColor.setRed(red_);
                material.EmissiveColor.setGreen(green_);
                material.EmissiveColor.setBlue(blue_);
            }
        }
    }
}

irr::u32 IrrlichtManager::getMeshVertexCount(const irr::scene::IMesh* mesh) const
{
	u32 result = 0;
	for ( u32 i=0; i<mesh->getMeshBufferCount(); ++i )
	{
		result += mesh->getMeshBuffer(i)->getVertexCount();
	}
	return result;
}

void IrrlichtManager::SetRealMaterial(video::E_MATERIAL_TYPE &type)
{
	if ( mES2ShaderMaterials.empty() )
		return;

	ES2ShaderMaterialIter it = mES2ShaderMaterials.find((s32)type);
	if ( it != mES2ShaderMaterials.end() )
	{
		type = (video::E_MATERIAL_TYPE)it->second;
	}
	else
	{
		// int dbg = 1;
	}
}

void IrrlichtManager::SetES2ShaderMaterials(irr::scene::IAnimatedMesh* animatedMesh)
{
    if ( !animatedMesh || mES2ShaderMaterials.empty() )
        return;

	scene::IMesh * mesh = animatedMesh->getMesh(0);
    for ( u32 i=0; i < mesh->getMeshBufferCount() ; ++i )
    {
    	scene::IMeshBuffer * meshBuffer = mesh->getMeshBuffer(i);
        video::SMaterial & material = meshBuffer->getMaterial();
        SetRealMaterial(material.MaterialType);
	}
}

void IrrlichtManager::SetShadersUseDynamicLight(bool enable)
{
#ifdef HC1_OGLES
	for ( ES2ShaderIter it = mES2Shaders.begin(); it != mES2Shaders.end(); ++it )
	{
		static_cast<hc1::COGLES2MaterialBaseCB*>(it->second)->SetUseDynamicLight(enable);
	}
#endif
}

void IrrlichtManager::setMaterialFlags(const Config& config, irr::scene::IAnimatedMesh* animatedMesh)
{
    if ( !animatedMesh )
        return;

	// Note that materials are frame-independent in animated meshes (at least in all formats we use so far).
	// That's why we can just get the meshes for the first frame and they will have the same materials as the rest
	// of the frames.

	scene::IMesh * mesh = animatedMesh->getMesh(0);

	mesh->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, config.GetAnisotropic() );
	mesh->setMaterialFlag(video::EMF_BILINEAR_FILTER, config.GetBiLinear());
	mesh->setMaterialFlag(video::EMF_TRILINEAR_FILTER, config.GetTriLinear());
	mesh->setMaterialFlag(video::EMF_GOURAUD_SHADING, config.GetGouraud() );
}

scene::IAnimatedMesh* IrrlichtManager::LoadMesh(const Config& config, const c8* filename, bool animated)
{
    if ( !filename || !strlen(filename) )
        return NULL;

	u32 beginTime = GetIrrlichtTimer()->getRealTime();

   	io::path extension;
	core::getFileNameExtension(extension, filename);
	extension.make_lower();

	// some models might want special handling
	// bool isLmo = (extension == ".lmo");
	bool isX = (extension == ".x");

	// load a mesh into the engine
	bool wasCached = IsMeshInCache(filename);
	scene::IAnimatedMesh* m = mSceneManager->getMesh(filename);
	if (!m)
	{
		return NULL;
	}
	//fprintf(stderr, "loaded mesh %s\n", filename);

	if ( !wasCached )
	{
		LOG.DebugLn("Time meshloading: ", core::stringc(GetIrrlichtTimer()->getRealTime() - beginTime).c_str());

		// Let's fix some stuff which couldn't easily be set in the modeling tool (or would have gotten lost on export)
		SetMaterialTypeByTextureName(m, video::EMT_TRANSPARENT_ALPHA_CHANNEL, "alpha");
		SetMaterialTypeByTextureName(m, video::EMT_SOLID, "race_light01");
		SetMaterialTypeByTextureName(m, video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF, "mask");

		if ( isX && animated )
		{
			SetSpecularMaterial(m, 0, 0, 0);
		}

		setMaterialFlags(config, m);

		// must be after SetMaterialTypeByTextureName as it replaces those materials again
		SetES2ShaderMaterials(m);

		if ( !animated )
		{
			// Reduce number of vertices by welding those which are identical.
			// TODO: There might be a way to pre-process that, but probably difficult without changing the file-format.
			// 		Because I have the suspicion those are create by the obj loader and are not caused by duplicates in the obj file itself (it also has some - but I don't think that many)
			//		So maybe another solution would be fixing that in the obj loader (or maybe it's fixed there already just not in the lmo loader? Not tried.)
			u32 weldTime = GetIrrlichtTimer()->getRealTime();
			//scene::IMesh * weldedMesh = mSceneManager->getMeshManipulator()->createMeshWelded(m->getMesh(0));	// too slow
			scene::IMesh * weldedMesh = ExtIrr::CreateMeshWelded(m->getMesh(0));
			LOG.DebugLn("Time createMeshWelded: ", core::stringc(GetIrrlichtTimer()->getRealTime() - weldTime).c_str());
			weldTime = GetIrrlichtTimer()->getRealTime();
			scene::IAnimatedMesh*  m2 = mSceneManager->getMeshManipulator()->createAnimatedMesh(weldedMesh, m->getMeshType ());
			weldedMesh->drop();
			weldedMesh = NULL;
			LOG.DebugLn("Time createAnimatedMesh: ", core::stringc(GetIrrlichtTimer()->getRealTime() - weldTime).c_str());
			if ( m2 )
			{
				LOG.DebugLn("Old num vertices: ", core::stringc(getMeshVertexCount(m)).c_str());
				LOG.DebugLn("New num vertices: ", core::stringc(getMeshVertexCount(m2)).c_str());
				scene::IMeshCache * meshCache = mSceneManager->getMeshCache ();
				meshCache->removeMesh(m);
				meshCache->addMesh(filename, m2);
				m = m2;
				m2->drop();
				m2 = NULL;
			}

			// In our case only the .X files are animated. So all others can be static
			m->getMesh(0)->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_VERTEX_AND_INDEX);
		}
		else
		{
			// Don't actually notice any speed difference when setting this.
			m->getMesh(0)->setHardwareMappingHint(scene::EHM_STATIC, scene::EBT_INDEX);
		}
    }

    return m;
}


scene::ISceneNode* IrrlichtManager::LoadModel(const Config& config, const c8* fileModel, scene::ITriangleSelector** createSelector)
{
	u32 beginTime = GetIrrlichtTimer()->getRealTime();

	scene::IAnimatedMesh* m = LoadMesh(config, fileModel, false);
	if (!m)
	{
		return NULL;
	}

	// set default material properties
    scene::ISceneNode* meshResult = NULL;

	// Octree might still make sense if EHM_STATIC doesn't work on a platform.
	// But needs profiling and a way to figure out _if_ EHM_STATIC does not work (unfortunately some Android devices seem to lie about it).
	// meshResult = mSceneManager->addOctreeSceneNode(m, /*parent*/NULL, /*id*/-1, /*minimalPolysPerNode*/ 128);
	meshResult = mSceneManager->addMeshSceneNode(m);

    if ( createSelector )
    {
        *createSelector = meshResult->getTriangleSelector();
        if ( *createSelector )
        {
			(*createSelector)->grab();
        }
        else
        {
        	LOG.LogLn(LP_INFO, "mesh had no triangle selector: ", fileModel);

        	// We got no loopings, so we can be sure we never collide against triangles with normals pointing downwards.
        	// So we can kick them out and only collide against the rest.
			scene::IMesh * reducedMesh = ExtIrr::CreateSubMeshForNormal(m->getMesh(0), irr::core::vector3df(0, 1, 0), -0.5f);

			// create some fitting triangle-selector. Best choice depends a lot on level layout.
			//*createSelector = mSceneManager->createOctreeTriangleSelector(reducedMesh, meshResult, 32 );

			float cellSize = config.GetHoverRadius() * 2.f + 1.f;	// We know we use radius*2 in physics - so this way never more than 4 cells are checked
			*createSelector = new GridTriangleSelector(reducedMesh, meshResult, cellSize);

			reducedMesh->drop();
			LOG.DebugLn("Time created selector: ", core::stringc(GetIrrlichtTimer()->getRealTime() - beginTime).c_str());
        }
    }

    meshResult->setName(fileModel);	// Having the name makes debugging a lot easier
	meshResult->setAutomaticCulling(scene::EAC_BOX);	// EAC_OFF EAC_BOX EAC_FRUSTUM_BOX EAC_FRUSTUM_SPHERE
	meshResult->setDebugDataVisible(false);

    // LOG.DebugLn("Time LoadModel sum: ", core::stringc(GetIrrlichtTimer()->getRealTime() - beginTime).c_str());

	return meshResult;
}

scene::IAnimatedMeshSceneNode* IrrlichtManager::LoadAnimatedModel(const Config& config, const c8* fileModel)
{
	// load a model-mesh into the engine
	scene::IAnimatedMesh* m = LoadMesh(config, fileModel, true);
	if (!m)
	{
		return NULL;
	}
    //printf("loaded model %s\n", filename_);

	// set default material properties
    scene::IAnimatedMeshSceneNode* meshResult = NULL;
	meshResult = mSceneManager->addAnimatedMeshSceneNode(m);

	meshResult->setDebugDataVisible(false);
    meshResult->setAutomaticCulling(scene::EAC_OFF);	// EAC_OFF EAC_BOX EAC_FRUSTUM_BOX EAC_FRUSTUM_SPHERE

	return meshResult;
}

void IrrlichtManager::Quit()
{
	ShutDownEventReceiver();	// usually already down here.

	if (mStaticMeshTextureLoader)
	{
		mStaticMeshTextureLoader->drop();
		mStaticMeshTextureLoader = 0;
	}
	if (mDynamicMeshTextureLoader)
	{
		mDynamicMeshTextureLoader->drop();
		mDynamicMeshTextureLoader = 0;
	}

	ClearES2Shaders();
    if ( mIrrlichtDevice )
    {
        mVideoDriver = NULL;
        mSceneManager = NULL;

        mIrrlichtDevice->closeDevice();
        mIrrlichtDevice->clearSystemMessages();
        mIrrlichtDevice->drop();
        mIrrlichtDevice = NULL;
    }
}

void IrrlichtManager::ForceIrrlichtUpdate()
{
    LOG.Debug("ForceIrrlichtUpdate\n");
    if ( mIrrlichtDevice && mIrrlichtDevice->run() && mVideoDriver && mSceneManager )
    {
        LOG.Debug("beginScene\n");
        mVideoDriver->beginScene(true, true, video::SColor(150,50,50,50));
        mSceneManager->drawAll();
        if ( mIrrlichtDevice->getGUIEnvironment() )
        {
            mIrrlichtDevice->getGUIEnvironment()->drawAll();
        }
        LOG.Debug("endScene\n");
        mVideoDriver->endScene();
    }
    LOG.Debug("ForceIrrlichtUpdate done\n");
}

void IrrlichtManager::SetCameraMaya()
{
    if ( GetActiveCamera() == mCameraFPS
        || GetActiveCamera() == mCameraGame )
    {
        core::vector3df oldPos(GetActiveCamera()->getPosition());
        core::vector3df oldRot(GetActiveCamera()->getRotation());

        mCameraMaya->setPosition(oldPos);
        mCameraMaya->setRotation(oldRot);
    }
    if ( mSceneManager)
    {
        mSceneManager->setActiveCamera(mCameraMaya);
    }

    if ( mIrrlichtDevice )
    {
        ICursorControl* cursor = mIrrlichtDevice->getCursorControl();
        cursor->setVisible(true);
    }
}

void IrrlichtManager::SetCameraFPS()
{
    if ( GetActiveCamera() == mCameraMaya )
    {
        core::vector3df oldPos(GetActiveCamera()->getPosition());
        core::vector3df oldRot(GetActiveCamera()->getRotation());

        mCameraFPS->setPosition(oldPos);
        mCameraFPS->setRotation(oldRot);
    }
    if ( mSceneManager)
    {
        mSceneManager->setActiveCamera(mCameraFPS);
    }

    if ( mIrrlichtDevice )
    {
        ICursorControl* cursor = mIrrlichtDevice->getCursorControl();
        cursor->setVisible(false);
    }
}

void IrrlichtManager::SetCameraGame(bool hideCursor)
{
    if ( mSceneManager)
    {
        mSceneManager->setActiveCamera(mCameraGame);
    }

    if ( mIrrlichtDevice && hideCursor)
    {
        ICursorControl* cursor = mIrrlichtDevice->getCursorControl();
        if(cursor)
			cursor->setVisible(false);
    }
}

void IrrlichtManager::SetCameraGui()
{
    if ( GetActiveCamera() )
    {
        mCameraGui->setPosition( GetActiveCamera()->getPosition() );
        mCameraGui->setUpVector( GetActiveCamera()->getUpVector() );
        mCameraGui->setTarget( GetActiveCamera()->getTarget() );
    }

    if ( mSceneManager)
    {
        mSceneManager->setActiveCamera(mCameraGui);
    }

    if ( mIrrlichtDevice )
    {
        ICursorControl* cursor = mIrrlichtDevice->getCursorControl();
        if ( cursor )
        {
			cursor->setVisible(true);
		}
    }
}

void IrrlichtManager::SetCameraEditor()
{
    if ( mSceneManager)
    {
        mSceneManager->setActiveCamera(mCameraEditor);
    }

    if ( mIrrlichtDevice )
    {
        ICursorControl* cursor = mIrrlichtDevice->getCursorControl();
        if(cursor)
			cursor->setVisible(true);
    }
}

// That was before Irrlicht offered it
scene::SMeshBuffer* IrrlichtManager::CreateBoxMeshBuffer(const core::aabbox3d<f32> &box_)
{
    scene::SMeshBuffer * buffer = new scene::SMeshBuffer();
    buffer->Material = mDefaultEditorMaterial;

//      /4--------/0
//     /  |      / |
//    /   |     /  |
//    6---------2  |
//    |   5- - -| -1
//    |  /      |  /
//    |/        | /
//    7---------3/

    core::vector3df edges[8];
    box_.getEdges(edges);

    for ( int i=0; i<8; ++i)
    {
        video::S3DVertex v;
        v.Pos = edges[i];
        v.Color = video::SColor(0xffffffff);
        buffer->Vertices.push_back(v);
    }

    // top
    buffer->Indices.push_back(6);
    buffer->Indices.push_back(2);
    buffer->Indices.push_back(0);
    buffer->Indices.push_back(0);
    buffer->Indices.push_back(4);
    buffer->Indices.push_back(6);

    // front
    buffer->Indices.push_back(7);
    buffer->Indices.push_back(3);
    buffer->Indices.push_back(2);
    buffer->Indices.push_back(2);
    buffer->Indices.push_back(6);
    buffer->Indices.push_back(7);

    // right
    buffer->Indices.push_back(2);
    buffer->Indices.push_back(3);
    buffer->Indices.push_back(1);
    buffer->Indices.push_back(1);
    buffer->Indices.push_back(0);
    buffer->Indices.push_back(2);

    // back
    buffer->Indices.push_back(0);
    buffer->Indices.push_back(1);
    buffer->Indices.push_back(5);
    buffer->Indices.push_back(5);
    buffer->Indices.push_back(4);
    buffer->Indices.push_back(0);

    // left
    buffer->Indices.push_back(5);
    buffer->Indices.push_back(7);
    buffer->Indices.push_back(6);
    buffer->Indices.push_back(6);
    buffer->Indices.push_back(4);
    buffer->Indices.push_back(5);

    // bottom
    buffer->Indices.push_back(1);
    buffer->Indices.push_back(3);
    buffer->Indices.push_back(7);
    buffer->Indices.push_back(7);
    buffer->Indices.push_back(5);
    buffer->Indices.push_back(1);

    scene::IMeshManipulator * mm = mSceneManager->getMeshManipulator();
    mm->recalculateNormals(buffer);

    buffer->recalculateBoundingBox();

    return buffer;
}

scene::SMeshBuffer* IrrlichtManager::CreateExtrudedPolyLineMeshBuffer(int numVertices_, const core::vector3df * vertices_, const core::vector3df &extrude_)
{
    if ( numVertices_ < 3 || !vertices_ )
        return NULL;

    scene::SMeshBuffer * buffer = new scene::SMeshBuffer();
    buffer->Material = mDefaultEditorMaterial;

    // sides
    for ( int i=0; i<numVertices_; ++i)
    {
        video::S3DVertex v;
    	v.Color = video::SColor(0xffffffff);
        v.Pos = vertices_[i];
        buffer->Vertices.push_back(v);
        v.Pos = vertices_[i] + extrude_;
        buffer->Vertices.push_back(v);

        buffer->Indices.push_back( ((i+1)*2+1) % (numVertices_*2) );
        buffer->Indices.push_back( ((i+1)*2) % (numVertices_*2) );
        buffer->Indices.push_back(i*2);

        buffer->Indices.push_back(i*2);
        buffer->Indices.push_back(i*2+1);
        buffer->Indices.push_back( ((i+1)*2+1) % (numVertices_*2) );
    }


    for ( int i=1; i<numVertices_-1; ++i)
    {
        buffer->Indices.push_back(0);
        buffer->Indices.push_back(i*2);
        buffer->Indices.push_back(i*2+2);

        buffer->Indices.push_back(i*2+3);
        buffer->Indices.push_back(i*2+1);
        buffer->Indices.push_back(1);
    }

    scene::IMeshManipulator * mm = mSceneManager->getMeshManipulator();
    mm->recalculateNormals(buffer);

    buffer->recalculateBoundingBox();

    return buffer;
}

scene::SMeshBuffer* IrrlichtManager::CreateArrowMeshBuffer(float length_, float width_, float height_)
{
    core::vector3df vertices[7];
    vertices[0].set(0.f, -height_*0.5f, length_*0.5f);
    vertices[1].set(-width_*0.5f, -height_*0.5f, 0.f);
    vertices[2].set(-width_*0.25f, -height_*0.5f, 0.f);
    vertices[3].set(-width_*0.25f, -height_*0.5f, -length_*0.5f);
    vertices[4].set(width_*0.25f, -height_*0.5f, -length_*0.5f);
    vertices[5].set(width_*0.25f, -height_*0.5f, 0.f);
    vertices[6].set(width_*0.5f, -height_*0.5f, 0.f);

    return CreateExtrudedPolyLineMeshBuffer(7, vertices, core::vector3df(0, height_, 0));
}


scene::SMeshBuffer* IrrlichtManager::CreateQuadradMeshBuffer(const core::vector3df &leftTop_, const core::vector3df &rightTop_, const core::vector3df &leftBottom_, const core::vector3df &rightBottom_)
{
    scene::SMeshBuffer * buffer = new scene::SMeshBuffer();
    buffer->Material = mDefaultEditorMaterial;

    video::S3DVertex v;
    v.Color = video::SColor(0xffffffff);
    v.Pos = leftTop_;
    buffer->Vertices.push_back(v);
    v.Pos = rightTop_;
    buffer->Vertices.push_back(v);
    v.Pos = rightBottom_;
    buffer->Vertices.push_back(v);
    v.Pos = leftBottom_;
    buffer->Vertices.push_back(v);

    buffer->Indices.push_back(0);
    buffer->Indices.push_back(1);
    buffer->Indices.push_back(2);
    buffer->Indices.push_back(2);
    buffer->Indices.push_back(3);
    buffer->Indices.push_back(0);

    scene::IMeshManipulator * mm = mSceneManager->getMeshManipulator();
    mm->recalculateNormals(buffer);

    buffer->recalculateBoundingBox();

    return buffer;
}

void IrrlichtManager::MakeGhost(scene::ISceneNode* node_)
{
    if ( !node_ )
        return;

	video::E_MATERIAL_TYPE solid = video::EMT_SOLID;
	video::E_MATERIAL_TYPE transparent = video::EMT_TRANSPARENT_ADD_COLOR;

	SetRealMaterial(solid);
	SetRealMaterial(transparent);

    for ( u32 i=0; i < node_->getMaterialCount(); ++i )
    {
        video::SMaterial & 	mat = node_->getMaterial(i);

        if ( mat.MaterialType == solid )
        {
			mat.MaterialType = transparent;
        }
    }

    const core::list<scene::ISceneNode*> & children = node_->getChildren();
    core::list<scene::ISceneNode*>::ConstIterator it = children.begin();
    for (; it != children.end(); ++it)
    {
        MakeGhost(*it);
    }
}

void IrrlichtManager::SetNodeAndChildsAmbientMaterial(scene::ISceneNode* node_, int red_, int green_, int blue_, int ignore_)
{
    if ( !node_ )
        return;

    for ( u32 i=0; i < node_->getMaterialCount(); ++i )
    {
        video::SMaterial & 	mat = node_->getMaterial(i);
        if ( (int)mat.AmbientColor.getRed() != ignore_
            && (int)mat.AmbientColor.getGreen() != ignore_
            && (int)mat.AmbientColor.getBlue() != ignore_ )
        {
            mat.AmbientColor.setRed(red_);
            mat.AmbientColor.setGreen(green_);
            mat.AmbientColor.setBlue(blue_);
        }
    }

    const core::list<scene::ISceneNode*> & children = node_->getChildren();
    core::list<scene::ISceneNode*>::ConstIterator it = children.begin();
    for (; it != children.end(); ++it)
    {
        SetNodeAndChildsAmbientMaterial(*it, red_, green_, blue_, ignore_);
    }
}

void IrrlichtManager::SetSpecularMaterial(irr::scene::IAnimatedMesh* animatedMesh, int red_, int green_, int blue_, int ignore_)
{
    if ( !animatedMesh )
        return;

	scene::IMesh * mesh = animatedMesh->getMesh(0);
    for ( u32 i=0; i < mesh->getMeshBufferCount() ; ++i )
    {
    	scene::IMeshBuffer * meshBuffer = mesh->getMeshBuffer(i);
        video::SMaterial & mat = meshBuffer->getMaterial();
        if ( (int)mat.SpecularColor.getRed() != ignore_
            && (int)mat.SpecularColor.getGreen() != ignore_
            && (int)mat.SpecularColor.getBlue() != ignore_ )
        {
            mat.SpecularColor.setRed(red_);
            mat.SpecularColor.setGreen(green_);
            mat.SpecularColor.setBlue(blue_);
        }
    }
}

void IrrlichtManager::SetDriverDrawMode()
{
    video::SMaterial material;
    material.Lighting = false;
    mVideoDriver->setTransform(video::ETS_WORLD, core::matrix4());
    mVideoDriver->setMaterial(material);
}

void IrrlichtManager::SetNodeOrChildVisibleByName(scene::ISceneNode* node_, const char *name_, bool visible_)
{
    if (!node_ || !name_ || !strlen(name_) )
        return;

    const char * nodeName = node_->getName();
    if ( nodeName && strlen(nodeName) && 0 == strcmp( nodeName, name_) )
    {
        node_->setVisible(visible_);
        return; //
    }

    const core::list<scene::ISceneNode*> & children = node_->getChildren();
    core::list<scene::ISceneNode*>::ConstIterator it = children.begin();
    for (; it != children.end(); ++it)
    {
        SetNodeOrChildVisibleByName(*it, name_, visible_);
    }
}

void IrrlichtManager::LogAllTexturesInformation(LogPriority priority)
{
    if ( !mVideoDriver )
        return;

    for ( u32 i=0; i < mVideoDriver->getTextureCount(); ++i )
    {
        video::ITexture * tex = mVideoDriver->getTextureByIndex(i);
        if ( tex )
        {
//            const core::dimension2d<s32>& origSize = tex->getOriginalSize();
            const core::dimension2d<u32>& size = tex->getSize();
            video::ECOLOR_FORMAT format = tex->getColorFormat();
            bool hasMips = tex->hasMipMaps();

            std::ostringstream info;
            info << i << "."
                    << " mips:" << hasMips
                    << " width:" << size.Width
                    << " height:" << size.Height
                    << " format:" << (int)format
                    << " Name:" << tex->getName().getPath().c_str()
                    << std::endl;
            LOG.Log(priority, info.str());
        }
    }
}

void IrrlichtManager::LogAllMeshInformation(LogPriority priority)
{
    if ( !mSceneManager )
        return;

    for ( u32 i=0; i < mSceneManager->getMeshCache()->getMeshCount(); ++i )
    {
        const c8 * filename = mSceneManager->getMeshCache()->getMeshName(i).getPath().c_str();
        if ( filename )
        {
            LOG.LogLn(priority, filename);
        }
    }
}

void IrrlichtManager::SendKeyEvent( irr::EKEY_CODE key_, bool pressedDown_, wchar_t character_)
{
	if ( mIrrlichtDevice && mIrrlichtDevice->getGUIEnvironment() )
    {
        SEvent irrEvent;
        irrEvent.EventType = irr::EET_KEY_INPUT_EVENT;
        irrEvent.KeyInput.PressedDown = pressedDown_;
        irrEvent.KeyInput.Control = false;
        irrEvent.KeyInput.Shift = false;
        irrEvent.KeyInput.Char = character_;
        irrEvent.KeyInput.Key = key_;

        if ( mEventReceiver )
			mEventReceiver->OnEvent(irrEvent);
        mIrrlichtDevice->getGUIEnvironment()->postEventFromUser(irrEvent);
    }
}
