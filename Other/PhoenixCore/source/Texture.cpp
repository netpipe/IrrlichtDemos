 /*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#include "Texture.h"

using namespace phoenix;


/*!--------------------------
Build a blank texture
----------------------------*/
void Texture::build( const Vector2d& _s, const Color& _c )
{

    unsigned int a = (unsigned int)_s.getX();
	unsigned int b = (unsigned int)_s.getY();

	//! If they supplied width and height, lets make a blank texture.
	if( (a>0) && (b>0) )
	{

		// let our texture know what it's new width and height will be.
	    setWidth( a );
	    setHeight( b );

		//make some room for the texture's data
		data = new GLubyte[a*b*4];

		//make all the pixels the given color.
		for( unsigned int i = 0; i < a; ++i )
		{
			for( unsigned int j = 0; j < b; ++j )
			{
				setPixel( Vector2d( (float) i, (float) j ), _c );
			}
		}

		//Generate a texture, if we're not already one.
		if( ! glIsTexture(texture) )
		{
			glGenTextures(1,&texture);
		}

		//write the texture
		unlock();

		//set the parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
}

////////////////////////////////////////////////////////////////////////////////
// write and read pixel functions
////////////////////////////////////////////////////////////////////////////////

void Texture::setPixel( const Vector2d& _p, const Color& _c)
{
    const unsigned int x = (const unsigned int) _p.getX();
    const unsigned int y = (const unsigned int) _p.getY();
    if (data!=NULL)
    {

        *(data+(((y*width)+x)*4)) = _c.getRed();
        *(data+(((y*width)+x)*4)+1) = _c.getGreen();
        *(data+(((y*width)+x)*4)+2) = _c.getBlue();
        *(data+(((y*width)+x)*4)+3) = _c.getAlpha();

    }

}

const Color Texture::getPixel( const Vector2d& _p ) const
{
    const unsigned int x = ( const unsigned int) _p.getX();
    const unsigned int y = ( const unsigned int) _p.getY();
    if (data!=NULL)
    {

        Color col;
        col.setRed(*(data+(((y*width)+x)*4)));
        col.setGreen(*(data+(((y*width)+x)*4)+1));
        col.setBlue(*(data+(((y*width)+x)*4)+2));
        col.setAlpha(*(data+(((y*width)+x)*4)+3));
        return col;

    }
    else
    {
        return Color(255,255,255,255);
	}
}

////////////////////////////////////////////////////////////////////////////////
// lock and unlock texture
////////////////////////////////////////////////////////////////////////////////

void Texture::unlock()
{
    if (data!=NULL)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        delete [] data;

        data = NULL;
    }
}

void Texture::unlock(bool BGRA)
{
    if (data!=NULL)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
		if(!BGRA)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);

        delete [] data;

        data = NULL;
    }
}

bool Texture::lock()
{
    data = new GLubyte[width*height*4];
    if (data!=NULL)
    {
    	glBindTexture(GL_TEXTURE_2D, texture);
        glGetTexImage( GL_TEXTURE_2D , 0 , GL_RGBA , GL_UNSIGNED_BYTE, data );
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Copy texture
////////////////////////////////////////////////////////////////////////////////

boost::intrusive_ptr<Texture> Texture::copy()
{
     // Generate the destination texture
     GLuint nTexID_out = 0;
     glGenTextures( 1, &nTexID_out );
     glBindTexture( GL_TEXTURE_2D, nTexID_out );
     glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
     glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );

     // bind the source texture and get the texels
     lock();

     // bind the output texture and copy the image
     glBindTexture( GL_TEXTURE_2D, nTexID_out );
     glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

     unlock();

     TexturePtr newtexture = new Texture( getResourceManager() );
     newtexture->setTextureId( nTexID_out );
     newtexture->setWidth( width );
     newtexture->setHeight( height );
     newtexture->setName( getName() + " copy" );

     return newtexture;
}
