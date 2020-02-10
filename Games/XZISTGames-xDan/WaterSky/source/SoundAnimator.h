
#ifndef __SoundAnimator_h
#define __SoundAnimator_h

#include "ISceneNode.h"
#include <irrKlang.h>

using namespace irr;

enum
{
    SOUNDANIM_LOOP          = 1 << 0, // loop sound (otherwise play once)
    SOUNDANIM_ENQUEUE       = 1 << 1, // add to queue (otherwise play immediately)
    SOUNDANIM_NO_WAIT       = 1 << 2, // next sound will not wait until this has finished
    SOUNDANIM_UNTIL_NEXT    = 1 << 3, // sound will end as soon as another available (added to queue)
    SOUNDANIM_BREAK_LOOP    = 1 << 4  // if the previous sound is looped, sound will start at end of its current loop.
};

struct Sound
{
    core::stringc fileName;
    u32 flags;
    audio::ISound *isound;
    bool started;
    u32 lastPlayPosition;
};

class SoundAnimator : public scene::ISceneNodeAnimator
{
public:
    SoundAnimator(audio::ISoundEngine *);
    ~SoundAnimator();
    
    void clearAll();
    audio::ISound *play(core::stringc, u32);
    void breakLoop();
    
    void animateNode(scene::ISceneNode *, u32);
    
    // should only set one animator as listener
    // (e.g. attached to camera scene node)
    void setAsListener(bool);
    
private:
    audio::ISoundEngine *sengine;
    u32 lastTimeMs;
    bool listener;
    core::vector3df lastPos;
    
    core::array <Sound> sounds;
};

#endif
