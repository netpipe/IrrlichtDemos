// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// created by Dean Wadsworth aka Varmint Dec 31 2007

#ifndef __I_MOTION_TRAIL_SCENE_NODE_H_INCLUDED__
#define __I_MOTION_TRAIL_SCENE_NODE_H_INCLUDED__

#include "irrlicht.h"
#include "ISceneNode.h"

namespace irr
{
namespace scene
{
	class IMotionTrailSceneNode : public ISceneNode
	{
	public:

		//! constructor
		IMotionTrailSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
			IrrlichtDevice* inDevice, 
			const core::vector3df& position,
			const core::vector3df& rotation,
			const core::vector3df& scale)
			: ISceneNode(parent, mgr, id, position, rotation, scale) {};

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_CUBE; }

        virtual void addTrail(core::vector3df inStart, core::vector3df inEnd) = 0;
		
		virtual void updateTrail(core::vector3df inStart, core::vector3df inEnd,  bool addMoreTrails = true) = 0;
		
		virtual void setLifeTime(f32 inLifeTime) = 0;
		virtual f32  getLifeTime() = 0;
		
		virtual void setMaxMotionTrails(u32 inMax) = 0;
		virtual u32  getMaxMotionTrails() = 0;
		
		virtual void setCurveTessel(f32 inTessel) = 0;
		virtual f32 getCurveTessel() = 0;

	};

} // end namespace scene
} // end namespace irr

#endif

