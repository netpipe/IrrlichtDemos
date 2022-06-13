#include "../../../../config.h"
#ifdef OPENSTEER
/*! \file osVehicleSceneNode.h
    ----------------------------------------------------------------------------<br>
    Irrlicht implementation by A.Buschhüter<br>
    http://abusoft.g0dsoft.com<br>
    abusoft@g0dsoft.com<br>
    ----------------------------------------------------------------------------<br>
    OpenSteer -- Steering Behaviors for Autonomous Characters<br>
    <br>
    Copyright (c) 2002-2005, Sony Computer Entertainment America<br>
    Original author: Craig Reynolds <craig_reynolds@playstation.sony.com><br>
    <br>
    Permission is hereby granted, free of charge, to any person obtaining a<br>
    copy of this software and associated documentation files (the "Software"),<br>
    to deal in the Software without restriction, including without limitation<br>
    the rights to use, copy, modify, merge, publish, distribute, sublicense,<br>
    and/or sell copies of the Software, and to permit persons to whom the<br>
    Software is furnished to do so, subject to the following conditions:<br>
    <br>
    The above copyright notice and this permission notice shall be included in<br>
    all copies or substantial portions of the Software.<br>
    <br>
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR<br>
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,<br>
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL<br>
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER<br>
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING<br>
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER<br>
    DEALINGS IN THE SOFTWARE.<br>
    ----------------------------------------------------------------------------<br>
*/

#ifndef OPENSTEER_VehicleSceneNode_H
#define OPENSTEER_VehicleSceneNode_H

#include "osAbstractVehicle.h"
#include "osSteerLibrary.h"
#include "osAnnotation.h"

//! namespace OpenSteer
namespace OpenSteer{
  //! VehicleSceneNode_1 adds concrete LocalSpace methods to AbstractVehicle
  typedef LocalSpaceMixin<AbstractVehicle> VehicleSceneNode_1;
  //! VehicleSceneNode_2 adds concrete annotation methods to VehicleSceneNode_1
  typedef AnnotationMixin<VehicleSceneNode_1> VehicleSceneNode_2;
  //! VehicleSceneNode_3 adds concrete steering methods to VehicleSceneNode_2
  typedef SteerLibraryMixin<VehicleSceneNode_2> VehicleSceneNode_3;

  // VehicleSceneNode adds concrete vehicle methods to VehicleSceneNode_3
  class VehicleSceneNode : public VehicleSceneNode_3{
    private:
      float _mass;       // mass (defaults to unity so acceleration=force)
      float _radius;     // size of bounding sphere, for obstacle avoidance, etc.
      float _speed;      // speed along Forward direction.  Because local space
                         // is velocity-aligned, velocity = Forward * Speed
      float _maxForce;   // the maximum steering force this vehicle can apply
                         // (steering force is clipped to this magnitude)
      float _maxSpeed;   // the maximum speed this vehicle is allowed to move
                         // (velocity is clipped to this magnitude)

      float _curvature;
      Vec3 _lastForward;
      Vec3 _lastPosition;
      Vec3 _smoothedPosition;
      float _smoothedCurvature;
      Vec3 _smoothedAcceleration;

      // measure path curvature (1/turning-radius), maintain smoothed version
      void measurePathCurvature (const float elapsedTime);
    public:
      // constructor
      VehicleSceneNode ();
      // destructor
      ~VehicleSceneNode ();
      //! reset vehicle state
      void reset(void){
        // reset LocalSpace state
        resetLocalSpace ();

        // reset SteerLibraryMixin state
        // (XXX this seems really fragile, needs to be redesigned XXX)
        VehicleSceneNode_3::reset ();

        mass (1);          // mass (defaults to 1 so acceleration=force)
        speed (0);         // speed along Forward direction.

        radius (0.5f);     // size of bounding sphere

        maxForce (0.1f);   // steering force is clipped to this magnitude
        maxSpeed (1.0f);   // velocity is clipped to this magnitude

        // reset bookkeeping to do running averages of these quanities
        resetSmoothedPosition ();
        resetSmoothedCurvature ();
        resetSmoothedAcceleration ();
      }

      //! get the type of the vehicle, 0 = simple (unknown) vehicle
      virtual int getVecicleType(){ return 0;}

      //! get mass
      float mass (void) const {return _mass;}
      //! set mass
      float mass (float m) {return _mass = m;}

      //! get velocity of vehicle
      Vec3 velocity (void) const {return forward() * _speed;}

