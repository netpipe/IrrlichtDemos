// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "player.h"
#include "main.h"
#include "helper_irr.h"
#include "helper_math.h"
#include "game.h"
#include "physics.h"
#include "timer.h"
#include "controller.h"
#include "config.h"
#include "sound.h"
#include "streaming.h"
#include "node_manager.h"
#include "irrlicht_manager.h"
#include "level.h"
#include "nn.h"
#include "random.h"
#include "globals.h"
#include "logging.h"

#ifdef _IRR_ANDROID_PLATFORM_
#include "android_tools.h"
#endif

#include <assert.h>
#include <algorithm>

using namespace irr;

namespace
{
	const float SEARCH_ABOVE_TRACK = 300.f;
	const float SEARCH_BELOW_TRACK = 1500.f;
	bool EASY_BOT_CONTROL = false;
};

// --------------------------------------------------------
bool PlayerStreamData::StreamFrom(IArchive * archive_, bool wrapAround_)
{
    if ( archive_->GetReadPos() >= archive_->GetSize() )
    {
        if ( wrapAround_ )
            archive_->SetReadPos(0);
        else
            return false;
    }

    archive_->Read(mPos.X);
    archive_->Read(mPos.Y);
    archive_->Read(mPos.Z);
    archive_->Read(mRot.X);
    archive_->Read(mRot.Y);
    archive_->Read(mRot.Z);
    archive_->Read(mControllerPower);
    archive_->Read(mControllerRotation);

    return true;
}

bool PlayerStreamData::StreamTo(IArchive * archive_)
{
    archive_->Write(mPos.X);
    archive_->Write(mPos.Y);
    archive_->Write(mPos.Z);
    archive_->Write(mRot.X);
    archive_->Write(mRot.Y);
    archive_->Write(mRot.Z);
    archive_->Write(mControllerPower);
    archive_->Write(mControllerRotation);

    return true;
}


// --------------------------------------------------------
void AiInputData::Reset()
{
    memset(this, 0, sizeof(AiInputData));
}

#if defined(NEURAL_AI)
int AiInputData::FillNetInputLayer(NeuralNet* net_, int neuronIndex_)
{
    if( !net_ )
        return 0;

    net_->SetInputValue(neuronIndex_, mSpeedScaled);
    ++neuronIndex_;
    net_->SetInputValue(neuronIndex_, mVelToDirAngleScaled);
    ++neuronIndex_;
    net_->SetInputValue(neuronIndex_, mDistToCenter);
    ++neuronIndex_;
    net_->SetInputValue(neuronIndex_, mScaledDistDirToBorder);
    ++neuronIndex_;
    net_->SetInputValue(neuronIndex_, mScaledDistVelToBorder);
    ++neuronIndex_;
    for ( int i=0; i < AI_TRACK_PREVIEWS; ++i )
    {
        net_->SetInputValue(neuronIndex_, mVelToPreviewAngleScaled[i]);
        ++neuronIndex_;
        net_->SetInputValue(neuronIndex_, mPreviewHeightFactor[i]);
        ++neuronIndex_;
    }
    return neuronIndex_;
}
#endif // NEURAL_AI

void AiInputData::PrintToFile(FILE* file_)
{
    fprintf(file_, "%f %f %f %f %f", mSpeedScaled, mVelToDirAngleScaled, mDistToCenter, mScaledDistDirToBorder, mScaledDistVelToBorder);
    for ( int i=0; i < AI_TRACK_PREVIEWS; ++i )
    {
        fprintf(file_, " %f %f", mVelToPreviewAngleScaled[i], mPreviewHeightFactor[i]);
    }
}

// --------------------------------------------------------
Player::Player(const Game& game)
: mGame(game)
{
    static int profile = 0;
    if ( !profile )
    {
        ++profile;
        PROFILE_ADD_GROUP(PGROUP_PLAYER, "player");
        PROFILE_ADD(400, PGROUP_PLAYER, "post1");
        PROFILE_ADD(401, PGROUP_PLAYER, "post2");
        PROFILE_ADD(402, PGROUP_PLAYER, "post3");
        PROFILE_ADD(403, PGROUP_PLAYER, "post4");
    }

    static int sampleCache = 0;
    if ( 0 == sampleCache )
    {
        if ( APP.GetSound() )
        {
            sampleCache = 1;

            APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_collision01.wav")).c_str() );
            APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_hitground01.wav")).c_str() );
            APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_engine_split01.wav")).c_str() );
            APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_hovering01.wav")).c_str() );
            APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_air01.wav")).c_str() );
            APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_engine_split02.wav")).c_str() );
            APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_brake01.wav")).c_str() );
            APP.GetSound()->CacheSample( APP.GetConfig()->MakeFilenameSound(std::string("hc_teleport01.wav")).c_str() );
        }
    }

    mPlayerType = PT_UNKNOWN;
    mIsGhost = false;
    mPhysicsId = -1;
    mMeshHover = NULL;
    mMeshBlobShadow = NULL;
    mLastAlignmentScaling = 0.f;
    mLastTimeTouchedFloor = 0;
    mRoundStartTime = 0;
    mRoundStartPhysicsTicks = 0;
    mBestRoundTime = Game::TIME_UNUSED;
    mRecordingId = -1;
    mRecordPlayId = -1;
    mAiController = NULL;
    mAiBotSettings = NULL;
#if defined(NEURAL_AI)
    mNeuralNet = NULL;
    mTrainingNeuralNet = NULL;
    mAiTrainingRecord = NULL;
    mAiTrainingDataFile = 0;
//    mAiTrainingDataFile = fopen("aitrainingdata.txt", "at");
#endif
    mTrackStart = -1;
    mSoundRefEngine = -1;
    mSoundRefEngine2 = -1;
    mSoundRefBrake = -1;
    mSoundRefAir = -1;
    mSoundRefHovering = -1;
    mSoundRefCollision = -1;
    mSoundRefHitGround = -1;
    mCanPlayEngineAccSound = true;
    mCanPlayBrakeSound = true;
    ResetMarkersReached(0);
    mLastTrainingTimeIndex = 0;
    mLastTrainingTime = 0;
    mTrainingDroppedFromTrack = 0;
    mNumDroppedLastRound = 0;
    mNumDroppedThisRound = 0;
    mTimeDisableTeleportFx = 0;
    mUseStayOnTrackCheat = false;
    mEnableFxSounds = true;
    mPlacing = 0;
    mTimeNoCollision = 0;
    mTimeFreezing = 0;
    mTimeTouchedWall = 0;
    mTimeLastVibrate = 0;
    mTimeLastPostPhysicsUpdate = 0;
    mTimeLastObjCollision = 0;
}

Player::~Player()
{
    delete mAiController;
    delete mAiBotSettings;
#if defined(NEURAL_AI)
    if ( mAiTrainingDataFile )
    {
        fclose(mAiTrainingDataFile);
        mAiTrainingDataFile = 0;
    }
    if ( mAiTrainingRecord )
        mAiTrainingRecord->drop();
    delete mNeuralNet;
    delete mTrainingNeuralNet;
#endif
}

void Player::SetAiBotSettings(const AiBotSettings &settings_)
{
    if( !mAiBotSettings )
    {
        fprintf(stderr, "Warning: mAiBotSettings not set in %s %s\n", __FILE__, __func__);
        return;
    }
    *mAiBotSettings = settings_;

    ChangeModel(mAiBotSettings->mModel);
}

void Player::SetType(PLAYER_TYPE type_)
{
    mPlayerType = type_;
    mUseStayOnTrackCheat = false;
    mEnableFxSounds = true;

//    if ( mPlayerType != PT_AI )     // TEST
//        mEnableFxSounds = false;    // TEST

    if ( mPlayerType == PT_AI && !mAiController)
    {
        // mUseStayOnTrackCheat = true;
        mAiController = new Controller();
        mAiBotSettings = new AiBotSettings();
#if defined(NEURAL_AI)
//        mNeuralNet = new NeuralNet();
//        mTrainingNeuralNet = new NeuralNet();
//
//        int inputSize = AiInputData::GetNumInputs()+1;
//        int hiddenSize = 15;
//        int outputSize = 2;
//
//        mNeuralNet->Initialize(inputSize, hiddenSize, outputSize, 0.05f);
//
//        if ( mGame.GetAiTraining() )
//        {
//            mEnableFxSounds = false;
//            mNeuralNet->RandomizeWeights(-1.f, 1.f);
//    //        mNeuralNet->RandomizeWeightBias();
//    //        mNeuralNet->SetHiddenBiasForOutputNeuron(0, 0.5f);
//    //        mNeuralNet->SetHiddenBiasForOutputNeuron(1, 0.0f);
//    //        mNeuralNet->SetBoostWinnerWeights(0.0001f);
//            *mTrainingNeuralNet = *mNeuralNet;
//        }
//        else
//        {
//            std::string fileNN( APP.GetConfig()->MakeFilenameLevel("easy.nn") );
//            mNeuralNet->Load(fileNN.c_str());
//        }
#endif
    }
    else if ( mAiController )
    {
        delete mAiController;
        mAiController = NULL;
        delete mAiBotSettings;
        mAiBotSettings = NULL;
#if defined(NEURAL_AI)
//        delete mNeuralNet;
//        mNeuralNet = NULL;
//        delete mTrainingNeuralNet;
//        mTrainingNeuralNet = NULL;
#endif
    }
    UpdateShadow();
    UpdateDriftMarkers();
}

