// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _IButtonGfxHeader_
#define _IButtonGfxHeader_

#include "gfx/IMoveableInterfaceObject.h"

namespace lew 
{
	namespace gfx
	{
		class ITexture;
		class IInterfaceManager;
		class IButton : public IMoveableInterfaceObject
		{
			public:
				IButton(IInterfaceManager* m_pInterfaceManager = 0) : 
					IMoveableInterfaceObject(m_pInterfaceManager) {}
				
				virtual void setTheme(ITheme* m_pTheme) = 0;
				virtual void setPosition(const Vector pPosition) = 0;
				virtual void setLabel(const std::string pLabel) = 0;
				
				virtual std::string getLabel() const = 0;
				virtual bool getClicked() const = 0;
								
				virtual void update() = 0;
				virtual void draw() = 0;
		};
	};
};

#endif

