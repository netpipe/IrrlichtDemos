// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "level.h"
#include "irrlicht_manager.h"
#include "main.h"
#include "config.h"
#include "physics.h"
#include "editor.h"
#include "mesh_texture_loader.h"
#include "node_manager.h"
#include "game.h"
#include "logging.h"
#include "helper_str.h"
#include "globals.h"
#include "tinyxml/tinyxml.h"
#include <math.h>
#include <float.h>

using namespace irr;

// --------------------------------------------------------
AiBotSettings::AiBotSettings()
{
    mName = "unknown bot";
    mModel = "hover_player";
    mLowPower = 1.f;
    mUppPower = 1.f;
    mSlowBorder = 0.9f;
    mLowSlow = 1.f;
    mUppSlow = 1.f;
    mLowEnergy = 1.f;
    mUppEnergy = 1.f;
    mLowIdeal = 1.f;
    mUppIdeal = 1.f;
    mCollTime = 1000;
    mCollTimeReturn = 2000;
    mStartTime = 1000;
    mLowPowerStart = 1.f;
    mUppPowerStart = 1.f;
    mPreviewDist1 = 150.f;
    mPreviewDist2 = 800.f;
}

void AiBotSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const char * val = settings_->Attribute("name");
    if ( val )
        mName = val;
    val = settings_->Attribute("model");
    if ( val )
        mModel = val;

    settings_->QueryFloatAttribute("pow_min", &mLowPower);
    settings_->QueryFloatAttribute("pow_max", &mUppPower);
    settings_->QueryFloatAttribute("slow_val", &mSlowBorder);
    settings_->QueryFloatAttribute("slow_min", &mLowSlow);
    settings_->QueryFloatAttribute("slow_max", &mUppSlow);
    settings_->QueryFloatAttribute("change_min", &mLowEnergy);
    settings_->QueryFloatAttribute("change_max", &mUppEnergy);
    settings_->QueryFloatAttribute("ideal_min", &mLowIdeal);
    settings_->QueryFloatAttribute("ideal_max", &mUppIdeal);
    settings_->QueryIntAttribute("coll_time", &mCollTime);
    settings_->QueryIntAttribute("coll_time_return", &mCollTimeReturn);
    settings_->QueryIntAttribute("start_time", &mStartTime);
    settings_->QueryFloatAttribute("start_min", &mLowPowerStart);
    settings_->QueryFloatAttribute("start_max", &mUppPowerStart);
    settings_->QueryFloatAttribute("preview_1", &mPreviewDist1);
    settings_->QueryFloatAttribute("preview_2", &mPreviewDist2);
}

// --------------------------------------------------------
void LevelCameraSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const char * val = settings_->Attribute("model");
    if ( val )
        mModel = val;
}

// --------------------------------------------------------
void TrackStartSettings::ReadFromXml(const TiXmlElement * settings_)
{
    assert(settings_);

    int valid=0;
    settings_->QueryIntAttribute("is_valid", &valid);
    mIsValid = valid ? true : false;

    settings_->QueryFloatAttribute("c_x", &mCenter.X);
    settings_->QueryFloatAttribute("c_y", &mCenter.Y);
    settings_->QueryFloatAttribute("c_z", &mCenter.Z);

    settings_->QueryFloatAttribute("r_x", &mRotation.X);
    settings_->QueryFloatAttribute("r_y", &mRotation.Y);
    settings_->QueryFloatAttribute("r_z", &mRotation.Z);
}

void TrackStartSettings::WriteToXml(TiXmlElement * settings_) const
{
    assert(settings_);

    settings_->SetAttribute("is_valid", mIsValid ? 1 : 0 );

    settings_->SetDoubleAttribute("c_x", mCenter.X);
    settings_->SetDoubleAttribute("c_y", mCenter.Y);
    settings_->SetDoubleAttribute("c_z", mCenter.Z);

    settings_->SetDoubleAttribute("r_x", mRotation.X);
    settings_->SetDoubleAttribute("r_y", mRotation.Y);
    settings_->SetDoubleAttribute("r_z", mRotation.Z);
}

// --------------------------------------------------------
TrackMarkerSettings::TrackMarkerSettings()
{
    Reset();
}

void TrackMarkerSettings::Reset()
{
    mIsValid = false;
    mUseDefaultSizes = true;
    mHasLeftWall = false;
    mHasRightWall = false;
//    mHasTopWall = false;
    mHasBottomWall = false;
    mRelocate = true;
}

void TrackMarkerSettings::EnableWalls()
{
    mHasLeftWall = true;
    mHasRightWall = true;
//    mHasTopWall = true;
    mHasBottomWall = true;
}

void TrackMarkerSettings::ReadFromXml(const TiXmlElement * settings_)
{
    assert(settings_);

    int val=0;
    settings_->QueryIntAttribute("is_valid", &val);
    mIsValid = val ? true : false;

    const char * valChar = settings_->Attribute("relocate");
    if ( valChar )
        mRelocate = atoi(valChar) ? true : false;

    settings_->QueryFloatAttribute("c_x", &mCenter.X);
    settings_->QueryFloatAttribute("c_y", &mCenter.Y);
    settings_->QueryFloatAttribute("c_z", &mCenter.Z);

    settings_->QueryFloatAttribute("u_x", &mUpVector.X);
    settings_->QueryFloatAttribute("u_y", &mUpVector.Y);
    settings_->QueryFloatAttribute("u_z", &mUpVector.Z);

    settings_->QueryFloatAttribute("r_x", &mRotation.X);
    settings_->QueryFloatAttribute("r_y", &mRotation.Y);
    settings_->QueryFloatAttribute("r_z", &mRotation.Z);

    valChar = settings_->Attribute("def_sizes");
    if ( valChar )
        mUseDefaultSizes = atoi(valChar) ? true : false;

    settings_->QueryFloatAttribute("left", &mLeft);
    settings_->QueryFloatAttribute("right", &mRight);
    settings_->QueryFloatAttribute("top", &mTop);
    settings_->QueryFloatAttribute("bottom", &mBottom);

    settings_->QueryIntAttribute("wall_left", &val);
    mHasLeftWall = val ? true : false;
    settings_->QueryIntAttribute("wall_right", &val);
    mHasRightWall = val ? true : false;
//    settings_->QueryIntAttribute("wall_top", &val);
//    mHasTopWall = val ? true : false;
    settings_->QueryIntAttribute("wall_bottom", &val);
    mHasBottomWall = val ? true : false;
}

void TrackMarkerSettings::WriteToXml(TiXmlElement * settings_) const
{
    assert(settings_);

    settings_->SetAttribute("is_valid", mIsValid ? 1 : 0 );

    settings_->SetAttribute("relocate", mRelocate ? 1 : 0 );

    settings_->SetDoubleAttribute("c_x", mCenter.X);
    settings_->SetDoubleAttribute("c_y", mCenter.Y);
    settings_->SetDoubleAttribute("c_z", mCenter.Z);

    settings_->SetDoubleAttribute("u_x", mUpVector.X);
    settings_->SetDoubleAttribute("u_y", mUpVector.Y);
    settings_->SetDoubleAttribute("u_z", mUpVector.Z);

    settings_->SetDoubleAttribute("r_x", mRotation.X);
    settings_->SetDoubleAttribute("r_y", mRotation.Y);
    settings_->SetDoubleAttribute("r_z", mRotation.Z);

    settings_->SetAttribute("def_sizes", mUseDefaultSizes ? 1 : 0 );
    settings_->SetDoubleAttribute("left", mLeft);
    settings_->SetDoubleAttribute("right", mRight);
    settings_->SetDoubleAttribute("top", mTop);
    settings_->SetDoubleAttribute("bottom", mBottom);

    settings_->SetAttribute("wall_left", mHasLeftWall ? 1 : 0 );
    settings_->SetAttribute("wall_right", mHasRightWall? 1 : 0 );
//    settings_->SetAttribute("wall_top", mHasTopWall ? 1 : 0 );
    settings_->SetAttribute("wall_bottom", mHasBottomWall ? 1 : 0 );
}

