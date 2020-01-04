/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include "CBillboardParticleDrawer.h"
#include <quaternion.h>

CBillboardParticleDrawer::CBillboardParticleDrawer()
{
    //ctor
}

CBillboardParticleDrawer::~CBillboardParticleDrawer()
{
    //dtor
}

void CBillboardParticleDrawer::createParticle(const irr::u32& id, const irr::scene::particle::Particle* particle, const irr::core::vector3df& view, const irr::core::matrix4& transform)
{
    irr::f32 f = 0;
    if (particle->Rotation.X != 0)
    {
        f = 0.5f * particle->Size.X;
        irr::core::vector3df left ( transform[0] * f, transform[4] * f, transform[8] * f);

        f = -0.5f * particle->Size.Y;
        irr::core::vector3df forward ( transform[1] * f, transform[5] * f, transform[9] * f);

        irr::core::matrix4 mat;
        irr::core::quaternion quad;
        quad.fromAngleAxis(particle->Rotation.X, view);
        mat = quad.getMatrix();

        mat.rotateVect(forward);
        mat.rotateVect(left);

        Buffer->Vertices[id*4].Pos       = particle->Position+forward+left;
        Buffer->Vertices[id*4].Color     = particle->Color;
        Buffer->Vertices[id*4].Normal    = -view;
        Buffer->Vertices[id*4].TCoords   = particle->UVs.UV0;

        Buffer->Vertices[id*4+1].Pos       = particle->Position+forward-left;
        Buffer->Vertices[id*4+1].Color     = particle->Color;
        Buffer->Vertices[id*4+1].Normal    = -view;
        Buffer->Vertices[id*4+1].TCoords   = particle->UVs.UV1;

        Buffer->Vertices[id*4+2].Pos       = particle->Position-forward+left;
        Buffer->Vertices[id*4+2].Color     = particle->Color;
        Buffer->Vertices[id*4+2].Normal    = -view;
        Buffer->Vertices[id*4+2].TCoords   = particle->UVs.UV2;

        Buffer->Vertices[id*4+3].Pos       = particle->Position-forward-left;
        Buffer->Vertices[id*4+3].Color     = particle->Color;
        Buffer->Vertices[id*4+3].Normal    = -view;
        Buffer->Vertices[id*4+3].TCoords   = particle->UVs.UV3;
    }
    else
    {
        f = 0.5f * particle->Size.X;
        const irr::core::vector3df left ( transform[0] * f, transform[4] * f, transform[8] * f);

        f = -0.5f * particle->Size.Y;
        const irr::core::vector3df forward ( transform[1] * f, transform[5] * f, transform[9] * f);

        Buffer->Vertices[id*4].Pos       = particle->Position+forward+left;
        Buffer->Vertices[id*4].Color     = particle->Color;
        Buffer->Vertices[id*4].Normal    = -view;
        Buffer->Vertices[id*4].TCoords   = particle->UVs.UV0;

        Buffer->Vertices[id*4+1].Pos       = particle->Position+forward-left;
        Buffer->Vertices[id*4+1].Color     = particle->Color;
        Buffer->Vertices[id*4+1].Normal    = -view;
        Buffer->Vertices[id*4+1].TCoords   = particle->UVs.UV1;

        Buffer->Vertices[id*4+2].Pos       = particle->Position-forward+left;
        Buffer->Vertices[id*4+2].Color     = particle->Color;
        Buffer->Vertices[id*4+2].Normal    = -view;
        Buffer->Vertices[id*4+2].TCoords   = particle->UVs.UV2;

        Buffer->Vertices[id*4+3].Pos       = particle->Position-forward-left;
        Buffer->Vertices[id*4+3].Color     = particle->Color;
        Buffer->Vertices[id*4+3].Normal    = -view;
        Buffer->Vertices[id*4+3].TCoords   = particle->UVs.UV3;
    }
}
