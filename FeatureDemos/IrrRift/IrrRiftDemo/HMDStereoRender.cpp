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

#include "HMDStereoRender.h"

#include <iostream>
#include <cassert>

#include <IrrRiftDll.h>

static const char* vertexShader =
"void main() {"
"  gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);"
"  gl_TexCoord[0].st = gl_MultiTexCoord0;"
"}";

static const char *fragShader =
"uniform vec2 scale;"
"uniform vec2 scaleIn;"
"uniform vec2 lensCenter;"
"uniform vec4 hmdWarpParam;"
"uniform sampler2D texid;"
"void main()"
"{"
"  vec2 uv = (gl_TexCoord[0].st*2.0)-1.0;" // range from [0,1] to [-1,1]
"  vec2 theta = (uv-lensCenter)*scaleIn;"
"  float rSq = theta.x*theta.x + theta.y*theta.y;"
"  vec2 rvector = theta*(hmdWarpParam.x + hmdWarpParam.y*rSq + hmdWarpParam.z*rSq*rSq + hmdWarpParam.w*rSq*rSq*rSq);"
"  vec2 tc = (lensCenter + scale * rvector);"
"  tc = (tc+1.0)/2.0;" // range from [-1,1] to [0,1]
"  if (any(bvec2(clamp(tc, vec2(0.0,0.0), vec2(1.0,1.0))-tc)))"
"    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);"
"  else"
"    gl_FragColor = texture2D(texid, tc);"
"}";

