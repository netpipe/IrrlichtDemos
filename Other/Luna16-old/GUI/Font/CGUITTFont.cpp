#include <irrlicht.h>
using namespace irr;
#include "CGUITTFont.h"

namespace irr
{
namespace gui
{

FT_Library library;
bool CGUITTFace::libraryLoaded = false;
s32 facesCount = 0;

//////////////////////

CGUITTFace::CGUITTFace()
: faceLoaded(false)
{
}

CGUITTFace::~CGUITTFace()
{
	if (faceLoaded)
	{
		FT_Done_Face(face);
		if (--facesCount == 0)
			FT_Done_FreeType(library);
	}
}

bool CGUITTFace::load(const io::path& filename)
{
	if (!libraryLoaded)
	{
		if (FT_Init_FreeType(&library))
			return false;
		CGUITTFace::libraryLoaded = true;
	}

	if (FT_New_Face(library, core::stringc(filename).c_str(), 0, &face))
		return false;

	++facesCount;
	faceLoaded = true;
	return true;
}

//////////////////////

CGUITTGlyph::CGUITTGlyph()
: IReferenceCounted(), cached(false), face(0), image(0), texture(0), texture_mono(0),
size(0), size_is_pixels(false), hasDefault(false), hasMonochrome(false)
{
}

CGUITTGlyph::~CGUITTGlyph()
{
	if (image) delete[] image;
	if (texture) Driver->removeTexture(texture);
	if (texture_mono) Driver->removeTexture(texture_mono);
}

void CGUITTGlyph::cache(u32 idx, bool fontHinting, bool autoHinting)
{
	// Set the size of the glyph.
	if (size_is_pixels)
		FT_Set_Pixel_Sizes(*face, 0, size);
	else
		FT_Set_Char_Size(*face, size * 64, size * 64, 0, 0);

	FT_Int32 loadFlags = FT_LOAD_DEFAULT;
	if (!fontHinting) loadFlags |= FT_LOAD_NO_HINTING;
	if (!autoHinting) loadFlags |= FT_LOAD_NO_AUTOHINT;
	if (!FT_Load_Glyph(*face, idx, loadFlags))
	{
		hasDefault = true;
		FT_GlyphSlot glyph = (*face)->glyph;
		FT_Bitmap bits;
		FT_Error err = 0;
		if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
			err = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

		if (!err)
		{
			// Generate the image.
			bits = glyph->bitmap;
			u8 *pt = bits.buffer;
			image = new u8[bits.width * bits.rows];
			memcpy(image, pt, bits.width * bits.rows);

			// Bitmap information.
			bitmap.top = glyph->bitmap_top;
			bitmap.left = glyph->bitmap_left;
			bitmap.width = bits.width;
			bitmap.height = bits.rows;

			// Initialize the texture information.
			texture_size.set(1, 1);

			// Determine what our texture size should be.
			// Keep multiplying by 2 until we get it.
			while (texture_size.Width <= bitmap.width)
				texture_size.Width <<= 1;
			while (texture_size.Height <= bitmap.height)
				texture_size.Height <<= 1;

			// Square our texture.
			if (texture_size.Height < texture_size.Width)
				texture_size.Height = texture_size.Width;
			else texture_size.Width = texture_size.Height;

			// Create our texture data.
			u32 *texture_data = new u32[texture_size.Width * texture_size.Height];
			memset(texture_data, 0, texture_size.Width * texture_size.Height * sizeof(u32));
			u32 *texp = texture_data;
			bool cflag = (Driver->getDriverType() == video::EDT_DIRECT3D8);
			for (int i = 0; i < bits.rows; i++)
			{
				u32 *rowp = texp;
				for (int j = 0; j < bits.width; j++)
				{
					if (*pt)
					{
						if (cflag)
						{
							*rowp = *pt;
							*rowp *= 0x01010101;
						}
						else
						{
							*rowp = *pt << 24;
							*rowp |= 0x00ffffff;
						}
					}
					else *rowp = 0;
					pt++;
					rowp++;
				}
				texp += texture_size.Width;
			}

			// Name our texture.
			c8 name[128];
			sprintf(name, "TTFontGlyph%d", idx);

			// Create our texture.
			video::IImage *img = Driver->createImageFromData(video::ECF_A8R8G8B8, core::dimension2d<u32>(texture_size.Width, texture_size.Height), texture_data);
			bool flg16 = Driver->getTextureCreationFlag(video::ETCF_ALWAYS_16_BIT);
			bool flg32 = Driver->getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
			bool flgmip = Driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
			Driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT, false);
			Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
			Driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
			if (texture) Driver->removeTexture(texture);
			texture = Driver->addTexture(name, img);
			img->drop();
			Driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, flgmip);
			Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, flg32);
			Driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT, flg16);
			delete[] texture_data;

