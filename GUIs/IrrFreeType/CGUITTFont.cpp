#include "irrlicht.h"
#include "CGUITTFont.h"
namespace irr
{
namespace gui
{

// --------------------------------------------------------
CGUITTGlyph::CGUITTGlyph()
: IReferenceCounted()
    ,size(0)
	,top(0)
	,left(0)
	,texw(0)
	,texh(0)
	,imgw(0)
	,imgh(0)
	,tex(NULL)
	,top16(0)
	,left16(0)
	,texw16(0)
	,texh16(0)
	,imgw16(0)
	,imgh16(0)
	,tex16(NULL)
	,offset(0)
	,image(NULL)
{
}

CGUITTGlyph::~CGUITTGlyph()
{
	delete[] image;
}

void CGUITTGlyph::cache(u32 idx_, FT_Face face_, video::IVideoDriver* driver_)
{
	FT_Set_Pixel_Sizes(face_, 0, size);
	if ( !FT_Load_Glyph(face_, idx_, FT_LOAD_NO_HINTING|FT_LOAD_NO_BITMAP) )
	{
		FT_GlyphSlot glyph = face_->glyph;
		FT_Bitmap  bits;

		if (glyph->format == ft_glyph_format_outline )
		{
			if (!FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL))
			{
				bits = glyph->bitmap;
				u8 *pt = bits.buffer;
				delete[] image;
				image = new u8[bits.width * bits.rows];
				memcpy(image,pt,bits.width * bits.rows);
				top = glyph->bitmap_top;
				left = glyph->bitmap_left;
				imgw = 1;
				imgh = 1;
				texw = bits.width;
				texh = bits.rows;
				for(;;)
				{
					if (imgw > texw)
					{
						break;
					}
					else
					{
						imgw <<= 1;
					}
				}
				for(;;)
				{
					if (imgh > texh)
					{
						break;
					}
					else
					{
						imgh <<= 1;
					}
				}
				if (imgw > imgh)
				{
					imgh = imgw;
				}
				else
				{
					imgw = imgh;
				}
				u32 *texd = new u32[imgw*imgh];
				memset(texd,0,imgw*imgh*sizeof(u32));
				u32 *texp = texd;
				offset = size - bits.rows;
				bool cflag = (driver_->getDriverType() == video::EDT_OPENGL);
				for (int i = 0;i < bits.rows;i++)
				{
					u32 *rowp = texp;
					for (int j = 0;j < bits.width;j++)
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
								*rowp |= 0xffffff;
							}
						}
						else
						{
                            *rowp = 0;
						}
						pt++;
						rowp++;
					}
					texp += imgw;
				}
				c8 name[128];
//				sprintf_s(name,"TTFontGlyph%d",idx_);
				video::IImage *img = driver_->createImageFromData(video::ECF_A8R8G8B8,core::dimension2d<u32>(imgw,imgh),texd);
				bool flg16 = driver_->getTextureCreationFlag(video::ETCF_ALWAYS_16_BIT);
				bool flg32 = driver_->getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT);
				driver_->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,false);
				driver_->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,true);
				tex = driver_->addTexture(name,img);
				img->drop();
				driver_->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT,flg32);
				driver_->setTextureCreationFlag(video::ETCF_ALWAYS_16_BIT,flg16);
				delete[] texd;
				cached = true;
			}
		}
	}

	if (!FT_Load_Glyph(face_,idx_,FT_LOAD_NO_HINTING|FT_LOAD_RENDER|FT_LOAD_MONOCHROME))
	{
		FT_GlyphSlot glyph = face_->glyph;
		FT_Bitmap bits = glyph->bitmap;
		u8 *pt = bits.buffer;
		top16 = glyph->bitmap_top;
		left16 = glyph->bitmap_left;
		imgw16 = 1;
		imgh16 = 1;
		texw16 = bits.width;
		texh16 = bits.rows;
		for(;;)
		{
			if (imgw16 >= texw16)
			{
				break;
			}
			else
			{
				imgw16 <<= 1;
			}
		}
		for(;;)
		{
			if (imgh16 >= texh16)
			{
				break;
			}
			else
			{
				imgh16 <<= 1;
			}
		}
		if (imgw16 > imgh16)
		{
			imgh16 = imgw16;
		}
		else
		{
			imgw16 = imgh16;
		}
		u16 *texd16 = new u16[imgw16*imgh16];
		memset(texd16,0,imgw16*imgh16*sizeof(u16));
		u16 *texp16 = texd16;
		offset = size - bits.rows;
		for (int y = 0;y < bits.rows;y++)
		{
			u16 *rowp = texp16;
			for (int x = 0;x < bits.width;x++)
			{
				if (pt[y * bits.pitch + (x / 8)] & (0x80 >> (x % 8)))
				{
					*rowp = 0xffff;
				}
				rowp++;
			}
			texp16 += imgw16;
		}
		c8 name[128];
//		sprintf_s(name,"TTFontGlyph%d_16",idx_);
		video::IImage *img = driver_->createImageFromData(video::ECF_A1R5G5B5,core::dimension2d<u32>(imgw16,imgh16),texd16);
		tex16 = driver_->addTexture(name,img);
		img->drop();
		driver_->makeColorKeyTexture(tex16,video::SColor(0,0,0,0));
		delete[] texd16;
	}
}

