// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "config.h"
#include "compile_config.h"
#include "game.h"
#include "helper_file.h"
#include "helper_str.h"
#include "globals.h"
#include "string_table.h"
#include "logging.h"
#include "tinyxml/tinyxml.h"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>

#ifdef __GNUWIN32__
#include <direct.h>
#include <time.h>
#endif // __GNUWIN32__

#ifdef _IRR_ANDROID_PLATFORM_
#include "android_tools.h"
#include <sys/stat.h>
#endif

using namespace irr;

// --------------------------------------------------------
Config::Config(void * systemData)
: mXmlDocument(NULL)
, mXmlProviderDocument(NULL)
, mDoesNeedLargeButtons(false)
#ifdef _IRR_ANDROID_PLATFORM_
, mUseTouchInput(ETI_TOUCH_HARDWARE)
#elif defined(HOVER_RELEASE)
, mUseTouchInput(ETI_NO_TOUCH)
#else
//, mUseTouchInput(ETI_TOUCH_SIMULATION)
, mUseTouchInput(ETI_NO_TOUCH)
#endif
{
    mDriverType = irr::video::EDT_OPENGL;
    mNearClipping = 10.f;
    mFarClipping = 20000.f;
    mFullscreen = false;
    mUseScreenResolution = false;
    mResolutionX = 640;
    mResoutionY = 480;
    mBitDepth = 16;
    mMinTimePerFrameMs = 10;
    mVSync = 1;
    mFrequency = 0;
    mTextureDownscaleDivider = 1;
    mBiLinear = true;
    mTriLinear = false;
    mAnisotropic = true;
    mGouraud = true;
    mAntiAlias = false;
    mLightMode = ELM_DYNAMIC;
    mParticleMode = EPM_ON;
    mUseShadow = true;
    mUseMipMaps = true;
    mUseTex16Bit = false;
    mUseTex32Bit = false;
    mUseTexQuality = false;
    mUseTexSpeed = false;

    mHoverRadius = 100.f;
    mBlobHeight = -50.f;

    mSparkleRotMin = 0.7f;
    mSparkleSpeedMin = 0.9f;
#ifdef HC1_ENABLE_SOUND
    mDisableSound = false;
#else
	mDisableSound = true;
#endif

	time_t timeNow = time(0);
	tm * timeNowTm = localtime(&timeNow);
	if ( timeNowTm )
	{
		std::ostringstream streamTime;
		streamTime << 1900+timeNowTm->tm_year << "_" << timeNowTm->tm_mon+1 << "_" << timeNowTm->tm_mday;
		streamTime << "_" << timeNowTm->tm_hour << "_" << timeNowTm->tm_min << "_" << timeNowTm->tm_sec;
		mDateTimeStart = streamTime.str();
	}

#ifdef _IRR_ANDROID_PLATFORM_
    mBasePath       = "";	// assets filesystem has no real path
    mPathMedia      = "";	// assets filesystem has no real path

    // we can write into the path of the android application itself
    mAndroidFilesPath = hc1::android::getFilesDir((android_app*)systemData);
    if ( !mAndroidFilesPath.empty() && mAndroidFilesPath[mAndroidFilesPath.size()-1] != '/' ) // no c++ 11 with back() yet :-(
		mAndroidFilesPath += '/';
    mPathUserData   = mAndroidFilesPath + "userdata/";
    mPathUserDataAssets = mPathMedia + "userdata/";
    mPathGhosts     = mAndroidFilesPath + "ghosts/";	// (if we ever support that on Android....)

	mkdir(mPathUserData.c_str(), 0770);
	mkdir(mPathGhosts.c_str(), 0770);

	// Check if we have a small touch-display where we can use larger buttons
	const float MIN_SIZE_CM = 9.f;
	float cmX = 0.f;
	float cmY = 0.f;
	hc1::android::SDisplayMetrics metrics;
	if ( hc1::android::getDisplayMetrics((android_app*)systemData, metrics) )
	{
		if ( metrics.xdpi > 0.f )
		{
			cmX = (2.54f * (float)metrics.widthPixels) / metrics.xdpi;
			LOG.LogLn(LP_DEBUG, "Display width (cm): ", cmX);
		}
		if ( metrics.ydpi > 0.f )
		{
			cmY = (2.54f * (float)metrics.heightPixels) / metrics.ydpi;
			LOG.LogLn(LP_DEBUG, "Display height (cm): ", cmY);
		}
	}
	if ( cmX < MIN_SIZE_CM || cmY < MIN_SIZE_CM )
	{
		mDoesNeedLargeButtons = true;
	}
#else
	mBasePath       = "./";
	mPathMedia      = "media/";
    mPathUserData   = mPathMedia + "userdata/";
    mPathGhosts     = "ghosts/";
#endif
    mPathTextures   = mPathMedia + "textures/";
    mPathSounds     = mPathMedia + "sounds/";
    mPathLevels     = mPathMedia + "levels/";
    mPathUI         = mPathMedia + "ui/";
    mPathModels     = mPathMedia + "models/";
    mPathRecordings = mPathMedia + "recordings/";
    mPathShader     = "shaders/";

    mPathTests 		= "tests/";
	mPathTestResults = "tests/"; // TODO: would prefer a sub-directory, but have no os-independent mkdir yet.
}

