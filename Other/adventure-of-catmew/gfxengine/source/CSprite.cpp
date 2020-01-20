// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "Leeuwen" Library.
// For conditions of distribution and use, see copyright notice in Leeuwen.h

#include "CSprite.h"
#include "CSceneManager.h"
#include "CRoot.h"
#include "CTexture.h"

namespace lew
{
	namespace gfx
	{
		CSprite::CSprite(IRoot* m_pRoot, const bool pRegister) :
		ISprite(m_pRoot, pRegister)
		{
			calculateRelativeBoundingBox();
		}

		void CSprite::calculateRelativeBoundingBox()
		{
			RelativeBoundingBox = Box();

			for(unsigned int i = 0; i != 4; ++i)
				RelativeBoundingBox.point[i] *= Scale;

			if(getRotation() != 0)
				RelativeBoundingBox.rotate(getRotation());
		}

		void CSprite::draw()
		{
			if(getMaterial()->getColor().a > 0 || getMaterial()->getBlendMode() == EBM_NONE)
			{
				pRoot->getVideoDriver()->renderBox(Box(), pMaterial);
			}
		}
	};
};

