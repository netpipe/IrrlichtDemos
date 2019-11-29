/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHOENIXPOLY_H__
#define __PHOENIXPOLY_H__

#include <vector>
#include <algorithm>
#include <boost/foreach.hpp>
#include "config.h"
#include "Vector2d.h"
#include "RotationMatrix.h"

namespace phoenix
{

    //forward decl of Rectangle
    class Rectangle;

    //! Polygon class.
    /*!
        This class represents polygons for drawing and collision detection.
    	A polygon is basically any geometric object that can be represented by a
        triangle fan. Polygons are constructed with vertices, vertices are
        defined as a vector from the center of the polygon.
    */
    class Polygon
    {

    public:

        //! Constructor.
        /*!
			Makes a new empty polygon. You'll need to define it using addVertex() or addPoint().
            \param _c Vector representing the center (position) of the polygon.
        */
        Polygon(const Vector2d& _c = Vector2d(0,0))
                : verts(), pos(_c), radius(0.0f)
        {
        }

        //! Implict conversion from Rectangle
        /*!
        	This constructor makes a polygon from the given rectangle.
        */
        Polygon(const Rectangle& other);

        //! Destructor.
        virtual ~Polygon()
        {
        }

        //! Remove all the vertices from the polygon.
        inline void clear()
        {
            pos = Vector2d(0,0);
            radius = 0.0f;
            verts.clear();
        }

        //! The center of the polygon (The position).
        inline const Vector2d& getPosition() const
        {
            return pos;
        }

        //! Set position.
        /*!
            Sets the center (position) of the polygon. Note that the vertices are relative to the center
            of the polygon, therefore changing this value will not effect the shape of the polygon.
            \param _c The new center of the polygon.
            \sa getPosition()
        */
        inline void setPosition(const Vector2d& _c)
        {
            pos = _c;
        }

        //! Add vertex.
        /*!
            Adds a vertex to the polygon. It doesn't take into account the position of
            the polygon, so Vector2d(0.0f,10.0f) would add an vector that's 10 units
            above the polygon's center.
            \note This function does not sort the verticles by angle, you will need to call sort() if this polygon needs to be sorted.
            \sa addPoint()
        */
        inline void addVertex(const Vector2d& _v)
        {
            verts.push_back(_v);
            if (_v.getMagnitude()>radius){ radius = _v.getMagnitude(); }
        }

        //! Add point.
        /*!
            Adds a point to the polygon by making a vertex based on the position of the point and the center
            point of the polygon. Add point does care about where the polygon is:
            it takes the vector between our position and where the point is and puts it
            onto the polygon. Basically, it translates world coordiates to polygon coordinates.
            \note This function does not sort the verticles by angle, you will need to call sort() if this polygon needs to be sorted.
            \sa addVertex()
        */
        inline void addPoint(const Vector2d& _v)
        {
            verts.push_back( _v-pos );
            if ((_v-pos).getMagnitude()>radius){ radius = (_v-pos).getMagnitude(); }
        }

        //! Get the current number of vertices in the polygon.
        inline const unsigned int getVertexCount() const
        {
            return verts.size();
        }

		//! Remove a vertex.
		/*!
			\note This function works like a ring buffer. this is to reduce the complexity of certain
				geometric algorithms. 
		*/
		inline void removeVertex( const signed int& a )
		{
			verts.erase( verts.begin() + (a % verts.size()) );
		}

        //! Get a vertex.
        /*!
			\note This function acts like a ring buffer, this is to reduce the complexity of implementing some geometric algorithms.
        */
        inline const Vector2d& getVertex(const signed int& _i) const
        {
            return verts[ _i % verts.size() ];
        }

        //! Set a vertex's value.
        /*!
			\note This function acts like a ring buffer, this is to reduce the complexity of implementing some geometric algorithms.
        */
        inline void setVertex(const signed int& _i, const Vector2d& _v)
        {
            verts[ _i % verts.size() ] = _v;
            if ((_v).getMagnitude()>radius){ radius = (_v).getMagnitude(); }
        }

        //! Get the polygon's radius, the magnitude of the largest vertex in the polygon.
        inline float getRadius() const
        {
            return radius;
        }

        //! Updates the polygon's radius.
        /*!
            You should call this after modifying vertices with the
            element access operator[].
        */
        inline void update()
        {
            float radiussq = pow( radius, 2 );
            float maxmagsq = 0.0f;
            BOOST_FOREACH( Vector2d& vert, verts )
            {
                float magsq = vert.getMagnitudeSquared();
                if( magsq > maxmagsq ) maxmagsq = magsq;
            }
            if( maxmagsq != radiussq ) radius = sqrt( maxmagsq );
        }

        //! Rotate
        /*!
            Rotates the polygon by the given matrix.
        */
        inline void rotate(const RotationMatrix& _m)
        {
			BOOST_FOREACH( Vector2d& vert, verts )
			{
				vert *= _m;
			}
        }

        //! Sorts the vectors to ensure that they are wound counterclockwise.
        inline void sort()
        {
            std::sort( verts.begin(), verts.end(), &Vector2d::AngleSort );
        }

        //! Array operator
        /*!
            This allows you to treat the polygon like an array of vectors.
            \note If you modify the vertices in the Polygon, it is advisable to call update() before doing anything with the radius.
        */
        inline const Vector2d& operator[] (const unsigned int& _i)
        {
            return getVertex(_i);
        }

        //! Scalar Multiplaction ( Uniform Scale )
        inline const Polygon operator* (float rhs) const
        {
            Polygon temp = *this;
            BOOST_FOREACH( Vector2d& vert, temp.verts )
			{
                vert *= rhs;
            }
			temp.radius *= rhs;
            return temp;
        }

        //! Uniform scale and assignment
        inline const Polygon& operator*= (float rhs)
        {
            (*this) = (*this) * rhs;
            return (*this);
        }

        //! Rotation
        inline const Polygon operator* (const RotationMatrix& rhs) const
        {
            Polygon temp = *this;
            temp.rotate(rhs);
            return temp;
        }

        //! Rotate and assign
        inline const Polygon& operator*= (const RotationMatrix& rhs)
        {
            rotate(rhs);
            return *this;
        }

        //! Comparison Operator
        inline bool operator== (const Polygon& rhs) const
        {
            if (rhs.pos == pos&&rhs.radius == radius)
            {
                for (unsigned int i = 0; i < verts.size(); i++)
                {
                    if (!(verts[i] == rhs.verts[i]))
                    {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

	private:

        std::vector<Vector2d> verts; //!< Stores the list of verticies.
        Vector2d pos; //!< The vector representing the position of this polygon.
        float radius; //!< the magnitude of the largest vector in the polygon, use for circle-exclusion collision tests.

    }; //Polygon

} //namespace phoenix

#include "Rectangle.h"

#endif //__PHOENIXPOLY_H__
