// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.
// Original code from Pazystamo's ATMOSphere

#ifndef __ICompoundCelestialBodySceneNode_H_INCLUDED__
#define __ICompoundCelestialBodySceneNode_H_INCLUDED__

#include <irrlicht.h>

//almost all code from CBillboardSceneNode
class ICompoundCelestialBodySceneNode : public irr::scene::ISceneNode
{
    public:

        ICompoundCelestialBodySceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr,
            irr::s32 id,irr::core::vector3df position,
            const irr::core::dimension2d<irr::f32>& sunSize=irr::core::dimension2d<irr::f32>(105,105),
            const irr::core::dimension2d<irr::f32>& moonSize=irr::core::dimension2d<irr::f32>(40,40));

        //virtual void setSize(const core::dimension2d<f32>& size);

        void OnRegisterSceneNode();

        void render();

        /// sets the size of the billboard
        //void setSize(const irr::core::dimension2d<irr::f32>& size) {};

        void setSunSize(const irr::core::dimension2d<irr::f32>& size) { SunSize = size; };

        void setMoonSize(const irr::core::dimension2d<irr::f32>& size) { MoonSize = size; };



        const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

        irr::u32 getMaterialCount() const;

        irr::video::SMaterial& getMaterial(irr::u32 i);

        // Change sky texture
        irr::video::SMaterial& setMaterial(irr::video::ITexture* tex);

        // Update UV mapping X coordinate
        //const irr::core::dimension2d<irr::f32>& getSize();

        const irr::core::dimension2d<irr::f32>& getSunSize() const { return SunSize; };

        const irr::core::dimension2d<irr::f32>& getMoonSize() const { return MoonSize; };

    private:
        irr::core::aabbox3d<irr::f32> BBox;
        //irr::core::dimension2d<irr::f32> Size;
        irr::video::S3DVertex Vertices[4];
        irr::video::SMaterial Material;
        irr::video::SMaterial MoonMaterial;
        irr::u16 indices[6];
        irr::video::S3DVertex vertices[4];
        irr::core::vector3df MoonPosition;
        irr::core::dimension2d<irr::f32> SunSize;
        irr::core::dimension2d<irr::f32> MoonSize;
};

#endif // __ICompoundCelestialBodySceneNode_H_INCLUDED__