bool Player::Init()
{
    ChangeModel(mSettings.mModelFileName.c_str());

    mMeshBlobShadow = APP.GetNodeManager()->LoadNode(APP.GetConfig()->GetBlobModelName());
    if ( mMeshBlobShadow )
    {
        mMeshBlobShadow->setVisible(false);
    }

    for ( int i=0; i < 10; ++i )
    {
        scene::ISceneNode* node = APP.GetNodeManager()->LoadNode("drift_marker");
        if ( node )
        {
            node->setVisible(false);
            mDriftMarkers.push_back(node);
        }
    }

    return true;
}

void Player::ChangeModel(const std::string &name_)
{
    if ( name_ == mNameHoverLoaded && !mIsGhost )
        return;
    mNameHoverLoaded.clear();

    if ( mMeshHover )
    {
        mMeshHover->removeAll();
        mMeshHover->remove();
        mMeshHover = NULL;
    }

    mMeshHover = APP.GetNodeManager()->LoadNode(name_);
    mIsGhost = false;
	if ( mMeshHover )
	{
	    mNameHoverLoaded = name_;
//	    std::string fileName(APP.GetConfig()->MakeFilenameModels("hc_shadow.obj"));
//	    scene::IAnimatedMeshSceneNode* shadowParent = APP.GetIrrlichtManager()->LoadAnimatedModel(*APP.GetConfig(), fileName.c_str());
//	    shadowParent->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
//	    shadowParent->setPosition( core::vector3df(100, 100, 0) );
//	    scene::IShadowVolumeSceneNode *shadow = shadowParent->addShadowVolumeSceneNode(-1);
////        //scene::IShadowVolumeSceneNode *shadow =  shadowParent->addShadowVolumeSceneNode(-1, true, 0.5);
////	    //shadowParent->setVisible(false);
//        mMeshHover->addChild(shadowParent);

        mMeshHover->setVisible(false);
        DisableAllEffects();
	}
}

void Player::DisableAllEffects()
{
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "teleport_fx", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "teleport_fx2", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "dust01", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "dust02", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "Hover_fire", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "fireparticles", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "smoke01", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "smoke02", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "sparkles01", false);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "sparkles02", false);
}

void Player::EnablePermanentEffects()
{
    if ( APP.GetConfig()->GetParticleMode() != EPM_ALL_OFF )
    {
        APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "smoke01", true);
        APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "smoke02", true);
    }
}

Controller* Player::GetActiveController()
{
    if ( mGame.GetMode() != GM_RUNNING )
        return NULL;

    switch ( mPlayerType )
    {
        case PT_LOCAL:
        {
            return APP.GetController();
        }
        case PT_AI:
        {
            return mAiController;
        }
        default:
            break;
    }
    return NULL;
}

// hardcoded rules
void Player::UpdateAiManual(u32 time_)
{
    RandomGenerator randGen;
    float power = mAiController->GetPower();
    float oldRotation = mAiController->GetRotation();
    float rotation = oldRotation;

    if ( !mAiBotSettings )
        return;

    PhysicsObject* hoverPhysics = APP.GetPhysics()->GetPhysicsObject(mPhysicsId);
    if ( !hoverPhysics )
        return;
    const SteeringSettings& steeringSettings = mGame.GetSteeringSettings();

    // mVelToPreviewAngleScaled: value between 0 and 1 where: 0 is for 0 deg, 0.5 for 90 deg. 1 or -1 for 180 deg and -0.5 for 270 deg
    float angle = mAiInputData.mVelToIdealAngleScaled[1] - mAiInputData.mVelToDirAngleScaled;
    if ( time_ - mTimeTouchedWall < 500)    // this prevents getting stuck at u-turns
    {
        angle = mAiInputData.mVelToIdealAngleScaled[0] - mAiInputData.mVelToDirAngleScaled;
    }
    angle *= 180;

    //float approximationTime = 0.01f;
    float approximationTime = mGame.GetGameTimer()->GetLastTickInSeconds();
    rotation = -ApproximateRotationForAngle(angle, approximationTime, *hoverPhysics, steeringSettings);

    float speed = hoverPhysics->GetSpeedAbsolute();
    const AiTrack& aiTrack = APP.GetLevelManager()->GetAiTrack();
    const AiTrackInfo& info = aiTrack.GetTrackInfo(mTrackMarkerReached);
    float targetSpeed = info.mSettings.mMaxSpeed;
    // printf("targetSpeed %f power %f\n", targetSpeed, power);
    if ( info.mSettings.mMaxSpeed < mAiBotSettings->mSlowBorder*hoverPhysics->mSettings.mMaxSpeed )
    {
        targetSpeed *= randGen.GetNumberInRange(mAiBotSettings->mLowSlow, mAiBotSettings->mUppSlow);
    }
    targetSpeed *= randGen.GetNumberInRange(mAiBotSettings->mLowPower, mAiBotSettings->mUppPower);

    float MAX_SPEED_CHANGE = approximationTime*10.f;
    if ( targetSpeed < speed )
    {
        power -= MAX_SPEED_CHANGE;
    }
    else if ( targetSpeed > speed )
    {
        power += MAX_SPEED_CHANGE;
    }
    // printf("targetSpeed %f power %f\n", targetSpeed, power);

    power = MAX(power, -1);
    power = MIN(power, 1);
    rotation = MAX(rotation, -1);
    rotation = MIN(rotation, 1);

    // first few seconds (Everything allowed here - even power bigger 1)
    if ( time_ - mGame.GetRaceStartTime() < (u32)mAiBotSettings->mStartTime )
    {
        power = randGen.GetNumberInRange(mAiBotSettings->mLowPowerStart, mAiBotSettings->mUppPowerStart);
    }

    mAiController->SetPower(power);
    mAiController->SetRotation(rotation);

    // cheating AI - just change the energy vectors of physics, so it fit's... (not nice, but works)
    int timeSinceCollision = time_-mTimeLastObjCollision;
    if (  timeSinceCollision > mAiBotSettings->mCollTime )
    {
        int timeReturningToPath = timeSinceCollision-mAiBotSettings->mCollTime;
        float changeFactor = randGen.GetNumberInRange(mAiBotSettings->mLowEnergy, mAiBotSettings->mUppEnergy);

        if ( mAiBotSettings->mCollTimeReturn > 0 && timeReturningToPath < mAiBotSettings->mCollTimeReturn )
        {
            changeFactor *= (float)timeReturningToPath / (float)mAiBotSettings->mCollTimeReturn;
        }


        core::vector2df oldVelocity(hoverPhysics->GetVelocity().X, hoverPhysics->GetVelocity().Z);
        float velLength = oldVelocity.getLength();
        oldVelocity.normalize();

        core::vector2df previewIdeal(mPreviewDirIdeal[0].X, mPreviewDirIdeal[0].Z);
        core::vector2df previewCenter(mPreviewDir[0].X, mPreviewDir[0].Z);

        previewIdeal.normalize();
        previewCenter.normalize();

        float deltaIdeal = randGen.GetNumberInRange(mAiBotSettings->mLowIdeal, mAiBotSettings->mUppIdeal);
        core::vector2df targetVelocity( previewIdeal*deltaIdeal + previewCenter*(1-deltaIdeal) );
        targetVelocity.normalize();

        core::vector2df newVelocity( (targetVelocity * changeFactor) + (oldVelocity * (1.f-changeFactor)) );
        newVelocity.normalize();
        newVelocity *= velLength;

        hoverPhysics->SetVelocity( core::vector3df( newVelocity.X, hoverPhysics->GetVelocity().Y, newVelocity.Y ) );
    }
}

