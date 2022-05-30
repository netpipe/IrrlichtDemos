#include "CPlayerEntity.h"

namespace agEngine
{
    namespace entities
    {
        CPlayerEntity::CPlayerEntity()
        : CBaseEntity()
        {

        }

        CPlayerEntity::~CPlayerEntity()
        {

        }

        void CPlayerEntity::setName(const core::stringw& newName)
        {
            stringParameters[nodeName] = newName;
        }

        const core::stringw CPlayerEntity::getName() const
        {
            return stringParameters[nodeName];
        }

        void CPlayerEntity::setPosition(const core::vector3d<f32>& newPosition)
        {
            floatParameters[xPosition] = newPosition.X;
            floatParameters[yPosition] = newPosition.Y;
            floatParameters[zPosition] = newPosition.Z;

            if (associatedSceneNode)
                associatedSceneNode->setPosition(newPosition);
        }

        void CPlayerEntity::setRotation(const core::vector3d<f32>& newRotation)
        {
            floatParameters[xRotation] = newRotation.X;
            floatParameters[yRotation] = newRotation.Y;
            floatParameters[zRotation] = newRotation.Z;

            if (associatedSceneNode)
                associatedSceneNode->setRotation(newRotation);
        }

        const core::vector3d<f32> CPlayerEntity::getPosition() const
        {
            return core::vector3d<f32>(floatParameters[0], floatParameters[1], floatParameters[2]);
        }
    }
}
