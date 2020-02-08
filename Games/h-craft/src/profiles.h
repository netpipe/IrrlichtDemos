// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef PROFILES_H
#define PROFILES_H

#include <string>
#include <vector>

#include "championship.h"
#include "controller.h"
#include "device_joystick_settings.h"
#include "device_touch_settings.h"
#include "sound_settings.h"

// TODO: there's a general problem with profiles as they are now not working
// as originally expected. Currently all the settings in config.xml are always used.
// Those are overwritten by data in PlayerProfile. Currently this means hacking in
// load/saves in several dialogs and also in the ProfileManager to get that behaviour
// and this is very error-prone. The load/save/defaultvalues mechanism should be better
// defined.

struct PlayerProfile
{
    PlayerProfile();
    void InfoGameFinished();
    void CheatChampionship(int score_);
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    std::wstring            mName;
    ChampionshipProgress    mChampionship;
    ControllerSettings      mControllerSettings;
    DeviceJoystickSettings  mJoystickSettings;
    DeviceTouchSettings     mTouchSettings;
    SoundSettings           mSoundSettings;
    int                     mHover;
    size_t                  mActiveCameraIndex;

    bool IsHoverUnlocked(int hoverId_) const;
    bool ShouldShowHoverUnlockedScreen(int hoverId_) const;
    void SetHoverUnlockedScreenShown(int hoverId_);

protected:
    bool WasHoverUnlockedShown( int hoverId_ ) const;
    void CheckUnlockHovers();

private:
    std::vector<int>        mHoversUnlocked;
    std::vector<int>        mHoversUnlockedShown;
};

class ProfileManager
{
public:
    ProfileManager();
    ~ProfileManager();

    void LoadProfiles();
    void SaveProfiles();

    void AddProfile(std::wstring name_);
    void DeleteProfile(unsigned int index_);

    unsigned int GetNumProfiles() const;
    void SelectProfile(int index_);
    int GetSelectedProfileIndex() const  { return mSelectedProfile; }
    PlayerProfile* GetSelectedProfile() const;
    PlayerProfile* GetProfile(int index_) const;

    bool IsLevelUnlocked( int levelId_ ) const;
    void CheckUnlockLevels();

protected:
    void DeleteAllProfiles();

private:
    int mSelectedProfile;
    std::vector<PlayerProfile*>  mProfiles;
    std::vector<int> mLevelIdsUnlocked;
};

#endif // PROFILES_H