void Player::UpdateAiInputData(u32 time_)
{
    Physics * physics = APP.GetPhysics();
    if ( !physics )
        return;

    PhysicsObject* hoverPhysics = physics->GetPhysicsObject(mPhysicsId);
    if ( !hoverPhysics )
        return;

    mAiLastInputData = mAiInputData;
    float speed = hoverPhysics->GetSpeedAbsolute();
    mAiInputData.mSpeedScaled = speed / hoverPhysics->mSettings.mMaxSpeed;   // 0 to 1mVelToPreviewAngleScaled

    core::vector3df objCenter(mMeshHover->getTransformedBoundingBox().getCenter());
//    // debug draw
//    video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();
//    APP.GetIrrlichtManager()->SetDriverDrawMode();
//    driver->draw3DLine (objCenter, core::vector3df(objCenter.X, objCenter.Y + 100, objCenter.Z), video::SColor(255, 255, 255, 255));

    core::vector3df currentDir(0,0,1);
    const core::matrix4 & transform =	mMeshHover->getAbsoluteTransformation();
    transform.rotateVect(currentDir);
    core::vector3df currDirXZ(currentDir);
    currDirXZ.Y = 0.f;
    currentDir.normalize();
    currDirXZ.normalize();
//    // debug draw
//    video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();
//    APP.GetIrrlichtManager()->SetDriverDrawMode();
//    driver->draw3DLine (objCenter, objCenter+currDirXZ*100.f, video::SColor(255, 255, 255, 255));

    core::vector3df normVelocity(speed > 1.f ? hoverPhysics->GetVelocity() : currentDir);
    core::vector3df normVelocityXZ(normVelocity);
    normVelocityXZ.Y = 0.f;
    normVelocityXZ.normalize();
    normVelocity.normalize();

    mAiInputData.mVelToDirAngleScaled = CalcScaledAngleToVector(normVelocityXZ, currDirXZ);

    static int highestTrainingRecordIndex = 0;
    AiTrack& aiTrack = APP.GetLevelManager()->GetAiTrack();

    if ( mHighestValidTrackMarker > highestTrainingRecordIndex )
    {
        //fprintf(stderr, "rec: %d ", mHighestValidTrackMarker );
        highestTrainingRecordIndex = mHighestValidTrackMarker;
    }
    if ( mTrackMarkerReached >= 0 )
    {
        const float MAX_DIST_TO_CENTER = 500.f;
        float deltaLine = 0.f;
        core::vector3df closestPoint;
        int indexNearest = aiTrack.GetNearestPosOnCenterLine(objCenter, mTrackMarkerReached, deltaLine, mClosestPointOnTrack, mDistToTrackCenter);
        mTrainingDistToTrackCenterSum += fabs(mDistToTrackCenter);
        if ( mDistToTrackCenter >= MAX_DIST_TO_CENTER )
            mAiInputData.mDistToCenter =  1.f;
        else if ( mDistToTrackCenter <= -MAX_DIST_TO_CENTER )
            mAiInputData.mDistToCenter = -1.f;
        else
            mAiInputData.mDistToCenter = mDistToTrackCenter / MAX_DIST_TO_CENTER;

        const float MAX_DIST_TO_BORDER = 5000.f;
        float distDirToBorder = MAX_DIST_TO_BORDER;
        float distVelToBorder = MAX_DIST_TO_BORDER;
        aiTrack.GetBorderDist(indexNearest, objCenter, currentDir, MAX_DIST_TO_BORDER, distDirToBorder);
        aiTrack.GetBorderDist(indexNearest, objCenter, normVelocity, MAX_DIST_TO_BORDER, distVelToBorder);
        mAiInputData.mScaledDistDirToBorder = fabs(distDirToBorder)/MAX_DIST_TO_BORDER;
        mAiInputData.mScaledDistVelToBorder = fabs(distVelToBorder)/MAX_DIST_TO_BORDER;
//        // debug draw
//        core::vector3df borderPoint1(objCenter + currentDir*fabs(distDirToBorder));
//        core::vector3df borderPoint2(objCenter + normVelocity*fabs(distVelToBorder));
//        video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();
//        APP.GetIrrlichtManager()->SetDriverDrawMode();
//        driver->draw3DLine (objCenter, borderPoint1, video::SColor(255, 255, 255, 255));
//        driver->draw3DLine (objCenter, borderPoint2, video::SColor(255, 255, 255, 255));


        const float MAX_PREVIEW_HEIGHT_DIFF = 500.f;

        // next position on track
        core::vector3df previewPos;
        core::vector3df previewPosIdeal;
        core::vector3df previewDir;
        core::vector3df previewDirXZ;
        core::vector3df previewDirIdealXZ;

        mRelativePreviewDist[0] = 150.f;
        mRelativePreviewDist[1] = 800.f;
        if ( mAiBotSettings )
        {
            mRelativePreviewDist[0] = mAiBotSettings->mPreviewDist1;
            mRelativePreviewDist[1] = mAiBotSettings->mPreviewDist2;
        }

        for ( int pr = 0; pr < AI_TRACK_PREVIEWS; ++pr )
        {
            int dummyTargetIdx=0;
            aiTrack.GetPosAtDist(indexNearest, deltaLine, mRelativePreviewDist[pr], dummyTargetIdx, previewPos, previewPosIdeal);

//            // debug draw
//            video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();
//            APP.GetIrrlichtManager()->SetDriverDrawMode();
//            //driver->draw3DLine (previewPos, core::vector3df(previewPos.X, previewPos.Y + 100, previewPos.Z), video::SColor(255, 255, 255, 255));
//            driver->draw3DLine (previewPosIdeal, core::vector3df(previewPosIdeal.X, previewPosIdeal.Y + 100, previewPosIdeal.Z), video::SColor(255, 255, 255, 255));

            mPreviewDir[pr] = previewPos - objCenter;
            mPreviewDirIdeal[pr] = previewPosIdeal - objCenter;

            previewDirXZ = mPreviewDir[pr];
            if ( previewDirXZ.getLengthSQ() < 0.00001 )
                previewDirXZ = currDirXZ;
            previewDirXZ.Y = 0.f;
            previewDirXZ.normalize();

            previewDirIdealXZ = mPreviewDirIdeal[pr];
            if ( previewDirIdealXZ.getLengthSQ() < 0.00001 )
                previewDirIdealXZ = currDirXZ;
            previewDirIdealXZ.Y = 0.f;
            previewDirIdealXZ.normalize();

            if ( previewDir.Y >= MAX_PREVIEW_HEIGHT_DIFF )
                mAiInputData.mPreviewHeightFactor[pr] = 1.f;
            else if ( previewDir.Y <= -MAX_PREVIEW_HEIGHT_DIFF )
                mAiInputData.mPreviewHeightFactor[pr] = -1.f;
            else
                mAiInputData.mPreviewHeightFactor[pr] = previewDir.Y / MAX_PREVIEW_HEIGHT_DIFF;

            mAiInputData.mVelToPreviewAngleScaled[pr] = CalcScaledAngleToVector(normVelocityXZ, previewDirXZ);
            mAiInputData.mVelToIdealAngleScaled[pr] = CalcScaledAngleToVector(normVelocityXZ, previewDirIdealXZ);
        }
    }
}

float Player::CalcScaledAngleToVector(const core::vector3df &normDirXZ_, const core::vector3df &normTargetDirXZ_ )
{
    float crossY = normDirXZ_.crossProduct(normTargetDirXZ_).Y;
    float dot = normDirXZ_.dotProduct(normTargetDirXZ_);

    float result = ExtMath::acos_save(dot)/M_PI;
    if ( crossY < 0 )
        result *= -1.f;

    return result;
}

void Player::UpdateAi(u32 time_)
{
    if ( !mAiController )
        return;

    UpdateAiInputData(time_);

    UpdateAiManual(time_);
    return;

//    float timeLastFrame = mGame.GetGameTimer()->GetLastTickInSeconds();
//    int neurons = mAiInputData.FillNetInputLayer(mNeuralNet, 0);
//    mNeuralNet->SetInputValue(neurons, timeLastFrame);
//
//    mNeuralNet->Run();
//
//    float power = 3 * mNeuralNet->GetOutputValue(0) - 1.5f;  // 3* instead of 2* to make it easier to get to min/max values
//    power = MAX(power, -1);
//    power = MIN(power, 1);
//    float rotation = 3 * mNeuralNet->GetOutputValue(1)  - 1.5f;
//    rotation = MAX(rotation, -1);
//    rotation = MIN(rotation, 1);
//
//    mAiController->SetPower(power);
//    mAiController->SetRotation(rotation);
}

#if defined(NEURAL_AI)
float Player::CalculateAiTrainingAward(u32 time_)
{
    float award = 0.f;
    const float MALUS_NO_IMPROVE = 0.0f;
    const float BONUS_NO_DROP = 1.f;
    const float BONUS_TRACK = 2.f;
    int numAiTrackInfos = APP.GetLevelManager()->GetAiTrack().GetNumTrackInfos();

    if ( mLastTrainingTime == 0)
    {
        mLastTrainingTime = time_;
        mLastTrainingTimeIndex = 0;
        //fprintf(stderr, "reset, new index: %d\n", mLastTrainingTimeIndex);
    }

    award = mCurrentRound*numAiTrackInfos*BONUS_TRACK;
    if ( mTrackMarkerReached + 10 < mHighestValidTrackMarker )
        award += mTrackMarkerReached*BONUS_TRACK;
    else
        award -= BONUS_TRACK*(((mHighestValidTrackMarker+numAiTrackInfos)-mTrackMarkerReached) % numAiTrackInfos);
    award -= BONUS_TRACK*mTrainingStartIndex;

    award -= (time_ - mTimeHighestValidTrackMarker)*MALUS_NO_IMPROVE;

    const int MAX_TIME_NO_DROP = 15;
    int timeNoDrop = time_ - mTrainingLastDroppedTime;
    if ( timeNoDrop > MAX_TIME_NO_DROP )
        timeNoDrop = MAX_TIME_NO_DROP;
    award += BONUS_NO_DROP* ((float)timeNoDrop/(float)MAX_TIME_NO_DROP);

    // TEST
//    award -= mTrainingDistToTrackCenterSum * 0.01f;
    mTrainingDistToTrackCenterSum = 0.f;

    mLastTrainingTime = time_;
    mLastTrainingTimeIndex = mTrackMarkerReached;
    if ( mNeuralNet && mTrainingNeuralNet )
        *mTrainingNeuralNet = *mNeuralNet;

    return award;
}
#endif // NEURAL_AI

