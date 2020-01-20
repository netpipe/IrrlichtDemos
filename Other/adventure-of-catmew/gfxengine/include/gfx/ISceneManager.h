// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _ISceneManagerGfxHeader_
#define _ISceneManagerGfxHeader_

#include <vector>
#include <string>

#include "gfx/Reference.h"
#include "gfx/Color.h"
#include "gfx/Box.h"

namespace lew 
{
	namespace gfx
	{
		class IObject;
		class ISprite;
		class IText;
		class IFont;
		class ITexture;
		class ILine;
		class ISceneManager : public Reference
		{
			public:			
				ISceneManager() : Reference() {} 
				virtual ~ISceneManager() {}
				
				virtual ISprite* createSprite(ITexture* pTexture = 0, 
					const Vector pSize = Vector(0.0f, 0.0f),
					const bool pRegister = true) = 0;
				
				virtual ILine* createLine(const Vector pStart = 
					Vector(0.0f, 0.0f), const Vector pEnd = Vector(0.0f, 0.0f),
					const bool pRegister = true) = 0;
				
				virtual IText* createText(const std::string pText = "",
					IFont* pFont = 0,
					const bool pRegister = true) = 0;				


				virtual void setLayerEnabled(const int pLayer, const bool pFlag) = 0;
				virtual void setWorldPosition(const Vector pPosition) = 0;
				virtual void setWorldRotation(const float pRotation) = 0;
				
				virtual Vector getWorldPosition() const = 0;
				virtual Box getViewFrustum() const = 0;
				virtual bool getLayerEnabled(const int pLayer) = 0;
				virtual int getObjectCount() const = 0;
				virtual int	getVisibleObjectCount() const = 0;
				virtual float getWorldRotation() const = 0;
				
				virtual std::vector<IObject*> getObjectsByName(const 
					std::string pName) = 0;
					
				virtual std::vector<IObject*> getObjectsByID(const
					int pID) = 0;

				virtual void addObject(IObject* pObject) = 0;
				virtual void removeObject(IObject* pObject) = 0;
				virtual void removeAllObjects() = 0;
				
				virtual void renderSingleObject(IObject* pObject, 
					const bool pIgnoreLayerMask = false) = 0;
				
				virtual void renderSingleLayer(const int pLayer) = 0;		
				virtual void update() = 0;
				virtual void prepareUpdate() = 0;	
		};
	};
};

#endif