// --------------------------------------------------------
bool TrackStart::GetCenter( core::vector3df &pos_)
{
    if ( !mSettings.mIsValid )
        return false;
    pos_ = mSettings.mCenter;
    return true;
}

// --------------------------------------------------------
LevelModelSettings::LevelModelSettings()
{
    mScale.set(1.f, 1.f, 1.f);
    mAmbientRed = 255;
    mAmbientGreen = 255;
    mAmbientBlue = 255;
    mHasCollisions = false;
}

void LevelModelSettings::ReadFromXml(const TiXmlElement * settings_)
{
    assert(settings_);

    const char * val = settings_->Attribute("name");
    if ( val )
        mName = val;

    settings_->QueryFloatAttribute("c_x", &mCenter.X);
    settings_->QueryFloatAttribute("c_y", &mCenter.Y);
    settings_->QueryFloatAttribute("c_z", &mCenter.Z);

    settings_->QueryFloatAttribute("r_x", &mRotation.X);
    settings_->QueryFloatAttribute("r_y", &mRotation.Y);
    settings_->QueryFloatAttribute("r_z", &mRotation.Z);

    settings_->QueryFloatAttribute("s_x", &mScale.X);
    settings_->QueryFloatAttribute("s_y", &mScale.Y);
    settings_->QueryFloatAttribute("s_z", &mScale.Z);

    settings_->QueryIntAttribute("ar", &mAmbientRed);
    settings_->QueryIntAttribute("ag", &mAmbientGreen);
    settings_->QueryIntAttribute("ab", &mAmbientBlue);

    int iVal=0;
    settings_->QueryIntAttribute("coll", &iVal);
    mHasCollisions = iVal ? true : false;
}

void LevelModelSettings::WriteToXml(TiXmlElement * settings_) const
{
    assert(settings_);

    settings_->SetAttribute("name", mName.c_str() );

    settings_->SetDoubleAttribute("c_x", mCenter.X);
    settings_->SetDoubleAttribute("c_y", mCenter.Y);
    settings_->SetDoubleAttribute("c_z", mCenter.Z);

    settings_->SetDoubleAttribute("r_x", mRotation.X);
    settings_->SetDoubleAttribute("r_y", mRotation.Y);
    settings_->SetDoubleAttribute("r_z", mRotation.Z);

    settings_->SetDoubleAttribute("s_x", mScale.X);
    settings_->SetDoubleAttribute("s_y", mScale.Y);
    settings_->SetDoubleAttribute("s_z", mScale.Z);

    settings_->SetAttribute("ar", mAmbientRed);
    settings_->SetAttribute("ag", mAmbientGreen);
    settings_->SetAttribute("ab", mAmbientBlue);

    settings_->SetAttribute("coll", mHasCollisions ? 1 : 0 );
}

// --------------------------------------------------------
LevelModel::LevelModel()
: mNodeModel(0)
{
}

bool LevelModel::GetCenter( core::vector3df &pos_)
{
    pos_ = mSettings.mCenter;
    return true;
}

// --------------------------------------------------------
TrackMarker::TrackMarker()
: mEditNodeCenter(0)
, mEditNodeLeftTop(0)
, mEditNodeRightBottom(0)
, mNodeCollision(0)
, mNodeWallLeft(0)
, mNodeWallRight(0)
, mNodeWallTop(0)
, mNodeWallBottom(0)
{
}

bool TrackMarker::GetCenter( core::vector3df &pos_)
{
    if ( !mSettings.mIsValid )
        return false;
    pos_ = mSettings.mCenter;
    return true;
}

// --------------------------------------------------------
LevelSettings::LevelSettings()
{
    mId = 0;

    mLaps = 3;
    mTargetTime = 600.f;
    mDifficulty = LD_EASY;

    mAmbientAlpha = 150;
    mAmbientRed = 150;
    mAmbientGreen = 150;
    mAmbientBlue = 150;

    mAmbHoverRed = 255;
    mAmbHoverGreen = 255;
    mAmbHoverBlue = 255;

    mDefaultMarkerLeft = -500.f;
    mDefaultMarkerRight = 500.f;
    mDefaultMarkerTop = 300.f;
    mDefaultMarkerBottom = -300.f;

    mHudPosScaling = 1.f;
}

void LevelSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    settings_->Attribute("id", &mId);
    const char * val = settings_->Attribute("name");
    if ( val )
    {
        std::string str(val);
        mName = ExtStr::FromUtf8(str);
    }
    val = settings_->Attribute("obj_file");
    if ( val )
        mMeshFile = val;
    val = settings_->Attribute("dat_file");
    if ( val )
        mDataFile = val;
    val = settings_->Attribute("lm_file");
    if ( val )
        mLightmapFile = val;
    val = settings_->Attribute("amb_alpha");
    if ( val )
        mAmbientAlpha = atoi(val);
    val = settings_->Attribute("amb_red");
    if ( val )
        mAmbientRed = atoi(val);
    val = settings_->Attribute("amb_green");
    if ( val )
        mAmbientGreen = atoi(val);
    val = settings_->Attribute("amb_blue");
    if ( val )
        mAmbientBlue = atoi(val);

    val = settings_->Attribute("hover_red");
    if ( val )
        mAmbHoverRed = atoi(val);
    val = settings_->Attribute("hover_green");
    if ( val )
        mAmbHoverGreen = atoi(val);
    val = settings_->Attribute("hover_blue");
    if ( val )
        mAmbHoverBlue = atoi(val);

    val = settings_->Attribute("laps");
    if ( val )
        mLaps = atoi(val);
    settings_->QueryFloatAttribute("targettime", &mTargetTime);

    val = settings_->Attribute("difficulty");
    if ( val )
    {
        int d = atoi(val);
        if (d >= LD_EASY && d <= LD_HARD )
            mDifficulty  = (LevelDifficulty)d;
        else
            mDifficulty = LD_EASY;
    }

    settings_->QueryFloatAttribute("hud_pos_scaling", &mHudPosScaling);
    settings_->QueryFloatAttribute("marker_left", &mDefaultMarkerLeft);
    settings_->QueryFloatAttribute("marker_right", &mDefaultMarkerRight);
    settings_->QueryFloatAttribute("marker_top", &mDefaultMarkerTop);
    settings_->QueryFloatAttribute("marker_bottom", &mDefaultMarkerBottom);

    int botId = 0;
    const TiXmlNode * nodeAiBot = settings_->IterateChildren("ai_bot", NULL);
    while ( nodeAiBot && botId < MAX_BOT_SETTINGS)
    {
        mBotSettings[botId].ReadFromXml(nodeAiBot->ToElement());
        ++botId;
        nodeAiBot = settings_->IterateChildren("ai_bot", nodeAiBot);
    }

    const TiXmlNode * nodeCam = settings_->IterateChildren("cam_node", NULL);
    if ( nodeCam )
    {
        mCameraSettings.ReadFromXml( nodeCam->ToElement() );
    }
}

// --------------------------------------------------------
LevelManager::LevelManager()
{
    mLevelNode = NULL;
    mSelector = NULL;
    mCurrentLevel = -1;
    mEditDataNode = NULL;
    mTrackDataNode = NULL;
    mNodeCamera = NULL;
}