Config::~Config()
{
	delete mXmlProviderDocument;
    delete mXmlDocument;
}

void Config::Init(const char *argv0_)
{
#ifndef _IRR_ANDROID_PLATFORM_
    char cwd[MAXPATHLEN];
    getcwd(cwd, MAXPATHLEN);
    if ( argv0_ )
    {
        std::string strCwd(cwd);
        std::string strArgv0(argv0_);
        if ( strCwd[strCwd.length()-1] != '/'
            &&strCwd[strCwd.length()-1] != '\\')
            strCwd += '/';
        ExtFile::EraseFilenameFromPath(strArgv0);
        LOG.DebugLn("strArgv0", strArgv0.c_str());
        LOG.DebugLn("strCwd", strCwd.c_str());
        ExtFile::MakeLinuxTypePath(strCwd);
        ExtFile::MakeLinuxTypePath(strArgv0);
        LOG.DebugLn("strArgv0", strArgv0.c_str());
        mBasePath = ExtFile::FindExePath(strCwd, strArgv0.c_str());
        LOG.InfoLn("BasePath: ", mBasePath.c_str());
    }
#endif
}

// given a complete filename with path make a path that is relative to the media dir
// so: /home/micha/hover/media/models/abc.X would become models/abc.X
std::string Config::MakeRelativeMediaName(const std::string &filename_) const
{
    std::string result(filename_);

    // replace all '\' by '/'
    for ( unsigned int i=0; i<result.size(); ++i )
    {
        if ( result[i] == '\\' )
            result[i] = '/';
    }

    std::string::size_type p = result.rfind(mPathMedia);
    if ( p == std::string::npos )
    {
        return result;
    }
    return result.substr(p + mPathMedia.size(), result.size() - (p + mPathMedia.size()));
}

std::string Config::GetPathMedia() const
{
    return mBasePath+mPathMedia;
}

std::string Config::GetPathTextures() const
{
    return mBasePath+mPathTextures;
}

std::string Config::GetPathSounds() const
{
    return mBasePath+mPathSounds;
}

std::string Config::GetPathMusicMenu() const
{
	return GetPathSounds()+"music_menu";
}

std::string Config::GetPathMusicGame() const
{
	return GetPathSounds()+"music_level";
}


std::string Config::GetPathLevels() const
{
    return mBasePath+mPathLevels;
}

std::string Config::GetPathUI() const
{
    return mBasePath+mPathUI;
}

std::string Config::GetPathRecordings() const
{
    return mBasePath+mPathRecordings;
}

std::string Config::GetPathModels() const
{
    return mBasePath+mPathModels;
}

