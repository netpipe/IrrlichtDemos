// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "camera.h"
#include "device_keyboard.h"
#include "device_mouse.h"
#include "input_device_manager.h"
#include "level.h"
#include "gui.h"
#include "tinyxml/tinyxml.h"
#include "main.h"
#include "config.h"
#include "helper_irr.h"
#include "helper_str.h"
#include "physics.h"
#include "globals.h"

using namespace irr;

CameraSettings::CameraSettings()
{
    mCamBehind = -400.f;;
    mCamHeight = 300.f;
    mCamHeightUpwards = 5.f;
    mCamHeightDownwards = 5.f;
    mCamTargetHeight = 1500.f;
    mCamTrailing = 7.f;
    mCamSpeedFactorFast = 0.05f;
    mCamSpeedFactorSlow = 0.05f;
    mIsFirstPerson = false;
    mFovSpeed = 1500.f;
    mFovDefault = 2.5;
    mFovFactor = -1.f;
    mPitchCam = 1.f;
    mPitchTarget = 0.f;
    mSmoothSpeedY = 1000.f;
    mSmoothMaxY = 30.f;
    mCamRoll = 0.2f;
    mTimeToAirCam = 100;

    mIntroTimeReducedMs = 1000;
    mIntroCamX = 0.f;
    mIntroCamSqrX = 0.00002f;
    mIntroCamY = 0.f;
    mIntroCamSqrY = 0.00002f;
    mIntroCamZ = 0.3f;
    mIntroCamSqrZ = 0.f;

    mOutroTimeMaxMs = 3000;
    mOutroCamX = 0.1f;
    mOutroCamSqrX = 0.f;
    mOutroCamY = 0.1;
    mOutroCamSqrY = 0.f;
    mOutroCamZ = 0.1f;
    mOutroCamSqrZ = 0.f;
}

void CameraSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const char * val = settings_->Attribute("cam_name");
    if ( val )
    {
        std::string strText(val);
        mName = ExtStr::FromUtf8(strText);
    }

    int iVal = 0;
    settings_->QueryIntAttribute("cam_first_person", &iVal );
    mIsFirstPerson = iVal != 0 ? true : false;

    settings_->QueryFloatAttribute("cam_behind", &mCamBehind);
    settings_->QueryFloatAttribute("cam_height", &mCamHeight);
    settings_->QueryFloatAttribute("cam_height_upwards", &mCamHeightUpwards);
    settings_->QueryFloatAttribute("cam_height_downwards", &mCamHeightDownwards);
    settings_->QueryFloatAttribute("target_angle", &mCamTargetHeight);
    settings_->QueryFloatAttribute("cam_trailing", &mCamTrailing);
    settings_->QueryFloatAttribute("cam_speed_factor_slow", &mCamSpeedFactorSlow);
    settings_->QueryFloatAttribute("cam_speed_factor_fast", &mCamSpeedFactorFast);
    settings_->QueryFloatAttribute("cam_fov_speed", &mFovSpeed);
    settings_->QueryFloatAttribute("cam_fov_default", &mFovDefault);
    settings_->QueryFloatAttribute("cam_fov_factor", &mFovFactor);
    settings_->QueryFloatAttribute("pitch_cam", &mPitchCam);
    settings_->QueryFloatAttribute("pitch_target", &mPitchTarget);
    settings_->QueryFloatAttribute("smooth_speed_y", &mSmoothSpeedY);
    settings_->QueryFloatAttribute("smooth_max_y", &mSmoothMaxY);
    settings_->QueryFloatAttribute("cam_roll", &mCamRoll);
    settings_->QueryIntAttribute("time_to_air", &mTimeToAirCam);

    settings_->QueryIntAttribute("intro_time_reduced", &mIntroTimeReducedMs);
    settings_->QueryFloatAttribute("intro_cam_x", &mIntroCamX);
    settings_->QueryFloatAttribute("intro_cam_sqr_x", &mIntroCamSqrX);
    settings_->QueryFloatAttribute("intro_cam_y", &mIntroCamY);
    settings_->QueryFloatAttribute("intro_cam_sqr_y", &mIntroCamSqrY);
    settings_->QueryFloatAttribute("intro_cam_z", &mIntroCamZ);
    settings_->QueryFloatAttribute("intro_cam_sqr_z", &mIntroCamSqrZ);

    settings_->QueryIntAttribute("outro_time_max", &mOutroTimeMaxMs);
    settings_->QueryFloatAttribute("outro_cam_x", &mOutroCamX);
    settings_->QueryFloatAttribute("outro_cam_sqr_x", &mOutroCamSqrX);
    settings_->QueryFloatAttribute("outro_cam_y", &mOutroCamY);
    settings_->QueryFloatAttribute("outro_cam_sqr_y", &mOutroCamSqrY);
    settings_->QueryFloatAttribute("outro_cam_z", &mOutroCamZ);
    settings_->QueryFloatAttribute("outro_cam_sqr_z", &mOutroCamSqrZ);
}

