// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "Leeuwen" Library.
// For conditions of distribution and use, see copyright notice in Leeuwen.h

#include "CLine.h"
#include "CRoot.h"
#include "CSceneManager.h"
#include "CTexture.h"

namespace lew 
{
	namespace gfx
	{
		CLine::CLine(IRoot* m_pRoot, const bool pRegister) : ILine(m_pRoot, pRegister)
		{
			Width = 1.0f;
			calculateRelativeBoundingBox();
		}
			
		void CLine::draw()
		{
			if(getMaterial()->getColor().a > 0 || getMaterial()->getBlendMode() == EBM_NONE)
			{
				pRoot->getVideoDriver()->renderLine(Start, End, pMaterial);
			}
		}
						
		void CLine::calculateRelativeBoundingBox()
		{
			if(Start > End)
			{
				RelativeBoundingBox = Box(End, Start);
			}
			else
			{
				RelativeBoundingBox = Box(Start, End);
			}
			
			for(unsigned int i = 0; i != 4; ++i)
				RelativeBoundingBox.point[i] *= Scale;	
				
				
			if(getRotation() != 0)
				RelativeBoundingBox.rotate(getRotation());
		}
		
		void CLine::setStart(const Vector pStart) 
		{
			Start = pStart;
			
			calculateRelativeBoundingBox();
		}
		
		void CLine::setEnd(const Vector pEnd)
		{
			End = pEnd;
			
			calculateRelativeBoundingBox();
		}
		
		void CLine::setWidth(const float pWidth)
		{
			Width = pWidth;
		}
		
		Vector CLine::getStart() const
		{
			return Start;
		}
		
		Vector CLine::getEnd() const
		{
			return End;
		}
		
		float CLine::getWidth() const
		{
			return Width;
		}
	};
};

