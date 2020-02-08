// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "compile_config.h"

#include <irrlicht.h>
#include <string>
#include <map>

namespace irr
{
namespace gui
{
    class IGUIFont;
};
};
class CGUITTFace;

class FontManager
{
public:
    FontManager();
    ~FontManager();
    irr::gui::IGUIFont* GetTtFont(irr::video::IVideoDriver* driver, irr::io::IFileSystem * fs, const char* filename_, unsigned int size_, bool antiAlias_=true, bool transparency_=true);

protected:
    std::string MakeFontIdentifier(const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_) const;

private:
    typedef std::map<std::string, irr::gui::IGUIFont*> FontMap;
    FontMap mFontMap;
};

#endif // FONT_MANAGER_H
