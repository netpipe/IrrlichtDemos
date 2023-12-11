/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#ifndef __PHBATCHGEOMETRY_H__
#define __PHBATCHGEOMETRY_H__

#include <vector>
#include "config.h"
#include "Vertex.h"
#include "Texture.h"
#include "Rectangle.h"
#include "TrackingInvariant.h"
#include "BatchRenderer.h"
#include "Droppable.h"
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace phoenix
{

// workaround for GCC 4.7 unqualified call issue
using boost::intrusive_ptr;
using boost::intrusive_ptr_add_ref;
using boost::intrusive_ptr_release;

class BatchGeometry;

//! Friendly Batch Geometry Pointer
typedef boost::intrusive_ptr<BatchGeometry> BatchGeometryPtr;

//! Batch Geometry Class.
/*
	This class is used by the optimizing batch renderer to display geometry.
	Users are able to overload this class and highly customize it. This class
	is garbage collected and managed in very similar manner to Resource, but
	it should be noted that they are only alike in that they are both Droppable().
	Geometry is organized in the BatchRenderer by depth, group, texture id, and primitive type. 
	Any changes to any of these properties must be followed by an update() call.
*/
class BatchGeometry
    : public Droppable, boost::noncopyable
{

public:
    //! Default Constructor
    /*!
        The geometry is automatically added to the given BatchRenderer.
		\param _r The batch renderer that will display this geometry.
		\param _p The primitive type.
		\param _t The texture.
		\param _g The group id.
		\param _d The depth.
    */
	BatchGeometry(BatchRenderer& _r, unsigned int _p = GL_QUADS, TexturePtr _t = TexturePtr(), signed int _g = 0, float _d = 0.0f )
		: Droppable(), renderer(_r), primitivetype(_p), textureid( _t ? _t->getTextureId() : 0 ), texture(_t), groupid(_g), depth(_d), enabled(true), vertices(), immediate(false), clip(false)
	{
		_r.add( this );
	}

	//! Create from Rectangle
	/*!
		Exactly like the regular constructor but also calls fromRectangle().
	*/
	BatchGeometry( BatchRenderer& _r, const Rectangle& _rect, TexturePtr _t = TexturePtr(), signed int _g = 0, float _d = 0.0f )
        : Droppable(), renderer(_r), primitivetype( GL_QUADS ), textureid( _t ? _t->getTextureId() : 0 ), texture(_t), groupid(_g), depth(_d), enabled(true), vertices(), immediate(false), clip(false)
	{
		fromRectangle( _rect );
		_r.add( this );
	}

	//! Create from Polygon
	/*!
		Exactly like the regular constructor but also calls fromPolygon().
	*/
	BatchGeometry( BatchRenderer& _r, const Polygon& _poly, TexturePtr _t = TexturePtr(), signed int _g = 0, float _d = 0.0f )
        : Droppable(), renderer(_r), primitivetype( GL_TRIANGLES ), textureid( _t ? _t->getTextureId() : 0 ), texture(_t), groupid(_g), depth(_d), enabled(true), vertices(), immediate(false), clip(false)
	{
        fromPolygon( _poly );
		_r.add( this );
	}

	/*!
		Does nothing.
		\sa drop()
	*/
	virtual ~BatchGeometry()
	{
	}

    //! Grab
	/*!
		Grabs a pointer to this geometry. This is very akin to Resource::grab().
	*/
	template <typename T>
    inline boost::intrusive_ptr<T> grab()
    {
        try
        {
            return boost::static_pointer_cast<T,BatchGeometry>( this );
        }
        catch ( ... )
        {
            return boost::intrusive_ptr<T>();
        }
    }

	//! Drop
	/*!
		Drops this geometry from the renderer, and adds it to the garbage collection
		list. The geometry should be considered deleted. Derived classes should
		call this manually. This is very akin to Resource::drop().
		\sa dropped(), grab(), Droppable::drop(), Resource::drop()
	*/
	inline virtual void drop()
	{
		if( ! dropped() )
		{
			renderer.remove( this );
			Droppable::drop();
		}
	}

	//! Get vertex
	/*!
		\note This function works like a ring buffer. this is to reduce the complexity of certain geometric algorithms. 
	*/
	inline const Vertex& getVertex(const signed int& a) const
    {
        return vertices[ a % vertices.size() ];
    }

	//! Set vertex.
    /*!
		\note This function acts like a ring buffer, this is to reduce the complexity of implementing some geometric algorithms.
    */
    inline void setVertex(const signed int& a, const Vertex& v)
    {
        vertices[ a % vertices.size() ] = v;
    }


	//! Add vertex.
    inline void addVertex(const Vertex& a)
    {
        vertices.push_back(a);
    }

	//! Clear all vertices
	inline void clear() { vertices.clear(); }

	//! Remove vertex.
	/*!
		\note This function works like a ring buffer. this is to reduce the complexity of certain geometric algorithms. 
	*/
	inline void removeVertex( const signed int& a )
	{
		vertices.erase( vertices.begin() + (a % vertices.size()) );
	}

	//! The current number of vertices in the geometry.
    inline const unsigned int getVertexCount() const
    {
        return vertices.size();
    }

	//! Array operator for vertices. (operates as a ring buffer).
	inline Vertex& operator[] ( signed int _i ) { return vertices[ _i % vertices.size() ]; }

	//! Returns the invariant for the Primitive Type (used by BatchRender).
	inline TrackingInvariant< unsigned int >& getPrimitiveTypeInvariant() { return primitivetype; }

	//! Returns the invariant for the Textured ID (used by BatchRender).
	inline TrackingInvariant< unsigned int >& getTextureIdInvariant() { return textureid; }

	//! Returns the invariant for the Group ID (used by BatchRender).
	inline TrackingInvariant< signed int >& getGroupInvariant() { return groupid; }

	//! Returns the invariant for the Depth (used by BatchRender).
	inline TrackingInvariant< float >& getDepthInvariant() { return depth; }

	//! Get the texture associated with this geometry.
	inline TexturePtr getTexture() { return texture; }

	//! Get the OpenGL Primitive Type associated with this geometry.
	inline const unsigned int& getPrimitiveType() const { return primitivetype; }

	//! Get the OpenGL Texture ID associated with this geometry.
	inline const unsigned int& getTextureId() const { return textureid; }

	//! Get the Group ID associated with this geometry.
	inline const signed int& getGroup() const { return groupid; }

	//! Get the Depth associated with this geometry.
	inline float getDepth() const { return depth; }

	//! Check if this geometry is enabled.
	inline bool getEnabled() const { return enabled; }

	//! Check if this geometry is immediate.
	inline bool getImmediate() const { return immediate; }

	//! Set OpenGL Primitive type.
	/*
		Sets OpenGL primitive type of this piece of geometry. It defaults to
		GL_QUADS, but may be GL_TRIANGLES, GL_LINES, GL_POINTS, etc.
		\see getPrimitiveType(), update()
		\note update() must be called before this change will take effect!
	*/
	inline virtual void setPrimitiveType( const unsigned int& _v ) { primitivetype = _v; }

	//! Set Texture.
	/*!
		Sets the texture for this geometry. If it is an invalid texture or empty
		pointer, the Id is set to 0 to disable texturing for this geometry. 
		\see getTexture(), getTextureId(), update()
		\note update() must be called before this change will take effect!
	*/
	inline virtual void setTexture( TexturePtr _t ) 
	{ 
		texture = _t; 
		textureid = _t ? _t->getTextureId() : 0 ;
	}

	//! Set Group Id.
	/*
		Sets the Group identifier of this geometry. Geometries of the same group usually have the same properties and
		share render states that are set and unset via GroupStates in the BatchRenderer.
		\see getGroup(), update(), BatchRender::addGroupState()
		\note update() must be called before this change will take effect!
	*/
	inline virtual void setGroup( const signed int& _v ) { groupid = _v; }

	//! Set Depth
	/*!
		Sets the rendering depth of this geometry.
		\see getDepth(), update()
		\note update() must be called before this change will take effect!
	*/
	inline virtual void setDepth( float _v ) { depth = _v; }

	//! Enable or Disable.
	/*!
		Disabled geometry is skipped over during rendering.
		\see getEnabled()
	*/
	inline virtual void setEnabled( bool _e ) { enabled = _e; }

	//! Immediate rendering.
	/*!
		Immediate geometry is drawn once then dropped, so this is useful for non-persistent geometry. It's highly
		suggested that you only use this when needed, as persistent geometry is much faster.
		\see getImmediate(), drop()
	*/
	inline virtual void setImmediate( bool _i ) { immediate = _i; }

	//! Enable or Disable Clipping
	/*!
		If enabled, the geometry will be clipped by the Rectangle provided to setClippingRectangle(). This will cause the geometry not to be batched with other geometry
		of the same material. 
	*/
	inline virtual void setClipping( bool _c ){ clip = _c; }
	inline bool getClipping(){ return clip; }

	//! Set the Clipping Rectangle
	inline virtual void setClippingRectangle( const Rectangle& _r ){ clip_rect = _r; }

	//! Get the Clipping Rectangle
	inline const Rectangle& getClippingRectangle() { return clip_rect; }

	//! Update
	/*!
		This function will check all invariants and move the geometry's location in the renderer's graph
		if any of them have been broken, and then resets all the invariants. This function should be called
		after the depth, group, texture, or primitive type of this geometry is changed.
	*/
	virtual void update()
	{
		if( ! (primitivetype.check() && textureid.check() && groupid.check() && depth.check()) )
		{
			renderer.move( this );
			primitivetype.reset();
			textureid.reset();
			groupid.reset();
			depth.reset();
		}
	}

	//! Batch
	/*!
		This function is called by BatchRenderer when it draws the current render
		graph. The geometry should append all of it's vertices to the renderer's
		list.
		\param list The current vertex list from the current BatchRender.
		\param persist If true, immediate geometry will not drop itself.
	*/
	virtual unsigned int batch( std::vector<Vertex>& list, bool persist = false )
	{
		if( immediate && !persist )
		{
			drop();
		}

		if( enabled )
		{
			if( ! vertices.empty() )
			{
                std::vector<Vertex>::iterator verticesend = vertices.end();
                for( std::vector<Vertex>::iterator v = vertices.begin(); v != verticesend; ++v )
					list.push_back( *v );
			}
			return 1;
		}
		return 0;
	}

	//! Combine with another
	/*
		Combines this geometry with another geometry by pushing the vertices of 
		the other onto this one. This function does not perform any checks. It
		assume you know what you're doing. 
		\sa BatchGeometryComposite::combine
	*/
	virtual void combine( const BatchGeometryPtr& other, bool dropOther = true ) {
		vertices.reserve(vertices.size() + other->vertices.size());
		BOOST_FOREACH( Vertex& v, other->vertices ) {
			vertices.push_back( v );
		}

		if( dropOther ) {
			other->drop();
		}
	}

	//! Translate
	/*!
		Adds the given vector to each vertex in the geometry, translating the geometery.
		\f$ v_n = v_n + t \f$
	*/
	inline virtual void translate( const Vector2d& _t )
	{
		BOOST_FOREACH( Vertex& v, vertices )
		{
			v.position += _t;
		}
	}

	//! Scale
	/*!
		Multiples every vertex in the geometry the given vector linearly.
		\f$ v_n = ( v_{nx} + s_x , v_{ny} + s_y ) \f$
	*/
	inline virtual void scale( const Vector2d& _s )
	{
		BOOST_FOREACH( Vertex& v, vertices )
		{
			v.position.setX( v.position.getX() * _s.getX() );
			v.position.setY( v.position.getY() * _s.getY() );
		}
	}

	//! Rotate
	/*!
		Rotates each vertex by the given rotation matrix.
		\f$ v_n = v_n * m \f$
	*/
	inline virtual void rotate( const RotationMatrix& _m )
	{
		BOOST_FOREACH( Vertex& v, vertices )
		{
			v.position *= _m;
		}
	}

	//! Sets the color on all vertices.
	inline virtual void colorize( const Color& _c )
	{
		BOOST_FOREACH( Vertex& v, vertices )
		{
			v.color = _c;
		}
	}

	//! Define vertices using a Polygon.
	/*!
		Sets our vertices equal to the given Polygon. This is used by the polygon constructor.
		\note No colors or texture coords are set.
	*/
	inline void fromPolygon ( const Polygon& rhs )
	{
		setPrimitiveType( GL_TRIANGLES ); //primive type must be triangles.
		update();
		vertices.clear();
		//simple polygon to triangle expansion.
		if( rhs.getVertexCount() > 2 )
		{
			for( signed int i = 0; i < (signed int)rhs.getVertexCount(); ++i )
			{
				vertices.push_back( rhs.getPosition() + rhs.getVertex(i+1) );
                vertices.push_back( rhs.getPosition() + rhs.getVertex(i) );
                vertices.push_back( rhs.getPosition() );
			}
		}
	}

	//! Define vertices using a Rectangle.
	/*!
		Sets our vertices equal to the given Rectangle. The type is set to GL_QUADS. 
		The texture coordinates are automatically set to (0,0), (0,1), (1,1), (1,0). 
		This is used by the Rectangle constructor.
	*/
	inline void fromRectangle( const Rectangle &rhs )
	{
		setPrimitiveType( GL_QUADS ); //primive type must be quads.
		update();
		vertices.clear();
		vertices.push_back( Vertex( Vector2d(0,0), Color(), TextureCoords(0,0) ) );
		vertices.push_back( Vertex( Vector2d(0, rhs.getSize().getY() ), Color(), TextureCoords(0,1) ) );
		vertices.push_back( Vertex( rhs.getSize(), Color(), TextureCoords(1,1) ) );
		vertices.push_back( Vertex( Vector2d( rhs.getSize().getX(), 0 ), Color(), TextureCoords(1,0) ) );
		translate( rhs.getPosition() );
	}

protected:

	//! Renderer
	BatchRenderer& renderer;

	//! Primitive type
	/*
		This is the OpenGL primitive type of this piece of geometry. It defaults to
		GL_QUADS, but may be GL_TRIANGLES, GL_LINES, GL_POINTS, etc.
	*/
	TrackingInvariant<unsigned int> primitivetype;

	//! Texture ID
	/*
		The invariant of the Texture's ID. This is 0 if this geometry has no texture.
	*/
	TrackingInvariant< unsigned > textureid;

	//! Texture
	TexturePtr texture;

	//! Group ID.
	TrackingInvariant< signed int > groupid;

	//! Depth
	TrackingInvariant< float > depth;

	//! Enabled
	bool enabled;

	//! Vertices
	std::vector< Vertex > vertices;

	//! Immediate
	/*
		If geometry is immediate, it is dropped right after it is drawn, so it only stays for one frame.
	*/
	bool immediate;

	//! Clipping
	bool clip;

	//! Clip rectangle
	Rectangle clip_rect;
};

} //namespace phoniex

#endif // __PHBATCHGEOMETRY_H__
