// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "node_manager.h"
#include "config.h"
#include "main.h"
#include "irrlicht_manager.h"
#include "editor.h"
#include "logging.h"
#include "helper_xml.h"
#include "tinyxml/tinyxml.h"

using namespace irr;

// --------------------------------------------------------
NodeSettingsUnknown::NodeSettingsUnknown()
{
    mType = NT_UNKNOWN;
    mAnimatorSettings = NULL;
}

NodeSettingsUnknown::~NodeSettingsUnknown()
{
    RemoveAllChilds();
}

void NodeSettingsUnknown::ReadFromXml(const TiXmlElement * settings_)
{
    RemoveAllChilds();

    // NODE_TYPE mType  // must be already set

    const char * val = settings_->Attribute("name");
    if ( val )
    {
        mName = val;
    }

    const TiXmlNode* nodeAnimator = settings_->IterateChildren("animator", NULL);
    if ( nodeAnimator )
    {
        const TiXmlElement * elementAnimator = nodeAnimator->ToElement();
        assert(elementAnimator);

        const char * val = elementAnimator->Attribute("type");
        mAnimatorSettings = NodeManager::MakeAnimatorSettings(val);

        if ( mAnimatorSettings )
        {
            mAnimatorSettings->ReadFromXml(elementAnimator);
        }
    }

    const TiXmlNode* nodeNode = settings_->IterateChildren("node", NULL);
    while ( nodeNode )
    {
        const TiXmlElement * nodeElement = nodeNode->ToElement();
        assert(nodeElement);

        const char * val = nodeElement->Attribute("type");
        NodeSettingsUnknown* nodeSettings = NodeManager::MakeNodeSettings(val);

        if ( nodeSettings )
        {
            nodeSettings->ReadFromXml(nodeElement);
            mChildNodeSettings.push_back(nodeSettings);
        }

        nodeNode = settings_->IterateChildren("node", nodeNode);
    }
}

void NodeSettingsUnknown::WriteToXml(TiXmlElement * settings_) const
{
    switch ( mType )
    {
        case NT_UNKNOWN:
            settings_->SetAttribute("type", "unknown");
            break;
        case NT_MESH:
            settings_->SetAttribute("type", "mesh");
            break;
        case NT_ANIMATED_MESH:
            settings_->SetAttribute("type", "anim_mesh");
            break;
        case NT_LIGHT:
            settings_->SetAttribute("type", "light");
            break;
        case NT_PARTICLE:
            settings_->SetAttribute("type", "particle");
            break;
        case NT_SKYBOX:
            settings_->SetAttribute("type", "skybox");
            break;
    }

    std::string str(mName.begin(), mName.end());
    settings_->SetAttribute("name", str.c_str() );

    if ( mAnimatorSettings )
    {
        TiXmlElement ele("animator");
        mAnimatorSettings->WriteToXml(&ele);
        settings_->InsertEndChild(ele);
    }

    for ( unsigned int i=0; i < mChildNodeSettings.size(); ++i )
    {
        TiXmlElement ele("node");
        mChildNodeSettings[i]->WriteToXml(&ele);
        settings_->InsertEndChild(ele);
    }
}

NodeSettingsUnknown* NodeSettingsUnknown::AddChild(std::string name_, NODE_TYPE type_)
{
    NodeSettingsUnknown * node = NodeManager::MakeNodeSettings(type_);
    node->mName = name_;
    mChildNodeSettings.push_back(node);
    return node;
}

NodeSettingsUnknown* NodeSettingsUnknown::GetChild(unsigned int index_)
{
    if ( index_ >= mChildNodeSettings.size() )
        return NULL;

    return mChildNodeSettings[index_];
}

void NodeSettingsUnknown::RemoveChild(unsigned int index_)
{
    if ( index_ >= mChildNodeSettings.size() )
        return;
    delete mChildNodeSettings[index_];
    mChildNodeSettings.erase(mChildNodeSettings.begin() + index_);
}

void NodeSettingsUnknown::RemoveAllChilds()
{
    for ( unsigned int i=0; i < mChildNodeSettings.size(); ++i )
    {
        delete mChildNodeSettings[i];
    }
    delete mAnimatorSettings;
    mAnimatorSettings = NULL;
}

// --------------------------------------------------------
NodeSettingsMesh::NodeSettingsMesh()
: NodeSettingsUnknown()
, mScale(1,1,1)
{
    mType = NT_MESH;
    mBackfaceCulling = true;
}

NodeSettingsMesh::~NodeSettingsMesh()
{
}

void NodeSettingsMesh::ReadFromXml(const TiXmlElement * settings_)
{
    NodeSettingsUnknown::ReadFromXml(settings_);

    const char * valChar = settings_->Attribute("file");
    if ( valChar )
        mFile = valChar;

    settings_->QueryFloatAttribute("c_x", &mCenter.X);
    settings_->QueryFloatAttribute("c_y", &mCenter.Y);
    settings_->QueryFloatAttribute("c_z", &mCenter.Z);

    settings_->QueryFloatAttribute("r_x", &mRotation.X);
    settings_->QueryFloatAttribute("r_y", &mRotation.Y);
    settings_->QueryFloatAttribute("r_z", &mRotation.Z);

    settings_->QueryFloatAttribute("s_x", &mScale.X);
    settings_->QueryFloatAttribute("s_y", &mScale.Y);
    settings_->QueryFloatAttribute("s_z", &mScale.Z);

    int valInt = mBackfaceCulling;
    if ( TIXML_SUCCESS == settings_->QueryIntAttribute("backface_culling", &valInt) )
        mBackfaceCulling = valInt ? true : false;
}

