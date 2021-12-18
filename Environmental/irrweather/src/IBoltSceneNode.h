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
#ifndef IBoltSceneNode_H
#define IBoltSceneNode_H

#include <irrTypes.h>
#include <vector3d.h>
#include <SMaterial.h>
#include <ISceneNode.h>
#include <IVideoDriver.h>

namespace irr
{
    namespace video
    {
    }

    namespace scene
    {
        class IBoltSceneNode : public ISceneNode
        {
        public:
            IBoltSceneNode( scene::ISceneNode* parent, scene::ISceneManager *mgr, s32 id);

            virtual ~IBoltSceneNode(void);

            void OnRegisterSceneNode(void);

            void OnAnimate(u32 timeMs);

            void render(void);

            const core::aabbox3d<f32>& getBoundingBox() const;

            u32 getMaterialCount() const;

            video::SMaterial& getMaterial(u32 i);

            void setLine(core::vector3df start, core::vector3df end, u32 updateTime = 300, u32 height = 10, u32 parts = 10, u32 bolts = 1, bool steddyend = true, video::SColor color = video::SColor(255,255,0,0));
        protected:
            struct Bolt
            {
                u32 m_height;
                u32 m_updateTime;
                bool m_end;
                Bolt(u32 height, u32 updateTime, bool end)
                {
                    m_height = height;
                    m_updateTime = updateTime;
                    time = 0;
                    startTime = 0;
                    m_end = end;
                }
                irr::core::vector3df normal;
                core::array<f32> delta;
                core::array<irr::core::vector3df> points;
                u32 time;
                u32 startTime;
                void Update(u32 timeMs)
                {
                    time = timeMs;
                    if (startTime == 0)
                        startTime = timeMs;
                }
                void draw(video::IVideoDriver* driver, video::SColor color)
                {
                   u32 h = m_height*2;

                    for (u32 i=0;i<points.size()-1;++i)
                    {
                        driver->draw3DLine(points[i]+normal*delta[i], points[i+1]+normal*delta[i+1], color);

                        if (time-startTime >= m_updateTime)
                        {
                            if (i == 0)
                            {
                                delta[i] = 0.0f;
                            }
                            else
                            {
                                delta[i] = rand()%h;
                                delta[i] -= (f32)m_height;
                            }
                        }
                    }

                    if (!m_end && time-startTime >= m_updateTime)
                    {
                       delta[delta.size()-1] = rand()%h;
                  delta[delta.size()-1] -= (f32)m_height;
                    }

                    if (time-startTime >= m_updateTime)
                  startTime = 0;
                }
            };

            // The beam material.
            video::SMaterial material;

            // The bolts
            core::array<Bolt> boltarray;

            // Bounding Box
            core::aabbox3d<f32> Box;

            core::vector3df m_start;
            core::vector3df m_end;
            u32 m_parts;
        private:
        };
    }
}
#endif // IBoltSceneNode_H