      //! get speed of vehicle  (may be faster than taking mag of velocity)
      float speed (void) const {return _speed;}
      //! set speed of vehicle  (may be faster than taking mag of velocity)
      float speed (float s) {return _speed = s;}

      //! get size of bounding sphere, for obstacle avoidance, etc.
      float radius (void) const {return _radius;}
      //! set size of bounding sphere, for obstacle avoidance, etc.
      float radius (float m) {return _radius = m;}

      //! get maxForce
      float maxForce (void) const {return _maxForce;}
      //! set maxForce
      float maxForce (float mf) {return _maxForce = mf;}

      //! get maxSpeed
      float maxSpeed (void) const {return _maxSpeed;}
      //! set maxSpeed
      float maxSpeed (float ms) {return _maxSpeed = ms;}

      //! ratio of speed to max possible speed (0 slowest, 1 fastest)
      float relativeSpeed (void) const {return speed () / maxSpeed ();}


      //! apply a given steering force to our momentum, adjusting our orientation to maintain velocity-alignment.
      void applySteeringForce (const Vec3& force, const float deltaTime);

      //! the default version: keep FORWARD parallel to velocity, change UP as little as possible.
      virtual void regenerateLocalSpace (const Vec3& newVelocity, const float elapsedTime);

      //! alternate version: keep FORWARD parallel to velocity, adjust UP according to a no-basis-in-reality "banking" behavior, something like what birds and airplanes do.
      void regenerateLocalSpaceForBanking (const Vec3& newVelocity, const float elapsedTime);

      //! adjust the steering force passed to applySteeringForce.
      /*! allows a specific vehicle class to redefine this adjustment.
          default is to disallow backward-facing steering at low speed. */
      virtual Vec3 adjustRawSteeringForce (const Vec3& force, const float deltaTime);

      //! apply a given braking force (for a given dt) to our momentum.
      void applyBrakingForce (const float rate, const float deltaTime);

      //! predict position of this vehicle at some time in the future (assumes velocity remains constant)
      Vec3 predictFuturePosition (const float predictionTime) const;

      //! get instantaneous curvature (since last update)
      float curvature (void) const {return _curvature;}

      //! get smoothedCurvature
      float smoothedCurvature (void) {return _smoothedCurvature;}
      //! reset smoothedCurvature
      float resetSmoothedCurvature (float value = 0){
          _lastForward = Vec3::zero;
          _lastPosition = Vec3::zero;
          return _smoothedCurvature = _curvature = value;
      }
      //! get smoothedAcceleration
      Vec3 smoothedAcceleration (void) {return _smoothedAcceleration;}
      //! reset smoothedAcceleration
      Vec3 resetSmoothedAcceleration (const Vec3& value = Vec3::zero){
          return _smoothedAcceleration = value;
      }
      //! get smoothedPosition
      Vec3 smoothedPosition (void) {return _smoothedPosition;}
      //! reset smoothedPosition
      Vec3 resetSmoothedPosition (const Vec3& value = Vec3::zero){
          return _smoothedPosition = value;
      }

      //! set a random "2D" heading: set local Up to global Y, then effectively rotate about it by a random angle (pick random forward, derive side).
      void randomizeHeadingOnXZPlane (void){
          up (Vec3::up);
          forward (RandomUnitVectorOnXZPlane ());
          side (localRotateForwardToSide (forward()));
      }

      //! give each vehicle a unique number
      int serialNumber;
      //! count of serial numbers (helps to get a unique serialNumber)
      static int serialNumberCounter;

      // Annotation
      //! draw lines from vehicle's position showing its velocity and acceleration
      void annotationVelocityAcceleration(video::IVideoDriver* driver, float maxLengthA, float maxLengthV);
      /*dpc*/ void annotationVelocityAcceleration (float maxLengthA, float maxLengthV);

      //! draw lines from vehicle's position showing its velocity and acceleration
      void annotationVelocityAcceleration(video::IVideoDriver* driver, float maxLength){
        annotationVelocityAcceleration(driver, maxLength, maxLength);
      }
      /*dpc*/ void annotationVelocityAcceleration (float maxLength){
                annotationVelocityAcceleration (maxLength, maxLength);
              }

      //! draw lines from vehicle's position showing its velocity and acceleration
      void annotationVelocityAcceleration(video::IVideoDriver* driver){
        annotationVelocityAcceleration(driver, 3, 3);
      }
      /*dpc*/ void annotationVelocityAcceleration (void){
                annotationVelocityAcceleration (3, 3);
              }

  };

} // namespace OpenSteer

#endif // OPENSTEER_VehicleSceneNode_H
#endif