std::string Config::GetPathUserData(bool checkAssetsFs) const
{
	if ( checkAssetsFs )
	{
		return mBasePath+mPathUserDataAssets;
	}
	else
	{
		return mBasePath+mPathUserData;
	}
}

std::string Config::GetPathGhosts() const
{
    return mBasePath+mPathGhosts;
}

std::string Config::GetPathTests() const
{
	return mBasePath+mPathTests;
}

std::string Config::GetPathTestResults() const
{
	return mBasePath+mPathTestResults;
}

std::string Config::GetPathShader() const
{
	return mBasePath+mPathShader;
}

std::string Config::MakeFilenameBase(const std::string &filename_) const
{
	return mBasePath + filename_;
}

std::string Config::MakeFilenameTexture(const std::string &file_) const
{
    return GetPathTextures() + file_;
}

std::string Config::MakeFilenameSound(const std::string &file_) const
{
    return GetPathSounds() + file_;
}

std::string Config::MakeFilenameLevel(const std::string &file_) const
{
    return GetPathLevels() + file_;
}

std::string Config::MakeFilenameUI(const std::string &file_) const
{
    return GetPathUI() + file_;
}

std::string Config::MakeFilenameModels(const std::string &file_) const
{
    return GetPathModels() + file_;
}

std::string Config::MakeFilenameRecordings(const std::string &file_) const
{
    return GetPathRecordings() + file_;
}

std::string Config::MakeFilenameUserData(const std::string &file_, bool checkAssetsFs) const
{
    return GetPathUserData(checkAssetsFs) + file_;
}

std::string Config::MakeFilenameGhosts(const std::string &file_) const
{
    return GetPathGhosts() + file_;
}

std::string Config::MakeFilenameTests(const std::string &file_) const
{
	return GetPathTests() + file_;
}

std::string Config::MakeFilenameTestResults(const std::string &file_) const
{
	return GetPathTestResults() + mDateTimeStart + std::string("_") + file_;
}

std::string Config::MakeFilenameShader(const std::string &file_) const
{
	return GetPathShader() + file_;
}

std::string Config::GetConfigFilename(bool checkAssetsFs) const
{
#ifdef _IRR_ANDROID_PLATFORM_
	if ( checkAssetsFs )
	{
		return GetPathMedia() + "config.xml";
	}
	else
	{
		return MakeFilenameUserData("config.xml");
	}
#else
	// TODO: should probably move it somewhere else also on other platforms.
	// But have to be careful to not break downward compatiblity for old releases.
	// Also have to check first which _is_ the best place to put that stuff on different
	// operating systems.
    return GetPathMedia() + "config.xml";
#endif
}

std::string Config::GetProviderFilename(bool checkAssetsFs) const
{
#ifdef _IRR_ANDROID_PLATFORM_
	if ( checkAssetsFs )
	{
		return GetPathMedia() + "provider.xml";
	}
	else
	{
		return MakeFilenameUserData("provider.xml");
	}
#else
    return "";
#endif
}

TiXmlElement * Config::GetConfigGroup(const char* value) const
{
    if ( mXmlDocument )
	{
		TiXmlNode * configNode = mXmlDocument->FirstChild("config");
		if (configNode)
		{
			TiXmlNode * groupNode = configNode->FirstChild(value);
			if (groupNode)
				return groupNode->ToElement();
		}
	}

    return NULL;
}

