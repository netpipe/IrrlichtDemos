#ifndef __C_GUI_TTFONT_H_INCLUDED__
#define __C_GUI_TTFONT_H_INCLUDED__

#include <irrlicht.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace irr
{
namespace gui
{
	//! Represents a font face.
	class CGUITTFace : public virtual IReferenceCounted
	{
		public:
			CGUITTFace();
			virtual ~CGUITTFace();

			//! Loads a font face.
			//! \param filename Path to the font face.
			//! \param filesystem The Irrlicht filesystem to use.  If 0, fonts will be loaded into memory by FreeType instead of by Irrlicht.
			//! \return Returns true if the font face loaded, false if it failed to load.
			bool load(const io::path& filename, io::IFileSystem* filesystem = 0);

			//! The font face.
			FT_Face face;

		private:
			//! Flag to load the library.
			static bool libraryLoaded;
			bool faceLoaded;
			FT_Byte* font_buffer;
			FT_Long font_size;
	};

	//! Represents a glyph's bitmap info.
	struct CGUITTBitmapInfo
	{
		u32 top;
		u32 left;
		u32 width;
		u32 height;
	};

	//! Class representing a single glyph.
	class CGUITTGlyph
	{
		public:
			CGUITTGlyph();
			virtual ~CGUITTGlyph();

			//! Loads the glyph.
			void cache(u32 idx, bool fontHinting, bool autoHinting);

			//! Informs if the glyph was loaded.
			bool cached;

			//! Video driver.
			video::IVideoDriver* Driver;

			//! The face.
			FT_Face *face;

			//! The image data.
			//! Used for rendering with the software engine.
			u8* image;

			// Texture/image data.
			video::ITexture *texture;
			video::ITexture *texture_mono;

			// Size of the glyph.
			u32 size;

			// The size of the glyph is expressed in pixels.
			bool size_is_pixels;

			// Info.
			bool hasDefault;
			bool hasMonochrome;

			// Bitmap information.
			CGUITTBitmapInfo bitmap;
			CGUITTBitmapInfo bitmap_mono;

			// Texture information.
			core::dimension2du texture_size;
			core::dimension2du texture_mono_size;
	};

	//! Class representing a font.
	class CGUITTFont : public IGUIFont
	{
		public:
			//! Constructor
			CGUITTFont(IGUIEnvironment *env);

			//! Destructor
			virtual ~CGUITTFont();

			//! Binds a font face to the class.
			//! \param face The font face to attach.
			//! \param size The size you want to load the font at.
			//! \param size_is_pixels If true, size is represented as pixels instead of points.
			bool attach(CGUITTFace *face, u32 size, bool size_is_pixels = false);

			//! Draws some text and clips it to the specified rectangle if wanted.
			virtual void draw(const core::stringw& text, const core::rect<s32>& position,
				video::SColor color, bool hcenter=false, bool vcenter=false,
				const core::rect<s32>* clip=0);

			//! Returns the dimension of a text string.
			virtual core::dimension2d<u32> getDimension(const wchar_t* text) const;

			//! Calculates the index of the character in the text which is on a specific position.
			virtual s32 getCharacterFromPos(const wchar_t* text, s32 pixel_x) const;

			//! Sets global kerning width for the font.
			virtual void setKerningWidth(s32 kerning);

			//! Sets global kerning height for the font.
			virtual void setKerningHeight(s32 kerning);

			//! Gets kerning values (distance between letters) for the font. If no parameters are provided,
			virtual s32 getKerningWidth(const wchar_t* thisLetter=0, const wchar_t* previousLetter=0) const;

			//! Returns the distance between letters
			virtual s32 getKerningHeight() const;

			//! Define which characters should not be drawn by the font.
			virtual void setInvisibleCharacters(const wchar_t *s);

			//! Determines if the font will be loaded with antialiasing.
			//! Defaults true.
			bool AntiAlias;

			//! Determines if the font will be drawn with transparency.
			//! Defaults true.
			bool Transparency;

			//! Turns font hinting on or off.  If a font looks odd, try toggling this option.
			//! This setting controls whether or not FreeType uses a font's built-in hinting.
			//! Defaults true.
			bool FontHinting;

			//! Turns FreeType auto-hinting on or off.  If a font looks odd, try toggling this option.
			//! This setting controls whether or not FreeType uses its built-in auto hinting.
			//! Defaults to true.
			bool AutoHinting;

		private:
			u32 getWidthFromCharacter(wchar_t c) const;
			u32 getHeightFromCharacter(wchar_t c) const;
			u32 getGlyphByChar(wchar_t c) const;
			core::vector2di getKerning(const wchar_t thisLetter, const wchar_t previousLetter) const;

			gui::IGUIEnvironment* Environment;
			video::IVideoDriver* Driver;
			mutable core::array< CGUITTGlyph > Glyphs;
			CGUITTFace *tt_face;
			s32 GlobalKerningWidth;
			s32 GlobalKerningHeight;
			core::stringw Invisible;
	};

} // end namespace gui
} // end namespace irr

#endif
