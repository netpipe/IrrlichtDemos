
#ifndef I_TRANSFORMABLE_H
#define I_TRANSFORMABLE_H

#include "litha_internal.h"
#include <vector>
#include "IMotionAnimator.h"
#include "IWorld.h"

class ITransformable : public virtual IReferenceCounted
{
	IWorld *world;
	ITransformable *parent;
	
	std::vector<ITransformable *> children;
	std::vector<IMotionAnimator *> animators;
	
	// For interpolation
	core::vector3df cachedPos;
	core::vector3df cachedRot;
	
	// Velocity temporary storage
	core::vector3df linearVel;
	
	bool firstVelocityCalculation;
	
protected:
	core::vector3df relativePos;
	core::vector3df relativeRot;
	
public:
	ITransformable()
	{
		world = NULL;
		parent = NULL;
		firstVelocityCalculation = true;
	}
	
	virtual ~ITransformable()
	{
		RemoveAllAnimators();
		RemoveAllChildren();
	}
	
	// This should be called after SetPosition/SetRotation in order to position an object
	// immediately. (i.e. when positioning initially at the start of a game or when teleporting to a new location).
	virtual void ApplyTransformNow()
	{
		CacheInterpolatableState();
		
		linearVel = core::vector3df(0,0,0);
		
		// Also immediately position all children.
		for (u32 i = 0; i < children.size(); i ++)
			children[i]->ApplyTransformNow();
	}
	
	// ************* Scene Graph *************
	
	void AddChild(ITransformable *child)
	{
		if (child == this || !child || !world)
			return;
		
		// Not already a child?
		
		for (u32 i = 0; i < children.size(); i ++)
		{
			if (children[i] == child)
				return;
		}
		
		// Child doesn't need to grab() this parent object since the parent will
		// destroy the child on destruction.
		child->parent = this;
		
		child->grab();
		children.push_back(child);
	}
	
	void RemoveAllChildren()
	{
		// Completely remove all children from world.
		while (children.size())
			RemoveChild(children.front());
		
		
		/*
		for (u32 i = 0; i < children.size(); i ++)
		{
			children[i]->parent = NULL;
			children[i]->drop();
			world->RemoveTransformable(children[i]);
		}
		
		children.clear();
		*/
	}
	
	void RemoveChild(ITransformable *child)
	{
		for (u32 i = 0; i < children.size(); i ++)
		{
			if (children[i] == child)
			{
				children.erase(children.begin()+i);
				child->parent = NULL;
				child->drop();
				world->RemoveTransformable(child);
				return;
			}
		}
	}
	
	ITransformable *GetParent()
	{
		return parent;
	}
	
	// Get all children which are a specific class.
	template<class Type> std::vector<Type *> GetChildrenByType()
	{
		std::vector<Type *> childrenOfType;
		
		for (u32 i = 0; i < children.size(); i ++)
		{
			if (Type *child = dynamic_cast<Type *>(children[i]))
				childrenOfType.push_back(child);
		}
		
		return childrenOfType;
	}
	
	template<class Type> Type *GetFirstChildOfType()
	{
		for (u32 i = 0; i < children.size(); i ++)
		{
			if (Type *child = dynamic_cast<Type *>(children[i]))
				return child;
		}
		
		return NULL;
	}
	
	// This is called internally by World.AddTransformable(..)
	// Requiring the scene graph to be enabled is really just a hack to avoid having
	// to pass the world pointer through all derived class constructors.
	void EnableSceneGraph(IWorld *world)
	{
		this->world = world;
	}
	
	// ************** Transformation **********
	// Rotations are euler angles in degrees.
	
	virtual void SetPosition(const core::vector3df &pos)
	{
		relativePos = pos;
	}
	
	virtual void SetRotation(const core::vector3df &rot)
	{
		relativeRot = rot;
	}
	
	virtual core::vector3df GetPosition()
	{
		return relativePos;
	}
	
	virtual core::vector3df GetRotation()
	{
		return relativeRot;
	}
	
	core::vector3df GetAbsolutePosition()
	{
		if (parent)
		{
			// this is hacked in.
			// also in GetAbsoluteRotation and the interpolation methods below.
			// perhaps should store transform in a single matrix...
			// needed to preserve rotate-then-translate behaviour. or something.
			core::matrix4 parentMat;
			parentMat.setTranslation(parent->GetAbsolutePosition());
			parentMat.setRotationDegrees(parent->GetAbsoluteRotation());
			core::matrix4 mat;
			mat.setTranslation(GetPosition());
			mat.setRotationDegrees(GetRotation());
			core::matrix4 absMat = parentMat * mat;
			return absMat.getTranslation();
			//return parent->GetAbsolutePosition() + GetPosition();
		}
		else
			return GetPosition();
	}
	
	core::vector3df GetAbsoluteRotation()
	{
		if (parent)
		{
			core::matrix4 parentMat;
			parentMat.setTranslation(parent->GetAbsolutePosition());
			parentMat.setRotationDegrees(parent->GetAbsoluteRotation());
			core::matrix4 mat;
			mat.setTranslation(GetPosition());
			mat.setRotationDegrees(GetRotation());
			core::matrix4 absMat = parentMat * mat;
			return absMat.getRotationDegrees();
			//return combine_rotations(parent->GetAbsoluteRotation(), GetRotation());
		}
		else
			return GetRotation();
	}
	