bool Config::Load(irr::io::IFileSystem * fs_)
{
    delete mXmlDocument;
    delete mXmlProviderDocument;

	std::string configFileName(GetConfigFilename());

    mXmlDocument = new TiXmlDocument(configFileName.c_str());
	if ( !mXmlDocument )
		return false;

	std::string providerFileName(GetProviderFilename());
	mXmlProviderDocument = new TiXmlDocument(configFileName.c_str());

    mXmlDocument->SetIrrFs(fs_, TiXmlDocument::E_ON_READ_FAIL_ANDROID);
    if (mXmlProviderDocument)
		mXmlProviderDocument->SetIrrFs(fs_, TiXmlDocument::E_ON_READ_FAIL_ANDROID);

	if ( !mXmlDocument->LoadFile() )
	{
#ifdef _IRR_ANDROID_PLATFORM_
		// get defaults from assets filesystem
		configFileName = GetConfigFilename(true);
		if ( !mXmlDocument->LoadFile(configFileName.c_str()) )
		{
#endif
			LOG.DebugLn("TiXmlDocument::ErrorDesc: ", mXmlDocument->ErrorDesc());
			delete mXmlDocument;
			mXmlDocument = 0;

			return false;
#ifdef _IRR_ANDROID_PLATFORM_
		}
#endif
	}

	if (mXmlProviderDocument)
	{
		if ( !mXmlProviderDocument->LoadFile() )
		{
#ifdef _IRR_ANDROID_PLATFORM_
			// get defaults from assets filesystem
			providerFileName = GetProviderFilename(true);
			if ( !mXmlProviderDocument->LoadFile(providerFileName.c_str()) )
			{
				LOG.Warn(L"Did not find the provider.xml");
				delete mXmlProviderDocument;
				mXmlProviderDocument = 0;
			}
#endif
		}
	}

    return UpdateCache();
}

bool Config::Save()
{
	if ( !mXmlDocument )
        return false;

	return mXmlDocument->SaveFile(GetConfigFilename().c_str());
}

void Config::WriteToString(std::string &str_)
{
    if ( !mXmlDocument )
    {
        str_ = "Config::LogDebug - no xml document loaded\n";
        return;
    }
    str_ << *(mXmlDocument->RootElement());
}

