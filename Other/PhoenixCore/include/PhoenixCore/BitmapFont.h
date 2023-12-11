/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHBITMAPFONT_H__
#define __PHBITMAPFONT_H__

#include <vector>
#include <boost/unordered_map.hpp>
#include "config.h"
#include "Font.h"
#include "BatchRenderer.h"
#include "BatchGeometry.h"
#include "BatchGeometryComposite.h"

namespace phoenix
{

//! Bitmap Font
/*!
    This class provides a simple interface to draw fonts based on textures containing a glyph for all 256
    characters. A built-in texture is supplied for the default font used by RenderSystem.
*/
class BitmapFont
    : public Font
{
public:

	//! Structure to hold information about each character
	struct Character{
		int x;
		int y;
		int width;
		int height;
		int xoffset;
		int yoffset;
		int xadvance;
		unsigned int page;
	};

	//! Constructor
	/*!
		Makes a new Bitmap font and loads everything from a FNT file.
	*/
	BitmapFont( RenderSystem& _r, std::string _fnt = "");

    //! Advanced Constructor
    /*!
        Makes a new (empty) Bitmap Font.
		You must manually load in pages, etc.
        \param _rm A resource manager to keep track of this object.
        \param _br A batch renderer for this font to draw to.
        \note Sets the resource type to ERT_BITMAP_FONT
    */
    BitmapFont( ResourceManager& _r, BatchRenderer& _b )
        : Font( _r, _b, 3), pages(), characters(256), kernings(), line_height(16.0f), spacing( 10.0f )
    {
        setName("Unnamed font");
		std::fill( characters.begin(), characters.end(), BitmapFont::Character() );
    }

    virtual ~BitmapFont()
    {}

	//! Loads a BMFont text format .fnt file.
	/*
		Requires a rendersystem to load textures
	*/
	virtual void load( RenderSystem& _r, std::string _fnt = "" );

	//! Gets the rendered dimensions of a piece of text.
	virtual const Vector2d getTextDimensions( const std::string& _s, const Vector2d& _scale = Vector2d(0,0) );

	//! Draws the given string at the given location.
	virtual BatchGeometryPtr drawText( const std::string& s, const Vector2d& p = Vector2d(0,0), const Color& _c = Color(255,255,255), const Vector2d& _scale = Vector2d(0,0), E_TEXT_ALIGNMENT _align = T_ALIGN_LEFT );

    //! Gets the spacing between words.
    inline float getSpacing() const { return spacing; }

    //! Set spacing, default is 10.0f.
	inline void setSpacing( float s = 10.0f) { spacing = s; }

	//! Sets the character properties (Loader Interface).
	inline void setCharacterProperties( int _ix, const BitmapFont::Character& _c ){ characters[_ix] = _c; }

	//! Sets a kerning pair (Loader Interface).
	inline void setKerning( int _f, int _s, int _v ) { kernings[ KerningKey(_f,_s) ] = _v; }

	//! Fetches the kerning value for a pair
	inline int getKerning( int _f, int _s ) { return kernings[ KerningKey(_f,_s) ]; }

	//! Gets the line height.
    inline float getLineHeight() const { return line_height; }

    //! Set the line height. (loader interface)
	inline void setLineHeight( float l ) { line_height = l; }

	//! Sets a texture for a specific page
	inline void setPage( unsigned int _ix, TexturePtr _t ){
		if( pages.size() < _ix+1 ) pages.resize( _ix + 1 );
		pages[_ix] = _t;
		if( _ix == 0 ) setTexture(_t);
	}

	//! Get a texture for a specific page
	inline TexturePtr getPage( unsigned int _ix ){
		if( _ix >= pages.size() ) return TexturePtr();
		return pages[_ix];
	}

protected:

	//! Textures for pages
	std::vector< TexturePtr > pages; 

	//! Character List
	std::vector< BitmapFont::Character > characters; 

	//! Kerning pairs
	typedef std::pair<int,int> KerningKey;
	typedef boost::unordered_map< KerningKey, int > KerningMap;
	KerningMap kernings;

	//! Line height
	float line_height;

    //! Spacing between characters when drawn.
    float spacing;

	//! Temporary storage of line widths used when drawing text (speeds things up).
	std::vector<float> line_widths;

};

//! Friendly BitmapFont pointer
typedef boost::intrusive_ptr<BitmapFont> BitmapFontPtr;

} //namespace phoenix
#endif // __PHBITMAPFONT_H__