LevelManager::~LevelManager()
{
    if ( APP.GetIrrlichtManager()->GetIrrlichtDevice() )
    {
        if ( mSelector )
        {
            mSelector->drop();
        }
    }
}

void LevelManager::LoadSettings()
{
	HC1_PROFILE(CProfileScope ps1(L"LvlMan:LoadSet", L"STARTUP");)

    std::string xmlName ( APP.GetConfig()->GetPathLevels() + "levels.xml" );
    TiXmlDocument xmlDoc(xmlName.c_str());
    xmlDoc.SetIrrFs(APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem(), TiXmlDocument::E_ON_READ_ANDROID);

    if ( !xmlDoc.LoadFile() )
    {
		LOG.DebugLn("TiXmlDocument::ErrorDesc: ", xmlDoc.ErrorDesc());
		LOG.WarnLn("LevelManager::LoadSettings: could not load xml ", xmlName.c_str());
        return;
    }

    TiXmlNode * levelsNode = xmlDoc.FirstChild("levellist");
	if (!levelsNode)
		return;

    TiXmlNode * defaultNode = levelsNode->FirstChild("default");
    if ( defaultNode )
    {
        mDefaultLevelSettings.ReadFromXml(defaultNode->ToElement());
    }

    mLevelSettings.clear();
    TiXmlNode* nodeLevel = levelsNode->IterateChildren("level", NULL);
    while ( nodeLevel )
    {
        LevelSettings settings;
        settings = mDefaultLevelSettings;
        settings.ReadFromXml(nodeLevel->ToElement());
        mLevelSettings.push_back(settings);

        nodeLevel = levelsNode->IterateChildren("level", nodeLevel);
    }
}

bool LevelManager::LoadLevel(const std::wstring &levelName_)
{
    int index = -1;
    for ( unsigned int i=0; i<mLevelSettings.size(); ++i)
    {
        if ( mLevelSettings[i].mName == levelName_ )
        {
            index = (int)i;
            break;
        }
    }

    if ( index < 0 )
        return false;

    mCurrentLevel = index;

    return Load(&(mLevelSettings[index]));
}

int LevelManager::GetNumLevels(LevelDifficulty difficulty_) const
{
    if ( LD_ALL == difficulty_ )
        return mLevelSettings.size();

    int count = 0;
    for ( unsigned int i=0; i < mLevelSettings.size(); ++i )
    {
        if ( mLevelSettings[i].mDifficulty == difficulty_ )
        {
            ++count;
        }
    }
    return count;
}

unsigned int LevelManager::GetRealIndex( unsigned int index_, LevelDifficulty difficulty_) const
{
    if ( index_ >= mLevelSettings.size() || difficulty_ == LD_ALL )
        return index_;

    unsigned int count = 0;
    for ( unsigned int i=0; i < mLevelSettings.size(); ++i )
    {
        if ( mLevelSettings[i].mDifficulty == difficulty_ )
        {
            if ( count == index_ )
            {
                return i;
            }
            ++count;
        }
    }
    return index_;
}

const LevelSettings& LevelManager::GetLevel(unsigned int index_, LevelDifficulty difficulty_) const
{
    index_ = GetRealIndex( index_, difficulty_ );
    if ( index_ >= mLevelSettings.size() )
    {
        return mDefaultLevelSettings;
    }

    return mLevelSettings[index_];
}

int LevelManager::GetIndexForId(int id_) const
{
    for ( unsigned int i=0; i < mLevelSettings.size(); ++i )
    {
        if ( mLevelSettings[i].mId == id_ )
        {
            return i;
        }
    }

    return -1;
}

int LevelManager::GetIndexForName( const std::wstring &levelName_ ) const
{
    LOG.Log(LP_DEBUG, "GetIndexForName - levelName: ");
    LOG.Log(LP_DEBUG, levelName_);
    LOG.Log(LP_DEBUG, "\n");
    for ( unsigned int i=0; i < mLevelSettings.size(); ++i )
    {
        if ( mLevelSettings[i].mName == levelName_ )
        {
            return i;
        }
    }

    return -1;
}

void LevelManager::SetMeshScale(const core::vector3df &scale_)
{
    if ( mLevelNode )
        mLevelNode->setScale(scale_);
}

bool LevelManager::Load(LevelSettings * settings_)
{
    LOG.Debug("LevelManager::Load\n");

    if ( !settings_ )
    {
        LOG.Warn("Level::Load - settings_ is NULL\n");
        return false;
    }

	MeshTextureLoader* staticMeshTextureLoader = APP.GetIrrlichtManager()->GetStaticMeshTextureLoader();

    ClearTrackData();

    APP.GetPhysics()->ClearCollisionMeshes();

	if (mLevelNode)
	{
	    std::string newLevelName = APP.GetConfig()->MakeFilenameLevel(settings_->mMeshFile);
	    if( newLevelName != mLevelFileName )
	    {
	        scene::IAnimatedMesh * mesh = APP.GetIrrlichtManager()->GetMeshInCache( mLevelFileName.c_str() );
	        if ( mesh )
	        {
                APP.GetIrrlichtManager()->GetSceneManager()->getMeshCache()->removeMesh(mesh);
	        }

			if ( staticMeshTextureLoader )
			{
				staticMeshTextureLoader->clearTextureCache();
				staticMeshTextureLoader->setTextureCaching(MeshTextureLoader::ADD_TEXTURES);
			}
	    }

		mLevelNode->remove();
        mLevelNode = 0;
	}

    if ( mSelector)
    {
        mSelector->drop();
        mSelector = NULL;
    }

    if ( !mEditDataNode )
    {
        mEditDataNode = APP.GetIrrlichtManager()->GetSceneManager()->addEmptySceneNode();
        mEditDataNode->setVisible(false);
    }
    if ( !mTrackDataNode )
    {
        mTrackDataNode = APP.GetIrrlichtManager()->GetSceneManager()->addEmptySceneNode();
        mTrackDataNode->setVisible(false);
    }

    LOG.Debug("old level removed\n");

    mLevelFileName = APP.GetConfig()->MakeFilenameLevel(settings_->mMeshFile);
    LOG.LogLn( LP_DEBUG, "Load level ", mLevelFileName.c_str() );

    mLevelNode = APP.GetIrrlichtManager()->LoadModel(*APP.GetConfig(), mLevelFileName.c_str(), &mSelector);

    mLevelNode->setPosition(core::vector3df(0,0,0));
    mLevelNode->setRotation(core::vector3df(0,0,0));

	if ( staticMeshTextureLoader )
	{
		staticMeshTextureLoader->setTextureCaching(MeshTextureLoader::REMOVE_TEXTURES);
	}

    APP.GetIrrlichtManager()->GetSceneManager()->setAmbientLight(video::SColor(settings_->mAmbientAlpha, settings_->mAmbientRed, settings_->mAmbientGreen, settings_->mAmbientBlue));

    if ( mNodeCamera )
    {
        mNodeCamera->remove();
        mNodeCamera = NULL;
    }
    if ( APP.GetConfig()->GetParticleMode() == EPM_ON)
    {
        mNodeCamera = APP.GetNodeManager()->LoadNode( settings_->mCameraSettings.mModel );
    }
    APP.GetIrrlichtManager()->SetShadersUseDynamicLight(APP.GetConfig()->GetLightMode() == ELM_DYNAMIC);

    LOG.Debug("add level physics\n");
    APP.GetPhysics()->AddCollisionMesh(mSelector);

    LOG.Debug("load track data\n");
    LoadTrackData(settings_->mDataFile);

    LOG.Debug("build track data\n");
    BuildTrackData(settings_);

    APP.GetGame()->UpdateHoverAmbience(*settings_);

    LOG.Debug("level loaded\n");

    return true;
}