void CameraSettings::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    settings_->SetAttribute("cam_name", ExtStr::ToUtf8(mName).c_str() );
    settings_->SetAttribute("cam_first_person", mIsFirstPerson ? 1 : 0 );
    settings_->SetDoubleAttribute("cam_behind", mCamBehind);
    settings_->SetDoubleAttribute("cam_height", mCamHeight);
    settings_->SetDoubleAttribute("cam_height_upwards", mCamHeightUpwards);
    settings_->SetDoubleAttribute("cam_height_downwards", mCamHeightDownwards);
    settings_->SetDoubleAttribute("target_angle", mCamTargetHeight);
    settings_->SetDoubleAttribute("cam_trailing", mCamTrailing);
    settings_->SetDoubleAttribute("cam_speed_factor_slow", mCamSpeedFactorSlow);
    settings_->SetDoubleAttribute("cam_speed_factor_fast", mCamSpeedFactorFast);
    settings_->SetDoubleAttribute("cam_fov_speed", mFovSpeed);
    settings_->SetDoubleAttribute("cam_fov_default", mFovDefault);
    settings_->SetDoubleAttribute("cam_fov_factor", mFovFactor);
    settings_->SetDoubleAttribute("pitch_cam", mPitchCam);
    settings_->SetDoubleAttribute("pitch_target", mPitchTarget);
    settings_->SetDoubleAttribute("smooth_speed_y", mSmoothSpeedY);
    settings_->SetDoubleAttribute("smooth_max_y", mSmoothMaxY);
    settings_->SetDoubleAttribute("cam_roll", mCamRoll);
    settings_->SetAttribute("time_to_air", mTimeToAirCam);

    settings_->SetAttribute("intro_time_reduced", mIntroTimeReducedMs);
    settings_->SetDoubleAttribute("intro_cam_x", mIntroCamX);
    settings_->SetDoubleAttribute("intro_cam_sqr_x", mIntroCamSqrX);
    settings_->SetDoubleAttribute("intro_cam_y", mIntroCamY);
    settings_->SetDoubleAttribute("intro_cam_sqr_y", mIntroCamSqrY);
    settings_->SetDoubleAttribute("intro_cam_z", mIntroCamZ);
    settings_->SetDoubleAttribute("intro_cam_sqr_z", mIntroCamSqrZ);

    settings_->SetAttribute("outro_time_max", mOutroTimeMaxMs);
    settings_->SetDoubleAttribute("outro_cam_x", mOutroCamX);
    settings_->SetDoubleAttribute("outro_cam_sqr_x", mOutroCamSqrX);
    settings_->SetDoubleAttribute("outro_cam_y", mOutroCamY);
    settings_->SetDoubleAttribute("outro_cam_sqr_y", mOutroCamSqrY);
    settings_->SetDoubleAttribute("outro_cam_z", mOutroCamZ);
    settings_->SetDoubleAttribute("outro_cam_sqr_z", mOutroCamSqrZ);
}

// --------------------------------------------------------
Camera::Camera()
{
    mCameraAdditionalHeight = 0.f;
    mQuatRotation.set(0,0,0,1);
    mLastParentY = 0.f;
    mTimeRest = 0.f;
}

Camera::~Camera()
{
}

