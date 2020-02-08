// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef PLAYER_H
#define PLAYER_H

#include "compile_config.h"
#include "irrlicht_manager.h"
#include <string>
#include <vector>
#include "recorder.h"

#if defined(NEURAL_AI)
class NeuralNet;
#endif

class Controller;
class Game;
struct SteeringSettings;
struct PhysicsObject;
struct AiBotSettings;

#define AI_TRACK_PREVIEWS 2

struct PlayerSettings
{
    std::wstring    mName;
    std::string     mModelFileName;
};

enum PLAYER_TYPE
{
    PT_UNKNOWN,
    PT_LOCAL,
    PT_GHOST_LAP,
    PT_GHOST_TRACK,
    PT_REPLAY,
    PT_AI,
};

struct PlayerStreamData
{
    bool StreamFrom(IArchive * archive_, bool wrapAround_=true);
    bool StreamTo(IArchive * archive_);

    irr::core::vector3df mPos;
    irr::core::vector3df mRot;
    float mControllerPower;
    float mControllerRotation;
};

// input data available on each tick for the AI-bots
struct AiInputData
{
    float   mSpeedScaled;
    float   mVelToDirAngleScaled;
    float   mDistToCenter;
    float   mScaledDistDirToBorder;
    float   mScaledDistVelToBorder;
    float   mVelToPreviewAngleScaled[AI_TRACK_PREVIEWS];
    float   mVelToIdealAngleScaled[AI_TRACK_PREVIEWS];
    float   mPreviewHeightFactor[AI_TRACK_PREVIEWS];

    static int GetNumInputs()               { return 5 + 2*AI_TRACK_PREVIEWS; }
#if defined(NEURAL_AI)
    int FillNetInputLayer(NeuralNet* nn_, int neuronIndex_=0);
#endif
    void PrintToFile(FILE* file_);
    void Reset();
};

class Player : public IRecordable
{
public:
    Player(const Game& game);
    virtual ~Player();

    void SetType(PLAYER_TYPE type_);
    PLAYER_TYPE GetType() const         { return mPlayerType; }
    bool IsActiveType() const           { return mPlayerType == PT_LOCAL || mPlayerType == PT_AI; }

    void SetAiBotSettings(const AiBotSettings &settings_);

    bool Init();
    void ChangeModel(const std::string &name_);
    void PrePhysicsUpdate(irr::u32 time_);
    void PostPhysicsUpdate(irr::u32 time_);

    int GetPhysicsId() const { return mPhysicsId; }
    irr::scene::ISceneNode * GetHoverNode() const  		 { return mMeshHover; }
    Controller* GetActiveController();
    const irr::core::matrix4& GetRollingMatrix() const   { return mRollingMat; }
    const irr::core::matrix4& GetYawMatrix() const       { return mYawMat; }

    void EnableTeleportFx(bool enable_);

    void InfoPrepareForRace();
    void InfoStartCountDown(irr::u32 time_);
    void InfoStartRace(irr::u32 time_);      // countdown finished
    void InfoRoundFinished(irr::u32 time_);
    void InfoLevelMarkerReached(int index_);
    void InfoTeleport(irr::u32 time_);
    void InfoDroppedFromTrack(irr::u32 time_);
    void InfoGamePaused();
    void InfoGameResumePause();
    void InfoGameResumes();
    void InfoGameFinished();
    void InfoTrainingAiTeleport(irr::u32 time_);

    irr::u32 GetRoundStartTime() const          { return mRoundStartTime; }
    irr::u32 GetRoundStartPhysicsTicks() const  { return mRoundStartPhysicsTicks; }
    irr::u32 GetBestRoundTime() const           { return mBestRoundTime; }
    irr::u32 GetLastRoundTime() const           { return mLastRoundTime; }
    irr::u32 GetCurrentRound() const            { return mCurrentRound; }
    int GetCurrentMarkerReached() const   	{ return mTrackMarkerReached < mHighestValidTrackMarker ? mTrackMarkerReached : mHighestValidTrackMarker; }
    irr::u32 GetMarkerReachedTime(irr::u32 idx_) const { return idx_ < mTrackMarkerReachedTimes.size() ? mTrackMarkerReachedTimes[idx_] : 0; }
    int GetLastMarkerReached() const        	{ return mLevelMarkerReached; }
    irr::u32 GetLastTimeTouchedFloor() const    { return mLastTimeTouchedFloor; }
    irr::core::vector3df GetLastPos() const     { return mLastPos; }    // position before physics update

    void SetPlacing(int pos_) { mPlacing = pos_; }
    int GetPlacing() const    { return mPlacing; }

    PlayerSettings mSettings;

    // IRecordable-Interface
    virtual void Recording(IArchive * ar_);
    virtual void Playing(IArchive * ar_);
    virtual void PlayingInterpolated(Record & record_, irr::u32 time_);
    virtual void PlayingInterpolated(Record & record_, irr::f32 time_);

    void SetRecordingId(int id_)  { mRecordingId = id_; }
    int GetRecordingId() const    { return mRecordingId; }
    void SetRecordPlayId(int id_) { mRecordPlayId = id_; }
    int GetRecordPlayId() const   { return mRecordPlayId; }

    void SetTrackStart(int startId_) { mTrackStart = startId_; }
    int GetTrackStart() const        { return mTrackStart; }

#if defined(NEURAL_AI)
    void SetAiTrainingRecord(Record * record_);
    float CalculateAiTrainingAward(irr::u32 time_);