void NodeSettingsMesh::WriteToXml(TiXmlElement * settings_) const
{
    NodeSettingsUnknown::WriteToXml(settings_);

    settings_->SetAttribute("file", mFile.c_str());

    settings_->SetDoubleAttribute("c_x", mCenter.X);
    settings_->SetDoubleAttribute("c_y", mCenter.Y);
    settings_->SetDoubleAttribute("c_z", mCenter.Z);

    settings_->SetDoubleAttribute("r_x", mRotation.X);
    settings_->SetDoubleAttribute("r_y", mRotation.Y);
    settings_->SetDoubleAttribute("r_z", mRotation.Z);

    settings_->SetDoubleAttribute("s_x", mScale.X);
    settings_->SetDoubleAttribute("s_y", mScale.Y);
    settings_->SetDoubleAttribute("s_z", mScale.Z);

    settings_->SetAttribute("backface_culling", mBackfaceCulling ? 1: 0 );
}

// --------------------------------------------------------
NodeSettingsAnimatedMesh::NodeSettingsAnimatedMesh()
: NodeSettingsMesh()
{
    mType = NT_ANIMATED_MESH;
    mAnimationSpeed = 25;
}

NodeSettingsAnimatedMesh::~NodeSettingsAnimatedMesh()
{
}

void NodeSettingsAnimatedMesh::ReadFromXml(const TiXmlElement * settings_)
{
    NodeSettingsMesh::ReadFromXml(settings_);

    settings_->QueryIntAttribute("anim_speed", &mAnimationSpeed);
}

void NodeSettingsAnimatedMesh::WriteToXml(TiXmlElement * settings_) const
{
    NodeSettingsMesh::WriteToXml(settings_);

    settings_->SetAttribute("anim_speed", mAnimationSpeed);
}

// --------------------------------------------------------
NodeSettingsLight::NodeSettingsLight()
: NodeSettingsUnknown()
{
    mType = NT_LIGHT;
    mLightType = video::ELT_POINT;
    mCastShadows = false;
    mRadius = 100.f;
    mDiffuseColor = video::SColorf(1.f, 1.f, 1.f);
}

NodeSettingsLight::~NodeSettingsLight()
{
}

void NodeSettingsLight::ReadFromXml(const TiXmlElement * settings_)
{
    NodeSettingsUnknown::ReadFromXml(settings_);

    settings_->QueryFloatAttribute("c_x", &mCenter.X);
    settings_->QueryFloatAttribute("c_y", &mCenter.Y);
    settings_->QueryFloatAttribute("c_z", &mCenter.Z);

    settings_->QueryFloatAttribute("r_x", &mRotation.X);
    settings_->QueryFloatAttribute("r_y", &mRotation.Y);
    settings_->QueryFloatAttribute("r_z", &mRotation.Z);

    int valInt = mCastShadows;
    if ( TIXML_SUCCESS == settings_->QueryIntAttribute("shadow", &valInt) )
        mCastShadows = valInt ? true : false;

    valInt = mLightType;
    if ( TIXML_SUCCESS == settings_->QueryIntAttribute("lighttype", &valInt) )
        mLightType = valInt == 1 ? video::ELT_DIRECTIONAL : video::ELT_POINT;

    settings_->QueryFloatAttribute("radius", &mRadius);

    settings_->QueryFloatAttribute("d_r", &mDiffuseColor.r);
    settings_->QueryFloatAttribute("d_g", &mDiffuseColor.g);
    settings_->QueryFloatAttribute("d_b", &mDiffuseColor.b);
    settings_->QueryFloatAttribute("d_a", &mDiffuseColor.a);

    settings_->QueryFloatAttribute("s_r", &mSpecularColor.r);
    settings_->QueryFloatAttribute("s_g", &mSpecularColor.g);
    settings_->QueryFloatAttribute("s_b", &mSpecularColor.b);
    settings_->QueryFloatAttribute("s_a", &mSpecularColor.a);
}

void NodeSettingsLight::WriteToXml(TiXmlElement * settings_) const
{
    NodeSettingsUnknown::WriteToXml(settings_);

    settings_->SetDoubleAttribute("c_x", mCenter.X);
    settings_->SetDoubleAttribute("c_y", mCenter.Y);
    settings_->SetDoubleAttribute("c_z", mCenter.Z);

    settings_->SetDoubleAttribute("r_x", mRotation.X);
    settings_->SetDoubleAttribute("r_y", mRotation.Y);
    settings_->SetDoubleAttribute("r_z", mRotation.Z);

    settings_->SetAttribute("shadow", mCastShadows ? 1: 0 );
    settings_->SetAttribute("lighttype", mLightType == video::ELT_DIRECTIONAL ? 1 : 0 );
    settings_->SetDoubleAttribute("radius", mRadius);

    settings_->SetDoubleAttribute("d_r", mDiffuseColor.r);
    settings_->SetDoubleAttribute("d_g", mDiffuseColor.g);
    settings_->SetDoubleAttribute("d_b", mDiffuseColor.b);
    settings_->SetDoubleAttribute("d_a", mDiffuseColor.a);

    settings_->SetDoubleAttribute("s_r", mSpecularColor.r);
    settings_->SetDoubleAttribute("s_g", mSpecularColor.g);
    settings_->SetDoubleAttribute("s_b", mSpecularColor.b);
    settings_->SetDoubleAttribute("s_a", mSpecularColor.a);
}

// --------------------------------------------------------
AnimatorSettings::AnimatorSettings()
: mType(AT_NONE)
{
}

AnimatorSettings::~AnimatorSettings()
{
}

void AnimatorSettings::ReadFromXml(const TiXmlElement * settings_)
{
    // ANIMATOR_TYPE mType  // must be already set
}

void AnimatorSettings::WriteToXml(TiXmlElement * settings_) const
{
    switch ( mType )
    {
        case AT_ROTATION:
            settings_->SetAttribute("type", "anirot");
            break;
        default:
            break;
    }
}

// --------------------------------------------------------
AnimatorSettingsRotation::AnimatorSettingsRotation()
: AnimatorSettings()
{
    mType = AT_ROTATION;
}

AnimatorSettingsRotation::~AnimatorSettingsRotation()
{
}

void AnimatorSettingsRotation::ReadFromXml(const TiXmlElement * settings_)
{
    AnimatorSettings::ReadFromXml(settings_);

    XmlHelper::Read(*settings_, mRotationPerSec, "r_x", "r_y", "r_z");
}