float Player::CalcAngleForRotation(float rotation_, float timeSec_, const PhysicsObject& hoverPhysics_, const SteeringSettings& settings_)
{
    float speed = hoverPhysics_.GetSpeedAbsolute();

    float angle = timeSec_ * 360 * rotation_;
    if ( speed > settings_.mScaleMinSpeed
        &&    (mPlayerType != PT_AI || !EASY_BOT_CONTROL ) )
    {
        float speedFactor = 0.f;
        if ( hoverPhysics_.mSettings.mMaxSpeed-settings_.mScaleMinSpeed != 0.f )
            speedFactor = (speed-settings_.mScaleMinSpeed) / (hoverPhysics_.mSettings.mMaxSpeed-settings_.mScaleMinSpeed);

        angle *= (1-settings_.mScaleRotationBySpeed * speedFactor);
    }

    angle *= settings_.mRotationSpeed;

    return angle;
}

float Player::ApproximateRotationForAngle(float angle_, float timeSec_, const PhysicsObject& hoverPhysics_, const SteeringSettings& settings_)
{
    if ( timeSec_ <= 0 )
        return 0.f;

    float speed = hoverPhysics_.GetSpeedAbsolute();

    angle_ /= settings_.mRotationSpeed;
    if ( speed > settings_.mScaleMinSpeed
        &&    (mPlayerType != PT_AI || !EASY_BOT_CONTROL ) )
    {
        float speedFactor = 0.f;
        if ( hoverPhysics_.mSettings.mMaxSpeed-settings_.mScaleMinSpeed != 0.f )
            speedFactor = (speed-settings_.mScaleMinSpeed) / (hoverPhysics_.mSettings.mMaxSpeed-settings_.mScaleMinSpeed);
        float h=(1-settings_.mScaleRotationBySpeed * speedFactor);
        if ( h != 0.f )
            angle_ /= h;
    }

    angle_ /= (timeSec_ * 360);
    return angle_;
}

void Player::PrePhysicsUpdate(u32 time_)
{
    if ( mMeshHover )
    {
        mLastPos = mMeshHover->getAbsolutePosition();
    }

    if ( !IsActiveType() )
    {
        return;
    }

    if ( mPhysicsId < 0 )
        return;

    if ( mPlayerType == PT_AI )
    {
        UpdateAi(time_);
    }

    Physics * physics = APP.GetPhysics();
    assert(physics);

    PhysicsObject* hoverPhysics = physics->GetPhysicsObject(mPhysicsId);
    if ( !hoverPhysics )
        return;

    Controller* controller = GetActiveController();

    // rotate hover depending on controller rotation axis
    const SteeringSettings& steeringSettings = mGame.GetSteeringSettings();
    float timeLastFrame = mGame.GetGameTimer()->GetLastTickInSeconds();
    float angle = 0.f;
    if ( controller )
    {
        angle = CalcAngleForRotation(controller->GetRotation(), timeLastFrame, *hoverPhysics, steeringSettings);
    }

    core::vector3df rotVector(0,-angle,0);
    core::matrix4 newYawMat;
    newYawMat.setRotationDegrees(rotVector);
    mYawMat *= newYawMat;

    core::matrix4 matRot( mYawMat );
    matRot *= mAlignmentMat;
    matRot *= mRollingMat;

    mMeshHover->setRotation(ExtIrr::GetRotationDegreesSave(matRot));

    // add physics force
    core::matrix4 transformAbs(mMeshHover->getAbsoluteTransformation());
    core::matrix4 transformXZ;
    ExtIrr::AlignToUpVector(transformXZ, transformAbs, core::vector3df(0,1,0), 1.f);
    if ( controller )
    {
        if ( controller->GetPower() > 0.f )
        {
            core::vector3df force(0.f, 0.f, controller->GetPower() * steeringSettings.mForceFactor);
            transformXZ.rotateVect(force);

            hoverPhysics->AddForce(core::vector3df(force));
        }
        else if ( controller->GetPower() < 0.f )
        {
            hoverPhysics->AddFriction(-controller->GetPower() * steeringSettings.mBrakeFactor);
        }

        if ( steeringSettings.mSteeringStyle > 0 )
        {
			// H-crafts should usually push away from rotor.
			// Let's try faking this a little bit.
			// Added for H-Craft 1.3, might not be used at all.
			core::vector3df vecLeft(-1.f,0,0);
			mYawMat.rotateVect(vecLeft);
			hoverPhysics->AddForce(vecLeft*controller->GetRotation()*steeringSettings.mForceSideFactor);

			// Was an idea - make side-movement depend on rotation angle. Would have made some
			// sense physically, but plays rather horrible.
	//        core::vector3df rollVec(1, 0, 0);
	//		mRollingMat.rotateVect(rollVec);
	//		hoverPhysics->AddForce(vecLeft*rollVec.Y*100000.f);
		}
    }
    else
    {
        // brake!
        hoverPhysics->AddFriction(steeringSettings.mBrakeFactor);
    }

#if 1
    // Helper for humans - to be introduced in h-craft 1.3
    // Fake some kind of energy wall around the tracks to prevent players from falling off so often
    if ( mPlayerType == PT_LOCAL )
    {
		// This should maybe be inside the physics itself as it's about collisions
		// and this way it's delayed one frame. But to implement that clean the physics
		// has to learn the concept of collision lines and track-centers which I don't
		// have the time for so far.
		PhysicsTrackBorderInfo trackBorderInfo;

		if ( mTrackMarkerCurrent >= 0 )
		{
			// debug draw
			video::IVideoDriver * driver = 0; // APP.GetIrrlichtManager()->GetVideoDriver();
//			APP.GetIrrlichtManager()->SetDriverDrawMode();

			AiTrack& aiTrack = APP.GetLevelManager()->GetAiTrack();

			trackBorderInfo.mDistanceBorder = aiTrack.GetNearestPosOnBorder(mLastPos, mTrackMarkerCurrent, trackBorderInfo.mNearestBorder, true, driver);
			float deltaDummy = 0.f;
			aiTrack.GetNearestPosOnCenterLine(mLastPos, mTrackMarkerCurrent, deltaDummy, trackBorderInfo.mNearestTrackCenter, trackBorderInfo.mDistanceTrackCenter);
			trackBorderInfo.mTrackDir = aiTrack.GetCenterLineDirection(mTrackMarkerCurrent);

			// debug draw
//			driver->draw3DLine(trackBorderInfo.mNearestTrackCenter, trackBorderInfo.mNearestBorder, video::SColor(255, 255, 255, 255));
//			driver->draw3DLine(mLastPos, trackBorderInfo.mNearestBorder, video::SColor(255, 255, 0, 0));
//			driver->draw3DLine(mLastPos, trackBorderInfo.mNearestTrackCenter, video::SColor(255, 0, 255, 0));
		}

		hoverPhysics->SetTrackBorderInfo(trackBorderInfo);
    }
#endif

    // evil cheat for ai (currently no longer used)
    if ( mUseStayOnTrackCheat && hoverPhysics->mHasTouchedWorldGeometry )
    {
        core::vector3df objCenter(mMeshHover->getTransformedBoundingBox().getCenter());
        core::vector3df cheatDir(mClosestPointOnTrack-objCenter);
        cheatDir.Y = 0.f;
        cheatDir *= timeLastFrame * 80;
        cheatDir *= cheatDir.getLength();
        hoverPhysics->SetForceAccu( hoverPhysics->GetForceAccu() + cheatDir );
    }

#if defined(NEURAL_AI)
    if ( mAiTrainingDataFile )
    {
        UpdateAiInputData(time_);
        mAiInputData.PrintToFile(mAiTrainingDataFile);
        fprintf(mAiTrainingDataFile, "\n");
        if ( controller )
        {
            fprintf(mAiTrainingDataFile, "%f %f\n",  controller->GetPower(), controller->GetRotation());
        }
    }
#endif

    if ( mTimeFreezing && time_ >= mTimeFreezing )
    {
        mTimeFreezing = 0;
    }
    else if ( mTimeFreezing )
    {
        // freeze
        hoverPhysics->SetForceAccu(core::vector3df(0,0,0));
        if ( controller )
        {
            controller->SetPower(0.f);
            controller->SetRotation(0.f);
        }
    }
    if ( mTimeNoCollision && time_ >= mTimeNoCollision )
    {
        mTimeNoCollision = 0;
        hoverPhysics->SetCollisionType(CT_COLL_ALL);
        hoverPhysics->mDelayObjectCollUntilSafe = true;
    }
}

void Player::ResetMarkersReached(u32 time_, bool createTimers_)
{
	mTrackMarkerCurrent = -1;
    mTrackMarkerReached = 0;
    mTimeMarkerChanged = time_;
    mHighestValidTrackMarker = 0;
    mTimeHighestValidTrackMarker = time_;
    mTrackMarkerReachedTimes.clear();

    if ( createTimers_ )
    {
        AiTrack& aiTrack = APP.GetLevelManager()->GetAiTrack();
        mTrackMarkerReachedTimes.resize(aiTrack.GetNumTrackInfos(), time_);
    }
}

