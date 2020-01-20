// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _ILineGfxHeader_
#define _ILineGfxHeader_

#include "gfx/IObject.h"

namespace lew 
{
	namespace gfx
	{
		class ISceneManager;
		class ILine : public IObject
		{
			public:			
				ILine(IRoot* m_pRoot, const bool pRegister = true) : 
					IObject(m_pRoot, pRegister) {}
							
				virtual void setStart(const Vector pStart) = 0; 				
				virtual void setEnd(const Vector pEnd) = 0;
				virtual void setWidth(const float pWidth) = 0;
				
				virtual Vector getStart() const = 0;
				virtual Vector getEnd() const = 0;
				virtual float getWidth() const = 0;
		};
	};
};

#endif

