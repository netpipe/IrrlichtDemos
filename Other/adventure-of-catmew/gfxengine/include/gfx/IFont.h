// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _IFontGfxHeader_
#define _IFontGfxHeader_

#include "gfx/Reference.h"
#include "gfx/Vector.h"
#include <string>

namespace lew 
{
	namespace gfx
	{
		class IFont : public Reference
		{
			public:			
				IFont() : Reference() {} 
				
				virtual std::string getFileName() const = 0;
				virtual Vector getCharDimension(const std::string pChar) = 0;
				virtual int getSize() const = 0;
				virtual bool getBold() const = 0;
				virtual bool getItalic() const = 0;
		};
	};
};

#endif