// --------------------------------------------------------
FT_Library	CGUITTFace::library  = 0;

//! loads a font file
bool CGUITTFace::load(const c8* filename)
{
    if ( !library )
    {
        if (FT_Init_FreeType( &library ))
        {
            return	false;
        }
    }
	if (FT_New_Face( library,filename,0,&face ))
	{
		return	false;
	}
	return	true;
}

// --------------------------------------------------------
//! constructor
CGUITTFont::CGUITTFont(video::IVideoDriver* driver)
: Driver(driver)
, tt_face(0)
{
	#ifdef _DEBUG
	setDebugName("CGUITTFont");
	#endif

	if (Driver)
		Driver->grab();
	AntiAlias = false;
	TransParency = false;
}



//! destructor
CGUITTFont::~CGUITTFont()
{
    if ( tt_face )
        tt_face->drop();
	if (Driver)
		Driver->drop();
    clearGlyphs();
}

bool CGUITTFont::attach(CGUITTFace *Face,u32 size)
{
	if (!Driver || !Face)
		return false;

    if ( tt_face )
        tt_face->drop();
	tt_face = Face;
	if ( !tt_face )
        return false;
	tt_face->grab();

    clearGlyphs();
	Glyphs.reallocate(tt_face->face->num_glyphs);
	Glyphs.set_used(tt_face->face->num_glyphs);
	for (int i = 0;i < tt_face->face->num_glyphs;i++)
	{
        CGUITTGlyph * glyph = new CGUITTGlyph();

		glyph->size = size;
		glyph->cached = false;
//		glyph->cache((wchar_t)i + 1);

        Glyphs[i] = glyph;
	}
	return	true;
}

void CGUITTFont::clearGlyphs()
{
    for ( unsigned int i=0; i < Glyphs.size(); ++i )
    {
        delete Glyphs[i];
        Glyphs[i] = 0;
    }
}

u32 CGUITTFont::getGlyphByChar(wchar_t c) const
{
	u32 idx = FT_Get_Char_Index( tt_face->face, c );
	if ( idx && !Glyphs[idx - 1]->cached )
        Glyphs[idx - 1]->cache(idx, tt_face->face, Driver);
	return	idx;
}

//! returns the dimension of a text
core::dimension2d<u32> CGUITTFont::getDimension(const wchar_t* text) const
{
	core::dimension2d<u32> dim(0, Glyphs[0]->size);

	for(const wchar_t* p = text; *p; ++p){
		dim.Width += getWidthFromCharacter(*p);
	}

	return dim;
}


inline s32 CGUITTFont::getWidthFromCharacter(wchar_t c) const
{
	u32 n = getGlyphByChar(c);
	if ( n > 0)
	{
		int w = Glyphs[n - 1]->texw;
		s32 left = Glyphs[n - 1]->left;
		if (w + left > 0)
            return w + left;
	}
	if (c >= 0x2000)
	{
		return	Glyphs[0]->size;
	}
	else
	{
		return	Glyphs[0]->size / 2;
	}
}


