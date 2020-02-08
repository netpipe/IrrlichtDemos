// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "sound_settings.h"
#include "tinyxml/tinyxml.h"
#include <float.h>

#ifdef HC1_ENABLE_SOUND
	#include <AL/al.h>
#else
// we still don't want to lose the values for the config, so copying this from al.h
	#define AL_NONE                                   0
	#define AL_INVERSE_DISTANCE                       0xD001
	#define AL_INVERSE_DISTANCE_CLAMPED               0xD002
	#define AL_LINEAR_DISTANCE                        0xD003
	#define AL_LINEAR_DISTANCE_CLAMPED                0xD004
	#define AL_EXPONENT_DISTANCE                      0xD005
	#define AL_EXPONENT_DISTANCE_CLAMPED              0xD006
#endif

// --------------------------------------------------------
SoundSettings::SoundSettings()
{
    mSfxVolume = 1.f;
    mMusicVolume = 1.f;
    mRolloffFactor = 1.f;
    mReferenceDist = 200.f;
    mDistanceModel = AL_INVERSE_DISTANCE_CLAMPED;
    mMaxDist = FLT_MAX;
}

void SoundSettings::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;
    settings_->QueryFloatAttribute("sfx_vol", &mSfxVolume);
    settings_->QueryFloatAttribute("music_vol", &mMusicVolume);
    settings_->QueryFloatAttribute("sfx_rolloff", &mRolloffFactor);
    settings_->QueryFloatAttribute("sfx_ref_dist", &mReferenceDist);
    settings_->QueryFloatAttribute("sfx_max_dist", &mMaxDist);

    const char * model = settings_->Attribute("sfx_dist_model");
    if ( model && strlen(model) )
    {
        if ( 0 == strcmp(model, "AL_NONE") )
            mDistanceModel = AL_NONE;
        else if ( 0 == strcmp(model, "AL_INVERSE_DISTANCE") )
            mDistanceModel = AL_INVERSE_DISTANCE;
        else if ( 0 == strcmp(model, "AL_INVERSE_DISTANCE_CLAMPED") )
            mDistanceModel = AL_INVERSE_DISTANCE_CLAMPED;
        else if ( 0 == strcmp(model, "AL_LINEAR_DISTANCE") )
            mDistanceModel = AL_LINEAR_DISTANCE;
        else if ( 0 == strcmp(model, "AL_LINEAR_DISTANCE_CLAMPED") )
            mDistanceModel = AL_LINEAR_DISTANCE_CLAMPED;
        else if ( 0 == strcmp(model, "AL_EXPONENT_DISTANCE") )
            mDistanceModel = AL_EXPONENT_DISTANCE;
        else if ( 0 == strcmp(model, "AL_EXPONENT_DISTANCE_CLAMPED") )
            mDistanceModel = AL_EXPONENT_DISTANCE_CLAMPED;
    }
}

void SoundSettings::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;
    settings_->SetDoubleAttribute("sfx_vol", mSfxVolume);
    settings_->SetDoubleAttribute("music_vol", mMusicVolume);
    settings_->SetDoubleAttribute("sfx_rolloff", mRolloffFactor);
    settings_->SetDoubleAttribute("sfx_ref_dist", mReferenceDist);
    settings_->SetDoubleAttribute("sfx_max_dist", mMaxDist);

    switch ( mDistanceModel )
    {
        case AL_NONE:
            settings_->SetAttribute("sfx_dist_model", "AL_NONE");
        break;
        case AL_INVERSE_DISTANCE:
            settings_->SetAttribute("sfx_dist_model", "AL_INVERSE_DISTANCE");
        break;
        case AL_INVERSE_DISTANCE_CLAMPED:
            settings_->SetAttribute("sfx_dist_model", "AL_INVERSE_DISTANCE_CLAMPED");
        break;
        case AL_LINEAR_DISTANCE:
            settings_->SetAttribute("sfx_dist_model", "AL_LINEAR_DISTANCE");
        break;
        case AL_LINEAR_DISTANCE_CLAMPED:
            settings_->SetAttribute("sfx_dist_model", "AL_LINEAR_DISTANCE_CLAMPED");
        break;
        case AL_EXPONENT_DISTANCE:
            settings_->SetAttribute("sfx_dist_model", "AL_EXPONENT_DISTANCE");
        break;
        case AL_EXPONENT_DISTANCE_CLAMPED:
            settings_->SetAttribute("sfx_dist_model", "AL_EXPONENT_DISTANCE_CLAMPED");
        break;
        default:
            settings_->SetAttribute("sfx_dist_model", "AL_NONE");
    }
}
