#ifndef _CPLAYERENTITY_H_
#define _CPLAYERENTITY_H_

#include "CBaseEntity.h"

namespace agEngine
{
    namespace entities
    {
        class CPlayerEntity : public CBaseEntity
        {
        public:
            CPlayerEntity();
            virtual ~CPlayerEntity();

            virtual ENTITY_TYPE getEType() { return E_PLAYER_ENTITY; };

            virtual void setName(const core::stringw& newName);
            virtual const core::stringw getName() const;
            virtual void setPosition(const core::vector3d<f32>& newPosition);
            virtual void setRotation(const core::vector3d<f32>& newRotation);
            virtual const core::vector3d<f32> getPosition() const;
        };
    }
}

#endif
