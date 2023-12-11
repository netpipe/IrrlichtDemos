/*! \file osLocalSpace.h
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
#ifndef OPENSTEER_LOCALSPACE_H
#define OPENSTEER_LOCALSPACE_H

#include "osVec3.h"

//! namespace OpenSteer
namespace OpenSteer{

  //! abstract local space class
  class AbstractLocalSpace{
    public:
      virtual ~AbstractLocalSpace() { /* Nothing to do. */ }

      //! accessors get for up
      virtual irr::core::vector3df up3df    (void) const =0;
      //! accessors Set for up
      virtual irr::core::vector3df up3df    (irr::core::vector3df u) =0;
      //! accessors get for side
      virtual irr::core::vector3df side3df  (void) const =0;
      //! accessors Set for side
      virtual irr::core::vector3df side3df  (irr::core::vector3df s) =0;
      //! accessors get for forward
      virtual irr::core::vector3df forward3df(void) const =0;
      //! accessors Set for forward
      virtual irr::core::vector3df forward3df(irr::core::vector3df f) =0;
      //! accessors get for position
      virtual irr::core::vector3df position3df(void) const =0;
      //! accessors Set for position
      virtual irr::core::vector3df position3df(irr::core::vector3df p) =0;
      //! accessors get for rotation
      virtual Vec3 rotation (void) const =0;
      //! accessors Set for rotation
      virtual Vec3 rotation (Vec3 r) =0;
      //! accessors Set for rotation
      virtual Vec3 rotation (float x, float y, float z)=0;
      //! accessors get for rotation
      virtual irr::core::vector3df rotation3df(void) const =0;
      //! accessors Set for rotation
      virtual irr::core::vector3df rotation3df(irr::core::vector3df r) =0;


      //! accessors get for side
      virtual Vec3 side (void) const = 0;
      //! accessors Set for side
      virtual Vec3 side (Vec3 s) = 0;
      //! accessors get for up
      virtual Vec3 up (void) const = 0;
      //! accessors Set for up
      virtual Vec3 up (Vec3 u) = 0;
      //! accessors get for forward
      virtual Vec3 forward (void) const = 0;
      //! accessors Set for forward
      virtual Vec3 forward (Vec3 f) = 0;
      //! accessors get for position
      virtual Vec3 position (void) const = 0;
      //! accessors Set for position
      virtual Vec3 position (Vec3 p) = 0;

      //! use right-(or left-)handed coordinate space
      virtual bool rightHanded (void) const = 0;
      //! reset transform to identity
      virtual void resetLocalSpace (void) = 0;
      //! transform a direction in global space to its equivalent in local space
      virtual Vec3 localizeDirection (const Vec3& globalDirection) const = 0;
      //! transform a point in global space to its equivalent in local space
      virtual Vec3 localizePosition (const Vec3& globalPosition) const = 0;
      //! transform a point in local space to its equivalent in global space
      virtual Vec3 globalizePosition (const Vec3& localPosition) const = 0;
      //! transform a direction in local space to its equivalent in global space
      virtual Vec3 globalizeDirection (const Vec3& localDirection) const = 0;
      //! set "side" basis vector to normalized cross product of forward and up
      virtual void setUnitSideFromForwardAndUp (void) = 0;
      //! regenerate the orthonormal basis vectors given a new forward (which is expected to have unit length)
      virtual void regenerateOrthonormalBasisUF (const Vec3& newUnitForward) = 0;
      //! for when the new forward is NOT of unit length
      virtual void regenerateOrthonormalBasis (const Vec3& newForward) = 0;
      //! for supplying both a new forward and and new up
      virtual void regenerateOrthonormalBasis (const Vec3& newForward, const Vec3& newUp) = 0;
      //! rotate 90 degrees in the direction implied by rightHanded()
      virtual Vec3 localRotateForwardToSide (const Vec3& v) const = 0;
      //! rotate 90 degrees in the direction implied by rightHanded()
      virtual Vec3 globalRotateForwardToSide (const Vec3& globalForward) const=0;
  };

  //! LocalSpaceMixin is a mixin layer, a class template with a paramterized base class.  Allows "LocalSpace-ness" to be layered on any class.
  /*! transformation as three orthonormal unit basis vectors and the
      origin of the local space.  These correspond to the "rows" of
      a 3x4 transformation matrix with [0 0 0 1] as the final column */
  template <class Super>
  class LocalSpaceMixin : public Super{
    private:
      Vec3 _side;     //    side-pointing unit basis vector
      Vec3 _up;       //  upward-pointing unit basis vector
      Vec3 _forward;  // forward-pointing unit basis vector
      Vec3 _position; // origin of local space

    public:
      //! accessors get for side
      Vec3 side     (void) const {return _side;};
      //! accessors Set for side
      Vec3 side     (Vec3 s) {return _side = s;};
      //! accessors Set for side
      Vec3 side     (float x, float y, float z){return _side.set    (x,y,z);};
      //! accessors get for side
      irr::core::vector3df side3df  (void) const {
        return irr::core::vector3df(_side.x, _side.y, _side.z);
      }
      //! accessors Set for side
      irr::core::vector3df side3df  (irr::core::vector3df s) {
        _side = Vec3(s);
        return s;
      }

      //! accessors get for up
      Vec3 up       (void) const {return _up;};
      //! accessors Set for up
      Vec3 up       (Vec3 u) {return _up = u;};
      //! accessors Set for up
      Vec3 up       (float x, float y, float z){return _up.set      (x,y,z);};
      //! accessors get for up
      irr::core::vector3df up3df    (void) const {
        return irr::core::vector3df(_up.x, _up.y, _up.z);
      }
      //! accessors Set for up
      irr::core::vector3df up3df    (irr::core::vector3df u) {
        _up = Vec3(u);
        return u;
      }

      //! accessors get for forward
      Vec3 forward  (void) const {return _forward;};
      //! accessors Set for forward
      Vec3 forward  (Vec3 f) {return _forward = f;};
      //! accessors Set for forward
      Vec3 forward  (float x, float y, float z){return _forward.set (x,y,z);};
      //! accessors get for forward
      irr::core::vector3df forward3df(void) const {
        return irr::core::vector3df(_forward.x, _forward.y, _forward.z);
      }
      //! accessors Set for forward
      irr::core::vector3df forward3df(irr::core::vector3df f) {
        _forward = Vec3(f);
        return f;
      }

      //! accessors get for position
      Vec3 position (void) const {return _position;};
      //! accessors Set for position
      Vec3 position (Vec3 p) {return _position = p;};
      //! accessors Set for position
      Vec3 position (float x, float y, float z){return _position.set(x,y,z);};
      //! accessors get for position
      irr::core::vector3df position3df(void) const {
        return irr::core::vector3df(_position.x, _position.y, _position.z);
      }
      //! accessors Set for position
      irr::core::vector3df position3df(irr::core::vector3df p) {
        _position = Vec3(p);
        return p;
      }

      //! accessors get for rotation
      Vec3 rotation (void) const {
        return Vec3(forward().vector3df().getHorizontalAngle());
      }
      //! accessors Set for rotation
      Vec3 rotation (Vec3 r) {
        irr::core::matrix4 mat;
        mat.setRotationDegrees(r.vector3df());
        _forward.set(mat(2,0), mat(2,1), mat(2,2));
        _forward.normalize();
        return Vec3(_forward.vector3df().getHorizontalAngle());
      }
      //! accessors Set for rotation
      Vec3 rotation (float x, float y, float z){
        irr::core::matrix4 mat;
        mat.setRotationDegrees(irr::core::vector3df(x,y,z));
        _forward.set(mat(2,0), mat(2,1), mat(2,2));
        _forward.normalize();
        return Vec3(_forward.vector3df().getHorizontalAngle());
      }
      //! accessors get for rotation
      irr::core::vector3df rotation3df(void) const {
        return forward().vector3df().getHorizontalAngle();
      }
      //! accessors Set for rotation
      irr::core::vector3df rotation3df(irr::core::vector3df r) {
        irr::core::matrix4 mat;
        mat.setRotationDegrees(r);
        _forward.set(mat(2,0), mat(2,1), mat(2,2));
        _forward.normalize();
        return r;
      }

      //! Global compile-time switch to control handedness/chirality.
      /*! should LocalSpace use a left- or right-handed coordinate system?
          This can be overloaded in derived types (e.g. vehicles) to change handedness. */
      bool rightHanded(void) const {return true;}

        //! constructor
        LocalSpaceMixin(void){
          resetLocalSpace ();
        };
        //! constructor
        LocalSpaceMixin(const Vec3& Side,
                        const Vec3& Up,
                        const Vec3& Forward,
                        const Vec3& Position)
            : _side( Side ), _up( Up ), _forward( Forward ), _position( Position ) {}
        //! constructor
        LocalSpaceMixin(const Vec3& Up,
                        const Vec3& Forward,
                        const Vec3& Position)
            : _side(), _up( Up ), _forward( Forward ), _position( Position ){
            setUnitSideFromForwardAndUp ();
        }

        virtual ~LocalSpaceMixin() { /* Nothing to do. */ }

        //! reset transform
        /*! set local space to its identity state, equivalent to a 4x4 homogeneous transform like this:
            *
            *  [ X 0 0 0 ]
            *  [ 0 1 0 0 ]
            *  [ 0 0 1 0 ]
            *  [ 0 0 0 1 ]
            *
            where X is 1 for a left-handed system and -1 for a right-handed system. */
        void resetLocalSpace(void){
            _forward.set (0, 0, 1);
            _side = localRotateForwardToSide (_forward);
            _up.set (0, 1, 0);
            _position.set (0, 0, 0);
        };

        //! transform a direction in global space to its equivalent in local space
        Vec3 localizeDirection (const Vec3& globalDirection) const{
            // dot offset with local basis vectors to obtain local coordiantes
            return Vec3 (globalDirection.dot (_side),
                         globalDirection.dot (_up),
                         globalDirection.dot (_forward));
        };

        //! transform a point in global space to its equivalent in local space
        Vec3 localizePosition (const Vec3& globalPosition) const{
            // global offset from local origin
            Vec3 globalOffset = globalPosition - _position;

            // dot offset with local basis vectors to obtain local coordiantes
            return localizeDirection (globalOffset);
        };

        //! transform a point in local space to its equivalent in global space
        Vec3 globalizePosition (const Vec3& localPosition) const{
            return _position + globalizeDirection (localPosition);
        };

        //! transform a direction in local space to its equivalent in global space
        Vec3 globalizeDirection (const Vec3& localDirection) const{
            return ((_side    * localDirection.x) +
                    (_up      * localDirection.y) +
                    (_forward * localDirection.z));
        };

        //! set "side" basis vector to normalized cross product of forward and up
        void setUnitSideFromForwardAndUp (void){
            // derive new unit side basis vector from forward and up
            if (rightHanded())
                _side.cross (_forward, _up);
            else
                _side.cross (_up, _forward);
            _side = _side.normalize ();
        }

        //! regenerate the orthonormal basis vectors given a new forward (which is expected to have unit length)
        void regenerateOrthonormalBasisUF (const Vec3& newUnitForward){
            _forward = newUnitForward;

            // derive new side basis vector from NEW forward and OLD up
            setUnitSideFromForwardAndUp ();

            // derive new Up basis vector from new Side and new Forward
            // (should have unit length since Side and Forward are
            // perpendicular and unit length)
            if (rightHanded())
                _up.cross (_side, _forward);
            else
                _up.cross (_forward, _side);
        }

        //! for when the new forward is NOT know to have unit length
        void regenerateOrthonormalBasis (const Vec3& newForward){
            regenerateOrthonormalBasisUF (newForward.normalize());
        }

        //! for supplying both a new forward and and new up
        void regenerateOrthonormalBasis (const Vec3& newForward, const Vec3& newUp){
            _up = newUp;
            regenerateOrthonormalBasis (newForward.normalize());
        }

        //! rotate, in the canonical direction, a vector pointing in the "forward" (+Z) direction to the "side" (+/-X) direction
        Vec3 localRotateForwardToSide (const Vec3& v) const{
            return Vec3 (rightHanded () ? -v.z : +v.z,
                         v.y,
                         v.x);
        }

        //! not currently used, just added for completeness
        Vec3 globalRotateForwardToSide (const Vec3& globalForward) const{
            const Vec3 localForward = localizeDirection (globalForward);
            const Vec3 localSide = localRotateForwardToSide (localForward);
            return globalizeDirection (localSide);
        }
    };

    //! Concrete LocalSpace class
    typedef LocalSpaceMixin<AbstractLocalSpace> LocalSpace;
    //! a global constant for the identity transform
    const LocalSpace gGlobalSpace;

} // namespace OpenSteer

#endif // OPENSTEER_LOCALSPACE_H
#endif
