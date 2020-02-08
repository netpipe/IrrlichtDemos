// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "ogg_stream.h"

#ifdef HC1_ENABLE_OPENAL
#ifdef HC1_ENABLE_OGG

#include "logging.h"
#include "sound_openal.h"
#include <iostream>

#define SOUND_STREAM_BUF_SIZE (4096*4)

// --------------------------------------------------------
OggStream::OggStream()
{
    mVorbisInfo = 0;
    mVorbisComment = 0;
    mStreamValid = false;
    mSource = 0;
}

OggStream::~OggStream()
{
}

bool OggStream::Open(std::string fileName_, irr::io::IFileSystem * fs)
{
    int result;
	LOG.LogLn(LP_DEBUG, fileName_.c_str() );
    mStreamValid = false;

	if ( fs )
	{
		irr::io::IReadFile* file = fs->createAndOpenFile(irr::io::path(fileName_.c_str()));
		if (!file )
		{
			LOG.LogLn(LP_WARN, "OggStream::Open - createAndOpenFile failed for: ", fileName_.c_str());
			return false;
		}

		long length = (long)file->getSize();
		if ( length == 0 )
		{
			file->drop();
			LOG.LogLn(LP_WARN, "OggStream::Open - filesize is 0 for: ", fileName_.c_str());
			return false;
		}

		if((result = ov_open_callbacks(file, &mOggStream, 0, 0, ov_callback_irr_fs)) < 0)
		{
			file->drop();

			LOG.LogLn(LP_WARN, "Could not open Ogg stream. ", ErrorString(result).c_str() );

			return false;
		}
	}
	else
	{
		FILE*  fileOgg;       // file handle

		if(!(fileOgg = fopen(fileName_.c_str(), "rb")))
		{
			LOG.LogLn(LP_WARN, "Could not open Ogg file.");
			return false;
		}

		if((result = ov_open_callbacks(fileOgg, &mOggStream, 0, 0, OV_CALLBACKS_DEFAULT)) < 0)
		{
			fclose(fileOgg);

			LOG.LogLn(LP_WARN, "Could not open Ogg stream. ", ErrorString(result).c_str() );

			return false;
		}
	}

	mVorbisInfo = ov_info(&mOggStream, -1);
	mVorbisComment = ov_comment(&mOggStream, -1);

	if(mVorbisInfo->channels == 1)
		mFormat = AL_FORMAT_MONO16;
	else
		mFormat = AL_FORMAT_STEREO16;

	alGenBuffers(OGG_BUFFERS, mBuffers);
	if ( !SoundOpenAL::CheckOpenAlError("OggStream::Open alGenBuffers") )
		return false;
	alGenSources(1, &mSource);
	if ( !SoundOpenAL::CheckOpenAlError("OggStream::Open alGenSources") )
		return false;

    alSource3f(mSource, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(mSource, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(mSource, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (mSource, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (mSource, AL_SOURCE_RELATIVE, AL_TRUE      );
    alSourcef (mSource, AL_GAIN, 1.f );

    mStreamValid = true;

    return true;
}

void OggStream::Release()
{
    if ( mStreamValid )
    {
        alSourceStop(mSource);
        SoundOpenAL::CheckOpenAlError("OggStream::Release - alSourceStop");

        Empty();

        alDeleteSources(1, &mSource);
        SoundOpenAL::CheckOpenAlError("OggStream::Release alDeleteSources");

        alDeleteBuffers(OGG_BUFFERS, mBuffers);
        SoundOpenAL::CheckOpenAlError("OggStream::Release alDeleteBuffers");

        ov_clear(&mOggStream);
        mStreamValid = false;
    }
}

void OggStream::Empty()
{
    if ( !mStreamValid )
        return;

    int queued;
    alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
    SoundOpenAL::CheckOpenAlError("OggStream::Empty - AL_BUFFERS_QUEUED");

//    char numQueued[16];
//    sprintf( numQueued, ": %d", queued);
//    LOG.InfoLn("OggStream::Empty", numQueued );

    while(queued > 0)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(mSource, 1, &buffer);
        SoundOpenAL::CheckOpenAlError("OggStream::Empty alSourceUnqueueBuffers");

        --queued;
    }

//    LOG.InfoLn("OggStream::Empty", " - finished" );
}

void OggStream::Display()
{
    if ( !mStreamValid )
        return;
    std::cout
        << "version         " << mVorbisInfo->version         << "\n"
        << "channels        " << mVorbisInfo->channels        << "\n"
        << "rate (hz)       " << mVorbisInfo->rate            << "\n"
        << "bitrate upper   " << mVorbisInfo->bitrate_upper   << "\n"
        << "bitrate nominal " << mVorbisInfo->bitrate_nominal << "\n"
        << "bitrate lower   " << mVorbisInfo->bitrate_lower   << "\n"
        << "bitrate window  " << mVorbisInfo->bitrate_window  << "\n"
        << "\n"
        << "vendor " << mVorbisComment->vendor << "\n";

    for(int i = 0; i < mVorbisComment->comments; i++)
        std::cout << "   " << mVorbisComment->user_comments[i] << "\n";

    std::cout << std::endl;
}

bool OggStream::Playback()
{
    if ( !mStreamValid )
        return false;

//    LOG.InfoLn("OggStream::Playback", NULL);

    if(IsPlaying())
        return true;

    int queue = 0;
    for ( int i=0; i < OGG_BUFFERS; ++i )
    {
        if ( !Stream(mBuffers[i]))
        {
            break;
        }
        ++queue;
    }
    if( !queue )
        return false;

    LOG.LogLn(LP_DEBUG, "alSourceQueueBuffers:", queue);
    alSourceQueueBuffers(mSource, queue, mBuffers);
    SoundOpenAL::CheckOpenAlError("OggStream::Playback alSourceQueueBuffers");

    LOG.LogLn(LP_DEBUG, "alSourcePlay:", (int)mSource);
    alSourcePlay(mSource);
    SoundOpenAL::CheckOpenAlError("OggStream::Playback alSourcePlay");

    return true;
}

bool OggStream::IsPlaying()
{
    if ( !mStreamValid )
        return false;
    ALenum state;

    alGetSourcei(mSource, AL_SOURCE_STATE, &state);

    return (state == AL_PLAYING);
}

bool OggStream::Update(const SoundSettings &settings_)
{
    if ( !mStreamValid )
        return false;

    int processed;
    bool active = true;

    alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
//    if( processed > OGG_BUFFERS )
//    {
//        LOG.Info("processed > OGG_BUFFERS\n");
//    }

    while(processed > 0)
    {
        --processed;

        ALuint buffer;

        // unque buffers which were processed
        alSourceUnqueueBuffers(mSource, 1, &buffer);
        SoundOpenAL::CheckOpenAlError("OggStream::Update alSourceUnqueueBuffers");

        // immediatly fill that buffer again
        active = Stream(buffer);
        if ( active )
        {
            // ... and push it back into queue
            alSourceQueueBuffers(mSource, 1, &buffer);
            SoundOpenAL::CheckOpenAlError("OggStream::Update alSourceQueueBuffers");
        }
    }

    // Check for underrun
    if (active)
    {
        alSourcef (mSource, AL_GAIN, settings_.mMusicVolume );

        // We have data, so we should be playing...
        int state;
        alGetSourcei(mSource, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING)
        {
            alSourcePlay(mSource);
        }
    }
    else
    {
        // No more data. Seek to beginning and stop
        ov_raw_seek(&mOggStream, 0);
        //if (!looping)
        Release();
        active = false;
    }

    return active;
}

bool OggStream::Stream(ALuint buffer)
{
    int  size = 0;
    int  section = 0;
    int  result = 0;
    char data[SOUND_STREAM_BUF_SIZE];

    while(size < SOUND_STREAM_BUF_SIZE)
    {
#ifdef _IRR_ANDROID_PLATFORM_	// tremor
		result = ov_read(&mOggStream, data + size, SOUND_STREAM_BUF_SIZE - size, & section);
#else
        result = ov_read(&mOggStream, data + size, SOUND_STREAM_BUF_SIZE - size, 0, 2, 1, & section);
#endif

        if ( OV_HOLE == result )
        {
            LOG.Debug("oggstream OV_HOLE\n");
            return false;
        }
        else if ( OV_EBADLINK == result )
        {
            LOG.Debug("oggstream OV_EBADLINK\n");
            return false;
        }

        if(result > 0)
            size += result;
        else if(result < 0)
        {
            return false;
        }
        else
            break;
    }

    if(size == 0)
        return false;

    alBufferData(buffer, mFormat, data, size, mVorbisInfo->rate);
    if ( !SoundOpenAL::CheckOpenAlError("OggStream::Stream alBufferData") )
        return false;

    return true;
}

std::string OggStream::ErrorString(int code)
{
    switch(code)
    {
        case OV_EREAD:
            return std::string("Read from media.");
        case OV_ENOTVORBIS:
            return std::string("Not Vorbis data.");
        case OV_EVERSION:
            return std::string("Vorbis version mismatch.");
        case OV_EBADHEADER:
            return std::string("Invalid Vorbis header.");
        case OV_EFAULT:
            return std::string("Internal logic fault (bug or heap/stack corruption.");
        default:
            return std::string("Unknown Ogg error.");
    }
}

#endif // HC1_ENABLE_OGG
#endif // HC1_ENABLE_OPENAL