// Markers where originally only for AI, but turned out to be useful for
// other stuff as well so all player update that info.
void Player::UpdateMarkersReached(u32 time_, bool searchAll_ )
{
    core::vector3df objCenter(mMeshHover->getTransformedBoundingBox().getCenter());
    AiTrack& aiTrack = APP.GetLevelManager()->GetAiTrack();
    float dist = 0.f;
    mTrackMarkerCurrent = aiTrack.GetIndexForNearestCenter(mTrackMarkerReached, objCenter, dist, searchAll_);
    if ( dist > 1000.f && !searchAll_ )
    {
		searchAll_ = true;
		mTrackMarkerCurrent = aiTrack.GetIndexForNearestCenter(mTrackMarkerReached, objCenter, dist, searchAll_);
    }

    if (    aiTrack.GetMarkerSequence( mTrackMarkerCurrent, mTrackMarkerReached) >= 0
        ||  time_ - mTimeMarkerChanged > 500
        ||  searchAll_ )
    {
        mTrackMarkerReached = mTrackMarkerCurrent;
        mTimeMarkerChanged = time_;
        if ( mTrackMarkerReached >= 0 && mTrackMarkerReached < (int)mTrackMarkerReachedTimes.size() )
        {
            mTrackMarkerReachedTimes[mTrackMarkerReached] = time_;
        }
    }

    if (    mTrackMarkerReached > mHighestValidTrackMarker
        &&  aiTrack.GetMarkerSequence( mTrackMarkerReached, mHighestValidTrackMarker) > 0 )
    {
        mHighestValidTrackMarker = mTrackMarkerReached;
        mTimeHighestValidTrackMarker = time_;
    }
//    if( PT_AI == mPlayerType )
//    {
//        printf("lastmarker: %d highestmarker: %d\n", mTrackMarkerReached, mHighestValidTrackMarker );
//    }
}

void Player::PostPhysicsUpdate(u32 time_)
{
    if ( PT_UNKNOWN != mPlayerType )
    {
        PROFILE_START(400);
        // where is the hover, we need that info even for ghosts sometimes
        UpdateMarkersReached(time_, false);
        PROFILE_STOP(400);
    }

    if ( !IsActiveType() )
    {
		mTimeLastPostPhysicsUpdate = time_;
        return;
    }

    Physics * physics = APP.GetPhysics();
    assert(physics);

    PhysicsObject* hoverPhysics = physics->GetPhysicsObject(mPhysicsId);

    PROFILE_START(401);
    const Controller* controller = GetActiveController();

    mMeshHover->updateAbsolutePosition();

    float rotation = 0.f;
    if ( controller )
        rotation = -controller->GetRotation();
    CalcRollMatrices(rotation);
    core::matrix4 matRot( mYawMat );
    matRot *= mAlignmentMat;
    matRot *= mRollingMat;

    mMeshHover->setRotation(ExtIrr::GetRotationDegreesSave(matRot));
    PROFILE_STOP(401);

    PROFILE_START(402);
    UpdateShadow();
    UpdateDriftMarkers();

    // disable tp effect
    if ( mTimeDisableTeleportFx > 0
        && mTimeDisableTeleportFx <= time_
        && hoverPhysics
        && !hoverPhysics->mDelayObjectCollUntilSafe
        )
    {
        EnableTeleportFx(false);
        mTimeDisableTeleportFx = 0;
    }

    // brakelight
    if ( controller && controller->GetPower() < 0.f )
    {
        APP.GetIrrlichtManager()->SetEmissiveByTextureName(mMeshHover, 255, 0, 0, "ra" );
    }
    else
    {
        APP.GetIrrlichtManager()->SetEmissiveByTextureName(mMeshHover, 255, 255, 255, "ra" );
    }
    PROFILE_STOP(402);

    PROFILE_START(403);
    if ( hoverPhysics )
    {
        if ( hoverPhysics->HasTouchedWall() )
        {
            mTimeTouchedWall = time_;
        }

        if ( hoverPhysics->mHasTouchedWorldGeometry )
        {
            if (    mLastTimeTouchedFloor != 0
                &&  mPlayerType == PT_LOCAL
                &&  (time_ - mLastTimeTouchedFloor) > 500
                )
            {
                PlaySample(mSoundRefHitGround, "hc_hitground01.wav", 1.f, false);
            }
            mLastTimeTouchedFloor = time_;
        }

		// Vibrate mobile phone when the player run too hard into the cushions.
		// TODO: I would prefer adding a graphical effect when I find time for it.
		const int VIBRATION_TIME_MS = 10;
		const int VIBRATION_REPEAT = VIBRATION_TIME_MS + 20;
		const float MIN_CUSHION_TO_VIBRATE = 65.f;	// try & error value
		float timeDiffSeconds = float(time_-mTimeLastPostPhysicsUpdate)/1000.f;
		float cushioningPerSec = timeDiffSeconds > 0 ? hoverPhysics->GetCushioningLastFrame() / timeDiffSeconds : 0.f;
		if ( mPlayerType == PT_LOCAL && cushioningPerSec > MIN_CUSHION_TO_VIBRATE && mTimeLastVibrate+VIBRATION_REPEAT < time_)
		{
			mTimeLastVibrate = time_;
#ifdef _IRR_ANDROID_PLATFORM_
			hc1::android::vibrate(APP.GetAndroidApp(), VIBRATION_TIME_MS);
#endif
		}

        float speed = hoverPhysics->GetSpeedAbsolute();
        float speedScaled = hoverPhysics->GetSpeedScaled();
        if ( hoverPhysics->HasTouchedObject() )
        {
            mTimeLastObjCollision = time_;
        }

        if ( APP.GetConfig()->GetParticleMode() != EPM_ALL_OFF )
        {
            // sparkle-effekt on left/right when rolling much on high speeds
            core::vector3df sparkVec(1, 0, 0);
            mRollingMat.rotateVect(sparkVec);
            if ( sparkVec.Y > APP.GetConfig()->GetSparkleRotMin()
                && speedScaled > APP.GetConfig()->GetSparkleSpeedMin() )
            {
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "sparkles01", true);
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "sparkles02", false);
            }
            else if ( sparkVec.Y < -APP.GetConfig()->GetSparkleRotMin()
                    && speedScaled > APP.GetConfig()->GetSparkleSpeedMin())
            {
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "sparkles01", false);
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "sparkles02", true);
            }
            else
            {
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "sparkles01", false);
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "sparkles02", false);
            }

            if (    hoverPhysics->HasTouchedWall()
                ||  hoverPhysics->HasTouchedObject() )
            {
                if ( mPlayerType == PT_LOCAL )
                {
                    PlaySample(mSoundRefCollision, "hc_collision01.wav", 1.f, false);
                }
            }

            if ( speed < 100.f )
            {
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "dust01", true);
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "dust02", true);
            }
            else
            {
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "dust01", false);
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "dust02", false);
            }
        }
        if ( controller )
        {
            if ( controller->GetPower() > 0.1f )
            {
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "Hover_fire", true);
                if ( APP.GetConfig()->GetParticleMode() != EPM_ALL_OFF )
                {
                    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "fireparticles", true);
                }
            }
            else
            {
                APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "Hover_fire", false);
                if ( APP.GetConfig()->GetParticleMode() != EPM_ALL_OFF )
                {
                    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "fireparticles", false);
                }
            }
        }

        // sound for local player
        if ( mEnableFxSounds && mPlayerType == PT_LOCAL && APP.GetSound() )
        {
            float len = speedScaled * 0.5f;
            if ( controller )
                len += 0.5f * fabs(controller->GetPower());

            PlaySample(mSoundRefEngine, "hc_engine_split01.wav", 0.f, true);
            PlaySample(mSoundRefHovering, "hc_hovering01.wav", 0.f, true);

            if ( time_ - GetLastTimeTouchedFloor() > 100 )
            {
                PlaySample(mSoundRefAir, "hc_air01.wav", 1.f, false);
            }
            else
            {
                StopSample(mSoundRefAir);
            }

            if ( controller && controller->GetPower() <= 0 )
                mCanPlayEngineAccSound = true;
            if ( controller && controller->GetPower() >= 0 )
                mCanPlayBrakeSound = true;
            if ( controller && controller->GetPower() >= 0.f )
            {
                if ( mCanPlayEngineAccSound && controller->GetPower() > 0.f)
                {
                    if ( speed < 850.f )
                    {
                        PlaySample(mSoundRefEngine2, "hc_engine_split02.wav", 1.f, false);
                    }
                    else
                    {
                        StopSample(mSoundRefEngine2);
                    }
                    mCanPlayEngineAccSound = false;
                }

                APP.GetSound()->SetGain(mSoundRefBrake, 0.f);
                APP.GetSound()->SetGain(mSoundRefEngine, speedScaled);
                APP.GetSound()->SetGain(mSoundRefHovering, 1-speedScaled);
            }
            else
            {
                if ( mCanPlayBrakeSound )
                {
                    PlaySample(mSoundRefBrake, "hc_brake01.wav", 0.f, false);
                    mCanPlayBrakeSound = false;
                }

                APP.GetSound()->SetGain(mSoundRefBrake, len);
                APP.GetSound()->SetGain(mSoundRefEngine, 0.f);
                APP.GetSound()->SetGain(mSoundRefHovering, 1.f);
            }
        }
        // sound for other players
        else if ( mEnableFxSounds && mPlayerType != PT_LOCAL && APP.GetSound() )
        {
            if ( mGame.GetMode() == GM_RUNNING )
            {
                if ( mSoundRefHovering < 0 || !APP.GetSound()->IsPlaying(mSoundRefHovering) )
                {
                    std::string name(APP.GetConfig()->MakeFilenameSound(std::string("hc_hovering01.wav")));
                    //std::string name(APP.GetConfig()->MakeFilenameSound(std::string("hc_menu_confirm01.wav")));    // a noticable test
                    mSoundRefHovering = APP.GetSound()->PlaySample3D(name.c_str(), mMeshHover->getPosition(), 1.f, true);
                }
                else
                {
                    APP.GetSound()->SetPosition(mSoundRefHovering, mMeshHover->getPosition());
                }
            }
        }
    }
    mTimeLastPostPhysicsUpdate = time_;
    PROFILE_STOP(403);
}

