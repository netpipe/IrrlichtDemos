// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#ifndef CCOLLSAMPLER_H_INCLUDED
#define CCOLLSAMPLER_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "ICollSampler.h"
#include "CCollCommon.h"


namespace COLL
{

//! forward decl.
class CCollEntity;


//! CCollSampler class
class CCollSampler : virtual public ICollSampler
{
    public:
        //! empty constructor
        CCollSampler() { sphereRadius = standValue = slideValue = inverseMass = bounceFactor = 0; }

        //! constructor
        CCollSampler(const core::vector3df& Scale, const core::vector3df& Origin,
            float StandAngle, float SlideAngle, float InverseMass, float BounceFactor)
        { init(Scale, Origin, StandAngle, SlideAngle, InverseMass, BounceFactor); }

        //! init
        void init(const core::vector3df& Scale, const core::vector3df& Origin,
                  float StandAngle, float SlideAngle, float InverseMass, float BounceFactor)
        {
            setScale(Scale);
            setOrigin(Origin);
            setStandAngle(StandAngle);
            setSlideAngle(SlideAngle);
            setInverseMass(InverseMass);
            setBounceFactor(BounceFactor);
        }


        //! ----- Get functions:
        const core::vector3df& getScale() const { return scale; }
        const core::vector3df& getOrigin() const { return origin; }

        //! getSphereRadius
        float getSphereRadius() const { return sphereRadius; }

        float getStandValue() const { return standValue; }
        float getSlideValue() const { return slideValue; }
        float getStandAngle() const { return (getStandValue() / COLL_SQRT_2) * 90.0f; }
        float getSlideAngle() const { return (getSlideValue() / COLL_SQRT_2) * 90.0f; }
        float getInverseMass() const { return inverseMass; }
        float getBounceFactor() const { return bounceFactor; }


        //! ----- Set functions:
        virtual void setScale(const core::vector3df& Scale);
        void setOrigin(const core::vector3df& Origin) { origin = Origin; }
        void setStandAngle(float Degrees) { standValue = (Degrees / 90.0f) * COLL_SQRT_2; }
        void setSlideAngle(float Degrees) { slideValue = (Degrees / 90.0f) * COLL_SQRT_2; }
        void setInverseMass(float InverseMass) { inverseMass = InverseMass > 1 ? 1 : (InverseMass < 0 ? 0 : InverseMass); }
        void setBounceFactor(float BounceFactor) { bounceFactor = BounceFactor > 1 ? 1 : (BounceFactor < 0 ? 0 : BounceFactor); }


        //! ----- Functions:
        //! getBBox functions
        core::aabbox3df getBBoxOS() const { return core::aabbox3df(-scale, scale); }
        core::aabbox3df getBBoxWS(const core::vector3df& Pos) const { return core::aabbox3df(Pos - scale, Pos + scale); }

        //! transform functions
        virtual core::vector3df transformPointOS(const core::vector3df& Point) { return Point; }
        virtual core::vector3df transformPointWS(const core::vector3df& Point) { return Point; }

        CCollTriangloid transformTriOS(const CCollTriangloid& Tri)
        {
            CCollTriangloid ret(Tri);
            ret.setTri(transformPointOS(Tri.getPointA()), transformPointOS(Tri.getPointB()), transformPointOS(Tri.getPointC()));
            ret.recalcPlane();
            return ret;
        }
        CCollTriangloid transformTriWS(const CCollTriangloid& Tri)
        {
            CCollTriangloid ret(Tri);
            ret.setTri(transformPointWS(Tri.getPointA()), transformPointWS(Tri.getPointB()), transformPointWS(Tri.getPointC()));
            ret.recalcPlane();
            return ret;
        }

        //! prepareTriangloid
        virtual void prepareTriangloid(CCollTriangloid& Tri, const core::matrix4& Matrix) { }


        //! spherePoint
        bool spherePoint(const core::line3df& S, const core::vector3df& P, SCollContact& Contact, float ToleranceSQ);

        //! sphereEdge
        bool sphereEdge(const core::line3df& S, const core::line3df& E, SCollContact& Contact, float ToleranceSQ);

        //! sphereTriangloid
        bool sphereTriangloid(const core::line3df& S, const CCollTriangloid& T, SCollContact& Contact, float ToleranceSQ);

        //! collideTriangloid
        bool collideTriangloid(const core::line3df& S, const CCollTriangloid& T, SCollContact& Contact, float ToleranceSQ);


        //! collideEntity
        bool collideEntity(const core::line3df& S, CCollEntity* Entity, SCollContact& Contact);


        //! rayIntersect
        bool rayIntersect(const core::vector3df& Pos, const core::line3df& Ray, SCollContact& Contact);

    protected:
        //! vars:
        core::vector3df origin;
        core::vector3df scale;
        float sphereRadius;
        float standValue;
        float slideValue;
        float inverseMass;
        float bounceFactor;

}; // end class CCollSampler


//! CCollSphereSampler class
class CCollSphereSampler : virtual public ICollSphereSampler, public CCollSampler
{
    public:
        //! constructor
        CCollSphereSampler(const core::vector3df& Scale, const core::vector3df& Origin,
            float StandAngle, float SlideAngle, float InverseMass, float BounceFactor) : CCollSampler()
        { init(Scale, Origin, StandAngle, SlideAngle, InverseMass, BounceFactor); }

        //! Sets the dimensions of the shape, the componenets should be larger than 0.
        /// The largest component determines the final dimension.
        void setScale(const core::vector3df& Scale);

        //! prepareTriangloid
        void prepareTriangloid(CCollTriangloid& Tri, const core::matrix4& Matrix);

}; // end class CCollSphereSampler


//! CCollEllipsoidSampler class
class CCollEllipsoidSampler : virtual public ICollEllipsoidSampler, public CCollSampler
{
    public:
        //! constructor
        CCollEllipsoidSampler(const core::vector3df& Scale, const core::vector3df& Origin,
            float StandAngle, float SlideAngle, float InverseMass, float BounceFactor) : CCollSampler()
        { init(Scale, Origin, StandAngle, SlideAngle, InverseMass, BounceFactor); }

        //! Sets the dimensions of the shape, the componenets should be larger than 0.
        void setScale(const core::vector3df& Scale);

        //! transform functions
        core::vector3df transformPointOS(const core::vector3df& Point) { return Point * mps; }
        core::vector3df transformPointWS(const core::vector3df& Point) { return Point * spm; }

        //! prepareTriangloid
        void prepareTriangloid(CCollTriangloid& Tri, const core::matrix4& Matrix);

    private:
        //! vars:
        core::vector3df mps, spm;

}; // end class CCollEllipsoidSampler

} // end namespace COLL

#endif // CCOLLSAMPLER_H_INCLUDED