CIrrRiftInterface::CIrrRiftInterface(irr::IrrlichtDevice *a_pDevice, CIrrRiftInterface::SHMDDescriptor a_cHMD, irr::f32 a_fWorldScale) : m_pRenderTexture(NULL), m_fWorldScale(a_fWorldScale) {
  m_pSmgr   = a_pDevice->getSceneManager();
  m_pDriver = a_pDevice->getVideoDriver();
  m_pTimer  = a_pDevice->getTimer();

  // Create perspectiva camera used for rendering
  m_pCamera = m_pSmgr->addCameraSceneNode();

  // Init shader parameters
  m_cDistortionCB.m_fScale     [0] = 1.0f; m_cDistortionCB.m_fScale     [1] = 1.0f;
  m_cDistortionCB.m_fScaleIn   [0] = 1.0f; m_cDistortionCB.m_fScaleIn   [1] = 1.0f;
  m_cDistortionCB.m_fLensCenter[0] = 0.0f; m_cDistortionCB.m_fLensCenter[1] = 0.0f;

  m_cDistortionCB.m_fHmdWarpParam[0] = 1.0f;
  m_cDistortionCB.m_fHmdWarpParam[1] = 0.0f;
  m_cDistortionCB.m_fHmdWarpParam[2] = 0.0f;
  m_cDistortionCB.m_fHmdWarpParam[3] = 0.0f;

  // Plane
  m_cPlaneVertices[0] = irr::video::S3DVertex(-1.0f, -1.0f, 1.0f,1,1,0, irr::video::SColor(255,0,255,255), 0.0f, 0.0f);
  m_cPlaneVertices[1] = irr::video::S3DVertex(-1.0f,  1.0f, 1.0f,1,1,0, irr::video::SColor(255,0,255,255), 0.0f, 1.0f);
  m_cPlaneVertices[2] = irr::video::S3DVertex( 1.0f,  1.0f, 1.0f,1,1,0, irr::video::SColor(255,0,255,255), 1.0f, 1.0f);
  m_cPlaneVertices[3] = irr::video::S3DVertex( 1.0f, -1.0f, 1.0f,1,1,0, irr::video::SColor(255,0,255,255), 1.0f, 0.0f);

  m_iPlaneIndices[0] = 0;
  m_iPlaneIndices[1] = 1;
  m_iPlaneIndices[2] = 2;
  m_iPlaneIndices[3] = 0;
  m_iPlaneIndices[4] = 2;
  m_iPlaneIndices[5] = 3;

  // Create shader material
  m_cRenderMaterial.Wireframe = false;
  m_cRenderMaterial.Lighting = false;
  m_cRenderMaterial.TextureLayer[0].TextureWrapU = irr::video::ETC_CLAMP;
  m_cRenderMaterial.TextureLayer[0].TextureWrapV = irr::video::ETC_CLAMP;

  irr::video::IGPUProgrammingServices* gpu = m_pDriver->getGPUProgrammingServices();
  m_cRenderMaterial.MaterialType=(irr::video::E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(vertexShader, "main", irr::video::EVST_VS_3_0, fragShader, "main", irr::video::EPST_PS_3_0, &m_cDistortionCB);

  // Start of Oculus Rift Code provided by Christian Keimel / bulletbyte.de

  m_bRiftAvailable = IrrRift_install();

  m_pYaw   = m_pSmgr->addEmptySceneNode(0, 0);
  m_pHeadX = m_pSmgr->addEmptySceneNode(m_pYaw  , 0);
  m_pHeadY = m_pSmgr->addEmptySceneNode(m_pHeadX, 0);
  m_pHeadZ = m_pSmgr->addEmptySceneNode(m_pHeadY, 0);

  m_pLeftEye = m_pSmgr->addEmptySceneNode(m_pHeadZ, 0); m_pLeftEye->setPosition(irr::core::vector3df(-0.25, 0, 0));
  m_pRghtEye = m_pSmgr->addEmptySceneNode(m_pHeadZ, 0); m_pRghtEye->setPosition(irr::core::vector3df( 0.25, 0, 0));

  setHMD(a_cHMD);
  // End of Oculus Rift Code provided by Christian Keimel / bulletbyte.de
}

CIrrRiftInterface::~CIrrRiftInterface() {
  IrrRift_destall();
}

void CIrrRiftInterface::setHMD(SHMDDescriptor a_cHMD) {
  m_cHMD = a_cHMD;

  // Compute aspect ratio and FOV
  irr::f32 l_fAspect    = m_cHMD.m_iResolutionH / (2.0f * m_cHMD.m_iResolutionV),
           l_fR         = -1.0f - (4.0f * (m_cHMD.m_fSizeH / 4.0f - m_cHMD.m_fLensSeparationDistance / 2.0f) / m_cHMD.m_fSizeH),
           l_fDistScale = (m_cHMD.m_fDistortionK[0] + m_cHMD.m_fDistortionK[1] * pow(l_fR,2) + m_cHMD.m_fDistortionK[2] * pow(l_fR,4) + m_cHMD.m_fDistortionK[3] * pow(l_fR,6)),
           l_fFov       = 2.0f * atan2(m_cHMD.m_fSizeV * l_fDistScale, 2.0f * m_cHMD.m_fEyeToScreenDistance),
           l_fH         = 4 * (m_cHMD.m_fSizeH / 4 - m_cHMD.m_fInterpupillaryDistance/2) / m_cHMD.m_fSizeH;

  // Compute camera projection matrices
  irr::core::matrix4 l_cCenterProjection = irr::core::matrix4().buildProjectionMatrixPerspectiveFovLH (l_fFov, l_fAspect, 1, 10000);

  m_cProjectionLeft = irr::core::matrix4().setTranslation(irr::core::vector3df( l_fH, 0.0, 0.0)) * l_cCenterProjection;
  m_cProjectionRght = irr::core::matrix4().setTranslation(irr::core::vector3df(-l_fH, 0.0, 0.0)) * l_cCenterProjection;

  // Compute camera offset
  m_fEyeSeparation = m_fWorldScale * m_cHMD.m_fInterpupillaryDistance/2.0f;

  m_pLeftEye->setPosition(irr::core::vector3df( m_fEyeSeparation, 0.0f, 0.0f));
  m_pRghtEye->setPosition(irr::core::vector3df(-m_fEyeSeparation, 0.0f, 0.0f));

  // Compute Viewport
  m_cViewportLeft = irr::core::rect<irr::s32>(                        0, 0, m_cHMD.m_iResolutionH / 2, m_cHMD.m_iResolutionV);
  m_cViewportRght = irr::core::rect<irr::s32>(m_cHMD.m_iResolutionH / 2, 0, m_cHMD.m_iResolutionH    , m_cHMD.m_iResolutionV);

  // Distortion shader parameters
  m_fLensShift = 4.0f * (m_cHMD.m_fSizeH / 4.0f - m_cHMD.m_fLensSeparationDistance/2.0f) / m_cHMD.m_fSizeH;

  m_cDistortionCB.m_fScale[0] = 1.0f            / l_fDistScale;
  m_cDistortionCB.m_fScale[1] = 1.0f * l_fAspect/ l_fDistScale;

  m_cDistortionCB.m_fScaleIn[0] = 1.0f;
  m_cDistortionCB.m_fScaleIn[1] = 1.0f / l_fAspect;

  m_cDistortionCB.m_fHmdWarpParam[0] = m_cHMD.m_fDistortionK[0];
  m_cDistortionCB.m_fHmdWarpParam[1] = m_cHMD.m_fDistortionK[1];
  m_cDistortionCB.m_fHmdWarpParam[2] = m_cHMD.m_fDistortionK[2];
  m_cDistortionCB.m_fHmdWarpParam[3] = m_cHMD.m_fDistortionK[3];

  // Create render target
  if (m_pDriver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
  {
    if (m_pRenderTexture != NULL) m_pRenderTexture->drop();
    m_pRenderTexture = m_pDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>((irr::u32)(m_cHMD.m_iResolutionH * l_fDistScale / 2.0f), (irr::u32)(m_cHMD.m_iResolutionV * l_fDistScale)));
    m_cRenderMaterial.setTexture(0, m_pRenderTexture);
  }
  else {
    // Render to target not supported
    assert(0);
  }
}


void CIrrRiftInterface::drawAll() {
  irr::scene::ICameraSceneNode *l_pCamera = m_pSmgr->getActiveCamera();
  l_pCamera->OnAnimate(m_pTimer->getTime());

  // Start of Oculus Rift Code provided by Christian Keimel / bulletbyte.de

  irr::core::matrix4 l_cMat;
  if (m_bRiftAvailable) {
    irr::core::vector3df v;

    IrrRift_poll(v.X, v.Y, v.Z);

    v.X *=  irr::core::RADTODEG;
    v.Y *= -irr::core::RADTODEG;
    v.Z *= -irr::core::RADTODEG;

    m_pYaw  ->setRotation(l_pCamera->getRotation()); // irr::core::vector3df(        0, l_pCamera->getRotation().Y,   0));
    m_pHeadY->setRotation(irr::core::vector3df(        0, v.Y                       ,   0));
    m_pHeadX->setRotation(irr::core::vector3df(      v.X,   0                       ,   0));
    m_pHeadZ->setRotation(irr::core::vector3df(        0,   0                       , v.Z));

    l_cMat.setRotationDegrees(m_pHeadZ->getAbsoluteTransformation().getRotationDegrees());
  }

  irr::core::vector3df vFore = irr::core::vector3df(0, 0, -1),
                       vUp   = irr::core::vector3df(0, 1,  0);

  l_cMat.transformVect(vFore);
  l_cMat.transformVect(vUp  );

  l_pCamera->setTarget  (l_pCamera->getPosition() + vFore);
  l_pCamera->setUpVector(                           vUp  );

  // Render Left
  m_pDriver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));

  // End of Oculus Rift Code provided by Christian Keimel / bulletbyte.de

  m_pCamera->setProjectionMatrix(m_cProjectionLeft);

  irr::core::vector3df l_vR = l_pCamera->getRotation();
  irr::core::vector3df l_vTx(0, 0, 1);//-m_fEyeSeparation, 0.0, 0.0);
  l_vTx.rotateXZBy(-l_vR.Y);
  l_vTx.rotateYZBy(-l_vR.X);
  l_vTx.rotateXYBy(-l_vR.Z);

  m_pCamera->setPosition(l_pCamera->getPosition() + m_pLeftEye->getAbsolutePosition());
  m_pCamera->setTarget  (l_pCamera->getTarget  () + m_pLeftEye->getAbsolutePosition());//getTarget  () + l_vTx);
  m_pCamera->setUpVector(l_pCamera->getUpVector());

  m_pSmgr->setActiveCamera(m_pCamera);
  m_pSmgr->drawAll();

  m_pDriver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));
  m_pDriver->setViewPort(m_cViewportLeft);

  m_cDistortionCB.m_fLensCenter[0] = m_fLensShift;

  m_pDriver->setMaterial(m_cRenderMaterial);
  m_pDriver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

  // Render Right
  m_pDriver->setRenderTarget(m_pRenderTexture, true, true, irr::video::SColor(0,0,0,0));
  m_pCamera->setProjectionMatrix(m_cProjectionRght);

  irr::core::vector3df l_vTxt2(0, 0, 1);//m_fEyeSeparation, 0.0, 0.0);
  l_vTxt2.rotateXZBy(-l_vR.Y);
  l_vTxt2.rotateYZBy(-l_vR.X);
  l_vTxt2.rotateXYBy(-l_vR.Z);

  m_pCamera->setPosition(l_pCamera->getPosition() + m_pRghtEye->getAbsolutePosition()); //l_vTxt2);
  m_pCamera->setTarget  (l_pCamera->getTarget  () + m_pRghtEye->getAbsolutePosition());
  m_pCamera->setUpVector(l_pCamera->getUpVector());

  m_pSmgr->drawAll();

  m_pDriver->setRenderTarget(0, false, false, irr::video::SColor(0,100,100,100));
  m_pDriver->setViewPort(m_cViewportRght);

  m_cDistortionCB.m_fLensCenter[0] = -m_fLensShift;

  m_pDriver->setMaterial(m_cRenderMaterial);
  m_pDriver->drawIndexedTriangleList(m_cPlaneVertices, 4, m_iPlaneIndices, 2);

  m_pSmgr->setActiveCamera(l_pCamera);
}
