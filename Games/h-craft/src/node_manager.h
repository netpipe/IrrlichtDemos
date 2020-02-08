// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

// A wrapper to be able to load/save Irrlicht nodes to a custom XML format

#include "irrlicht_manager.h"
#include <string>
#include <vector>

class TiXmlElement;

enum NODE_TYPE
{
    NT_UNKNOWN,
    NT_MESH,
    NT_ANIMATED_MESH,
    NT_LIGHT,
    NT_PARTICLE,
    NT_SKYBOX,
};

enum ANIMATOR_TYPE
{
    AT_NONE,
    AT_ROTATION,
};

enum PARTICLE_AFFECTOR_TYPE
{
    PAT_NONE,
    PAT_FADE_OUT,
    PAT_GRAVITY,
};

enum PARTICLE_EMITTER_TYPE
{
    PET_NONE,
    PET_BOX,
    PET_POINT,
};

struct MaterialSettings
{
    MaterialSettings();

    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    irr::video::E_MATERIAL_TYPE  mMaterialType;
    std::string             mTexture1;
    bool                    mLighting;
    irr::f32                mMaterialTypeParam;
};

struct AnimatorSettings
{
    AnimatorSettings();
    virtual ~AnimatorSettings();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    ANIMATOR_TYPE mType;
};

struct AnimatorSettingsRotation : public AnimatorSettings
{
    AnimatorSettingsRotation();
    virtual ~AnimatorSettingsRotation();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    irr::core::vector3df mRotationPerSec;
};

struct NodeSettingsUnknown
{
    NodeSettingsUnknown();
    virtual ~NodeSettingsUnknown();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    NodeSettingsUnknown* AddChild(std::string name_, NODE_TYPE type_);
    NodeSettingsUnknown* GetChild(unsigned int index_);
    void RemoveChild(unsigned int index_);
    void RemoveAllChilds();
    unsigned int GetNumChilds() const { return mChildNodeSettings.size(); }

    // return true not only for exact type, but also for derived types
    virtual bool IsOfClassType(NODE_TYPE type_) { return type_ == NT_UNKNOWN; }

    NODE_TYPE mType;
    std::string mName;
    AnimatorSettings * mAnimatorSettings;

    std::vector<NodeSettingsUnknown*>   mChildNodeSettings;
};

struct NodeSettingsMesh : public NodeSettingsUnknown
{
    NodeSettingsMesh();
    virtual ~NodeSettingsMesh();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    virtual bool IsOfClassType(NODE_TYPE type_) { return type_ == NT_UNKNOWN || type_ == NT_MESH; }

    std::string mFile;

    irr::core::vector3df mCenter;
    irr::core::vector3df mRotation;
    irr::core::vector3df mScale;

    bool mBackfaceCulling;
};

struct NodeSettingsAnimatedMesh : public NodeSettingsMesh
{
    NodeSettingsAnimatedMesh();
    virtual ~NodeSettingsAnimatedMesh();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    virtual bool IsOfClassType(NODE_TYPE type_) { return type_ == NT_UNKNOWN || type_ == NT_MESH || type_ == NT_ANIMATED_MESH; }

    int  mAnimationSpeed; // fps
};

struct NodeSettingsLight : public NodeSettingsUnknown
{
    NodeSettingsLight();
    virtual ~NodeSettingsLight();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    virtual bool IsOfClassType(NODE_TYPE type_) { return type_ == NT_UNKNOWN || type_ == NT_LIGHT; }

    irr::video::E_LIGHT_TYPE mLightType;
    irr::core::vector3df mCenter;
    irr::core::vector3df mRotation;

    bool 	        mCastShadows;
    irr::f32 	    mRadius;
    //video::SColorf 	mAmbientColor;
 	irr::video::SColorf 	mDiffuseColor;
    irr::video::SColorf 	mSpecularColor;
};

struct ParticleEffectorSettings
{
    ParticleEffectorSettings();
    virtual ~ParticleEffectorSettings();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    PARTICLE_AFFECTOR_TYPE mType;
};

struct ParticleEffectorSettingsFadeOut : public ParticleEffectorSettings
{
    ParticleEffectorSettingsFadeOut();
    virtual ~ParticleEffectorSettingsFadeOut();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    irr::video::SColor   mTargetColor;
    irr::u32             mTimeNeededToFadeOut;
};

