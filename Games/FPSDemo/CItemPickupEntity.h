#ifndef _CITEMPICKUPENTITY_H_
#define _CITEMPICKUPENTITY_H_

#include <irrlicht.h>
#include "CBaseEntity.h"

using namespace irr;

namespace agEngine
{
    namespace entities
    {
        class CItemPickupEntity : public CBaseEntity
        {
        public:
            CItemPickupEntity();
            virtual ~CItemPickupEntity();

            virtual void update(CEntityManager* emgr);
        private:
            core::array<ENTITY_TYPE> allowableEntities;
        };
    }
}

#endif
