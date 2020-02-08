// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef LEVEL_H
#define LEVEL_H

// TODO: this file has more grown than planned and it starts showing

#include "globals.h"
#include "ai_track.h"
#include <string>
#include <vector>

class TiXmlElement;

enum LevelDifficulty
{
    LD_EASY = 0,
    LD_MEDIUM,
    LD_HARD,
    LD_ALL,
};

struct AiBotSettings
{
    AiBotSettings();
    void ReadFromXml(const TiXmlElement * settings_);

    std::string mName;
    std::string mModel;
    float mLowPower;  // lower border for range of random power modifier
    float mUppPower;  // upper border for range of random power modifier
    float mSlowBorder; // if the speed is below mSlowBorder*maxspeed the slow modifiers are used
    float mLowSlow;   // lower border for range of random power modifier for slow speeds
    float mUppSlow;   // upper border for range of random power modifier for slow speeds
    float mLowEnergy;   // lower border for range of energy modifier (energy pushing object along a path)
    float mUppEnergy;   // upper border for range of energy modifier (energy pushing object along a path)
    float mLowIdeal;    // lower border for range or random modifier towards ideal line (instead of line throught track center)
    float mUppIdeal;    // upper border for range or random modifier towards ideal line (instead of line throught track center)
    int mCollTime;    // milliseconds after collision where bot's don't cheat
    int mCollTimeReturn;      // milliseconds it takes to get the bot back on track
    int mStartTime;   // first milliseconds in which startsettings are used
    float mLowPowerStart;  // lower border for random power first seconds
    float mUppPowerStart;  // upper border for random power first seconds
    float mPreviewDist1;
    float mPreviewDist2;
};

struct TrackStartSettings
{
    TrackStartSettings() : mIsValid(false) {}
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;
    void Reset()    { mIsValid = false; }

    bool            mIsValid;
    irr::core::vector3df mCenter;
    irr::core::vector3df mRotation;
};

struct TrackMarkerSettings
{
    TrackMarkerSettings();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;
    void Reset();
    void EnableWalls();

    bool            mIsValid;
    irr::core::vector3df mCenter;
    irr::core::vector3df mUpVector;
    irr::core::vector3df mRotation;
    bool            mRelocate;  // hover can teleport to this track when he drops off
    bool            mUseDefaultSizes;
    float           mLeft;
    float           mRight;
    float           mTop;
    float           mBottom;

    bool            mHasLeftWall;
    bool            mHasRightWall;
    bool            mHasBottomWall;
//    bool            mHasTopWall;
};

struct LevelCameraSettings
{
    void ReadFromXml(const TiXmlElement * settings_);

    std::string     mModel;
};

// settings used in levels.xml
struct LevelSettings
{
    LevelSettings();
    void ReadFromXml(const TiXmlElement * settings_);

    int             mId;
    std::wstring    mName;
    AiBotSettings   mBotSettings[MAX_BOT_SETTINGS];
    LevelCameraSettings mCameraSettings;
    std::string     mMeshFile;
    std::string     mDataFile;
    std::string     mLightmapFile;

    LevelDifficulty mDifficulty;
    int             mLaps;
    float           mTargetTime;

    int             mAmbientAlpha;
    int             mAmbientRed;
    int             mAmbientGreen;
    int             mAmbientBlue;

    int             mAmbHoverRed;
    int             mAmbHoverGreen;
    int             mAmbHoverBlue;

    float           mDefaultMarkerLeft;
    float           mDefaultMarkerRight;
    float           mDefaultMarkerTop;
    float           mDefaultMarkerBottom;

    float           mHudPosScaling;
};

struct TrackStart
{
    TrackStart() : mEditNode(0) {}

    bool GetCenter( irr::core::vector3df &pos_);

    irr::scene::ISceneNode*  mEditNode;
    TrackStartSettings  mSettings;
};

struct TrackMarker
{
    TrackMarker();

    bool GetCenter( irr::core::vector3df &pos_);