void Camera::ReloadSettings(const TiXmlElement * settings_)
{
    mCameraSettings.ReadFromXml(settings_);
    mCameraAdditionalHeight = 0.f;
}

void Camera::ResetGameCam(const core::matrix4 &target_, scene::ICameraSceneNode* gameCam_, PhysicsObject * physicsObject_)
{
    mCameraAdditionalHeight = 0.f;
    UpdateGameCam( 100.f, target_, gameCam_, physicsObject_, 0);
}

void Camera::UpdateGameIntroCam( float time_, const core::matrix4 &target_, scene::ICameraSceneNode* gameCam_, PhysicsObject * physicsObject_, u32 timeLeft_ )
{
    core::matrix4 transform( target_ );
    float restTime = timeLeft_;
    restTime -= mCameraSettings.mIntroTimeReducedMs;
    if ( restTime < 0.f )
        restTime = 0.f;
    float restTimeSqr = restTime * restTime;

    core::vector3df trans( restTimeSqr*mCameraSettings.mIntroCamSqrX + restTime*mCameraSettings.mIntroCamX
                            , restTimeSqr*mCameraSettings.mIntroCamSqrY + restTime*mCameraSettings.mIntroCamY
                            , -(restTimeSqr*mCameraSettings.mIntroCamSqrZ + restTime*mCameraSettings.mIntroCamZ) );
    transform.rotateVect( trans );
    transform.setTranslation( transform.getTranslation() + trans );
    UpdateGameCam(time_, transform, gameCam_, physicsObject_, 0);
}

void Camera::UpdateGameOutroCam( float time_, const core::matrix4 &target_, scene::ICameraSceneNode* gameCam_, PhysicsObject * physicsObject_, u32 timeLeft_ )
{
    core::matrix4 transform( target_ );
    float restTime = timeLeft_;
    if ( restTime > mCameraSettings.mOutroTimeMaxMs )
        restTime = (float)mCameraSettings.mOutroTimeMaxMs;
    float restTimeSqr = restTime * restTime;

    core::vector3df trans(  restTimeSqr*mCameraSettings.mOutroCamSqrX + restTime*mCameraSettings.mOutroCamX
                            , restTimeSqr*mCameraSettings.mOutroCamSqrY + restTime*mCameraSettings.mOutroCamY
                            , (restTimeSqr*mCameraSettings.mOutroCamSqrZ + restTime*mCameraSettings.mOutroCamZ) );
    transform.rotateVect( trans );
    transform.setTranslation( transform.getTranslation() + trans );
    UpdateGameCam(time_, transform, gameCam_, physicsObject_, 0);
}