			// Set our glyph as "cached".
			cached = true;
		}
	}

	loadFlags = FT_LOAD_RENDER | FT_LOAD_MONOCHROME;
	if (!fontHinting) loadFlags |= FT_LOAD_NO_HINTING;
	if (!autoHinting) loadFlags |= FT_LOAD_NO_AUTOHINT;
	if (fontHinting || autoHinting) loadFlags |= FT_LOAD_TARGET_MONO;
	if (!FT_Load_Glyph(*face, idx, loadFlags))
	{
		hasMonochrome = true;
		FT_GlyphSlot glyph = (*face)->glyph;
		FT_Bitmap bits = glyph->bitmap;
		u8 *pt = bits.buffer;

		// Bitmap information.
		bitmap_mono.top = glyph->bitmap_top;
		bitmap_mono.left = glyph->bitmap_left;
		bitmap_mono.width = bits.width;
		bitmap_mono.height = bits.rows;

		// Initialize the texture information.
		texture_mono_size.set(1, 1);

		// Determine what our texture size should be.
		// Keep multiplying by 2 until we get it.
		while (texture_mono_size.Width <= bitmap_mono.width)
			texture_mono_size.Width <<= 1;
		while (texture_mono_size.Height <= bitmap_mono.height)
			texture_mono_size.Height <<= 1;

		// Square our texture.
		if (texture_mono_size.Height < texture_mono_size.Width)
			texture_mono_size.Height = texture_mono_size.Width;
		else texture_mono_size.Width = texture_mono_size.Height;

		// Create our texture data.
		u16 *texture_mono_data = new u16[texture_mono_size.Width * texture_mono_size.Height];
		memset(texture_mono_data, 0, texture_mono_size.Width * texture_mono_size.Height * sizeof(u16));
		u16 *texpm = texture_mono_data;
		for (int y = 0; y < bits.rows; y++)
		{
			u16 *rowp = texpm;
			for (int x = 0; x < bits.width; x++)
			{
				if (pt[y * bits.pitch + (x / 8)] & (0x80 >> (x % 8)))
					*rowp = 0xffff;
				rowp++;
			}
			texpm += texture_mono_size.Width;
		}

		// Name our texture.
		c8 name[128];
		sprintf(name, "TTFontGlyph%d_mono", idx);

		// Create our texture.
		video::IImage *img = Driver->createImageFromData(video::ECF_A1R5G5B5, core::dimension2d<u32>(texture_mono_size.Width, texture_mono_size.Height), texture_mono_data);
		bool flg16 = Driver->getTextureCreationFlag(video::ETCF_ALWAYS_16_BIT);
		bool flg32 = Driver->getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
		bool flgmip = Driver->getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
		Driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,true);
		Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,false);
		Driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,false);
		if (texture_mono) Driver->removeTexture(texture_mono);
		texture_mono = Driver->addTexture(name, img);
		img->drop();
		Driver->makeColorKeyTexture(texture_mono, video::SColor(0,0,0,0));
		Driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS,flgmip);
		Driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,flg32);
		Driver->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,flg16);
		delete[] texture_mono_data;
	}
}

//////////////////////

//! constructor
CGUITTFont::CGUITTFont(IGUIEnvironment *env)
: AntiAlias(true), Transparency(true), FontHinting(true), AutoHinting(true),
Environment(env), Driver(0), tt_face(0), GlobalKerningWidth(0), GlobalKerningHeight(0)
{
	#ifdef _DEBUG
	setDebugName("CGUITTFont");
	#endif

	if (Environment)
	{
		// don't grab environment, to avoid circular references
		Driver = Environment->getVideoDriver();
	}

	if (Driver)
		Driver->grab();

	setInvisibleCharacters ( L" " );
}

//! destructor
CGUITTFont::~CGUITTFont()
{
	Glyphs.clear();
	if (Driver)
		Driver->drop();
}

bool CGUITTFont::attach(CGUITTFace *face, u32 size, bool size_is_pixels)
{
	if (!Driver)
		return false;

	tt_face = face;

	Glyphs.reallocate(tt_face->face->num_glyphs);
	Glyphs.set_used(tt_face->face->num_glyphs);
	for (int i = 0; i < tt_face->face->num_glyphs; i++)
	{
		Glyphs[i].Driver = Driver;
		Glyphs[i].size = size;
		Glyphs[i].size_is_pixels = size_is_pixels;
		Glyphs[i].face = &(tt_face->face);
		Glyphs[i].cached = false;
	}
	return true;
}

