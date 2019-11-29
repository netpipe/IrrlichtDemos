/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHRENDETARGET_H__
#define __PHRENDETARGET_H__

#include <boost/unordered_map.hpp>
#include "config.h"
#include "View.h"
#include "Resource.h"
#include "Texture.h"

namespace phoenix
{

	//! Render Target view management
	enum E_TARGET_VIEW_BEHAVIOR {
		TVB_NOTHING, //!< Do nothing, don't modify the view at all.
		TVB_RESIZE_VIEWPORT, //!< Default behavior, resizes the viewport to match the first texture size.
		TVB_RESIZE_PROJECTION, //!< Resizes just the projection matrix.
		TVB_RESIZE_BOTH, //!< Resizes both the viewport and the projection matrix.
	};

    //! Render Target
    /*!
        This class provides the ability to bind textures as the current render target. 
    */
    class RenderTarget
                : public Resource
    {

    public:

		//! Default Constructor
        /*!
			Create a framebuffer object, but does not create a texture. A texture must be attached before this can be used.
		*/
        RenderTarget( ResourceManager& r )
                : Resource(r), FBO_id(0), textures(32), view_behavior(TVB_RESIZE_VIEWPORT), old_view(Vector2d(0.0f,0.0f), Vector2d(0.0f,0.0f))
        {
            setName( "RenderTarget" );
			generateFramebufferObject();
        }

		//! Quick Constructor
        /*!
			Create a framebuffer object and a texture with the given size, and attaches the texture.
		*/
        RenderTarget( ResourceManager& r, Vector2d size )
                : Resource(r), FBO_id(0), textures(32), view_behavior(TVB_RESIZE_VIEWPORT), old_view(Vector2d(0.0f,0.0f), Vector2d(0.0f,0.0f))
        {
            setName( "RenderTarget" );
			generateFramebufferObject();

			TexturePtr t = new Texture( r, size );
			attach( t );
        }

        virtual ~RenderTarget()
        {
			if( GLEW_VERSION_2_0 && FBO_id){
				glDeleteFramebuffersEXT( 1, &FBO_id );
			}
        }

		//! Attaches a texture to the FBO.
		inline void attach( TexturePtr t, const GLuint _where = GL_COLOR_ATTACHMENT0_EXT ){

			textures[_where] = t;

			if( GLEW_VERSION_2_0  && FBO_id ){
				bind();
					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
								  (GLuint) _where,
								  GL_TEXTURE_2D,
								  t->getTextureId(),
								  0);
				unbind();
			}
		}
		
		//! Gets the currently attached texture
		inline TexturePtr getTexture( const GLuint _where = GL_COLOR_ATTACHMENT0_EXT ) { return textures[_where]; }


		//! Sets which buffers to draw to, this is analogous to glDrawBuffers/glDrawBuffer
		inline void setDrawBuffers( GLsizei count, const GLenum * bufs ){
			if( GLEW_VERSION_2_0  && FBO_id ){
				bind();
				
				glDrawBuffers( count,  bufs );

				unbind();
			}
		}

		inline void setDrawBuffer( const GLenum buf ){
			if( GLEW_VERSION_2_0  && FBO_id ){
				bind();
				
				glDrawBuffer( buf );

				unbind();
			}
		}

		//! Binds the FBO
		inline void bind(){ glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBO_id); }

		//! Unbinds the FBO
		inline void unbind(){ glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); }

        //! Binds the FBO and prepares to render, returns false if anything went wrong.
        inline bool start()
		{

			if( GLEW_VERSION_2_0  && FBO_id ){
				bind();

				if( glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT ) != GL_FRAMEBUFFER_COMPLETE_EXT ) return false;

				return true;
			}

			return false;

		}


        //! Unbinds the FBO
        inline void end()
		{
			if( GLEW_VERSION_2_0){
				unbind();
				
			}
		}

		//! Set the view resize behavior, only used when attached to a batch renderer.
		inline void setViewResizeBehavior( E_TARGET_VIEW_BEHAVIOR _b ) { view_behavior = _b; }

		//! Get the view resize behavior.
		inline E_TARGET_VIEW_BEHAVIOR getViewResizeBehavior() { return view_behavior; }

		//! Modifies the view, if needed
		/*!
			Automatically called by BatchRenderer if this is attached.
			Must be called manually for unattached targets.
		*/
		inline void modifyView(View& _view){
			old_view = _view;
			TexturePtr t = getTexture();

			if( view_behavior == TVB_RESIZE_VIEWPORT || view_behavior == TVB_RESIZE_BOTH){
				if( t ){
					_view.setSize( t->getSize() );
				}
			}

			if( view_behavior == TVB_RESIZE_PROJECTION || view_behavior == TVB_RESIZE_BOTH){
				if( t ){
					glMatrixMode(GL_PROJECTION);
					glPushMatrix();
					glLoadIdentity();
					glOrtho(0.0f, t->getSize().getX(), t->getSize().getY(), 0.0f, 1000.0f, -1000.0f);
					glMatrixMode(GL_MODELVIEW);
				}
			}
		}

		//! Restores the view to the last one that was passed to modifyView, also restore the projection matrix if it was modified.
		/*!
			Automatically called by BatchRenderer if this is attached.
		*/
		inline void restoreView(View& _view){
			if( view_behavior != TVB_NOTHING ){
				_view = old_view;

				if( view_behavior == TVB_RESIZE_PROJECTION || view_behavior == TVB_RESIZE_BOTH ){
					glMatrixMode(GL_PROJECTION);
					glPopMatrix();
					glMatrixMode(GL_MODELVIEW);
				}
			}
		}


	protected:

		//! Generate Framebuffer Object Id, returns true if everything was okay, or false if it was unable to do so.
		bool generateFramebufferObject(){
			if( GLEW_VERSION_2_0 ){
				glGenFramebuffersEXT( 1, &FBO_id );
				return FBO_id ? true : false;
			}
			return true;
		}

		GLuint FBO_id;
		boost::unordered_map<GLuint, TexturePtr> textures;
		E_TARGET_VIEW_BEHAVIOR view_behavior;
		View old_view;
    };

    typedef boost::intrusive_ptr<RenderTarget> RenderTargetPtr;
} //namespace phoenix

#endif // __PHRENDETARGET_H__
