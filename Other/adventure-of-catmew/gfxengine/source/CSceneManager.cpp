// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#include "gfx/Error.h"
#include "gfx/ITexture.h"
#include "gfx/IVideoDriver.h"

#include "CSceneManager.h"
#include "CSprite.h"
#include "CText.h"
#include "CLine.h"

namespace lew
{
	namespace gfx
	{
		class ObjectSorter
		{
			public:
			bool operator()(IObject* pStart, IObject* pEnd) const
			{
				return pStart->getLayer() > pEnd->getLayer();
			}
		};
		
		CSceneManager::CSceneManager(IRoot* m_pRoot) : ISceneManager()
		{
			pRoot = m_pRoot;
			ObjectsVisible = 0;
			WorldRotation = 0.0f;
			CurrentTime = 0;
			updatePrepared = false;
		}
		
		CSceneManager::~CSceneManager()
		{
			removeAllObjects();
		}
		
		ISprite* CSceneManager::createSprite(ITexture* pTexture, 
		const Vector pSize, const bool pRegister)
		{
			ISprite* spr = new CSprite(pRoot, pRegister);
			
			spr->getMaterial()->setTexture(0, pTexture);
			
			if(pSize == 0.0f && pTexture)
			{
				spr->setScale(pTexture->getDimension());
			}
			else
			{
				spr->setScale(pSize);
			}
			
			return spr;
		}
		
		ILine* CSceneManager::createLine(const Vector pStart, 
		const Vector pEnd, const bool pRegister)
		{
			ILine* pLine = new CLine(pRoot, pRegister);

			pLine->setStart(pStart);
			pLine->setEnd(pEnd);

			return pLine;		
		}
		
		IText* CSceneManager::createText(const std::string pText, 
		IFont* pFont, const bool pRegister)
		{
			IText* m_pText = new CText(pRoot, pRegister);

			m_pText->setText(pText);
			m_pText->setFont(pFont);

			return m_pText;		
		}
					
		void CSceneManager::setLayerEnabled(const int pLayer, const bool pFlag)
		{
			DisabledLayerMap[pLayer] = pFlag;	
		}
		
		void CSceneManager::setWorldPosition(const Vector pPosition)
		{
			WorldPosition = pPosition;
		}
		
		void CSceneManager::setWorldRotation(const float pRotation)
		{
			WorldRotation = pRotation;
		}
		
		Vector CSceneManager::getWorldPosition() const
		{
			return WorldPosition;
		}
		
		Box CSceneManager::getViewFrustum() const
		{
			return Frustum;
		}
		
		bool CSceneManager::getLayerEnabled(const int pLayer)
		{
			return DisabledLayerMap[pLayer];
		}
		
		int CSceneManager::getObjectCount() const
		{
			return ObjectList.size();
		}
		
		int	CSceneManager::getVisibleObjectCount() const
		{
			return ObjectsVisible;
		}
		
		float CSceneManager::getWorldRotation() const
		{
			return WorldRotation;
		}
		
		std::vector<IObject*> CSceneManager::getObjectsByName(const 
		std::string pName)
		{
			std::vector<IObject*> Mask;
			
			for(unsigned int i = 0; i != ObjectList.size(); ++i)
			{
				if(ObjectList[i]->getName() == pName)
					Mask.push_back(ObjectList[i]);
			}
			
			return Mask;
		}
						
		std::vector<IObject*> CSceneManager::getObjectsByID(const int pID)
		{
			std::vector<IObject*> Mask;
			
			for(unsigned int i = 0; i != ObjectList.size(); ++i)
			{
				if(ObjectList[i]->getID() == pID)
					Mask.push_back(ObjectList[i]);
			}
			
			return Mask;
		}
		
		void CSceneManager::addObject(IObject* pObject)
		{
			ObjectList.push_back(pObject);
		}
		
