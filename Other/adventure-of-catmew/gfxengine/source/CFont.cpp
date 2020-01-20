// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "Leeuwen" Library.
// For conditions of distribution and use, see copyright notice in Leeuwen.h

#include "CFont.h"
#include "CRoot.h"
#include "CVideoDriver.h"
#include "CTexture.h"

namespace lew
{
	namespace gfx
	{
		CFont::CFont(IRoot* m_pRoot, const std::string pFont, 
		const int pSize, const bool pBold, const bool pItalic,
		const bool pEditable) : IFont()
		{
			pRoot = m_pRoot;
			
			Size 	= pSize;
			FileName= pFont;
			Bold	= pBold;
			Italic	= pItalic;
			
			std::string CharString = "";
			CharString += "abcdefghijklmnopqrstuvwxyz";
			CharString += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
			CharString += "0123456789";
			CharString += "!\"§$%&/()=?`´,.-;:_+*#~€@<>|{[]}²³ ";
			CharString += "äöüÄÖÜ";
			
			for(unsigned int i = 0; i != CharString.size(); ++i)
			{
				CTexture* tex = new CTexture(pRoot, pEditable);
				tex->lock();

				std::string pChar = "";
				pChar += CharString[i];
	
				CharList.push_back(tex);
				CharMap[pChar] = tex;

				tex->create(pFont, pChar, pSize, pBold, pItalic);
			}
		}			

		
		CFont::~CFont()
		{
			for(unsigned int i = 0; i != CharList.size(); ++i)
			{	
				CharList[i]->unlock();
				CharList[i]->remove();
			}
			
			((CVideoDriver*)pRoot->getVideoDriver())->removeFont(this);
		}
		
		ITexture* CFont::getChar(const std::string pChar)
		{
			return CharMap[pChar];
		}
		
		std::string CFont::getFileName() const
		{
			return FileName;
		}
			
		Vector CFont::getCharDimension(const std::string pChar)
		{
			if(CharMap[pChar])
				return CharMap[pChar]->getOriginalDimension();
				
			return Vector();
		}
		
		int CFont::getSize() const
		{
			return Size;
		}
		
		bool CFont::getBold() const
		{
			return Bold;
		}
		
		bool CFont::getItalic() const
		{
			return Italic;
		}
	};
};