void Camera::UpdateGameCam(float timeTick_, const core::matrix4 &target_, scene::ICameraSceneNode* gameCam_, PhysicsObject * physicsObject_, u32 timeInAir_)
{
    assert(gameCam_);

    float maxSpeed = 2000.f;
    float speed = 0.f;

    // it depends on the speed how much the camera is behind the target object
    float behind = mCameraSettings.mCamBehind;
    if ( physicsObject_ )
    {
        maxSpeed = physicsObject_->GetSettings().mMaxSpeed;
        float halfSpeed = maxSpeed  * 0.5f;
        speed = physicsObject_->GetSpeedAbsolute();
        if ( speed < halfSpeed )
        {
            behind -= (speed-halfSpeed) * mCameraSettings.mCamSpeedFactorSlow;
        }
        else
        {
            behind -= (speed-halfSpeed) * mCameraSettings.mCamSpeedFactorFast;
        }
    }

    // camera goes up when object in air and down when touching geometry
    if ( (s32)timeInAir_ < mCameraSettings.mTimeToAirCam )
    {
        if ( mCameraAdditionalHeight > mCameraSettings.mCamHeightDownwards  )
            mCameraAdditionalHeight -= mCameraSettings.mCamHeightDownwards ;
    }
    else if ( physicsObject_ )
    {
        mCameraAdditionalHeight += mCameraSettings.mCamHeightUpwards;
    }

    // Get the X/Z coordinates of the direction in which the cam would look in infinite time (final direction)
    // That's the direction in which the object does point.
    core::matrix4 camTransform;
    core::matrix4 transformAbs(target_);
    ExtIrr::AlignToUpVector(camTransform, transformAbs, core::vector3df(0,1,0), 1.f);

# if (IRRLICHT_VERSION_MAJOR <=1 && IRRLICHT_VERSION_MINOR <= 7)
    core::quaternion quatTargetRot(camTransform.getTransposed());
#else
	core::quaternion quatTargetRot(camTransform);
#endif

    // Make quaternions and direction vectors for the old and the target direction of the camera
    core::quaternion quatNewRot;
    core::vector3df oldRotVec(0,0,1);
    oldRotVec = mQuatRotation * oldRotVec;
    core::vector3df newRotVec(0,0,1);
    newRotVec = quatTargetRot * newRotVec;

//    // find the angle between those directions
//    float dot = oldRotVec.dotProduct(newRotVec);    // careful, rounding errors will get this beyond 1
//    dot = MIN(dot, 1.f);
//    dot = MAX(dot, -1.f);
//    float angle = fabs(acos(dot));

    // Find out how fast the camera should rotate towards the new direction
    // It's still not perfect, but better than anything else i found so far.
    float slerpFactor = 1 - pow(1,timeTick_) * pow((1-mCameraSettings.mCamTrailing), timeTick_);

    if ( slerpFactor > 1.f )
        slerpFactor = 1.f;

//    // TEST
//    static int dbgTest = 0;
//    if ( (++dbgTest) % 50 == 0 )
//        printf("cam: slerp %f angle %f timeTick_%f \n", slerpFactor, angle, timeTick_);

    // make a quaternion for that rotation (which can work on an "unrotated" vector)
    quatNewRot.slerp(mQuatRotation, quatTargetRot, slerpFactor);
    mQuatRotation = quatNewRot;
    mQuatRotation.normalize();

    core::vector3df camPosParent(target_.getTranslation());

    // smooth Y
    if ( timeTick_ > 0.5 )
        mLastParentY = 0.f;
    if ( mLastParentY != 0.f )
    {
        float lowBorder = mCameraSettings.mSmoothSpeedY*timeTick_;
        float maxBorder = mCameraSettings.mSmoothMaxY;
        float y = camPosParent.Y;
        if ( camPosParent.Y-lowBorder > mLastParentY )
        {
            y = mLastParentY+lowBorder;
            if ( camPosParent.Y-y > maxBorder )
            {
                y = camPosParent.Y-maxBorder;
            }
        }
        if ( camPosParent.Y+lowBorder < mLastParentY )
        {
            y = mLastParentY-lowBorder;
            if ( y-camPosParent.Y > maxBorder )
            {
                y = camPosParent.Y+maxBorder;
            }
        }
        camPosParent.Y = y;
    }
    mLastParentY = camPosParent.Y;

    core::vector3df camPosRel(0.f, mCameraSettings.mCamHeight, behind);
    camPosRel = quatNewRot * camPosRel;

    // keep pitch of target
    core::vector3df parentPitch(0,0,1);
    transformAbs.rotateVect(parentPitch);
    parentPitch.Y *= behind;
    parentPitch.set(0, parentPitch.Y, 0);

    core::vector3df flyHeight(0, mCameraAdditionalHeight, 0);
    gameCam_->setPosition(camPosParent + camPosRel + parentPitch*mCameraSettings.mPitchCam + flyHeight);

    float addHeight = sin(core::DEGTORAD * mCameraSettings.mCamTargetHeight)*behind;  // make sure it's always the same angle
    core::vector3df camTarget(0.f, mCameraSettings.mCamHeight + addHeight, 0.f);
    camTarget = quatNewRot * camTarget;
    gameCam_->setTarget (camPosParent + camTarget + parentPitch*mCameraSettings.mPitchTarget);

    // change fov on high speeds
    if ( speed > mCameraSettings.mFovSpeed )
    {
        float fovIncrease = (speed-mCameraSettings.mFovSpeed)/maxSpeed;
        fovIncrease *= mCameraSettings.mFovFactor;
        gameCam_->setFOV( M_PI/ (mCameraSettings.mFovDefault + fovIncrease) );
    }
    else
    {
        gameCam_->setFOV( M_PI / mCameraSettings.mFovDefault );
    }

    // set rolling
    core::vector3df camRealUp(0,1,0);
    core::vector3df camUp(0,1,0);
    transformAbs.rotateVect(camUp);
    camUp = mCameraSettings.mCamRoll*camUp + (1-mCameraSettings.mCamRoll)*camRealUp;
    gameCam_->setUpVector(camUp);

    gameCam_->updateAbsolutePosition();

    // TODO: unclean addition done in Masterphase. We just update another node here (for particles)
    // Clean solution would be to add this somehow to camera.. i think.
    if ( APP.GetLevelManager()->GetNodeCamera() )
    {
        APP.GetLevelManager()->GetNodeCamera()->setPosition( gameCam_->getPosition() );
    }
}

