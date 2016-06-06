#ifndef CBEAMSCENENODE_H 
#define CBEAMSCENENODE_H 
#include <irrlicht.h> 
namespace irr 
{ 
    namespace scene 
    { 
        class CBeamNode : public ISceneNode 
        { 
        public: 
        private: 
            // The beam material. 
            video::SMaterial material; 
            video::SMaterial material2; 

            // Bounding Box 
            core::aabbox3d<f32> Box; 

            core::vector3df m_start; 
            core::vector3df m_end; 
            f32 m_thickness; 

        public: 

            CBeamNode( scene::ISceneNode* parent, scene::ISceneManager *mgr, s32 id, char* szBeam , char* szBeamFront ); 

            ~CBeamNode(void); 

            virtual void OnRegisterSceneNode(void); 

            virtual void OnAnimate(u32 timeMs); 

            virtual void render(void); 

            virtual const core::aabbox3d<f32>& getBoundingBox() const; 

            virtual u32 getMaterialCount() const; 

            virtual video::SMaterial& getMaterial(u32 i); 

            void setLine(core::vector3df start, core::vector3df end, f32 thickness); 
        }; 
    } 
} 
#endif 
