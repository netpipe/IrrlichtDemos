/*
Copyright (C) 2012 Luca Siciliano

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __C_IRR_RIFT_INTERFACE__
#define __C_IRR_RIFT_INTERFACE__

#include <irrlicht.h>

class CIrrRiftInterface {
  public:
    struct SHMDDescriptor {
      irr::s32 m_iResolutionH,
               m_iResolutionV;
      irr::f32 m_fSizeH,
               m_fSizeV,
               m_fInterpupillaryDistance,
               m_fLensSeparationDistance,
               m_fEyeToScreenDistance,
               m_fDistortionK[4];
    };

  private:
    irr::video::IVideoDriver  *m_pDriver;
    irr::video::ITexture      *m_pRenderTexture;
    irr::scene::ISceneManager *m_pSmgr;

    // Start of Oculus Rift Code provided by Christian Keimel / bulletbyte.de

    irr::scene::ISceneNode    *m_pHeadX,
                              *m_pHeadY,
                              *m_pHeadZ,
                              *m_pYaw,
                              *m_pLeftEye,
                              *m_pRghtEye;

    CIrrRiftInterface::SHMDDescriptor m_cHMD;

    // End of Oculus Rift Code provided by Christian Keimel / bulletbyte.de

    irr::f32 m_fWorldScale,
             m_fEyeSeparation,
             m_fLensShift;

    bool m_bRiftAvailable;

    irr::core::matrix4 m_cProjectionLeft,
                       m_cProjectionRght;

    irr::core::rect<irr::s32> m_cViewportLeft,
                              m_cViewportRght;

    irr::scene::ICameraSceneNode *m_pCamera;

    irr::video::SMaterial m_cRenderMaterial;
    irr::video::S3DVertex m_cPlaneVertices[4];

    irr::u16 m_iPlaneIndices[6];

    irr::ITimer *m_pTimer;

    class RiftDistortionCallback: public irr::video::IShaderConstantSetCallBack {
      public:
        irr::f32 m_fScale       [2],
                 m_fScaleIn     [2],
                 m_fLensCenter  [2],
                 m_fHmdWarpParam[4];

        virtual void OnSetConstants(irr::video::IMaterialRendererServices *a_pServices, irr::s32 a_iUserData)
        {
          a_pServices->setPixelShaderConstant("scale"       , m_fScale       , 2);
          a_pServices->setPixelShaderConstant("scaleIn"     , m_fScaleIn     , 2);
          a_pServices->setPixelShaderConstant("lensCenter"  , m_fLensCenter  , 2);
          a_pServices->setPixelShaderConstant("hmdWarpParam", m_fHmdWarpParam, 4);
        }
    };
    RiftDistortionCallback m_cDistortionCB;

  public:
    CIrrRiftInterface(irr::IrrlichtDevice *device, CIrrRiftInterface::SHMDDescriptor HMD, irr::f32 worldScale = 1.0);
    ~CIrrRiftInterface();

    CIrrRiftInterface::SHMDDescriptor getHMD();
    void setHMD(CIrrRiftInterface::SHMDDescriptor HMD);

    irr::f32 getWorldScale();
    void setWorldScale(irr::f32 worldScale);

    void drawAll();
};
#endif
