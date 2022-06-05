#ifndef CSIMPLEGRAPHIC_H
#define CSIMPLEGRAPHIC_H

#include <CComponent.h>

class CSprite;
class CSimpleGraphic : public CComponent
{
    public:
        /** Default constructor */
        CSimpleGraphic(CEntity* entity, CEntityManager* manager);
        /** Default destructor */
        virtual ~CSimpleGraphic();

        void update(irr::f32 timeDiff);
    protected:
        CSprite* Sprite;
        irr::core::vector3df& Position;
        irr::core::vector3df& Dir;
        irr::core::stringc& Texture;
    private:
};

#endif // CSIMPLEGRAPHIC_H
