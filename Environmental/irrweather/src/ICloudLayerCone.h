// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.
// Original code from Pazystamo's ATMOSphere

#ifndef __I_CLOUD_LAYER_CONE_H_INCLUDED__
#define __I_CLOUD_LAYER_CONE_H_INCLUDED__

#include <ISceneNode.h>
#include <irrTypes.h>
#include <S3DVertex.h>

namespace irr
{
    namespace scene
    {
        class ISceneManager;
    }
}


class ICloudLayerCone : public irr::scene::ISceneNode
{
    public:
        ICloudLayerCone(irr::video::ITexture* const tex,irr::scene::ISceneNode* const parent, irr::scene::ISceneManager* const mgr,
            irr::s32 faces, irr::s32 id);

        virtual ~ICloudLayerCone();

        void OnRegisterSceneNode();

        void OnAnimate(irr::u32 timeMs);

        void render();

        const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

        irr::u32 getMaterialCount() const;

        irr::video::SMaterial& getMaterial(irr::u32 i);

        /// Change the sky texture
        irr::video::SMaterial& setMaterial(irr::video::ITexture* const tex);


    private:
        irr::f32 ICloudLayerCone::findnoise2(irr::f32 x, irr::f32 y);

        irr::f32 interpolate1(irr::f32 a, irr::f32 b, irr::f32 x);

        irr::f32 noise(irr::f32 x, irr::f32 y);

        void generatePerlinData(irr::u32* data, irr::video::ITexture* const texture,
            irr::f32 zoom, irr::f32 persistence, irr::u16 octaves, const irr::video::SColor& baseColor, irr::f32 mixAmount);

        void generatePerlinTexture(irr::video::ITexture* const texture);

        void applyPerlinTexture();


        irr::core::aabbox3d<irr::f32> Box;
        irr::video::S3DVertex *Vertices;
        irr::video::SMaterial Material;
        irr::u16 *indices;
        irr::s32 vert;//number of vertices
        irr::s32 face;//number of faces
        irr::f32 posX,sizeX;
        irr::f32 uvX;
        irr::f32 random;
        bool FirstRun;

        irr::core::vector2df textureDimensions;
};

#endif // __I_CLOUD_LAYER_CONE_H_INCLUDED__
