#ifndef CLASS_AUDIO_INTERFACE_OPEN_AL_H_INC
#define CLASS_AUDIO_INTERFACE_OPEN_AL_H_INC

#include <al.h>
#include <alc.h>
#include <TheoraAudioInterface.h>
#include <TheoraTimer.h>
#include <TheoraVideoClip.h>
#include <queue>

struct SBuffer_OpenAL
{
  ALuint id;
  int    nSamples;
};

class CAudioInterface_OpenAL : public TheoraAudioInterface, TheoraTimer
{
private :

  int   mBuffSize;
  int   mMaxBuffSize;
  short *mTempBuffer;
  float mTimeOffset;

  SBuffer_OpenAL             mBuffers[1000];
  std::queue<SBuffer_OpenAL> mBufferQueue;

  ALuint mSource;
  int    mNumProcessedSamples;
  int    mNumPlayedSamples;

public :

  CAudioInterface_OpenAL(TheoraVideoClip* owner, int nChannels, int freq);
  ~CAudioInterface_OpenAL();

  short floatToShort(float f);

  void insertData(float** data, int nSamples);
  void destroy();

  void update(float time_increase);

  void play();
  void pause();
  void seek(float time);
};

class CAudioInterfaceFactory_OpenAL : public TheoraAudioInterfaceFactory
{
private :

  ALCdevice  *gDevice;
  ALCcontext *gContext;

public :

  CAudioInterfaceFactory_OpenAL();
  ~CAudioInterfaceFactory_OpenAL();

  CAudioInterface_OpenAL* createInstance(TheoraVideoClip* owner, int nChannels, int freq);
};

#endif