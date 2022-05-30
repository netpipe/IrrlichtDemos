#include "CDefaultEntityFactory.h"

namespace agEngine
{
    namespace entities
    {
        CDefaultEntityFactory::CDefaultEntityFactory(CEntityManager* mgr)
        : Manager(mgr)
        {
            SupportedEntityTypes.push_back(SEntityTypePair(E_BASE_ENTITY, "base"));
            SupportedEntityTypes.push_back(SEntityTypePair(E_PLAYER_ENTITY, "player"));
            SupportedEntityTypes.push_back(SEntityTypePair(E_AUDIO_ENTITY, "audio"));
        }

        CDefaultEntityFactory::~CDefaultEntityFactory()
        {

        }

        CBaseEntity* CDefaultEntityFactory::addEntity(ENTITY_TYPE type)
        {
            switch(type)
            {
                case E_BASE_ENTITY:
                    return Manager->addBaseEntity();
                case E_PLAYER_ENTITY:
                    return Manager->addPlayerEntity();
                case E_AUDIO_ENTITY:
                    return Manager->addPositionalAudioEntity();
                default:
                    return 0;
            }
        }

        CBaseEntity* CDefaultEntityFactory::addEntity(const c8* typeName)
        {
            return addEntity(getTypeFromName(typeName));
        }

        ENTITY_TYPE CDefaultEntityFactory::getTypeFromName(const c8* name) const
        {
            for (u32 i = 0; i < SupportedEntityTypes.size(); ++i)
                if (SupportedEntityTypes[i].TypeName == name)
                    return SupportedEntityTypes[i].Type;

            return E_ENTITY_UNKNOWN;
        }

        const c8* CDefaultEntityFactory::getCreateableEntityTypeName(ENTITY_TYPE type) const
        {
            for (u32 i = 0; i < SupportedEntityTypes.size(); ++i)
                if (SupportedEntityTypes[i].Type == type)
                    return SupportedEntityTypes[i].TypeName.c_str();

            return 0;
        }
    }
}
