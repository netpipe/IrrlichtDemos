// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CLineGfxHeader_
#define _CLineGfxHeader_

#include "gfx/ILine.h"

namespace lew 
{
	namespace gfx
	{
		class IRoot;
		class ISceneManager;
		class CLine : public ILine
		{
			public:			
				CLine(IRoot* m_pRoot, const bool pRegister = true);
				
				void setStart(const Vector pStart);
				void setEnd(const Vector pEnd);
				void setWidth(const float pWidth);

				Vector getStart() const;
				Vector getEnd() const;
				float getWidth() const;

				void draw();		
				void calculateRelativeBoundingBox();
				
				
			private:				
				Vector Start;
				Vector End;
				
				float Width;
		};
	};
};

#endif

