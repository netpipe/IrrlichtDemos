// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _ITextGfxHeader_
#define _ITextGfxHeader_

#include <string>

#include "gfx/IObject.h"

namespace lew 
{
	namespace gfx
	{
		class IFont;
		class ISceneManager;
		class IText : public IObject
		{
			public:		
				IText(IRoot* m_pRoot, const bool pRegister = true) : 
					IObject(m_pRoot, pRegister) {}
								
				virtual void setText(const std::string m_pText) = 0;
				virtual void setHorizontalSpacing(const float pValue) = 0;
				virtual void setFont(IFont* m_pFont) = 0;
				virtual void setCharsVisible(const int pCount) = 0;
				virtual void setCentered(const bool pFlag) = 0;
				
				virtual std::string getText() const = 0;
				virtual IFont* getFont() const = 0;
				virtual float getHorizontalSpacing() const = 0;
				virtual int getCharsVisible() const = 0;
				virtual bool getCentered() const = 0;
		};
	};
};

#endif

