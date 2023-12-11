
#ifndef ISCENENODEANIMATORCAMERAARCBALL_H_
#define ISCENENODEANIMATORCAMERAARCBALL_H_

namespace irr {
   namespace scene {

      class ISceneNodeAnimatorCameraArcball : public ISceneNodeAnimator {
      public:
         virtual void captureMouse() = 0;
         virtual bool isMouseCaptured() const = 0;
         virtual void freeMouse() = 0;
         virtual void setCaptureMouseEvent(EMOUSE_INPUT_EVENT event) = 0;
         virtual EMOUSE_INPUT_EVENT getCaptureMouseEvent() const = 0;
         virtual void setFreeMouseEvent(EMOUSE_INPUT_EVENT event) = 0;
         virtual EMOUSE_INPUT_EVENT getFreeMouseEvent() const = 0;
      };

   } // end scene
} // end irr

#endif /* ISCENENODEANIMATORCAMERAARCBALL_H_ */ 
