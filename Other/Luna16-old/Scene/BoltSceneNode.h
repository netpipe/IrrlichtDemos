#ifndef CBOLTSCENENODE_H 
#define CBOLTSCENENODE_H 

#include <irrlicht.h> 

namespace irr 
{ 
    namespace scene 
    { 
        class CBoltSceneNode : public ISceneNode 
        { 
        public: 
            CBoltSceneNode( scene::ISceneNode* parent, scene::ISceneManager *mgr, s32 id,char* szBeam ); 

            virtual ~CBoltSceneNode(void); 

            virtual void OnRegisterSceneNode(void); 

            virtual void OnAnimate(u32 timeMs); 			

            virtual void render(void); 

            virtual const core::aabbox3d<f32>& getBoundingBox() const; 

            virtual u32 getMaterialCount() const; 

            virtual video::SMaterial& getMaterial(u32 i); 

			void setLine(core::vector3df start, core::vector3df end, u32 updateTime = 300, u32 height = 10, u32 parts = 10, u32 bolts = 1, bool steddyend = true, f32 thick=5.0f , video::SColor color = video::SColor(255,255,0,0)); 
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

                    for (u32 i=0;i<points.size()-1;i++) 
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
			void drawbeam(Bolt beam);
            // The beam material. 
            video::SMaterial material; 
            // The bolts 
            core::array<Bolt> boltarray; 
            // Bounding Box 
            core::aabbox3d<f32> Box; 
            core::vector3df m_start; 
            core::vector3df m_end; 
            u32 m_parts; 
			f32 m_thickness;
        private: 
        }; 
    } 
} 
#endif // CBOLTSCENENODE_H 