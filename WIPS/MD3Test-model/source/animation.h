#ifndef __ANIMATION_H_INCLUDED__
#define __ANIMATION_H_INCLUDED__

#include "iparser.h"
#include "IQ3ModelMesh.h"


namespace irr
{
namespace scene
{
namespace quake3
{
    /* Animations types */
    enum EMD3_MODEL_ANIMATION_TYPE
    {
        BOTH_DEATH1 = 0,
        BOTH_DEAD1 = 1,
        BOTH_DEATH2,
        BOTH_DEAD2,
        BOTH_DEATH3,
        BOTH_DEAD3,

        TORSO_GESTURE,
        TORSO_ATTACK,
        TORSO_ATTACK2,
        TORSO_DROP,
        TORSO_RAISE,
        TORSO_STAND,
        TORSO_STAND2,

        LEGS_WALKCR,
        LEGS_WALK,
        LEGS_RUN,
        LEGS_BACK,
        LEGS_SWIM,
        LEGS_JUMP,
        LEGS_LAND,
        LEGS_JUMPB,
        LEGS_LANDB,
        LEGS_IDLE,
        LEGS_IDLECR,
        LEGS_TURN,

        TORSO_GETFLAG,
        TORSO_GUARDBASE,
        TORSO_PATROL,
        TORSO_FOLLOWME,
        TORSO_AFFIRMATIVE,
        TORSO_NEGATIVE,

        MAX_ANIMATIONS,

        LEGS_BACKCR,
        LEGS_BACKWALK,
        FLAG_RUN,
        FLAG_STAND,
        FLAG_STAND2RUN,

        MAX_TOTALANIMATIONS
    };

    /* Struc for animations */
    struct SMD3_Animation
    {
        s32 firstFrame;
        f32 frameLerp;
        s32 fps;
        f32 initialLerp;
        s32 loopFrames;
        s32 numFrames;
        bool reversed;
    };

    enum EMD3_SEX_TYPE
    {
        GENDER_MALE,
        GENDER_FEMALE,
        GENDER_NEUTER
    };

    enum EMD3_FOOTSTEP_TYPE
    {
        FOOTSTEP_NORMAL,
        FOOTSTEP_BOOT,
        FOOTSTEP_FLESH,
        FOOTSTEP_MECH,
        FOOTSTEP_ENERGY,
        FOOTSTEP_METAL,
        FOOTSTEP_SPLASH
    };

class Animation : public IParser<SMD3_Animation>
{
public:
    Animation(Script & script) : IParser<SMD3_Animation>(&script) { parse(); }

    Animation(Script * script) : IParser<SMD3_Animation>(script) { parse(); }

    Animation(io::IReadFile* file, u32 size=0, s32 offset = 0)
          : IParser<SMD3_Animation>( new Script(file, size, offset) ) { parse(); }

    Animation(const c8* name, const c8* script, u32 size=0, u32 offset = 0)
          : IParser<SMD3_Animation>( new Script(name, script, size, offset) ) { parse(); }

    ~Animation() {}

    virtual u32 size() const { return Animations != NULL ? MAX_ANIMATIONS : 0; }

    virtual s32 find(const c8 * name) const;

    virtual const SMD3_Animation * get(u32 index) const { return ( index < size() ? &Animations[index] : NULL ); }

    virtual const SMD3_Animation * get(EMD3_MODEL_ANIMATION_TYPE type) const { return get( (u32) type ); }

    EMD3_SEX_TYPE sex() const { return Sex; }

    EMD3_FOOTSTEP_TYPE footSteps() const { return FootSteps; }

    bool fixedLegs() const { return FixedLegs; }

    bool fixedTorso() const { return FixedTorso; }

    core::vector3df headOffset() const { return HeadOffset; }

protected:
    virtual void parse();

private:
    EMD3_SEX_TYPE Sex;
    EMD3_FOOTSTEP_TYPE FootSteps;
    bool FixedLegs;
    bool FixedTorso;
    core::vector3df HeadOffset;
    SMD3_Animation Animations[MAX_TOTALANIMATIONS];
};

} // end namespace quake3
} // end namespace scene
} // end namespace irr

#endif
