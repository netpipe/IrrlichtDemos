#ifndef _CDEFAULTENTITYFACTORY_H_
#define _CDEFAULTENTITYFACTORY_H_

#include "CEntityManager.h"
#include "CEntityFactory.h"

namespace agEngine
{
    namespace entities
    {
        class CEntityManager;

        class CDefaultEntityFactory : public CEntityFactory
        {
        public:
            CDefaultEntityFactory(CEntityManager* mgr);
            virtual ~CDefaultEntityFactory();

            virtual CBaseEntity* addEntity(ENTITY_TYPE);
            virtual CBaseEntity* addEntity(const c8* typeName);

            virtual const c8* getCreateableEntityTypeName(ENTITY_TYPE type) const;
        private:
            ENTITY_TYPE getTypeFromName(const c8* name) const;

            struct SEntityTypePair
            {
                SEntityTypePair(ENTITY_TYPE type, const c8* name)
                : Type(type), TypeName(name)
                { }

                ENTITY_TYPE Type;
                core::stringc TypeName;
            };

            core::array<SEntityTypePair> SupportedEntityTypes;

            CEntityManager* Manager;
        };
    }
}

#endif
