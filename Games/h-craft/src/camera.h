// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef CAMERA_H
#define CAMERA_H

// TODO: this class needs some refactoring by now (since it's no longer just used for game-camera)

#include <irrlicht.h>
#include <vector>
#include <string>

class TiXmlElement;
class PhysicsObject;

struct CameraSettings
{
    CameraSettings();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    std::wstring mName;
    bool        mIsFirstPerson;
    float       mCamBehind;
    float       mCamHeight;
    float       mCamHeightUpwards;
    float       mCamHeightDownwards;
    float       mCamTargetHeight;
    float       mCamTrailing;
    float       mCamSpeedFactorSlow;
    float       mCamSpeedFactorFast;
    float       mFovSpeed;
    float       mFovDefault;
    float       mFovFactor;
    float       mPitchCam;
    float       mPitchTarget;
    float       mSmoothSpeedY;
    float       mSmoothMaxY;
    float       mCamRoll;
    int         mTimeToAirCam;  // time in ms until the camera switches to behaviour for hover-in-air

    int         mIntroTimeReducedMs;  // intro cam stops moving that many ms before end of countdown
    float       mIntroCamX;
    float       mIntroCamSqrX;
    float       mIntroCamY;
    float       mIntroCamSqrY;
    float       mIntroCamZ;
    float       mIntroCamSqrZ;

    int         mOutroTimeMaxMs;
    float       mOutroCamX;
    float       mOutroCamSqrX;
    float       mOutroCamY;
    float       mOutroCamSqrY;
    float       mOutroCamZ;
    float       mOutroCamSqrZ;
};

class Camera
{
public:
    Camera();
    ~Camera();

    void ReloadSettings(const TiXmlElement * settings_);
    const CameraSettings& GetSettings() const  { return mCameraSettings; }

    void UpdateGameCam(float time_, const irr::core::matrix4 &target_, irr::scene::ICameraSceneNode* gameCam_, PhysicsObject * physicsObject_, irr::u32 timeInAir_);
    void UpdateGameIntroCam( float time_, const irr::core::matrix4 &target_, irr::scene::ICameraSceneNode* gameCam_, PhysicsObject * physicsObject_, irr::u32 timeLeft_ );
    void UpdateGameOutroCam( float time_, const irr::core::matrix4 &target_, irr::scene::ICameraSceneNode* gameCam_, PhysicsObject * physicsObject_, irr::u32 timeLeft_ );
    static void UpdateEditCam(float time_, irr::scene::ICameraSceneNode* editCam_, bool mouseRotation_=true);
    void ResetGameCam(const irr::core::matrix4 &target_, irr::scene::ICameraSceneNode* gameCam_, PhysicsObject * physicsObject_);


private:
    CameraSettings      	mCameraSettings;
    float               	mCameraAdditionalHeight;
    irr::core::quaternion 	mQuatRotation;
    float               	mLastParentY;

    float mTimeRest;
};

#endif // CAMERA_H
