#include "CSprite.h"
int CSprite::SpriteCounter = 0;
irr::scene::IMeshBuffer* CSprite::Static_Sprite_MeshBuffer = NULL;

CSprite::CSprite(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
                 const irr::core::vector3df& position,
                 const irr::core::vector3df& rotation,
                 const irr::core::vector3df& scale) : irr::scene::ISceneNode(parent, mgr, id, position, rotation, scale)
{
    //ctor
    SpriteCounter++;
    if (!Static_Sprite_MeshBuffer)
    {
        irr::scene::SMeshBuffer* buffer = new irr::scene::SMeshBuffer;
        buffer->Vertices.push_back(irr::video::S3DVertex(
                                       irr::core::vector3df(-0.5, 0.5, 0.0),
                                       irr::core::vector3df(0.0, 0.0, 1.0), irr::video::SColor(255,255,255,255),
                                       irr::core::vector2df(0.0, 0.0)));
        buffer->Vertices.push_back(irr::video::S3DVertex(
                                       irr::core::vector3df(0.5, 0.5, 0.0),
                                       irr::core::vector3df(0.0, 0.0, 1.0), irr::video::SColor(255,255,255,255),
                                       irr::core::vector2df(1.0, 0.0)));
        buffer->Vertices.push_back(irr::video::S3DVertex(
                                       irr::core::vector3df(0.5, -0.5, 0.0),
                                       irr::core::vector3df(0.0, 0.0, 1.0), irr::video::SColor(255,255,255,255),
                                       irr::core::vector2df(1.0, 1.0)));
        buffer->Vertices.push_back(irr::video::S3DVertex(
                                       irr::core::vector3df(-0.5, -0.5, 0.0),
                                       irr::core::vector3df(0.0, 0.0, 1.0), irr::video::SColor(255,255,255,255),
                                       irr::core::vector2df(0.0, 1.0)));

        buffer->Indices.push_back(0);
        buffer->Indices.push_back(1);
        buffer->Indices.push_back(2);
        buffer->Indices.push_back(3);
        buffer->Indices.push_back(0);
        buffer->Indices.push_back(2);
        buffer->recalculateBoundingBox();
        buffer->setHardwareMappingHint(irr::scene::EHM_STATIC);

        Static_Sprite_MeshBuffer = buffer;
    }

    //Material stuff
    Material.Lighting = false;
    Material.MaterialType = irr::video::EMT_SOLID;
}

CSprite::~CSprite()
{
    //dtor
    SpriteCounter--;
    if (SpriteCounter <= 0 && Static_Sprite_MeshBuffer)
    {
        Static_Sprite_MeshBuffer->drop();
        Static_Sprite_MeshBuffer = NULL;
    }
}

irr::u32 CSprite::getMaterialCount(void) const
{
    return 1;
}

irr::video::SMaterial& CSprite::getMaterial(irr::u32 num)
{
    return Material;
}

const irr::core::aabbox3d<irr::f32>& CSprite::getBoundingBox(void) const
{
    return Static_Sprite_MeshBuffer->getBoundingBox();
}

void CSprite::render(void)
{
    irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    driver->setMaterial(Material);
    driver->drawMeshBuffer(Static_Sprite_MeshBuffer);
}

void CSprite::OnAnimate(irr::u32 timeMs)
{
    irr::scene::ISceneNode::OnAnimate(timeMs);
}

void CSprite::OnRegisterSceneNode(void)
{
    if (this->isVisible())
        SceneManager->registerNodeForRendering(this);

    irr::scene::ISceneNode::OnRegisterSceneNode();
}

void CSprite::setTexture(const irr::c8* file)
{
    if (irr::core::hasFileExtension ( file, "png" ))
        Material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    else
        Material.MaterialType = irr::video::EMT_SOLID;
    Material.setTexture(0, SceneManager->getVideoDriver()->getTexture(file));
}

