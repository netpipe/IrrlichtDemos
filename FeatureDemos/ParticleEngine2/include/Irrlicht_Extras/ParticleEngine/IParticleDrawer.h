#ifndef IPARTICLEDRAWER_H
#define IPARTICLEDRAWER_H

#include <IReferenceCounted.h>
#include <matrix4.h>
namespace irr
{
    namespace video
    {
        class IVideoDriver;
        class SMaterial;
    }
    namespace scene
    {
        class ICameraSceneNode;

        namespace particle
        {
            class IParticleEmitter;
            class Particle;
            class SParticleUV;

            enum E_TRAILPARTICLE_DRAW_OPTION
            {
                ETDO_CENTER = 0,
                ETDO_START,
                ETDO_END,
                ETDO_COUNT
            };

            class IParticleAffector : public irr::IReferenceCounted
            {
            public:
                IParticleAffector(void){}
                virtual ~IParticleAffector(void){}

                virtual void affect(Particle* particle, irr::u32 timeMs, irr::f32 diff) = 0;
            };

            class IParticleDrawer : public irr::IReferenceCounted
            {
                public:
                    IParticleDrawer() {}
                    virtual ~IParticleDrawer() {}

                    virtual IParticleEmitter* addStandardEmitter(const irr::core::vector3df& rotation = irr::core::vector3df(0,0,0), const irr::core::vector3df& rotationspeed = irr::core::vector3df(0,0,0), const irr::core::vector3df& speed = irr::core::vector3df(0,1,0), const irr::core::vector3di& maxAngle = irr::core::vector3di(45,0,45), const irr::u32& minPpS = 50, const irr::u32& maxPpS = 100, const irr::u32& maxEmitt = 100, const irr::u32& minlifetime = 100, const irr::u32& maxlifetime = 1000, const irr::core::vector2df& minsize = irr::core::vector2df(1,1), const irr::core::vector2df& maxsize = irr::core::vector2df(1,1), const irr::core::vector2df& sizespeed = irr::core::vector2df(0,0), const irr::video::SColor& minColor = irr::video::SColor(255,255,255,255), const irr::video::SColor& maxColor = irr::video::SColor(255,255,255,255)) = 0;

                    virtual irr::video::SMaterial& getMaterial(void) = 0;

                    virtual void addUVCoords(const SParticleUV& coord) = 0;

                    virtual void doParticles(bool localParticles, const irr::scene::ICameraSceneNode* camera, const irr::core::matrix4& transform, irr::u32 timeMs, irr::f32 diff) = 0;

                    virtual void createParticle(const irr::u32& id, const Particle* particle, const irr::core::vector3df& view, const irr::core::matrix4& transform) = 0;

                    virtual void drawParticles(const irr::core::matrix4& transform, irr::video::IVideoDriver* driver, bool useOneMaterial) = 0;

                    virtual void setEmitter(particle::IParticleEmitter* emitter) = 0;

                    virtual particle::IParticleEmitter* getEmitter(void) = 0;

                    virtual void addAffector(particle::IParticleAffector* affector) = 0;

                    virtual const irr::core::aabbox3df& getBBox(void) const = 0;
                protected:
                private:
            };
        }
    }
}
#endif // IPARTICLEDRAWER_H
