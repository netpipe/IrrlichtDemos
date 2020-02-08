// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "profiles.h"
#include "main.h"
#include "game.h"
#include "config.h"
#include "controller.h"
#include "device_joystick.h"
#include "device_touch.h"
#include "input_device_manager.h"
#include "sound.h"
#include "app_restrictions.h"
#include "helper_str.h"
#include "logging.h"
#include "compile_config.h"
#include "tinyxml/tinyxml.h"

// --------------------------------------------------------
PlayerProfile::PlayerProfile()
: mHover(0)
, mActiveCameraIndex(0)
{
    mChampionship.Reset();
    mHoversUnlocked.push_back(0);   // we need at least one open hover
    APP.GetProfileManager()->CheckUnlockLevels();
}

void PlayerProfile::CheatChampionship(int score_)
{
    if ( mName != L"carabas" )
        return;

    GameResult cheatResult;

    cheatResult.mGameWasFinished = true;
    cheatResult.mLocalPlayerTrackTimeMs = 999999;
    cheatResult.mLocalPlayerLastLapTimeMs = 999999;
    cheatResult.mLocalPlayerBestLapTimeMs = 999999;
    cheatResult.mLocalPlayerNumLaps = 9;
    cheatResult.mLocalPlayerPlacing = 3-score_;
    cheatResult.mChampionshipScore = score_;

    mChampionship.UpdateRace(APP.GetChampionship()->GetRaceSelected(), cheatResult);
    CheckUnlockHovers();
    APP.GetProfileManager()->CheckUnlockLevels();
    APP.GetProfileManager()->SaveProfiles();
}

void PlayerProfile::InfoGameFinished()
{
    if ( GT_CHAMPIONSHIP == APP.GetGame()->GetSettings().mGameType )
    {
        mChampionship.UpdateRace(APP.GetChampionship()->GetRaceSelected(), APP.GetGame()->GetResult());
        CheckUnlockHovers();
        APP.GetProfileManager()->CheckUnlockLevels();
        APP.GetProfileManager()->SaveProfiles();
    }
}

void PlayerProfile::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    const char * val = settings_->Attribute("name");
    if ( val )
    {
        std::string str(val);
        std::wstring wstr = ExtStr::FromUtf8(str);

        mName = wstr;
    }

    settings_->Attribute("hover", &mHover);
    int camIndex = 0;
    settings_->Attribute("cam_index", &camIndex);
    mActiveCameraIndex = camIndex < 0 ? 0 : (size_t)camIndex;

    const TiXmlNode * nodeCS = settings_->FirstChild("cs");
    if ( nodeCS )
    {
        mChampionship.ReadFromXml(nodeCS->ToElement());
    }

    mHoversUnlocked.clear();
    mHoversUnlockedShown.clear();
    const TiXmlNode * nodeHu = settings_->FirstChild("hu");
    if ( nodeHu )
    {
        const TiXmlNode * nodeHover = nodeHu->IterateChildren("hov", NULL);
        while ( nodeHover )
        {
            const TiXmlElement * eleHover = nodeHover->ToElement();
            int hoverId=0;
            eleHover->Attribute("id", &hoverId);
            mHoversUnlocked.push_back(hoverId);

            int shown=0;
            eleHover->Attribute("shown", &shown);
            if ( shown )
            {
                mHoversUnlockedShown.push_back(hoverId);
            }

            nodeHover = nodeHu->IterateChildren("hov", nodeHover);
        }
    }

    bool hasController = false;
    bool hasJoystick = false;
    bool hasTouch = false;
    const TiXmlNode * nodeInput = settings_->FirstChild("input");
    if ( nodeInput )
    {
        const TiXmlNode* nodeController = nodeInput->FirstChild("controller");
        if ( nodeController )
        {
            hasController = true;
            mControllerSettings.ReadFromXml(nodeController->ToElement());
        }

        const TiXmlNode* nodeJoystick = nodeInput->FirstChild("js");
        if ( nodeJoystick )
        {
            hasJoystick = true;
            mJoystickSettings.ReadFromXml(nodeJoystick->ToElement());
        }

        const TiXmlNode* nodeTouch = nodeInput->FirstChild("touch");
        if ( nodeTouch )
        {
            hasTouch = true;
            mTouchSettings.ReadFromXml(nodeTouch->ToElement());
        }
    }
    if ( !hasController && APP.GetController() )
    {
        mControllerSettings = APP.GetController()->GetUserSettings();
    }
    if ( !hasJoystick && APP.GetInputDeviceManager() && APP.GetInputDeviceManager()->GetJoystick(0) )
    {
        mJoystickSettings = APP.GetInputDeviceManager()->GetJoystick(0)->mSettings;
    }
    if ( !hasTouch && APP.GetInputDeviceManager() )
    {
		DeviceTouch * deviceTouch = static_cast<DeviceTouch *>(APP.GetInputDeviceManager()->GetDevice(IDP_TOUCH));
		if ( deviceTouch )
		{
			mTouchSettings = deviceTouch->GetSettings();
		}
    }

    const TiXmlNode * nodeSound = settings_->FirstChild("sound");
    if ( nodeSound)
    {
        mSoundSettings.ReadFromXml(nodeSound->ToElement());
    }
    else if ( APP.GetSound() )
    {
        mSoundSettings = APP.GetSound()->GetSettings();
    }
}

