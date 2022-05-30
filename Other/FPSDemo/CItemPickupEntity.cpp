#include "CItemPickupEntity.h"
#include "CEntityManager.h"

namespace agEngine
{
    namespace entities
    {
        CItemPickupEntity::CItemPickupEntity()
        {
            // Set the default allowable entities
            allowableEntities.push_back(E_PLAYER_ENTITY);
        }

        CItemPickupEntity::~CItemPickupEntity()
        {

        }

        void CItemPickupEntity::update(CEntityManager* emgr)
        {
            CBaseEntity::update(emgr);

            // gets the closest entity to this item
            CBaseEntity* temp = emgr->getClosestEntity(this);

            if (!temp)
                return;

            // See if the closest entity is something we can parse
            bool okFlag = false;
            for (u32 i = 0; i < allowableEntities.size(); ++i)
            {
                if (temp->getEType() == allowableEntities[i])
                {
                    okFlag = true;
                    break;
                }
            }

            if (!okFlag)
                return;

            // Invoke the script handler for it
        }
    }
}
