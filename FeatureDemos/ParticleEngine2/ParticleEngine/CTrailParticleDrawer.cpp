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
#include "CTrailParticleDrawer.h"

CTrailParticleDrawer::CTrailParticleDrawer(const bool& bindSpeedSize, const bool& useRotationAsVector, const irr::scene::particle::E_TRAILPARTICLE_DRAW_OPTION& drawOption)
{
    //ctor
    DrawOption = drawOption;
    UseRotationAsVector = useRotationAsVector;
    BindSpeedSize = bindSpeedSize;
}

CTrailParticleDrawer::~CTrailParticleDrawer()
{
    //dtor
}

void CTrailParticleDrawer::createParticle(const irr::u32& id, const irr::scene::particle::Particle* particle, const irr::core::vector3df& view, const irr::core::matrix4& transform)
{
    irr::core::vector3df forward;
    if (particle->Speed == irr::core::vector3df() || UseRotationAsVector)
        forward = particle->Rotation;
    else
        forward = particle->Speed;
    forward.normalize();

    irr::core::vector3df left = view.crossProduct(forward);
    left.normalize();

    if (DrawOption == irr::scene::particle::ETDO_CENTER)
        forward = forward*particle->Size.Y*0.5f;
    else
        forward = forward*particle->Size.Y;

    if (BindSpeedSize)
        forward *= particle->Speed.getLengthSQ()/particle->StartSpeed.getLengthSQ();

    left = left*particle->Size.X*0.5f;

    if (DrawOption == irr::scene::particle::ETDO_START)
    {
        Buffer->Vertices[id*4].Pos       = particle->Position+forward+left;
        Buffer->Vertices[id*4].Color     = particle->Color;
        Buffer->Vertices[id*4].Normal    = -view;
        Buffer->Vertices[id*4].TCoords   = particle->UVs.UV0;

        Buffer->Vertices[id*4+1].Pos       = particle->Position+forward-left;
        Buffer->Vertices[id*4+1].Color     = particle->Color;
        Buffer->Vertices[id*4+1].Normal    = -view;
        Buffer->Vertices[id*4+1].TCoords   = particle->UVs.UV1;

        Buffer->Vertices[id*4+2].Pos       = particle->Position+left;
        Buffer->Vertices[id*4+2].Color     = particle->Color;
        Buffer->Vertices[id*4+2].Normal    = -view;
        Buffer->Vertices[id*4+2].TCoords   = particle->UVs.UV2;

        Buffer->Vertices[id*4+3].Pos       = particle->Position-left;
        Buffer->Vertices[id*4+3].Color     = particle->Color;
        Buffer->Vertices[id*4+3].Normal    = -view;
        Buffer->Vertices[id*4+3].TCoords   = particle->UVs.UV3;
    }
    else if (DrawOption == irr::scene::particle::ETDO_END)
    {
        Buffer->Vertices[id*4].Pos       = particle->Position+left;
        Buffer->Vertices[id*4].Color     = particle->Color;
        Buffer->Vertices[id*4].Normal    = -view;
        Buffer->Vertices[id*4].TCoords   = particle->UVs.UV0;

        Buffer->Vertices[id*4+1].Pos       = particle->Position-left;
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
