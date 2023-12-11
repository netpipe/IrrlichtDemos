
#ifndef I_SOUND_QUEUE_H
#define I_SOUND_QUEUE_H

#include "litha_internal.h"
#include "IPausable.h"
#include <vector>

class ISound;

enum E_SOUNDQUEUE_FLAG
{
    ESF_LOOP          = 1 << 0, // loop sound (otherwise play once)
    ESF_ENQUEUE       = 1 << 1, // add to queue (otherwise play immediately)
    ESF_NO_WAIT       = 1 << 2, // next sound will not wait until this has finished
    ESF_UNTIL_NEXT    = 1 << 3, // sound will end as soon as another available (added to queue)
    ESF_BREAK_LOOP    = 1 << 4  // if the previous sound is looped, sound will start at end of its current loop.
};

class ISoundQueue : public virtual IReferenceCounted, public virtual IPausable
{
public:
	virtual ~ISoundQueue(){}
	
	// Add a sound to the queue. It will be played as soon as the given flag's condition is met.
	// sound - a unique sound created from ISoundSystem
	// soundFile - the file to play
	virtual void Add(ISound *sound, const c8 *soundFile, s32 flags) = 0;
	
	// Same as above but auto creates a 2D sound.
	virtual void Add2D(const c8 *soundFile, s32 flags) = 0;
	
	// Same as above but auto creates a 3D sound for use.
	virtual void Add3D(const c8 *soundFile, s32 flags) = 0;
	
	// I've forgotten what this does exactly. Something pretty darned awesome I expect.
	virtual void BreakLoop() = 0;
	
	// A list of all sounds currently being played by sound queue.
	// This is used by SoundSource to set the position/velocity of 3D sounds.
	virtual const std::vector<ISound *> &GetAllSounds() = 0;
};

#endif
