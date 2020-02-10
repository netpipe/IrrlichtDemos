
#include "SoundAnimator.h"

SoundAnimator::SoundAnimator(audio::ISoundEngine *sengine)
{
    this->sengine = sengine;
    lastTimeMs = 0;
    listener = false;
}

SoundAnimator::~SoundAnimator()
{
    clearAll();
}

void SoundAnimator::clearAll()
{
    // stop and remove all sounds
    for (u32 i = 0; i < sounds.size(); i ++)
    {
        if (sounds[i].isound)
        {
            if (!sounds[i].isound->isFinished()) sounds[i].isound->stop();
            sounds[i].isound->drop();
        }
    }
    sounds.clear();
}

audio::ISound *SoundAnimator::play(core::stringc fileName, u32 flags)
{
    Sound s =
    {
        fileName,
        flags,
        sengine->play3D(fileName.c_str(), core::vector3df(0,0,0), flags & SOUNDANIM_LOOP, true, true),
        false,
        0
    };
    
    if (s.isound) sounds.push_back(s);
    
    return s.isound;
}

void SoundAnimator::breakLoop()
{
    Sound s =
    {
        core::stringc(),
        SOUNDANIM_ENQUEUE|SOUNDANIM_BREAK_LOOP,
        NULL,
        false,
        0
    };
    
    sounds.push_back(s);
}

void SoundAnimator::animateNode(scene::ISceneNode *node, u32 timeMs)
{
    if (!node) return;
    
    // position...
    core::vector3df pos = node->getAbsolutePosition();
    
    // on first frame...
    if (lastTimeMs == 0)
    {
        // initial velocity of zero
        lastTimeMs = timeMs;
        lastPos = pos;
    }
    
    // calculate dt
	f32 dt = ((f32)(timeMs-lastTimeMs)) / 1000.0;
	lastTimeMs = timeMs;
	
	// velocity (for doppler effect)
    core::vector3df vel = (pos - lastPos) / dt;
    lastPos = pos;
	
    if (listener)
    {
        // up vector
        core::vector3df up(0,1,0);
        node->getAbsoluteTransformation().rotateVect(up);
        // forward vector
        core::vector3df forwards(0,0,1);
        node->getAbsoluteTransformation().rotateVect(forwards);
        // set listener
        sengine->setListenerPosition(pos, forwards, vel, up); 
    }
    
    bool noWait = true;
    
    for (u32 i = 0; i < sounds.size(); i ++)
    {
        if (sounds[i].started)
        {
            // remove if finished OR has SOUNDANIM_UNTIL_NEXT and another sound waiting
            int endSound = (sounds[i].isound->isFinished()
                || ( (sounds[i].flags & SOUNDANIM_UNTIL_NEXT) && sounds.size() > i+1)
                );
            // OR if: SOUNDANIM_LOOP and current getPlayPosition is smaller than last (loop over)
            // and another sound waiting and other sound has SOUNDANIM_BREAK_LOOP
            if ( (sounds[i].flags & SOUNDANIM_LOOP) && sounds.size() > i+1
                && sounds[i].isound->getPlayPosition() < sounds[i].lastPlayPosition)
            {
                endSound += sounds[i+1].flags & SOUNDANIM_BREAK_LOOP;
            }
            
            if (endSound)
            {
                // remove a finished sound
                if (!sounds[i].isound->isFinished()) sounds[i].isound->stop();
                sounds[i].isound->drop();
                sounds.erase(i);
                i --;
                continue;
            }
            else
            {
                sounds[i].isound->setPosition(pos);
                sounds[i].isound->setVelocity(vel);
                sounds[i].lastPlayPosition = sounds[i].isound->getPlayPosition();
            }
        }
        else // not playing yet, so decide whether it needs playing
        {
            if (sounds[i].flags & SOUNDANIM_ENQUEUE)
            {
                // Will play if first in queue OR all sounds so far were SOUNDANIM_NO_WAIT
                if (noWait)
                {
                    if (sounds[i].fileName.size() == 0)
                    {
                        // empty sound? remove it. Probably from breakLoop().
                        sounds.erase(i);
                        i --;
                        continue;
                    }
                    sounds[i].started = true;
                    sounds[i].isound->setIsPaused(false);
                    sounds[i].isound->setPosition(pos);
                    sounds[i].isound->setVelocity(vel);
                }
            }
            else // play instantly
            {
                sounds[i].started = true;
                sounds[i].isound->setIsPaused(false);
                sounds[i].isound->setPosition(pos);
                sounds[i].isound->setVelocity(vel);
                
                continue;
            }
        }
        
        if ( !(sounds[i].flags & SOUNDANIM_NO_WAIT) && !(sounds[i].flags & SOUNDANIM_UNTIL_NEXT) )
        {
            noWait = false;
        }
    }
}

void SoundAnimator::setAsListener(bool f)
{
    listener = f;
}

