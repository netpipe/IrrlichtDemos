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
#include "CParticleSystem.h"

#include "CTrailParticleDrawer.h"
#include "CBillboardParticleDrawer.h"
#include "COrientedParticleDrawer.h"
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <IVideoDriver.h>

namespace irr
{
    namespace scene
    {
        IParticleSystem* createParticleSystem(ISceneNode* parent, ISceneManager* mgr, s32 id,
                        const core::vector3df& position,
                        const core::vector3df& rotation,
                        const core::vector3df& scale)
        {
            return new CParticleSystem(parent, mgr, id, position, rotation, scale);
        }

        CParticleSystem::CParticleSystem(ISceneNode* parent, ISceneManager* mgr, s32 id,
                        const core::vector3df& position,
                        const core::vector3df& rotation,
                        const core::vector3df& scale) : irr::scene::IParticleSystem(parent, mgr, id, position, rotation, scale)
        {
            //ctor
            LastTime = 0;
            diff = 0.f;
            Paused = false;
            UseOverrideMaterial = false;
            localParticles = false;
        }

        CParticleSystem::~CParticleSystem()
        {
            //dtor
            for (irr::u32 i=0;i<Drawer.size();i++)
            {
                Drawer[i]->drop();
            }
            Drawer.clear();
        }

        void CParticleSystem::update(irr::u32 timeMs, irr::f32 diff)
        {
            for (irr::u32 i=0;i<Drawer.size();i++)
            {
                Drawer[i]->doParticles(localParticles, SceneManager->getActiveCamera(), AbsoluteTransformation, timeMs, Paused ? 0.f : diff);
                if (i == 0)
                {
                    Box.reset(Drawer[i]->getBBox());
                }
                else
                    Box.addInternalBox(Drawer[i]->getBBox());
            }
        }

        void CParticleSystem::addDrawer(irr::scene::particle::IParticleDrawer* drawer)
        {
            if (drawer)
            {
                drawer->grab();
                Drawer.push_back(drawer);
            }
        }

        irr::scene::particle::IParticleDrawer* CParticleSystem::getDrawer(const irr::u32& id)
        {
            if (id < Drawer.size())
                return Drawer[id];
            return 0;
        }

        void CParticleSystem::setOverrideMaterial(const bool& use)
        {
            UseOverrideMaterial = use;
        }

        void CParticleSystem::setOverrideMaterial(const irr::video::SMaterial& material)
        {
            OverrideMaterial = material;
            UseOverrideMaterial = true;
        }

        irr::video::SMaterial& CParticleSystem::getOverrideMaterial(void)
        {
            return OverrideMaterial;
        }

        irr::scene::particle::IParticleDrawer* CParticleSystem::addTrailParticleDrawer(const bool& bindSpeedSize, const bool& useRotationAsVector, const irr::scene::particle::E_TRAILPARTICLE_DRAW_OPTION& drawOption)
        {
            irr::scene::particle::IParticleDrawer* drawer = new CTrailParticleDrawer(bindSpeedSize, useRotationAsVector, drawOption);
            addDrawer(drawer);
            drawer->drop();
            return drawer;
        }

        irr::scene::particle::IParticleDrawer* CParticleSystem::addParticleDrawer(void)
        {
            irr::scene::particle::IParticleDrawer* drawer = new CBillboardParticleDrawer();
            addDrawer(drawer);
            drawer->drop();
            return drawer;
        }

        irr::scene::particle::IParticleDrawer* CParticleSystem::addOrientedParticleDrawer(void)
        {
            irr::scene::particle::IParticleDrawer* drawer = new COrientedParticleDrawer();
            addDrawer(drawer);
            drawer->drop();
            return drawer;
        }

        void CParticleSystem::setLocalParticles(bool local)
        {
            localParticles = local;
        }

        bool CParticleSystem::isLocalParticles(void)
        {
            return localParticles;
        }

        void CParticleSystem::OnRegisterSceneNode(void)
        {
            if (isVisible())
            {
                SceneManager->registerNodeForRendering(this);
            }
            ISceneNode::OnRegisterSceneNode();
        }

        void CParticleSystem::OnAnimate(u32 timeMs)
        {
            if (LastTime == 0)
            {
                LastTime = timeMs;
                return;
            }
            diff = timeMs-LastTime;
            diff /= 1000.f;
            ISceneNode::OnAnimate(timeMs);
            LastTime = timeMs;
        }

        void CParticleSystem::render(void)
        {
            update(LastTime, diff);

            if (UseOverrideMaterial)
                SceneManager->getVideoDriver()->setMaterial(OverrideMaterial);

            irr::core::matrix4 trans = irr::core::IdentityMatrix;
            if (localParticles)
            {
                trans.setTranslation(AbsoluteTransformation.getTranslation());
            }

            SceneManager->getVideoDriver()->setTransform(irr::video::ETS_WORLD, trans);

            for (u32 i=0;i<Drawer.size();++i)
            {
                Drawer[i]->drawParticles(trans, SceneManager->getVideoDriver(), UseOverrideMaterial);
            }
        }

        const core::aabbox3d<f32>& CParticleSystem::getBoundingBox() const
        {
            return Box;
        }

        video::SMaterial& CParticleSystem::getMaterial(u32 num)
        {
            if (UseOverrideMaterial)
                return OverrideMaterial;

            if (num < Drawer.size())
                return Drawer[num]->getMaterial();
            return video::IdentityMaterial;
        }

        u32 CParticleSystem::getMaterialCount() const
        {
            if (UseOverrideMaterial)
                return 1;

            return Drawer.size();
        }
    }
}
