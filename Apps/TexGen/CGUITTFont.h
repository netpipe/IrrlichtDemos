#ifndef __C_GUI_TTFONT_H_INCLUDED__
#define __C_GUI_TTFONT_H_INCLUDED__

#include <irrlicht.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "irrUString.h"

namespace irr
{
namespace gui
{
	// Assists us in deleting Glyphs.
	class CGUITTAssistDelete
	{
		public:
			template <class T, typename TAlloc>
			static void Delete(core::array<T, TAlloc>& a)
			{
				TAlloc allocator;
				allocator.deallocate(a.pointer());
			}
	};

	//! Holds a sheet of glyphs.
	class CGUITTGlyphPage
	{
		public:
			CGUITTGlyphPage(video::IVideoDriver* Driver, const io::path& texture_name) : image(0), texture(0), available_slots(0), used_slots(0), dirty(false), driver(Driver), name(texture_name) {}
			~CGUITTGlyphPage()
			{
				if (image)
					image->drop();
				if (texture)
				{
					if (driver)
						driver->removeTexture(texture);
					else texture->drop();
				}
			}

			//! Updates the texture if dirty.
			void updateTexture()
			{
				static u32 test = 0;
				dirty = false;

				// Save our texture creation flags and disable mipmaps.
				bool flg16 = driver->getTextureCreationFlag(video::ETCF_ALWAYS_16_BIT);
				bool flg32 = driver->getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
				bool flgmip = driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
				driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

				// Set the texture color format.
				switch (image->getColorFormat())
				{
					case video::ECF_A1R5G5B5:
						// We want to create a 16-bit texture.
						driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT, true);
						driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, false);
						break;

					case video::ECF_A8R8G8B8:
					default:
						// We want to create a 32-bit texture.
						driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT, false);
						driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
						break;
				}

				// Add the texture.
				if (texture)
				{
					driver->setMaterial(video::SMaterial());
					driver->removeTexture(texture);
				}
				texture = driver->addTexture(name, image);

