// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#include "gfx/LeeuwenGfx.h"
#include "CRoot.h"

namespace lew
{
	namespace gfx
	{
		IRoot* createRoot(const Vector pDimension, const bool pFullscreen,
		const bool pVsync, const bool pResizeable)
		{
			CRoot* pRoot = new CRoot(pDimension, pFullscreen, pVsync, pResizeable);

			return pRoot;
		}
	};
};

