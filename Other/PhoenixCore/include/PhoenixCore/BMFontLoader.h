/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHBITMAPFONTLDR_H__
#define __PHBITMAPFONTLDR_H__

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include "config.h"
#include "BitmapFont.h"

namespace phoenix
{

//! BMFont Text format Loader
/*!
    This class provides means to load a font created by AngelCode's BMFont tool.
*/
class BMFontLoader
{

public:
	BMFontLoader( RenderSystem& _r, BitmapFontPtr _font )
		: system(_r), font(_font)
	{};

	void load( const std::string& fnt_file );
	void loadFromString( const std::string& _data, const std::string& _directory = "" );

private:
	RenderSystem& system;
	BitmapFontPtr font;

};

} //namespace phoenix
#endif // __PHBITMAPFONTLDR_H__