void PlayerProfile::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    std::string nameStr( ExtStr::ToUtf8(mName) );
    settings_->SetAttribute("name", nameStr.c_str() );
    settings_->SetAttribute("hover", mHover);
    settings_->SetAttribute("cam_index", (int)mActiveCameraIndex);

    TiXmlElement eleCS("cs");
    mChampionship.WriteToXml(&eleCS);
    settings_->InsertEndChild(eleCS);

    TiXmlElement eleHU("hu");
    for ( unsigned int i=0; i < mHoversUnlocked.size(); ++i )
    {
        TiXmlElement eleHover("hov");
        eleHover.SetAttribute("id", mHoversUnlocked[i] );

        int shown = WasHoverUnlockedShown(mHoversUnlocked[i])  ? 1:0;
        eleHover.SetAttribute("shown", shown);

        eleHU.InsertEndChild(eleHover);
    }
    settings_->InsertEndChild(eleHU);

    TiXmlElement eleInput("input");

    TiXmlElement eleController("controller");
    mControllerSettings.WriteToXml(&eleController);
    eleInput.InsertEndChild(eleController);

    TiXmlElement eleJoystick("js");
    mJoystickSettings.WriteToXml(&eleJoystick);
    eleInput.InsertEndChild(eleJoystick);

    TiXmlElement eleTouch("touch");
    mTouchSettings.WriteToXml(&eleTouch);
    eleInput.InsertEndChild(eleTouch);

    settings_->InsertEndChild(eleInput);

    TiXmlElement eleSound("sound");
    mSoundSettings.WriteToXml(&eleSound);
    settings_->InsertEndChild(eleSound);
}

bool PlayerProfile::IsHoverUnlocked(int hoverId_) const
{
    if ( IS_DEMO_VERSION )
    {
        if ( hoverId_ != 0 )
            return false;
    }
    for ( unsigned int i=0; i < mHoversUnlocked.size(); ++i )
    {
        if ( mHoversUnlocked[i] == hoverId_ )
        {
            return true;
        }
    }
    return false;
}

bool PlayerProfile::ShouldShowHoverUnlockedScreen(int hoverId_) const
{
    if ( IsHoverUnlocked(hoverId_) && ! WasHoverUnlockedShown(hoverId_) )
        return true;
    return false;
}

void PlayerProfile::SetHoverUnlockedScreenShown(int hoverId_)
{
    mHoversUnlockedShown.push_back(hoverId_);
}

bool PlayerProfile::WasHoverUnlockedShown( int hoverId_ ) const
{
    for ( unsigned int i=0; i < mHoversUnlockedShown.size(); ++i )
    {
        if ( hoverId_ == mHoversUnlockedShown[i] )
        {
            return true;
        }
    }
    return false;
}

void PlayerProfile::CheckUnlockHovers()
{
    for ( unsigned int h=0; h < APP.GetNumHovers(); ++h )
    {
        if ( !IsHoverUnlocked(h) )
        {
            const Hover& hover = APP.GetHover(h);
            if ( mChampionship.GetTotalScore() >= hover.mPointsNeeded )
            {
                mHoversUnlocked.push_back(h);
            }
        }
    }
}


