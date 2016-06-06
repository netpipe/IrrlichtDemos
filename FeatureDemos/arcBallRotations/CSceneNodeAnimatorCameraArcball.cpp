#include "CSceneNodeAnimatorCameraArcball.h"

irr::scene::CSceneNodeAnimatorCameraArcball::CSceneNodeAnimatorCameraArcball(
   gui::ICursorControl* cursorControl,
   ICameraSceneNode* camera,
   EMOUSE_INPUT_EVENT captureEvent,
   EMOUSE_INPUT_EVENT freeEvent)
   :   CursorControl(cursorControl),
       Camera(camera),
       mouseDrag(false),
       CaptureMouseEvent(captureEvent),
       FreeMouseEvent(freeEvent)
{
   CamOri = Camera->getPosition();
   CamUp = Camera->getUpVector();
   QuatOri.makeIdentity();

   // should solve some camera miss placement effect
   core::vector3df begin(0, 0, 1);
   InvertAxes = (begin.dotProduct(Camera->getTarget() - CamOri) > 0);
   QuatOri.rotationFromTo(begin, CamOri);
}

irr::scene::CSceneNodeAnimatorCameraArcball::~CSceneNodeAnimatorCameraArcball() {}

irr::core::vector3df irr::scene::CSceneNodeAnimatorCameraArcball::position2vector(
   const irr::core::position2df& pos) const
{
   float fx = -(0.5 - pos.X);
   float fy = ((InvertAxes)?1:-1) * (0.5 - pos.Y);

   float fz   = 0.0f;
   float mag = fx*fx + fy*fy;

   if( mag > 1.0f)
   {
      float scale = 1.0f / sqrtf(mag);
      fx *= scale;
      fy *= scale;
   }
   else
      fz = sqrtf( 1.0f - mag );

   return irr::core::vector3df(fx, fy, fz);
}

void irr::scene::CSceneNodeAnimatorCameraArcball::animateNode(ISceneNode *node, u32 timeMs)
{
   if (!node || node->getType() != ESNT_CAMERA)
      return;

   if (!mouseDrag)
      return;

   if (!Camera->isInputReceiverEnabled())
      return;

   ISceneManager* smgr = Camera->getSceneManager();
   if (smgr && smgr->getActiveCamera() != Camera)
      return;

   core::vector3df target = Camera->getTarget();
   core::vector3df pos; // = Camera->getPosition() - target;
   core::vector3df up; // = Camera->getUpVector();
   core::quaternion quat;
   quat.makeIdentity();

   // compute the new quaternion
   if (CursorControl) {
      core::position2df CursorPos = CursorControl->getRelativePosition();
      core::vector3df from = position2vector(CursorDown);
      core::vector3df to = position2vector(CursorPos);
      quat.rotationFromTo(from, to);
      CursorDown = CursorPos;
   }

   // should solve some camera miss placement effect
   core::vector3df begin(0, 0, -1);
   core::quaternion quatAxe;
   quatAxe.rotationFromTo(CamOri, begin);

   // increment the movement quaternion
   QuatOri = quat * QuatOri;

   // move position into the origine Axes
   pos = quatAxe * CamOri;
   up = quatAxe * CamUp;

   // move position with the incremented quaternion
   pos = QuatOri * pos;
   up = QuatOri * up;

   // inverte the quatAxe
   quatAxe.makeInverse();

   // put it back into the position coordinates
   pos = quatAxe * pos;
   up = quatAxe * up;

   // move the center according to target
   Camera->setPosition(pos + target);
   Camera->setUpVector(up);
}

void irr::scene::CSceneNodeAnimatorCameraArcball::captureMouse() {
   CursorDown = CursorControl->getRelativePosition();
   mouseDrag = true;
}

void irr::scene::CSceneNodeAnimatorCameraArcball::freeMouse() {
   mouseDrag = false;
}

bool irr::scene::CSceneNodeAnimatorCameraArcball::OnEvent(const irr::SEvent & event)
{
   if (event.EventType == EET_MOUSE_INPUT_EVENT) {
      if (event.MouseInput.Event == CaptureMouseEvent) {
         captureMouse();
         return true;
      }
      if (event.MouseInput.Event == FreeMouseEvent) {
         freeMouse();
         return true;
      }
   }
   return false;
}

irr::scene::ISceneNodeAnimator* irr::scene::CSceneNodeAnimatorCameraArcball::createClone(
   ISceneNode *node,
   ISceneManager *newManager)
{
   CSceneNodeAnimatorCameraArcball* newAnimator =
      new CSceneNodeAnimatorCameraArcball(CursorControl, Camera);
   return newAnimator;
}
