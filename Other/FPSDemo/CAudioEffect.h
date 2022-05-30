#ifndef _CAUDIOEFFECT_H_
#define _CAUDIOEFFECT_H_

#include <irrlicht.h>

using namespace irr;

namespace agEngine
{
    namespace audio
    {
        typedef enum
        {
            EFFECT_NULL,
            EFFECT_REVERB,
            EFFECT_CHORUS,
            EFFECT_DISTORTION,
            EFFECT_ECHO,
            EFFECT_FLANGER,
            EFFECT_FREQUENCY_SHIFTER,
            EFFECT_VOCAL_MORPHER,
            EFFECT_PITCH_SHIFTER,
            EFFECT_RING_MODULATOR,
            EFFECT_AUTOWAH,
            EFFECT_COMPRESSOR,
            EFFECT_EQUALIZER,
            EFFECT_EAXREVERB
        } audioEffects;

        typedef enum
        {
            REVERB_NULL,
            REVERB_PLATE,
            REVERB_ROOM,
            REVERB_HALL,
            REVERB_AUDITORIUM,
            REVERB_ARENA,
            REVERB_HANGAR,
            REVERB_MOUNTAIN,
        } reverbType;

        // OpenAL extensions
        typedef struct
        {
            // Effect object functions
            LPALGENEFFECTS alGenEffects;
            LPALDELETEEFFECTS alDeleteEffects;
            LPALISEFFECT alIsEffect;
            LPALEFFECTI alEffecti;
            LPALEFFECTIV alEffectiv;
            LPALEFFECTF alEffectf;
            LPALEFFECTFV alEffectfv;
            LPALGETEFFECTI alGetEffecti;
            LPALGETEFFECTIV alGetEffectiv;
            LPALGETEFFECTF alGetEffectf;
            LPALGETEFFECTFV alGetEffectfv;

            // Filter object functions
            LPALGENFILTERS alGenFilters;
            LPALDELETEFILTERS alDeleteFilters;
            LPALISFILTER alIsFilter;
            LPALFILTERI alFilteri;
            LPALFILTERIV alFilteriv;
            LPALFILTERF alFilterf;
            LPALFILTERFV alFilterfv;
            LPALGETFILTERI alGetFilteri;
            LPALGETFILTERIV alGetFilteriv;
            LPALGETFILTERF alGetFilterf;
            LPALGETFILTERFV alGetFilterfv;

            // Auxiliary slot object functions
            LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
            LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
            LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
            LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
            LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
            LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
            LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
            LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
            LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
            LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
            LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

        } EFX_EXTENSION_STRUCT;

        class CAudioEffect : public virtual IReferenceCounted
        {
        public:
            CAudioEffect(audioEffect effectType);
            virtual ~CAudioEffect();

            virtual void setSlots(u32 auxSlot, effectSlot);
            virtual void activate(u32 auxSlot, u32 effectSlot);
            virtual void setReverbType(reverbType reverb);
        protected:
            audioEffects effectType;
            ALuint effectSlot;
            ALuint auxSlot;

            // These are all the variables used to set the different effects

            // Reverb
            f32 reverbDensity;
            f32 reverbDiffusion;
            f32 reverbGain;
            f32 reverbGainHf;
            f32 reverbDecayTime;
            f32 reverbDecayHfRatio;
            f32 reverbReflectionsGain;
            f32 reverbReflectionsDelay;
            f32 reverbLateReverbGain;
            f32 reverbLateReverbDelay;
            f32 reverbAirAbsorptionGainHf;
            f32 reverbRoomRolloffFactor;
            s32 reverbDecayHfLimit;

            // Chorus
            s32 chorusWaveform;
            s32 chorusPhase;
            f32 chorusRate;
            f32 chorusDepth;
            f32 chorusFeedback;
            f32 chorusDelay;

            // Distortion
            f32 distortionEdge;
            f32 distortionGain;
            f32 distortionLowpassCutoff;
            f32 distortionEQCenter;
            f32 distortionEQBandwidth;

            // Echo
            f32 echoDelay;
            f32 echoLRDelay;
            f32 echoDamping;
            f32 echoFeedback;
            f32 echoSpread;

            // Flanger
            s32 flangerWaveform;
            s32 flangerPhase;
            f32 flangerRate;
            f32 flangerDepth;
            f32 flangerFeedback;
            f32 flangerDelay;

            // Frequency Shifter
            f32 shifterFrequency;
            u32 shifterLeftDirection;
            u32 shifterRightDirection;

            // Vocal Morpher
            u32 morpherPhonemeA;
            u32 morpherPhonemeB;
            s32 morpherPhonemeACoarseTuning;
            s32 morpherPhonemeBCoarseTuning;
            u32 morpherWaveform;
            f32 morpherRate;

            // Pitch Shifter
            s32 shifterCoarseTune;
            s32 shifterFineTune;

            // Ring Modulator
            f32 ringModulatorFrequency;
            f32 ringModulatorHighpassCutoff;
            s32 ringModulatorWaveform;

            // Autowah
            f32 autowahAttackTime;
            f32 autowahReleaseTime;
            f32 autowahResonance;
            f32 autowahPeakGain;

            // Equalizer
            f32 eqLowGain;
            f32 eqLowCutoff;
            f32 eqMid1Gain;
            f32 eqMid1Center;
            f32 eqMid1Width;
            f32 eqMid2Gain;
            f32 eqMid2Center;
            f32 eqMid2Width;
            f32 eqHighGain;
            f32 eqHighCutoff;

            // Compressor
            s32 compressorOnOff;
        };
    }
}

#endif
