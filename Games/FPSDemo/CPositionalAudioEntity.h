#ifndef _CPOSITIONALAUDIOENTITY_H_
#define _CPOSITIONALAUDIOENTITY_H_

#include <irrlicht.h>

#include "CBaseEntity.h"

#include "CAudioSource.h"

using namespace irr;

namespace agEngine
{
    namespace entities
    {
        class CPositionalAudioEntity : public CBaseEntity
        {
        public:
            CPositionalAudioEntity(audio::CAudioSource* aSource = 0, scene::ISceneNode* node = 0);
            virtual ~CPositionalAudioEntity();

            virtual void setEPosition(const core::vector3d<float>& newPosition);
            virtual const core::vector3d<float> getEPosition() const;

            virtual ENTITY_TYPE getEType() { return E_AUDIO_ENTITY; };

        private:
            audio::CAudioSource* positionalAudio;
        };
    }
}

#endif