void CGUITTFont::draw(const core::stringw& text, const core::rect<s32>& position, video::SColor color, bool hcenter, bool vcenter, const core::rect<s32>* clip)
{
	if (!Driver)
		return;

	core::dimension2d<s32> textDimension;
	core::position2d<s32> offset = position.UpperLeftCorner;
	video::SColor colors[4];
	for (int i = 0; i < 4; i++)
		colors[i] = color;

    if (hcenter || vcenter)
	{
		textDimension = getDimension(text.c_str());

		if (hcenter)
			offset.X = ((position.getWidth() - textDimension.Width) >> 1) + offset.X;

		if (vcenter)
			offset.Y = ((position.getHeight() - textDimension.Height) >> 1) + offset.Y;
	}

	u32 n;
	
	wchar_t previousChar = 0;
	const wchar_t* ptext = text.c_str();
	while (*ptext)
	{
		wchar_t currentChar = *ptext;
		n = getGlyphByChar(currentChar);
		bool visible = (Invisible.findFirst(currentChar) == -1);
		if (n > 0 && visible)
		{
			bool lineBreak=false;
			if (currentChar == L'\r') // Mac or Windows breaks
			{
				lineBreak = true;
				if (*(ptext + 1) == L'\n') // Windows breaks
					currentChar = *(++ptext);
			}
			else if (currentChar == L'\n') // Unix breaks
			{
				lineBreak = true;
			}

			if (lineBreak)
			{
				offset.Y += Glyphs[0].size;
				offset.X = position.UpperLeftCorner.X;

				if (hcenter)
				{
					core::dimension2d<u32> lineDim = getDimension(text.c_str());
					offset.X += (position.getWidth() - lineDim.Width) >> 1;
				}
				continue;
			}

			// Only use the 256-color font if we are anti-aliasing, or if there was no monochrome font.
			if (AntiAlias || !Glyphs[n-1].hasMonochrome)
			{
				u32 texw = Glyphs[n-1].texture_size.Width;
				u32 texh = Glyphs[n-1].texture_size.Height;
				u32 bmpw = Glyphs[n-1].bitmap.width;
				u32 bmph = Glyphs[n-1].bitmap.height;
				s32 offx = Glyphs[n-1].bitmap.left;
				s32 offy = Glyphs[n-1].size - Glyphs[n-1].bitmap.top;

				// Apply kerning.
				core::vector2di k = getKerning(currentChar, previousChar);
				offx += k.X;
				offy += k.Y;

				if (Driver->getDriverType() != video::EDT_SOFTWARE)
				{
					if (!Transparency) color.color |= 0xff000000;
					Driver->draw2DImage(Glyphs[n-1].texture, core::position2d<s32>(offset.X + offx, offset.Y + offy), core::rect<s32>(0, 0, texw-1, texh-1), clip, color, true);
				}
				else
				{
					// Software driver doesn't do transparency correctly, so if we are using the driver,
					// we must manually apply the transparency.
					u32 a = color.getAlpha();
					u32 r = color.getRed();
					u32 g = color.getGreen();
					u32 b = color.getBlue();
					u8 *pt = Glyphs[n-1].image;
					if (!Transparency) a = 255;
					for (u32 y = 0; y < bmph; y++)
					{
						for (u32 x = 0; x < bmpw; x++)
						{
							if (!clip || clip->isPointInside(core::position2d<s32>(offset.X + x + offx, offset.Y + y + offy)))
							{
								if (*pt)
								{
									Driver->draw2DRectangle(video::SColor((a * (*pt)) / 255, r, g, b), core::rect<s32>(offset.X + x + offx, offset.Y + y + offy, offset.X + x + offx + 1, offset.Y + y + offy + 1));
								}
								pt++;
							}
						}
					}
				}
			}
			else if (Glyphs[n-1].hasMonochrome)
			{
				u32 texw = Glyphs[n-1].texture_mono_size.Width;
				u32 texh = Glyphs[n-1].texture_mono_size.Height;
				s32 offx = Glyphs[n-1].bitmap_mono.left;
				s32 offy = Glyphs[n-1].size - Glyphs[n-1].bitmap_mono.top;

				// Apply kerning.
				core::vector2di k = getKerning(currentChar, previousChar);
				offx += k.X;
				offy += k.Y;

				if (!Transparency) color.color |= 0xff000000;
				Driver->draw2DImage(Glyphs[n-1].texture_mono, core::position2d<s32>(offset.X + offx, offset.Y + offy), core::rect<s32>(0, 0, texw - 1, texh - 1), clip, color, true);
			}
			offset.X += getWidthFromCharacter(currentChar);
		}
		else offset.X += getWidthFromCharacter(currentChar);

		previousChar = currentChar;
		++ptext;
	}
}

