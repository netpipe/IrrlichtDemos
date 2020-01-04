#ifndef CORIENTEDPARTICLEDRAWER_H
#define CORIENTEDPARTICLEDRAWER_H

#include "CParticleDrawer.h"


class COrientedParticleDrawer : public CParticleDrawer
{
    public:
        COrientedParticleDrawer();
        virtual ~COrientedParticleDrawer();

        //void doParticles(const irr::core::vector3df& view, const irr::core::matrix4& transform, irr::u32 timeMs, irr::f32 diff);

        void createParticle(const irr::u32& id, const irr::scene::particle::Particle* particle, const irr::core::vector3df& view, const irr::core::matrix4& transform);
    protected:
    private:
};

#endif // CORIENTEDPARTICLEDRAWER_H
