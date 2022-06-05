#include "CSimpleGraphic.h"
#include "CSprite.h"
#include "CEntityManager.h"
#include "CStateManager.h"

CSimpleGraphic::CSimpleGraphic(CEntity* entity, CEntityManager* manager) : CComponent(entity, manager, ECT_GRAPHIC, "SimpleGraphic"), Position(getPropertyValue<irr::core::vector3df>("Position")), Dir(getPropertyValue<irr::core::vector3df>("Dir")), Texture(getPropertyValue<irr::core::stringc>("Texture"))
{
    Sprite = new CSprite(Manager->getManager()->getDevice()->getSceneManager()->getRootSceneNode(), Manager->getManager()->getDevice()->getSceneManager(), -1,
                        irr::core::vector3df(0,0,0),
                        irr::core::vector3df(0,0,0),
                        irr::core::vector3df(tile_size, tile_size, 0.0f));

    Sprite->setTexture(Texture.c_str());
}

CSimpleGraphic::~CSimpleGraphic()
{
    Sprite->remove();
    Sprite->drop();
}

void CSimpleGraphic::update(irr::f32 timeDiff)
{
    irr::core::vector2df d(Dir.X, Dir.Y);
    Sprite->setRotation(irr::core::vector3df(0,0,-d.getAngle()));
    Sprite->setPosition(irr::core::vector3df(Position.X, Position.Y, -1.f));
    Sprite->updateAbsolutePosition();
    //printf("GraphicPos[%f - %f - %f]\n", Position.X, Position.Y, -5.f);
}