void Player::UpdateShadow()
{
    // shadow
    if ( APP.GetConfig()->GetUseShadow() && mMeshBlobShadow )
    {
        if ( !mMeshHover->isVisible() || !IsActiveType() )
        {
            if ( mMeshBlobShadow->isVisible() )
            {
                mMeshBlobShadow->setVisible(false);
            }
            return;
        }

        core::line3df lineToFloor;
        lineToFloor.start = mMeshHover->getPosition();
        lineToFloor.end = lineToFloor.start;
        lineToFloor.end.Y -= 1000.f;
        core::vector3df floorPos;

            // debug draw
//            video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();
//            APP.GetIrrlichtManager()->SetDriverDrawMode();
//            driver->draw3DLine (lineToFloor.start, lineToFloor.end, video::SColor(255, 255, 255, 255));

        if ( APP.GetPhysics()->GetTrackIntersection(lineToFloor, lineToFloor.start, floorPos) )
        {
            if ( !mMeshBlobShadow->isVisible() )
                mMeshBlobShadow->setVisible(true);
            floorPos.Y += APP.GetConfig()->GetBlobHeight();
            mMeshBlobShadow->setPosition( floorPos );

            core::matrix4 matRot( mYawMat );
            matRot *= mAlignmentMat;
            mMeshBlobShadow->setRotation(ExtIrr::GetRotationDegreesSave(matRot));
        }
        else
        {
            if ( mMeshBlobShadow->isVisible() )
                mMeshBlobShadow->setVisible(false);
        }
    }
}

void Player::UpdateDriftMarkers()
{
    return; // TODO

    PhysicsObject* hoverPhysics = APP.GetPhysics()->GetPhysicsObject(mPhysicsId);
    if ( !hoverPhysics )
        return;

    core::vector3df velocity(hoverPhysics->GetVelocity());
    velocity = velocity*0.1;

    bool noUpdate = false;

    if ( velocity.getLength() <= 0.001f )
    {
        noUpdate = true;
    }

    for ( unsigned int i=0; i < mDriftMarkers.size(); ++ i )
    {
        if ( noUpdate || !mMeshHover->isVisible() || mPlayerType != PT_LOCAL )
        {
            if ( mDriftMarkers[i]->isVisible() )
            {
                mDriftMarkers[i]->setVisible(false);
            }
            noUpdate = true;
        }
    }
    if ( noUpdate )
    {
        return;
    }

    core::line3df oldLine;
    oldLine.start = mMeshHover->getPosition();
    oldLine.end = oldLine.start;
    oldLine.end.Y -= 1000.f;

    for ( unsigned int i=0; i < mDriftMarkers.size(); ++ i )
    {
        if ( !mDriftMarkers[i]->isVisible() )
        {
            mDriftMarkers[i]->setVisible(true);
        }

        core::vector3df floorPos;
        core::line3df newLine(oldLine);
        newLine.start += velocity;
        newLine.end += velocity;

        if ( !APP.GetPhysics()->GetTrackIntersection(newLine, newLine.start, floorPos) )
        {
            newLine.start.Y = oldLine.start.Y;
            newLine.end.Y = oldLine.end.Y;
            floorPos = newLine.start;
        }
        oldLine = newLine;
        floorPos.Y += APP.GetConfig()->GetBlobHeight();
        mDriftMarkers[i]->setPosition( floorPos );

        // TODO
        float dot = velocity.dotProduct(core::vector3df(1,0,0));    // careful, rounding errors will get this beyond 1
        dot = MIN(dot, 1.f);
        dot = MAX(dot, -1.f);
        float angle = fabs(acos(dot));
        mDriftMarkers[i]->setRotation(core::vector3df(0,angle,0));
    }
}

void Player::CalcRollMatrices(float additionalRoll_)
{
    Physics * physics = APP.GetPhysics();
    PhysicsObject* hoverPhysics = physics->GetPhysicsObject(mPhysicsId);
    if ( !hoverPhysics )
        return;

//    float speed = hoverPhysics->GetSpeedAbsolute();
//    float speedFactor = 1.f;
//    if ( hoverPhysics->mSettings.mMaxSpeed )
//        speedFactor = speed / hoverPhysics->mSettings.mMaxSpeed;

    const SteeringSettings& steeringSettings = mGame.GetSteeringSettings();
    float time = mGame.GetGameTimer()->GetLastTickInSeconds();

    // rotate the alignment matrix so that it's aligned to the sliding plane
    core::vector3df repulsion(hoverPhysics->GetRepulsionNormal());
    //core::matrix4 transformAbs(mMeshHover->getAbsoluteTransformation());
    //transformAbs.inverseRotateVect(repulsion);
    mYawMat.inverseRotateVect(repulsion);
    float scaleRotation = time * steeringSettings.mScaleAlignment;
    core::matrix4 oldMat( mAlignmentMat );
    if ( !hoverPhysics->mHasTouchedWorldGeometry )
    {
        core::matrix4 matRotXZ;
        ExtIrr::AlignToUpVector(matRotXZ, oldMat, core::vector3df(0,1,0), 1.f);
        repulsion.set(0, 0, -1.f);
        matRotXZ.rotateVect(repulsion);
        scaleRotation *= steeringSettings.mScalePitchFlying;
    }
    if ( mLastAlignmentVector != repulsion )
    {
        mLastAlignmentScaling = 0.f;
        mLastAlignmentVector = repulsion;
    }
    mLastAlignmentScaling = scaleRotation;
    if ( mLastAlignmentScaling > 1.f )
        mLastAlignmentScaling = 1.f;
    ExtIrr::AlignToUpVector(mAlignmentMat, oldMat, repulsion, mLastAlignmentScaling);

    // roll by controller
    float roll = additionalRoll_; // *additionalRoll_;
    roll = (4 / (1+exp(-additionalRoll_))) - 2;
    //mScaleRolling
    //if ( additionalRoll_ > 0.f )
        roll *= -1.f;

    float smoothRoll = 1-fabs(mLastRolling);
    if ( mLastRolling < roll )
    {
        if ( mLastRolling < 0 )
            mLastRolling += steeringSettings.mScaleRollingBack*time;
        else
            mLastRolling += steeringSettings.mScaleRolling*time*smoothRoll;
        if ( mLastRolling > roll )
            mLastRolling = roll;
    }
    else if ( mLastRolling > roll )
    {
        if ( mLastRolling > 0 )
            mLastRolling -= steeringSettings.mScaleRollingBack*time;
        else
            mLastRolling -= steeringSettings.mScaleRolling*time*smoothRoll;
        if ( mLastRolling < roll )
            mLastRolling = roll;
    }

    core::vector3df rollVec(0, 0, mLastRolling*steeringSettings.mMaxRolling);
    mRollingMat.setRotationDegrees(rollVec);
}

void Player::EnableTeleportFx(bool enable_)
{
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "teleport_fx", enable_);
    APP.GetIrrlichtManager()->SetNodeOrChildVisibleByName(mMeshHover, "teleport_fx2", enable_);
}

void Player::InfoPrepareForRace()
{
    if (    GetType() == PT_LOCAL
        ||  GetType() == PT_AI )
    {
		mPhysicsId = APP.GetPhysics()->AddPhysicsObject(mMeshHover, APP.GetConfig()->GetHoverRadius());
    }

    if ( GetType() == PT_UNKNOWN )
    {
        mMeshHover->setVisible(false);
    }
    else
    {
        TrackStart & spawn = APP.GetLevelManager()->GetTrackStart(GetTrackStart());
        mMeshHover->setPosition(spawn.mSettings.mCenter);
        mMeshHover->setRotation(spawn.mSettings.mRotation);
        mMeshHover->updateAbsolutePosition();
        mMeshHover->setVisible(true);
        PhysicsObject* hoverPhysics = APP.GetPhysics()->GetPhysicsObject(mPhysicsId);
        if ( hoverPhysics )
        {
            hoverPhysics->ForcePositionUpdate();

            if ( GetType() == PT_LOCAL )
				hoverPhysics->SetHuman(true);
			else
				hoverPhysics->SetHuman(false);
        }
    }

    if (    GetType() == PT_GHOST_TRACK
        ||  GetType() == PT_GHOST_LAP )
    {
        mMeshHover->setVisible(false);
        APP.GetIrrlichtManager()->MakeGhost(mMeshHover);
        mIsGhost = true;
    }

    mAlignmentMat.makeIdentity();
    mRollingMat.makeIdentity();
    mLastRolling = 0.f;
    mYawMat.makeIdentity();
    ExtIrr::SetMatrixRotation(mYawMat, mMeshHover->getAbsoluteTransformation());
    UpdateShadow();
    UpdateDriftMarkers();
    DisableAllEffects();
}