void LevelManager::BuildTrackData(LevelSettings * settings_)
{
    assert(settings_);
    for ( int i=0; i < MAX_SPAWNS; ++i )
    {
        SetTrackStart(GetTrackStart(i).mSettings, i);
    }

    for ( int i=0; i < (int)mTrackMarkers.size(); ++i )
    {
        SetTrackMarker(mTrackMarkers[i].mSettings, i);
    }

    SetFinishLine(mFinishLine.mSettings);
    SetTeleportSource(mTpSource.mSettings);
    SetTeleportTarget(mTpTarget.mSettings);

    for ( int i=0; i <(int)mModels.size(); ++i )
    {
        SetModel(mModels[i].mSettings, i);
    }

    mAiTrack.CalculateCushions();
}

void LevelManager::ClearTrackData()
{
    for ( int i=0; i < MAX_SPAWNS; ++i )
    {
        if ( GetTrackStart(i).mEditNode )
        {
            GetTrackStart(i).mEditNode->remove();
            GetTrackStart(i).mEditNode = NULL;
            GetTrackStart(i).mSettings.Reset();
        }
    }
    for ( int i=0; i < (int)mTrackMarkers.size(); ++i )
    {
        ClearTrackMarkerData(mTrackMarkers[i]);
    }
    mTrackMarkers.clear();
    ClearTrackMarkerData(mFinishLine);
    ClearTrackMarkerData(mTpSource);
    ClearTrackMarkerData(mTpTarget);
    for ( int i=0; i < (int)mModels.size(); ++i )
    {
        ClearModelData(mModels[i]);
    }
    mModels.clear();
}

void LevelManager::ClearTrackMarkerData(TrackMarker & marker_)
{
    if ( marker_.mEditNodeCenter )
    {
        marker_.mEditNodeCenter->remove();
        marker_.mEditNodeCenter = NULL;
    }
    if ( marker_.mEditNodeLeftTop )
    {
        marker_.mEditNodeLeftTop->remove();
        marker_.mEditNodeLeftTop = NULL;
    }
    if ( marker_.mEditNodeRightBottom )
    {
        marker_.mEditNodeRightBottom->remove();
        marker_.mEditNodeRightBottom = NULL;
    }
    if ( marker_.mNodeCollision )
    {
        marker_.mNodeCollision->remove();
        marker_.mNodeCollision = NULL;
    }
    RemoveCollisionWalls(marker_);

    marker_.mSettings.Reset();
}

void LevelManager::ClearModelData(LevelModel & model_)
{
    if ( model_.mNodeModel )
    {
        if ( model_.mSettings.mHasCollisions )
        {
            APP.GetPhysics()->RemoveCollisionMesh(model_.mNodeModel->getTriangleSelector());
        }

        model_.mNodeModel->remove();
        model_.mNodeModel = NULL;

        APP.GetNodeManager()->UnloadNode(model_.mSettings.mName);
    }
}

void LevelManager::ShowEditData(bool enable_)
{
    if ( enable_)
    {
        if ( mEditDataNode )
            mEditDataNode->setVisible(true);
        if ( mTrackDataNode )
            mTrackDataNode->setVisible(true);
    }
    else
    {
        if ( mEditDataNode )
            mEditDataNode->setVisible(false);
        if ( mTrackDataNode )
            mTrackDataNode->setVisible(false);
    }
}

void LevelManager::SetTrackStart(const TrackStartSettings& ts_, int player_)
{
    if ( ts_.mIsValid )
    {
#ifdef HC1_ENABLE_EDITOR
        scene::ISceneNode * node = GetTrackStart(player_).mEditNode;
        if ( !node )
        {
            scene::SMesh * mesh = APP.GetEditor()->GetDefaultArrowMesh();
            node = APP.GetIrrlichtManager()->GetSceneManager()->addMeshSceneNode(mesh, mEditDataNode, /*id*/1, ts_.mCenter);
            assert(node);

            scene::ITriangleSelector* selector = APP.GetIrrlichtManager()->GetSceneManager()->createTriangleSelector(mesh, node);
            node->setTriangleSelector(selector);
            selector->drop();

            GetTrackStart(player_).mEditNode = node;
        }
        node->setPosition(ts_.mCenter);
        node->setRotation(ts_.mRotation);
        node->updateAbsolutePosition();
#endif
        GetTrackStart(player_).mSettings = ts_;
    }
    else    // !ts_.mIsValid
    {
        GetTrackStart(player_).mSettings.Reset();
    }
}

TrackStart& LevelManager::GetTrackStart(unsigned int index_)
{
    assert( index_ < MAX_SPAWNS );
    return mTrackStarts[index_];
}

unsigned int LevelManager::GetNrOfTrackMarkers() const
{
    return mTrackMarkers.size();
}

void LevelManager::SetTrackMarker(const TrackMarkerSettings &markerSettings_, int index_)
{
    SetTrackMarkerSettings(markerSettings_, mTrackMarkers[index_]);
    if ( index_ > 0 )
    {
        SetCollisionWalls(mTrackMarkers[index_-1], mTrackMarkers[index_]);
    }
    if ( index_ < (int)mTrackMarkers.size() -1 )
    {
        SetCollisionWalls(mTrackMarkers[index_], mTrackMarkers[index_+1]);
    }
    if( mTrackMarkers.size() > 1 && (index_ == 0 || index_ == (int)mTrackMarkers.size()-1) )
    {
        SetCollisionWalls(mTrackMarkers[mTrackMarkers.size()-1], mTrackMarkers[0]);
    }
}

void LevelManager::InsertTrackMarker(const TrackMarkerSettings &markerSettings_, int index_)
{
    TrackMarker marker;
    SetTrackMarkerSettings(markerSettings_, marker);
    mTrackMarkers.insert( mTrackMarkers.begin() + index_, marker);
    if ( index_ > 0 )
    {
        SetCollisionWalls(mTrackMarkers[index_-1], mTrackMarkers[index_]);
    }
    if ( index_ < (int)mTrackMarkers.size() -1 )
    {
        SetCollisionWalls(mTrackMarkers[index_], mTrackMarkers[index_+1]);
    }
    if( mTrackMarkers.size() > 1 && index_ == 0 )
    {
        SetCollisionWalls(mTrackMarkers[mTrackMarkers.size()-1], mTrackMarkers[0]);
    }
}

void LevelManager::AppendTrackMarker(const TrackMarkerSettings &markerSettings_, int index_)
{
    TrackMarker marker;
    SetTrackMarkerSettings(markerSettings_, marker);
    mTrackMarkers.push_back(marker);
    if ( mTrackMarkers.size() > 1 )
    {
        SetCollisionWalls(mTrackMarkers[mTrackMarkers.size()-2], mTrackMarkers[mTrackMarkers.size()-1]);
        SetCollisionWalls(mTrackMarkers[mTrackMarkers.size()-1], mTrackMarkers[0]);
    }
}

void LevelManager::RotateTrackMarker(float angle_, int index_)
{
    if ( index_ < 0 || index_ >= (int)mTrackMarkers.size() )
        return;

    mTrackMarkers[index_].mSettings.mRotation.Y += 180.f;
    SetTrackMarker(mTrackMarkers[index_].mSettings, index_);
}

