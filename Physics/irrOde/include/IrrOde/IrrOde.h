#ifndef _IRR_ODE_INCLUDED
  #define _IRR_ODE_INCLUDED

#include <CIrrOdeSurfaceParameters.h>
#include <CIrrOdeSpace.h>
#include <CIrrOdeWorld.h>
#include <IIrrOdeDevice.h>
#include <CIrrOdeManager.h>
#include <CIrrOdeSceneNodeFactory.h>

#include <geom/CIrrOdeGeomBox.h>
#include <geom/CIrrOdeGeomRay.h>
#include <geom/CIrrOdeGeomPlane.h>
#include <geom/CIrrOdeGeomSphere.h>
#include <geom/CIrrOdeGeomTrimesh.h>
#include <geom/CIrrOdeGeomCapsule.h>
#include <geom/CIrrOdeGeomCylinder.h>
#include <event/CIrrOdeEventRayHit.h>
#include <geom/CIrrOdeGeomHeightfield.h>

#include <event/IIrrOdeEvent.h>
#include <event/IIrrOdeEventListener.h>

#include <event/CIrrOdeEventStep.h>
#include <event/CIrrOdeEventInit.h>
#include <event/CIrrOdeEventClose.h>
#include <event/CIrrOdeEventRayHit.h>
#include <event/CIrrOdeEventBodyMoved.h>
#include <event/CIrrOdeEventBeforeStep.h>
#include <event/CIrrOdeEventBodyCreated.h>
#include <event/CIrrOdeEventBodyRemoved.h>
#include <event/CIrrOdeEventActivationChanged.h>

#include <joints/CIrrOdeJoint.h>
#include <joints/CIrrOdeJointFixed.h>
#include <joints/CIrrOdeJointHinge.h>
#include <joints/CIrrOdeJointHinge2.h>
#include <joints/CIrrOdeJointSlider.h>
#include <joints/CIrrOdeJointPiston.h>
#include <joints/CIrrOdeJointBallSocket.h>
#include <joints/CIrrOdeJointUniversal.h>

#include <IIrrOdeContactParameters.h>

#include <motors/CIrrOdeMotor.h>
#include <motors/CIrrOdeServo.h>

#endif
