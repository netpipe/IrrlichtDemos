#ifndef CMAIN_H
#define CMAIN_H

#include <irrlicht.h>

namespace irr
{
    namespace scene
    {
        class CParticleSystem;
    }
}
class CMain : public irr::IEventReceiver
{
    public:
        CMain();
        virtual ~CMain();

        bool OnEvent(const irr::SEvent& event);

        void run(void);
    protected:
        irr::IrrlichtDevice* Device;
        irr::scene::ISceneManager* smgr;
        irr::video::IVideoDriver* driver;

        irr::core::vector2di MousePos;
        irr::scene::ITriangleSelector* Selector;

    private:
};

#endif // CMAIN_H
