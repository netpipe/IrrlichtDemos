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

#include <irrlicht.h>
#include "HMDStereoRender.h"

class CRunner : public irr::IEventReceiver {
  private:
    irr::IrrlichtDevice *m_pDevice;

    irr::scene::ISceneManager *m_pSmgr;
    irr::video::IVideoDriver  *m_pDrv;
    irr::gui::IGUIEnvironment *m_pGui;

    irr::gui::ICursorControl *m_pCursor;

    irr::s32 m_iMove,
             m_iStrafe;

    CIrrRiftInterface *m_pRenderer;

  public:
    CRunner(irr::u32 a_iScreenWidth, irr::u32 a_iScreenHeight, bool a_bFullscreen) {
      m_pDevice = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(a_iScreenWidth, a_iScreenHeight), 16, a_bFullscreen, false, false, this);

      m_pSmgr = m_pDevice->getSceneManager  ();
      m_pDrv  = m_pDevice->getVideoDriver   ();
      m_pGui  = m_pDevice->getGUIEnvironment();

      m_pCursor = m_pDevice->getCursorControl();

      CIrrRiftInterface::SHMDDescriptor l_cHMD;
      // Parameters from the Oculus Rift DK1
      l_cHMD.m_iResolutionH = 1280;
      l_cHMD.m_iResolutionV = 800;
      l_cHMD.m_fSizeH = 0.14976f;
      l_cHMD.m_fSizeV= 0.0936f;
      l_cHMD.m_fInterpupillaryDistance = 0.064f;
      l_cHMD.m_fLensSeparationDistance = 0.064f;
      l_cHMD.m_fEyeToScreenDistance = 0.041f;
      l_cHMD.m_fDistortionK[0] = 1.0f;
      l_cHMD.m_fDistortionK[1] = 0.22f;
      l_cHMD.m_fDistortionK[2] = 0.24f;
      l_cHMD.m_fDistortionK[3] = 0.0f;

      m_pRenderer = new CIrrRiftInterface(m_pDevice, l_cHMD, 10);

      // Create world
      m_pSmgr->addCameraSceneNode();

      // load the quake map
      printf("%i\n", (int)m_pDevice->getFileSystem()->addFileArchive("../data/map-20kdm2.pk3"));
      irr::scene::IAnimatedMesh *l_pMesh      = m_pSmgr->getMesh("20kdm2.bsp");
      irr::scene::ISceneNode    *l_pLevelNode = m_pSmgr->addOctreeSceneNode(l_pMesh->getMesh(0), 0, -1, 128);
      l_pLevelNode->setPosition(irr::core::vector3df(-1350,-90,-1400));

      // load a faerie
      irr::scene::IAnimatedMesh          *l_pFaerie     = m_pSmgr->getMesh("../data/faerie.md2");
      irr::scene::IAnimatedMeshSceneNode *l_pFaerieNode = m_pSmgr->addAnimatedMeshSceneNode(l_pFaerie);
      l_pFaerieNode->setMaterialTexture(0, m_pDrv->getTexture("../data/faerie2.bmp"));
      l_pFaerieNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
      l_pFaerieNode->setPosition(irr::core::vector3df(40,190,-1030));
      l_pFaerieNode->setRotation(irr::core::vector3df(0,-90,0));
      l_pFaerieNode->setMD2Animation(irr::scene::EMAT_SALUTE);

      // load a dwarf
      irr::scene::IAnimatedMesh          *l_pDwarf     = m_pSmgr->getMesh("../data/dwarf.x");
      irr::scene::IAnimatedMeshSceneNode *l_pDwarfNode = m_pSmgr->addAnimatedMeshSceneNode(l_pDwarf);
      l_pDwarfNode->setPosition(irr::core::vector3df(40,-25,20));

      m_pDevice->getCursorControl()->setVisible(false);

      m_iMove   = 0;
      m_iStrafe = 0;
    }

    void run() {
      irr::scene::ICameraSceneNode *l_pCam = m_pSmgr->getActiveCamera();
      irr::ITimer *l_pTimer = m_pDevice->getTimer();

      irr::u32 l_iLastTime = l_pTimer->getTime();
      irr::f32 fRot = 0.0f;

      // Render loop
      while(m_pDevice->run()){
        irr::u32 l_iThisTime   = l_pTimer->getTime(),
                 l_iDiffTime   = l_iThisTime - l_iLastTime;
        irr::s32 l_iMouseX     = 640 - m_pCursor->getPosition().X;
        irr::f32 l_fMoveFactor = ((irr::f32)l_iDiffTime) / 10.0f;

        m_pCursor->setPosition(640, 400);

        l_iLastTime = l_iThisTime;

        irr::core::vector3df l_vPos  = l_pCam->getPosition(),
                             l_vTgt  = l_pCam->getTarget() - l_vPos,
                             l_vUp   = l_pCam->getUpVector(),
                             l_vSide = l_vTgt.crossProduct(l_vUp),
                             l_vRot  = l_pCam->getRotation();

        l_pCam->setPosition(l_vPos + l_fMoveFactor * m_iMove * l_vTgt + l_fMoveFactor * m_iStrafe * l_vSide);
        l_pCam->setRotation(irr::core::vector3df(l_vRot.X, l_vRot.Y - ((irr::f32)l_iMouseX) / 15.0f, l_vRot.Z + fRot));

        m_pDrv->beginScene(true,true,irr::video::SColor(0,100,100,100));

        m_pRenderer->drawAll();

        // end scene
        m_pDrv->endScene();
      }
      m_pDevice->drop();
    }

    bool OnEvent(const irr::SEvent& event) {
      bool l_bRet = false;

      if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
        if (event.KeyInput.PressedDown) {
          switch (event.KeyInput.Key) {
            case irr::KEY_KEY_W : l_bRet = true; m_iMove   =  1; break;
            case irr::KEY_KEY_S : l_bRet = true; m_iMove   = -1; break;
            case irr::KEY_KEY_A : l_bRet = true; m_iStrafe =  1; break;
            case irr::KEY_KEY_D : l_bRet = true; m_iStrafe = -1; break;
            case irr::KEY_ESCAPE: l_bRet = true; m_pDevice->closeDevice(); break;
            default: break;
          }
        }
        else {
          switch (event.KeyInput.Key) {
            case irr::KEY_KEY_W: if (m_iMove   ==  1) { l_bRet = true; m_iMove   = 0; } break;
            case irr::KEY_KEY_S: if (m_iMove   == -1) { l_bRet = true; m_iMove   = 0; } break;
            case irr::KEY_KEY_A: if (m_iStrafe ==  1) { l_bRet = true; m_iStrafe = 0; } break;
            case irr::KEY_KEY_D: if (m_iStrafe == -1) { l_bRet = true; m_iStrafe = 0; } break;
            default: break;
          }
        }
      }
      return l_bRet;
    }
};

int main(int argc, char* argv[]){
  irr::u32 l_iScreenWidth  = 1280,
           l_iScreenHeight = 800;
  bool     l_bFullscreen   = false;

  // Check fullscreen
  for (int i=1;i<argc;i++) l_bFullscreen |= !strcmp("-f", argv[i]);

  CRunner theRunner(l_iScreenWidth, l_iScreenHeight, !l_bFullscreen);
  theRunner.run();

  return 0;
}