void AnimatorSettingsRotation::WriteToXml(TiXmlElement * settings_) const
{
    AnimatorSettings::WriteToXml(settings_);

    settings_->SetDoubleAttribute("r_x", mRotationPerSec.X);
    settings_->SetDoubleAttribute("r_y", mRotationPerSec.Y);
    settings_->SetDoubleAttribute("r_z", mRotationPerSec.Z);
}

// --------------------------------------------------------
MaterialSettings::MaterialSettings()
{
    mMaterialTypeParam = 0.02f;
    mMaterialType = irr::video::EMT_SOLID;
	APP.GetIrrlichtManager()->SetRealMaterial(mMaterialType);
    mLighting = false;
}

void MaterialSettings::ReadFromXml(const TiXmlElement * settings_)
{
    settings_->Attribute("mat_type", (int*)&mMaterialType);
    APP.GetIrrlichtManager()->SetRealMaterial(mMaterialType);
    mTexture1.clear();
    const char * valChar = settings_->Attribute("tex1");
    if ( valChar )
    {
        mTexture1 = valChar;
    }
    settings_->QueryBoolAttribute("light", &mLighting);
    settings_->QueryFloatAttribute("param1", &mMaterialTypeParam);
}

void MaterialSettings::WriteToXml(TiXmlElement * settings_) const
{
    settings_->SetAttribute("mat_type", (int)mMaterialType);
    settings_->SetAttribute("tex1", mTexture1.c_str() );
    settings_->SetBoolAttribute("light", mLighting);
    settings_->SetDoubleAttribute("param1", mMaterialTypeParam);

}

// --------------------------------------------------------
ParticleEffectorSettings::ParticleEffectorSettings()
{
    mType = PAT_NONE;
}

ParticleEffectorSettings::~ParticleEffectorSettings()
{
}

void ParticleEffectorSettings::ReadFromXml(const TiXmlElement * settings_)
{
    //mType     // must be set already before calling ReadFromXml
}

void ParticleEffectorSettings::WriteToXml(TiXmlElement * settings_) const
{
    switch ( mType )
    {
        case PAT_NONE:
            settings_->SetAttribute("type", "none");
            break;
        case PAT_FADE_OUT:
            settings_->SetAttribute("type", "fade");
            break;
        case PAT_GRAVITY:
            settings_->SetAttribute("type", "gravity");
            break;
    }
}

// --------------------------------------------------------
ParticleEffectorSettingsFadeOut::ParticleEffectorSettingsFadeOut()
: ParticleEffectorSettings()
{
    mType = PAT_FADE_OUT;
    mTargetColor = video::SColor(0,0,0,0);
    mTimeNeededToFadeOut = 1000;
}

ParticleEffectorSettingsFadeOut::~ParticleEffectorSettingsFadeOut()
{
}

void ParticleEffectorSettingsFadeOut::ReadFromXml(const TiXmlElement * settings_)
{
    ParticleEffectorSettings::ReadFromXml(settings_);

    XmlHelper::Read(*settings_, mTargetColor, "target_r", "target_g", "target_b", "target_a");
    settings_->Attribute("time_fadedout", (int*)&mTimeNeededToFadeOut);
}

void ParticleEffectorSettingsFadeOut::WriteToXml(TiXmlElement * settings_) const
{
    ParticleEffectorSettings::WriteToXml(settings_);

    XmlHelper::Write(*settings_, mTargetColor, "target_r", "target_g", "target_b", "target_a");
    settings_->SetAttribute("time_fadedout", (int)mTimeNeededToFadeOut);
}


// --------------------------------------------------------
ParticleEffectorSettingsGravity::ParticleEffectorSettingsGravity()
: ParticleEffectorSettings()
{
    mType = PAT_GRAVITY;
    mGravity = core::vector3df(0.0f,-0.03f, 0.0f);
    mTimeForceLost = 1000;
}

ParticleEffectorSettingsGravity::~ParticleEffectorSettingsGravity()
{
}

void ParticleEffectorSettingsGravity::ReadFromXml(const TiXmlElement * settings_)
{
    ParticleEffectorSettings::ReadFromXml(settings_);

    XmlHelper::Read(*settings_, mGravity, "g_x", "g_y", "g_z");
    settings_->Attribute("timeforcelost", (int*)&mTimeForceLost);
}

void ParticleEffectorSettingsGravity::WriteToXml(TiXmlElement * settings_) const
{
    ParticleEffectorSettings::WriteToXml(settings_);

    XmlHelper::Write(*settings_, mGravity, "g_x", "g_y", "g_z");
    settings_->SetAttribute("timeforcelost", (int)mTimeForceLost );
}

// --------------------------------------------------------
ParticleEmitterSettings::ParticleEmitterSettings()
{
    mType = PET_NONE;
    mDirection = core::vector3df(0.0f, 0.03f, 0.0f);
    mMinParticlesPerSecond = 5;
    mMaxParticlePerSecond = 10;
    mMinStartColor = video::SColor(255, 0, 0, 0);
    mMaxStartColor = video::SColor(255, 255, 255, 255);
    mLifeTimeMin = 2000;
    mLifeTimeMax = 4000;
    mMaxAngleDegrees = 0;
}

ParticleEmitterSettings::~ParticleEmitterSettings()
{
}

void ParticleEmitterSettings::ReadFromXml(const TiXmlElement * settings_)
{
    // mType must be known already before calling ReadFromXml
    XmlHelper::Read(*settings_, mDirection, "dir_x", "dir_y", "dir_z");
    settings_->Attribute("particles_sec_min", (int*)&mMinParticlesPerSecond );
    settings_->Attribute("particles_sec_max", (int*)&mMaxParticlePerSecond );
    XmlHelper::Read(*settings_, mMinStartColor, "min_r", "min_g", "min_b", "min_a" );
    XmlHelper::Read(*settings_, mMaxStartColor, "max_r", "max_g", "max_b", "max_a" );
    settings_->Attribute("life_min", (int*)&mLifeTimeMin);
    settings_->Attribute("life_max", (int*)&mLifeTimeMax);
    settings_->Attribute("angle_max", (int*)&mMaxAngleDegrees);
}

