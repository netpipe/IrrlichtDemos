#ifndef CPARTICLE_CPP
#define CPARTICLE_CPP
// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in Irrlicht.h

#include "CParticleSystem2SceneNode.h"

namespace irr
{
namespace scene
{

//! constructor
CParticleSystem2SceneNode::CParticleSystem2SceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
	const core::vector3df& position, const core::vector3df& rotation,
	const core::vector3df& scale)
	: ISceneNode(parent, mgr, id, position, rotation, scale)
{
	#ifdef _DEBUG
	setDebugName("CParticleSystem2SceneNode");
	#endif
    Particles.set_used(0);

	setParticleSize();
}



//! destructor
CParticleSystem2SceneNode::~CParticleSystem2SceneNode()
{
}


//! Returns the material based on the zero based index i.
video::SMaterial& CParticleSystem2SceneNode::getMaterial(s32 i)
{
	return Material;
}



//! Returns amount of materials used by this scene node.
s32 CParticleSystem2SceneNode::getMaterialCount()
{
	return 1;
}


//! pre render event
void CParticleSystem2SceneNode::OnPreRender()
{
	if (IsVisible)
	{
		if (Particles.size() != 0)
		{
			SceneManager->registerNodeForRendering(this);
		//	scene::ISceneNode::OnPreRender();
		}
	}
}


void CParticleSystem2SceneNode::OnPostRender(u32 t)
{
    doParticleSystem(t);
}

//! render
void CParticleSystem2SceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	// calculate vectors for letting particles look to camera
	core::vector3df campos = camera->getAbsolutePosition();
	core::vector3df target = camera->getTarget();
	core::vector3df up = camera->getUpVector();
	core::vector3df view = target - campos;
	view.normalize();

	core::vector3df horizontal = up.crossProduct(view);
	horizontal.normalize();

	core::vector3df vertical = horizontal.crossProduct(view);
	vertical.normalize();

	horizontal *= 0.5f * ParticleSize.Width;
	vertical *= 0.5f * ParticleSize.Height;

	view *= -1.0f;

	// reallocate arrays, if they are too small
	reallocateBuffers();

	// create particle vertex data
	for (u32 i=0; i<Particles.size(); ++i)
	{
		SParticle2& particle = Particles[i];

		s32 idx = i*4;

		Vertices[0+idx].Pos = particle.pos + horizontal + vertical;
		Vertices[0+idx].Color = particle.color;
		Vertices[0+idx].Normal = view;

		Vertices[1+idx].Pos = particle.pos + horizontal - vertical;
		Vertices[1+idx].Color = particle.color;
		Vertices[1+idx].Normal = view;

		Vertices[2+idx].Pos = particle.pos - horizontal - vertical;
		Vertices[2+idx].Color = particle.color;
		Vertices[2+idx].Normal = view;

		Vertices[3+idx].Pos = particle.pos - horizontal + vertical;
		Vertices[3+idx].Color = particle.color;
		Vertices[3+idx].Normal = view;
	}

	// render all
	if (ParticlesAreGlobal)
	{
		core::matrix4 mat;
		driver->setTransform(video::ETS_WORLD, mat);
	}
	else
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);


	driver->setMaterial(Material);

	driver->drawIndexedTriangleList(Vertices.pointer(), Particles.size()*4,
		Indices.pointer(), Particles.size()*2);

	// for debug purposes only:
	if (DebugDataVisible)
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(Box, video::SColor(0,255,255,255));
	}
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CParticleSystem2SceneNode::getBoundingBox() const
{
	return Box;
}



void CParticleSystem2SceneNode::doParticleSystem(u32 time)
{
    /*
	u32 now = device->getTimer()->getTime();
	u32 timediff = now - lastAnimTime;
	lastAnimTime = now;

	// run emitter

///	core::list<IParticleEmitter*>::Iterator it = EmitterList.begin();
	while (it != EmitterList.end())
	{
		SParticle2* array = 0;
		s32 newParticles = (*it)->emitt(now, timediff, array);

		if (newParticles && array)
			for (s32 i=0; i<newParticles && Particles.size() < 16250; ++i)
			{
				AbsoluteTransformation.rotateVect(array[i].startVector);

				if (ParticlesAreGlobal)
					AbsoluteTransformation.transformVect(array[i].pos);

				Particles.push_back(array[i]);
			}
	}
//
	if (ParticlesAreGlobal)
		Box.reset(AbsoluteTransformation.getTranslation());
	else
		Box.reset(core::vector3df(0,0,0));

	// animate all particles
	f32 scale = (f32)timediff;

	for (s32 i=0; i<(s32)Particles.size();)
	{

		if (now > Particles[i].endTime)
		{
			Particles.erase(i);
        }
		else
		{
			Particles[i].pos += (Particles[i].vector * scale);
			Box.addInternalPoint(Particles[i].pos);

			// fade out

            if (Particles[i].endTime - now < 50)
            {
                f32 d = (Particles[i].endTime - now) / 50.0f;
                Particles[i].color = Particles[i].startColor.getInterpolated(video::SColor(1,1,1,1), d);
            }

			++i;
		}
	}

	f32 m = ParticleSize.Width > ParticleSize.Height ? ParticleSize.Width : ParticleSize.Height;
	m *= 0.5f;
	Box.MaxEdge.X += m;
	Box.MaxEdge.Y += m;
	Box.MaxEdge.Z += m;

	Box.MinEdge.X -= m;
	Box.MinEdge.Y -= m;
	Box.MinEdge.Z -= m;

	if (ParticlesAreGlobal)
	{
		core::matrix4 absinv = AbsoluteTransformation;
		absinv.makeInverse();
		absinv.transformBox(Box);
	}
	*/
}


//! Sets if the particles should be global. If it is, the particles are affected by
//! the movement of the particle system scene node too, otherwise they completely
//! ignore it. Default is true.
void CParticleSystem2SceneNode::setParticlesAreGlobal(bool global)
{
	ParticlesAreGlobal = global;
}



//! Sets the size of all particles.
void CParticleSystem2SceneNode::setParticleSize(const core::dimension2d<f32> &size)
{
	ParticleSize = size;
}


void CParticleSystem2SceneNode::reallocateBuffers()
{
	if (Particles.size() * 4 > Vertices.size() ||
        Particles.size() * 6 > Indices.size())
	{
		s32 oldSize = Vertices.size();
		Vertices.set_used(Particles.size() * 4);

		u32 i;

		// fill remaining vertices
		for (i=oldSize; i<Vertices.size(); i+=4)
		{
			Vertices[0+i].TCoords.set(0.0f, 0.0f);
			Vertices[1+i].TCoords.set(0.0f, 1.0f);
			Vertices[2+i].TCoords.set(1.0f, 1.0f);
			Vertices[3+i].TCoords.set(1.0f, 0.0f);
		}

		// fill remaining indices
		s32 oldIdxSize = Indices.size();
		s32 oldvertices = oldSize;
		Indices.set_used(Particles.size() * 6);

		for (i=oldIdxSize; i<Indices.size(); i+=6)
		{
			Indices[0+i] = 0+oldvertices;
			Indices[1+i] = 2+oldvertices;
			Indices[2+i] = 1+oldvertices;
			Indices[3+i] = 0+oldvertices;
			Indices[4+i] = 3+oldvertices;
			Indices[5+i] = 2+oldvertices;
			oldvertices += 4;
		}
	}
}


} // end namespace scene
} // end namespace irr
#endif
