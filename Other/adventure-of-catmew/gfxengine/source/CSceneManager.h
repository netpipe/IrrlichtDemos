// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CSceneManagerGfxHeader_
#define _CSceneManagerGfxHeader_

#include <vector>
#include <map>

#include "gfx/ISceneManager.h"
#include "gfx/IRoot.h"

namespace lew
{
	namespace gfx
	{
		class CSceneManager : public ISceneManager
		{
			public:			
				CSceneManager(IRoot* m_pRoot);
				~CSceneManager();

				ISprite* createSprite(ITexture* pTexture = 0, 
					const Vector pSize = Vector(0.0f, 0.0f),
					const bool pRegister = true);
				
				ILine* createLine(const Vector pStart = Vector(0.0f, 0.0f), 
					const Vector pEnd = Vector(0.0f, 0.0f),
					const bool pRegister = true);
				
				IText* createText(const std::string pText = "", 
				IFont* pFont = 0,
				const bool pRegister = true);				

				void setWorldPosition(const Vector pPosition);
				void setWorldRotation(const float pRotation);
				void setLayerEnabled(const int pLayer, const bool pFlag);
				
				Vector getWorldPosition() const;
				Box getViewFrustum() const;
				bool getLayerEnabled(const int pLayer);
				int getObjectCount() const;
				int	getVisibleObjectCount() const;
				float getWorldRotation() const;
				
				std::vector<IObject*> getObjectsByName(const std::string pName);					
				std::vector<IObject*> getObjectsByID(const int pID);

				void addObject(IObject* pObject);
				void removeObject(IObject* pObject);
				void removeAllObjects();
				
				void renderSingleObject(IObject* pObject, 
					const bool pIgnoreLayerMask = false);
				
				void renderSingleLayer(const int pLayer);		
				void update();
				void prepareUpdate();				
				void applyWorldCoordinates();									
									
			private:			
				IRoot* pRoot;
				void calculateViewFrustum();
				
				int ObjectsVisible;				
				Vector WorldPosition;
				Vector WorldScale;
				float WorldRotation;
				
				std::vector<IObject*> ObjectList;
				std::map<int, bool> DisabledLayerMap;
				
				Box Frustum;
				
				int CurrentTime;
				int MaxTextureLayers;
				
				bool updatePrepared;
		};
	};
};

#endif

