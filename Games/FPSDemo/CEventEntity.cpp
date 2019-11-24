#include "CEventEntity.h"
#include "CEntityManager.h"

namespace agEngine
{
    namespace entities
    {
        CEventEntity::CEventEntity(script::CScriptManager* smgr)
        : CBaseScriptedEntity(smgr)
        {

        }

        CEventEntity::~CEventEntity()
        {

        }

        void CEventEntity::update(CEntityManager* emgr)
        {
            CBaseEntity::update(emgr);

            if (boolParameters[isActivatedOnTouch])
            {
                CBaseEntity* tempEntity = emgr->getClosestEntity(this);

                if ((tempEntity->getEType() == E_PLAYER_ENTITY) || (tempEntity->getEType() == E_ENEMY_ENTITY))
                    trigger();
            }
        }

        void CEventEntity::trigger()
        {

        }
    }
}