// --------------------------------------------------------
ProfileManager::ProfileManager()
{
    mSelectedProfile = -1;
}

ProfileManager::~ProfileManager()
{
    DeleteAllProfiles();
}

void ProfileManager::DeleteAllProfiles()
{
    for ( unsigned int i=0; i < mProfiles.size(); ++i )
        delete mProfiles[i];
    mProfiles.clear();
}

void ProfileManager::LoadProfiles()
{
    std::string xmlName(APP.GetConfig()->MakeFilenameUserData(std::string("profiles.xml")));
    TiXmlDocument xmlDoc(xmlName.c_str());
    xmlDoc.SetIrrFs(APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem(), TiXmlDocument::E_ON_READ_FAIL_ANDROID);

    if ( !xmlDoc.LoadFile() )
    {
#ifdef _IRR_ANDROID_PLATFORM_
		// We sometimes need profiles with everything played clear for testing, so we try the default profiles.xml from PC as well first.
		std::string xmlName(APP.GetConfig()->MakeFilenameUserData(std::string("profiles.xml"), true));
		if ( !xmlDoc.LoadFile(xmlName.c_str()) )
		{
#endif

        LOG.WarnLn("ProfileManager::LoadProfiles: could not load xml ", xmlName.c_str());
        LOG.DebugLn("TiXmlDocument::ErrorDesc: ", xmlDoc.ErrorDesc());
        return;
#ifdef _IRR_ANDROID_PLATFORM_
		}
#endif
    }

    TiXmlNode * nodeProfiles = xmlDoc.FirstChild("profiles");
	if (!nodeProfiles)
		return;

    DeleteAllProfiles();

    TiXmlNode* nodePlayerProfile = nodeProfiles->IterateChildren("player", NULL);
    while ( nodePlayerProfile )
    {
        const TiXmlElement * nodeElement = nodePlayerProfile->ToElement();
        assert(nodeElement);

        PlayerProfile * playerProfile = new PlayerProfile();
        playerProfile->ReadFromXml(nodeElement);
        mProfiles.push_back(playerProfile);
        mSelectedProfile = mProfiles.size()-1;

        nodePlayerProfile = nodeProfiles->IterateChildren("player", nodePlayerProfile);
    }

    TiXmlElement *eleProfiles=nodeProfiles->ToElement();
	if ( eleProfiles )
	{
	    int selected = 0;
	    eleProfiles->QueryIntAttribute("selected", &selected);
	    SelectProfile(selected);
	}

    mLevelIdsUnlocked.clear();
	TiXmlNode* nodeLevelsUnlocked = nodeProfiles->IterateChildren("liu", NULL);
    while ( nodeLevelsUnlocked )
    {
        TiXmlElement *eleLevelsUnlocked=nodeLevelsUnlocked->ToElement();
        int id = 0;
	    eleLevelsUnlocked->QueryIntAttribute("id", &id);
	    mLevelIdsUnlocked.push_back(id);

        nodeLevelsUnlocked = nodeProfiles->IterateChildren("liu", nodeLevelsUnlocked);
    }
}

void ProfileManager::SaveProfiles()
{
    std::string xmlName( APP.GetConfig()->MakeFilenameUserData(std::string("profiles.xml")) );
    TiXmlDocument xmlDoc(xmlName.c_str());
    xmlDoc.SetIrrFs(APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem(), TiXmlDocument::E_ON_READ_FAIL_ANDROID);

    if ( !xmlDoc.LoadFile() )
    {
        TiXmlDeclaration xmlDec("1.0", "", "yes");
        xmlDoc.InsertEndChild( xmlDec );
    }

    TiXmlNode * nodeProfiles = xmlDoc.FirstChild("profiles");
	if (!nodeProfiles)
	{
	    nodeProfiles = xmlDoc.InsertEndChild( TiXmlElement("profiles") );
	}

    nodeProfiles->Clear();
	TiXmlElement *eleProfiles=nodeProfiles->ToElement();
	if ( eleProfiles )
	{
        eleProfiles->SetAttribute("selected", mSelectedProfile );
	}

    for ( unsigned int i=0; i < mProfiles.size(); ++i )
    {
        TiXmlElement ele("player");
        mProfiles[i]->WriteToXml(&ele);
        nodeProfiles->InsertEndChild(ele);
    }

    for ( unsigned int i=0; i < mLevelIdsUnlocked.size(); ++i )
    {
        TiXmlElement ele("liu");
        ele.SetAttribute("id", mLevelIdsUnlocked[i] );
        nodeProfiles->InsertEndChild(ele);
    }

    xmlDoc.SaveFile();
}