// works basically like the fps-cam. But the fps-cam doesn't work sometimes...
void Camera::UpdateEditCam(float time_, scene::ICameraSceneNode* editCam_, bool mouseRotation_)
{
    const DeviceKeyboard * keyboard = APP.GetInputDeviceManager()->GetKeyboard();
    assert(keyboard);
    const DeviceMouse * mouse = APP.GetInputDeviceManager()->GetMouse();
    assert(mouse);

    bool noVerticalMove = keyboard->IsButtonDown(KEY_LCONTROL) || keyboard->IsButtonDown(KEY_RCONTROL) || keyboard->IsButtonDown(KEY_CONTROL);
    bool fast = keyboard->IsButtonDown(KEY_LSHIFT) || keyboard->IsButtonDown(KEY_RSHIFT) || keyboard->IsButtonDown(KEY_SHIFT);

    scene::ICameraSceneNode* camera = editCam_;
    assert(camera);

    core::vector3df lookAt(camera->getTarget() - camera->getPosition() );
    lookAt.normalize();
    core::vector3df up(camera->getUpVector());
    core::vector3df strafe(lookAt);
	strafe = strafe.crossProduct(up);
    core::vector3df pos = camera->getPosition();
    core::vector3df move(lookAt);
    if ( noVerticalMove )
    {
        move.Y = 0.f;
        strafe.Y = 0.f;
    }
	strafe.normalize();
    move.normalize();

    if ( mouseRotation_ )
    {
        float mouseSpeed = 20.f;
        mouseSpeed *= time_;
        float mouseX = -mouse->GetAxisValue(0) * mouseSpeed;
        float mouseY = mouse->GetAxisValue(1) * mouseSpeed;

        core::quaternion quatRotX;
        quatRotX.fromAngleAxis (mouseX, up);
        lookAt = quatRotX * lookAt;

        core::vector3df gimbalLockSafe(lookAt);

        core::quaternion quatRotY;
        quatRotY.fromAngleAxis (mouseY, strafe);
        lookAt = quatRotY * lookAt;

        if ( fabs( lookAt.dotProduct(core::vector3df(0,1,0)) ) > 0.99f)
            lookAt = gimbalLockSafe;
    }

    float speed = fast ? 2000.f : 500.f;
    speed *= time_;

    if ( keyboard->IsButtonDown(KEY_UP) )
    {
        pos += move * speed;
    }
    if ( keyboard->IsButtonDown(KEY_DOWN) )
    {
        pos -= move * speed;
    }

    if ( keyboard->IsButtonDown(KEY_RIGHT) )
    {
        pos -= strafe * speed;
    }
    if ( keyboard->IsButtonDown(KEY_LEFT) )
    {
        pos += strafe * speed;
    }

    // mouse wheel does change the height of the cam
    if ( !APP.GetGui()->IsHovered() )
    {
        float wheelSpeed = fast ? 4500.f : 1500.f;
        wheelSpeed *= time_;
        if ( mouse->GetAxisValue(2) < 0 )
        {
            pos.Y += wheelSpeed;
        }
        else if ( mouse->GetAxisValue(2) > 0 )
        {
            pos.Y -= wheelSpeed;
        }
    }

    camera->setPosition( pos );
    camera->updateAbsolutePosition();
    camera->setTarget(camera->getPosition() + lookAt );
}