    irr::scene::ISceneNode*  mEditNodeCenter;
    irr::scene::ISceneNode*  mEditNodeLeftTop;
    irr::scene::ISceneNode*  mEditNodeRightBottom;
    irr::scene::ISceneNode*  mNodeCollision;
    irr::scene::ISceneNode*  mNodeWallLeft;
    irr::scene::ISceneNode*  mNodeWallRight;
    irr::scene::ISceneNode*  mNodeWallTop;
    irr::scene::ISceneNode*  mNodeWallBottom;

    TrackMarkerSettings mSettings;
};


struct LevelModelSettings
{
    LevelModelSettings();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    std::string     mName;
    irr::core::vector3df mCenter;
    irr::core::vector3df mRotation;
    irr::core::vector3df mScale;
    bool            mHasCollisions;

    // those ambient values will influence all meshes which DO NOT have ambience already set to 0
    int             mAmbientRed;
    int             mAmbientGreen;
    int             mAmbientBlue;
};

struct LevelModel
{
    LevelModel();

    bool GetCenter( irr::core::vector3df &pos_);

    irr::scene::ISceneNode*  mNodeModel;
    LevelModelSettings  mSettings;
};

class LevelManager
{
public:
    LevelManager();
    ~LevelManager();

    void LoadSettings();    // metadata for all levels
    bool LoadLevel(const std::wstring &levelName_);
    bool SaveCurrentTrackData();

    int GetNumLevels(LevelDifficulty difficulty_ = LD_ALL) const;
    int GetCurrentLevelIndex()  const { return mCurrentLevel; }
    unsigned int GetRealIndex( unsigned int index_, LevelDifficulty difficulty_ = LD_ALL) const;
    int GetIndexForId(int id_) const;
    int GetIndexForName( const std::wstring &levelName_ ) const;

    const LevelSettings& GetLevel(unsigned int index_, LevelDifficulty difficulty_ = LD_ALL) const;
    const LevelSettings& GetCurrentLevel() const   { return GetLevel(mCurrentLevel); }

    void SetMeshScale(const irr::core::vector3df &scale_);
    irr::scene::ISceneNode* GetMesh()                { return mLevelNode; }
    irr::scene::ITriangleSelector* GetSelector()     { return mSelector; }
    irr::scene::ISceneNode* GetNodeCamera()          { return mNodeCamera; }

    void ShowEditData(bool enable_);

    bool CheckWallCollision(const irr::core::line3d<irr::f32> &moveLine_, int &index_) const;
    bool CheckMarkerCollision(const irr::core::line3d<irr::f32> &moveLine_, int &index_) const;
    bool CheckFinishLineCollision(const irr::core::line3d<irr::f32> &moveLine_) const;
    bool CheckTeleportLineCollision(const irr::core::line3d<irr::f32> &moveLine_, irr::core::vector3df &targetPos_, irr::core::vector3df &rotation_, irr::core::vector3df &velocity_) const;

    int GetTrackRelocatesBetween(int indexFront_, int indexBack_);

    // remember: GetTrackMarker and GetNrOfModels return no references because they are part of a vector
    TrackStart& GetTrackStart(unsigned int index_);
    unsigned int GetNrOfTrackMarkers() const;
    TrackMarker GetTrackMarker(unsigned int index_) const       { return mTrackMarkers[index_]; }
    TrackMarker& GetFinishLine()                        { return mFinishLine; }
    TrackMarker& GetTpSource()                          { return mTpSource; }
    TrackMarker& GetTpTarget()                          { return mTpTarget; }
    LevelModel GetModel(unsigned int index_) const      { return mModels[index_]; }
    unsigned int GetNrOfModels() const                  { return mModels.size(); }
    AiTrack& GetAiTrack()                               { return mAiTrack; }

    void SetTrackStart(const TrackStartSettings& ts_, int player_);
    void SetTrackMarker(const TrackMarkerSettings &marker_, int index_);
    void InsertTrackMarker(const TrackMarkerSettings &marker_, int index_);
    void AppendTrackMarker(const TrackMarkerSettings &marker_, int index_);
    void RotateTrackMarker(float angle_, int index_);
    void AutoAlignTrackMarker(int index_);
    void ChangeTrackMarkerSizes(bool useDefault_, float changeLeft_, float changeRight_, float changeTop_, float changeBottom_, int index_);
    void RemoveTrackMarker(int index_);

