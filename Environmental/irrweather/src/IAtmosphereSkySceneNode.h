// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.
// Original code from Pazystamo's ATMOSphere

#ifndef __IAtmosphereSkySceneNode_H_INCLUDED__
#define __IAtmosphereSkySceneNode_H_INCLUDED__

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


class IAtmosphereSkySceneNode : public irr::scene::ISceneNode
{
    public:
        IAtmosphereSkySceneNode(irr::video::ITexture* const tex,irr::scene::ISceneNode* const parent, irr::scene::ISceneManager* const mgr, irr::s32 faces, irr::s32 id);

        virtual ~IAtmosphereSkySceneNode();


        void OnRegisterSceneNode();

        void render();

        const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

        irr::u32 getMaterialCount() const;

        irr::video::SMaterial& getMaterial(irr::u32 i);

        /// Change the sky texture
        irr::video::SMaterial& setMaterial(irr::video::ITexture* const tex);

        /// Update UV mapping X coordinate
        void setuvX(irr::f64 v);

    private:
        irr::core::aabbox3d<irr::f32> Box;
        irr::video::S3DVertex *Vertices;
        irr::video::SMaterial Material;
        irr::u16 *indices;
        irr::s32 vert;//number of vertices
        irr::s32 face;//number of faces
        irr::f32 posX,sizeX;
        irr::f32 uvX;
};

#endif // __IAtmosphereSkySceneNode_H_INCLUDED__