				// Restore our texture creation flags.
				driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, flgmip);
				driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, flg32);
				driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT, flg16);
			}

			video::IImage* image;
			video::ITexture* texture;
			u32 available_slots;
			u32 used_slots;
			bool dirty;

			core::array<core::vector2di> render_positions;
			core::array<core::recti> render_source_rects;

		private:
			video::IVideoDriver* driver;
			io::path name;
	};

	//! Structure representing a single TrueType glyph.
	struct SGUITTGlyph
	{
		//! Loads the glyph.
		void load(u32 character, FT_Face face, video::IVideoDriver* driver, u32 size, core::dimension2du max_texture_size, core::array<CGUITTGlyphPage*>* Glyph_Pages, bool fontHinting, bool autoHinting, bool useMonochrome);

		//! Unloads the glyph.
		void unload();

		//! If true, the glyph has been loaded.
		bool isLoaded;

		//! The page the glyph is on.
		u32 glyph_page;

		//! The source rectangle for the glyph.
		core::recti source_rect;

		//! The offset of glyph when drawn.
		core::vector2di offset;

		// Glyph advance information.
		FT_Vector advance;
	};

	//! Class representing a font.
	class CGUITTFont : public IGUIFont
	{
		public:
			//! Creates a new TrueType font and returns a pointer to it.  The pointer must be drop()'ed when finished.
			//! \param env The IGUIEnvironment the font loads out of.
			//! \param filename The filename of the font.
			//! \param size The size of the font glyphs in pixels.  Since this is the size of the individual glyphs, the true height of the font may change depending on the characters used.
			//! \return Returns a pointer to a CGUITTFont.  Will return 0 if the font failed to load.
			static CGUITTFont* createTTFont(IGUIEnvironment *env, const io::path& filename, const u32 size);

			//! Destructor
			virtual ~CGUITTFont();

			//! Sets the amount of glyphs to batch load.
			//! When a new glyph is loaded, the class also loads extra glyphs which surround the target glyph.
			//! For example, if batch_size is set to 50 and you try to load glyph 100, the class will load glyphs 75-125.
			//! \param batch_size The amount of glyphs to batch load.  Defaults to 100.
			void setBatchLoadSize(u32 batch_size)
			{
				batch_load_size = batch_size;
			}

			//! Sets the maximum texture size for a page of glyphs.
			//! By default, it creates a texture large enough for at least 400 glyphs.
			//! \param texture_size The maximum size of the texture.
			void setMaxPageTextureSize(const core::dimension2du& texture_size)
			{
				max_page_texture_size = texture_size;
			}

			//! Tells the font to allow transparency when rendering.
			//! Default: true.
			//! \param flag If true, the font draws using transparency.
			void setTransparency(const bool flag)
			{
				use_transparency = flag;
			}

			//! Tells the font to use monochrome rendering.
			//! Default: false.
			//! \param flag If true, the font draws using a monochrome image.  If false, the font uses a grayscale image.
			void setMonochrome(const bool flag);

			//! Enables or disables font hinting.
			//! Default: Hinting and auto-hinting true.
			//! \param enable If false, font hinting is turned off.  If true, font hinting is turned on.
			//! \param enable_auto_hinting If true, FreeType uses its own auto-hinting algorithm.  If false, it tries to use the algorithm specified by the font.
			void setFontHinting(const bool enable, const bool enable_auto_hinting = true);

			//! Draws some text and clips it to the specified rectangle if wanted.
			virtual void draw(const core::stringw& text, const core::rect<s32>& position,
				video::SColor color, bool hcenter=false, bool vcenter=false,
				const core::rect<s32>* clip=0);

			//! Returns the dimension of a text string.
			virtual core::dimension2d<u32> getDimension(const wchar_t* text) const;
			virtual core::dimension2d<u32> getDimension(const core::ustring& text) const;

			//! Calculates the index of the character in the text which is on a specific position.
			virtual s32 getCharacterFromPos(const wchar_t* text, s32 pixel_x) const;
			virtual s32 getCharacterFromPos(const core::ustring& text, s32 pixel_x) const;

			//! Sets global kerning width for the font.
			virtual void setKerningWidth(s32 kerning);

			//! Sets global kerning height for the font.
			virtual void setKerningHeight(s32 kerning);

			//! Gets kerning values (distance between letters) for the font. If no parameters are provided,
			virtual s32 getKerningWidth(const wchar_t* thisLetter=0, const wchar_t* previousLetter=0) const;
			virtual s32 getKerningWidth(const uchar32_t thisLetter=0, const uchar32_t previousLetter=0) const;

			//! Returns the distance between letters
			virtual s32 getKerningHeight() const;

			//! Define which characters should not be drawn by the font.
			virtual void setInvisibleCharacters(const wchar_t *s);
			virtual void setInvisibleCharacters(const core::ustring& s);

			void forceGlyphUpdate()
			{
				for (u32 i = 0; i != Glyph_Pages.size(); ++i)
					Glyph_Pages[i]->updateTexture();
			}

		private:
			CGUITTFont(IGUIEnvironment *env);
			bool load(const io::path& filename, const u32 size);
			void reset_images();
			void update_glyph_pages() const;

			u32 getWidthFromCharacter(wchar_t c) const;
			u32 getWidthFromCharacter(uchar32_t c) const;
			u32 getHeightFromCharacter(wchar_t c) const;
			u32 getHeightFromCharacter(uchar32_t c) const;
			u32 getGlyphByChar(wchar_t c) const;
			u32 getGlyphByChar(uchar32_t c) const;
			core::vector2di getKerning(const wchar_t thisLetter, const wchar_t previousLetter) const;
			core::vector2di getKerning(const uchar32_t thisLetter, const uchar32_t previousLetter) const;

			gui::IGUIEnvironment* Environment;
			video::IVideoDriver* Driver;
			io::path filename;
			FT_Face tt_face;
			FT_Size_Metrics font_metrics;

			bool use_monochrome;
			bool use_transparency;
			bool use_hinting;
			bool use_auto_hinting;
			u32 size;
			u32 batch_load_size;
			core::dimension2du max_page_texture_size;
			mutable core::array<CGUITTGlyphPage*> Glyph_Pages;
			mutable core::array<SGUITTGlyph> Glyphs;

			s32 GlobalKerningWidth;
			s32 GlobalKerningHeight;
			core::ustring Invisible;
	};

} // end namespace gui
} // end namespace irr

#endif