//! draws an text and clips it to the specified rectangle if wanted
#ifdef SDFSDFSD
//void CGUITTFont::draw2(const wchar_t* text, const core::rect<s32>& position, video::SColor color, bool hcenter, bool vcenter, const core::rect<s32>* clip)
#else
void CGUITTFont::draw2(const core::stringw& text2, const core::rect<s32>& position, video::SColor color, bool hcenter, bool vcenter, const core::rect<s32>* clip)
{

const wchar_t* text= text2.c_str();

	if (!Driver)
		return;

	core::dimension2d<u32> textDimension;
	core::position2d<s32> offset = position.UpperLeftCorner;
	video::SColor colors[4];
	for (int i = 0;i < 4;i++)
	{
		colors[i] = color;
	}

    if (hcenter || vcenter)
	{
		textDimension = getDimension(text);

		if (hcenter)
			offset.X = ((position.getWidth() - textDimension.Width)>>1) + offset.X;

		if (vcenter)
			offset.Y = ((position.getHeight() - textDimension.Height)>>1) + offset.Y;
	}

	u32 n;

	while(*text)
	{
		n = getGlyphByChar(*text);
		if ( n > 0)
		{
			if (AntiAlias)
			{
				s32 imgw = Glyphs[n-1]->imgw;
				s32 imgh = Glyphs[n-1]->imgh;
				s32 texw = Glyphs[n-1]->texw;
				s32 texh = Glyphs[n-1]->texh;
				s32 offx = Glyphs[n-1]->left;
				s32 offy = Glyphs[n-1]->size - Glyphs[n-1]->top;
				if (Driver->getDriverType() != video::EDT_SOFTWARE)
				{
					if (!TransParency)	color.color |= 0xff000000;
					Driver->draw2DImage(Glyphs[n-1]->tex,core::position2d<s32>(offset.X+offx,offset.Y+offy),core::rect<s32>(0,0,imgw-1,imgh-1),clip,color,true);
				}
				else
				{
					s32 a = color.getAlpha();
					s32 r = color.getRed();
					s32 g = color.getGreen();
					s32 b = color.getBlue();
					u8 *pt = Glyphs[n-1]->image;
					if (!TransParency)	a = 255;
					for (int y = 0;y < texh;y++)
					{
						for (int x = 0;x < texw;x++)
						{
							if (!clip || clip->isPointInside(core::position2d<s32>(offset.X+x+offx,offset.Y+y+offy)))
							{
								if (*pt)
								{
									Driver->draw2DRectangle(video::SColor((a * *pt)/255,r,g,b),core::rect<s32>(offset.X+x+offx,offset.Y+y+offy,offset.X+x+offx+1,offset.Y+y+offy+1));
								}
								pt++;
							}
						}
					}
				}
			}
			else
			{
				s32 imgw = Glyphs[n-1]->imgw16;
				s32 imgh = Glyphs[n-1]->imgh16;
//				s32 texw = Glyphs[n-1]->texw16;
//				s32 texh = Glyphs[n-1]->texh16;
				s32 offx = Glyphs[n-1]->left16;
				s32 offy = Glyphs[n-1]->size - Glyphs[n-1]->top16;
				if (!TransParency)
				{
					color.color |= 0xff000000;
				}
				Driver->draw2DImage(Glyphs[n-1]->tex16,
                                    core::position2d<s32>(offset.X+offx,offset.Y+offy),
                                    core::rect<s32>(0,0,imgw-1,imgh-1),
                                    clip, color, true);
			}
			offset.X += getWidthFromCharacter(*text);
		}
		else
		{
			offset.X += getWidthFromCharacter(*text);
		}

		++text;
	}
}
#endif

//! Calculates the index of the character in the text which is on a specific position.
s32 CGUITTFont::getCharacterFromPos(const wchar_t* text, s32 pixel_x) const
{
	s32 x = 0;
	s32 idx = 0;

	while (text[idx]){
		x += getWidthFromCharacter(text[idx]);
		if (x >= pixel_x)
			return idx;
		++idx;
	}

	return -1;
}

void CGUITTFont::setKerningWidth(s32 kerning){
}

void CGUITTFont::setKerningHeight(s32 kerning){
}

s32 CGUITTFont::getKerningWidth(const wchar_t* thisLetter, const wchar_t* previousLetter) const{
	return 0;
}

s32 CGUITTFont::getKerningHeight() const{
	return 0;
}

} // end namespace gui
} // end namespace irr
