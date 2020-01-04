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
#include "CParticleDrawer.h"
#include "CParticleEmitter.h"
#include <IVideoDriver.h>
#include <ICameraSceneNode.h>
#include <SViewFrustum.h>

irr::core::list<irr::scene::SMeshBuffer*>* CParticleDrawer::BufferPool = 0;
//irr::core::list<Particle*> CParticleDrawer::ParticlePool;

CParticleDrawer::CParticleDrawer()
{
    //ctor
    Emitter = 0;
    ParticleCount = 0;

    if (!BufferPool)
        BufferPool = new irr::core::list<irr::scene::SMeshBuffer*>();

    if (BufferPool->size() > 0)
    {
        irr::core::list<irr::scene::SMeshBuffer*>::Iterator it = BufferPool->begin();
        Buffer = *it;
        BufferPool->erase(it);
    }
    else
    {
        Buffer = new irr::scene::SMeshBuffer();
        //printf("create MeshBuffer\n");
    }

    //Buffer->setHardwareMappingHint(irr::scene::EHM_NEVER, irr::scene::EBT_VERTEX_AND_INDEX);
    Buffer->setHardwareMappingHint(irr::scene::EHM_STATIC, irr::scene::EBT_INDEX);
    Buffer->setHardwareMappingHint(irr::scene::EHM_STREAM, irr::scene::EBT_VERTEX);
}

CParticleDrawer::~CParticleDrawer()
{
    //dtor
    irr::core::list<irr::scene::particle::Particle*>::Iterator it = Particles.begin();
    while (it != Particles.end())
    {
        (*it)->drop();
        //ParticlePool.push_back(*it);
        ++it;
    }
    Particles.clear();

    it = ParticlePool.begin();
    while (it != ParticlePool.end())
    {
        (*it)->drop();
        ++it;
    }
    ParticlePool.clear();

    irr::core::list<irr::scene::particle::IParticleAffector*>::Iterator at = Affectors.begin();
    while (at != Affectors.end())
    {
        (*at)->drop();
        ++at;
    }
    Affectors.clear();

    BufferPool->push_back(Buffer);
    Buffer = 0;
}

irr::scene::particle::IParticleEmitter* CParticleDrawer::addStandardEmitter(const irr::core::vector3df& rotation, const irr::core::vector3df& rotationspeed, const irr::core::vector3df& speed, const irr::core::vector3di& maxAngle, const irr::u32& minPpS, const irr::u32& maxPpS, const irr::u32& maxEmitt, const irr::u32& minlifetime, const irr::u32& maxlifetime, const irr::core::vector2df& minsize, const irr::core::vector2df& maxsize, const irr::core::vector2df& sizespeed, const irr::video::SColor& minColor, const irr::video::SColor& maxColor)
{
    irr::scene::particle::IParticleEmitter* emitter = new CParticleEmitter(rotation, rotationspeed, speed, maxAngle, minPpS, maxPpS, maxEmitt, minlifetime, maxlifetime, minsize, maxsize, sizespeed, minColor, maxColor);
    setEmitter(emitter);
    emitter->drop();
    return emitter;
}

void CParticleDrawer::drawParticles(const irr::core::matrix4& transform, irr::video::IVideoDriver* driver, bool useOneMaterial)
{
    if (ParticleCount == 0)
        return;

    if (!useOneMaterial)
        driver->setMaterial(getMaterial());

    driver->drawVertexPrimitiveList(Buffer->getVertices(), ParticleCount*4,
                                    Buffer->getIndices(), ParticleCount*2, irr::video::EVT_STANDARD, irr::scene::EPT_TRIANGLES, Buffer->getIndexType());
}

irr::video::SMaterial& CParticleDrawer::getMaterial(void)
{
    return Material;
}

void CParticleDrawer::addUVCoords(const irr::scene::particle::SParticleUV& coord)
{
    UVCoords.push_back(coord);
}

