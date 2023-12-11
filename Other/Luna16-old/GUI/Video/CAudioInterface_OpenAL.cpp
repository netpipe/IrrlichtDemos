#include "CAudioInterface_OpenAL.h"
#include <climits>

short CAudioInterface_OpenAL::floatToShort(float f)
{
  if      (f >  1) f =  1;
  else if (f < -1) f = -1;
  return (short)(f * SHRT_MAX);
}

CAudioInterface_OpenAL::CAudioInterface_OpenAL(TheoraVideoClip* owner, int nChannels, int freq) :
                        TheoraAudioInterface(owner, nChannels, freq),
                        mBuffSize(0), mNumProcessedSamples(0), mTimeOffset(0), mNumPlayedSamples(0), mTempBuffer(0)
{
  mMaxBuffSize = freq * mNumChannels * 2;
  mTempBuffer  = new short[mMaxBuffSize];
  alGenSources(1, &mSource);
  owner->setTimer(this);
}

void CAudioInterface_OpenAL::destroy()
{
}

CAudioInterface_OpenAL::~CAudioInterface_OpenAL()
{
  if (mTempBuffer) delete mTempBuffer;
}

void CAudioInterface_OpenAL::insertData(float** data, int nSamples)
{
  for (int i = 0; i < nSamples; ++i)
  {
    if (mBuffSize < mMaxBuffSize)
    {
      mTempBuffer[++mBuffSize] = floatToShort(data[0][i]);

      if (mNumChannels == 2)
        mTempBuffer[++mBuffSize] = floatToShort(data[1][i]);
    }
    if (mBuffSize == mFreq * mNumChannels / 4)
    {
      SBuffer_OpenAL buff;
      alGenBuffers(1, &buff.id);

      ALuint format;
      if (mNumChannels == 1)
        format = AL_FORMAT_MONO16;
      else
        format = AL_FORMAT_STEREO16;

      alBufferData(buff.id, format, mTempBuffer, mBuffSize * 2, mFreq);
      alSourceQueueBuffers(mSource, 1, &buff.id);
      buff.nSamples = mBuffSize / mNumChannels;
      mNumProcessedSamples += mBuffSize / mNumChannels;
      mBufferQueue.push(buff);

      mBuffSize = 0;

      int state;
      alGetSourcei(mSource, AL_SOURCE_STATE, &state);
      if (state != AL_PLAYING)
      {
        alSourcef(mSource, AL_SAMPLE_OFFSET, (float)mNumProcessedSamples - mFreq / 4);
        alSourcePlay(mSource);
      }
    }
  }
}

void CAudioInterface_OpenAL::update(float time_increase)
{
  int i, state, nProcessed;
  SBuffer_OpenAL buff;

  // process played buffers

  alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &nProcessed);
  for (i = 0; i < nProcessed; ++i)
  {
    buff = mBufferQueue.front();
    mBufferQueue.pop();
    mNumPlayedSamples += buff.nSamples;
    alSourceUnqueueBuffers(mSource, 1, &buff.id);
    alDeleteBuffers(1, &buff.id);
  }

  // control playback and return time position

  alGetSourcei(mSource, AL_SOURCE_STATE, &state);
  if (state == AL_PLAYING)
  {
    alGetSourcef(mSource, AL_SEC_OFFSET, &mTime);
    mTime += (float)mNumPlayedSamples / mFreq;
    mTimeOffset = 0;
  }
  else
  {
    mTime = (float)mNumProcessedSamples / mFreq + mTimeOffset;
    mTimeOffset += time_increase;
  }

  float duration = mClip->getDuration();
  if (mTime > duration) mTime = duration;
}

void CAudioInterface_OpenAL::pause()
{
  alSourcePause(mSource);
  TheoraTimer::pause();
}

void CAudioInterface_OpenAL::play()
{
  alSourcePlay(mSource);
  TheoraTimer::play();
}

void CAudioInterface_OpenAL::seek(float time)
{
  SBuffer_OpenAL buff;

  alSourceStop(mSource);

  while (!mBufferQueue.empty())
  {
    buff = mBufferQueue.front();
    mBufferQueue.pop();
    alSourceUnqueueBuffers(mSource, 1, &buff.id);
    alDeleteBuffers(1, &buff.id);
  }

  mBuffSize = 0;
  mTimeOffset = 0;

  mNumPlayedSamples = mNumProcessedSamples = (int)time * mFreq;
}

CAudioInterfaceFactory_OpenAL::CAudioInterfaceFactory_OpenAL() : gDevice(0), gContext(0)
{
  gDevice = alcOpenDevice("");

  if (alcGetError(gDevice) != ALC_NO_ERROR) goto Fail;
  gContext = alcCreateContext(gDevice, NULL);
  if (alcGetError(gDevice) != ALC_NO_ERROR) goto Fail;
  alcMakeContextCurrent(gContext);
  if (alcGetError(gDevice) != ALC_NO_ERROR) goto Fail;

  return;

  Fail :

    gDevice  = NULL;
    gContext = NULL;
}

CAudioInterfaceFactory_OpenAL::~CAudioInterfaceFactory_OpenAL()
{
  if (gDevice)
  {
    alcMakeContextCurrent(NULL);
    alcDestroyContext(gContext);
    alcCloseDevice(gDevice);
  }
}

CAudioInterface_OpenAL* CAudioInterfaceFactory_OpenAL::createInstance(TheoraVideoClip* owner, int nChannels, int freq)
{
  return new CAudioInterface_OpenAL(owner, nChannels, freq);
}
