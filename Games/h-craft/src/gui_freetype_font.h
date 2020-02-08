#ifndef _GUI_FREETYPE_FONT_H
#define _GUI_FREETYPE_FONT_H

#include "compile_config.h"

#ifdef HC1_HAS_FREETYPE

#include <ft2build.h>
#include <freetype.h>
#include <irrlicht.h>

class CGUIFreetypeFont;

class CGUITTFace : public irr::IReferenceCounted
{
public:
	CGUITTFace();
	virtual ~CGUITTFace();

	bool load(const irr::io::path& filename, irr::io::IFileSystem * fs);

	FT_Face		face;               // handle to face

private:
	FT_Byte* mMemoryFile;
	static int			countClassObjects;
	static FT_Library	library;    // handle to library
};

class CGUITTGlyph : public irr::IReferenceCounted
{
public:
	CGUITTGlyph();
	virtual ~CGUITTGlyph();

    bool cached;
	void cache(irr::u32 idx_, const CGUIFreetypeFont * freetypeFont);

	irr::u32 size;
	irr::u32 top;
	irr::u32 left;
	irr::u32 texw;
	irr::u32 texh;
	irr::u32 imgw;
	irr::u32 imgh;
	irr::video::ITexture *tex;
	irr::u32 top16;
	irr::u32 left16;
	irr::u32 texw16;
	irr::u32 texh16;
	irr::u32 imgw16;
	irr::u32 imgh16;
	irr::video::ITexture *tex16;
	irr::u8 *image;

private:
	void setGlyphTextureFlags(irr::video::IVideoDriver* driver_);
    void restoreTextureFlags(irr::video::IVideoDriver* driver_);

	static bool mTexFlag16;
	static bool mTexFlag32;
	static bool mTexFlagMip;
};

class CGUIFreetypeFont : public irr::gui::IGUIFont
{
	friend class CGUITTGlyph;

public:

	//! constructor
	CGUIFreetypeFont(irr::video::IVideoDriver* Driver);

	//! destructor
	virtual ~CGUIFreetypeFont();

	//! loads a truetype font file
	bool attach(CGUITTFace *Face,irr::u32 size);

	//! draws an text and clips it to the specified rectangle if wanted
	virtual void draw(const irr::core::stringw& text, const irr::core::rect<irr::s32>& position, irr::video::SColor color, bool hcenter=false, bool vcenter=false, const irr::core::rect<irr::s32>* clip=0);

	//! returns the dimension of a text
	virtual irr::core::dimension2d<irr::u32> getDimension(const wchar_t* text) const;

	//! Calculates the index of the character in the text which is on a specific position.
	virtual irr::s32 getCharacterFromPos(const wchar_t* text, irr::s32 pixel_x) const;

	//! Not yet supported
	virtual void setKerningWidth (irr::s32 kerning) {}

	//! Not yet supported
	virtual void setKerningHeight (irr::s32 kerning) {}

	//! Not yet supported
	virtual irr::s32 getKerningWidth(const wchar_t* thisLetter=0, const wchar_t* previousLetter=0)	const { return 0; }

	//! Not yet supported
	virtual irr::s32 getKerningHeight()	const { return 0; }

	//! Not yet supported
    virtual void setInvisibleCharacters( const wchar_t *s ) {}


	bool AntiAlias;
	bool Transparency;

protected:
    void clearGlyphs();

private:
	irr::u32 getWidthFromCharacter(wchar_t c) const;
	irr::u32 getGlyphByChar(wchar_t c) const;
	irr::video::IVideoDriver* Driver;
	irr::core::array< CGUITTGlyph* > Glyphs;
	CGUITTFace * TrueTypeFace;
	mutable irr::core::dimension2d<irr::u32> LargestGlyph;
};

#endif // HC1_HAS_FREETYPE

#endif	// _GUI_FREETYPE_FONT_H
