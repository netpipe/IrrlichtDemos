#ifndef _ENTITY_TYPE_HEADLER_
#define _ENTITY_TYPE_HEADLER_

//! the licence is on EntityManager.h
#include <string>
#include "../GamePhysique/GamePhysique.h"

using namespace GamePhysique::all;
#define  ENTITY_INVALIDE 0x0

namespace GameEntity
{
    struct EntityAttribute
    {
        EntityAttribute(const std::string & Ent,const std::string & Att):ent(Ent),att(Att){}
        std::string ent;
        std::string att;
    };
    typedef GamePhysique::OBJECT_CONSTRAINT_TYPE OBJECT_CONSTRAINT_TYPE;
    typedef GamePhysique::OBJECT_FORME     OBJECT_FORME;
    typedef GamePhysique::OBJECT_TYPE      OBJECT_TYPE;
    typedef GamePhysique::OBJECT_MATERIAL  OBJECT_MATERIAL;
    typedef struct GamePhysique::OBJECT    OBJECT;
}

#endif
