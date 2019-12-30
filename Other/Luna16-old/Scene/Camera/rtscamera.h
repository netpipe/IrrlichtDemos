#ifndef __RTSCAMERA__
#define __RTSCAMERA__

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class RTSCamera : public ICameraSceneNode
{
   public:
      RTSCamera(IrrlichtDevice* devicepointer,ISceneNode* parent,ISceneManager* smgr,s32 id,
         f32 rotateSpeed = -1000.0f,f32 zoomSpeed = 1000.0f,f32 translationSpeed = 1000.0f);

      virtual ~RTSCamera();

      //Events
      virtual void render();
      virtual bool OnEvent(const SEvent& event);
      virtual void OnRegisterSceneNode();
      virtual void OnAnimate(u32 timeMs);

      //Setup
      virtual void setInputReceiverEnabled(bool enabled);
      virtual bool isInputReceiverEnabled() const;

      //Gets
      virtual const aabbox3d<f32>& getBoundingBox() const;
      virtual const matrix4& getProjectionMatrix() const;
      virtual const SViewFrustum* getViewFrustum() const;
      virtual const core::vector3df& getTarget() const;
      virtual const matrix4& getViewMatrix() const;
      virtual const core::vector3df& getUpVector() const;
      virtual f32 getNearValue() const;
      virtual f32 getFarValue() const;
      virtual f32 getAspectRatio() const;
      virtual f32 getFOV() const;

      //Sets
      virtual void setNearValue(f32 zn);
      virtual void setFarValue(f32 zf);
      virtual void setAspectRatio(f32 aspect);
      virtual void setFOV(f32 fovy);
      virtual void setUpVector(const vector3df& pos);
      virtual void setProjectionMatrix(const matrix4& projection);
      virtual void setPosition(const vector3df& newpos);
      virtual void setTarget(const vector3df& newpos);

     virtual void setRotation(const irr::core::vector3df &) {}
     virtual void setProjectionMatrix(const irr::core::matrix4 &,bool) {}
     virtual void setViewMatrixAffector(const irr::core::matrix4 &) {}
     virtual const core::matrix4& getViewMatrixAffector() const {return core::matrix4();};
     virtual void bindTargetAndRotation(bool) {}
     virtual bool getTargetAndRotationBinding() const {return 0;}

      virtual void setZoomSpeed(f32 value);
      virtual void setTranslateSpeed(f32 value);
      virtual void setRotationSpeed(f32 value);


      //Helper Functions
      //virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);
      //virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);
      void pointCameraAtNode(ISceneNode* selectednode);
      void setMinZoom(f32 amount);
      void setMaxZoom(f32 amount);

      //Type Return
      virtual ESCENE_NODE_TYPE getType() const { return ESNT_CAMERA; }

      //Public Attributes
      bool atMinDistance;
      bool atMaxDistance;
      ISceneNode* selectednode;
   protected:
      //Properties
      vector3df Target;
      vector3df UpVector;
      matrix4 Projection;
      matrix4 View;
      SViewFrustum ViewArea;
      aabbox3d<f32> BBox;
      bool InputReceiverEnabled;
      dimension2d<f32> screenDim;
      f32 Fovy;      // Field of view, in radians.
      f32 Aspect;   // Aspect ratio.
      f32 ZNear;   // value of the near view-plane.
      f32 ZFar;   // Z-value of the far view-plane.

      void recalculateProjectionMatrix();
      void recalculateViewArea();

   private:
      IrrlichtDevice* device;
      vector3df Pos;
      bool zooming, rotating, moving, translating;
      f32 zoomSpeed;
      f32 translateSpeed;
      f32 rotateSpeed;
      f32 rotateStartX, rotateStartY;
      f32 zoomStartX, zoomStartY;
      f32 translateStartX, translateStartY;
      f32 currentZoom;
      f32 rotX, rotY;
      vector3df oldTarget;
      vector2df MousePos;
      bool Keys[KEY_KEY_CODES_COUNT];
      bool MouseKeys[3];
      f32 targetMinDistance;
      f32 targetMaxDistance;

      enum MOUSE_BUTTON
      {
         MOUSE_BUTTON_RIGHT,
         MOUSE_BUTTON_MIDDLE,
         MOUSE_BUTTON_LEFT
      };

      void allKeysUp();
      void allMouseButtonsUp();
      bool isKeyDown(s32 key);
      bool isMouseButtonDown(s32 key);
      void animate();
      void updateAnimationState();
};

#endif 