bool Config::UpdateCache()
{
    if ( !mXmlDocument )
        return false;

	TiXmlNode * configNode = mXmlDocument->FirstChild("config");
	if (!configNode)
		return false;

    TiXmlNode * node = 0;
    node = configNode->FirstChild("graphics");
    if ( node )
    {
        TiXmlElement* element = node->ToElement();

        const char *val = element->Attribute("drivertype");
        if ( val && strlen(val) )
        {
            if ( 0 == strcmp(val, "DIRECT3D9") )
                mDriverType = video::EDT_DIRECT3D9;
       //     else if ( 0 == strcmp(val, "DIRECT3D8") )
//                mDriverType = video::EDT_DIRECT3D8;
            else if ( 0 == strcmp(val, "OPENGL") )
                mDriverType = video::EDT_OPENGL;
            else if ( 0 == strcmp(val, "SOFTWARE") )
                mDriverType = video::EDT_SOFTWARE;
            else if ( 0 == strcmp(val, "EDT_BURNINGSVIDEO") )
                mDriverType = video::EDT_BURNINGSVIDEO;
            else if ( 0 == strcmp(val, "NULL") )
                mDriverType = video::EDT_NULL;
        }
        double farClipping = mFarClipping;
        if ( element->Attribute("far_clipping", &farClipping) )
            mFarClipping = (float)farClipping;
		double nearClipping = mNearClipping;
		if ( element->Attribute("near_clipping", &nearClipping) )
            mNearClipping = (float)nearClipping;
        int fullscreen = mFullscreen ? 1: 0;
        if ( element->Attribute("fullscreen", &fullscreen) )
            mFullscreen = fullscreen;
        int useScreenResolution = mUseScreenResolution ? 1 : 0;
        if ( element->Attribute("use_screen_res", &useScreenResolution) )
            mUseScreenResolution = useScreenResolution;
        int resX = mResolutionX;
        if ( element->Attribute("res_x", &resX) )
            mResolutionX = resX;
        int resY = mResoutionY;
        if ( element->Attribute("res_y", &resY) )
            mResoutionY = resY;
        int bitdepth = mBitDepth;
        if ( element->Attribute("bitdepth", &bitdepth) )
            mBitDepth = bitdepth;
        int minTime = mMinTimePerFrameMs;
        if ( element->Attribute("min_time", &minTime) )
            mMinTimePerFrameMs = minTime;

        int vsync = mVSync;
        if ( element->Attribute("vsync", &vsync) )
            mVSync = vsync;
        int freq = mFrequency;
        if ( element->Attribute("freq", &freq) )
            mFrequency = freq;
        int tex_div = mTextureDownscaleDivider;
		if ( element->Attribute("tex_div", &tex_div) )
		{
            mTextureDownscaleDivider = core::max_(tex_div, 1);
        }
        int bilinear = mBiLinear ? 1 : 0;
        if ( element->Attribute("bi", &bilinear) )
            mBiLinear = bilinear;
        int trilinear = mTriLinear ? 1 : 0;
        if ( element->Attribute("tri", &trilinear) )
            mTriLinear = trilinear;
        int anisotropic = mAnisotropic ? 1 : 0;
        if ( element->Attribute("ani", &anisotropic) )
            mAnisotropic = anisotropic;
        int gouraud = mGouraud ? 1 : 0;
        if ( element->Attribute("gouraud", &gouraud) )
            mGouraud = gouraud;
        int aa = mAntiAlias ? 1 : 0;
        if ( element->Attribute("aa", &aa) )
            mAntiAlias = aa;
		int lightMode = (int)mLightMode;
		if ( element->Attribute("light", &lightMode) )
            mLightMode = (ELightMode)lightMode;
		int particles = (int)mParticleMode;
        if ( element->Attribute("particles", &particles) )
            mParticleMode = (EParticleMode)particles;
        int shadow = mUseShadow ? 1 : 0;
        if ( element->Attribute("shadow", &shadow) )
            mUseShadow = shadow;
        int mip = mUseMipMaps ? 1 : 0;
        if ( element->Attribute("mip", &mip) )
            mUseMipMaps = mip;
        int tex16 = mUseTex16Bit ? 1 : 0;
        if ( element->Attribute("tex16", &tex16) )
            mUseTex16Bit = tex16;
        int tex32 = mUseTex32Bit ? 1 : 0;
        if ( element->Attribute("tex32", &tex32) )
            mUseTex32Bit = tex32;
        int texQuality = mUseTexQuality  ? 1 : 0;
        if ( element->Attribute("tex_qual", &texQuality) )
            mUseTexQuality = texQuality;
        int texSpeed = mUseTexSpeed ? 1 : 0;
        if ( element->Attribute("tex_speed", &texSpeed) )
            mUseTexSpeed = texSpeed;
    }

    node = configNode->FirstChild("dlgMain");
    if ( node )
    {
        TiXmlElement* element = node->ToElement();
        const char *val = element->Attribute("caption");
        if ( val )
        {
            std::string str(val);
            mDlgMainCaption = ExtStr::FromUtf8(str);
        }
    }

    node = configNode->FirstChild("hovers");
    if ( node )
    {
        TiXmlNode * nodeBlob = node->FirstChild("blob");
        if ( nodeBlob )
        {
            TiXmlElement* eleBlob = nodeBlob->ToElement();
            const char *val = eleBlob->Attribute("model");
            if ( val )
            {
                mBlobModel = val;
            }
            eleBlob->QueryFloatAttribute("height", &mBlobHeight);
        }

        TiXmlNode * nodeParticles = node->FirstChild("particles");
        if ( nodeParticles )
        {
            TiXmlElement* eleParticles = nodeParticles->ToElement();
            eleParticles->QueryFloatAttribute("sparkle_rot_min", &mSparkleRotMin);
            eleParticles->QueryFloatAttribute("sparkle_speed_min", &mSparkleSpeedMin);
        }
    }

    node = configNode->FirstChild("i18n");
    if ( node )
    {
        TiXmlElement * nodeLang = node->ToElement();
        if ( nodeLang )
        {
            const char *val = nodeLang->Attribute("strings");
            if ( val )
            {
                mStringTableName = GetPathMedia() + val;
            }
        }
    }

    node = configNode->FirstChild("etc");
    if ( node )
    {
        TiXmlElement * eleEtc = node->ToElement();
        if ( eleEtc )
        {
            int soundDisabled=0;
            eleEtc->Attribute("disable_sound", &soundDisabled);
            mDisableSound = soundDisabled;
        }
    }

    return true;
}

