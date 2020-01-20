// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _IObjectGfxHeader_
#define _IObjectGfxHeader_

#include <iostream>

#include "gfx/Reference.h"
#include "gfx/Vector.h"
#include "gfx/Box.h"
#include "gfx/IMaterial.h"
#include "gfx/IRoot.h"
#include "gfx/ISceneManager.h"

namespace lew 
{
	namespace gfx
	{
		class IObject : public Reference
		{
			public:			
				IObject(IRoot* m_pRoot, const bool pRegister = true) : Reference()
				{
					pRoot = m_pRoot;
					Register = pRegister;
					
					Layer = 0;
					Track = 0;
					ID = -1;
					Visible = true;
					TrackPosition = false; 
					TrackRotation = false;
					Local = false;
					Culling = true;
					Rotation = 0.0f;
					OriginalRotation = 0.0f;
					AdditionalRotation = Vector(0.0f, 0.0f);
					Position = Vector(0.0f, 0.0f);
					OriginalPosition = Vector(0.0f, 0.0f);
					Scale = Vector(1.0f, 1.0f);
					OnScreen = false;
					Name = "";
					
					pMaterial = new IMaterial();
										
					if(Register)
					{
						pRoot->getSceneManager()->addObject(this);
					}
				} 
				
				virtual ~IObject()
				{
					setMaterial(0);
					setAutoTrack(0);
					
					if(Register)
					{
						pRoot->getSceneManager()->removeObject(this);
					}
				}
				
				virtual void update(const int pTime)
				{
					if(pMaterial)
					{
						pMaterial->update(pTime);
					}
					
					if(Track)
					{
						if(TrackPosition)
						{
							Position = OriginalPosition + Track->getPosition();
						}
							
						if(TrackRotation)
						{
							Rotation = OriginalRotation + Track->getRotation();
						}
					}
				}
				
				virtual void setPosition(const Vector pPosition)
				{
					OriginalPosition = pPosition;
					Position = pPosition;
				}
				
				virtual void setScale(const Vector pScale)
				{
					if(Scale != pScale)
					{
						Scale = pScale;
						calculateRelativeBoundingBox();
					}
				}
				
				virtual void setLayer(const int pLayer)
				{
					Layer = pLayer;
					
					if(Layer < 0)
						Layer = 0;
				}
				
				virtual void setRotation(const float pRotation)
				{
					if(Rotation != pRotation)
					{
						OriginalRotation = pRotation;
						Rotation = pRotation;
						
						calculateRelativeBoundingBox();
					}
				}
				
				virtual void setAdditionalRotation(const Vector pRotation)
				{
					AdditionalRotation = pRotation;
				}
				
				virtual void setVisible(const bool pFlag)
				{
					Visible = pFlag;
					
					if(!Visible)
						OnScreen = false;
				}
				
				virtual void setName(const std::string pName)
				{
					Name = pName;
				}
				
				virtual void setMaterial(IMaterial* mat)
				{
					if(pMaterial)
						pMaterial->remove();
						
					pMaterial = mat;
					
					if(pMaterial)
						pMaterial->grab();		
				}
				
				virtual void setLocal(const bool pFlag)
				{
					Local = pFlag;
					
					if(Local)
					{
						setCulling(false);
					}
					else
					{
						setCulling(true);
					}
				}
				
				virtual void setAutoTrack(IObject* pObject, 
				const bool pPosition = true, const bool pRotation = true)
				{
					TrackPosition = pPosition;
					TrackRotation = pRotation;
					
					if(Track)
						Track->remove();
						
					Track = pObject;
					
					if(Track)
					{
						Track->grab();						
						update(0);
					}
				}
				
				virtual IObject* getAutoTrack() const
				{
					return Track;
				}
				
				virtual void setCulling(const bool pFlag)
				{
					Culling = pFlag;
				}
				
				virtual void setID(const int pID)
				{
					ID = pID;
				}
				
				virtual Vector getPosition() const
				{
					return Position;
				}
				
				virtual Vector getAdditionalRotation() const
				{
					return AdditionalRotation;
				}
				
				virtual Vector getScale() const
				{
					return Scale;
				}
				
				virtual int getLayer() const
				{
					return Layer;
				}	
				
				virtual float getRotation() const
				{
					return Rotation;
				}
				
				virtual bool getVisible() const
				{
					return Visible;
				}
				
				virtual bool getLocal() const
				{
					return Local;
				}
				
				virtual bool getCulling() const
				{	
					return Culling;
				}
				
				virtual int getID() const
				{
					return ID;
				}	

				virtual IMaterial* getMaterial() const
				{
					return pMaterial;
				}
				
				virtual Box getRelativeBoundingBox() const
				{
					return RelativeBoundingBox;
				}
				
				virtual std::string getName() const
				{
					return Name;
				}
				
				virtual bool getOnScreen() 
				{ 
					if(!Visible)
						return false;
						
					if(!Culling)
						return true;
						
					Box AbsoluteBoundingBox = getAbsoluteBoundingBox();
					Box ViewFrustum = pRoot->getSceneManager()->getViewFrustum();
						
					if(ViewFrustum.isBoxIntersecting(AbsoluteBoundingBox))
					{
						return true;
					}
					
					//is this even needed for objects that are bigger than the frustum?
					/*if(AbsoluteBoundingBox.isBoxIntersecting(ViewFrustum))
					{
						return true;
					}*/
						
					return false;				
				}
				
				virtual bool getMouseOver()
				{
					//if(!Visible)
					//	return false;
						
					if(getAbsoluteBoundingBox().isPointInside
					(pRoot->getAbsoluteCursorPosition()))
					{
						return true;
					}
						
					return false;					 
				}
					
				virtual Box getAbsoluteBoundingBox() 
				{
					Box AbsoluteBoundingBox = 
					Box
					(
						Position + Vector(-32,32),
						Position + Vector(32,32),
						Position + Vector(32,-32),
						Position + Vector(-32,-32)
					);
					
					/*for(unsigned int i = 0; i != 4; ++i)
					{
						AbsoluteBoundingBox.point[i] += Position;
					
						if(Local)
						{
							AbsoluteBoundingBox.point[i] += pRoot->
								getSceneManager()->getWorldPosition();
								
							 AbsoluteBoundingBox.rotate(pRoot->
							 	getSceneManager()->getWorldRotation());
						}
					}*/

					return AbsoluteBoundingBox;						
				}

				//you need to derrive those
				virtual void calculateRelativeBoundingBox() = 0;
				virtual void draw() = 0;

			
			protected:			
				IRoot* pRoot;
				IObject* Track;
				
				bool TrackPosition; 
				bool TrackRotation;
				bool Local;
				bool Culling;
				
				Vector OriginalPosition;
				Vector Position;
				Vector Scale;
				Vector AdditionalRotation;
				
				bool Visible;
				bool OnScreen;
				bool Register;
				
				IMaterial* pMaterial;
				int Layer;
				int ID;
				float OriginalRotation;
				float Rotation;
				
				Box RelativeBoundingBox;
		
				std::string Name;
		};
	};
};

#endif