void Player::InfoStartCountDown(u32 time_)
{
    mCurrentRound = 0;
    mBestRoundTime = Game::TIME_UNUSED;
    mLevelMarkerReached = 0;
    mLastRoundTime = 0;
    mLastTimeTouchedFloor = 0;
    mLastTrainingTime = time_;
    mLastTrainingTimeIndex = 0;
    ResetMarkersReached(time_, true);
    mTrainingDroppedFromTrack = 0;
    mTrainingLastAward = 0.f;
    mTrainingAverageAward = 0.f;
    mTrainingTimeFirstDropped = 0;
    mNumDroppedLastRound = 0;
    mNumDroppedThisRound = 0;
    mTrainingLastDroppedTime = time_;
    mTrainingLastPos = mMeshHover->getTransformedBoundingBox().getCenter();

    mTrainingStartIndex = 0;
    mDistToTrackCenter = 0.f;
    mTrainingDistToTrackCenterSum = 0.f;
    mTimeDisableTeleportFx = 0;
    mAlignmentMat.makeIdentity();
    mRollingMat.makeIdentity();
    mLastRolling = 0.f;
    mLastPos = mMeshHover->getAbsolutePosition();
    mYawMat.makeIdentity();
    ExtIrr::SetMatrixRotation(mYawMat, mMeshHover->getAbsoluteTransformation());
    mAiLastInputData.Reset();
    mAiInputData.Reset();
    mTimeLastObjCollision = 0;
    mPlacing = 0;
    mTimeNoCollision = 0;
    mTimeTouchedWall = 0;
    mTimeLastVibrate = 0;
    mTimeFreezing = 0;
    UpdateShadow();
    UpdateDriftMarkers();
    EnablePermanentEffects();

    if ( mEnableFxSounds && mPlayerType == PT_LOCAL )
    {
        PlaySample(mSoundRefHovering, "hc_hovering01.wav", 1.f, true);
    }
}

void Player::InfoStartRace(u32 time_)
{
}

void Player::InfoTrainingAiTeleport(u32 time_)
{
    InfoStartRace(time_);
    UpdateMarkersReached(time_, true);
    mLastTrainingTimeIndex = mTrackMarkerReached;
    mTrainingStartIndex = mTrackMarkerReached;
}

void Player::InfoRoundFinished(u32 time_)
{
    bool validRound = false;
    if ( mCurrentRound == 0 )
    {
        validRound = true;
    }
    else
    {
        u32 roundTime = time_ - mRoundStartTime;
        const u32 NO_CHEAT = 5000;
        if ( roundTime > NO_CHEAT )
        {
            validRound = true;

            mLastRoundTime = roundTime;
            if ( mLastRoundTime < mBestRoundTime )
            {
                mBestRoundTime = mLastRoundTime;
            }
        }
    }
    if ( validRound )
    {
        ++ mCurrentRound;
        mRoundStartTime = time_;
        mRoundStartPhysicsTicks = APP.GetPhysics()->GetTicksSinceGameStart();
        mLevelMarkerReached = 0;
        mHighestValidTrackMarker = 0;
        mTimeHighestValidTrackMarker = time_;
        mNumDroppedLastRound = mNumDroppedThisRound;
        mNumDroppedThisRound = 0;
        mTrainingStartIndex = 0;

#ifdef _IRR_ANDROID_PLATFORM_
		if ( mPlayerType == PT_LOCAL && mCurrentRound > 1 )
		{
			const int VIBRATION_TIME_MS = 15;

			hc1::android::vibrate(APP.GetAndroidApp(), VIBRATION_TIME_MS);
		}
#endif
    }
}

void Player::InfoLevelMarkerReached( int marker_ )
{
    if ( marker_ > mLevelMarkerReached )
        mLevelMarkerReached = marker_;
}

void Player::InfoTeleport(u32 time_)
{
    if (  mPlayerType == PT_UNKNOWN || mPlayerType == PT_GHOST_TRACK || mPlayerType == PT_GHOST_LAP )
        return;

    if ( mEnableFxSounds && APP.GetSound() )
    {
        std::string name(APP.GetConfig()->MakeFilenameSound(std::string("hc_teleport01.wav")));
        APP.GetSound()->PlaySample(name.c_str(), 1.f, false);
    }

    ExtIrr::SetMatrixRotation(mYawMat, mMeshHover->getAbsoluteTransformation());
    mAlignmentMat.makeIdentity();
    mRollingMat.makeIdentity();
    mLastRolling = 0.f;
}

void Player::InfoDroppedFromTrack(u32 time_)
{
    if (  mPlayerType == PT_UNKNOWN || mPlayerType == PT_GHOST_TRACK || mPlayerType == PT_GHOST_LAP )
        return;

    ExtIrr::SetMatrixRotation(mYawMat, mMeshHover->getAbsoluteTransformation());
    mAlignmentMat.makeIdentity();
    mRollingMat.makeIdentity();
    mLastRolling = 0.f;
    mLastTimeTouchedFloor = time_;

    const SteeringSettings& steeringSettings = mGame.GetSteeringSettings();
    mTimeFreezing = time_ + steeringSettings.mRelocateFreeze;
    mTimeNoCollision = time_ + steeringSettings.mRelocateNoColl;
    PhysicsObject* hoverPhysics = APP.GetPhysics()->GetPhysicsObject(mPhysicsId);
    if ( hoverPhysics )
    {
        hoverPhysics->SetCollisionType(CT_COLL_WORLD);
    }

    if ( mPlayerType == PT_LOCAL && mEnableFxSounds && APP.GetSound() )
    {
        std::string name(APP.GetConfig()->MakeFilenameSound(std::string("hc_teleport01.wav")));
        APP.GetSound()->PlaySample(name.c_str(), 1.f, false);
    }

    EnableTeleportFx(true);
    mTimeDisableTeleportFx = time_ + steeringSettings.mRelocateNoColl;

    ++mNumDroppedThisRound;
    mTrainingLastDroppedTime = time_;

    UpdateMarkersReached(time_, true);

#if defined(NEURAL_AI)
    if ( mAiTrainingRecord )
    {
        if ( !mTrainingDroppedFromTrack )
        {
            mTrainingTimeFirstDropped = time_;
        }
        ++mTrainingDroppedFromTrack;
        mLastTrainingTimeIndex = mTrackMarkerReached;
        //fprintf(stderr, "dropped, new index: %d", mLastTrainingTimeIndex);
    }
#endif
}

void Player::StopAllSoundSamples()
{
    StopSample(mSoundRefEngine);
    StopSample(mSoundRefEngine2);
    StopSample(mSoundRefBrake);
    StopSample(mSoundRefAir);
    StopSample(mSoundRefHovering);
    StopSample(mSoundRefCollision);
    StopSample(mSoundRefHitGround);
}

void Player::FadeoutAllSoundSamples()
{
    if ( !APP.GetSound() )
        return;

    unsigned int fadeout_time = 2000;
    APP.GetSound()->FadeoutSample(mSoundRefEngine, fadeout_time);
    APP.GetSound()->FadeoutSample(mSoundRefEngine2, fadeout_time);
    APP.GetSound()->FadeoutSample(mSoundRefBrake, fadeout_time);
    APP.GetSound()->FadeoutSample(mSoundRefAir, fadeout_time);
    APP.GetSound()->FadeoutSample(mSoundRefHovering, fadeout_time);
    APP.GetSound()->FadeoutSample(mSoundRefCollision, fadeout_time);
    APP.GetSound()->FadeoutSample(mSoundRefHitGround, fadeout_time);
}

void Player::InfoGamePaused()
{
    DisableAllEffects();
    StopAllSoundSamples();
}

void Player::InfoGameResumePause()
{
    EnableTeleportFx(true);
    if ( mEnableFxSounds && mPlayerType == PT_LOCAL )
    {
        PlaySample(mSoundRefHovering, "hc_hovering01.wav", 1.f, true);
    }
}

void Player::InfoGameResumes()
{
    EnableTeleportFx(false);
    EnablePermanentEffects();
}

void Player::InfoGameFinished()
{
    DisableAllEffects();
    FadeoutAllSoundSamples();
}