		void CSceneManager::removeObject(IObject* pObject)
		{
			bool _Success = false;
			std::vector<IObject*>::iterator Iter;

			for(Iter = ObjectList.begin(); Iter != ObjectList.end(); ++Iter)
			{
				if((*Iter) == pObject)
				{
					_Success = true;
					break;
				}
			}

			if(_Success)
				ObjectList.erase(Iter);		
		}
		
		void CSceneManager::removeAllObjects()
		{
			std::vector<IObject*> Queue = ObjectList;

			for(unsigned int i = 0; i != Queue.size(); ++i)
				Queue[i]->remove();	
				
			ObjectList.clear();
		}
		
		void CSceneManager::renderSingleObject(IObject* pObject, 
		const bool pIgnoreLayerMask)
		{	
			pObject->update(CurrentTime);
		
			bool DisabledLayer = DisabledLayerMap[pObject->getLayer()];
			
			if(pIgnoreLayerMask)
				DisabledLayer = false;

						
			if(!DisabledLayer && pObject->getOnScreen())
			{
				pRoot->getVideoDriver()->saveMatrix();
				ObjectsVisible += 1;
				
				Vector Pos = pObject->getPosition();
				Vector Scale = pObject->getScale();
				float Rot = pObject->getRotation() + WorldRotation;
				
				if(pObject->getLocal())
				{
					Pos += WorldPosition;
					Rot -= WorldRotation;
				}
				
				pRoot->getVideoDriver()->setMatrix(EMT_TRANSFORMATION, 
					Pos, (pObject->getLayer())*-1);
				
				pRoot->getVideoDriver()->setMatrix(EMT_ROTATION,
					pObject->getAdditionalRotation().x, 
					pObject->getAdditionalRotation().y, Rot);
					
				pRoot->getVideoDriver()->setMatrix(EMT_SCALE, Scale, 1.0f);				
				pObject->draw();
				
				pRoot->getVideoDriver()->restoreMatrix();
			}	
		}
		
		void CSceneManager::renderSingleLayer(const int pLayer)
		{
			for(unsigned int i = 0; i != ObjectList.size(); ++i)
			{
				if(ObjectList[i]->getLayer() == pLayer)
					renderSingleObject(ObjectList[i]);		
			}		
		}
		
		void CSceneManager::prepareUpdate()
		{
			if(!updatePrepared)
			{
				updatePrepared = true;
				
				CurrentTime = pRoot->getTime();
				ObjectsVisible = 0;	

				//depth-sort all renderobjects
				std::sort(ObjectList.begin(), ObjectList.end(), ObjectSorter());
				calculateViewFrustum();
			}

			applyWorldCoordinates();
		}	
				
		void CSceneManager::update()
		{
			prepareUpdate();
			
			for(unsigned int i = 0; i != ObjectList.size(); ++i)
				renderSingleObject(ObjectList[i]);		
				
				
			updatePrepared = false;
		}
						
		void CSceneManager::calculateViewFrustum()
		{	
			Vector vpMin = pRoot->getVideoDriver()->getViewPortMin();
			Vector vpMax = pRoot->getVideoDriver()->getViewPortMax();
			
			Vector Dimension = vpMax.difference(vpMin); 
			
			
			Frustum.point[0] = Vector(WorldPosition.x, 
				WorldPosition.y + Dimension.y);
			
			Frustum.point[1] = Vector(WorldPosition.x + Dimension.x, 
				WorldPosition.y + Dimension.y);
			
			Frustum.point[2] = Vector(WorldPosition.x + Dimension.x, 
				WorldPosition.y);
			
			Frustum.point[3] = Vector(WorldPosition.x, WorldPosition.y);				
		}
		
		void CSceneManager::applyWorldCoordinates()
		{
			//clear previous coordinates
			pRoot->getVideoDriver()->resetActiveMatrix();	
			
			//pRoot->getVideoDriver()->setMatrix(EMT_ROTATION, 0.0f, 0.0f, 
			//WorldRotation);	
			
			//we use other coordinates in Box, so flip them around
			pRoot->getVideoDriver()->setMatrix(EMT_TRANSFORMATION,
			WorldPosition*-1, -1);
		}
	};
};

