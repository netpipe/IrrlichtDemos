#include "CPositionalAudioEntity.h"

namespace agEngine
{
    namespace entities
    {
        CPositionalAudioEntity::CPositionalAudioEntity(audio::CAudioSource* aSource, scene::ISceneNode* node)
        : CBaseEntity(node), positionalAudio(aSource)
        {
            if (aSource)
                aSource->grab();

            // make the attached node invisible by default
            if (node)
                node->setVisible(false);

            // Set the audio attached to this
            stringParameters.push_back(L"noaudio.ogg");

            // Set the default positional parameters for the entity
            floatParameters.push_back(0.0f);
            floatParameters.push_back(0.0f);
            floatParameters.push_back(0.0f);
        }

        CPositionalAudioEntity::~CPositionalAudioEntity()
        {
            floatParameters.clear();
        }

        void CPositionalAudioEntity::setEPosition(const core::vector3d<float>& newPosition)
        {
            floatParameters[0] = newPosition.X;
            floatParameters[1] = newPosition.Y;
            floatParameters[2] = newPosition.Z;

            if (associatedSceneNode)
                associatedSceneNode->setPosition(newPosition);

            if (positionalAudio)
                positionalAudio->setPosition(newPosition);
        }

        const core::vector3d<float> CPositionalAudioEntity::getEPosition() const
        {
            return core::vector3d<float>(floatParameters[0], floatParameters[1], floatParameters[2]);
        }
    }
}