    NeuralNet* GetNeuralNet() { return mNeuralNet; }
    NeuralNet* GetTrainingNeuralNet() { return mTrainingNeuralNet; }
#endif

protected:
    void CalcRollMatrices(float additionalRoll_=0.f);
    void UpdateAi(irr::u32 time_);
    void UpdateAiManual(irr::u32 time_);
    void UpdateAiInputData(irr::u32 time_);

    void UpdateShadow();
    void UpdateDriftMarkers();
    void StopAllSoundSamples();
    void FadeoutAllSoundSamples();
    void DisableAllEffects();
    void EnablePermanentEffects();

    // functions needed for ai training
    int GetTimeIndexForPos(int lastIndex_, Record * record_, const irr::core::vector3df& pos_, bool searchAll_=false);

    float CalcAngleForRotation(float rotation_, float timeSec_, const PhysicsObject& hoverPhysics_, const SteeringSettings& settings_);
    float ApproximateRotationForAngle(float angle_, float timeSec_, const PhysicsObject& hoverPhysics_, const SteeringSettings& settings_);

    // return value between -1 and 1 where: 0 is for 0 deg, 0.5 for 90 deg. 1 or -1 for 180 deg and -0.5 for 270 deg
    // Y should be 0 for the input vectors and both input vectors must be normalized
    float CalcScaledAngleToVector(const irr::core::vector3df &normDirXZ_, const irr::core::vector3df &normTargetDirXZ_ );

    // play sound only if it's not played already
    bool PlaySample(int & soundRef_, const std::string & sampleName_, float gain_, bool repeat_);

    // stop sample when it's played
    void StopSample(int & soundRef_);

    void ResetMarkersReached(irr::u32 time_, bool createTimers_=false);
    void UpdateMarkersReached(irr::u32 time_, bool searchAll_);

private:
	const Game& mGame;
    PLAYER_TYPE	mPlayerType;
    std::string mNameHoverLoaded;
    bool mIsGhost;
    irr::scene::ISceneNode* mMeshHover;
    irr::scene::ISceneNode* mMeshBlobShadow;
    std::vector<irr::scene::ISceneNode*> mDriftMarkers;  // currently not used
    int         mPhysicsId;
    float       mLastAlignmentScaling;
    irr::core::vector3df    mLastAlignmentVector;
    irr::core::matrix4      mAlignmentMat;
    irr::core::vector3df    mLastPos;
    float       mLastRolling;
    irr::core::matrix4      mRollingMat;
    irr::core::matrix4      mYawMat;
    irr::u32    mLastTimeTouchedFloor;

    irr::u32	mRoundStartTime;
    irr::u32    mRoundStartPhysicsTicks;
    irr::u32    mBestRoundTime;
    irr::u32    mLastRoundTime;
    irr::u32    mCurrentRound;
    int         mLevelMarkerReached;

	int 		mTrackMarkerCurrent;
    int         mTrackMarkerReached;
    irr::u32    mTimeMarkerChanged;
    int         mHighestValidTrackMarker;
    irr::u32    mTimeHighestValidTrackMarker;
    std::vector<irr::u32> mTrackMarkerReachedTimes;

    int  		mTrackStart;
    int         mNumDroppedLastRound;
    int         mNumDroppedThisRound;
    irr::u32    mTimeDisableTeleportFx;
    bool        mEnableFxSounds;
    irr::u32    mTimeLastObjCollision;
    int         mPlacing;
    irr::u32    mTimeNoCollision;   // no hover/hover collision until that time is reached
    irr::u32    mTimeFreezing;      // no hover movement until that time is reached
    irr::u32    mTimeTouchedWall;
    irr::u32    mTimeLastVibrate;	// last vibrate feedback (for devices which support it)
    irr::u32    mTimeLastPostPhysicsUpdate;

    int mSoundRefEngine;
    int mSoundRefEngine2;
    int mSoundRefHovering;
    int mSoundRefBrake;
    int mSoundRefAir;
    int mSoundRefCollision;
    int mSoundRefHitGround;
    bool mCanPlayEngineAccSound;
    bool mCanPlayBrakeSound;

    Controller*         mAiController;
    AiBotSettings*      mAiBotSettings;

    int mRecordingId;
    int mRecordPlayId;

    AiInputData mAiInputData;
    AiInputData mAiLastInputData;
    float mDistToTrackCenter;
    irr::core::vector3df mClosestPointOnTrack;
    float mTrainingDistToTrackCenterSum;
    float mTrainingLastAward;
    float mTrainingAverageAward;
    int mLastTrainingTimeIndex;
    int mLastTrainingTime;
    int mTrainingDroppedFromTrack;
    int mTrainingStartIndex;
    irr::u32 mTrainingLastDroppedTime;
    irr::u32 mTrainingTimeFirstDropped;
    irr::core::vector3df mTrainingLastPos;
    float   mRelativePreviewDist[AI_TRACK_PREVIEWS];
    irr::core::vector3df mPreviewDir[AI_TRACK_PREVIEWS];
    irr::core::vector3df mPreviewDirIdeal[AI_TRACK_PREVIEWS];
    bool    mUseStayOnTrackCheat;

#if defined(NEURAL_AI)
    NeuralNet*          mNeuralNet;
    NeuralNet*          mTrainingNeuralNet;
    FILE*               mAiTrainingDataFile;
    Record*             mAiTrainingRecord;
#endif
};

#endif // PLAYER_H
