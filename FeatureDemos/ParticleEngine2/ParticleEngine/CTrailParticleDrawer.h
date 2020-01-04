#ifndef CTRAILPARTICLEDRAWER_H
#define CTRAILPARTICLEDRAWER_H

#include "CParticleDrawer.h"
class CTrailParticleDrawer : public CParticleDrawer
{
    public:
        CTrailParticleDrawer(const bool& bindSpeedSize = false, const bool& useRotationAsVector = false, const irr::scene::particle::E_TRAILPARTICLE_DRAW_OPTION& drawOption = irr::scene::particle::ETDO_CENTER);
        virtual ~CTrailParticleDrawer();

        //void doParticles(const irr::core::vector3df& view, const irr::core::matrix4& transform, irr::u32 timeMs, irr::f32 diff);

        void createParticle(const irr::u32& id, const irr::scene::particle::Particle* particle, const irr::core::vector3df& view, const irr::core::matrix4& transform);
    protected:
        irr::scene::particle::E_TRAILPARTICLE_DRAW_OPTION DrawOption;
        bool UseRotationAsVector;
        bool BindSpeedSize;
    private:
};

#endif // CTRAILPARTICLEDRAWER_H