TiXmlElement * Config::GetPhysicsSettings() const
{
	TiXmlElement * physicsNode = GetConfigGroup("physic");	// sorry, for "physic" instead of "physics", but won't change as config's are already out there.
	if (!physicsNode)
		return NULL;

    TiXmlNode * node = physicsNode->FirstChild("settings");
    if ( node )
    {
        return node->ToElement();
    }

    return NULL;
}

TiXmlElement * Config::GetPhysicsObjectSettings(const char * objName_) const
{
    if ( !objName_ || !strlen(objName_) )
        return NULL;

	TiXmlElement * physicsNode = GetConfigGroup("physic");	// sorry, for "physic" instead of "physics", but won't change as config's are already out there.
	if (!physicsNode)
		return NULL;

    TiXmlNode * node = physicsNode->FirstChild("physicobjects");	// sorry, for "physicobjects" instead of "physicsobjects", but won't change as config's are already out there.
    if ( node )
    {
        node = node->FirstChild(objName_);
        if ( node )
        {
            return node->ToElement();
        }
    }

    return NULL;
}

TiXmlElement * Config::GetSteeringSettings() const
{
	return GetConfigGroup("steering");
}

TiXmlElement * Config::GetCamerasSettings() const
{
	return GetConfigGroup("cameras");
}

TiXmlElement * Config::GetCameraSettings(int camIndex_) const
{
    TiXmlElement * camerasNode = GetCamerasSettings();
    if ( !camerasNode )
        return NULL;

    TiXmlNode* nodeCamera = camerasNode->IterateChildren(NULL);
    for ( int i=0; i< camIndex_; ++i )
    {
        if ( nodeCamera )
        {
            nodeCamera = camerasNode->IterateChildren(nodeCamera);
        }
    }
    return nodeCamera->ToElement();
}

TiXmlElement * Config::GetInputSettings() const
{
    return GetConfigGroup("input");
}

TiXmlElement * Config::GetSoundSettings() const
{
    return GetConfigGroup("sound");
}

TiXmlElement * Config::GetHoverSettings() const
{
    return GetConfigGroup("hovers");
}

TiXmlElement * Config::GetGraphicSettings() const
{
   return GetConfigGroup("graphics");
}

TiXmlElement * Config::GetEtcSettings() const
{
	return GetConfigGroup("etc");
}

TiXmlElement * Config::GetBillingSettings() const
{
	return GetConfigGroup("billing");
}

TiXmlElement * Config::GetAdProviderSettings() const
{
	if ( mXmlProviderDocument )
	{
		TiXmlNode * node = mXmlProviderDocument->FirstChild("provider");
		if (node)
		{
			return node->ToElement();
		}
	}

	return 0;
}

TiXmlElement * Config::GetBillingProviderSettings() const
{
	if ( mXmlProviderDocument )
	{
		TiXmlNode * node = mXmlProviderDocument->FirstChild("provider");
		if (node)
		{
			return node->ToElement();
		}
	}

	return 0;
}

int Config::GetVirtualKeyboard() const
{
	int keyboard = 0;
	const TiXmlElement * ele = GetEtcSettings();
	if ( ele )
	{
		ele->Attribute("keyboard", &keyboard);
	}
	return keyboard;
}

std::string Config::MakeLapRecordName(int levelId_, StringTable * stringTable_) const
{
    std::ostringstream filenameLapRecord;
    std::string idFile( ExtStr::ToUtf8( std::wstring(stringTable_->Get("id_filename_ghostlap").c_str()) ) );
    filenameLapRecord   << GetPathUserData()
                        << idFile
                        << levelId_
                        << ".rec";
    return filenameLapRecord.str();
}