void ParticleEmitterSettings::WriteToXml(TiXmlElement * settings_) const
{
    switch ( mType )
    {
        case PET_NONE:
            settings_->SetAttribute("type", "none");
            break;
        case PET_BOX:
            settings_->SetAttribute("type", "box");
            break;
        case PET_POINT:
            settings_->SetAttribute("type", "point");
            break;
    }

    XmlHelper::Write(*settings_, mDirection, "dir_x", "dir_y", "dir_z");
    settings_->SetAttribute("particles_sec_min", (int)mMinParticlesPerSecond );
    settings_->SetAttribute("particles_sec_max", (int)mMaxParticlePerSecond );
    XmlHelper::Write(*settings_, mMinStartColor, "min_r", "min_g", "min_b", "min_a" );
    XmlHelper::Write(*settings_, mMaxStartColor, "max_r", "max_g", "max_b", "max_a" );
    settings_->SetAttribute("life_min", (int)mLifeTimeMin);
    settings_->SetAttribute("life_max", (int)mLifeTimeMax);
    settings_->SetAttribute("angle_max", (int)mMaxAngleDegrees);
}

// --------------------------------------------------------
ParticleEmitterSettingsBox::ParticleEmitterSettingsBox()
{
    mType = PET_BOX;
    mBox = core::aabbox3df(-10, 28,-10, 10, 30, 10);
}

ParticleEmitterSettingsBox::~ParticleEmitterSettingsBox()
{
}

void ParticleEmitterSettingsBox::ReadFromXml(const TiXmlElement * settings_)
{
    ParticleEmitterSettings::ReadFromXml(settings_);
    XmlHelper::Read(*settings_, mBox, "maxx", "maxy", "maxz", "minx", "miny", "minz");
}

void ParticleEmitterSettingsBox::WriteToXml(TiXmlElement * settings_) const
{
    ParticleEmitterSettings::WriteToXml(settings_);
    XmlHelper::Write(*settings_, mBox, "maxx", "maxy", "maxz", "minx", "miny", "minz");
}

// --------------------------------------------------------
ParticleEmitterSettingsPoint::ParticleEmitterSettingsPoint()
{
    mType = PET_POINT;
}

ParticleEmitterSettingsPoint::~ParticleEmitterSettingsPoint()
{
}

void ParticleEmitterSettingsPoint::ReadFromXml(const TiXmlElement * settings_)
{
    ParticleEmitterSettings::ReadFromXml(settings_);
}

void ParticleEmitterSettingsPoint::WriteToXml(TiXmlElement * settings_) const
{
    ParticleEmitterSettings::WriteToXml(settings_);
}

// --------------------------------------------------------
NodeSettingsParticle::NodeSettingsParticle()
: NodeSettingsUnknown()
, mEmitter(0)
{
    mType = NT_PARTICLE;
    mDefaultEmitter = false;
    mCenter = core::vector3df(0.f, 0.f, 0.f);
    mRotation = core::vector3df(0.f, 0.f, 0.f);
    mScale = core::vector3df(1.f, 1.f, 1.f);
    mParticlesGlobal = true;
    mParticleSize = core::dimension2d<f32>(5.0f, 5.0f);

    mMaterialSettings.mMaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
    APP.GetIrrlichtManager()->SetRealMaterial(mMaterialSettings.mMaterialType);
    mMaterialSettings.mLighting = false;
    mMaterialSettings.mMaterialTypeParam = 0.02f;
}

NodeSettingsParticle::~NodeSettingsParticle()
{
    DeleteAllEffectors();
    delete mEmitter;
    mEmitter = NULL;
}

void NodeSettingsParticle::DeleteAllEffectors()
{
    for ( unsigned int i=0; i < mEffectors.size(); ++i )
        delete mEffectors[i];
    mEffectors.clear();
}

void NodeSettingsParticle::ReadFromXml(const TiXmlElement * settings_)
{
    NodeSettingsUnknown::ReadFromXml(settings_);

    int valInt = mDefaultEmitter;
    if ( TIXML_SUCCESS == settings_->QueryIntAttribute("default_emitter", &valInt) )
        mDefaultEmitter = valInt ? true : false;

    settings_->QueryFloatAttribute("c_x", &mCenter.X);
    settings_->QueryFloatAttribute("c_y", &mCenter.Y);
    settings_->QueryFloatAttribute("c_z", &mCenter.Z);

    settings_->QueryFloatAttribute("r_x", &mRotation.X);
    settings_->QueryFloatAttribute("r_y", &mRotation.Y);
    settings_->QueryFloatAttribute("r_z", &mRotation.Z);

    settings_->QueryFloatAttribute("s_x", &mScale.X);
    settings_->QueryFloatAttribute("s_y", &mScale.Y);
    settings_->QueryFloatAttribute("s_z", &mScale.Z);

    valInt = mParticlesGlobal;
    if ( TIXML_SUCCESS == settings_->QueryIntAttribute("global", &valInt) )
        mParticlesGlobal = valInt ? true : false;

    settings_->QueryFloatAttribute("height", &mParticleSize.Height);
    settings_->QueryFloatAttribute("width", &mParticleSize.Width);

    mMaterialSettings.ReadFromXml( settings_ );

    DeleteAllEffectors();
    const TiXmlNode* nodeEffector = settings_->IterateChildren("effector", NULL);
    while ( nodeEffector )
    {
        const TiXmlElement * eleEffector = nodeEffector->ToElement();
        assert(eleEffector);
        const char * type = eleEffector->Attribute("type");
        ParticleEffectorSettings* effector = NodeManager::MakeParticleEffectorSettings(type);
        if ( effector )
        {
            effector->ReadFromXml( eleEffector );
            mEffectors.push_back(effector);
        }

        nodeEffector = settings_->IterateChildren("effector", nodeEffector);
    }

    delete mEmitter;
    mEmitter = NULL;
    const TiXmlNode* nodeEmitter = settings_->IterateChildren("emitter", NULL);
    if ( nodeEmitter )
    {
        const TiXmlElement * eleEmitter = nodeEmitter->ToElement();
        assert(eleEmitter);
        const char * type = eleEmitter->Attribute("type");
        mEmitter = NodeManager::MakeParticleEmitterSettings(type);
        if ( mEmitter )
        {
            mEmitter->ReadFromXml( eleEmitter );
        }
    }
}

