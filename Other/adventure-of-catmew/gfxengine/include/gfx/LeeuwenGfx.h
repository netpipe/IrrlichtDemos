// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

/*
	LeeuwenGfx.h -- interface of the "LeeuwenGfx" Library

	Copyright (C) 2007 Kevin van Leeuwen

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you must not
		claim that you wrote the original software. If you use this software
		in a product, an acknowledgment in the product documentation would be
		appreciated but is not required.

	2.	Altered source versions must be plainly marked as such, and must not be
     	misrepresented as being the original software.

	3.	This notice may not be removed or altered from any source distribution.
*/

#ifndef _LeeuwenGfxHeader_
#define _LeeuwenGfxHeader_

#include <string>

#include "gfx/Vector.h"
#include "gfx/Error.h"
#include "gfx/KeyCodes.h"
#include "gfx/Color.h"
#include "gfx/Box.h"
#include "gfx/Noise.h"
#include "gfx/ITexture.h"
#include "gfx/IShader.h"
#include "gfx/IMaterial.h"
#include "gfx/IObject.h"
#include "gfx/ISprite.h"
#include "gfx/ILine.h"
#include "gfx/IFont.h"
#include "gfx/IText.h"
#include "gfx/ISceneManager.h"
#include "gfx/IVideoDriver.h"
#include "gfx/IRoot.h"

/*! \mainpage LeeuwenGfx Library 1.0 API documentation
 *
 *
 * \section intro Introduction
 *
 * Text Text Text Text Text Text Text Text Text Text Text
 * Text Text Text Text Text Text Text Text Text Text Text
 * Text Text Text Text Text Text Text Text Text Text Text
 * Text Text Text Text Text Text Text Text Text Text Text
 * Text Text Text Text Text Text Text Text Text Text Text
 * Text Text Text Text Text Text Text Text Text Text Text
 * Text Text Text Text Text Text Text Text Text Text Text
 * Text Text Text Text Text Text Text Text Text Text Text
 *
 *
 * <br>
 *
 * \section example Short example
 *
 * This example shows how to quickly initialize the engine.
 *
 * \code
#include "gfx/LeeuwenGfx.h"

#if defined(WINDOWS)
#include <windows.h>
#endif

using namespace lew;
using namespace gfx;

#if defined(LINUX)
int main(int argc, char* argv[])
#endif
#if defined(WINDOWS)
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
#endif
{
	IRoot* pRoot = createRoot(Vector(320, 240));

	ISceneManager* pSceneManager = pRoot->getSceneManager();
	IVideoDriver* pVideoDriver = pRoot->getVideoDriver();


	pVideoDriver->setBackgroundColor(Color(0.0f, 0.0f, 1.0f, 1.0f));
	pRoot->setWindowTitle("LeeuwenGfx Library");


	while(pRoot->loop())
	{
		if(pRoot->getWindowActive())
		{
			pVideoDriver->beginFrame();
			pSceneManager->update();
			pVideoDriver->endFrame();
		}
	}

	pRoot->remove();
	return 0;
}
 * \endcode
 */

namespace lew
{
	namespace gfx
	{
		//! creates an instance of the IRoot class. It is used to manage events
		//! and all other managers.
		//! You have to call instance->remove() yourself to delete it.
		//! \param pDimension: the window dimension.
		//! \param pFullscreen: enable or disable fullscreen-mode.
		//! \param pVSync: enable or disable vertical synchronization of the window.
		//! \param pResizeable: enable or disable window resizing.
		//! \return a pointer to the new IRoot instance.
		IRoot* createRoot(const Vector pDimension = Vector(),
			const bool pFullscreen = false, const bool pVsync = false, const bool pResizeable = false);
	};
};

#endif

