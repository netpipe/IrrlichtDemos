// Copyright 2017 Nicolaus Anderson
// see license.txt for details

#ifndef __IRR_AGG_ARRAY_PATH_2D_H__
#define __IRR_AGG_ARRAY_PATH_2D_H__

//*** Irrlicht ***
#include <irrArray.h>
#include <vector2d.h>

//*** IRRAGG ***
#include "irragg_vertexsources.h"

namespace irr {
namespace vecg {


	//! Basic Static-Size Vertex Source
	/*
		Self-contained unit acting as a vertex source.

		Example usage:
		[code]
			StaticSizeVertexSource<3>  path;
			path
				.set(0, 10, 100)
				.set(1, 20, 0)
				.set(2, 30, -100);

			//...
			renderVectorPath(path, image, irr::core::SColor(0xffff0000));
		[/code]
	*/
	template<size_t Size>
	class StaticSizeVertexSource
		: public VertexSourceBase
	{
		double _x[Size];
		double _y[Size];

	public:
		//! cstor
		StaticSizeVertexSource()
			: VertexSourceBase()
		{
			size_t  i = 0;
			for (; i < Size; ++i)
			{
				_x[i] = 0;
				_y[i] = 0;
			}
		}

		//! Set point coords of given index
		StaticSizeVertexSource&
			set( size_t  index, double  x, double  y )
		{
			if ( index < Size )
			{
				_x[index] = x;
				_y[index] = y;
			}
			return *this;
		}

		//! Get size
		size_t
			getPointCount()
		{
			return Size;
		}

		//! AGG template requirement: reset
		void
			rewind(unsigned)
		{
			_step = 0;
		}

		//! AGG template requirement: vertex return
		unsigned
			vertex( double*  x, double*  y )
		{
			if ( _step > Size + 1 )
				return ::agg::path_cmd_stop;

			if ( _step == Size )
			{
				_step++;
				return ::agg::path_cmd_stop;
				//return ::agg::path_cmd_end_poly;
				// Oddity: returning path_cmd_end_poly sets the drawing brush back to zero for some reason
			}

			*x = _x[_step];
			*y = _y[_step];
			_step++;

			if ( _step == 1 )
				return ::agg::path_cmd_move_to;

			return ::agg::path_cmd_line_to;
		}

		//! Get the point at the given index
		Point2D
			getPoint( size_t index )
		{
			if ( index < Size )
				return Point2D( _x[index], _y[index] );
			else
				return Point2D();
		}
	};


	//! Irrlicht Array Vertex Source
	/*
		Container that acts as a vertex source.
		It can be passed directly to render functions that accept a raw vertex source.

		Example Usage:
			ArrayVertexSource path;
			path
				.add( 10, 100 )
				.add( 100, 10 )
				.add( 200, -100 );
	*/
	class ArrayVertexSource
		: public VertexSourceBase
	{
		// TODO: Replace with a better array container.
		// irr::core::array is naturally insecure due to method of allocation but
		// can have its allocator replaced.
		// However, it also doesn't use size_t for indexing.
		::irr::core::array<Point2D>  data;

	public:
		//! cstor
		ArrayVertexSource();

		//! copy cstor
		ArrayVertexSource( const ArrayVertexSource&  other );

		//! cstor
		ArrayVertexSource( const ::irr::core::array< Point2D >&  data_array );

		//! cstor
		ArrayVertexSource( const ::irr::core::array< ::irr::core::vector2di >&  data_array );

		//! cstor
		/*
			Construct a vertex source from two arrays of size num_values.
			WARNING: Does NOT check for premature array end!
		*/
		ArrayVertexSource(
			const size_t  num_values,
			const double*  x_values,
			const double*  y_values
		);

		//! Set From Given Array
		/*
			Sets this vertex source from the points in the given array.
		*/
		void
			setFrom( const ArrayVertexSource&  other );

		//! Set From Given Array
		/*
			Sets this vertex source from the points in the given array.
		*/
		void
			setFrom( const ::irr::core::array< Point2D >&  data_array );

		//! Set From Given Array
		/*
			Sets this vertex source from the points in the given array.
		*/
		void
			setFrom( const ::irr::core::array< ::irr::core::vector2di >&  data_array );

		//! Set From Given Array
		/*
			Sets this vertex source from the points in the given array.
			WARNING: Does NOT check for premature array end!
		*/
		void
			setFrom(
				const size_t  num_values,
				const double*  x_values,
				const double*  y_values
		);

		//! Add a point to the array
		/*
			\return - Pointer to this, allowing builder notation.
		*/
		ArrayVertexSource&
			add( double  x, double  y );

		//! Add a point to the array
		/*
			\return - Pointer to this, allowing builder notation.
		*/
		ArrayVertexSource&
			add( const Point2D&  point );

		//! Remove last points in the array
		/*
			Returns the number of remaining points.
		*/
		size_t
			pop( size_t  num_points_to_remove=1 );

		//! Delete all points
		void
			clear();

		//! Number of points stored here
		size_t
			getPointCount();

		//! Prepare
		/*
			Performs space allocation in preparation for adding points.
		*/
		void
			prepare( size_t  num_points_to_expect );

		//! AGG template requirement: reset
		void
			rewind(unsigned);

		//! AGG template requirement: vertex return
		unsigned
			vertex( double*  x, double*  y );

		//! Get the point at the given index
		Point2D
			getPoint( size_t index );
	};


	// TODO: List Vertex Source
	// Should allow for easy setting, insertion, growth, shrinkage, etc.


} // end sub namespace agg
} // end namespace irr

#endif // __IRR_AGG_ARRAY_PATH_2D_H__