void NodeSettingsParticle::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;
    settings_->Clear();
    NodeSettingsUnknown::WriteToXml(settings_);

    settings_->SetAttribute("default_emitter", mDefaultEmitter ? 1: 0 );

    settings_->SetDoubleAttribute("c_x", mCenter.X);
    settings_->SetDoubleAttribute("c_y", mCenter.Y);
    settings_->SetDoubleAttribute("c_z", mCenter.Z);

    settings_->SetDoubleAttribute("r_x", mRotation.X);
    settings_->SetDoubleAttribute("r_y", mRotation.Y);
    settings_->SetDoubleAttribute("r_z", mRotation.Z);

    settings_->SetDoubleAttribute("s_x", mScale.X);
    settings_->SetDoubleAttribute("s_y", mScale.Y);
    settings_->SetDoubleAttribute("s_z", mScale.Z);

    settings_->SetAttribute("global", mParticlesGlobal ? 1: 0 );

    settings_->SetDoubleAttribute("height", mParticleSize.Height);
    settings_->SetDoubleAttribute("width", mParticleSize.Width);

    mMaterialSettings.WriteToXml( settings_ );

    for ( unsigned int i=0; i < mEffectors.size(); ++i )
    {
        TiXmlElement ele("effector");
        mEffectors[i]->WriteToXml( &ele );
        settings_->InsertEndChild( ele );
    }

    if ( mEmitter )
    {
        TiXmlElement ele("emitter");
        mEmitter->WriteToXml( &ele );
        settings_->InsertEndChild( ele );
    }
}

// --------------------------------------------------------
NodeSettingsSkybox::NodeSettingsSkybox()
: NodeSettingsUnknown()
{
    mType = NT_SKYBOX;
}

NodeSettingsSkybox::~NodeSettingsSkybox()
{
}

void NodeSettingsSkybox::ReadFromXml(const TiXmlElement * settings_)
{
    NodeSettingsUnknown::ReadFromXml(settings_);

    const char * val = settings_->Attribute("up");
    if ( val )
        mTextureUp = val;
    val = settings_->Attribute("down");
    if ( val )
        mTextureDown = val;
    val = settings_->Attribute("left");
    if ( val )
        mTextureLeft = val;
    val = settings_->Attribute("right");
    if ( val )
        mTextureRight = val;
    val = settings_->Attribute("front");
    if ( val )
        mTextureFront = val;
    val = settings_->Attribute("back");
    if ( val )
        mTextureBack = val;
}

void NodeSettingsSkybox::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;
    settings_->Clear();
    NodeSettingsUnknown::WriteToXml(settings_);

    settings_->SetAttribute("up", mTextureUp.c_str() );
    settings_->SetAttribute("down", mTextureDown.c_str() );
    settings_->SetAttribute("left", mTextureLeft.c_str() );
    settings_->SetAttribute("right", mTextureRight.c_str() );
    settings_->SetAttribute("front", mTextureFront.c_str() );
    settings_->SetAttribute("back", mTextureBack.c_str() );
}

// --------------------------------------------------------
NodeManager::NodeManager()
{
}

NodeManager::~NodeManager()
{
    RemoveAllNodes();
}

void NodeManager::RemoveAllNodes()
{
    for ( unsigned int i=0; i < mNodeSettings.size(); ++i )
    {
        delete mNodeSettings[i];
    }
    mNodeSettings.clear();
}

void NodeManager::LoadSettings()
{
    std::string xmlName ( APP.GetConfig()->GetPathMedia() + "models.xml" );
    TiXmlDocument xmlDoc(xmlName.c_str());
    xmlDoc.SetIrrFs(APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem(), TiXmlDocument::E_ON_READ_ANDROID);

    if ( !xmlDoc.LoadFile() )
    {
		LOG.DebugLn("TiXmlDocument::ErrorDesc: ", xmlDoc.ErrorDesc());
        LOG.WarnLn("NodeManager::LoadSettings: could not load xml ", xmlName.c_str());
        return;
    }

    TiXmlNode * modelsNode = xmlDoc.FirstChild("models");
	if (!modelsNode)
		return;

    RemoveAllNodes();
    const TiXmlNode* nodeNode = modelsNode->IterateChildren("node", NULL);
    while ( nodeNode )
    {
        const TiXmlElement * nodeElement = nodeNode->ToElement();
        assert(nodeElement);

        NodeSettingsUnknown* nodeSettings = NULL;
        const char * val = nodeElement->Attribute("type");
        nodeSettings = MakeNodeSettings(val);

        if ( nodeSettings )
        {
            nodeSettings->ReadFromXml(nodeElement);
            mNodeSettings.push_back(nodeSettings);
        }

        nodeNode = modelsNode->IterateChildren("node", nodeNode);
    }
}

void NodeManager::SaveSettings()
{
    std::string xmlName ( APP.GetConfig()->GetPathMedia() + "models.xml" );
    TiXmlDocument xmlDoc(xmlName.c_str());
	xmlDoc.SetIrrFs(APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem(), TiXmlDocument::E_ON_READ_FAIL_ANDROID);

    if ( !xmlDoc.LoadFile() )
        return;

    TiXmlNode * modelsNode = xmlDoc.FirstChild("models");
	if (!modelsNode)
		return;

    modelsNode->Clear();

    for ( unsigned int i=0; i < mNodeSettings.size(); ++i )
    {
        TiXmlElement ele("node");
        mNodeSettings[i]->WriteToXml(&ele);
        modelsNode->InsertEndChild(ele);
    }

    xmlDoc.SaveFile();
}

