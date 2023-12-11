// Copyright 2017 Nicolaus Anderson
// See license.txt for details.

#include "../include/irragg_array_path2d.h"

namespace irr {
namespace vecg {

	//! cstor
	ArrayVertexSource::ArrayVertexSource()
		: data()
	{}

	//! copy cstor
	ArrayVertexSource::ArrayVertexSource(
		const ArrayVertexSource&  other
	)
		: data( other.data )
	{}

	//! cstor
	ArrayVertexSource::ArrayVertexSource(
		const ::irr::core::array< Point2D >&  data_array
	)
		: data( data_array )
	{}

	//! cstor
	ArrayVertexSource::ArrayVertexSource(
		const ::irr::core::array< ::irr::core::vector2di >&  data_array
	)
		: data()
	{
		setFrom(data_array);
	}

	//! cstor
	ArrayVertexSource::ArrayVertexSource(
		const size_t  num_values,
		const double*  x_values,
		const double*  y_values
	)
		: data()
	{
		setFrom(num_values, x_values, y_values);
	}

	//! Set From Given Array
	void
	ArrayVertexSource::setFrom( const ArrayVertexSource&  other )
	{
		data = other.data;
	}

	//! Set From Given Array
	void
	ArrayVertexSource::setFrom( const ::irr::core::array< Point2D >&  data_array )
	{
		data = data_array;
	}

	//! Set From Given Array
	void
	ArrayVertexSource::setFrom( const ::irr::core::array< ::irr::core::vector2di >&  data_array )
	{
		u32 i = 0;
		u32 da_size = data_array.size();

		data.reallocate(da_size);

		for (; i < da_size; ++i )
		{
			data.push_back( Point2D( data_array[i].X, data_array[i].Y ) );
		}
	}

	//! Set From Given Array
	void
	ArrayVertexSource::setFrom(
			const size_t  num_values,
			const double*  x_values,
			const double*  y_values
	) {
		size_t i = 0;

		data.reallocate(num_values);

		for (; i < num_values; ++i )
		{
			data.push_back( Point2D( x_values[i], y_values[i] ) );
		}
	}

	//! Add a point to the array
	ArrayVertexSource&
	ArrayVertexSource::add( double  x, double  y )
	{
		data.push_back( Point2D(x,y) );
		return *this;
	}

	//! Add a point to the array
	ArrayVertexSource&
	ArrayVertexSource::add( const Point2D&  point )
	{
		data.push_back( point );
		return *this;
	}

	//! Remove last point in the array
	size_t
	ArrayVertexSource::pop( size_t  num_points_to_remove )
	{
		data.erase(data.size()-1, num_points_to_remove);
		return data.size();
	}

	//! Delete all points
	void
	ArrayVertexSource::clear()
	{
		data.clear();
	}

	//! Number of points stored here
	size_t
	ArrayVertexSource::getPointCount()
	{
		return data.size();
	}

	//! Prepare
	void
	ArrayVertexSource::prepare( size_t  num_points_to_expect )
	{
		data.reallocate(num_points_to_expect);
	}

	//! AGG template requirement: vertex return
	void
	ArrayVertexSource::rewind( unsigned )
	{
		_step = 0;
	}

	//! AGG template requirement: vertex return
	unsigned
	ArrayVertexSource::vertex( double*  x, double*  y )
	{
		if ( _step > data.size() + 1 )
			return ::agg::path_cmd_stop;

		if ( _step == data.size() )
		{
			_step++;
			return ::agg::path_cmd_stop;
			//return ::agg::path_cmd_end_poly;
			// Oddity: returning path_cmd_end_poly sets the drawing brush back to zero for some reason
		}

		*x = data[_step].x;
		*y = data[_step].y;
		_step++;

		if ( _step == 1 )
			return ::agg::path_cmd_move_to;

		return ::agg::path_cmd_line_to;
	}

	//! Get the point at the given index
	Point2D
	ArrayVertexSource::getPoint( size_t index )
	{
		if ( index < data.size() )
			return data[index];

		return Point2D();
	}

} // end namespace vecg
} // end namespace irr
