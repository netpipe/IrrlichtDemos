#include <irrlicht.h>
//! the licence is on EntityManager.h

namespace GameShader
{
    class toonShader : public irr::video::IShaderConstantSetCallBack
    {
        public :
            toonShader(irr::IrrlichtDevice*dev);
            irr::video::E_MATERIAL_TYPE getMaterial(){ return material; }

            virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
            {
                irr::core::matrix4 world = device->getVideoDriver()->getTransform(irr::video::ETS_WORLD);

                irr::core::matrix4 invWorld;
                world.getInverse(invWorld);

                irr::core::vector3df lightPosOS;
                invWorld.transformVect(lightPosOS, lightPosition);
                services->setVertexShaderConstant("LightDir", &lightPosOS.X, 3);

                irr::core::vector3df camPosOS;
                invWorld.transformVect(camPosOS, camera->getPosition());
                services->setVertexShaderConstant("CamPos", &camPosOS.X, 3);

                irr::core::matrix4 wvp = device->getVideoDriver()->getTransform(irr::video::ETS_PROJECTION);
                wvp *= device->getVideoDriver()->getTransform(irr::video::ETS_VIEW);
                wvp *= world;

                services->setVertexShaderConstant("mWorldViewProj", wvp.pointer(), 16);
            }
        protected :
            irr::IrrlichtDevice *device;
            irr::scene::ICameraSceneNode *camera;
            virtual void sourceShader();
            irr::video::E_MATERIAL_TYPE material;
        private:
            irr::core::stringc ToonShaderGLSL;
            irr::core::stringc ToonShaderHLSL;
            irr::core::vector3df lightPosition;
    };
}
