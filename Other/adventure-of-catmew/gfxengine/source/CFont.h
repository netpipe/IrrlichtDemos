// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CFontGfxHeader_
#define _CFontGfxHeader_

#include <vector>
#include <map>
#include <sstream>

#include "gfx/IFont.h"
#include "gfx/ITexture.h"

namespace lew
{
	namespace gfx
	{
		class IRoot;
		class CFont : public IFont
		{
			public:			
				CFont(IRoot* m_pRoot, const std::string pFont, 
					const int pSize, const bool pBold, const bool pItalic,
					const bool pEditable);
				
				virtual ~CFont();
				
				ITexture* getChar(const std::string pChar);
				std::string getFileName() const;	
				Vector getCharDimension(const std::string pChar);
				int getSize() const;
				bool getBold() const;
				bool getItalic() const;
			
			
			private:			
				IRoot* pRoot;
				
				std::vector<ITexture*> CharList;
				std::map<std::string, ITexture*> CharMap;
				
				std::string FileName;
				int Size;
				
				bool Bold;
				bool Italic;
		};
	}
};

#endif

