// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CTextGfxHeader_
#define _CTextGfxHeader_

#include "gfx/IText.h"

namespace lew
{
	namespace gfx
	{
		class IRoot;
		class ISceneManager;
		class CText : public IText
		{
			public:			
				CText(IRoot* m_pRoot, const bool pRegister = true);
				
				void setText(const std::string m_pText);
				void setHorizontalSpacing(const float pValue);
				void setFont(IFont* m_pFont);
				void setCharsVisible(const int pCount);
				void setCentered(const bool pFlag);
					
				std::string getText() const;
				IFont* getFont() const;
				float getHorizontalSpacing() const;
				int getCharsVisible() const;
				bool getCentered() const;
				
				void draw();
				void calculateRelativeBoundingBox();
			
			
			private:
				void calculateAverageHeight();
				Vector getCharOffset(const std::string pChar);
				Vector getCharPosition(const std::string pChar);
				
				std::string pText;
				IFont* pFont;
				
				float HorizontalSpacing;
				float AverageHeight;
				
				unsigned int CharsVisible;
				bool Centered;
		};
	};
};

#endif
