#ifndef __C_GUI_TTFONT_H_INCLUDED__
#define __C_GUI_TTFONT_H_INCLUDED__

#include <ft2build.h>
#include <freetype/freetype.h>
#include <irrlicht.h>

namespace irr
{
namespace gui
{

class CGUITTFace : public IReferenceCounted
{
public:
	static FT_Library	library;    // handle to library
	FT_Face		face;               // handle to face
	bool load(const c8* filename);
};

class CGUITTGlyph : public IReferenceCounted
{
public:
	CGUITTGlyph();
	virtual ~CGUITTGlyph();

    bool cached;
	void cache(u32 idx_, FT_Face face_, video::IVideoDriver* driver_);

	u32 size;
	u32 top;
	u32 left;
	u32 texw;
	u32 texh;
	u32 imgw;
	u32 imgh;
	video::ITexture *tex;
	u32 top16;
	u32 left16;
	u32 texw16;
	u32 texh16;
	u32 imgw16;
	u32 imgh16;
	video::ITexture *tex16;
	s32 offset;
	u8 *image;
};

class CGUITTFont : public IGUIFont
{
public:

	//! constructor
	CGUITTFont(video::IVideoDriver* Driver);

	//! destructor
	virtual ~CGUITTFont();

	//! loads a truetype font file
	bool attach(CGUITTFace *Face,u32 size);

	//! draws an text and clips it to the specified rectangle if wanted
	virtual void draw(const wchar_t* text, const core::rect<s32>& position, video::SColor color, bool hcenter=false, bool vcenter=false, const core::rect<s32>* clip=0);

	//! returns the dimension of a text
	virtual core::dimension2d<s32> getDimension(const wchar_t* text) const;

	//! Calculates the index of the character in the text which is on a specific position.
	virtual s32 getCharacterFromPos(const wchar_t* text, s32 pixel_x) const;

	virtual void setKerningWidth (s32 kerning);
	virtual void setKerningHeight (s32 kerning);

	virtual s32 getKerningWidth(const wchar_t* thisLetter=0, const wchar_t* previousLetter=0) const;
	virtual s32 getKerningHeight() const;

	bool AntiAlias;
	bool TransParency;

protected:
    void clearGlyphs();

private:
	s32 getWidthFromCharacter(wchar_t c) const;
	u32 getGlyphByChar(wchar_t c) const;
	video::IVideoDriver* Driver;
	core::array< CGUITTGlyph* > Glyphs;
	CGUITTFace *tt_face;
};

} // end namespace gui
} // end namespace irr

#endif

