#ifndef CBILLBOARDPARTICLEDRAWER_H
#define CBILLBOARDPARTICLEDRAWER_H

#include "CParticleDrawer.h"


class CBillboardParticleDrawer : public CParticleDrawer
{
    public:
        CBillboardParticleDrawer();
        virtual ~CBillboardParticleDrawer();

        void createParticle(const irr::u32& id, const irr::scene::particle::Particle* particle, const irr::core::vector3df& view, const irr::core::matrix4& transform);
    protected:
    private:
};

#endif // CBILLBOARDPARTICLEDRAWER_H