void Player::Recording(IArchive * ar_)
{
    const Controller * controller = GetActiveController();
    if ( !controller )
        return;

    PlayerStreamData data;

    PhysicsObject* hoverPhysics = APP.GetPhysics()->GetPhysicsObject(mPhysicsId);
    if ( hoverPhysics )
    {
        data.mPos = hoverPhysics->GetCurrentStepCollCenter();
        data.mPos -= hoverPhysics->GetCollisionCenterToModelPos();
    }
    else
    {
        mMeshHover->updateAbsolutePosition();
        data.mPos = mMeshHover->getPosition();
    }

    data.mRot = mMeshHover->getRotation();
    data.mControllerPower = controller->GetPower();
    data.mControllerRotation = controller->GetRotation();
    data.StreamTo(ar_);
}

void Player::Playing(IArchive * ar_)
{
    PlayerStreamData data;
    data.StreamFrom(ar_);

    mMeshHover->setPosition(data.mPos);
    mMeshHover->setRotation(data.mRot);
    mMeshHover->updateAbsolutePosition();
}

// interpolation using physics ticks
void Player::PlayingInterpolated(Record & record_, f32 time_)
{
    const std::vector<RecordTimeEntry>& timeTable = record_.GetTimeTable();
    std::vector<RecordTimeEntry>::const_iterator itTimeStart = std::lower_bound( timeTable.begin(), timeTable.end()-1, (s32)time_);
    std::vector<RecordTimeEntry>::const_iterator itTimeEnd = std::lower_bound( timeTable.begin(), timeTable.end()-1, (s32)time_+1);
    if ( itTimeEnd == timeTable.end() )
    {
        itTimeEnd = itTimeStart;
    }

    MemArchive * archive = record_.GetStreamArchive();

    if (    itTimeStart < timeTable.end()
        &&  itTimeEnd < timeTable.end()
    )
    {
        PlayerStreamData data1;
        PlayerStreamData data2;
        core::vector3df pos;
        core::vector3df rot;

        archive->SetReadPos(itTimeStart->mStreamPos);
        data1.StreamFrom(archive);
        archive->SetReadPos(itTimeEnd->mStreamPos);
        data2.StreamFrom(archive);

        float timePart = time_ - ((s32)time_);

        pos = data1.mPos + (data2.mPos-data1.mPos) * timePart;

        mMeshHover->setPosition(pos);
        mMeshHover->setRotation(data1.mRot);
        mMeshHover->updateAbsolutePosition();
    }
}

void Player::PlayingInterpolated(Record & record_, u32 time_)
{
    return;
    const std::vector<RecordTimeEntry>& timeTable = record_.GetTimeTable();
    std::vector<RecordTimeEntry>::const_iterator itTimeStart = std::lower_bound( timeTable.begin(), timeTable.end()-1, time_);
    if ( itTimeStart != timeTable.end() && itTimeStart != timeTable.begin() && itTimeStart->mTime > time_)
        --itTimeStart;
    std::vector<RecordTimeEntry>::const_iterator itTimeEnd = std::upper_bound( timeTable.begin(), timeTable.end()-1, time_);
    MemArchive * archive = record_.GetStreamArchive();

    if (    itTimeStart < timeTable.end()
        &&  itTimeEnd < timeTable.end()
    )
    {
        PlayerStreamData data1;
        core::vector3df pos;
        core::vector3df rot;
        float timeEntry = itTimeEnd->mTime - itTimeStart->mTime;

        archive->SetReadPos(itTimeStart->mStreamPos);
        data1.StreamFrom(archive);

        if ( timeEntry <= 0.f )
        {
            //fprintffprintf(stderr, "***time:%d start:%d end:%d entry:%.2f\n", time_, itTimeStart->mTime, itTimeEnd->mTime, timeEntry);
            mMeshHover->setPosition(data1.mPos);
            mMeshHover->setRotation(data1.mRot);
            return;
        }

        PlayerStreamData data2;

        float timePart = (float)(time_-itTimeStart->mTime) / timeEntry;

        //fprintf(stderr, "time:%d start:%d end:%d entry:%.2f part:%.2f\n", time_, itTimeStart->mTime, itTimeEnd->mTime, timeEntry, timePart);

        archive->SetReadPos(itTimeEnd->mStreamPos);
        data2.StreamFrom(archive);

        pos = data1.mPos + (data2.mPos-data1.mPos) * timePart;

        mMeshHover->setPosition(pos);
        mMeshHover->setRotation(data1.mRot);
        mMeshHover->updateAbsolutePosition();
    }
    else
    {
        //fprintf(stderr, "no entry\n");
    }
}


int Player::GetTimeIndexForPos(int lastIndex_, Record * record_, const core::vector3df& pos_, bool searchAll_)
{
    const int SEARCH_TIME = 5000;

    if ( !record_ )
        return -1;

    MemArchive * archive = record_->GetStreamArchive();
    if ( !archive )
        return -1;

    const std::vector<RecordTimeEntry>& timeTable = record_->GetTimeTable();
    int bestIndex = lastIndex_;
    core::vector3df pos;

    // search the closest point
    if ( searchAll_ )
    {
        bestIndex = 0;
        archive->SetReadPos(timeTable[0].mStreamPos);
        archive->Read(pos.X);
        archive->Read(pos.Y);
        archive->Read(pos.Z);
        float bestDist = (pos_-pos).getLengthSQ();
        for ( unsigned int i=1; i < timeTable.size(); ++i )
        {
            archive->SetReadPos(timeTable[i].mStreamPos);
            archive->Read(pos.X);
            archive->Read(pos.Y);
            archive->Read(pos.Z);
            float dist = (pos_-pos).getLengthSQ();
            if ( dist < bestDist )
            {
                bestDist = dist;
                bestIndex = i;
            }
        }
        return bestIndex;
    }

    if ( lastIndex_ >= (int)timeTable.size() )
    {
//        fprintf(stderr, "lastIndex:%d, size:%d\n", lastIndex_, record_->mTimeTable.size());
        return -1;
    }

    if ( lastIndex_ < 0 )
        lastIndex_ = 0;

    archive->SetReadPos(timeTable[lastIndex_].mStreamPos);
    archive->Read(pos.X);
    archive->Read(pos.Y);
    archive->Read(pos.Z);

    float bestDist = (pos_-pos).getLengthSQ();
//    fprintf(stderr, "bestDist1: %f\n", bestDist);
    int oldTime = (int)timeTable[lastIndex_].mTime;

    int i = lastIndex_-1;
    // search backwards
    while ( i >= 0 && (oldTime-(int)timeTable[i].mTime) < SEARCH_TIME )
    {
        archive->SetReadPos(timeTable[i].mStreamPos);
        archive->Read(pos.X);
        archive->Read(pos.Y);
        archive->Read(pos.Z);
        float dist = (pos_-pos).getLengthSQ();
//        fprintf(stderr, "dist1: %f\n", dist);
        if ( dist < bestDist )
        {
            bestDist = dist;
            bestIndex = i;
        }
        --i;
    }

    // search forward
    bool wrap = false;
    float wrapTime = oldTime;
    i = lastIndex_+1;
    while ( 1 )
    {
        if ( i >= (int)timeTable.size() )
        {
            wrap = true;
            i = 0;
        }
        if ( wrap )
        {
            if ( ((int)timeTable[i].mTime+wrapTime - oldTime) >= SEARCH_TIME )
                break;
        }
        else
        {
            if ( ((int)timeTable[i].mTime - oldTime) >= SEARCH_TIME )
                break;
            wrapTime = timeTable[i].mTime;
        }

        archive->SetReadPos(timeTable[i].mStreamPos);
        archive->Read(pos.X);
        archive->Read(pos.Y);
        archive->Read(pos.Z);
        float dist = (pos_-pos).getLengthSQ();
//        fprintf(stderr, "dist2: %f\n", dist);
        if ( dist < bestDist )
        {
            bestDist = dist;
            bestIndex = i;
        }
        ++i;
    }
//    fprintf(stderr, "bestDist2: %f\n", bestDist);

    return bestIndex;
}

#if defined(NEURAL_AI)
void Player::SetAiTrainingRecord(Record * record_)
{
    if ( mAiTrainingRecord )
        mAiTrainingRecord->drop();
    mAiTrainingRecord = record_;
    if ( mAiTrainingRecord )
    {
        mAiTrainingRecord->grab();
    }
    mTrackMarkerReached = 0;
    mTimeMarkerChanged = 0;
}
#endif

bool Player::PlaySample(int & soundRef_, const std::string & sampleName_, float gain_, bool repeat_)
{
#ifdef HC1_ENABLE_SOUND
    if (    mGame.GetMode() != GM_RUNNING
        &&  mGame.GetMode() != GM_START)
        return false;

    if ( APP.GetSound() && mEnableFxSounds && (soundRef_ < 0 || !APP.GetSound()->IsPlaying(soundRef_)) )
    {
        std::string name(APP.GetConfig()->MakeFilenameSound(sampleName_));
        soundRef_ = APP.GetSound()->PlaySample(name.c_str(), gain_, repeat_);
        return true;
    }
#endif
    return false;
}

void Player::StopSample(int & soundRef_)
{
#ifdef HC1_ENABLE_SOUND
    if ( APP.GetSound() && soundRef_ >= 0 && APP.GetSound()->IsPlaying(soundRef_) )
    {
        APP.GetSound()->StopSample(soundRef_);
        soundRef_ = -1;
    }
#endif
}