NodeSettingsUnknown* NodeManager::AddNode(std::string name_, NODE_TYPE type_)
{
    NodeSettingsUnknown * node = MakeNodeSettings(type_);
    node->mName = name_;
    mNodeSettings.push_back(node);
    return node;
}

void NodeManager::RemoveNode(unsigned int index_)
{
    if ( index_ >= mNodeSettings.size() )
        return;

    delete mNodeSettings[index_];
    mNodeSettings.erase(mNodeSettings.begin() + index_);
}

int NodeManager::FindNodeIndex(const std::string & name_)
{
    for ( unsigned int i=0; i < mNodeSettings.size(); ++i )
    {
        if ( mNodeSettings[i]->mName == name_ )
        {
            return i;
        }
    }
    return -1;
}

NodeSettingsUnknown* NodeManager::GetNode(unsigned int index_)
{
    if ( index_ >= mNodeSettings.size() )
        return NULL;

    return mNodeSettings[index_];
}

scene::ISceneNode* NodeManager::LoadNode(const std::string &name_)
{
    for ( unsigned int i=0; i < mNodeSettings.size(); ++i )
    {
        if ( mNodeSettings[i]->mName == name_ )
        {
            return AddNode(mNodeSettings[i]);
        }
    }
    return NULL;
}

void NodeManager::UnloadNode(const std::string &name_)
{
    int idx = FindNodeIndex(name_);
    if ( idx < 0 )
        return;

    std::string filename;
    bool hasFileToRemove = false;
    switch ( mNodeSettings[idx]->mType )
    {
        case NT_MESH:
        {
            NodeSettingsMesh* meshSettings = static_cast<NodeSettingsMesh*>(mNodeSettings[idx]);
            filename = meshSettings->mFile;
            hasFileToRemove = true;
        }
        break;
        case NT_ANIMATED_MESH:
        {
            NodeSettingsAnimatedMesh * aniMeshSettings = static_cast<NodeSettingsAnimatedMesh*>(mNodeSettings[idx]);
            filename = aniMeshSettings->mFile;
            hasFileToRemove = true;
        }
        break;
        default:
            // ignore for now (not nice, but good enought for this game)
        break;
    }
    if ( hasFileToRemove )
    {
        scene::IAnimatedMesh * mesh = APP.GetIrrlichtManager()->GetMeshInCache( filename.c_str() );
        if ( mesh )
        {
            APP.GetIrrlichtManager()->GetSceneManager()->getMeshCache()->removeMesh(mesh);
        }
    }
}

scene::ISceneNode* NodeManager::AddNode(NodeSettingsUnknown* settings_)
{
    scene::ISceneNode* result = NULL;
    if ( !settings_ )
        return result;
    switch ( settings_->mType )
    {
        case NT_UNKNOWN:
            result = APP.GetIrrlichtManager()->GetSceneManager()->addEmptySceneNode();
        break;
        case NT_MESH:
        {
            NodeSettingsMesh* meshSettings = static_cast<NodeSettingsMesh*>(settings_);

            irr::io::IFileSystem * fs = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem();
            irr::io::path oldDir = fs->getWorkingDirectory().c_str();
            fs->changeWorkingDirectoryTo(APP.GetConfig()->GetPathMedia().c_str());

//            std::string filename(APP.GetConfig()->GetPathMedia() + meshSettings->mFile);
            std::string filename(meshSettings->mFile);
            result = APP.GetIrrlichtManager()->LoadModel(*APP.GetConfig(), filename.c_str(), NULL);

            fs->changeWorkingDirectoryTo(oldDir);
        }
        break;
        case NT_ANIMATED_MESH:
        {
            NodeSettingsAnimatedMesh * aniMeshSettings = static_cast<NodeSettingsAnimatedMesh*>(settings_);

            // TODO: figure out why that changeWorkingDirectoryTo was necessary
            irr::io::IFileSystem * fs = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem();
            irr::io::path oldDir = fs->getWorkingDirectory().c_str();
            fs->changeWorkingDirectoryTo(APP.GetConfig()->GetPathMedia().c_str());

//            std::string fileName(APP.GetConfig()->GetPathMedia() + aniMeshSettings->mFile);
            std::string fileName(aniMeshSettings->mFile);

            result = APP.GetIrrlichtManager()->LoadAnimatedModel(*APP.GetConfig(), fileName.c_str());

            fs->changeWorkingDirectoryTo(oldDir);
        }
        break;
        case NT_LIGHT:
        {
            result = APP.GetIrrlichtManager()->GetSceneManager()->addLightSceneNode();

            // TODO: Hack - should be possible to disable the arrow
#ifdef HC1_ENABLE_EDITOR
            scene::SMesh * mesh = APP.GetEditor()->GetDefaultArrowMesh();
            APP.GetIrrlichtManager()->GetSceneManager()->addMeshSceneNode(mesh, result);
#endif
            break;
        }
        case NT_PARTICLE:
        {
            NodeSettingsParticle* ps = static_cast<NodeSettingsParticle*>(settings_);

            result = APP.GetIrrlichtManager()->GetSceneManager()->addParticleSystemSceneNode(ps->mDefaultEmitter);
            break;
        }
        case NT_SKYBOX:
        {
            NodeSettingsSkybox * sb = static_cast<NodeSettingsSkybox*>(settings_);

            video::IVideoDriver* driver = APP.GetIrrlichtManager()->GetVideoDriver();
            result = APP.GetIrrlichtManager()->GetSceneManager()->addSkyBoxSceneNode(
                    driver->getTexture(APP.GetConfig()->MakeFilenameTexture(sb->mTextureUp).c_str()),
                    driver->getTexture(APP.GetConfig()->MakeFilenameTexture(sb->mTextureDown).c_str()),
                    driver->getTexture(APP.GetConfig()->MakeFilenameTexture(sb->mTextureBack).c_str()),
                    driver->getTexture(APP.GetConfig()->MakeFilenameTexture(sb->mTextureFront).c_str()),
                    driver->getTexture(APP.GetConfig()->MakeFilenameTexture(sb->mTextureLeft).c_str()),
                    driver->getTexture(APP.GetConfig()->MakeFilenameTexture(sb->mTextureRight).c_str())
                    );
			if (  result )
			{
				for ( u32 i=0; i<result->getMaterialCount(); ++i )
				{
					APP.GetIrrlichtManager()->SetRealMaterial( result->getMaterial(i).MaterialType );
				}
			}
            break;
        }
        default:
            LOG.Warn("AddNode has unknown node type\n");
    }

    if ( result )
    {
        std::string nodeName(settings_->mName.begin(), settings_->mName.end());
        result->setName(nodeName.c_str());
        if ( settings_->mAnimatorSettings )
        {
            switch ( settings_->mAnimatorSettings->mType )
            {
                case AT_ROTATION:
                {
                    AnimatorSettingsRotation * aniRotSettings = static_cast<AnimatorSettingsRotation*>(settings_->mAnimatorSettings);
                    scene::ISceneNodeAnimator * anim = APP.GetIrrlichtManager()->GetSceneManager()->createRotationAnimator(aniRotSettings->mRotationPerSec);
                    result->addAnimator(anim);
                    anim->drop();
                    break;
                }
                default:
                    break;
            }
        }

        for ( unsigned int i=0; i < settings_->mChildNodeSettings.size(); ++i )
        {
            result->addChild(AddNode(settings_->mChildNodeSettings[i]));
        }

        UpdateNodeSettings(result, settings_);

        result->updateAbsolutePosition();
    }

    return result;
}

