/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#ifndef __PHBATCHGEOMETRYCOMP_H__
#define __PHBATCHGEOMETRYCOMP_H__

#include "BatchGeometry.h"

namespace phoenix
{

//! Batch Geometry Composite Class.
/*
	Implements the composite pattern for batch geometry. Differences are noted. 
*/
class BatchGeometryComposite
    : public BatchGeometry
{

public:

	/*! Default constructor */
	BatchGeometryComposite( BatchRenderer& _r )
		: BatchGeometry(_r), geoms()
	{
	}


	/*!
		Does nothing.
		\sa drop()
	*/
	virtual ~BatchGeometryComposite()
	{
		geoms.clear();
	}

	//! Add a child geom
	inline void add( BatchGeometryPtr g ){ geoms.push_back(g); }

	//! Remove a child geom
	inline void remove( BatchGeometryPtr g ){ 
		geoms.erase( std::remove(geoms.begin(),geoms.end(),g), geoms.end() ); 
	}

	//! Removea all children
	inline void clear() { geoms.clear(); }

	//! Get children
	inline std::vector< BatchGeometryPtr >& getChildren() { return geoms; }

	//! Drop
	/*!
		Also drops all children.
	*/
	inline virtual void drop()
	{
		BatchGeometry::drop();
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->drop();
		}
	}

	inline virtual void drop(bool skip_children /* = true */){
		if(skip_children) BatchGeometry::drop();
		else drop();
	}

	//! Set OpenGL Primitive type.
	/*
		Affects all children.
	*/
	inline virtual void setPrimitiveType( const unsigned int& _v ) {
		BatchGeometry::setPrimitiveType(_v);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->setPrimitiveType(_v);
		}
	}

	//! Set Texture.
	/*!
		Affects all children
	*/
	inline virtual void setTexture( TexturePtr _t ) 
	{ 
		BatchGeometry::setTexture(_t);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->setTexture(_t);
		}
	}

	//! Set Group Id.
	/*
		Affects all children
	*/
	inline virtual void setGroup( const signed int& _v )
	{ 
		BatchGeometry::setGroup(_v);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->setGroup(_v);
		}
	}

	//! Set Depth
	/*!
		Affects all children
	*/
	inline virtual void setDepth( float _v )
	{ 
		BatchGeometry::setDepth(_v);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->setDepth(_v);
		}
	}

	//! Enable or Disable.
	/*!
		Affects all children
	*/
	inline virtual void setEnabled( bool _e )
	{ 
		BatchGeometry::setEnabled(_e);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->setEnabled(_e);
		}
	}

	//! Immediate rendering.
	/*!
		Affects all children
	*/
	inline virtual void setImmediate( bool _i )
	{ 
		BatchGeometry::setImmediate(_i);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->setImmediate(_i);
		}
	}

	//! Update
	/*!
		Affects all children
	*/
	virtual void update()
	{ 
		BatchGeometry::update();
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->update();
		}
	}

	//! Batch
	/*!
		Does nothing except the immediate check. (does not batch children, that is automatic)
	*/
	virtual unsigned int batch( std::vector<Vertex>& list, bool persist = false )
	{
		if( immediate && !persist )
		{
			drop(true); //children should all be immediate, so they should be collected automatically, no need for us to preemptively drop them, will cause them not to be drawn.
		}
		return 0;
	}

	//! Overridden Combine
	/*!
		Does nothing, but may drop the other geometry
	*/
	virtual void combine( const BatchGeometryPtr& other, bool dropOther = true ) {
		if( dropOther ) {
			other->drop();
		}
	}

	//! Overloaded Combine w/ Other
	/*!
		Combines all children and returns a new geometry.
	*/
	void combine( const boost::intrusive_ptr<BatchGeometryComposite>& other, bool dropOther = true) {
		BOOST_FOREACH( BatchGeometryPtr& g, other->geoms ){
			this->add(g);
		}
		if( dropOther ) other->drop(true);
	}

	//! Overloaded CNullary ombine
	/*!
		Combines all children and returns a new geometry.
	*/
	BatchGeometryPtr combine(bool dropChildren = true) {
		BatchGeometryPtr new_geom = new BatchGeometry(renderer, primitivetype, texture, groupid, depth);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			new_geom->combine(g,dropChildren);
		}
		if( dropChildren ) drop();
		return new_geom;
	}

	//! Translate
	/*!
		Affects all children
	*/
	inline virtual void translate( const Vector2d& _t )
	{ 
		BatchGeometry::translate(_t);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->translate(_t);
		}
	}

	//! Scale
	/*!
		Affects all children
	*/
	inline virtual void scale( const Vector2d& _s )
	{ 
		BatchGeometry::scale(_s);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->scale(_s);
		}
	}

	//! Rotate
	/*!
		Affects all children
	*/
	inline virtual void rotate( const RotationMatrix& _m )
	{ 
		BatchGeometry::rotate(_m);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->rotate(_m);
		}
	}

	//! Sets the color on all vertices. Affects all children
	inline virtual void colorize( const Color& _c )
	{ 
		BatchGeometry::colorize(_c);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->colorize(_c);
		}
	}

	//! Enables/disables clipping. Affects all children.
	inline virtual void setClipping( bool _c )
	{ 
		BatchGeometry::setClipping(false);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->setClipping(_c);
		}
	}

	//! Sets the clipping rectangle. Affects all children.
	inline virtual void setClippingRectangle( const Rectangle& _r )
	{ 
		BatchGeometry::setClippingRectangle(_r);
		BOOST_FOREACH( BatchGeometryPtr& g, geoms ){
			g->setClippingRectangle(_r);
		}
	}

protected:

	//! Geoms
	std::vector< BatchGeometryPtr > geoms;

};

typedef boost::intrusive_ptr<BatchGeometryComposite> BatchGeometryCompositePtr;

} //namespace phoniex

#endif // __PHBATCHGEOMETRY_H__
