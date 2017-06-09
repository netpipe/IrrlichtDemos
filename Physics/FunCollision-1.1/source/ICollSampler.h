// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef ICOLLSAMPLER_H_INCLUDED
#define ICOLLSAMPLER_H_INCLUDED

#include <irrlicht.h>
using namespace irr;


namespace COLL
{

//! ICollSampler class
/// Interface for sampling different collision entity shapes.
class ICollSampler : virtual public IReferenceCounted
{
    public:
        //! ----- Get functions:
        //! Returns the scale vector.
        virtual const core::vector3df& getScale() const = 0;

        //! Returns the relative translation.
        virtual const core::vector3df& getOrigin() const = 0;

        //! Returns the standing angle in degrees.
        virtual float getStandAngle() const = 0;

        //! Returns the sliding angle in degrees.
        virtual float getSlideAngle() const = 0;

        //! Returns the inverse mass.
        virtual float getInverseMass() const = 0;

        //! Returns the bouncing factor.
        virtual float getBounceFactor() const = 0;


        //! ----- Set functions:
        //! Sets new scale factors.
        virtual void setScale(const core::vector3df& Scale) = 0;

        //! Sets a new relative translation.
        virtual void setOrigin(const core::vector3df& Origin) = 0;

        //! Sets a new stand value in degrees, ranges from 0.0f to 90.0f
        virtual void setStandAngle(float Degrees) = 0;

        //! Sets a new sliding value in degrees, ranges from 0.0f to 90.0f
        virtual void setSlideAngle(float Degrees) = 0;

        //! Sets a new inverse mass, ranges from 0.0f (max.) to 1.0f (min. weight).
        virtual void setInverseMass(float InverseMass) = 0;

        //! Sets a new bouncing factor, ranges from 0.0f to 1.0f.
        virtual void setBounceFactor(float BounceFactor) = 0;

}; // end class ICollSampler


//! ICollSphereSampler class
/// Interface of the simple sphere shape sampler.
class ICollSphereSampler : virtual public ICollSampler
{

}; // end class ICollSphereSampler


//! ICollEllipsoidSampler class
/// Interface of the axis-aligned ellipsoid shape sampler.
class ICollEllipsoidSampler : virtual public ICollSampler
{

}; // end class ICollEllipsoidSampler


} // end namespace COLL


#endif // ICOLLSAMPLER_H_INCLUDED
