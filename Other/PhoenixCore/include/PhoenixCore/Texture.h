/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHOENIXTEX_H__
#define __PHOENIXTEX_H__

#include <string>
#include <boost/noncopyable.hpp>
#include "config.h"
#include "Color.h"
#include "Vector2d.h"
#include "Resource.h"

namespace phoenix
{

    //! Texture class.
    /*!
        Provides a high-level container for OpenGL Texture Objects. This class provides methods to create, bind, and
		modify textures.
    */
    class Texture
        : public Resource, boost::noncopyable
    {

        friend class Resource;

	public:


        //! Constructor
        /*!
            Constructor that creates a texture object with the given dimensions.
            \param _t A resource manager (for garbage collection).
            \param _s The size of the texture.
            \note The resource type for Textures is always ERT_TEXTURE.
        */
        Texture(ResourceManager& t, const Vector2d& _s = Vector2d(0,0))
			: Resource(t,1), texture(0), width(0), height(0), data(NULL) 
        {
            setName( "Untitled" );
			build(_s);
        }

        //! Destructor
        /*!
            Destroys this texture object and deletes the OpenGL texture.
        */
        virtual ~Texture()
		{
			if (glIsTexture(texture))
			{
			   glDeleteTextures(1, &texture);
			}
			if (data!=NULL)
			{
				delete [] data;
			}
		}


        //! Creates a solid texture with the given size and color.
        virtual void build( const Vector2d& _s, const Color& _c = Color(255,255,255) );

        //! Set texture ID.
        /*!
            Sets the OpenGL texture identifier of this texture. Should never be called directly by the user, but
            can be used by custom image loading routines.
        */
        inline void setTextureId(GLuint _t) { texture = _t; }

        //! Get the OpenGL texture identifier.
        inline GLuint getTextureId() const { return texture; }

        //! Changes the texture's width (Must be a power of 2).
        inline void setWidth(int _w) { width = _w; }

        //! Get the current width of the texture.
        inline int getWidth() const { return width; }

		//! Get write access to the data
		inline GLubyte* getData(){ return data; }

        //! Changes the texture's height (Must be a power of 2).
        inline void setHeight(int _h) { height = _h; }

        //! Get height.
        inline int getHeight() const { return height; }

        //! Get size.
        inline const Vector2d getSize() const { return Vector2d( (float) width,  (float) height ); }

        //! Set size.
		/*!
			\note Texture sizes must be a power of 2.
			\note This is a dangerous function, as it doesn't tell opengl about the new size, therefore this is not a valid way to resize a texture.
		*/
        inline void setSize(const Vector2d& sz)
        {
			width = int(sz.getX());
			height = int(sz.getY());
		}

        //! Lock.
        /*!
            Locks the texture. Before getPixel() or setPixel() can be called, this must be called first.
            \return True if it was able to lock the texture.
            \sa unlock(), setPixel(), getPixel()
        */
        bool lock();

        //! Unlock.
        /*!
            Unlocks the texture. It transfers all the texture data back into video memory. This must be called
            after the user is done modifying the texture with setPixel().
            \sa lock(), setPixel(), getPixel()
        */
        void unlock();

		//! Unlock (BGRA Version)
		/*!
			Uploads the texture to video memory assuming a BGRA format.
			\note Advanced
			\sa unlock()
		*/
		void unlock(bool BGRA);

        //! Changes a pixel to the given color. lock() must be called before this is possible.
        void setPixel( const Vector2d& _p, const Color& _c);

        //! Gets the color of the given pixel. lock() must be called before this is possible.
        const Color getPixel( const Vector2d& _p ) const;

        //! Binds this texture as the current OpenGL texture use for drawing.
        inline bool bind()
		{
			if (glIsTexture(texture)) //make sure this is a texture
			{
				glBindTexture(GL_TEXTURE_2D, texture); //set the texture
				return true;
			}
			return false;
		}

        //! Makes a hard (separate) copy of the texture.
		boost::intrusive_ptr<Texture> copy();

	protected:

        //! Pointer to the OpenGL Texture.
        GLuint texture;

        //! Width of the texture.
        int width;

        //! Height of the texture.
        int height;

        //! Data used during texture manipulation.
        /*!
            \sa lock(),unlock(),setPixel(),getPixel()
        */
        GLubyte* data;

    };

    //! Friendly name for texture pointers
    typedef boost::intrusive_ptr<Texture> TexturePtr;

}//namespace phoenix

#endif //__PHOENIXTEX_H__
