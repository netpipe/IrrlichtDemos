#ifndef __CORE_SHADOW_H_INCLUDED__
#define __CORE_SHADOW_H_INCLUDED__

#include "CLightSceneNode.h"


class CShadowLightSceneNode : public irr::scene::CLightSceneNode
{
public:

	CShadowLightSceneNode(EffectHandler * effhand, irr::u32 res, irr::scene::ISceneNode * parent, irr::scene::ISceneManager * mgr, irr::s32 id, 
			const irr::core::vector3df& position, irr::video::SColorf color, irr::f32 near, irr::f32 far)
			 : irr::scene::CLightSceneNode(parent, mgr, id, position, color, 5.0f*far), ehandler(effhand), mapRes(res), nearValue(near), farValue(far)
	{
		for(int i = 0; i < 6; i++)
		{
			this->shadowLightIndices[i] = this->ehandler->getShadowLightCount();
			this->ehandler->addShadowLight(SShadowLight(this->mapRes, irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, 0, 0),
								    color, this->nearValue, this->farValue, 90.0f * irr::core::DEGTORAD, false));
		}
	}
	
	virtual void OnAnimate(irr::u32 timeMs)
	{
		this->updateAbsolutePosition();
		
		if(this->getLightType() == irr::video::ELT_POINT)
		{	
			for(int i = 0; i < 6; i++) {
				this->ehandler->getShadowLight(this->shadowLightIndices[i]).setPosition(this->getAbsolutePosition());
				//printf("%d\n", this->shadowLightIndices[i]);
			}
			
			this->ehandler->getShadowLight(this->shadowLightIndices[0]).setTarget(this->getAbsolutePosition() + irr::core::vector3df(0,0,1));
			this->ehandler->getShadowLight(this->shadowLightIndices[1]).setTarget(this->getAbsolutePosition() + irr::core::vector3df(0,0,-1));
			this->ehandler->getShadowLight(this->shadowLightIndices[2]).setTarget(this->getAbsolutePosition() + irr::core::vector3df(1,0,0));
			this->ehandler->getShadowLight(this->shadowLightIndices[3]).setTarget(this->getAbsolutePosition() + irr::core::vector3df(-1,0,0));
			this->ehandler->getShadowLight(this->shadowLightIndices[4]).setTarget(this->getAbsolutePosition() + irr::core::vector3df(0,-1,0));
			this->ehandler->getShadowLight(this->shadowLightIndices[5]).setTarget(this->getAbsolutePosition() + irr::core::vector3df(0,1,0));
		}
		
		irr::scene::CLightSceneNode::OnAnimate(timeMs);
	}

private:
	
	EffectHandler * ehandler;
	
	irr::u32 mapRes;
	irr::f32 nearValue;
	irr::f32 farValue;
	
	irr::u32 shadowLightIndices[6];
};

#endif
