// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include <irrlicht.h>
using namespace irr;

namespace irr
{
namespace scene
{



	//! Struct for holding particle data
	struct SParticle2
	{
		//! Position of the particle
		core::vector3df pos;

		//! Direction and speed of the particle
		core::vector3df vector;

		//! Start life time of the particle
		u32 startTime;

		//! End life time of the particle
		u32 endTime;

		//! Current color of the particle
		video::SColor color;

		//! Original color of the particle. That's the color
		//! of the particle it had when it was emitted.
		video::SColor startColor;

		//! Original direction and speed of the particle,
		//! the direction and speed the particle had when
		//! it was emitted.
		core::vector3df startVector;
	};


//! A particle system scene node.
/** A scene node controlling a particle System. The behavior of the particles
can be controlling by setting the right particle emitters and effectors.
*/
class CParticleSystem2SceneNode : public ISceneNode
{
public:

	//! constructor
	CParticleSystem2SceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position,
		const core::vector3df& rotation,
		const core::vector3df& scale);

	//! destructor
	virtual ~CParticleSystem2SceneNode();

	//! Returns the material based on the zero based index i.
	virtual video::SMaterial& getMaterial(s32 i);

	//! Returns amount of materials used by this scene node.
	virtual s32 getMaterialCount();

	//! pre render event
	virtual void OnPreRender();

	//! render
	virtual void render();

	//! post render event
	virtual void OnPostRender(u32 t);

	//! returns the axis aligned bounding box of this node
	virtual const core::aabbox3d<f32>& getBoundingBox() const;


	//! Sets the size of all particles.
	virtual void setParticleSize(
		const core::dimension2d<f32> &size = core::dimension2d<f32>(5.0f, 5.0f));

	//! Sets if the particles should be global. If it is, the particles are affected by
	//! the movement of the particle system scene node too, otherwise they completely
	//! ignore it. Default is true.
	virtual void setParticlesAreGlobal(bool global);


	core::array<scene::SParticle2> Particles;

private:

	void doParticleSystem(u32 time);
	void reallocateBuffers();

	video::SMaterial Material;
	core::aabbox3d<f32> Box;
	u32 lastAnimTime;
	core::dimension2d<f32> ParticleSize;

	core::array<video::S3DVertex> Vertices; // 4 vertices per particle
	core::array<u16> Indices;               // 6 indices per particle
	bool ParticlesAreGlobal;
};


} // end namespace scene
} // end namespace irr

