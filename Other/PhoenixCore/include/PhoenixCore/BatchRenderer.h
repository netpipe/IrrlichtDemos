/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/


#ifndef __PH_BATCH_RENDERER_H__
#define __PH_BATCH_RENDERER_H__

#include <map>
#include <vector>
#include <iostream>
#include <boost/unordered_map.hpp>
#include <boost/thread.hpp>
#include "config.h"
#include "Shader.h"
#include "AbstractGarbageCollector.h"
#include "View.h"
#include "Droppable.h"
#include "GroupState.h"
#include "Vertex.h"
#include "Rectangle.h"
#include "RenderTarget.h"

namespace phoenix
{

// workaround for GCC 4.7 unqualified call issue
using boost::intrusive_ptr;
using boost::intrusive_ptr_add_ref;
using boost::intrusive_ptr_release;

class BatchGeometry;

//! Optimizing Batch Renderer.
/*!
	The Optimizing Batch Renderer is the soul of phoenix's rendering framework. All drawing calls 
	go through the batching renderer. The main purpose of the batching renderer is to store all geometry 
	and draw it at once, performing as many optimizations as possible. This adds a slight layer of complexity, 
	but the speed tradeoff is well worth it. BatchGeometry is automatically sorted in a graph based on
	depth, group, texture, and primitive type (in that order).
*/
class BatchRenderer
	: public AbstractGarbageCollector
{

public:

	//! Constructor
	/*!
		Initializes the geometry graph and starts the garbage collection routines.
	*/
	BatchRenderer( )
		: AbstractGarbageCollector(), geometry(), recyclelist(), groupstates(), shader(), target(), clear_color(0,0,0), enable_clear(false),persist_immediate(false)
	{
		//collect fast.
		setSleepTime( 5 );
        setCollectionRate( 2 );
	}

	//! Destructor
	/*!
		Releases all references to geometry. 
	*/
	virtual ~BatchRenderer()
	{
		clear(); //drop all geometry.
	}

	//! Add geometry to the render graph. (Automatically called by BatchGeometry::create() ).
	void add( boost::intrusive_ptr<BatchGeometry> _g );

	//! Add geometry to the recycle list. ( Automatically called by BatchGeometry::drop() ).
	void remove( boost::intrusive_ptr<BatchGeometry> _g );

	//! Update a geometry's position in the graph. ( Automatically called by BatchGeometry::update() ).
	void move( boost::intrusive_ptr<BatchGeometry> _g );

	//! Drops all geometry.
	void clear()
	{
		lock();
		recyclelist.clear();
		geometry.clear();
		unlock();
	}

	//! Counts all the geometry in the list (may be slow). 
	unsigned int count();

    //! Cleaning routine
	void clean();

	//! Sets the group state for a given group id.
	inline void addGroupState( const signed int _id, GroupStatePtr _gs ){
		groupstates[_id] = _gs; 
	}

	//! Removes the group state for the given group id.
	inline void removeGroupState( const signed int _id ){
		groupstates.erase( _id );
	}

	//! Gets the group state for the given group id.
	inline GroupStatePtr getGroupState( const signed int _id ){
		return groupstates[_id];
	}

    //! Sets the renderer's view.
    inline void setView( const View& other ) { view = other; }

    //! Gets a reference to the renderer's view.
    inline View& getView() { return view; }

	//! Sets the renderer's target, if an empty pointer, the target is the window's framebuffer
    inline void setRenderTarget( RenderTargetPtr _t = RenderTargetPtr() ) { target = _t; }

    //! Gets a reference to the renderer's view.
    inline RenderTargetPtr getRenderTarget() {return target; }

	//! Sets the renderer's shader. If an empty pointer, the renderer will use the FFP
	inline void setShader( ShaderPtr _s ) { shader = _s; }

	//! Get the renderer's shader.
	inline ShaderPtr getShader(){ return shader; }

	//! Draws everything in the graph.
	/*
		\param pesist_immediate If set to true, immediate geometry will persist until the next time draw() is called, this is useful for RTT.
	*/
	void draw( bool _persist_immediate = false );

	//! Draws a single geometry immediately
	/*!
		Automatically activates any attached rendertarget, and deactivates it before returning.
	*/
	void drawImmediately(  boost::intrusive_ptr<BatchGeometry> geom );

	//! Clears the screen to the given color. Does not activate the current render target.
    inline static void clearScreen( const Color& _c = Color(0,0,0) )
    {
        glClearColor( _c.getRed()/255.0f,_c.getGreen()/255.0f,_c.getBlue()/255.0f,_c.getAlpha()/255.0f );
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }

	//! Get the default clear color
    inline const Color& getClearColor() { return clear_color; }

    //! Set the default clear color
    inline void setClearColor(const Color& c) { clear_color = c; }

	//! Enable/disable clearing (disabled by default, except for the rendersystem's batcher).
	inline void setClearing( bool c ){ enable_clear = c; }

#ifdef DEBUG_BATCHRENDERER
	//! Lists all the geometry in the list.
	void listGeometry();
#endif

private:

	typedef std::list< boost::intrusive_ptr<BatchGeometry> > GEOMCONTAINER;
	typedef boost::unordered_map< unsigned int, GEOMCONTAINER > BATCHMAPALPHA; // Primitive Keyed
	typedef boost::unordered_map< unsigned int, BATCHMAPALPHA > BATCHMAPBETA; // Texture Keyed
	typedef boost::unordered_map< signed int, BATCHMAPBETA > BATCHMAPGAMMA; // Group Keyed
	typedef std::map< float, BATCHMAPGAMMA > BATCHMAPDELTA; // Depth Keyed (Ordered)

	//! Geometry List Container.
	BATCHMAPDELTA geometry;

	//! Recycle list
	std::vector< boost::intrusive_ptr<BatchGeometry> > recyclelist;

	typedef boost::unordered_map< signed int, boost::shared_ptr<GroupState> > GROUPSTATEMAP;
	//! Map of group states.
	GROUPSTATEMAP groupstates;

    //! View
    View view;

	//! Current shader
	ShaderPtr shader;

	//! Current render target
	RenderTargetPtr target;

	//! GL clear color
    Color clear_color;
	bool enable_clear;

	//! Immediate persistence
	bool persist_immediate;

	//! Real removal routine ( used by clean() and move() ).
	void removeProper( boost::intrusive_ptr<BatchGeometry> _g , bool _inv = false);

	//! Clipping Routine
	bool clipGeometry(  boost::intrusive_ptr<BatchGeometry> geom, bool &clipping, phoenix::Rectangle &clipping_rect );

	//! Vertex submission routine.
	void submitVertexList( std::vector< Vertex >& vlist, unsigned int type );
};

} //namespace phoenix

#include "BatchGeometry.h"

#endif //__PH_BATCH_RENDERER_H__
