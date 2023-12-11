
#include "Mesh.h"
#include "IShader.h"

Mesh::Mesh(scene::ISceneManager *smgr, const c8 *name)
{
	this->smgr = smgr;
	
	meshNode = smgr->addAnimatedMeshSceneNode( smgr->getMesh(name) );
	
	// this no longer has an effect since materials are loaded only from files
	// (due to addition of "pass" system)
	//meshNode->setMaterialFlag(video::EMF_LIGHTING, false);
	
	// Create the first pass using materials from the given mesh.
	// meshNode should (WILL!) have the same number of materials as this.
	AddPass(name);
	
	// Animation...
	meshNode->setJointMode(scene::EJUOR_CONTROL);
	currentAnimation = NULL;
	
	//onFirstFrame = true;
	
	// We animate the mesh here once to calculate the bounding box.
	// Otherwise it isn't calculated by Irrlicht until one render has been done.
	meshNode->OnAnimate(0);
}

Mesh::~Mesh()
{
	meshNode->remove();
	
	// struct Pass drops shaders on destruction.
}

core::aabbox3df Mesh::GetBoundingBox()
{
	// For the box to be valid it has to be OnAnimate()ed at least once.
	// That is done in the constructor.
	core::aabbox3df box = meshNode->getBoundingBox();
	
	// Apply the permanent translation, rotation, and scale, to the box.
	// (the AABB got from the meshNode is the BB of the completely untransformed animated mesh.
	// so no position, rot or scale will have been applied to it).
	
	// Scale...
	// Don't need a matrix for this.
	// Plus, in Irrlicht's getRelativeTransform() scaling is done separately.
	if (meshNode->getScale() != core::vector3df(1,1,1))
	{
		// Apply the "permanent" scale
		box.MinEdge *= meshNode->getScale();
		box.MaxEdge *= meshNode->getScale();
	}
	
	// Translation and rotation.
	core::matrix4 mat;
	mat.setTranslation(translation);
	mat.setRotationDegrees(rotation);
	
	// Apply the transform
	mat.transformBoxEx(box);
	
	return box;
}

u32 Mesh::GetMaterialCount()
{
	ASSERT( passes.size() > 0 );
	
	return passes[0].materials.size();
}

video::SMaterial &Mesh::GetMaterial(u32 material, u16 pass)
{
	ASSERT( pass < GetPassCount() );
	ASSERT( material < passes[pass].materials.size() );
	
	return passes[pass].materials[material];
}

void Mesh::SetShader(u32 material, IShader *shader, u16 pass)
{
	ASSERT( pass < GetPassCount() );
	ASSERT( material < passes[pass].materials.size() );
	ASSERT( passes[pass].materials.size() == passes[pass].shaders.size() );
	
	if (shader)
	{
		// New shader? Apply to the material.
		shader->ApplyToIrrMaterial(passes[pass].materials[material]);
	}
	else if (passes[pass].shaders[material])
	{
		// Shader is NULL, so...
		// Was previously a shader set? Clear it from this material.
		// (it will then be dropped by SET_REF_COUNTED_POINTER below)
		passes[pass].shaders[material]->ClearFromIrrMaterial(passes[pass].materials[material], video::EMT_SOLID);
	}
	
	SET_REF_COUNTED_POINTER(passes[pass].shaders[material], shader)
}

void Mesh::SetAllShaders(IShader *shader, u16 pass)
{
	ASSERT( pass < GetPassCount() );
	
	for (u32 i = 0; i < passes[pass].materials.size(); i ++)
		SetShader(i, shader, pass);
}

void Mesh::DisableMaterial(u32 material, u16 pass)
{
	ASSERT( pass < GetPassCount() );
	ASSERT( material < passes[pass].materials.size() );
	
	passes[pass].materials[material].FrontfaceCulling = true;
	passes[pass].materials[material].BackfaceCulling = true;
}

void Mesh::DisableAllMaterials(u16 pass)
{
	ASSERT( pass < GetPassCount() );
	
	for (u32 i = 0; i < passes[pass].materials.size(); i ++)
		DisableMaterial(i, pass);
}

void Mesh::SetVisible(bool visible)
{
	meshNode->setVisible(visible);
}

void Mesh::ReceiveRenderPosition(core::vector3df pos)
{
	meshNode->setPosition(pos);
}

void Mesh::ReceiveRenderRotation(core::vector3df rot)
{
	// Render at a permanent rotation set by Rotate
	meshNode->setRotation(maths::combine_rotations(rotation,rot));
	
	// Also, offset permanent translation here.
	// and apply rotation to it.
	core::quaternion quat(rot * core::DEGTORAD);
	meshNode->setPosition( meshNode->getPosition() + quat*translation );
}

u16 Mesh::GetPassCount()
{
	return passes.size();
}

