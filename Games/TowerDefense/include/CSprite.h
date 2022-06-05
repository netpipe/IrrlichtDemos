#ifndef CSPRITE_H
#define CSPRITE_H

#include <irrlicht.h>


class CSprite : public irr::scene::ISceneNode
{
    public:
        /** Default constructor */
        CSprite(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id=-1,
				const irr::core::vector3df& position = irr::core::vector3df(0,0,0),
				const irr::core::vector3df& rotation = irr::core::vector3df(0,0,0),
				const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));
        /** Default destructor */
        virtual ~CSprite();

        //irrlicht stuff
        virtual void OnRegisterSceneNode(void);
        virtual void OnAnimate(irr::u32 timeMs);
        virtual void render(void);
        virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox(void) const;
        virtual irr::video::SMaterial& getMaterial(irr::u32 num);
        virtual irr::u32 getMaterialCount(void) const;

        //specific stuff
        void setTexture(const irr::c8* file);
    protected:
        irr::video::SMaterial Material;

        static int SpriteCounter;
        static irr::scene::IMeshBuffer* Static_Sprite_MeshBuffer;
    private:
};
#endif // CSPRITE_H