void NodeManager::UpdateNodeSettings(scene::ISceneNode* node_, NodeSettingsUnknown* settings_)
{
    if ( !node_ || !settings_ )
        return;
    switch ( settings_->mType )
    {
        case NT_UNKNOWN:
        break;
        case NT_MESH:
        {
            NodeSettingsMesh* meshSettings = static_cast<NodeSettingsMesh*>(settings_);
            node_->setPosition(meshSettings->mCenter);
            node_->setRotation(meshSettings->mRotation);
            if ( meshSettings->mScale.equals(irr::core::vector3df(1,1,1)) )
            {
				node_->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
            }
            else
            {
				node_->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			}
			node_->setScale(meshSettings->mScale);
            node_->setMaterialFlag(video::EMF_BACK_FACE_CULLING, meshSettings->mBackfaceCulling);
        }
        break;
        case NT_ANIMATED_MESH:
        {
            scene::IAnimatedMeshSceneNode * aniNode = static_cast<scene::IAnimatedMeshSceneNode*>(node_);
            NodeSettingsAnimatedMesh * aniMeshSettings = static_cast<NodeSettingsAnimatedMesh*>(settings_);
            aniNode->setPosition(aniMeshSettings->mCenter);
            aniNode->setRotation(aniMeshSettings->mRotation);
            if ( aniMeshSettings->mScale.equals(irr::core::vector3df(1,1,1)) )
            {
				node_->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
			}
			else
			{
				node_->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			}
			aniNode->setScale(aniMeshSettings->mScale);
            aniNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, aniMeshSettings->mBackfaceCulling);
            aniNode->setAnimationSpeed(aniMeshSettings->mAnimationSpeed);
        }
        break;
        case NT_LIGHT:
        {
            scene::ILightSceneNode* lightNode = static_cast<scene::ILightSceneNode*>(node_);
            NodeSettingsLight * lightSettings = static_cast<NodeSettingsLight*>(settings_);
            lightNode->setPosition(lightSettings->mCenter);
            lightNode->setRotation(lightSettings->mRotation);
            lightNode->setLightType(lightSettings->mLightType);
            lightNode->enableCastShadow(lightSettings->mCastShadows);
            lightNode->setRadius(lightSettings->mRadius);

            video::SLight & light = lightNode->getLightData();
            light.DiffuseColor = video::SColorf(lightSettings->mDiffuseColor);
            light.SpecularColor = video::SColorf(lightSettings->mSpecularColor);
        }
        break;
        case NT_PARTICLE:
        {
            scene::IParticleSystemSceneNode * particleNode = static_cast<scene::IParticleSystemSceneNode*>(node_);
            NodeSettingsParticle* ps = static_cast<NodeSettingsParticle*>(settings_);

            particleNode->setPosition(ps->mCenter);
            particleNode->setRotation(ps->mRotation);
            if ( ps->mScale.equals(irr::core::vector3df(1,1,1)) )
            {
				particleNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, false);
            }
            else
            {
				particleNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            }
            particleNode->setScale(ps->mScale);
            particleNode->setParticlesAreGlobal(ps->mParticlesGlobal);
//            particleNode->setParticleSize(ps->mParticleSize);	// was used up to hcraft 1.2, is for now use in emitter settings.

            particleNode->removeAllAffectors();
            for ( unsigned int i=0; i < ps->mEffectors.size(); ++i )
            {
                scene::IParticleAffector * affector = 0;
                if ( ps->mEffectors[i]->mType == PAT_FADE_OUT )
                {
                    ParticleEffectorSettingsFadeOut *fade = static_cast<ParticleEffectorSettingsFadeOut*>(ps->mEffectors[i]);
                    affector = particleNode->createFadeOutParticleAffector(fade->mTargetColor, fade->mTimeNeededToFadeOut);
                }
                else if ( ps->mEffectors[i]->mType == PAT_GRAVITY )
                {
                    ParticleEffectorSettingsGravity * grav = static_cast<ParticleEffectorSettingsGravity*>(ps->mEffectors[i]);
                    affector = particleNode->createGravityAffector(grav->mGravity, grav->mTimeForceLost);
                }
                if ( affector )
                {
                    particleNode->addAffector(affector);
                    affector->drop();
                }
            }

            particleNode->setMaterialType(ps->mMaterialSettings.mMaterialType);
            video::SMaterial & material = particleNode->getMaterial(0);
            material.MaterialTypeParam = ps->mMaterialSettings.mMaterialTypeParam;
            particleNode->setMaterialFlag(video::EMF_LIGHTING, ps->mMaterialSettings.mLighting);

            if ( ps->mMaterialSettings.mTexture1.length() )
            {
                std::string texName ( APP.GetConfig()->MakeFilenameTexture( ps->mMaterialSettings.mTexture1 ) );

                video::ITexture * texture = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getVideoDriver()->getTexture(texName.c_str());
                particleNode->setMaterialTexture(0, texture);
            }

            particleNode->setEmitter(0);
            if ( ps->mEmitter )
            {
                scene::IParticleEmitter * emitter = 0;
                if ( ps->mEmitter->mType == PET_BOX )
                {
                    ParticleEmitterSettingsBox * box = static_cast<ParticleEmitterSettingsBox*>(ps->mEmitter);
                    emitter = particleNode->createBoxEmitter(box->mBox,
                                                            box->mDirection,
                                                            box->mMinParticlesPerSecond,
                                                            box->mMaxParticlePerSecond,
                                                            box->mMinStartColor,
                                                            box->mMaxStartColor,
                                                            box->mLifeTimeMin,
                                                            box->mLifeTimeMax,
                                                            box->mMaxAngleDegrees,
                                                            ps->mParticleSize,
                                                            ps->mParticleSize);
                }
                else if ( ps->mEmitter->mType == PET_POINT )
                {
                    ParticleEmitterSettingsPoint * point = static_cast<ParticleEmitterSettingsPoint*>(ps->mEmitter);
                    emitter = particleNode->createPointEmitter(point->mDirection,
                                                                point->mMinParticlesPerSecond,
                                                                point->mMaxParticlePerSecond,
                                                                point->mMinStartColor,
                                                                point->mMaxStartColor,
                                                                point->mLifeTimeMin,
                                                                point->mLifeTimeMax,
                                                                point->mMaxAngleDegrees,
                                                                ps->mParticleSize,
																ps->mParticleSize);
                }
                if ( emitter )
                {
                    particleNode->setEmitter(emitter);
                    emitter->drop();
                }
            }
        }
        break;
        case NT_SKYBOX:
            // TODO: maybe nothing to do here. Lets see if it works.
        break;
        default:
        break;
    }
}

