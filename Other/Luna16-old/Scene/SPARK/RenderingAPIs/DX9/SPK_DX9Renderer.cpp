//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2009 - foulon matthieu - stardeath@wanadoo.fr					//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////


#include "RenderingAPIs/DX9/SPK_DX9Renderer.h"

namespace SPK
{
namespace DX9
{
	bool DX9Renderer::statesSaved = false;

	DX9Renderer::DX9Renderer() :
		reinit(false),
		blendingEnabled(false),
		srcBlending(D3DBLEND_SRCALPHA),
		destBlending(D3DBLEND_INVSRCALPHA),
		textureBlending(D3DTOP_MODULATE)
	{}

	DX9Renderer::~DX9Renderer() {}

	void DX9Renderer::setBlending(BlendingMode blendMode)
	{
		switch(blendMode)
		{
		case BLENDING_NONE :
			srcBlending = D3DBLEND_ONE;
			destBlending = D3DBLEND_ZERO;
			blendingEnabled = false;
			break;

		case BLENDING_ADD :
			srcBlending = D3DBLEND_SRCALPHA;
			destBlending = D3DBLEND_ONE;
			blendingEnabled = true;
			break;

		case BLENDING_ALPHA :
			srcBlending = D3DBLEND_SRCALPHA;
			destBlending = D3DBLEND_INVSRCALPHA;
			blendingEnabled = true;
			break;
		}
	}

	bool DX9Renderer::DX9PrepareBuffers(const Group& group)
	{
		if (!checkBuffers(group))
		{
			if (isBuffersCreationEnabled())
			{
				destroyBuffers(group);
				createBuffers(group);
				return true;
			}
			return false;
		}
		return true;
	}
}}
