// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef OGG_STREAM_H
#define OGG_STREAM_H

#include "compile_config.h"

#ifdef HC1_ENABLE_OPENAL
#ifdef HC1_ENABLE_OGG

#include "sound_settings.h"
#include "ov_callbacks.h"
#include <AL/al.h>

#include <irrlicht.h>

#include <string>

#define OGG_BUFFERS 3
class OggStream
{
public:
    OggStream();
    ~OggStream();

    bool Open(std::string fileName_, irr::io::IFileSystem * fs=NULL);
    void Release();
    void Display();         // display some info on the Ogg
    bool Playback();        // start playing the Ogg stream
    bool IsPlaying();         // check if the source is playing
    bool Update(const SoundSettings &settings_);          // update the stream if necessary

protected:
    bool Stream(ALuint buffer);   // reloads a buffer
    void Empty();                 // empties the queue
    bool Check();                 // checks OpenAL error state
    std::string ErrorString(int code); // stringify an error code

private:
    bool            mStreamValid;
    OggVorbis_File  mOggStream;     // stream handle
    vorbis_info*    mVorbisInfo;    // some formatting data
    vorbis_comment* mVorbisComment; // user comments

    ALuint          mBuffers[OGG_BUFFERS];    // front and back buffers
    ALuint          mSource;        // audio source
    ALenum          mFormat;        // internal format
};

#endif // HC1_ENABLE_OGG
#endif // HC1_ENABLE_OPENAL

#endif // OGG_STREAM_H