	// ************** Animators ***********
	
	void AddAnimator(IMotionAnimator *animator)
	{
		animators.push_back(animator);
		animator->grab();
		animator->Init(this);
	}
	
	void RemoveAllAnimators()
	{
		for (u32 i = 0; i < animators.size(); i ++)
			animators[i]->drop();
		
		animators.clear();
	}
	
	const std::vector<IMotionAnimator *> &GetAnimators()
	{
		return animators;
	}
	
	// **************** Velocity utilities ***************
	
	// There is a lag of one frame (the velocity given is the velocity of the previous frame).
	// This is required because GetLinearVelocity might be called *after* the old position has been cached
	// but before a new position is calculated (which would always yield a velocity of zero).
	// On the first and second frames after calling ApplyTransformNow() the velocity will be zero.
	// NOTE: There is no GetAngularVelocity method as it is impossible to calculate simply by observing
	// the rotation: rotations wrap around so you cannot know how many revolutions have occurred.
	virtual core::vector3df GetLinearVelocity()
	{
		// velocity is calculated by CalculateVelocities(..)
		if (firstVelocityCalculation)
			return core::vector3df(0,0,0);
		else
			return linearVel;
	}
	
	core::vector3df GetAbsoluteLinearVelocity()
	{
		if (parent)
			return parent->GetAbsoluteLinearVelocity() + GetLinearVelocity();
		else
			return GetLinearVelocity();
	}
	
	// ************** Interpolation utility ***************
	// These interpolation methods are used internally and can be ignored.
	
	// Called by LogicTask. So in render task can interpolate from a cached
	// state to the current state, for smooth movement even if the logic
	// is run at a low FPS.
	// Can be overridden if there are extra factors that effect the positon
	// or rotation and need interpolating.
	// cachedPos is also used for velocity calculation.
	virtual void CacheInterpolatableState()
	{
		cachedPos = GetPosition();
		cachedRot = GetRotation();
	}
	
	core::vector3df GetCachedPosition()
	{
		return cachedPos;
	}
	
	core::vector3df GetCachedRotation()
	{
		return cachedRot;
	}
	
	// Called internally, do not use.
	void CalculateVelocities(f32 dt)
	{
		// Delta velocity since last frame is velocity per dt...
		// So we divide by dt to convert it to per second.
		linearVel = (GetPosition() - cachedPos) / dt;
		// angular velocity cannot be calculated since rotations wrap around.
		
		// mark that we have calculated velocity!
		if (firstVelocityCalculation)
		{
			firstVelocityCalculation = false;
			linearVel = core::vector3df(0,0,0);
		}
	}
	
	// Basic interpolation of position and rotation.
	// Called by render task just before rendering.
	// Interpolates from a cached transformation (cached at 2nd-to-last logic update) to the current
	// transformation (as set in the last logic update).
	// This can be overridden to give more complicated interpolation.
	// (for example, perfect camera movement at a low logic FPS requires interpolation
	// in polar coordinates around a target position)
	
	virtual core::vector3df GetInterpolatedPosition(f32 alpha)
	{
		return maths::interpolate_position(GetCachedPosition(), GetPosition(), alpha);
	}
	
	virtual core::vector3df GetInterpolatedRotation(f32 alpha)
	{
		return maths::interpolate_rotation(GetCachedRotation(), GetRotation(), alpha);
	}
	
	core::vector3df GetInterpolatedAbsolutePosition(f32 alpha)
	{
		if (parent)
		{
			core::matrix4 parentMat;
			parentMat.setTranslation(parent->GetInterpolatedAbsolutePosition(alpha));
			parentMat.setRotationDegrees(parent->GetInterpolatedAbsoluteRotation(alpha));
			core::matrix4 mat;
			mat.setTranslation(GetInterpolatedPosition(alpha));
			mat.setRotationDegrees(GetInterpolatedRotation(alpha));
			core::matrix4 absMat = parentMat * mat;
			return absMat.getTranslation();
			//return parent->GetInterpolatedAbsolutePosition(alpha) + GetInterpolatedPosition(alpha);
		}
		else
			return GetInterpolatedPosition(alpha);
	}
	
	core::vector3df GetInterpolatedAbsoluteRotation(f32 alpha)
	{
		if (parent)
		{
			core::matrix4 parentMat;
			parentMat.setTranslation(parent->GetInterpolatedAbsolutePosition(alpha));
			parentMat.setRotationDegrees(parent->GetInterpolatedAbsoluteRotation(alpha));
			core::matrix4 mat;
			mat.setTranslation(GetInterpolatedPosition(alpha));
			mat.setRotationDegrees(GetInterpolatedRotation(alpha));
			core::matrix4 absMat = parentMat * mat;
			return absMat.getRotationDegrees();
			//return combine_rotations(parent->GetInterpolatedAbsoluteRotation(alpha), GetInterpolatedRotation(alpha));
		}
		else
			return GetInterpolatedRotation(alpha);
	}
};

#endif
