// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "Leeuwen" Library.
// For conditions of distribution and use, see copyright notice in Leeuwen.h

#include "CText.h"
#include "CRoot.h"
#include "CSceneManager.h"
#include "CFont.h"
#include "CTexture.h"

namespace lew 
{
	namespace gfx
	{
		CText::CText(IRoot* m_pRoot, const bool pRegister) : IText(m_pRoot, pRegister)
		{
			pText = "";
			pFont = 0;
			HorizontalSpacing = 0;
			CharsVisible = 0;
			AverageHeight = 0.0f;
			Centered = true;
			
			setLocal(true);
			calculateRelativeBoundingBox();
			calculateAverageHeight();
		}
		
		void CText::calculateRelativeBoundingBox()
		{
			RelativeBoundingBox = Box();
			Vector scl = Vector(0, 0);
			
			if(pFont)
			{
				for(unsigned int i = 0; i != pText.size(); ++i)
				{
					std::string pChar = ""; pChar += pText[i];
					
					scl.x += getCharPosition(pChar).x;
					
					float Height = pFont->getCharDimension(pChar).y;
					
					if(Height > scl.y)
						scl.y = Height*-1;
				} 
			}
			
			RelativeBoundingBox.point[1].x += scl.x;
			RelativeBoundingBox.point[2] += scl;
			RelativeBoundingBox.point[3].y += scl.y;
			
			for(unsigned int i = 0; i != 4; ++i)
					RelativeBoundingBox.point[i].y += scl.y*-1;

			for(unsigned int i = 0; i != 4; ++i)
				RelativeBoundingBox.point[i] *= Scale;
				
			/*if(Rotation > 0)
			{
				RelativeBoundingBox.rotate(getRotation() + 
					pRoot->getSceneManager()->getWorldRotation());		
			}*/

			if(Centered)
			{
				Vector size = RelativeBoundingBox.getExtent();
				for(unsigned int i = 0; i != 4; ++i)
					RelativeBoundingBox.point[i] -= size/2;
			}
		}
		
		void CText::calculateAverageHeight()
		{					
			if(pFont)
			{
				AverageHeight = 0.0f;
				for(unsigned int i = 0; i != CharsVisible; ++i)
				{
					std::string ch = ""; ch += pText[i];
					AverageHeight += pFont->getCharDimension(ch).x/2;
				}	
				
				AverageHeight /= CharsVisible;
			}	
		}
			
		Vector CText::getCharOffset(const std::string pChar)
		{
			Vector pos = Vector();

			if(pFont)
			{
				ITexture* tex = ((CFont*)pFont)->getChar(pChar);
				
				if(tex)
				{
					pos.x += tex->getDimension().difference(pFont->getCharDimension(pChar)).x/2;
					pos.x += pFont->getCharDimension(pChar).x/2;
					
					pos.y += AverageHeight;
				}
			}

			return pos;
		}
		
		Vector CText::getCharPosition(const std::string pChar)
		{
			Vector pos = Vector();

			if(pFont)
			{
				ITexture* tex = ((CFont*)pFont)->getChar(pChar);
				
				if(tex)
				{
					pos.x += pFont->getCharDimension(pChar).x;
					pos.x += HorizontalSpacing;
				}
			}

			return pos;
		}
		
		bool CText::getCentered() const
		{
			return Centered;
		}
			
		void CText::draw()
		{
			if(getFont() && getMaterial()->getColor().a > 0)
			{	
				if(Centered)
				{
					Vector pscl = getRelativeBoundingBox().getExtent();
					
					//relative bounding box is already scaled,
					//but we scale below, so we need to revert it
					pscl /= Scale;
					
					pscl /= -2;
					
					pRoot->getVideoDriver()->setMatrix(EMT_TRANSFORMATION, 
					pscl, 0);
				}
				
				Box pBox = Box();
				Vector Pos = Vector(0, 0);
				for(unsigned int i = 0; i != CharsVisible; ++i)
				{	
					std::string pChar = ""; pChar += pText[i];
					
					if(pChar != "" && pChar != " " && pChar != "\n")
					{						
						ITexture* tex = ((CFont*)pFont)->getChar(pChar);

						if(tex)
						{
							pRoot->getVideoDriver()->saveMatrix();
							
							pRoot->getVideoDriver()->setMatrix(EMT_TRANSFORMATION,
							Pos + getCharOffset(pChar), 0);
							
							pRoot->getVideoDriver()->setMatrix(EMT_SCALE,
								tex->getDimension(), 0);
								
							pMaterial->setTexture(0, tex);
							pRoot->getVideoDriver()->renderBox(pBox, pMaterial);							
							pRoot->getVideoDriver()->restoreMatrix();
						}
					}
					
					Pos += getCharPosition(pChar); 
				}
				
				pMaterial->setTexture(0, 0);
			}
		}
		
		void CText::setCentered(const bool pFlag)
		{
			Centered = pFlag;			
			calculateRelativeBoundingBox();
		}	
		
		void CText::setText(const std::string m_pText)
		{
			if(pText != m_pText)
			{
				pText = m_pText;
			
				setCharsVisible(pText.size());
				calculateRelativeBoundingBox();
				calculateAverageHeight();
			}
		}	
		
		void CText::setHorizontalSpacing(const float pValue)
		{
			if(HorizontalSpacing != pValue)
			{
				HorizontalSpacing = pValue;
				calculateRelativeBoundingBox();
			}
		}
		
		void CText::setFont(IFont* m_pFont)
		{
			if(pFont != m_pFont)
			{
				if(pFont)
					pFont->remove();
					
				pFont = m_pFont;
				
				if(pFont)
					pFont->grab();
					
				calculateRelativeBoundingBox();
				calculateAverageHeight();
			}
		}

		void CText::setCharsVisible(const int pCount)
		{
			CharsVisible = pCount;
			
			if(CharsVisible > pText.size())
				CharsVisible = pText.size();
				
			if(CharsVisible < 0)
				CharsVisible = 0;
				
			calculateAverageHeight();
		}

		std::string CText::getText() const
		{
			return pText;
		}
		
		IFont* CText::getFont() const 
		{
			return pFont;
		}
		
		float CText::getHorizontalSpacing() const
		{
			return HorizontalSpacing;
		}
		
		int CText::getCharsVisible() const
		{
			return CharsVisible;
		}
	};
};

