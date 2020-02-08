// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "font_manager.h"
#include "CGUITTFont.h"
#include <sstream>

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
    FontMap::iterator itFont = mFontMap.begin();
    for ( ; itFont != mFontMap.end(); ++itFont )
    {
        itFont->second->drop();
    }
}

irr::gui::IGUIFont* FontManager::GetTtFont(irr::video::IVideoDriver* driver, irr::io::IFileSystem * fs, const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_)
{
    if ( !filename_ || !strlen(filename_) )
        return NULL;

    // Make a unique font name for the given settings.
    // We want to re-use the same font instead of create a new one for each setting
    std::string fontString(MakeFontIdentifier(filename_, size_, antiAlias_, transparency_));
    FontMap::const_iterator itFont = mFontMap.find(fontString);
    if  (itFont != mFontMap.end() )
        return itFont->second;

#ifdef HC1_HAS_FREETYPE
	irr::gui::CGUITTFont * font = irr::gui::CGUITTFont::createTTFont(driver, fs, irr::io::path(filename_), size_, antiAlias_, transparency_);
	if ( font )
	{
		font->setFontHinting(false, false);
		mFontMap[fontString] = font;
	}
	return font;
#else
	return NULL;
#endif
}

// make a unique font name for different settings.
std::string FontManager::MakeFontIdentifier(const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_) const
{
    std::ostringstream stream;
    stream << filename_ << size_;
    if ( antiAlias_ )
        stream << 'a';
    if ( transparency_ )
        stream << 't';

    //fprintf(stderr, "font: %s", stream.str().c_str());

    return stream.str();
}

