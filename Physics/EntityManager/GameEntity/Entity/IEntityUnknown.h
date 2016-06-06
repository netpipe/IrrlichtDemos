#ifndef _IENTITY_UNKNOWN_H_
#define _IENTITY_UNKNOWN_H_
#include "IEntity.h"

//! the licence is on EntityManager.h
//! exemple for you created EntityExtention

namespace GameEntity
{
    class IEntityUnknown : public IEntity
    {
        public:
            IEntityUnknown();
            ~IEntityUnknown();
            virtual const char *getTypeName() const { return "Unknown"; }
            virtual void draw();
    };
}

#endif // _IENTITY_UNKNOWN_H_
