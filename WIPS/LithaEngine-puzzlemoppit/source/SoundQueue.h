
#include "ISoundQueue.h"
#include "IUpdatable.h"

class ISoundSystem;

struct QueuedSoundInfo
{
	ISound *sound;
	s32 flags;
	bool started;
	f32 lastPlayPosition;
};

class SoundQueue : public ISoundQueue, public IUpdatable
{
	ISoundSystem *soundSystem;
	
	std::vector<QueuedSoundInfo> soundInfos;
	std::vector<ISound *> tempSounds;
	
protected:
	void OnPause();
	void OnResume();
	
public:
	SoundQueue(ISoundSystem *soundSystem);
	~SoundQueue();
	
	void Add(ISound *sound, const c8 *soundFile, s32 flags);
	
	void Add2D(const c8 *soundFile, s32 flags);
	void Add3D(const c8 *soundFile, s32 flags);
	
	void BreakLoop();
	
	const std::vector<ISound *> &GetAllSounds();
	
	void Update(f32 dt);
};


