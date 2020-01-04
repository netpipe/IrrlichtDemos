#ifndef CPARTICLESYSTEM_H
#define CPARTICLESYSTEM_H

#include "Irrlicht_Extras/ParticleEngine/IParticleSystem.h"
namespace irr
{
    namespace scene
    {
        class CParticleSystem : public irr::scene::IParticleSystem
        {
        public:

            CParticleSystem(ISceneNode* parent, ISceneManager* mgr, s32 id=-1,
                        const core::vector3df& position = core::vector3df(0,0,0),
                        const core::vector3df& rotation = core::vector3df(0,0,0),
                        const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));
            virtual ~CParticleSystem();

            void addDrawer(particle::IParticleDrawer* drawer);
            particle::IParticleDrawer* getDrawer(const irr::u32& id);
            void setOverrideMaterial(const bool& use);
            void setOverrideMaterial(const irr::video::SMaterial& material);
            irr::video::SMaterial& getOverrideMaterial(void);

            particle::IParticleDrawer* addTrailParticleDrawer(const bool& bindSpeedSize, const bool& useRotationAsVector, const irr::scene::particle::E_TRAILPARTICLE_DRAW_OPTION& drawOption);
            particle::IParticleDrawer* addParticleDrawer(void);
            particle::IParticleDrawer* addOrientedParticleDrawer(void);

            void setLocalParticles(bool local);
            bool isLocalParticles(void);

            //bool Paused;

            //ISceneNode
            void OnRegisterSceneNode(void);
            void OnAnimate(u32 timeMs);
            void render(void);
            const core::aabbox3d<f32>& getBoundingBox() const;
            video::SMaterial& getMaterial(u32 num);
            u32 getMaterialCount() const;
        protected:
            void update(irr::u32 timeMs, irr::f32 diff);

            irr::core::array<particle::IParticleDrawer*> Drawer;
            irr::video::SMaterial OverrideMaterial;
            bool UseOverrideMaterial;

            bool localParticles;

            core::aabbox3d<f32> Box;

            u32 LastTime;
            irr::f32 diff;
        private:
        };
    }
}

#endif // CPARTICLESYSTEM_H