void LevelManager::AutoAlignTrackMarker(int index_)
{
#ifdef HC1_ENABLE_EDITOR
    if ( index_ < 0 || index_ >= (int)mTrackMarkers.size() )
        return;

    core::vector3df center = mTrackMarkers[index_].mSettings.mCenter;
    float angle_step_size = 18;
    float radius = 400.f;
    core::vector3df rotBase(radius, 0, 0);
    float angle=0.f;
    int switchCount = 0;
    bool firstCheckNoCollision = false;
    float switchAngles[2];
    while ( angle < 360.f )
    {
        core::line3d<f32> line;
        core::vector3df rotVec(rotBase);
        core::matrix4 rotMat;
        rotMat.setRotationDegrees( core::vector3df(0, angle, 0) );
        rotMat.rotateVect(rotVec);

        line.start = center+rotVec;
        line.end = line.start;
        line.start.Y += 150;
        line.end.Y -= 300;

        core::vector3df intersection;
        if ( APP.GetEditor()->CheckTrackCollision(line, intersection) )
        {
            if ( switchCount == 1 )
                angle = 360.f;
        }
        else
        {
            if ( angle == 0 )
                firstCheckNoCollision = true;
            switchAngles[switchCount] = angle;
            if ( switchCount == 0 )
            {
                ++switchCount;
                switchAngles[switchCount] = angle;
            }
        }

        angle += angle_step_size;
    }
    if ( firstCheckNoCollision )
    {
        angle = 360.f;

        while ( angle > 0.f )
        {
            core::line3d<f32> line;
            core::vector3df rotVec(rotBase);
            core::matrix4 rotMat;
            rotMat.setRotationDegrees( core::vector3df(0, angle, 0) );
            rotMat.rotateVect(rotVec);

            line.start = center+rotVec;
            line.end = line.start;
            line.start.Y += 150;
            line.end.Y -= 300;

            core::vector3df intersection;
            if ( APP.GetEditor()->CheckTrackCollision(line, intersection) )
                angle = 0.f;
            else
                switchAngles[0] = angle;

            angle -= angle_step_size;
        }
    }
    if ( switchCount > 0 )
    {
        if ( switchAngles[0] > switchAngles[1] + 180.f )
            switchAngles[1] += 360.f;
        else if ( switchAngles[1] > switchAngles[0] + 180.f )
            switchAngles[0] += 360.f;
        mTrackMarkers[index_].mSettings.mRotation.Y = (int)((switchAngles[0] + switchAngles[1]) * 0.5f) % 360;
        SetTrackMarker(mTrackMarkers[index_].mSettings, index_);
    }
#endif // HC1_ENABLE_EDITOR
}

void LevelManager::ChangeTrackMarkerSizes(bool useDefault_, float changeLeft_, float changeRight_, float changeTop_, float changeBottom_, int index_)
{
    assert ( index_ >= 0 && index_ < (int)mTrackMarkers.size() );
    TrackMarkerSettings & settings = mTrackMarkers[index_].mSettings;
    if ( useDefault_ )
    {
        settings.mUseDefaultSizes = true;
    }
    else
    {
        if ( settings.mUseDefaultSizes )
        {
            assert( mCurrentLevel >= 0 );
            settings.mLeft = mLevelSettings[mCurrentLevel].mDefaultMarkerLeft;
            settings.mRight = mLevelSettings[mCurrentLevel].mDefaultMarkerRight;
            settings.mTop = mLevelSettings[mCurrentLevel].mDefaultMarkerTop;
            settings.mBottom = mLevelSettings[mCurrentLevel].mDefaultMarkerBottom;
        }
        settings.mUseDefaultSizes = false;
        settings.mLeft += changeLeft_;
        settings.mRight += changeRight_;
        settings.mTop += changeTop_;
        settings.mBottom += changeBottom_;
    }
    SetTrackMarker(settings, index_);
}

void LevelManager::RemoveTrackMarker(int index_)
{
    assert ( index_ >= 0 && index_ < (int)mTrackMarkers.size() );
    ClearTrackMarkerData(mTrackMarkers[index_]);
    mTrackMarkers.erase( mTrackMarkers.begin() + index_);
    if ( index_ > 0 )
    {
        if ( index_ < (int)mTrackMarkers.size() )
        {
            SetCollisionWalls(mTrackMarkers[index_-1], mTrackMarkers[index_]);
        }
        else
        {
            RemoveCollisionWalls(mTrackMarkers[index_-1]);
        }
    }
    if( mTrackMarkers.size() > 1 && (index_ == 0 || index_ >= (int)mTrackMarkers.size()) )
    {
        SetCollisionWalls(mTrackMarkers[mTrackMarkers.size()-1], mTrackMarkers[0]);
    }
    else if ( mTrackMarkers.size() == 1 )
    {
        RemoveCollisionWalls(mTrackMarkers[0]);
    }
}

void LevelManager::SetTeleportSource(const TrackMarkerSettings &markerSettings_)
{
    SetTrackMarkerSettings(markerSettings_, mTpSource);
}

void LevelManager::SetTeleportTarget(const TrackMarkerSettings &markerSettings_)
{
    SetTrackMarkerSettings(markerSettings_, mTpTarget);
}

void LevelManager::SetFinishLine(const TrackMarkerSettings &markerSettings_)
{
    SetTrackMarkerSettings(markerSettings_, mFinishLine);
}

void LevelManager::AddModel(const std::string &name_)
{
    LevelModel model;
    model.mSettings.mName = name_;

    model.mNodeModel = APP.GetNodeManager()->LoadNode(name_);

    mModels.push_back(model);
}

void LevelManager::SetModel(const LevelModelSettings &settings_, int index_)
{
    LevelModel & model = mModels[index_];
    model.mSettings = settings_;
    if ( model.mNodeModel )
    {
        model.mNodeModel->setPosition(model.mSettings.mCenter);
        model.mNodeModel->setRotation(model.mSettings.mRotation);
        model.mNodeModel->updateAbsolutePosition();

        if ( model.mSettings.mHasCollisions )
        {
            APP.GetPhysics()->RemoveCollisionMesh(model.mNodeModel->getTriangleSelector());
            APP.GetPhysics()->AddCollisionMesh(model.mNodeModel->getTriangleSelector());
        }

        APP.GetIrrlichtManager()->SetNodeAndChildsAmbientMaterial(model.mNodeModel, model.mSettings.mAmbientRed, model.mSettings.mAmbientGreen, model.mSettings.mAmbientBlue, 0);
    }
}

void LevelManager::ChangeModelRotation(float rotX_, float rotY_, float rotZ_, int index_)
{
    LevelModel & model = mModels[index_];
    model.mSettings.mRotation.X += rotX_;
    model.mSettings.mRotation.Y += rotY_;
    model.mSettings.mRotation.Z += rotZ_;
    SetModel(model.mSettings, index_);
}

void LevelManager::ChangeModelPosition(float posX_, float posY_, float posZ_, int index_)
{
    LevelModel & model = mModels[index_];
    model.mSettings.mCenter.X += posX_;
    model.mSettings.mCenter.Y += posY_;
    model.mSettings.mCenter.Z += posZ_;
    SetModel(model.mSettings, index_);
}

void LevelManager::SetModelAmbient(int ambR_, int ambG_, int ambB_, int index_)
{
    LevelModel & model = mModels[index_];
    model.mSettings.mAmbientRed = ambR_;
    model.mSettings.mAmbientGreen = ambG_;
    model.mSettings.mAmbientBlue = ambB_;
    SetModel(model.mSettings, index_);
}

void LevelManager::RemoveModel(int index_)
{
    if ( index_ < 0 || index_ >= (int)mModels.size() )
        return;
    ClearModelData(mModels[index_]);
    mModels.erase( mModels.begin() + index_);
}

