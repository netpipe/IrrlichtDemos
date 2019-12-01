// Copyright 2017 Nicolaus Anderson
// See license.txt for details.

#ifndef __IRR_AGG_VERTEX_SOURCES_2D_H__
#define __IRR_AGG_VERTEX_SOURCES_2D_H__

//*** Anti-Grain Geometry ***
#include <agg_basics.h> // For VertexSourcePump

//*** IrrAgg ***
#include "irragg_config.h"
#include "irragg_point.h"

namespace irr {
namespace vecg {

	//! Path Vertex Base
	/*
		Contains some odds and ends for creating vertex sources.
	*/
	class VertexSourceBase
	{
	protected:
		unsigned _step;
		double _origin_x;
		double _origin_y;

	public:
		//! cstor
		VertexSourceBase();

		//! cstor
		VertexSourceBase( double  origin_x, double  origin_y );

		//! dstor
		virtual ~VertexSourceBase() {}

		//! Set starting coordinates for the vertex.
		void
			setOrigin( double  x, double  y );

		//! Get the initial position
		Point2D
			getOrigin();
	};


	//! Vertex Source Pump
	/*
		Meant for creating a single polygon, this class is a vertex source template
		for wrapping a class having the following methods:
		-	Point2D  getPoint( unsigned step );
		-	unsigned  getPointCount();
	*/
	template< class VertexSource >
	class VertexSourcePump
		: public VertexSourceBase
	{
		VertexSource&  vertex_source;

	public:
		//! cstor
		VertexSourcePump( VertexSource& source )
			: VertexSourceBase()
			, vertex_source(source)
		{}

		//! AGG template requirement: reset
		void
			rewind( unsigned )
		{
			_step = 0;
		}

		//! AGG template requirement: vertex return
		unsigned
			vertex( double*  x, double*  y )
		{
			if ( _step > vertex_source.getPointCount() )
				return ::agg::path_cmd_stop;

			Point2D  p = vertex_source.getVertex( _step );
			*x = p.x + _origin_x;
			*y = p.y + _origin_y;
			_step++;

			if ( _step == vertex_source.getPointCount() )
				return ::agg::path_cmd_end_poly;

			if ( _step == 0 )
				return ::agg::path_cmd_move_to;

			return ::agg::path_cmd_line_to;
		}
	};


	//! Vertex Source Function Definition
	/*
		\param step -The current point along the path that the function
			should return. This is usually the same as the number of times this function
			has been called in a row.
		\return - The point at the current step.
	*/
	typedef Point2D (*VertexSourceFunction)(unsigned);


	//! Vertex Source Pump for Functions
	/*
		It accepts a point count value and a pointer to a VertexSourceFunction.
		When vertices are needed, this will call the given function for a number of
		times equal to the given point count.
		Example usage:

			Point2D  func(unsigned step) {
				Point2D  p;
				p.x = step;
				p.y = sin(p.x);
				return p;
			}

			VertexSourceFuncPump  pump(5,func);
			//...
			renderVertexPath(pump, image, color);
	*/
	class VertexSourceFuncPump
		: public VertexSourceBase
	{
		unsigned  _num_points;
		VertexSourceFunction  _function;

	public:
		//! cstor
		VertexSourceFuncPump(unsigned  num_points, VertexSourceFunction  function);

		//! AGG template requirement: reset
		void
			rewind( unsigned );

		//! AGG template requirement: vertex return
		unsigned
			vertex( double*  x, double*  y );
	};

} // end sub namespace vecg
} // end namespace irr

#endif // __IRR_AGG_VERTEX_SOURCES_2D_H__
