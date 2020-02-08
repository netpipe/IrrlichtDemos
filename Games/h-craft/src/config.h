// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef CONFIG_H
#define CONFIG_H

#include "config_enums.h"

// Do not include whole Irrlicht here because otherwise every file depends on it.
// And this would cause a lot of warnings because Irrlicht and -Wall did not work together well
#include <EDriverTypes.h>

#include <string>

class TiXmlDocument;
class TiXmlElement;
class StringTable;
struct GhostRecordSettings;
namespace irr
{
	namespace io
	{
		class IFileSystem;
	}
};

class Config
{
public:
    Config(void * systemData);
    ~Config();

    void Init(const char *argv0_);

    //! filesystem only needed on Android
    bool Load(irr::io::IFileSystem * fs_=0);
    bool Save();
    bool UpdateCache(); // update all internally cached values after xml-data was changed

    // CAREFUL: Graphic settings are cached in variables for speed
    // Therefore they are not automatically updated when the xml-data is changed
    irr::video::E_DRIVER_TYPE GetDriverType() const { return mDriverType; }
    float GetNearClipping() const					{ return mNearClipping; }
    float GetFarClipping() const                    { return mFarClipping; }
    bool GetFullscreen() const                      { return mFullscreen; }
    bool GetUseScreenResolution() const             { return mUseScreenResolution; }
    int GetResolutionX() const                      { return mResolutionX; }
    int GetResolutionY() const                      { return mResoutionY; }
    int GetBitDepth() const                         { return mBitDepth; }
    int GetMinTimePerFrameMs() const                { return mMinTimePerFrameMs; }
    int GetVSync() const                            { return mVSync; }
    int GetFrequency() const                        { return mFrequency; }
    int GetTextureDownscaleDivider() const			{ return mTextureDownscaleDivider; }
    bool GetBiLinear() const                        { return mBiLinear; }
    bool GetTriLinear() const                       { return mTriLinear; }
    bool GetAnisotropic() const                     { return mAnisotropic; }
    bool GetGouraud() const                         { return mGouraud; }
    bool GetAntiAlias() const                       { return mAntiAlias; }
	ELightMode GetLightMode() const					{ return mLightMode; }
    EParticleMode GetParticleMode() const           { return mParticleMode; }
    bool GetUseShadow() const                       { return mUseShadow; }
    bool GetUseMipMaps() const                      { return mUseMipMaps; }
    bool GetUseTex16Bit() const                     { return mUseTex16Bit; }
    bool GetUseTex32Bit() const                     { return mUseTex32Bit; }
    bool GetUseTexQuality() const                   { return mUseTexQuality; }
    bool GetUseTexSpeed() const                     { return mUseTexSpeed; }
    ETouchInput GetUseTouchInput() const			{ return mUseTouchInput; }

    // format is: "path/"
    std::string GetPathMedia() const;
    std::string GetPathTextures() const;
    std::string GetPathSounds() const;
    std::string GetPathMusicMenu() const;
    std::string GetPathMusicGame() const;
    std::string GetPathLevels() const;
    std::string GetPathUI() const;
    std::string GetPathModels() const;
    std::string GetPathRecordings() const;
    std::string GetPathUserData(bool checkAssetsFs=false) const;
    std::string GetPathGhosts() const;
    std::string GetPathTests() const;
    std::string GetPathTestResults() const;
    std::string GetPathShader() const;

    // filename with complete path
    std::string GetStringTableName() const  { return mStringTableName; }

    // given a complete filename with path make a path that is relative to the media dir
    // so: /home/micha/hover/media/models/abc.X would become models/abc.X
    std::string MakeRelativeMediaName(const std::string &filename_) const;

    // base is the path where the exe is located
    std::string MakeFilenameBase(const std::string &filename_) const;
    std::string MakeFilenameTexture(const std::string &file_) const;
    std::string MakeFilenameSound(const std::string &file_) const;
    std::string MakeFilenameLevel(const std::string &file_) const;
    std::string MakeFilenameUI(const std::string &file_) const;
    std::string MakeFilenameModels(const std::string &file_) const;
    std::string MakeFilenameRecordings(const std::string &file_) const;
    std::string MakeFilenameUserData(const std::string &file_, bool checkAssetsFs=false) const;
    std::string MakeFilenameGhosts(const std::string &file_) const;
    std::string MakeFilenameTests(const std::string &file_) const;
    std::string MakeFilenameTestResults(const std::string &file_) const;
    std::string MakeFilenameShader(const std::string &file_) const;