    void SetTeleportSource(const TrackMarkerSettings &marker_);
    void SetTeleportTarget(const TrackMarkerSettings &marker_);

    void SetFinishLine(const TrackMarkerSettings &marker_);

    void AddModel(const std::string &name_);
    void SetModel(const LevelModelSettings &settings_, int index_);
    void ChangeModelRotation(float addRotX_, float addRotY_, float addRotZ_, int index_);
    void ChangeModelPosition(float addPosX_, float addPosY_, float addPosZ_, int index_);
    void SetModelAmbient(int ambR_, int ambG_, int ambB_, int index_);
    void RemoveModel(int index_);

    bool CheckTrackStartEditCollision(const irr::core::line3d<irr::f32> line_, int &index_, float &dist_);
    bool CheckTrackMarkerEditCollision(const irr::core::line3d<irr::f32> line_, int &index_, float &dist_);
    bool CheckTpSourceEditCollision(const irr::core::line3d<irr::f32> line_, float &dist_);
    bool CheckTpTargetEditCollision(const irr::core::line3d<irr::f32> line_, float &dist_);
    bool CheckFinishLineEditCollision(const irr::core::line3d<irr::f32> line_, int &index_, float &dist_);
    bool CheckModelEditCollision(const irr::core::line3d<irr::f32> line_, int &index_, float &dist_);

protected:
    bool CheckEditCollision(irr::scene::ISceneNode*, const irr::core::line3d<irr::f32> line_, float &dist_) const;
    // only two triangles checked!
    bool CheckLineNodeCollision2T(const irr::core::line3d<irr::f32> &line_, irr::scene::ISceneNode* node_, irr::core::vector3df &outIntersection_) const;

    void BuildTrackData(LevelSettings * settings_);
    void ClearTrackData();
    void ClearTrackMarkerData(TrackMarker & marker_);
    void ClearModelData(LevelModel & model_);
    void SetTrackMarkerSettings(const TrackMarkerSettings &settings_, TrackMarker & marker_);
    void SetCollisionWalls(TrackMarker & marker1_, TrackMarker & marker2_ );
    void RemoveCollisionWalls(TrackMarker & marker_);
    void CalcMarkerBorders(const TrackMarker & marker_, irr::core::vector3df & leftTop_, irr::core::vector3df & rightTop_, irr::core::vector3df & leftBottom_, irr::core::vector3df & rightBottom_, bool relative_=true);
    irr::scene::ISceneNode* AddQuadradicNode(irr::scene::ISceneNode* parent_, const irr::core::vector3df &leftTop_, const irr::core::vector3df &rightTop_, const irr::core::vector3df &leftBottom_, const irr::core::vector3df &rightBottom_);

    bool Load(LevelSettings * settings_);
    bool LoadTrackData(const std::string &fileName_);
    bool SaveTrackData(const std::string &fileName_);

private:
    LevelSettings   mDefaultLevelSettings;
    std::vector<LevelSettings> mLevelSettings;
    int mCurrentLevel;

    std::string mLevelFileName;
    irr::scene::ISceneNode* mLevelNode;
    irr::scene::ITriangleSelector* mSelector;

    irr::scene::ISceneNode* mNodeCamera;     // that node is always added to the active camera
    irr::scene::ISceneNode* mEditDataNode;   // parent for all edit nodes
    irr::scene::ISceneNode* mTrackDataNode;  // parent for all track data nodes

    TrackStart                  mTrackStarts[MAX_SPAWNS];
    std::vector<TrackMarker>    mTrackMarkers;
    TrackMarker                 mFinishLine;
    TrackMarker                 mTpSource;
    TrackMarker                 mTpTarget;
    std::vector<LevelModel>     mModels;
    AiTrack                     mAiTrack;
};

#endif // LEVEL_H