void CParticleDrawer::doParticles(bool localParticles, const irr::scene::ICameraSceneNode* camera, const irr::core::matrix4& transform, irr::u32 timeMs, irr::f32 diff)
{
    ///Emitt new particles
    if (Emitter && diff != 0.f)
    {
        irr::core::matrix4 trans = transform;
        if (localParticles)
        {
            trans = irr::core::IdentityMatrix;
        }

        bool d = true;
        while(d)
        {
            Emitter->emitt(trans, Particles, ParticlePool, timeMs, diff, UVCoords);
            d = Emitter->emittAll();
        }
    }

    rellocateBuffer();

    irr::core::matrix4 viewMatrix = camera->getViewFrustum()->getTransform( irr::video::ETS_VIEW );
    irr::core::vector3df campos = camera->getAbsolutePosition();
    irr::scene::particle::Particle tmpParticle;
    if (localParticles)
        campos -= transform.getTranslation();

    ///Remove Dead Particles
    irr::core::list<irr::scene::particle::Particle*>::Iterator it = Particles.begin();
    irr::u32 idx = 0;
    while (it != Particles.end())
    {
        if ((*it)->TimeLived >= (*it)->LifeTime && diff != 0.f)
        {
            ParticlePool.push_back(*it);

            it = Particles.erase(it);
        }
        else
        {
            //Animate remaining
            irr::scene::particle::Particle* particle = *it;

            if (particle->CreateTimeMs != timeMs)
            {
                particle->TimeLived += diff;

                irr::core::list<irr::scene::particle::IParticleAffector*>::Iterator at = Affectors.begin();
                while (at != Affectors.end() && diff != 0.f)
                {
                    (*at)->affect(particle, timeMs, diff);
                    ++at;
                }

                particle->Position += particle->Speed*diff;
                particle->Size += particle->SizeSpeed*diff;
                particle->Rotation += particle->RotationSpeed*diff;
            }

            tmpParticle = *particle;
            if (localParticles)
            {
                transform.rotateVect(tmpParticle.Position);
            }
            irr::core::vector3df view = tmpParticle.Position-campos;
            view.normalize();
            createParticle(idx, &tmpParticle, view, viewMatrix);

            if (idx == 0)
            {
                if (!localParticles)
                    Box.reset(tmpParticle.Position-transform.getTranslation());
                else
                    Box.reset(tmpParticle.Position);
            }
            else
            {
                if (!localParticles)
                    Box.addInternalPoint(tmpParticle.Position-transform.getTranslation());
                else
                    Box.addInternalPoint(tmpParticle.Position);
            }

            idx++;
            ++it;
        }
    }

    ParticleCount = idx;
    if (idx > 0)
        Buffer->setDirty(irr::scene::EBT_VERTEX);
}

void CParticleDrawer::setEmitter(irr::scene::particle::IParticleEmitter* emitter)
{
    if (emitter)
    {
        emitter->grab();
        if (emitter->getMaxParticleEmitt())
            rellocateBuffer(emitter->getMaxParticleEmitt());
        else
            rellocateBuffer(emitter->getMaxLifeTime()/1000.f*emitter->getMaxParticlesPerSecond());
    }
    if (Emitter)
        Emitter->drop();
    Emitter = emitter;
}

irr::scene::particle::IParticleEmitter* CParticleDrawer::getEmitter(void)
{
    return Emitter;
}

void CParticleDrawer::addAffector(irr::scene::particle::IParticleAffector* affector)
{
    if (affector)
    {
        affector->grab();
        Affectors.push_back(affector);
    }
}

void CParticleDrawer::rellocateBuffer(irr::u32 particleCount)
{
    irr::u32 size = particleCount;
    if (size == 0)
        size = Particles.size();
    while (Particles.size()+ParticlePool.size() < size)
    {
        ParticlePool.push_back(new irr::scene::particle::Particle());
    }
    bool bufferDirty = false;
    while (Buffer->Vertices.size() < size*4 || Buffer->Indices.size() < size*6)
    {
        bufferDirty = true;

        Buffer->Vertices.push_back(irr::video::S3DVertex());
        Buffer->Vertices.push_back(irr::video::S3DVertex());
        Buffer->Vertices.push_back(irr::video::S3DVertex());
        Buffer->Vertices.push_back(irr::video::S3DVertex());

        Buffer->Indices.push_back(Buffer->Vertices.size()-4);
        Buffer->Indices.push_back(Buffer->Vertices.size()-3);
        Buffer->Indices.push_back(Buffer->Vertices.size()-2);

        Buffer->Indices.push_back(Buffer->Vertices.size()-2);
        Buffer->Indices.push_back(Buffer->Vertices.size()-3);
        Buffer->Indices.push_back(Buffer->Vertices.size()-1);
    }
    if (bufferDirty)
    {
        Buffer->setDirty(irr::scene::EBT_VERTEX_AND_INDEX);
        /*
        printf("SetIndexAndVertexData Dirty\n");
        if (rand()%2 == 0)
            printf("\n");
        */
    }
}

const irr::core::aabbox3df& CParticleDrawer::getBBox(void) const
{
    return Box;
}