void LevelManager::SetCollisionWalls(TrackMarker & marker1_, TrackMarker & marker2_ )
{
    RemoveCollisionWalls(marker1_);

    core::vector3df leftTopVec1, rightBottomVec1, rightTopVec1, leftBottomVec1;
    CalcMarkerBorders(marker1_, leftTopVec1, rightTopVec1, leftBottomVec1, rightBottomVec1, false);
    core::vector3df leftTopVec2, rightBottomVec2, rightTopVec2, leftBottomVec2;
    CalcMarkerBorders(marker2_, leftTopVec2, rightTopVec2, leftBottomVec2, rightBottomVec2, false);

    if ( marker1_.mSettings.mHasLeftWall )
    {
        marker1_.mNodeWallLeft = AddQuadradicNode(mTrackDataNode, leftTopVec1, leftTopVec2, leftBottomVec1, leftBottomVec2 );
    }
    if ( marker1_.mSettings.mHasRightWall )
    {
        marker1_.mNodeWallRight = AddQuadradicNode(mTrackDataNode, rightTopVec1, rightTopVec2, rightBottomVec1, rightBottomVec2 );
    }
    if ( marker1_.mSettings.mHasBottomWall )
    {
        marker1_.mNodeWallBottom = AddQuadradicNode(mTrackDataNode, leftBottomVec1, rightBottomVec1, leftBottomVec2, rightBottomVec2 );
    }
}

void LevelManager::RemoveCollisionWalls(TrackMarker & marker_)
{
    if ( marker_.mNodeWallLeft )
    {
        marker_.mNodeWallLeft->remove();
        marker_.mNodeWallLeft = NULL;
    }
    if ( marker_.mNodeWallRight )
    {
        marker_.mNodeWallRight->remove();
        marker_.mNodeWallRight = NULL;
    }
    if ( marker_.mNodeWallTop )
    {
        marker_.mNodeWallTop->remove();
        marker_.mNodeWallTop = NULL;
    }
    if ( marker_.mNodeWallBottom )
    {
        marker_.mNodeWallBottom->remove();
        marker_.mNodeWallBottom = NULL;
    }
}

void LevelManager::CalcMarkerBorders(const TrackMarker & marker_, core::vector3df & leftTop_, core::vector3df & rightTop_, core::vector3df & leftBottom_, core::vector3df & rightBottom_, bool relative_)
{
    core::matrix4 rotMat;
    rotMat.setRotationDegrees(marker_.mSettings.mRotation);
    core::vector3df forward(0, 0, 1);
    rotMat.rotateVect(forward);
    float left, right, top, bottom;
    if ( marker_.mSettings.mUseDefaultSizes )
    {
        assert(mCurrentLevel >= 0 && mCurrentLevel < (int)mLevelSettings.size());
        left = mLevelSettings[mCurrentLevel].mDefaultMarkerLeft;
        right = mLevelSettings[mCurrentLevel].mDefaultMarkerRight;
        top = mLevelSettings[mCurrentLevel].mDefaultMarkerTop;
        bottom = mLevelSettings[mCurrentLevel].mDefaultMarkerBottom;
    }
    else
    {
        left = marker_.mSettings.mLeft;
        right = marker_.mSettings.mRight;
        top = marker_.mSettings.mTop;
        bottom = marker_.mSettings.mBottom;
    }

    core::vector3df leftVec(marker_.mSettings.mUpVector.crossProduct(forward));
//    printf("up: %.3f %.3f %.3f\n", marker_.mSettings.mUpVector.X, marker_.mSettings.mUpVector.Y, marker_.mSettings.mUpVector.Z);
//    printf("forward: %.3f %.3f %.3f\n", forward.X, forward.Y, forward.Z );
//    printf("leftVec: %.3f %.3f %.3f\n", leftVec.X, leftVec.Y, leftVec.Z );

    leftTop_ = (leftVec * left + marker_.mSettings.mUpVector * top);
    rightBottom_ = (leftVec * right + marker_.mSettings.mUpVector * bottom);
    rightTop_ = (leftVec * right + marker_.mSettings.mUpVector * top);
    leftBottom_ = (leftVec * left + marker_.mSettings.mUpVector * bottom);

    if ( !relative_ )
    {
        leftTop_ += marker_.mSettings.mCenter;
        rightBottom_ += marker_.mSettings.mCenter;
        rightTop_ += marker_.mSettings.mCenter;
        leftBottom_ += marker_.mSettings.mCenter;
    }
}

void LevelManager::SetTrackMarkerSettings(const TrackMarkerSettings &settings_, TrackMarker & marker_)
{
    if ( settings_.mIsValid )
    {
        marker_.mSettings = settings_;

        core::vector3df leftTopVec, rightBottomVec, rightTopVec, leftBottomVec;
        CalcMarkerBorders(marker_, leftTopVec, rightTopVec, leftBottomVec, rightBottomVec );

        scene::ISceneNode * nodeCollision = marker_.mNodeCollision;
#ifdef HC1_ENABLE_EDITOR
        scene::ISceneNode * nodeCenter = marker_.mEditNodeCenter;
        scene::ISceneNode * nodeLeftTop = marker_.mEditNodeLeftTop;
        scene::ISceneNode * nodeRightBottom = marker_.mEditNodeRightBottom;
        if ( !nodeCenter )
        {
            scene::SMesh * mesh = APP.GetEditor()->GetDefaultArrowMesh();
            nodeCenter = APP.GetIrrlichtManager()->GetSceneManager()->addMeshSceneNode(mesh, mEditDataNode);
            assert(nodeCenter);

            scene::ITriangleSelector* selector = APP.GetIrrlichtManager()->GetSceneManager()->createTriangleSelector(mesh, nodeCenter);
            nodeCenter->setTriangleSelector(selector);
            selector->drop();

            marker_.mEditNodeCenter = nodeCenter;
        }
        if ( !nodeLeftTop )
        {
            scene::SMesh * mesh = APP.GetEditor()->GetDefaultBoxMesh();
            nodeLeftTop = APP.GetIrrlichtManager()->GetSceneManager()->addMeshSceneNode(mesh, mEditDataNode);
            assert(nodeLeftTop);

            scene::ITriangleSelector* selector = APP.GetIrrlichtManager()->GetSceneManager()->createTriangleSelector(mesh, nodeLeftTop);
            nodeLeftTop->setTriangleSelector(selector);
            selector->drop();

            marker_.mEditNodeLeftTop = nodeLeftTop;
        }
        if ( !nodeRightBottom )
        {
            scene::SMesh * mesh = APP.GetEditor()->GetDefaultBoxMesh();
            nodeRightBottom = APP.GetIrrlichtManager()->GetSceneManager()->addMeshSceneNode(mesh, mEditDataNode);
            assert(nodeRightBottom);

            scene::ITriangleSelector* selector = APP.GetIrrlichtManager()->GetSceneManager()->createTriangleSelector(mesh, nodeRightBottom);
            nodeRightBottom->setTriangleSelector(selector);
            selector->drop();

            marker_.mEditNodeRightBottom = nodeRightBottom;
        }

        nodeCenter->setPosition(settings_.mCenter);
        nodeCenter->setRotation(settings_.mRotation);
        nodeCenter->updateAbsolutePosition();

        nodeLeftTop->setPosition(settings_.mCenter + leftTopVec);
        nodeLeftTop->updateAbsolutePosition();
        nodeRightBottom->setPosition(settings_.mCenter + rightBottomVec);
        nodeRightBottom->updateAbsolutePosition();
#endif // HC1_ENABLE_EDITOR

        if ( nodeCollision )
        {
            nodeCollision->remove();
            nodeCollision = NULL;
        }
        nodeCollision = AddQuadradicNode(mTrackDataNode, leftTopVec,rightTopVec, leftBottomVec, rightBottomVec);
        nodeCollision->setPosition(settings_.mCenter);
        nodeCollision->updateAbsolutePosition();

        marker_.mNodeCollision = nodeCollision;
    }
    else    // !marker_.mIsValid
    {
        marker_.mSettings.Reset();
    }
}

