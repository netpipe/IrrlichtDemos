// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _ISpriteGfxHeader_
#define _ISpriteGfxHeader_

#include "gfx/IObject.h"

namespace lew 
{
	namespace gfx
	{
		class ISceneManager;
		class ISprite : public IObject
		{
			public:
				ISprite(IRoot* m_pRoot, const bool pRegister = true) : 
					IObject(m_pRoot, pRegister) {}
		};
	};
};

#endif

