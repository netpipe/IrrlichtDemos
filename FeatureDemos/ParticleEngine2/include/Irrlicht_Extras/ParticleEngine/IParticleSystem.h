#ifndef IPARTICLESYSTEM_H
#define IPARTICLESYSTEM_H

#include <ISceneNode.h>
#include "IParticleDrawer.h"
namespace irr
{
    namespace scene
    {
        namespace particle
        {
            #ifdef MAKE_IRR_ID
            const int PARTICLE_SYSTEM_SCENE_NODE_ID = MAKE_IRR_ID('p','s','y','s');
            #else
            const int PARTICLE_SYSTEM_SCENE_NODE_ID = 'psys';
            #endif // MAKE_IRR_ID
        }
        class IParticleSystem : public ISceneNode
        {
            public:
                IParticleSystem(ISceneNode* parent, ISceneManager* mgr, s32 id=-1,
                        const core::vector3df& position = core::vector3df(0,0,0),
                        const core::vector3df& rotation = core::vector3df(0,0,0),
                        const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f)) : ISceneNode(parent, mgr, id, position, rotation, scale) {}
                virtual ~IParticleSystem() {}

                virtual void addDrawer(particle::IParticleDrawer* drawer) = 0;
                virtual particle::IParticleDrawer* getDrawer(const irr::u32& id) = 0;
                virtual void setOverrideMaterial(const bool& use) = 0;
                virtual void setOverrideMaterial(const irr::video::SMaterial& material) = 0;
                virtual irr::video::SMaterial& getOverrideMaterial(void) = 0;

                virtual particle::IParticleDrawer* addTrailParticleDrawer(const bool& bindSpeedSize = false, const bool& useRotationAsVector = false, const irr::scene::particle::E_TRAILPARTICLE_DRAW_OPTION& drawOption = irr::scene::particle::ETDO_CENTER) = 0;
                virtual particle::IParticleDrawer* addParticleDrawer(void) = 0;
                virtual particle::IParticleDrawer* addOrientedParticleDrawer(void) = 0;

                virtual void setLocalParticles(bool local) = 0;
                virtual bool isLocalParticles(void) = 0;

                bool Paused;

                //ISceneNode
                virtual void OnRegisterSceneNode(void) = 0;
                virtual void OnAnimate(u32 timeMs) = 0;
                virtual void render(void) = 0;
                virtual const core::aabbox3d<f32>& getBoundingBox() const = 0;
                virtual video::SMaterial& getMaterial(u32 num) = 0;
                virtual u32 getMaterialCount() const = 0;
                ESCENE_NODE_TYPE getType() const
                {
                    return (ESCENE_NODE_TYPE)particle::PARTICLE_SYSTEM_SCENE_NODE_ID;
                }
            protected:
            private:
        };

        IParticleSystem* createParticleSystem(ISceneNode* parent, ISceneManager* mgr, s32 id=-1,
                        const core::vector3df& position = core::vector3df(0,0,0),
                        const core::vector3df& rotation = core::vector3df(0,0,0),
                        const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));
    }
}
#endif // IPARTICLESYSTEM_H
