
#ifndef CSCENENODEANIMATORCAMERAARCBALL_H_
#define CSCENENODEANIMATORCAMERAARCBALL_H_

#include <irrlicht.h>
#include "ISceneNodeAnimatorCameraArcball.h"

namespace irr {
   namespace scene {

      class CSceneNodeAnimatorCameraArcball : public ISceneNodeAnimatorCameraArcball {
      public:
         //! Constructor
         CSceneNodeAnimatorCameraArcball(
            gui::ICursorControl* cursorControl,
            ICameraSceneNode* camera,
            EMOUSE_INPUT_EVENT captureEvent = EMIE_LMOUSE_PRESSED_DOWN,
            EMOUSE_INPUT_EVENT freeEvent = EMIE_LMOUSE_LEFT_UP);

         //! Destructor
         virtual ~CSceneNodeAnimatorCameraArcball();

         //! Capture the mouse
         virtual void captureMouse();

         //! tell if the mouse is captured
         virtual bool isMouseCaptured() const { return mouseDrag; }

         //! Free the mouse
         virtual void freeMouse();

         //! set the capture event for the mouse
         virtual void setCaptureMouseEvent(EMOUSE_INPUT_EVENT event)
            { CaptureMouseEvent = event; }

         //! get the capture event of the mouse
         virtual EMOUSE_INPUT_EVENT getCaptureMouseEvent() const
            { return CaptureMouseEvent; }

         //! set the free event for the mouse
         virtual void setFreeMouseEvent(EMOUSE_INPUT_EVENT event)
            { FreeMouseEvent = event; }

         //! get the free event for the mouse
         virtual EMOUSE_INPUT_EVENT getFreeMouseEvent() const
            { return FreeMouseEvent; }

         //! Animate the scene node, currently only work on cameras
         virtual void animateNode(ISceneNode* node, u32 timeMs);

         //! Event receiver
         virtual bool OnEvent(const SEvent& event);

         //! This animator will receive events when attached to the active camera
         virtual bool isEventReceiverEnabled() const
            { return true; }

         //! Returns the type of this animator
         virtual ESCENE_NODE_ANIMATOR_TYPE getType() const
            { return ESNAT_CAMERA_FPS; }

         //! create clone of this animator
         virtual ISceneNodeAnimator* createClone(
            ISceneNode* node,
            ISceneManager* newManager = 0);
      private :
         core::vector3df position2vector(const core::position2df& pos) const;
         gui::ICursorControl* CursorControl;
         core::position2df CursorDown;
         core::quaternion QuatOri;
         core::vector3df CamOri;
         core::vector3df CamUp;
         ICameraSceneNode* Camera;
         bool InvertAxes;
         bool mouseDrag;
         EMOUSE_INPUT_EVENT CaptureMouseEvent;
         EMOUSE_INPUT_EVENT FreeMouseEvent;
      };

   }
}

#endif /* CSCENENODEANIMATORCAMERAARCBALL_H_ */
