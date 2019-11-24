#include "CEventManager.h"

namespace agEngine
{
    namespace events
    {
        CEventManager::CEventManager(console::CConsole* cons)
        : console(cons)
        {
            if (cons)
                cons->grab();
        }

        CEventManager::~CEventManager()
        {
            if (console)
                console->drop();
        }

        bool CEventManager::OnEvent(const SEvent& event)
        {
            if (console)
            {
                if (console->parseEvent(event))
                    return true;
            }

            // This will continuously loop until an entity parses the input
            for (u32 i = 0; i < receivingEntities.size(); ++i)
            {
                if (receivingEntities[i]->parseEvent(event))
                    break;
            }

            return false;
        }

        void CEventManager::registerEventReceivingEntity(entities::CBaseEntity* ent)
        {
            ent->grab();
            receivingEntities.push_back(ent);
        }

        void CEventManager::unregisterEventReceivingEntity(entities::CBaseEntity* ent)
        {
            for (u32 i = 0; i < receivingEntities.size(); ++i)
            {
                if (receivingEntities[i] == ent)
                {
                    ent->drop();
                    receivingEntities.erase(i);
                    break;
                }
            }
        }
    }
}