void Mesh::SetCurrentPass(u16 pass)
{
	if (pass < GetPassCount())
	{
		// Set all mesh node materials to those from the appropriate pass.
		
		ASSERT( meshNode->getMaterialCount() == passes[pass].materials.size() );
		
		for (u32 i = 0; i < meshNode->getMaterialCount(); i ++)
		{
			meshNode->getMaterial(i) = passes[pass].materials[i];
		}
	}
	else
	{
		// No materials set for this pass, so must hide node completely.
		// Do this by setting both frontface and backface culling to true.
		// (should not use meshNode->setVisible() as that effects child nodes too)
		for (u32 i = 0; i < meshNode->getMaterialCount(); i ++)
		{
			meshNode->getMaterial(i).BackfaceCulling = true;
			meshNode->getMaterial(i).FrontfaceCulling = true;
		}
	}
	
	if (pass == 0)
	{
		// Only need to do this for the first pass...
		
		if (currentAnimation)
		{
			/*
			// This seems not to be required any more... :S
			// Perhaps Irrlicht 1.6.1 fixed it?
			
			if (onFirstFrame)
			{
				// We need some special behaviour for the first frame after setting the frame loop
				// to fix an Irrlicht bug.
				onFirstFrame = false;
				
				// Fix Irrlicht bug.
				// Problem is, time glitches caused by loading.
				// Causes CurrentFrameNr to go way past end of loop.
				// Then, internal Irrlicht behaviour to fix this is to count down
				// by (EndFrame-StartFrame)
				// 		--> CurrentFrameNr -= (EndFrame-StartFrame);
				// until it reaches the correct range.
				// Which takes several frames...
				// This here is not a complete fix, as there may be pauses elsewhere which could
				// cause the glitch.
				meshNode->setCurrentFrame(currentAnimation->start);
			}
			*/
			
			// Animation
			meshNode->animateJoints();
			
			// Possibly end an animation if it is not looped.
			// Irrlicht's OnAnimationEnd callback didn't seem to work??
			// (At least in 1.4.2)
			if (!currentAnimation->looped && meshNode->getFrameNr() >= currentAnimation->end-1)
			{
				HaltAnimation();
			}
		}
	}
}

void Mesh::AddPass(const c8 *meshName)
{
	scene::IAnimatedMesh *animatedMesh = smgr->getMesh(meshName);
	
	if (animatedMesh)
	{
		scene::IMesh *mesh = animatedMesh->getMesh(0,0);
		
		if (mesh)
		{
			passes.push_back( Pass(mesh) );
			
			if (passes.size() > 1)
			{
				// Ensure new pass has the same amount of materials.
				ASSERT( passes[passes.size()-1].materials.size() == passes[0].materials.size() );
			}
		}
	}
}

void Mesh::Render(u16 pass)
{
	/*
	// Render the bounding box? Can be useful for debugging...
	{
		video::IVideoDriver *driver = GetEngine()->GetIrrlichtDevice()->getVideoDriver();
		
		// Absolute transformation. Scale is a permanent part of BB.
		core::matrix4 mat;
		mat.setRotationDegrees(GetAbsoluteRotation());
		mat.setTranslation(GetAbsolutePosition());
		
		driver->setTransform(video::ETS_WORLD, mat);
		driver->draw3DBox(GetBoundingBox(), video::SColor(255,255,0,0));
	}
	*/
}

void Mesh::DefineAnimation(s32 id, u32 start, u32 end, f32 fps, bool looped, bool interruptible, f32 transitionTime)
{
	Animation anim =
	{
		start,end,
		fps,
		looped,
		interruptible,
		transitionTime
	};
	
	animations[id] = anim;
}

void Mesh::SetAnimation(s32 id)
{
	if (currentAnimation && !currentAnimation->interruptible)
		return;
	
	if (animations.count(id))
	{
		// If this animation is already set as current, return.
		if (currentAnimation == &animations[id])
			return;
		
		bool wasAnimation = currentAnimation ? true : false;
		
		currentAnimation = &animations[id];
		
		meshNode->setFrameLoop(currentAnimation->start, currentAnimation->end);
		meshNode->setAnimationSpeed(currentAnimation->fps);
		meshNode->setLoopMode(currentAnimation->looped);
		
		if (wasAnimation)
			meshNode->setTransitionTime(currentAnimation->transitionTime);
		else // Don't need a transition with the first animation set.
			meshNode->setTransitionTime(0.f);
		
		// This is required for some reason.
		// Ensures transition is applied correctly.
		meshNode->setCurrentFrame(currentAnimation->start);
		
		//onFirstFrame = true;
	}
}

bool Mesh::IsPlayingAnimation(s32 id)
{
	if (!currentAnimation)
		return false;
	
	if (animations.count(id) && currentAnimation == &animations[id])
	{
		return true;
	}
	
	return false;
}

void Mesh::HaltAnimation()
{
	currentAnimation = NULL;
}

scene::IAnimatedMeshSceneNode *Mesh::GetIrrlichtNode()
{
	return meshNode;
}

void Mesh::Scale(const core::vector3df &scale)
{
	meshNode->setScale(scale);
	
	// If scaled then normalize normals
	meshNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,
			!(scale.X == 1.0 && scale.Y == 1.0 && scale.Z == 1.0)
			);
}

void Mesh::Rotate(const core::vector3df &rotation)
{
	// this is accounted for in ReceiveRenderRotation
	this->rotation = rotation;
}

void Mesh::Translate(const core::vector3df &translation)
{
	this->translation = translation;
}


