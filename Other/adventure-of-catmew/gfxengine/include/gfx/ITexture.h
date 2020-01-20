// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _ITextureGfxHeader_
#define _ITextureGfxHeader_

#include <string>

#include "gfx/Reference.h"
#include "gfx/Vector.h"
#include "gfx/Color.h"

namespace lew
{
	namespace gfx
	{
		enum TextureMode
		{
			ETF_NEAR,
			ETF_LINEAR
		};

		class ITexture : public Reference
		{
			public:			
				ITexture() : Reference() {} 

				virtual void setPixel(Vector pPixel, const Color pColor) = 0;
				virtual Color getPixel(Vector pPixel) = 0;

				virtual void mask(Vector pPixel) = 0;
				virtual void mask(Color pPixel) = 0;

				virtual Vector getDimension() const = 0;
				virtual Vector getOriginalDimension() const = 0;
				virtual std::string getFileName() const = 0;
				virtual bool getManipulated() const = 0;
				virtual bool getEditable() const = 0;

				virtual void update() = 0;
				virtual void save(const std::string pFileName) const = 0;
		};
	};
};

#endif

