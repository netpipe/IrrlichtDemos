// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// created by Dean Wadsworth aka Varmint Jan 12th 2008

#ifndef __C_MOTION_TRAILSCENE_NODE_H_INCLUDED__
#define __C_MOTION_TRAILSCENE_NODE_H_INCLUDED__

#include "IMotionTrailSceneNode.h"
#include "SMeshBuffer.h"

namespace irr
{
namespace scene
{
	class CMotionTrailSceneNode : public IMotionTrailSceneNode
	{
	public:

		//! constructor
		CMotionTrailSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
			IrrlichtDevice* inDevice, 
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));
		
		~CMotionTrailSceneNode();

		virtual void OnRegisterSceneNode();

		//! renders the node.
		virtual void render();

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hirachy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		virtual video::SMaterial& getMaterial(u32 i);

		//! returns amount of materials used by this scene node.
		virtual u32 getMaterialCount() const;

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_CUBE; }

		//! Writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

		//! Reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

		//! Creates a clone of this scene node and its children.
		virtual ISceneNode* clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
		
		virtual void addTrail(core::vector3df inStart, core::vector3df inEnd);
		
		virtual void updateTrail(core::vector3df inStart, core::vector3df inEnd, bool addMoreTrails = true);
		
		virtual void setLifeTime(f32 inLifeTime) { lifetime = inLifeTime; }
		virtual f32  getLifeTime() { return lifetime; }
		
		virtual void setMaxMotionTrails(u32 inMax) { maxMotionTrails = inMax; };
		virtual u32  getMaxMotionTrails() { return maxMotionTrails; };
		
		virtual void setCurveTessel(f32 inTessel) { curveTessel = inTessel; };
		virtual f32  getCurveTessel() { return curveTessel; };

	private:
	
		void renderTrail(u32 currentTrail);
		void updateBoundingBox();
		
		IrrlichtDevice* irrDevice;
		
		struct vertpoints_s {
			f32 x,y,z;
		};
		struct texcoords_s {
			f32 x,y;
		};

		struct motionTrailSection
		{
		   core::vector3df	start;
		   core::vector3df	end;
		   f32				ElapsedTime;
		   f32				startAlpha;

		   u8           * colour;
		   texcoords_s  * texCoords;
		   vertpoints_s * verts;
		   u16          * indicies;
		   s32 indices_count;

		   motionTrailSection * Next;
		   motionTrailSection * Prev;
		};
		
		struct motionTrailLinkList {
			motionTrailSection * Head;
			motionTrailSection * Curr;
		};
		
		u32 trailCount;
		motionTrailLinkList trailList;
		video::SMaterial trailMaterial;
		u32 maxMotionTrails;				// Number of trails
		
		f32 curveTessel;					// Small values produce a high curve tesselation
											// High values produce a low curve tesselation
		
		f32 lifetime;						//now long each trail section should last
		u32 lastTime;
		core::aabbox3d<f32> boundingBox;
		core::vector3df lastAddedPos;

	};

} // end namespace scene
} // end namespace irr

#endif