core::dimension2d<u32> CGUITTFont::getDimension(const wchar_t* text) const
{
	core::dimension2d<u32> dim(0, Glyphs[0].size);
	core::dimension2d<u32> thisLine(0, Glyphs[0].size);

	for (const wchar_t* p = text; *p; ++p)
	{
		bool lineBreak=false;
		if (*p == L'\r') // Mac or Windows breaks
		{
			lineBreak = true;
			if (p[1] == L'\n') // Windows breaks
				++p;
		}
		else if (*p == L'\n') // Unix breaks
		{
			lineBreak = true;
		}
		if (lineBreak)
		{
			dim.Height += thisLine.Height;
			if (dim.Width < thisLine.Width)
				dim.Width = thisLine.Width;
			thisLine.Width = 0;
			thisLine.Height = Glyphs[0].size;
			continue;
		}
		thisLine.Width += getWidthFromCharacter(*p);
		//u32 h = getHeightFromCharacter(*p);
		//if (h > thisLine.Height)
		//	thisLine.Height = h;
	}
	if (dim.Width < thisLine.Width)
		dim.Width = thisLine.Width;

	// Add a small amount of blank space between the lines.
	dim.Height += (Glyphs[0].size / 4);

	return dim;
}

inline u32 CGUITTFont::getWidthFromCharacter(wchar_t c) const
{
	u32 n = getGlyphByChar(c);
	if (n > 0)
	{
		int w = Glyphs[n - 1].bitmap.width;
		s32 left = Glyphs[n - 1].bitmap.left;
		if (w + left > 0) return w + left;
	}
	if (c >= 0x2000)
		return Glyphs[0].size;
	else return Glyphs[0].size / 2;
}

inline u32 CGUITTFont::getHeightFromCharacter(wchar_t c) const
{
	u32 n = getGlyphByChar(c);
	if (n > 0)
	{
		s32 top = Glyphs[n - 1].bitmap.top;
		return top;
	}
	if (c >= 0x2000)
		return Glyphs[0].size;
	else return Glyphs[0].size / 2;
}

u32 CGUITTFont::getGlyphByChar(wchar_t c) const
{
	u32 idx = FT_Get_Char_Index(tt_face->face, c);

	// If the glyph hasn't been loaded yet, do it now.
	if (idx && !Glyphs[idx - 1].cached)
		Glyphs[idx - 1].cache(idx, FontHinting, AutoHinting);

	return	idx;
}

s32 CGUITTFont::getCharacterFromPos(const wchar_t* text, s32 pixel_x) const
{
	s32 x = 0;
	s32 idx = 0;

	while (text[idx])
	{
		x += getWidthFromCharacter(text[idx]);

		if (x >= pixel_x)
			return idx;

		++idx;
	}

	return -1;
}

void CGUITTFont::setKerningWidth(s32 kerning)
{
	GlobalKerningWidth = kerning;
}

void CGUITTFont::setKerningHeight(s32 kerning)
{
	GlobalKerningHeight = kerning;
}

s32 CGUITTFont::getKerningWidth(const wchar_t* thisLetter, const wchar_t* previousLetter) const
{
	if (tt_face == 0)
		return GlobalKerningWidth;
	if (thisLetter == 0 || previousLetter == 0)
		return 0;

	// Return only the kerning width.
	return getKerning(*thisLetter, *previousLetter).X;
}

s32 CGUITTFont::getKerningHeight() const
{
	// FreeType 2 currently doesn't return any height kerning information.
	return GlobalKerningHeight;
}

core::vector2di CGUITTFont::getKerning(const wchar_t thisLetter, const wchar_t previousLetter) const
{
	core::vector2di ret(GlobalKerningWidth, GlobalKerningHeight);
	if (tt_face == 0)
		return ret;

	// Grab the face's flags to determine if the font is scalable or not.
	FT_Long flags = tt_face->face->face_flags;
	bool scalable = ((flags & FT_FACE_FLAG_SCALABLE) != 0);

	// Get the kerning information.
	FT_Vector v;
	FT_Get_Kerning(tt_face->face, getGlyphByChar(previousLetter), getGlyphByChar(thisLetter), FT_KERNING_DEFAULT, &v);

	// If we have a scalable font, the return value will be in font points.
	if (scalable)
	{
		// Font points, so divide by 64.
		ret.X += (v.x / 64);
		ret.Y += (v.y / 64);
	}
	else
	{
		// Pixel units.
		ret.X += v.x;
		ret.Y += v.y;
	}
	return ret;
}

void CGUITTFont::setInvisibleCharacters( const wchar_t *s )
{
	Invisible = s;
}

} // end namespace gui
} // end namespace irr
