/*! \file osAbstractVehicle.h
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
#ifdef OPENSTEER
#ifndef OPENSTEER_ABSTRACTVEHICLE_H
#define OPENSTEER_ABSTRACTVEHICLE_H

#include "osLocalSpace.h"
#include <vector>

//! namespace OpenSteer
namespace OpenSteer{

  //! abstract vehicle class
  class AbstractVehicle : public AbstractLocalSpace{
    public:
      virtual ~AbstractVehicle() { /* Nothing to do. */ }

      //! get mass (defaults to unity so acceleration=force)
      virtual float mass (void) const = 0;
      //! set mass (defaults to unity so acceleration=force)
      virtual float mass (float) = 0;

      //! get size of bounding sphere, for obstacle avoidance, etc.
      virtual float radius (void) const = 0;
      //! set size of bounding sphere, for obstacle avoidance, etc.
      virtual float radius (float) = 0;

      //! velocity of vehicle
      virtual Vec3 velocity (void) const = 0;

      //! get speed of vehicle  (may be faster than taking magnitude of velocity)
      virtual float speed (void) const = 0;
      //! set speed of vehicle  (may be faster than taking magnitude of velocity)
      virtual float speed (float) = 0;

      //! groups of (pointers to) abstract vehicles
      typedef std::vector<AbstractVehicle*> group;
      //! iterators over vehicle group
      typedef group::const_iterator iterator;

      //! predict position of this vehicle at some time in the future (assumes velocity remains constant)
      virtual Vec3 predictFuturePosition (const float predictionTime) const = 0;

      //! get the maximum steering force this vehicle can apply
      virtual float maxForce (void) const = 0;
      //! set the maximum steering force this vehicle can apply
      virtual float maxForce (float) = 0;

      //! get the maximum speed this vehicle is allowed to move
      virtual float maxSpeed (void) const = 0;
      //! set the maximum speed this vehicle is allowed to move
      virtual float maxSpeed (float) = 0;

      //! dp - added to support heterogeneous flocks
      virtual void update(const float currentTime, const float elapsedTime) = 0;

  };

  //! more convenient short name for AbstractVehicle group
  typedef AbstractVehicle::group AVGroup;
  //! more convenient short names for iterator of AbstractVehicle group
  typedef AbstractVehicle::iterator AVIterator;

} // namespace OpenSteer

// ----------------------------------------------------------------------------<br>
#endif // OPENSTEER_ABSTRACTVEHICLE_H
#endif