AnimatorSettings* NodeManager::MakeAnimatorSettings(const char * type_)
{
    if ( type_ )
    {
        if ( 0 == strcmp(type_, "anirot") )
        {
            return MakeAnimatorSettings(AT_ROTATION);
        }
    }
    return NULL;
}

AnimatorSettings* NodeManager::MakeAnimatorSettings(ANIMATOR_TYPE type_)
{
    switch ( type_ )
    {
        case AT_ROTATION:
            return new AnimatorSettingsRotation();
        default:
            break;
    }

    return NULL;
}

NodeSettingsUnknown* NodeManager::MakeNodeSettings(const char * type_)
{
    if ( type_ )
    {
        if ( 0 == strcmp(type_, "unknown") )
        {
            return MakeNodeSettings(NT_UNKNOWN);
        }
        else if ( 0 == strcmp(type_, "mesh") )
        {
            return MakeNodeSettings(NT_MESH);
        }
        else if ( 0 == strcmp(type_, "anim_mesh") )
        {
            return MakeNodeSettings(NT_ANIMATED_MESH);
        }
        else if ( 0 == strcmp(type_, "light") )
        {
            return MakeNodeSettings(NT_LIGHT);
        }
        else if ( 0 == strcmp(type_, "particle") )
        {
            return MakeNodeSettings(NT_PARTICLE);
        }
        else if ( 0 == strcmp(type_, "skybox") )
        {
            return MakeNodeSettings(NT_SKYBOX);
        }
    }

    return NULL;
}

NodeSettingsUnknown* NodeManager::MakeNodeSettings(NODE_TYPE type_)
{
    switch ( type_ )
    {
        case NT_UNKNOWN:
            return new NodeSettingsUnknown();
        case NT_MESH:
            return new NodeSettingsMesh();
        case NT_ANIMATED_MESH:
            return new NodeSettingsAnimatedMesh();
        case NT_LIGHT:
            return new NodeSettingsLight();
        case NT_PARTICLE:
            return new NodeSettingsParticle();
        case NT_SKYBOX:
            return new NodeSettingsSkybox();
    }

    return NULL;
}

ParticleEffectorSettings* NodeManager::MakeParticleEffectorSettings(PARTICLE_AFFECTOR_TYPE type_)
{
    switch ( type_ )
    {
        case PAT_NONE:
            return new ParticleEffectorSettings();
        case PAT_FADE_OUT:
            return new ParticleEffectorSettingsFadeOut();
        case PAT_GRAVITY:
            return new ParticleEffectorSettingsGravity();
    }
    return NULL;
}

ParticleEffectorSettings* NodeManager::MakeParticleEffectorSettings(const char * type_)
{
    if ( type_ )
    {
        if ( 0 == strcmp(type_, "none") )
            return MakeParticleEffectorSettings(PAT_NONE);
        else if ( 0 == strcmp(type_, "fade") )
            return MakeParticleEffectorSettings(PAT_FADE_OUT);
        else if ( 0 == strcmp(type_, "gravity") )
            return MakeParticleEffectorSettings(PAT_GRAVITY);
    }
    return NULL;
}

ParticleEmitterSettings* NodeManager::MakeParticleEmitterSettings(PARTICLE_EMITTER_TYPE type_)
{
    switch ( type_ )
    {
        case PET_NONE:
            return new ParticleEmitterSettings();
        case PET_BOX:
            return new ParticleEmitterSettingsBox();
        case PET_POINT:
            return new ParticleEmitterSettingsPoint();
    }
    return NULL;
}

ParticleEmitterSettings* NodeManager::MakeParticleEmitterSettings(const char * type_)
{
    if ( type_ )
    {
        if ( 0 == strcmp(type_, "none") )
            return MakeParticleEmitterSettings(PET_NONE);
        else if ( 0 == strcmp(type_, "box") )
            return MakeParticleEmitterSettings(PET_BOX);
        else if ( 0 == strcmp(type_, "point") )
            return MakeParticleEmitterSettings(PET_POINT);
    }
    return NULL;
}
