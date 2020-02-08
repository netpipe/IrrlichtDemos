// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef SOUND_SETTINGS_H
#define SOUND_SETTINGS_H

class TiXmlElement;

struct SoundSettings
{
    SoundSettings();
    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    float   mSfxVolume;   // [0-1]
    float   mMusicVolume; // [0-1]

    float   mRolloffFactor;  // i guess 0-1
    float   mReferenceDist;  // maybe in units
    float   mMaxDist;
	int 	mDistanceModel;	 // check openal docu for ALenum
};

#endif // SOUND_SETTINGS_H
