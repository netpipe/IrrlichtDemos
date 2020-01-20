
#include "SoundQueue.h"
#include "ISoundSystem.h"
#include "ISound2D.h"
#include "ISound3D.h"
#include "IEngine.h"

SoundQueue::SoundQueue(ISoundSystem *soundSystem)
{
	this->soundSystem  = soundSystem;
	
	// Update this in logic task.
	// But task must not keep a reference to this sound queue, as there is no RemoveSoundQueue method.
	GetEngine()->GetLogicUpdater().AddUpdatable(this, false);
}

SoundQueue::~SoundQueue()
{
	for (u32 i = 0; i < soundInfos.size(); i ++)
	{
		if (soundInfos[i].sound)
			soundInfos[i].sound->drop();
	}
	
	GetEngine()->GetLogicUpdater().RemoveUpdatable(this);
}

void SoundQueue::Add(ISound *sound, const c8 *soundFile, s32 flags)
{
	if (!sound)
	{
		WARN << "No ISound given (" << soundFile << ")";
		return;
	}
	
	// sound not already present?
	for (u32 i = 0; i < soundInfos.size(); i ++)
	{
		if (soundInfos[i].sound == sound)
			return;
	}
	
	sound->grab();
	
	QueuedSoundInfo info;
	info.sound = sound;
	info.flags = flags;
	info.started = false;
	info.lastPlayPosition = 0.0;
	soundInfos.push_back(info);
	
	// Now start the sound playing, then immediately pause it, ready!
	// Also set loop flag.
	
	if (flags & ESF_LOOP)
		sound->SetIsLooped(true);
	
	sound->Play(soundFile);
	sound->Pause();
	
	// If this sound queue is currently paused, we call pause again.
	// (as resume is called on all sounds when the sound queue resumes)
	if (IsPaused())
		sound->Pause();
}

void SoundQueue::Add2D(const c8 *soundFile, s32 flags)
{
	ISound2D *sound = soundSystem->CreateSound2D();
	Add(sound, soundFile, flags);
	sound->drop();
}

void SoundQueue::Add3D(const c8 *soundFile, s32 flags)
{
	ISound3D *sound = soundSystem->CreateSound3D();
	Add(sound, soundFile, flags);
	sound->drop();
}

void SoundQueue::BreakLoop()
{
	QueuedSoundInfo info;
	info.sound = NULL;
	info.flags = ESF_ENQUEUE|ESF_BREAK_LOOP;
	info.started = false;
	info.lastPlayPosition = 0.0;
	soundInfos.push_back(info);
}

const std::vector<ISound *> &SoundQueue::GetAllSounds()
{
	tempSounds.clear();
	
	for (u32 i = 0; i < soundInfos.size(); i ++)
	{
		// ignore NULL sounds from BreakLoop()
		if (soundInfos[i].sound)
			tempSounds.push_back(soundInfos[i].sound);
	}
	
	return tempSounds;
}

void SoundQueue::OnPause()
{
	IUpdatable::OnPause();
	
	// Pause all playback
	
	GetAllSounds();
	
	for (u32 i = 0; i < tempSounds.size(); i ++)
		tempSounds[i]->Pause();
}

void SoundQueue::OnResume()
{
	IUpdatable::OnResume();
	
	// Resume all
	
	GetAllSounds();
	
	for (u32 i = 0; i < tempSounds.size(); i ++)
		tempSounds[i]->Resume();
}

void SoundQueue::Update(f32 dt)
{
	if (IsPaused())
		return;
	
	bool noWait = true;
	
	for (u32 i = 0; i < soundInfos.size(); i ++)
	{
		if (soundInfos[i].started)
		{
			// remove if finished OR has ESF_UNTIL_NEXT and another sound waiting
			int endSound = (soundInfos[i].sound->IsFinished()
					|| ( (soundInfos[i].flags & ESF_UNTIL_NEXT) && soundInfos.size() > i+1)
					);
			// OR if: ESF_LOOP and current PlayPosition is smaller than last (loop over)
			// and another sound waiting and other sound has ESF_BREAK_LOOP
			if ( (soundInfos[i].flags & ESF_LOOP) && soundInfos.size() > i+1
					&& soundInfos[i].sound->GetPlayPosition() < soundInfos[i].lastPlayPosition)
			{
				endSound += soundInfos[i+1].flags & ESF_BREAK_LOOP;
			}
			
			if (endSound)
			{
				// remove a finished sound
				soundInfos[i].sound->Stop();
				soundInfos[i].sound->drop();
				soundInfos.erase(soundInfos.begin()+i);
				i --;
				continue;
			}
			else
			{
				soundInfos[i].lastPlayPosition = soundInfos[i].sound->GetPlayPosition();
			}
		}
		else // not playing yet, so decide whether it needs playing
		{
			if (soundInfos[i].flags & ESF_ENQUEUE)
			{
				// Will play if first in queue OR all sounds so far were SOUNDANIM_NO_WAIT
				if (noWait)
				{
					if (!soundInfos[i].sound)
					{
						// empty sound? remove it. Probably from BreakLoop().
						soundInfos.erase(soundInfos.begin()+i);
						i --;
						continue;
					}
					soundInfos[i].started = true;
					soundInfos[i].sound->Resume();
				}
			}
			else // play instantly
			{
				soundInfos[i].started = true;
				soundInfos[i].sound->Resume();
				
				continue;
			}
		}
		
		if ( !(soundInfos[i].flags & ESF_NO_WAIT) && !(soundInfos[i].flags & ESF_UNTIL_NEXT) )
		{
			noWait = false;
		}
	}
}




