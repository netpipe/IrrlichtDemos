// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef IRRLICHT_MANAGER_H
#define IRRLICHT_MANAGER_H

#include "logging_priorities.h"
#include <irrlicht.h>
#include <vector>
#include <map>

struct VideoMode
{
    irr::core::dimension2d<irr::u32> 	mResolution;
    irr::s32                     		mDepth;
};

struct android_app;
class Config;
class MeshTextureLoader;

// Interface to Irrlicht stuff
// Does also handle cameras
class IrrlichtManager
{
public:
    IrrlichtManager();
    ~IrrlichtManager();

	irr::IrrlichtDevice* CreateIrrlichtDevicePC(const Config& config);
	irr::IrrlichtDevice* CreateIrrlichtDeviceAndroid(const Config& config, android_app * systemData);

    bool Init(const Config& config);
    void Quit();

    // shutdown eventreceiver before the rest of the app so it doesn't try handling gui-events while the gui is getting removed already
    void ShutDownEventReceiver();

    void ForceIrrlichtUpdate();

    irr::IrrlichtDevice* GetIrrlichtDevice() const		{ return mIrrlichtDevice; }
    irr::video::IVideoDriver*  GetVideoDriver() const	{ return mVideoDriver; }
    irr::scene::ISceneManager* GetSceneManager() const 	{ return mSceneManager; }
    irr::IEventReceiver* GetEventReceiver() const     	{ return mEventReceiver; }
    irr::ITimer* GetIrrlichtTimer();

    irr::scene::IAnimatedMeshSceneNode* LoadAnimatedModel(const Config& config, const irr::c8* fn_);
    irr::scene::ISceneNode* LoadModel(const Config& config, const irr::c8* fileModel_, irr::scene::ITriangleSelector** createSelector);

    irr::scene::ICameraSceneNode* GetActiveCamera() const;
    irr::scene::ICameraSceneNode* GetGameCamera() const   { return mCameraGame; }
    irr::scene::ICameraSceneNode* GetEditorCamera() const { return mCameraEditor; }
    irr::scene::ICameraSceneNode* GetGuiCamera() const    { return mCameraGui; }
    void SetCameraMaya();
    void SetCameraFPS();
    void SetCameraGame(bool hideCursor);
    void SetCameraEditor();
    void SetCameraGui();

    irr::scene::SMeshBuffer* CreateBoxMeshBuffer(const irr::core::aabbox3d<irr::f32> &box_);
    // only convex polygons or polygons where all lines from the first point to other points are within the polygon
    irr::scene::SMeshBuffer* CreateExtrudedPolyLineMeshBuffer(int numVertices_, const irr::core::vector3df * vertices_, const irr::core::vector3df &extrude_);
    irr::scene::SMeshBuffer* CreateArrowMeshBuffer(float length_, float width_, float height_);
    irr::scene::SMeshBuffer* CreateQuadradMeshBuffer(const irr::core::vector3df &leftTop_, const irr::core::vector3df &rightTop_, const irr::core::vector3df &leftBottom_, const irr::core::vector3df &rightBottom_);

    // Set the material ambient color for this node and it's children.
    // Don't change the color of a material if one of it's ambient color values is set to ignore_
    void SetNodeAndChildsAmbientMaterial(irr::scene::ISceneNode* node_, int red_, int green_, int blue_, int ignore_=-1);
    void MakeGhost(irr::scene::ISceneNode* node_);

    // Change materials based on texture names
    // label_ has just to be a part of the texture name like "alpha_blueredsky" if you search for "alpha"
    // label_ is not case sensitive
    void SetMaterialTypeByTextureName(irr::scene::IAnimatedMesh* animatedMesh_, irr::video::E_MATERIAL_TYPE type_, const char *label_);
    void SetEmissiveByTextureName(irr::scene::ISceneNode* node_, int red_, int green_, int blue_, const char *label_);

    // set transformation and material so the draw3d-functions of the driver can be used
    void SetDriverDrawMode();

    void SetNodeOrChildVisibleByName(irr::scene::ISceneNode* node_, const char *name_, bool visible_);

    bool IsMeshInCache(const irr::io::path &path_);
    irr::scene::IAnimatedMesh * GetMeshInCache(const irr::io::path &path_);

    const std::vector<VideoMode>& GetVideoModes() const { return mVideoModes; }
    int GetVideoModeIndex( unsigned int width_, unsigned int height_) const;

    void LogAllTexturesInformation(LogPriority priority);
    void LogAllMeshInformation(LogPriority priority);

    // For shaders the old materials have to be replaced by shader-materials
    // NOTE: This can be kicked if Irrlicht ever allows _replacing_ material renderers
    void SetRealMaterial(irr::video::E_MATERIAL_TYPE &type);

    void SetShadersUseDynamicLight(bool enable);

    MeshTextureLoader* GetStaticMeshTextureLoader() const	{ return mStaticMeshTextureLoader; }
    MeshTextureLoader* GetDynamicMeshTextureLoader() const	{ return mDynamicMeshTextureLoader; }

	void SendKeyEvent( irr::EKEY_CODE key_, bool pressedDown_, wchar_t character_=0);

protected:

    irr::u32 getMeshVertexCount(const irr::scene::IMesh* mesh) const;

    void InitVideoModes(const Config& config, irr::IrrlichtDevice* device);

    void AddAllES2Shaders(const Config& config, irr::video::IVideoDriver * driver, irr::scene::ISceneManager* smgr);
    void ClearES2Shaders();
	void LoadES2Shader(irr::video::IGPUProgrammingServices* gpu, const irr::c8* vertexShaderName, const irr::c8* fragmentShaderName, irr::video::IShaderConstantSetCallBack * cb, irr::video::E_MATERIAL_TYPE baseMaterial);
    void SetES2ShaderMaterials(irr::scene::IAnimatedMesh* animatedMesh);

	irr::scene::IAnimatedMesh* LoadMesh(const Config& config, const irr::c8* filename, bool animated);

	void setMaterialFlags(const Config& config, irr::scene::IAnimatedMesh* animatedMesh);

	void SetSpecularMaterial(irr::scene::IAnimatedMesh* animatedMesh, int red_, int green_, int blue_, int ignore_=-1);

private:
    irr::IrrlichtDevice*             	mIrrlichtDevice;
    irr::video::IVideoDriver*        	mVideoDriver;
    irr::scene::ISceneManager*       	mSceneManager;
    irr::IEventReceiver*             	mEventReceiver;
    irr::scene::ICameraSceneNode*    	mCameraMaya;
    irr::scene::ICameraSceneNode*    	mCameraFPS;
    irr::scene::ICameraSceneNode*    	mCameraGame;
    irr::scene::ICameraSceneNode*    	mCameraEditor;
    irr::scene::ICameraSceneNode*    	mCameraGui;
    irr::video::SMaterial            	mDefaultEditorMaterial;
    std::vector<VideoMode>      		mVideoModes;

	typedef std::map<irr::s32, irr::s32>::iterator ES2ShaderMaterialIter;
    std::map<irr::s32, irr::s32> mES2ShaderMaterials;	// key is orig material id, value is shader material id
    typedef std::map<irr::s32, irr::video::IShaderConstantSetCallBack*>::iterator ES2ShaderIter;
    std::map<irr::s32, irr::video::IShaderConstantSetCallBack*>	mES2Shaders;	// key is shader material id

    MeshTextureLoader* mStaticMeshTextureLoader;
	MeshTextureLoader* mDynamicMeshTextureLoader;
};

#endif // IRRLICHT_MANAGER_H