std::string Config::MakeTrackRecordName(int levelId_, StringTable * stringTable_) const
{
    std::ostringstream filenameLapRecord;
    std::string idFile( ExtStr::ToUtf8( std::wstring(stringTable_->Get("id_filename_ghosttrack").c_str()) ) );
    filenameLapRecord   << GetPathUserData()
                        << idFile
                        << levelId_
                        << ".rec";
    return filenameLapRecord.str();
}

bool Config::IsGhostRecordFilename(const char* filename_) const
{
    if ( !filename_ || !strlen(filename_) )
        return false;

    std::string filename(filename_);

    // needs at least two '_'
    std::string::size_type p0 = 0;
    std::string::size_type p1 = filename.find("_");
    if ( p1 == std::string::npos )
        return false;
    p0 = p1+1;
    p1 = filename.find("_", p0);
    if ( p1 == std::string::npos )
        return false;

    // check for correct ending
    p0 = p1+1;
    p1 = filename.find(".rec", p0);
    if ( p1 == std::string::npos )
        return false;

    return true;
}

std::string Config::GhostRecordSettingsToFilename(const GhostRecordSettings & settings_, bool includePath_)
{
    std::ostringstream filename;

    if ( includePath_ )
    {
        filename << GetPathGhosts();
    }

    // Can't use ToUtf8 here, as resulting filenames would even be worse (not working on some systems)
    std::string strProfile( settings_.mProfileName.begin(), settings_.mProfileName.end() );
    std::string strLevel( settings_.mLevelName.begin(), settings_.mLevelName.end() );

    ExtStr::EraseSpacesAndSetUpperCharBehind(strProfile, false);
    ExtStr::EraseSpacesAndSetUpperCharBehind(strLevel, false);

    // Validation is mostly needed for international versions like cyrillic
    for ( size_t i=0; i < strProfile.length(); ++i )
    {
        strProfile[i] = ExtFile::GetValidFilenameChar(strProfile[i]);
    }
    for ( size_t i=0; i < strLevel.length(); ++i )
    {
        strLevel[i] = ExtFile::GetValidFilenameChar(strLevel[i]);
    }

    filename << strProfile << '_' << strLevel << '_';
    if ( settings_.mGhostType == GHT_EXPORTED_LAP )
    {
        filename << "lap";
    }
    else if ( settings_.mGhostType == GHT_EPORTED_TRACK )
    {
        filename << "race";
    }

    filename << ".rec";

    fprintf(stderr, "profile: %s level: %s filename: %s\n", strProfile.c_str(), strLevel.c_str(), filename.str().c_str() );

    return filename.str();
}

// search for any file in gamefolder called "*.key"
// return it's content in key_
bool Config::GetKey(std::string &key_, irr::io::IFileSystem * fs_)
{
    io::path oldDir = fs_->getWorkingDirectory();

    std::string keyDir( mBasePath );
    if ( !fs_->changeWorkingDirectoryTo(keyDir.c_str()) )
    {
        LOG.Warn("Warning: Config::GetKey changeWorkingDirectoryTo failed\n");
        return false;
    }

    bool hasKey = false;
    io::IFileList * fileList = fs_->createFileList();
    if ( fileList )
    {
        for ( u32 i = 0; i < fileList->getFileCount(); ++i )
        {
            if ( fileList->isDirectory(i) )
                continue;

            const io::path& fileName = fileList->getFileName(i);
            if ( strstr(fileName.c_str(), ".key") )
            {
                std::ifstream keyFile(fileName.c_str());
                if ( keyFile )
                {
                    hasKey = true;
                    keyFile >> key_;
                }
            }
        }
        fileList->drop();
    }

    if ( !fs_->changeWorkingDirectoryTo(oldDir) )
    {
        LOG.Warn("Warning: Config::GetKey changeWorkingDirectoryTo failed when setting oldDir\n");
    }
    return hasKey;
}

// Do we need to support tripplehead? (3 monitors)
bool Config::DoesNeedTrippleHeadMode(int screenWidth, int screenHeight) const
{
#ifdef _IRR_ANDROID_PLATFORM_
	return false;
#else
    return screenWidth / screenHeight >= 3.f;
#endif
}
