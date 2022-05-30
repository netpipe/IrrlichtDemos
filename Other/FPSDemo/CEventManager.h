#ifndef _CEVENTMANAGER_H_
#define _CEVENTMANAGER_H_

#include <irrlicht.h>

#include "CBaseEntity.h"
#include "CConsole.h"

using namespace irr;

namespace agEngine
{
    namespace events
    {
        class CEventManager : public IEventReceiver
        {
        public:
            CEventManager(console::CConsole* cons);
            virtual ~CEventManager();

            virtual bool OnEvent(const SEvent& event);

            virtual void registerEventReceivingEntity(entities::CBaseEntity* ent);
            virtual void unregisterEventReceivingEntity(entities::CBaseEntity* ent);
        private:
            core::array<entities::CBaseEntity*> receivingEntities;

            console::CConsole* console;
        };
    }
}

#endif