struct ParticleEffectorSettingsGravity : public ParticleEffectorSettings
{
    ParticleEffectorSettingsGravity();
    virtual ~ParticleEffectorSettingsGravity();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    irr::core::vector3df     mGravity;
    irr::u32                 mTimeForceLost;
};

struct ParticleEmitterSettings
{
    ParticleEmitterSettings();
    virtual ~ParticleEmitterSettings();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    PARTICLE_EMITTER_TYPE mType;

    irr::core::vector3df     mDirection;
    irr::u32                 mMinParticlesPerSecond;
    irr::u32                 mMaxParticlePerSecond;
    irr::video::SColor       mMinStartColor;
    irr::video::SColor       mMaxStartColor;
    irr::u32                 mLifeTimeMin;
    irr::u32                 mLifeTimeMax;
    irr::s32                 mMaxAngleDegrees;
};

struct ParticleEmitterSettingsBox : public ParticleEmitterSettings
{
    ParticleEmitterSettingsBox();
    virtual ~ParticleEmitterSettingsBox();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    irr::core::aabbox3df mBox;
};

struct ParticleEmitterSettingsPoint : public ParticleEmitterSettings
{
    ParticleEmitterSettingsPoint();
    virtual ~ParticleEmitterSettingsPoint();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;
};

struct NodeSettingsParticle : public NodeSettingsUnknown
{
    NodeSettingsParticle();
    virtual ~NodeSettingsParticle();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    virtual bool IsOfClassType(NODE_TYPE type_) { return type_ == NT_UNKNOWN || type_ == NT_PARTICLE; }

    bool            	 mDefaultEmitter;
    irr::core::vector3df mCenter;
    irr::core::vector3df mRotation;
    irr::core::vector3df mScale;

    bool                    mParticlesGlobal;
    irr::core::dimension2d<irr::f32>  mParticleSize;

    MaterialSettings        mMaterialSettings;
    std::vector<ParticleEffectorSettings*>  mEffectors;
    ParticleEmitterSettings*                mEmitter;

protected:
    void DeleteAllEffectors();
};

struct NodeSettingsSkybox : public NodeSettingsUnknown
{
    NodeSettingsSkybox();
    virtual ~NodeSettingsSkybox();

    virtual void ReadFromXml(const TiXmlElement * settings_);
    virtual void WriteToXml(TiXmlElement * settings_) const;

    virtual bool IsOfClassType(NODE_TYPE type_) { return type_ == NT_UNKNOWN || type_ == NT_SKYBOX; }

    std::string mTextureUp;
    std::string mTextureDown;
    std::string mTextureLeft;
    std::string mTextureRight;
    std::string mTextureFront;
    std::string mTextureBack;
};

class NodeManager
{
public:
    NodeManager();
    ~NodeManager();

    void LoadSettings();
    void SaveSettings();

    NodeSettingsUnknown* AddNode(std::string name_, NODE_TYPE type_);
    NodeSettingsUnknown* GetNode(unsigned int index_);
    int FindNodeIndex(const std::string & name_);
    void RemoveNode(unsigned int index_);   // careful: you can no longer even load it afterwards!
    void RemoveAllNodes();
    unsigned int GetNumNodes() const { return mNodeSettings.size(); }

    irr::scene::ISceneNode* LoadNode(const std::string &name_);
    void UnloadNode(const std::string &name_);  // removes it from memory
    void UpdateNodeSettings(irr::scene::ISceneNode* node_, NodeSettingsUnknown* settings_);

    static NodeSettingsUnknown* MakeNodeSettings(NODE_TYPE type_);
    static NodeSettingsUnknown* MakeNodeSettings(const char * type_);   // type as string in xml's
    static AnimatorSettings* MakeAnimatorSettings(ANIMATOR_TYPE type_);
    static AnimatorSettings* MakeAnimatorSettings(const char * type_);
    static ParticleEffectorSettings* MakeParticleEffectorSettings(PARTICLE_AFFECTOR_TYPE type_);
    static ParticleEffectorSettings* MakeParticleEffectorSettings(const char * type_);
    static ParticleEmitterSettings* MakeParticleEmitterSettings(PARTICLE_EMITTER_TYPE type_);
    static ParticleEmitterSettings* MakeParticleEmitterSettings(const char * type_);

protected:
    irr::scene::ISceneNode* AddNode(NodeSettingsUnknown* settings_);

private:
    std::vector<NodeSettingsUnknown*>   mNodeSettings;
};

#endif // NODE_MANAGER_H