scene::ISceneNode* LevelManager::AddQuadradicNode(scene::ISceneNode* parent_, const core::vector3df &leftTop_, const core::vector3df &rightTop_, const core::vector3df &leftBottom_, const core::vector3df &rightBottom_)
{
    scene::ISceneNode* node = NULL;
    scene::SMeshBuffer * buffer = APP.GetIrrlichtManager()->CreateQuadradMeshBuffer(leftTop_,rightTop_, leftBottom_, rightBottom_);
    assert(buffer);
    scene::SMesh * mesh = new scene::SMesh();
    assert(mesh);
    mesh->addMeshBuffer(buffer);
    buffer->drop();
    mesh->recalculateBoundingBox();
    node = APP.GetIrrlichtManager()->GetSceneManager()->addMeshSceneNode(mesh, parent_);
    mesh->drop();
    assert(node);
    scene::ITriangleSelector* selector = APP.GetIrrlichtManager()->GetSceneManager()->createTriangleSelector(mesh, node);
    node->setTriangleSelector(selector);
    selector->drop();

    return node;
}

bool LevelManager::CheckLineNodeCollision2T(const core::line3d<f32> &line_, scene::ISceneNode* node_, core::vector3df &outIntersection_) const
{
    if ( !node_ )
        return false;

    core::aabbox3d<f32> box(line_.start, line_.end);
    box.repair();
    core::aabbox3d<f32> box2(node_->getTransformedBoundingBox());

    if ( box.intersectsWithBox(box2) )
    {
        const s32 MAX_TRIANGLES = 2;
        s32 trianglesReceived = 0;
        core::triangle3df triangles[MAX_TRIANGLES];

        scene::ITriangleSelector* selector = node_->getTriangleSelector();
        selector->getTriangles( triangles, MAX_TRIANGLES, trianglesReceived, box, /*transform*/ 0 );

        for ( int i=0; i < trianglesReceived; ++i )
        {
            if ( triangles[i].getIntersectionWithLimitedLine(line_, outIntersection_) )
            {
                return true;
            }
        }
    }

    return false;
}

bool LevelManager::CheckFinishLineCollision(const core::line3d<f32> &moveLine_) const
{
    if ( !mFinishLine.mNodeCollision )
        return false;

    core::vector3df outIntersection;
    return CheckLineNodeCollision2T(moveLine_, mFinishLine.mNodeCollision, outIntersection);
}

bool LevelManager::CheckWallCollision(const core::line3d<f32> &moveLine_, int &index_) const
{
    for ( int i=0; i < (int)mTrackMarkers.size(); ++i)
    {
        core::vector3df outIntersection;
        const TrackMarker & marker = mTrackMarkers[i];

        if ( marker.mSettings.mHasLeftWall && marker.mNodeWallLeft)
        {
            if ( CheckLineNodeCollision2T(moveLine_, marker.mNodeWallLeft, outIntersection) )
            {
                index_ = i;
                return true;
            }
        }
        if ( marker.mSettings.mHasRightWall && marker.mNodeWallRight)
        {
            if ( CheckLineNodeCollision2T(moveLine_, marker.mNodeWallRight, outIntersection) )
            {
                index_ = i;
                return true;
            }
        }
        if ( marker.mSettings.mHasBottomWall && marker.mNodeWallBottom)
        {
            if ( CheckLineNodeCollision2T(moveLine_, marker.mNodeWallBottom, outIntersection) )
            {
                index_ = i;
                return true;
            }
        }
    }

    return false;
}

bool LevelManager::CheckMarkerCollision(const core::line3d<f32> &moveLine_, int &index_) const
{
    for ( int i=0; i < (int)mTrackMarkers.size(); ++i)
    {
        core::vector3df outIntersection;

        const TrackMarker & marker = mTrackMarkers[i];
        if (    marker.mSettings.mRelocate
            &&  marker.mNodeCollision
            &&  CheckLineNodeCollision2T(moveLine_, marker.mNodeCollision, outIntersection)
            )
        {
            index_ = i;
            return true;
        }
    }

    return false;
}

bool LevelManager::CheckTeleportLineCollision(const core::line3d<f32> &moveLine_, core::vector3df &targetPos_, core::vector3df &rotation_, core::vector3df &velocity_) const
{
    if ( !mTpSource.mNodeCollision || !mTpTarget.mNodeCollision )
        return false;

    core::vector3df outIntersection;
    if ( CheckLineNodeCollision2T(moveLine_, mTpSource.mNodeCollision, outIntersection) )
    {
        targetPos_ = outIntersection - mTpSource.mNodeCollision->getAbsolutePosition();
        core::matrix4 mat;
        mat.setRotationDegrees( mTpSource.mNodeCollision->getRotation() );
        mat.inverseRotateVect( targetPos_ );
        mat.setRotationDegrees( mTpTarget.mNodeCollision->getRotation() );
        mat.rotateVect( targetPos_ );

        targetPos_ += mTpTarget.mNodeCollision->getAbsolutePosition();
        rotation_ = mTpTarget.mSettings.mRotation;

        float oldVel = velocity_.getLength();
        velocity_.set(0,0,-1);
        core::matrix4 rotVel;
        rotVel.setRotationDegrees (mTpTarget.mNodeCollision->getRotation());
        rotVel.transformVect(velocity_);
        velocity_ *= oldVel;

        return true;
    }

    return false;
}

bool LevelManager::CheckEditCollision(scene::ISceneNode* node_, const core::line3d<f32> line_, float &dist_) const
{
    scene::ISceneCollisionManager * collMan = APP.GetIrrlichtManager()->GetSceneManager()->getSceneCollisionManager();

    if ( node_ )
    {
        core::vector3df intersection;
        core::triangle3df tri;
        scene::ITriangleSelector* selector = node_->getTriangleSelector();
#if IRR_SVN <= 3231
        const irr::scene::ISceneNode* resultNode = 0;
#else
		irr::scene::ISceneNode* resultNode = 0;
#endif
        if (collMan->getCollisionPoint(line_, selector, intersection, tri, resultNode))
        {
            float len = (intersection - line_.start).getLength();
            if ( len < dist_ )
            {
                dist_ = len;
                return true;
            }
        }
    }
    return false;
}

bool LevelManager::CheckTrackStartEditCollision(const core::line3d<f32> line_, int &index_, float &dist_)
{
    bool hasColl = false;
    for ( int i=0; i < MAX_SPAWNS; ++i )
    {
        if ( CheckEditCollision(GetTrackStart(i).mEditNode, line_, dist_) )
        {
            index_ = i;
            hasColl = true;
        }
    }

    return hasColl;
}

bool LevelManager::CheckTrackMarkerEditCollision(const core::line3d<f32> line_, int &index_, float &dist_)
{
    bool hasColl = false;

    for ( int i=0; i < (int)mTrackMarkers.size(); ++i )
    {
        if ( CheckEditCollision(mTrackMarkers[i].mEditNodeCenter, line_, dist_) )
        {
            index_ = i;
            hasColl = true;
        }
    }

    return hasColl;
}

bool LevelManager::CheckTpSourceEditCollision(const core::line3d<f32> line_, float &dist_)
{
    return CheckEditCollision(mTpSource.mEditNodeCenter, line_, dist_);
}

bool LevelManager::CheckTpTargetEditCollision(const core::line3d<f32> line_, float &dist_)
{
    return CheckEditCollision(mTpTarget.mEditNodeCenter, line_, dist_);
}

