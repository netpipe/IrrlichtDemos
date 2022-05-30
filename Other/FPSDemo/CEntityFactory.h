#ifndef _CENTITYFACTORY_H_
#define _CENTITYFACTORY_H_

#include <irrlicht.h>
#include "CBaseEntity.h"

namespace agEngine
{
    namespace entities
    {
        class CEntityFactory : public virtual IReferenceCounted
        {
        public:
            CEntityFactory() {};

            virtual ~CEntityFactory() {};

            virtual CBaseEntity* addEntity(ENTITY_TYPE type) = 0;
            virtual CBaseEntity* addEntity(const c8* typeName) = 0;

            virtual const c8* getCreateableEntityTypeName(ENTITY_TYPE type) const = 0;
        };
    }
}

#endif
