/*
  Copyright (C) 2010 Daniel Sudmann

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
#include "IBoltSceneNode.h"
#include <ISceneManager.h>

namespace irr
{
    namespace scene
    {
        IBoltSceneNode::IBoltSceneNode( scene::ISceneNode* parent, scene::ISceneManager *mgr, s32 id) : ISceneNode( parent, mgr, id )
        {
            //ctor
            material.Wireframe = false;
            material.Lighting = false;
            AutomaticCullingState = EAC_FRUSTUM_SPHERE;
        }

        IBoltSceneNode::~IBoltSceneNode()
        {
            //dtor
        }

        void IBoltSceneNode::OnRegisterSceneNode(void)
        {
            if ( IsVisible )
            {
                SceneManager->registerNodeForRendering( this );
            }
        }

        void IBoltSceneNode::OnAnimate(u32 timeMs)
        {

            if ( IsVisible )
            {
                for (u32 i=0;i<boltarray.size();++i)
                {
                    boltarray[i].Update(timeMs);
                }
            }

            ISceneNode::OnAnimate(timeMs);
        }

        void IBoltSceneNode::setLine(core::vector3df start, core::vector3df end, u32 updateTime, u32 height, u32 parts, u32 bolts, bool steddyend, video::SColor color)
        {
           setPosition(start);

            srand(parts);
            boltarray.clear();

            m_start = core::vector3df(0,0,0);
            m_end = end-start;
            m_parts = parts;
            material.DiffuseColor = color;

            core::vector3df dir = end-start;
            dir/=(f32)parts;

            for (u32 i=0;i<bolts;++i)
            {
                //printf("add Bolt\n");
                boltarray.push_back(Bolt(height, updateTime, steddyend));

                for (u32 a=0;a<parts;++a)
                {
                    //printf("add Part[%i] ", a);
                    boltarray[i].points.push_back(m_start+dir*a);
                    if (a == parts-1 || a == 0)
                        boltarray[i].delta.push_back(0.0f);
                    else
                    {
                        u32 h = height*2;
                        f32 d = (rand()%h);
                        d-=(f32)height;
                        //printf("Delta: %f", d);
                        boltarray[i].delta.push_back(d);
                    }
                    //printf("\n");
                }

                u32 vec = rand()%2;
            //printf("vec: %i\n", vec);

            u32 cord = rand()%10+1;

                if (dir.X != 0)
                {
                    if (vec == 0)
                        boltarray[i].normal = core::vector3df(-dir.Y/dir.X*cord,cord,0);
                    else
                        boltarray[i].normal = core::vector3df(-dir.Z/dir.X*cord,0,cord);
                }
                else if (dir.Y != 0)
                {
                    if (vec == 0)
                        boltarray[i].normal = core::vector3df(cord,-dir.X/dir.Y*cord,0);
                    else
                        boltarray[i].normal = core::vector3df(0,-dir.Z/dir.Y*cord,cord);
                }
                else if (dir.Z != 0)
                {
                    if (vec == 0)
                        boltarray[i].normal = core::vector3df(cord,0,-dir.X/dir.Z*cord);
                    else
                        boltarray[i].normal = core::vector3df(0,cord,-dir.Y/dir.Z*cord);
                }
                boltarray[i].normal.normalize();
            }
        }

        void IBoltSceneNode::render(void)
        {
            video::IVideoDriver* driver = SceneManager->getVideoDriver();
            driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
            driver->setMaterial(material);

            for (u32 i=0;i<boltarray.size();++i)
            {
                boltarray[i].draw(driver, material.DiffuseColor);
            }
        }

        const core::aabbox3d<f32>& IBoltSceneNode::getBoundingBox() const
        {
            return Box;
        }

        u32 IBoltSceneNode::getMaterialCount() const
        {
            return 1;
        }

        video::SMaterial& IBoltSceneNode::getMaterial(u32 i)
        {
            return material;
        }
    }
}