bool LevelManager::CheckFinishLineEditCollision(const core::line3d<f32> line_, int &index_, float &dist_)
{
    if ( CheckEditCollision(mFinishLine.mEditNodeCenter, line_, dist_) )
    {
        index_ = 0;
        return true;
    }

    return false;
}

bool LevelManager::CheckModelEditCollision(const core::line3d<f32> line_, int &index_, float &dist_)
{
    bool hasColl = false;

    for ( int i=0; i < (int)mModels.size(); ++i )
    {
        if ( CheckEditCollision(mModels[i].mNodeModel, line_, dist_) )
        {
            index_ = i;
            hasColl = true;
        }
    }

    return hasColl;
}


bool LevelManager::LoadTrackData(const std::string &fileName_)
{
    std::string dataFileName = APP.GetConfig()->MakeFilenameLevel(fileName_);
    TiXmlDocument doc(dataFileName.c_str());
    doc.SetIrrFs(APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem(), TiXmlDocument::E_ON_READ_ANDROID);
    if ( !doc.LoadFile())
    {
		LOG.DebugLn("TiXmlDocument::ErrorDesc: ", doc.ErrorDesc());
        return false;
	}

    TiXmlNode * nodeData = doc.FirstChild("data");
	if (!nodeData)
        return false;

    bool result = true;
    const TiXmlNode * node = nodeData->IterateChildren("trackstart", NULL);
    for ( int i=0; i<MAX_SPAWNS; ++i )
    {
        if ( node )
        {
            GetTrackStart(i).mSettings.ReadFromXml(node->ToElement());
            node = nodeData->IterateChildren("trackstart", node);
        }
    }

    node = nodeData->IterateChildren("finishline", NULL);
    if ( node )
        mFinishLine.mSettings.ReadFromXml(node->ToElement());

    node = nodeData->IterateChildren("tp_source", NULL);
    if ( node )
        mTpSource.mSettings.ReadFromXml(node->ToElement());

    node = nodeData->IterateChildren("tp_target", NULL);
    if ( node )
        mTpTarget.mSettings.ReadFromXml(node->ToElement());

    mTrackMarkers.clear();
    node = nodeData->IterateChildren("track_markers", NULL);
    if ( node )
    {
        const TiXmlNode * nodeMarker = node->IterateChildren("marker", NULL);
        while ( nodeMarker )
        {
            TrackMarker marker;
            marker.mSettings.ReadFromXml(nodeMarker->ToElement());
            mTrackMarkers.push_back(marker);
            nodeMarker = node->IterateChildren("marker", nodeMarker);
        }
    }

    mModels.clear();
    node = nodeData->IterateChildren("models", NULL);
    if ( node )
    {
        const TiXmlNode * nodeModel = node->IterateChildren("model", NULL);
        while ( nodeModel )
        {
            LevelModelSettings settings;
            settings.ReadFromXml(nodeModel->ToElement());

            AddModel(settings.mName);
            SetModel(settings, mModels.size()-1);

            nodeModel = node->IterateChildren("model", nodeModel);
        }
    }

    node = nodeData->IterateChildren("ai_track", NULL);
    if ( node )
    {
        result &= mAiTrack.ReadFromXml(node);
    }

    return result;
}

bool LevelManager::SaveCurrentTrackData()
{
    if ( mCurrentLevel < 0 )
        return false;
    return SaveTrackData(mLevelSettings[mCurrentLevel].mDataFile);
}

bool LevelManager::SaveTrackData(const std::string &fileName_)
{
    std::string dataFileName = APP.GetConfig()->MakeFilenameLevel(fileName_);
    TiXmlDocument doc(dataFileName.c_str());
    doc.SetIrrFs(APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem(), TiXmlDocument::E_ON_READ_FAIL_ANDROID);

    if ( !doc.LoadFile() )
        return false;

	TiXmlNode * nodeData = doc.FirstChild("data");
	if (!nodeData)
        return false;

    bool result = true;

    TiXmlNode* node = nodeData->IterateChildren("trackstart", NULL);
    for ( int i=0; i<MAX_SPAWNS; ++i )
    {
        if ( !node )
        {
            TiXmlElement ele("trackstart");
            node = nodeData->InsertEndChild( ele );
        }
        assert(node);

        GetTrackStart(i).mSettings.WriteToXml(node->ToElement());
        node = nodeData->IterateChildren("trackstart", node);
    }

    node = nodeData->IterateChildren("finishline", NULL);
    if ( !node )
    {
        TiXmlElement ele("finishline");
        node = nodeData->InsertEndChild( ele );
    }
    assert(node);
    mFinishLine.mSettings.WriteToXml(node->ToElement());

    node = nodeData->IterateChildren("tp_source", NULL);
    if ( !node )
    {
        TiXmlElement ele("tp_source");
        node = nodeData->InsertEndChild( ele );
    }
    assert(node);
    mTpSource.mSettings.WriteToXml(node->ToElement());

    node = nodeData->IterateChildren("tp_target", NULL);
    if ( !node )
    {
        TiXmlElement ele("tp_target");
        node = nodeData->InsertEndChild( ele );
    }
    assert(node);
    mTpTarget.mSettings.WriteToXml(node->ToElement());

    // track markers
    node = nodeData->IterateChildren("track_markers", NULL);
    if ( !node )
    {
        TiXmlElement ele("track_markers");
        node = nodeData->InsertEndChild( ele );
    }
    assert(node);
    TiXmlElement  * eleTrackMarkers = node->ToElement();
    eleTrackMarkers->Clear();
    for ( unsigned int i=0; i < mTrackMarkers.size(); ++i )
    {
        TiXmlElement ele("marker");
        mTrackMarkers[i].mSettings.WriteToXml(&ele);
        eleTrackMarkers->InsertEndChild(ele);
    }

    // models
    node = nodeData->IterateChildren("models", NULL);
    if ( !node )
    {
        TiXmlElement ele("models");
        node = nodeData->InsertEndChild( ele );
    }
    assert(node);
    TiXmlElement  * eleModels = node->ToElement();
    eleModels->Clear();
    for ( unsigned int i=0; i < mModels.size(); ++i )
    {
        TiXmlElement ele("model");
        mModels[i].mSettings.WriteToXml(&ele);
        eleModels->InsertEndChild(ele);
    }

    // mAiTrackInfo
    node = nodeData->IterateChildren("ai_track", NULL);
    if ( !node )
    {
        TiXmlElement ele("ai_track");
        node = nodeData->InsertEndChild( ele );
    }
    assert(node);
    result &= mAiTrack.WriteToXml(node);
    result &= doc.SaveFile();

    return result;
}

int LevelManager::GetTrackRelocatesBetween(int indexFront_, int indexBack_)
{
    if ( !mTrackMarkers.size() )
        return -1;
    if ( indexFront_ < 0 || indexBack_ < 0 || indexFront_ >= (int)mTrackMarkers.size() || indexBack_ >= (int)mTrackMarkers.size() )
    {
        return -1;
    }

    int realFront_ = indexFront_;
    int problem = indexFront_;
    while ( !mTrackMarkers[realFront_].mSettings.mRelocate )
    {
        if ( --realFront_ < 0 )
        {
            realFront_ = mTrackMarkers.size()-1;
        }
        if ( realFront_ == problem )
            return -1;
    }

    int result = 0;
    while ( realFront_ != indexBack_ )
    {
        if ( --realFront_ < 0 )
        {
            realFront_ = mTrackMarkers.size()-1;
        }
        if ( mTrackMarkers[realFront_].mSettings.mRelocate )
            ++result;
    }
    return result;
}
