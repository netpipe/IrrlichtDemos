// Copyright 2017 Nicolaus Anderson
// see license.txt for details

#ifndef __IRR_AGG_MATRIX_VERTEX_SOURCE_H__
#define __IRR_AGG_MATRIX_VERTEX_SOURCE_H__

//*** Anti-Grain Geometry ***
#include <agg_trans_affine.h>

//*** Irrlicht ***
#include <matrix4.h>

//*** IrrAgg ***
#include "irragg_matrix.h"


namespace irr {
namespace vecg {

	//! Matrix vertex source
	/*
		A basic matrix vertex source that wraps another vertex source and
		applies a matrix to its points.
		WARNING: This saves merely a pointer to the vertex source. If the vertex source
			is deleted elsewhere, this structure must be reset using attach() or destroyed
			before continued usage.
	*/
	template< class VertexSource >
	class MatrixVertexSource
	{
		VertexSource* _vertex_source;
		::agg::trans_affine  _matrix;

	public:
		//! cstor
		MatrixVertexSource()
			: _vertex_source(IRR_AGG_NULL)
			, _matrix()
		{}

		//! cstor
		MatrixVertexSource( VertexSource*  vertex_source )
			: _vertex_source(vertex_source)
			, _matrix()
		{}

		//! Set the vertex source
		void
			attach( VertexSource*  vertex_source )
		{
			_vertex_source = vertex_source;
		}

		//! Set from Irrlicht matrix
		void
			set( const ::irr::core::CMatrix4<double>&  new_matrix )
		{
			_matrix = ::irr::core::convertCMatrix4ToAGGaffine(new_matrix);
		}

		//! Set from AGG matrix
		void
			set( const ::agg::trans_affine&  new_matrix )
		{
			_matrix = new_matrix;
		}

		//! AGG template requirement: reset
		void
			rewind(unsigned)
		{
			if ( _vertex_source )
				_vertex_source->rewind();
		}

		//! AGG template requirement: vertex return
		unsigned
			vertex( double*  x, double*  y )
		{
			unsigned result = ::agg::path_cmd_stop;
			if ( _vertex_source )
			{
				result = _vertex_source->vertex(x,y);
				_matrix.transform(x,y);
			}
			return result;
		}
	};

} // end sub namespace vecg
} // end namespace irr

#endif // __IRR_AGG_MATRIX_VERTEX_SOURCE_H__
