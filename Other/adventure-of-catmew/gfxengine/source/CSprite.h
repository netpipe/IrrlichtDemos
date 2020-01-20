// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CSpriteGfxHeader_
#define _CSpriteGfxHeader_

#include "gfx/ISprite.h"

namespace lew
{
	namespace gfx
	{
		class IRoot;
		class ISceneManager;
		class CSprite : public ISprite
		{
			public:				
				CSprite(IRoot* m_pRoot, const bool pRegister = true);
				
				void draw();
				void calculateRelativeBoundingBox();
		};
	};
};

#endif