    std::wstring GetDlgMainCaption() const         { return mDlgMainCaption; }

    std::string MakeLapRecordName(int levelId_, StringTable * stringTable_) const;
    std::string MakeTrackRecordName(int levelId_, StringTable * stringTable_) const;

    bool IsGhostRecordFilename(const char* filename_) const;
    std::string GhostRecordSettingsToFilename(const GhostRecordSettings & settings_, bool includePath_=true);

    TiXmlElement * GetPhysicsSettings() const;
    TiXmlElement * GetPhysicsObjectSettings(const char * objName_) const;
    TiXmlElement * GetSteeringSettings() const;
    TiXmlElement * GetCamerasSettings() const;
    TiXmlElement * GetCameraSettings(int camIndex_) const;
    TiXmlElement * GetInputSettings() const;
    TiXmlElement * GetSoundSettings() const;
    TiXmlElement * GetHoverSettings() const;
    TiXmlElement * GetGraphicSettings() const;
    TiXmlElement * GetEtcSettings() const;
    TiXmlElement * GetBillingSettings() const;
    TiXmlElement * GetAdProviderSettings() const;
    TiXmlElement * GetBillingProviderSettings() const;

    void SetHoverRadius(float radius_)  { mHoverRadius = radius_; }
    float GetHoverRadius() const        { return mHoverRadius; }

    std::string GetBlobModelName() const    { return mBlobModel; }
    float GetBlobHeight() const             { return mBlobHeight; }
    float GetSparkleRotMin() const          { return mSparkleRotMin; }
    float GetSparkleSpeedMin() const        { return mSparkleSpeedMin; }
    bool GetDisableSound() const            { return mDisableSound; }

    // 0 = system keyboard, 1 = alternative
    int GetVirtualKeyboard() const;

    bool GetKey(std::string &key_, irr::io::IFileSystem * fs_);

    // Write the whole config to a string
    void WriteToString(std::string &str_);

	// Do we need to support tripplehead? (3 monitors)
	bool DoesNeedTrippleHeadMode(int screenWidth, int screenHeight) const;

	// On touch-devices with tiny screens (mobile phones) we need sometimes larger buttons
	bool DoesNeedLargeButtons() const { return mDoesNeedLargeButtons; }

protected:
	std::string GetConfigFilename(bool checkAssetsFs=false) const;
	std::string GetProviderFilename(bool checkAssetsFs=false) const;
	TiXmlElement * GetConfigGroup(const char* value) const;

private:
    TiXmlDocument * mXmlDocument;
    TiXmlDocument * mXmlProviderDocument;

    // graphic settings (cache data)
    irr::video::E_DRIVER_TYPE mDriverType;
    float mNearClipping;
    float mFarClipping;
    bool mFullscreen;
    bool mUseScreenResolution;
    int mResolutionX;
    int mResoutionY;
    int mBitDepth;
    int mMinTimePerFrameMs;
    int mVSync;
    int mFrequency;
    int mTextureDownscaleDivider;
    bool mBiLinear;
    bool mTriLinear;
    bool mAnisotropic;
    bool mGouraud;
    bool mAntiAlias;
    ELightMode mLightMode;
    EParticleMode mParticleMode;
    bool mUseShadow;
    bool mUseMipMaps;
    bool mUseTex16Bit;
    bool mUseTex32Bit;
    bool mUseTexQuality;
    bool mUseTexSpeed;

    // several settings which didn't really fit elsewhere (cache data)
    float           mHoverRadius;
    std::string     mBlobModel;
    float           mBlobHeight;
    float           mSparkleRotMin;
    float           mSparkleSpeedMin;
    std::string     mStringTableName;
    std::wstring    mDlgMainCaption;
    bool            mDisableSound;

    // paths
    std::string mAndroidFilesPath;
    std::string mBasePath;
    std::string mPathMedia;
    std::string mPathTextures;
    std::string mPathSounds;
    std::string mPathLevels;
    std::string mPathUI;
    std::string mPathModels;
    std::string mPathRecordings;
    std::string mPathUserData;
    std::string mPathUserDataAssets;
    std::string mPathGhosts;
    std::string mPathTests;
    std::string mPathTestResults;
    std::string mPathShader;

    // string with current date-time created once at config-start
    std::string mDateTimeStart;

	// true when we're on a small device.
    bool mDoesNeedLargeButtons;

    // fixed configuration values
    const ETouchInput mUseTouchInput;

};

#endif // CONFIG_H
