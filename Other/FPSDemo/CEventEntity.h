#ifndef _CEVENTENTITY_H_
#define _CEVENTENTITY_H_

#include <irrlicht.h>
#include "CBaseScriptedEntity.h"

using namespace irr;

namespace agEngine
{
    namespace entities
    {
        class CEventEntity : public CBaseScriptedEntity
        {
        public:
            CEventEntity(script::CScriptManager* smgr);
            ~CEventEntity();

            virtual void update(CEntityManager* emgr);
            virtual void trigger();
        private:

        };
    }
}

#endif // _CEVENTENTITY_H_