bool ProfileManager::IsLevelUnlocked( int levelId_ ) const
{
    for ( unsigned int i=0; i < mLevelIdsUnlocked.size(); ++i )
    {
        if ( mLevelIdsUnlocked[i] == levelId_ )
        {
            return true;
        }
    }
    return false;
}

void ProfileManager::CheckUnlockLevels()
{
    PlayerProfile* profile = GetProfile(mSelectedProfile);
    Championship* cs = APP.GetChampionship();
    if ( !profile || !cs )
        return;

    for ( unsigned int r=0; r < profile->mChampionship.GetNumRaceResults(); ++r )
    {
        const CS_RaceResult& raceResult = profile->mChampionship.GetRaceResult(r);

        if ( raceResult.mIsOpen )
        {
            const CS_Race& race = cs->GetRace(r);

            if ( !IsLevelUnlocked(race.mLevelId) )
            {
                mLevelIdsUnlocked.push_back ( race.mLevelId );
            }
        }
    }
}

void ProfileManager::AddProfile(std::wstring name_)
{
    PlayerProfile * profile = new PlayerProfile;

    profile->mName = name_;
    profile->mChampionship.Reset();

    if ( APP.GetController() )
        profile->mControllerSettings = APP.GetController()->GetUserSettings();

    if ( APP.GetInputDeviceManager() && APP.GetInputDeviceManager()->GetJoystick(0) )
        profile->mJoystickSettings = APP.GetInputDeviceManager()->GetJoystick(0)->mSettings;

	if ( APP.GetInputDeviceManager() )
	{
		DeviceTouch * deviceTouch = static_cast<DeviceTouch *>(APP.GetInputDeviceManager()->GetDevice(IDP_TOUCH));
		if ( deviceTouch )
			deviceTouch->SetSettings(profile->mTouchSettings);
	}

    if ( APP.GetSound() )
        profile->mSoundSettings = APP.GetSound()->GetSettings();

    mProfiles.push_back(profile);
    mSelectedProfile = mProfiles.size()-1;
}

void ProfileManager::DeleteProfile(unsigned int index_)
{
    if ( index_ >= mProfiles.size() )
        return;

    delete mProfiles[index_];
    mProfiles.erase( mProfiles.begin() + index_ );
    mSelectedProfile = mProfiles.size()-1;
}

void ProfileManager::SelectProfile(int index_)
{
    mSelectedProfile = index_;

    PlayerProfile* profile = GetProfile(mSelectedProfile);
    if ( !profile )
        return;

    Controller * controller = APP.GetController();
    if ( controller )
    {
        controller->SetUserSettings(profile->mControllerSettings);
        controller->WriteToXml(APP.GetConfig()->GetInputSettings());
    }

    if ( APP.GetInputDeviceManager() )
    {
		IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(0);
		if ( joystick )
		{
			joystick->mSettings = profile->mJoystickSettings;
		}
		DeviceTouch * deviceTouch = static_cast<DeviceTouch *>(APP.GetInputDeviceManager()->GetDevice(IDP_TOUCH));
		if ( deviceTouch )
		{
			deviceTouch->SetSettings(profile->mTouchSettings);
		}
		APP.GetInputDeviceManager()->WriteToXml(APP.GetConfig()->GetInputSettings());
	}

    if ( APP.GetSound() )
    {
        APP.GetSound()->GetSettings() = profile->mSoundSettings;
        APP.GetSound()->GetSettings().WriteToXml(APP.GetConfig()->GetSoundSettings());
    }

    APP.GetGame()->SetActiveCameraIndex(profile->mActiveCameraIndex);
}

unsigned int ProfileManager::GetNumProfiles() const
{
    return mProfiles.size();
}

PlayerProfile* ProfileManager::GetProfile(int index_) const
{
    if ( index_ < 0 || index_ >= (int)mProfiles.size() )
        return 0;

    return mProfiles[index_];
}

PlayerProfile* ProfileManager::GetSelectedProfile() const
{
	return GetProfile(mSelectedProfile);
}
